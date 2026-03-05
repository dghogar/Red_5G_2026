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
    nas_radio_if_t input;
    nas_tx_rx_info_t out;
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

    input.radio_if = QMI_NAS_RADIO_INTERFACE_LTE;

    ret = tqcm_nas_get_tx_rx_info_send(&ctx_nas, &input, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    if (out.rx_chain_0_info_available) {
        RLOGI("Rx chain 0 info, is_radio_tuned %d", out.rx_chain_0_info.is_radio_tuned);
        RLOGI("Rx chain 0 info, rx_pwr %d", out.rx_chain_0_info.rx_pwr);
        RLOGI("Rx chain 0 info, ecio %d", out.rx_chain_0_info.ecio);
        RLOGI("Rx chain 0 info, rscp %d", out.rx_chain_0_info.rscp);
        RLOGI("Rx chain 0 info, rsrp %d", out.rx_chain_0_info.rsrp);
        RLOGI("Rx chain 0 info, phase %d", out.rx_chain_0_info.phase);
    }

    if (out.rx_chain_1_info_available) {
        RLOGI("Rx chain 1 info, is_radio_tuned %d", out.rx_chain_1_info.is_radio_tuned);
        RLOGI("Rx chain 1 info, rx_pwr %d", out.rx_chain_1_info.rx_pwr);
        RLOGI("Rx chain 1 info, ecio %d", out.rx_chain_1_info.ecio);
        RLOGI("Rx chain 1 info, rscp %d", out.rx_chain_1_info.rscp);
        RLOGI("Rx chain 1 info, rsrp %d", out.rx_chain_1_info.rsrp);
        RLOGI("Rx chain 1 info, phase %d", out.rx_chain_1_info.phase);
    }

    if (out.tx_info_available) {
        RLOGI("Tx info, is_in_traffic %d", out.tx_info.is_in_traffic);
        RLOGI("Tx info, tx_pwr %d", out.tx_info.tx_pwr);
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
