#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <telit_sdk.h>
#include <telit_sdk_dms.h>
#include <tqcm.h>
#include <tqcm_dms.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

static void print_band_caps(dms_band_capabilities_t *out)
{
    uint16_t i;
    RLOGI("DMS band_caps %lu (0x%08X)", out->band_capabilities, out->band_capabilities);
    if (out->tds_band_capabilities_available) {
        RLOGI("DMS TDS band_caps %lu (0x%08X)", out->tds_band_capabilities, out->tds_band_capabilities);
    }
    RLOGI("Supported %u lte bands", out->supported_lte_bands_capabilities_size);
    for (i = 0; i < out->supported_lte_bands_capabilities_size; i++) {
        RLOGI("LTE band %u = %u", i, out->supported_lte_bands[i]);
    }
    RLOGI("Supported %u nr5g bands", out->supported_nr5g_bands_capabilities_size);
    for (i = 0; i < out->supported_nr5g_bands_capabilities_size; i++) {
        RLOGI("nr5g band %u = %u", i, out->supported_nr5g_bands[i]);
    }
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_dms;
    dms_band_capabilities_t out = {0};
    uint16_t sdk_error;

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

    ret = tqcm_init_client_context(QMI_SVC_DMS,
            &ctx,
            &ctx_dms,
            NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_dms.client_id);

    ret = tqcm_dms_get_band_capability_send(&ctx_dms,
            &out,
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

    print_band_caps(&out);

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_dms) != 0) {
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
