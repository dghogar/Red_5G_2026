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
    nas_signal_strength_t out;
    uint16_t sdk_error;
    int i = 0;

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

    ret = tqcm_nas_get_signal_strength_send(&ctx_nas, NULL, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    for (i = 0; i < out.signal_strength_list_size; i++) {
        RLOGI("strength[%d] %d, ", i, out.signal_strength_list[i].signal_strength);
        RLOGI("radio_if[%d] %u ", i, out.signal_strength_list[i].radio_if);
    }
    for (i = 0; i < out.ecio_list_size; i++) {
        RLOGI("ecio[%d] %u, ", i, out.ecio_list[i].ecio);
        RLOGI("radio_if[%d] %u ", i, out.ecio_list[i].radio_if);
    }
    for (i = 0; i < out.error_rate_list_size; i++) {
        RLOGI("error_rate[%d] %d, ", i, out.error_rate_list[i].error_rate);
        RLOGI("radio_if[%d] %u ", i, out.error_rate_list[i].radio_if);
    }
    if (out.io_available) {
        RLOGI("io %u ", out.io);
    }
    if (out.sinr_available) {
        RLOGI("sinr %d ", out.sinr);
    }
    if (out.rsrq_info_available) {
        RLOGI("rsrq_info.radio_if %d ", out.rsrq_info.radio_if);
        RLOGI("rsrq_info.rsrq %d ", out.rsrq_info.rsrq);
    }
    if (out.lte_snr_available) {
        RLOGI("io %d ", out.lte_snr);
    }
    if (out.lte_rsrp_available) {
        RLOGI("io %d ", out.lte_rsrp);
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
