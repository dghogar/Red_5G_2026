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

int cb(void *item __unused)
{
    RLOGE("### DEVICE NO MORE WORKING ### ");

    sem_post(&mutex_indication);

    return 0;
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_dms;
    dms_set_event_report_info_t input;
    uint16_t sdk_error;

    if (sem_init(&mutex_indication, 0, 0)) {
        RLOGE("%s", strerror(errno));
        ret = EXIT_FAILURE;
        goto end;
    }

    telit_set_log_level(LOG_DEBUG);
    RLOGI("### CONTEXT CREATION TEST ###");

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

    RLOGI("### INIT DONE ###");

    RLOGI("### SETTING ERROR CB ###");
    ret = tqcm_set_error_cb(&ctx, cb);

    RLOGI("### CONTEXT CREATION ###");

    ret = tqcm_init_client_context(QMI_SVC_DMS,
            &ctx,
            &ctx_dms,
            NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGI("Acquired client id %u", ctx_dms.client_id);

    /* Send a test request in order to check that all is working fine */
    input.operating_mode = QMI_IND_ACTION_SET;
    ret = tqcm_dms_set_event_report_send(&ctx_dms,
            &input,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X, check if retries available", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    /* Wait for error condition (to be triggered externally) */
    if (sem_wait(&mutex_indication)) {
        RLOGE("Failed sem_wait");
        ret = EXIT_FAILURE;
    }

context_disposal:
    RLOGI("### CONTEXT DISPOSAL ###");
    if (tqcm_deinit_client_context(&ctx, &ctx_dms) != 0) {
        RLOGE("Failed context disposal");
        ret = EXIT_FAILURE;
    }

    RLOGI("### STARTING DEINIT ###");
    if (tqcm_deinit(&ctx) != 0) {
        RLOGE("Failed tqcm deinitialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    /* Needed for valgrind complain about thread stack memory */
    sleep(1);
    RLOGI("### DEINIT DONE ###");

end:
    return ret;
}
