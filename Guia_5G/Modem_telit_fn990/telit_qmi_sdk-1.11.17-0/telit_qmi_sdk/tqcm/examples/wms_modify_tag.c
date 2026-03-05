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
    uint16_t sdk_error;
    wms_modify_tag_t input = { 0 };

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

    ret = tqcm_init_client_context(QMI_SVC_WMS, &ctx, &ctx_wms, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_wms.client_id);


    /* fill in wms_modify_tag_t info (adapt inputs accordin to the actual message list */
    input.message_tag.storage_type = QMI_WMS_STORAGE_UIM;;
    input.message_tag.storage_index = 3;
    input.message_tag.tag_type = QMI_WMS_TAG_TYPE_MO_SENT;
    input.message_mode = QMI_WMS_MESSAGE_MODE_GW;

    ret = tqcm_wms_modify_tag_send(&ctx_wms, &input, &sdk_error, NULL, 0);

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
