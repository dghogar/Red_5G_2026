#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_wms.h>
#include <tqcm.h>
#include <tqcm_wms.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_wms;
    wms_smsc_t input = { 0 };
    uint16_t sdk_error;
    char *number = "+391234567890";

    telit_set_log_level(LOG_INFO);

    RLOGI("Using number \"%s\", please change it", number);

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

    ret = tqcm_init_client_context(QMI_SVC_WMS, &ctx, &ctx_wms, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_wms.client_id);

    input.smsc_address_type_len = strlen("145");
    strcpy(input.smsc_address_type, "145");
    input.smsc_address_len = strlen(number);
    strcpy(input.smsc_address, number);

    ret = tqcm_wms_set_smsc_address_send(&ctx_wms, &input, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_wms) != 0) {
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
