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
        case QMI_LOC_REGISTER_MASTER_CLIENT_IND:
        case QMI_LOC_SET_NMEA_TYPES_IND:
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
    loc_master_client_t client;
    loc_nmea_types_t nmea;

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

    memset(&client, 0, sizeof(client));

    if (sem_init(&mutex_indication, 0, 0)) {
        RLOGE("%s", strerror(errno));
        ret = EXIT_FAILURE;
        goto end;
    }

    telit_set_log_level(LOG_INFO);

    if (argc < 2) {
        RLOGE("master key should be provided");
        ret = EXIT_FAILURE;
        goto end;
    } else if (argc == 2) {
        dev = default_rmnet_dev;
        client.key = (uint32_t)atoi(argv[1]);
    } else {
        dev = argv[1];
        client.key = (uint32_t)atoi(argv[1]);
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

    ret = tqcm_loc_register_master_client_send(&ctx_loc, &client, &sdk_error, NULL, 0);

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

    ret = tqcm_loc_set_nmea_types_send(&ctx_loc, &nmea, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    RLOGI("Set NMEA types success");

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
