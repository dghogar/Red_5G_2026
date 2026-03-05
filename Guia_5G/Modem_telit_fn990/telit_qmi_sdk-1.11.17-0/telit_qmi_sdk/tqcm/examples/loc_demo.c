#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <telit_sdk.h>
#include <telit_sdk_loc.h>
#include <tqcm.h>
#include <tqcm_loc.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;
static sem_t mutex_indication;
uint8_t session_id = 0;
tqcm_client_context_t ctx_loc;

static void cb(void *item)
{
    int ret;
    indication_item_t *ind = (indication_item_t *)item;

    switch (ind->msg_id) {
        case QMI_LOC_DELETE_ASSIST_DATA_IND:
        case QMI_LOC_SET_NMEA_TYPES_IND:
        case QMI_LOC_SET_OPERATION_MODE_IND: {
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
            break;
        }
        case QMI_LOC_EVENT_NMEA_IND: {
            loc_nmea_ind_t out;
            char *pos;

            ret = telit_loc_indication_handler(ind->msg_id,
                    ind->buf, ind->buf_len,
                    (uint8_t *)&out, sizeof(out));
            if (ret == QMI_SDK_ERR_NONE) {
                if ((pos = strchr(out.nmea_string, '\n')) != NULL) {
                    *pos = '\0';
                }
                RLOGI("NMEA string: %s", out.nmea_string);
            } else {
                RLOGE("unpack fail %d ", ret);
            }
            break;
        }
        case QMI_LOC_EVENT_GNSS_SV_INFO_IND: {
            loc_gnss_sv_info_t out;

            ret = telit_loc_indication_handler(ind->msg_id,
                    ind->buf, ind->buf_len,
                    (uint8_t *)&out, sizeof(out));
            if (ret == QMI_SDK_ERR_NONE) {
                if (out.sv_info_size)
                    RLOGI("Satellites info size: %u", out.sv_info_size);
                if (out.sv_ext_info_size)
                    RLOGI("Satellites extended info size: %u", out.sv_ext_info_size);
            }
            break;
        }
        default:
            RLOGW("Indication not managed");
            break;
    }

    free(ind->buf);
    free(ind);

    /*sem_post(&mutex_indication); */
}

static void sig_handler(int s __unused)
{
    int ret = EXIT_SUCCESS;
    loc_stop_t stop;
    uint16_t sdk_error;

    memset(&stop, 0, sizeof(stop));

    stop.session_id = session_id;

    ret = tqcm_loc_stop_send(&ctx_loc, &stop, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("Stop send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
    } else {
        RLOGI("Stop Success");
    }
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    uint16_t sdk_error;
    loc_start_t start;
    loc_event_register_t evt;
    loc_delete_assist_data_t del;
    loc_nmea_types_t nmea;
    loc_operation_mode_t mode;

    struct sigaction sigHandler;

    sigHandler.sa_handler = sig_handler;
    sigemptyset(&sigHandler.sa_mask);
    sigHandler.sa_flags = 0;

    session_id = rand() % 255;

    memset(&mode, 0, sizeof(mode));

    mode.operation_mode = QMI_LOC_OPER_MODE_DEFAULT;

    memset(&nmea, 0, sizeof(nmea));

    nmea.nmea_sentence_type = QMI_LOC_NMEA_MASK_GGA |
            QMI_LOC_NMEA_MASK_RMC |
            QMI_LOC_NMEA_MASK_GSV |
            QMI_LOC_NMEA_MASK_GSA |
            QMI_LOC_NMEA_MASK_VTG |
            QMI_LOC_NMEA_MASK_PQXFI |
            QMI_LOC_NMEA_MASK_PSTIS |
            QMI_LOC_NMEA_MASK_GLGSV |
            QMI_LOC_NMEA_MASK_GNGSA |
            QMI_LOC_NMEA_MASK_GNGNS |
            QMI_LOC_NMEA_MASK_GARMC |
            QMI_LOC_NMEA_MASK_GAGSV |
            QMI_LOC_NMEA_MASK_GAGSA |
            QMI_LOC_NMEA_MASK_GAVTG |
            QMI_LOC_NMEA_MASK_GAGGA |
            QMI_LOC_NMEA_MASK_PQGSA |
            QMI_LOC_NMEA_MASK_PQGSV;

    memset(&del, 0, sizeof(del));
    del.delete_all_flag = true;

    memset(&evt, 0, sizeof(evt));
    evt.event_mask = QMI_LOC_EVENT_MASK_POSITION_REPORT |
            QMI_LOC_EVENT_MASK_NMEA |
            QMI_LOC_EVENT_MASK_GNSS_SV_INFO;

    memset(&start, 0, sizeof(start));

    start.session_id = session_id;
    start.recurrence_available = true;
    start.recurrence = 1;
    if (sem_init(&mutex_indication, 0, 0)) {
        RLOGE("%s", strerror(errno));
        ret = EXIT_FAILURE;
        goto end;
    }


    sigaction(SIGINT, &sigHandler, NULL);
    sigaction(SIGTERM, &sigHandler, NULL);
    sigaction(SIGKILL, &sigHandler, NULL);

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

    ret = tqcm_loc_reg_events_send(&ctx_loc, &evt, &sdk_error, NULL, 0);

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

    ret = tqcm_loc_delete_assist_data_send(&ctx_loc, &del, &sdk_error, NULL, 0);

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

    ret = tqcm_loc_set_nmea_types_send(&ctx_loc, &nmea, &sdk_error, NULL, 0);

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

    ret = tqcm_loc_set_operation_mode_send(&ctx_loc, &mode, &sdk_error, NULL, 0);

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

    ret = tqcm_loc_start_send(&ctx_loc, &start, &sdk_error, NULL, 0);

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
