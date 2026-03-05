#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_wda.h>
#include <tqcm.h>
#include <tqcm_wda.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_wda;
    wda_data_format_t input;
    wda_data_format_t out;
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

    ret = tqcm_init_client_context(QMI_SVC_WDA,
            &ctx,
            &ctx_wda,
            NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_wda.client_id);

    input.ul_data_aggregation_protocol_available = true;
    input.ul_data_aggregation_protocol = QMI_WDA_DATA_AGG_QMAP;
    input.dl_data_aggregation_protocol_available = true;
    input.dl_data_aggregation_protocol = QMI_WDA_DATA_AGG_QMAP;
    input.link_prot_available = true;
    input.link_prot = QMI_WDA_LINK_PROTOCOL_IP;
    input.peripheral_endpoint_available = true;
    input.peripheral_endpoint.ep_type = QMI_WDA_DATA_EP_TYPE_HSUSB;
    input.peripheral_endpoint.iface_id = 2;

    ret = tqcm_wda_set_data_format_send(&ctx_wda,
            &input,
            &out,
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

    if (out.link_prot_available) {
        RLOGI("link_prot %d", out.link_prot);
    }

    if (out.ul_data_aggregation_protocol_available) {
        RLOGI("ul_data_aggregation_protocol %d", out.ul_data_aggregation_protocol);
    }

    if (out.dl_data_aggregation_protocol_available) {
        RLOGI("dl_data_aggregation_protocol %d", out.dl_data_aggregation_protocol);
    }

    if (out.dl_data_aggregation_max_datagrams_available) {
        RLOGI("dl_data_aggregation_max_datagrams %d", out.dl_data_aggregation_max_datagrams);
    }

    if (out.dl_data_aggregation_max_size_available) {
        RLOGI("dl_data_aggregation_max_size %d", out.dl_data_aggregation_max_size);
    }

    if (out.ul_data_aggregation_max_datagrams) {
        RLOGI("ul_data_aggregation_max_datagrams %d", out.ul_data_aggregation_max_datagrams);
    }

    if (out.ul_data_aggregation_max_size_available) {
        RLOGI("ul_data_aggregation_max_size %d", out.ul_data_aggregation_max_size);
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_wda) != 0) {
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
