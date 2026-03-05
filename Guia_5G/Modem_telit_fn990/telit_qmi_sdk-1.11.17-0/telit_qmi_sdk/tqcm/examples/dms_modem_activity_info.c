#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_dms.h>
#include <tqcm.h>
#include <tqcm_dms.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

static sem_t mutex_indication;

static void cb(void *item)
{
    indication_item_t *ind = (indication_item_t *)item;
    dms_modem_activity_info_t out;
    int ret;

    ret = telit_dms_indication_handler(ind->msg_id,
            ind->buf, ind->buf_len,
            (uint8_t *)&out, sizeof(out));
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("unpack fail %d ", ret);
    }

    switch (ind->msg_id) {
        case QMI_DMS_MODEM_ACTIVITY_INFO_IND:
            if (out.idle_mode_duration_available) {
                RLOGI("idle_mode_duration %u", out.idle_mode_duration);
            }
            if (out.sleep_mode_duration_available) {
                RLOGI("sleep_mode_duration %u", out.sleep_mode_duration);
            }
            if (out.rx_mode_duration_available) {
                RLOGI("rx_mode_duration %u", out.rx_mode_duration);
            }
            for (int i = 0; i < out.tx_mode_duration_size; i++) {
                RLOGI("tx_mode_duration[%d] tx_mode_level %u", i, out.tx_mode_duration[i].tx_mode_level);
                RLOGI("tx_mode_duration[%d] tx_mode_duration %u", i,
                        out.tx_mode_duration[i].tx_mode_duration);
            }
            break;
        default:
            RLOGI("Indication not managed");
            break;
    }

    free(ind->buf);
    free(ind);

    sem_post(&mutex_indication);
}

int main(int argc, char **argv)
{
    dms_modem_activity_info_calculation_t input = {
        .enable_statistics = 1,
    };
    tqcm_client_context_t ctx_dms;
    int ret = EXIT_SUCCESS;
    uint16_t sdk_error;

    if (sem_init(&mutex_indication, 0, 0)) {
        RLOGE("%s", strerror(errno));
        ret = EXIT_FAILURE;
        goto end;
    }

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

    ret = tqcm_init_client_context(QMI_SVC_DMS,
            &ctx,
            &ctx_dms,
            cb);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_dms.client_id);

    ret = tqcm_dms_trigger_modem_activity_info_calculation_send(&ctx_dms,
            &input,
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

    ret = tqcm_dms_get_modem_activity_info_send(&ctx_dms,
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

    if (sem_wait(&mutex_indication)) {
        RLOGE("Failed sem_wait");
        ret = EXIT_FAILURE;
    }

    input.enable_statistics = 0;
    ret = tqcm_dms_trigger_modem_activity_info_calculation_send(&ctx_dms,
            &input,
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
