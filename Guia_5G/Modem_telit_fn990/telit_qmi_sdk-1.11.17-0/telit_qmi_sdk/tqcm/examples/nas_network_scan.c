#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_nas.h>
#include <tqcm.h>
#include <tqcm_nas.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_nas;
    nas_network_scan_mask_t input;
    nas_network_scan_t out;
    uint16_t sdk_error;

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

    ret = tqcm_init_client_context(QMI_SVC_NAS, &ctx, &ctx_nas, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_nas.client_id);

    input.network_scan_req_mask = QMI_NAS_SCAN_UMTS | QMI_NAS_SCAN_LTE;

    ret = tqcm_nas_perform_network_scan_send(&ctx_nas, &input, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    RLOGI("%d networks found", out.network_3gpp_list_size);

    for (uint8_t i = 0; i < out.network_3gpp_list_size; i++) {
        RLOGI("Network info %d", i);
        RLOGI("\tmobile_country_code: %d", out.network_3gpp_info_list[i].mobile_country_code);
        RLOGI("\tmobile_network_code: %d", out.network_3gpp_info_list[i].mobile_network_code);
        RLOGI("\tin_use_status: %d", out.network_3gpp_info_list[i].in_use_status);
        RLOGI("\troaming_status: %d", out.network_3gpp_info_list[i].roaming_status);
        RLOGI("\tforbidden_status: %d", out.network_3gpp_info_list[i].forbidden_status);
        RLOGI("\tpreferred_status: %d", out.network_3gpp_info_list[i].preferred_status);
        if (out.network_3gpp_info_list[i].description_len) {
            RLOGI("\tdescription: %s", out.network_3gpp_info_list[i].description);
        }
    }

    RLOGI("%d rat info found", out.rat_list_size);
    for (uint8_t i = 0; i < out.rat_list_size; i++) {
        RLOGI("rat info %d", i);
        RLOGI("\tmobile_country_code: %d", out.rat_list[i].MCC);
        RLOGI("\tmobile_network_code: %d", out.rat_list[i].MNC);
        RLOGI("\tRAT: %d", out.rat_list[i].RAT);
    }

    RLOGI("%d pcs info found", out.pcs_digit_list_size);
    for (uint8_t i = 0; i < out.pcs_digit_list_size; i++) {
        RLOGI("pcs info %d", i);
        RLOGI("\tmobile_country_code: %d", out.pcs_digit_list[i].MCC);
        RLOGI("\tmobile_network_code: %d", out.pcs_digit_list[i].MNC);
        RLOGI("\tincludes_pcs_digit: %d", out.pcs_digit_list[i].includes_pcs_digit);
    }

    if (out.scan_result_available) {
        RLOGI("scan_result %d", out.scan_result);
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_nas) != 0) {
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
