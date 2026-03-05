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

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_loc;
    uint16_t sdk_error;
    loc_nmea_types_t input;

    memset(&input, 0, sizeof(input));

    input.nmea_sentence_type = QMI_LOC_NMEA_MASK_GGA |
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

    ret = tqcm_init_client_context(QMI_SVC_LOC, &ctx, &ctx_loc, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_loc.client_id);

    ret = tqcm_loc_set_nmea_types_send(&ctx_loc, &input, &sdk_error, NULL, 0);

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
