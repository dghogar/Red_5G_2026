#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <netinet/in.h>

#include <telit_sdk.h>
#include <telit_sdk_wds.h>
#include <telit_sdk_dms.h>
#include <tqcm.h>
#include <tqcm_wds.h>
#include <tqcm_dms.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

#define APN "phone"
#define IP_TYPE QMI_WDS_PDP_TYPE_IPV4

static bool set_operating_mode(tqcm_client_context_t *ctx, uint8_t operating_mode)
{
    dms_power_info_t input_power = { 0 };
    uint16_t sdk_error;
    int ret;

    input_power.power_mode = operating_mode;
    ret = tqcm_dms_set_operating_mode_send(ctx,
            &input_power,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("dms_set_operating_mode_send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        return false;
    }

    return true;
}

static bool is_profile_matched(tqcm_client_context_t *ctx, uint16_t profile_id,
        char *apn, uint8_t pdp_type)
{
    wds_settings_profile_id_t input_profile = { 0 };
    wds_profile_t out_profile = { 0 };
    uint16_t sdk_error;
    int ret;

    input_profile.type = QMI_WDS_PROFILE_TYPE_3GPP;
    input_profile.index = profile_id;

    ret = tqcm_wds_get_profile_settings_send(ctx, &input_profile, &out_profile, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("wds_get_profile_settings_send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }

        if (out_profile.extended_error_code_available) {
            RLOGI("Extended error code 0x%x ", out_profile.extended_error_code.extended_error_code);
        }

        return false;
    }

    if (!out_profile.profile_settings.pdp_type_available || out_profile.profile_settings.apn_len == 0) {
        return false;
    }

    if (out_profile.profile_settings.pdp_type != pdp_type) {
        return false;
    }

    if (strcmp(out_profile.profile_settings.apn, apn) != 0) {
        return false;
    }

    return true;
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_wds;
    tqcm_client_context_t ctx_dms;
    uint16_t sdk_error;
    wds_default_profile_input_t default_profile_input = {
        QMI_WDS_PROFILE_TYPE_3GPP,
        QMI_WDS_PROFILE_FAMILY_EMBEDDED
    };
    wds_default_profile_out_t default_profile_out;
    wds_profile_list_input_t input_profile_list = { 0 };
    wds_get_attach_pdn_list_t out_profile_list;
    wds_profile_list_t out = { 0 };
    wds_attach_pdn_list_t input = { 0 };
    int profile_id = -1;

    telit_set_log_level(LOG_INFO);

    if (argc == 1) {
        dev = default_rmnet_dev;
    } else {
        dev = argv[1];
    }

    ret = tqcm_init(dev, &ctx);
    if (ret != 0) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_init_client_context(QMI_SVC_DMS,
            &ctx,
            &ctx_dms,
            NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }
    ret = tqcm_init_client_context(QMI_SVC_WDS, &ctx, &ctx_wds, NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_wds_get_default_profile_num_send(&ctx_wds,
            &default_profile_input,
            &default_profile_out,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("wds_get_default_profile_num_send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        /* Continue anyway */
    } else {
        RLOGI("Default profile number %u", default_profile_out.profile_index);
    }

    /* Check first that the modem does not already have the required PDN attach list */
    RLOGI("Looking in the current pdn attach list for the requested profile");
    ret = tqcm_wds_get_lte_attach_pdn_list_send(&ctx_wds,
            &out_profile_list,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("wds_get_lte_attach_pdn_list_send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto end;
    }
    if (out_profile_list.attach_pdn_list_size != 0) {
        bool check_radio_cycle_required = false;
        bool radio_cycle_required = false;

        for (int i = 0; i < out_profile_list.attach_pdn_list_size; i++) {
            if (is_profile_matched(&ctx_wds, out_profile_list.attach_pdn_list[i], APN, IP_TYPE)) {
                RLOGI("Found the profile in the current modem pdn attach list");
                check_radio_cycle_required = true;
                break;
            }
        }

        if (check_radio_cycle_required == false) {
            RLOGI("Profile not found in the current pdn attach list");
            goto profile_not_found;
        }

        for (int i = 0; i < out_profile_list.pending_attach_pdn_list_size; i++) {
            if (is_profile_matched(&ctx_wds, out_profile_list.pending_attach_pdn_list[i],
                    APN, IP_TYPE)) {
                radio_cycle_required = true;
                break;
            }
        }

        if (radio_cycle_required) {
            RLOGI("pdn attach profile apply pending, reboot required");
            if (!set_operating_mode(&ctx_dms, QMI_DMS_OP_MODE_LOW_POWER)) {
                ret = EXIT_FAILURE;
                goto context_disposal;
            }
            sleep(1);
            if (!set_operating_mode(&ctx_dms, QMI_DMS_OP_MODE_ONLINE)) {
                ret = EXIT_FAILURE;
                goto context_disposal;
            }
            RLOGI("radio cycle done");
        }

        RLOGI("No further action required");
        goto context_disposal;
    }

profile_not_found:
    if (!set_operating_mode(&ctx_dms, QMI_DMS_OP_MODE_LOW_POWER)) {
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    input_profile_list.profile_type = QMI_WDS_PROFILE_TYPE_3GPP;
    input_profile_list.profile_type_available = true;
    input_profile_list.search_type_available = true;
    input_profile_list.search_type = QMI_WDS_PROFILE_SEARCH_TYPE_APN_NAME;
    input_profile_list.apn_name_len = strlen(APN);
    memcpy(&input_profile_list.apn_name, APN, input_profile_list.apn_name_len);
    ret = tqcm_wds_get_profile_list_send(&ctx_wds, &input_profile_list, &out, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("wds_get_profile_list_send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }

        if (out.extended_error_code_available) {
            RLOGI("Extended error code 0x%x ", out.extended_error_code.extended_error_code);
        }

        ret = EXIT_FAILURE;
        goto online;
    }
    /* Check if the required profile is present */
    for (int i = 0; i < out.profile_list_size; i++) {
        if (is_profile_matched(&ctx_wds, out.profile_list[i].profile_index,
                APN, IP_TYPE)) {
            profile_id = out.profile_list[i].profile_index;
            RLOGI("Profile id %d already available", profile_id);
            break;
        }
    }

    if (profile_id == -1) {
        /* If not, create the profile */
        wds_profile_t input_create_profile = { 0 };
        wds_profile_resp_t out_create = { 0 };

        input_create_profile.profile_id.type = QMI_WDS_PROFILE_TYPE_3GPP;
        input_create_profile.profile_settings.pdp_type_available = true;
        input_create_profile.profile_settings.pdp_type = IP_TYPE;
        input_create_profile.profile_settings.apn_len = strlen(APN);
        strlcpy(input_create_profile.profile_settings.apn,
                APN, sizeof(input_create_profile.profile_settings.apn));

        ret = tqcm_wds_create_profile_send(&ctx_wds, &input_create_profile, &out_create, &sdk_error, NULL, 0);
        if (ret != 0) {
            RLOGE("wds_create_profile_send fail %d", ret);
            if (ret == EPROTO) {
                RLOGE("qmi protocol error 0x%04X", sdk_error);
            }
            RLOGE("Extended error %u ", out_create.extended_error_code);
            ret = EXIT_FAILURE;
            goto online;
        }

        if (!out_create.profile_id_available) {
            RLOGE("Profile id not found");
            ret = EXIT_FAILURE;
            goto online;
        }

        profile_id = out_create.profile_id.index;
        RLOGI("New profile created %d", profile_id);
    }

    input.attach_pdn_list_size = 1;
    input.attach_pdn_list[0] = profile_id;
    input.action_available = true;
    input.action = QMI_WDS_ATTACH_PDN_LIST_ACTION_DETACH_OR_PDN_DISCONNECT;

    ret = tqcm_wds_set_lte_attach_pdn_list_send(&ctx_wds, &input, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("wds_set_lte_attach_pdn_list_send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
    }

online:
    if (!set_operating_mode(&ctx_dms, QMI_DMS_OP_MODE_ONLINE)) {
        ret = EXIT_FAILURE;
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_wds) != 0) {
        RLOGE("Failed context disposal");
        ret = EXIT_FAILURE;
    }
    if (tqcm_deinit_client_context(&ctx, &ctx_dms) != 0) {
        RLOGE("Failed context disposal");
        ret = EXIT_FAILURE;
    }

    if (tqcm_deinit(&ctx) != 0) {
        RLOGE("Failed tqcm deinitialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    /* Needed for valgrind complain about thread stack memory */
    sleep(1);

end:
    return ret;
}
