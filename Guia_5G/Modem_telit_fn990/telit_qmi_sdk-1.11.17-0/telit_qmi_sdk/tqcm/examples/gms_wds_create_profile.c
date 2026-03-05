#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <netinet/in.h>

#include <telit_sdk.h>
#include <telit_sdk_gms.h>
#include <tqcm.h>
#include <tqcm_gms.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_gms;
    uint16_t sdk_error;
    wds_profile_t input = { 0 };
    wds_profile_resp_t out = { 0 };
    char *apn;
    uint8_t pdp_type = 0;

    telit_set_log_level(LOG_INFO);

    if (argc < 2) {
        RLOGE("APN should be provided");
        ret = EXIT_FAILURE;
        goto end;
    } else if (argc == 2) {
        dev = default_rmnet_dev;
        apn = argv[1];
    } else if (argc == 3) {
        dev = argv[1];
        apn = argv[2];
        pdp_type = 4;
    } else {
        dev = argv[1];
        apn = argv[2];
        pdp_type = (uint8_t)atoi(argv[3]);
    }

    RLOGI("Creating profile with: device=%s, APN=%s, PDP type=%d",
            dev, apn, pdp_type);

    ret = tqcm_init(dev, &ctx);
    if (ret != 0) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_init_client_context(QMI_SVC_GMS, &ctx, &ctx_gms, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_gms.client_id);

    input.profile_id.type = QMI_WDS_PROFILE_TYPE_3GPP;
    input.profile_settings.pdp_type_available = true;
    input.profile_settings.pdp_type = (pdp_type == 6 ? QMI_WDS_PDP_TYPE_IPV6 : QMI_WDS_PDP_TYPE_IPV4);
    input.profile_settings.apn_len = strlen(apn);
    strlcpy(input.profile_settings.apn, apn, sizeof(input.profile_settings.apn));

    ret = tqcm_gms_wds_create_profile_send(&ctx_gms, &input, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        RLOGE("Extended error %u ", out.extended_error_code);
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    if (out.profile_id_available) {
        RLOGI("Profile type %u, ", out.profile_id.type);
        RLOGI("Profile index %u", out.profile_id.index);
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_gms) != 0) {
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
