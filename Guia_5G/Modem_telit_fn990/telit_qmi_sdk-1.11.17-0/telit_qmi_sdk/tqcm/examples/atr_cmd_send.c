#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_atr.h>
#include <tqcm.h>
#include <tqcm_atr.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

static sem_t mutex_indication;
static bool cmd_pending = true;

static void cb(void *item)
{
    int ret;
    atr_cmd_t out;
    indication_item_t *ind = (indication_item_t *)item;
    static char full_reply[8192] = {0};
    static uint16_t full_reply_len = 0;

    ret = telit_atr_indication_handler(ind->msg_id,
            ind->buf, ind->buf_len,
            (uint8_t *)&out, sizeof(out));
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("unpack fail %d ", ret);
    }

    switch (ind->msg_id) {
        case QMI_ATR_CMD_IND:
            if (cmd_pending) {
                if (strstr(out.cmd, "OK")) {
                    strcat(full_reply, out.cmd);
                    RLOGI("AT command answer:\n%s", full_reply);
                    cmd_pending = false;
                    memset(full_reply, '\0', sizeof(full_reply));
                    sem_post(&mutex_indication);
                } else {
                    strcat(full_reply, out.cmd);
                    full_reply_len += out.cmd_len;
                }
            } else {
                RLOGI("Unsolicited:\n%s", out.cmd);
            }
            break;
        default:
            RLOGI("Indication not managed");
            break;
    }

    free(ind->buf);
    free(ind);
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_atr;
    atr_cmd_t input = {0};
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

    ret = tqcm_init_client_context(QMI_SVC_ATR,
            &ctx,
            &ctx_atr,
            cb);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_atr.client_id);

    strcpy(input.cmd, "AT+CREG=2\r");
    input.cmd_len = strlen(input.cmd);
    cmd_pending = true;
    ret = tqcm_atr_cmd_send_send(&ctx_atr,
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

    if (sem_wait(&mutex_indication)) {
        RLOGE("Failed sem_wait");
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    memset(input.cmd, '\0', sizeof(input.cmd));
    strcpy(input.cmd, "AT+CFUN=4\r");
    input.cmd_len = strlen(input.cmd);
    cmd_pending = true;
    ret = tqcm_atr_cmd_send_send(&ctx_atr,
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

    if (sem_wait(&mutex_indication)) {
        RLOGE("Failed sem_wait");
        ret = EXIT_FAILURE;
        goto context_disposal;
    }
    sleep(5);

    memset(input.cmd, '\0', sizeof(input.cmd));
    strcpy(input.cmd, "AT+CFUN=1\r");
    input.cmd_len = strlen(input.cmd);
    cmd_pending = true;
    ret = tqcm_atr_cmd_send_send(&ctx_atr,
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

    if (sem_wait(&mutex_indication)) {
        RLOGE("Failed sem_wait");
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    sleep(20);

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_atr) != 0) {
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
