#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_loc.h>
#include <tqcm.h>
#include <tqcm_loc.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;
static sem_t mutex_indication;

static void cb(void *item)
{
    int ret;
    loc_status_ind_t out;
    indication_item_t *ind = (indication_item_t *)item;

    ret = telit_loc_indication_handler(ind->msg_id,
            ind->buf, ind->buf_len,
            (uint8_t *)&out, sizeof(out));
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("unpack fail %d ", ret);
    }

    switch (ind->msg_id) {
        case QMI_LOC_DELETE_ASSIST_DATA_IND:
            RLOGI("status %d", out.status);
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
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_loc;
    uint16_t sdk_error;
    loc_start_t input;

    memset(&input, 0, sizeof(input));

    input.session_id = 1;
    input.recurrence_available = true;
    input.recurrence = 1;

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

    ret = tqcm_init_client_context(QMI_SVC_LOC, &ctx, &ctx_loc, cb);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_loc.client_id);

    ret = tqcm_loc_start_send(&ctx_loc, &input, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    } else {
        RLOGI("Success");
    }

    if (sem_wait(&mutex_indication)) {
        RLOGE("Failed sem_wait");
        ret = EXIT_FAILURE;
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_loc) != 0) {
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
