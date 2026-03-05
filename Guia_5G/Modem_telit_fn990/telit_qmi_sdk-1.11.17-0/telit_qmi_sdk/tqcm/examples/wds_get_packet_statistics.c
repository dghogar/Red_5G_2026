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
    wds_get_packet_statistics_mask_t input = { 0 };
    wds_get_packet_statistics_t out = { 0 };

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

    ret = tqcm_init_client_context(QMI_SVC_WDS, &ctx, &ctx_wds, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_wds.client_id);

    input.stats_mask = (0x00000001 |
            0x00000002 |
            0x00000004 |
            0x00000008 |
            0x00000010 |
            0x00000020 |
            0x00000040 |
            0x00000080 |
            0x00000100 |
            0x00000200);

    ret = tqcm_wds_get_pkt_statistics_send(&ctx_wds, &input, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    if (out.tx_out_count_available) {
        RLOGI("wds tx_out_count: %u", out.tx_out_count);
    }
    if (out.rx_out_count_available) {
        RLOGI("wds rx_out_count: %u", out.rx_out_count);
    }
    if (out.tx_err_count_available) {
        RLOGI("wds tx_err_count: %u", out.tx_err_count);
    }
    if (out.rx_err_count_available) {
        RLOGI("wds rx_err_count: %u", out.rx_err_count);
    }
    if (out.tx_ofl_count_available) {
        RLOGI("wds tx_ofl_count: %u", out.tx_ofl_count);
    }
    if (out.rx_ofl_count_available) {
        RLOGI("wds rx_ofl_count: %u", out.rx_ofl_count);
    }
    if (out.tx_ok_bytes_count_available) {
        RLOGI("wds tx_ok_bytes_count: %u", out.tx_ok_bytes_count);
    }
    if (out.rx_ok_bytes_count_available) {
        RLOGI("wds rx_ok_bytes_count: %u", out.rx_ok_bytes_count);
    }
    if (out.last_call_tx_ok_bytes_count_available) {
        RLOGI("wds last_call_tx_ok_bytes_count: %u", out.last_call_tx_ok_bytes_count);
    }
    if (out.last_call_rx_ok_bytes_count_available) {
        RLOGI("wds last_call_rx_ok_bytes_count: %u", out.last_call_rx_ok_bytes_count);
    }
    if (out.tx_dropped_count_available) {
        RLOGI("wds tx_dropped_count: %u", out.tx_dropped_count);
    }
    if (out.rx_dropped_count_available) {
        RLOGI("wds rx_dropped_count: %u", out.rx_dropped_count);
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
