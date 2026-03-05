#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <netinet/in.h>

#include <telit_sdk.h>
#include <telit_sdk_wds.h>
#include <tqcm.h>
#include <tqcm_wds.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_wds;
    uint16_t sdk_error;
    wds_settings_profile_id_t input = { 0 };
    wds_profile_ext_err_t out = { 0 };
    uint8_t index;

    telit_set_log_level(LOG_INFO);

    if (argc < 2) {
        RLOGE("Profile index should be provided");
        ret = EXIT_FAILURE;
        goto end;
    } else if (argc == 2) {
        dev = default_rmnet_dev;
        index = (uint8_t)atoi(argv[1]);
    } else {
        dev = argv[1];
        index = (uint8_t)atoi(argv[2]);
    }

    ret = tqcm_init(dev, &ctx);
    if (ret != 0) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_init_client_context(QMI_SVC_WDS, &ctx, &ctx_wds, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_wds.client_id);

    input.type = QMI_WDS_PROFILE_TYPE_3GPP;
    input.index = index;

    RLOGI("Deleting profile %u", index);

    ret = tqcm_wds_delete_profile_send(&ctx_wds, &input, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        RLOGE("extended error code %u", out.extended_error_code);
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_wds) != 0) {
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
