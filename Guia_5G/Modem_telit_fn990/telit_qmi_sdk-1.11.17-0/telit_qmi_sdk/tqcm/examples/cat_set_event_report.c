#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <telit_sdk.h>
#include <telit_sdk_cat.h>
#include <tqcm.h>
#include <tqcm_cat.h>
#include <telit_sdk_test_helpers.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;
static sem_t stop_sem;

static void ctrl_c_handler(int sig)
{
    sem_post(&stop_sem);
}

static void cb(void *item)
{
    indication_item_t *ind = (indication_item_t *) item;
    uint8_t indication_buf[4096] = {0};
    int ret;

    ret = telit_cat_indication_handler(ind->msg_id,
            ind->buf, ind->buf_len,
            indication_buf, sizeof(indication_buf));
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("unpack fail %d ", ret);
    } else {
        switch (ind->msg_id) {
            case QMI_CAT_EVENT_REPORT_IND:
                cat_ind_event_report_t *out = (cat_ind_event_report_t *)indication_buf;

                RLOGI("Indication QMI_CAT_EVENT_REPORT_IND received:");
                if (out->display_text_event_available) {
                    RLOGI("uim_ref_id: %u, size = %u",
                            out->display_text_event.uim_ref_id,
                            out->display_text_event.cmd_sz);
                }
                if (out->setup_menu_event_available) {
                    RLOGI("uim_ref_id: %u, size = %u",
                            out->setup_menu_event.uim_ref_id,
                            out->setup_menu_event.cmd_sz);
                }
                if (out->slot_available) {
                    RLOGI("Slot index: %u", out->slot);
                }
                if (out->rsp_type_available) {
                    RLOGI("Response type: %u", out->rsp_type);
                }
                telit_print_buffer(ind->buf, ind->buf_len, LOG_INFO);
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
    cat_set_event_report_masks_t input = {0};
    cat_set_event_report_masks_t out = {0};
    cat_configuration_mode_t input_mode;
    tqcm_client_context_t ctx_cat;
    int ret = EXIT_SUCCESS;
    struct sigaction act;
    uint16_t sdk_error;

    telit_set_log_level(LOG_DEBUG);
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

    if (tqcm_init(dev, &ctx)) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto sem_disposal;
    }

    if (tqcm_init_client_context(QMI_SVC_CAT, &ctx, &ctx_cat, cb)) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto tqcm_disposal;
    }

    input_mode.cat_config_mode = QMI_CAT_MODE_ANDROID;
    ret = tqcm_cat_set_configuration_send(&ctx_cat, &input_mode, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO)
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    input.pc_evt_report_req_mask_available = true;
    input.pc_evt_report_req_mask = 0xFFFFFFFF;
    ret = tqcm_cat_set_event_report_send(&ctx_cat, &input, &out, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    /*
     * To trigger the indication you can for example:
     *   - insert/remove the SIM
     *   - issue at+cfun=4 on an AT port channel
     */

    sem_wait(&stop_sem);

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_cat) != 0) {
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
