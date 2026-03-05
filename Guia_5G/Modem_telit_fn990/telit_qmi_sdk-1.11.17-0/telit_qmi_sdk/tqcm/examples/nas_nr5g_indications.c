#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <telit_sdk.h>
#include <telit_sdk_nas.h>
#include <telit_sdk_test_helpers.h>
#include <tqcm.h>
#include <tqcm_nas.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;
static sem_t stop_sem;

static char lost_reasons[][64] = {
    "QMI_NAS_NR5G_LOST_FRAME_SYNC_RLF",
    "QMI_NAS_NR5G_LOST_FRAME_SYNC_HANDOVER",
    "QMI_NAS_NR5G_LOST_FRAME_SYNC_RESELECTION",
    "QMI_NAS_NR5G_LOST_FRAME_SYNC_OOS",
    "QMI_NAS_NR5G_LOST_FRAME_SYNC_STALE_SIB9",
    "QMI_NAS_NR5G_LOST_FRAME_SYNC_NO_SIB9"
};

static void ctrl_c_handler(int sig __unused)
{
    sem_post(&stop_sem);
}

static void cb(void *item)
{
    indication_item_t *ind = (indication_item_t *)item;
    uint8_t indication_buf[2048] = {0};
    int ret;

    ret = telit_nas_indication_handler(ind->msg_id,
            ind->buf, ind->buf_len,
            indication_buf, sizeof(indication_buf));
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("unpack fail %d ", ret);
    } else {
        switch (ind->msg_id) {
            case QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND: {
                nas_nr5g_time_sync_pulse_report_t *t = (nas_nr5g_time_sync_pulse_report_t *)indication_buf;

                if (t->gps_time_available) {
                    RLOGI("gps_time = %lu", t->gps_time);
                }
                if (t->sfn_available) {
                    RLOGI("sfn = %u", t->sfn);
                }
                if (t->nta_available) {
                    RLOGI("nta = %u", t->nta);
                }
                if (t->nta_offset_available) {
                    RLOGI("nta_offset = %u", t->nta_offset);
                }
                if (t->leapseconds_available) {
                    RLOGI("leapseconds = %u", t->leapseconds);
                }
                if (t->utc_time_available) {
                    RLOGI("utc_time = %lu", t->utc_time);
                }
                break;
            }
            case QMI_NAS_NR5G_LOST_FRAME_SYNC_IND: {
                nas_nr5g_lost_frame_sync_t *t = (nas_nr5g_lost_frame_sync_t *)indication_buf;

                if (t->nr5g_sync_lost_reason_available) {
                    RLOGI("nr5g_sync_lost_reason: %s", lost_reasons[t->nr5g_sync_lost_reason]);
                }
                break;
            }
            case QMI_NAS_NR5G_RRC_UTC_TIME_UPDATE_IND:
                print_nas_generic_nr5g_rrc_utc_time((nas_nr5g_rrc_utc_time_t *) indication_buf);
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

static void collect_user_input(nas_set_nr5g_sync_pulse_gen_t *input)
{
    fprintf(stderr, "After having set the input, press ENTER to stop\n");
    fprintf(stderr, "\nEnter pulse period (range: 0 - 128, in multiple of 10 milliseconds: ");
    if (scanf("%u", &input->pulse_period) <= 0)
        RLOGE("Unable to get value");
    fprintf(stderr, "\nEnter system frame number (range: 0 - 1024, 1024 = next available sfn): ");
    if (scanf("%u", &input->start_sfn) <= 0)
        RLOGE("Unable to get value");
    input->start_sfn_available = true;
    fprintf(stderr,
            "\nEnter pulse generation indication periodicity (range: 0 - 128, in multiple of 10 milliseconds, 0 = disabled): ");
    if (scanf("%u", &input->report_period) <= 0)
        RLOGE("Unable to get value");
    input->report_period_available = true;
}

int main(int argc, char **argv)
{
    nas_set_nr5g_sync_pulse_gen_t user_input = {0};
    nas_indication_register_t input = {0};
    tqcm_client_context_t ctx_nas;
    int ret = EXIT_SUCCESS;
    struct sigaction act;
    uint16_t sdk_error;

    telit_set_log_level(LOG_INFO);
    act.sa_handler = ctrl_c_handler;
    sigaction(SIGINT, &act, NULL);

    if (argc == 1) {
        dev = default_rmnet_dev;
    } else {
        dev = argv[1];
    }

    if (sem_init(&stop_sem, 0, 0)) {
        RLOGE("sem_init failed");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_init(dev, &ctx);
    if (ret != 0) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_init_client_context(QMI_SVC_NAS, &ctx, &ctx_nas, cb);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    collect_user_input(&user_input);

    ret = tqcm_nas_set_nr5g_sync_pulse_gen_send(&ctx_nas,
            &user_input, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }
    input.reg_nr5g_time_sync_pulse_report = QMI_IND_ACTION_SET;
    input.reg_nr5g_lost_sync_frame = QMI_IND_ACTION_SET;
    input.reg_nr5g_utc_time_update = QMI_IND_ACTION_SET;

    ret = tqcm_nas_indication_register_send(&ctx_nas, &input, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    sem_wait(&stop_sem);

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_nas) != 0) {
        RLOGE("Failed context disposal");
        ret = EXIT_FAILURE;
    }

    if (tqcm_deinit(&ctx) != 0) {
        RLOGE("Failed tqcm deinitialization");
        ret = EXIT_FAILURE;
        goto end;
    }
    sem_destroy(&stop_sem);

    /* Needed for valgrind complain about thread stack memory */
    sleep(1);

end:
    return ret;
}
