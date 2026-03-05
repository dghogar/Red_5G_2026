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
static sem_t sem_indication;

static void cb(void *item)
{
    int ret;
    indication_item_t *ind = (indication_item_t *)item;

    switch (ind->msg_id) {
        case QMI_LOC_INJECT_UTC_TIME_IND: {
            loc_status_ind_t out;

            ret = telit_loc_indication_handler(ind->msg_id,
                    ind->buf, ind->buf_len,
                    (uint8_t *)&out, sizeof(out));
            if (ret == QMI_SDK_ERR_NONE) {
                if (out.status == QMI_LOC_SUCCESS) {
                    RLOGI("Status of request 0x%02X: Success", ind->msg_id);
                } else {
                    RLOGI("Status of request 0x%02X: %d", ind->msg_id, out.status);
                }
            } else {
                RLOGE("unpack fail %d ", ret);
            }
            sem_post(&sem_indication);
            break;
        }
        default:
            RLOGW("Indication not managed");
            break;
    }

    free(ind->buf);
    free(ind);
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_loc;
    uint16_t sdk_error;
    loc_utc_time_t input;
    loc_start_t input_start;

    if (sem_init(&sem_indication, 0, 0)) {
        RLOGE("%s", strerror(errno));
        ret = EXIT_FAILURE;
        goto end;
    }

    memset(&input_start, 0, sizeof(input_start));

    input_start.session_id = 1;
    input_start.recurrence_available = true;
    input_start.recurrence = 1;

    memset(&input, 0, sizeof(input));

    input.time_utc = 1703846334577;
    input.time_unc = 1000;

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

    ret = tqcm_loc_start_send(&ctx_loc, &input_start, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    ret = tqcm_loc_inject_utc_time_send(&ctx_loc, &input, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    if (sem_wait(&sem_indication)) {
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
