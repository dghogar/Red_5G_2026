#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

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
    gms_dms_get_cust_features_v2_t input = {
        .cust_id = "DGENABLE",
        .cust_id_len = 8,
    };
    gms_dms_get_cust_features_v2_resp_t output = {0};
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

    ret = tqcm_init_client_context(QMI_SVC_GMS,
            &ctx,
            &ctx_gms,
            NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_gms.client_id);

    ret = tqcm_gms_dms_get_cust_features_send(&ctx_gms,
            &input,
            &output,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    RLOGI("cust id: %s", output.cust_id_len ? output.cust_id : "unknown");
    RLOGI("cust value: %s", output.cust_value_len ? (char *)output.cust_value : "uknown");

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
