#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <telit_sdk.h>
#include <telit_sdk_dms.h>
#include <tqcm.h>
#include <tqcm_dms.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

static void print_dev_cap(dms_device_caps_t *out)
{
    uint8_t i;

    RLOGI("DMS tx_rate %u", out->tx_rate);
    RLOGI("DMS rx_rate %u", out->rx_rate);
    for (i = 0; i < out->radio_interfaces_size; i++)
        RLOGI("DMS radio_interfaces[%d] = %u", i, out->radio_interfaces[i]);
    RLOGI("DMS data_svc_caps %u", out->data_svc_caps);
    RLOGI("DMS sim_caps %u", out->sim_caps);
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_dms;
    dms_device_caps_t out;
    uint16_t sdk_error;

    telit_set_log_level(LOG_INFO);
    RLOGI("TQCM version: %s", tqcm_get_version());

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

    RLOGD("Acquired client id %u", ctx_dms.client_id);

    ret = tqcm_dms_get_device_cap_send(&ctx_dms,
            &out,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X, check if retries available", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    print_dev_cap(&out);

context_disposal:
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
