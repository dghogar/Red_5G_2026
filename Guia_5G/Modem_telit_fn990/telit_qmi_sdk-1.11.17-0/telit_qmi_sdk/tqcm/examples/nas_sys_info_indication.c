#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <telit_sdk.h>
#include <telit_sdk_nas.h>
#include <tqcm.h>
#include <tqcm_nas.h>
#include <telit_sdk_test_helpers.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;
static sem_t stop_sem;

static void ctrl_c_handler(int sig __unused)
{
    sem_post(&stop_sem);
}

static void cb(void *item)
{
    indication_item_t *ind = (indication_item_t *) item;
    uint8_t indication_buf[2048] = {0};
    int ret;

    ret = telit_nas_indication_handler(ind->msg_id,
            ind->buf, ind->buf_len,
            indication_buf, sizeof(indication_buf));
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("unpack fail %d ", ret);
    } else {
        switch (ind->msg_id) {
            case QMI_NAS_SYS_INFO_IND:
                RLOGI("Indication QMI_NAS_SYS_INFO_IND received:");
                print_nas_sys_info((nas_system_info_t *) indication_buf);
                break;
            case QMI_NAS_EVENT_REPORT_IND:
                /* Nothing to do, just to avoid spamming the output, since it
                 * is by default enabled */
                break;
            default:
                RLOGI("Indication 0x%x not managed", ind->msg_id);
                break;
        }
    }

    free(ind->buf);
    free(ind);
}

int main(int argc, char **argv)
{
    nas_indication_register_t input = {0};
    tqcm_client_context_t ctx_nas;
    int ret = EXIT_SUCCESS;
    struct sigaction act;
    uint16_t sdk_error;

    telit_set_log_level(LOG_INFO);
    act.sa_handler = ctrl_c_handler;
    sigaction(SIGINT, &act, NULL);

    if (argc == 1)
        dev = default_rmnet_dev;
    else
        dev = argv[1];

    if (sem_init(&stop_sem, 0, 0)) {
        RLOGE("sem_init failed");
        ret = EXIT_FAILURE;
        goto end;
    }

    if (tqcm_init(dev, &ctx)) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto sem_disposal;
    }

    if (tqcm_init_client_context(QMI_SVC_NAS, &ctx, &ctx_nas, cb)) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto tqcm_disposal;
    }

    input.sys_info = QMI_IND_ACTION_SET;
    ret = tqcm_nas_indication_register_send(&ctx_nas, &input, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO)
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    /*
     * To trigger the indication you can for example:
     *   - attach/detach the antenna
     *   - insert/remove the SIM
     *   - issue at+cfun=4 on an AT port channel
     */

    sem_wait(&stop_sem);

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_nas) != 0) {
        RLOGE("Failed context disposal");
        ret = EXIT_FAILURE;
    }

tqcm_disposal:
    if (tqcm_deinit(&ctx) != 0) {
        RLOGE("Failed tqcm deinitialization");
        ret = EXIT_FAILURE;
        goto end;
    }

sem_disposal:
    sem_destroy(&stop_sem);

    /* Needed for valgrind complain about thread stack memory */
    sleep(1);

end:
    return ret;
}
