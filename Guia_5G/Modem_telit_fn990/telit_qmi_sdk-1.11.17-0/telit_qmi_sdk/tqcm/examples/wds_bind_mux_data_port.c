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
    wds_bind_mux_data_port_t input = { 0 };
    uint8_t mux_id;

    telit_set_log_level(LOG_INFO);

    if (argc < 2) {
        RLOGE("MUX ID should be provided");
        ret = EXIT_FAILURE;
        goto end;
    } else if (argc == 2) {
        dev = default_rmnet_dev;
        mux_id = (uint8_t)atoi(argv[1]);
    } else {
        dev = argv[1];
        mux_id = (uint8_t)atoi(argv[2]);
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

    input.mux_id_available = true;
    input.mux_id = mux_id;

    RLOGI("Binding Mux ID: %u", mux_id);

    ret = tqcm_wds_bind_mux_data_port_send(&ctx_wds, &input, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
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
