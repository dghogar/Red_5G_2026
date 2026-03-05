#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_nas.h>
#include <tqcm.h>
#include <tqcm_nas.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_nas;
    nas_system_selection_pref_t out;
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

    ret = tqcm_init_client_context(QMI_SVC_NAS, &ctx, &ctx_nas, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_nas.client_id);

    ret = tqcm_nas_get_system_selection_preference_send(&ctx_nas, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    if (out.emergency_mode_available) {
        RLOGI("emergency_mode_on: %d", out.emergency_mode_on);
    }

    if (out.mode_pref_available) {
        RLOGI("mode_pref: 0x%0004X%s%s%s%s%s", out.mode_pref,
                (out.mode_pref & (0x1L << 0)) ? " - CDMA2000" : "",
                (out.mode_pref & (0x1L << 1)) ? " - HRPD" : "",
                (out.mode_pref & (0x1L << 2)) ? " - GSM" : "",
                (out.mode_pref & (0x1L << 3)) ? " - UMTS" : "",
                (out.mode_pref & (0x1L << 4)) ? " - LTE" : "");
    }

    if (out.band_pref_available) {
        RLOGI("band_pref: 0x%016lX", out.band_pref);
    }

    if (out.roam_pref_available) {
        RLOGI("roam_pref: %d", out.roam_pref);
    }

    if (out.lte_band_pref_available) {
        RLOGI("lte_band_pref: 0x%016lX", out.lte_band_pref);
    }

    if (out.net_sel_pref_available) {
        RLOGI("net_sel_pref: %d (%s)", out.net_sel_pref,
                out.net_sel_pref ? "Manual" : "Auto");
    }

    if (out.srv_domain_pref_available) {
        RLOGI("srv_domain_pref: %d", out.srv_domain_pref);
    }

    if (out.gw_acq_order_pref_available) {
        RLOGI("gw_acq_order_pref: %d", out.gw_acq_order_pref);
    }

    if (out.lte_band_preference_extended_available) {
        RLOGI("lte_band_preference_extended bits_1_64: 0x%016lX",
                out.lte_band_preference_extended.bits_1_64);
        RLOGI("lte_band_preference_extended bits_65_128: 0x%016lX",
                out.lte_band_preference_extended.bits_65_128);
        RLOGI("lte_band_preference_extended bits_129_192: 0x%016lX",
                out.lte_band_preference_extended.bits_129_192);
        RLOGI("lte_band_preference_extended bits_193_256: 0x%016lX",
                out.lte_band_preference_extended.bits_193_256);
    }

    if (out.nr5g_band_preference_available) {
        RLOGI("nr5g_band_preference bits_1_64: 0x%016lX",
                out.nr5g_band_preference.bits_1_64);
        RLOGI("nr5g_band_preference bits_65_128: 0x%016lX",
                out.nr5g_band_preference.bits_65_128);
        RLOGI("nr5g_band_preference bits_129_192: 0x%016lX",
                out.nr5g_band_preference.bits_129_192);
        RLOGI("nr5g_band_preference bits_193_256: 0x%016lX",
                out.nr5g_band_preference.bits_193_256);
        RLOGI("nr5g_band_preference bits_257_320: 0x%016lX",
                out.nr5g_band_preference.bits_257_320);
        RLOGI("nr5g_band_preference bits_321_384: 0x%016lX",
                out.nr5g_band_preference.bits_321_384);
        RLOGI("nr5g_band_preference bits_385_448: 0x%016lX",
                out.nr5g_band_preference.bits_385_448);
        RLOGI("nr5g_band_preference bits_449_512: 0x%016lX",
                out.nr5g_band_preference.bits_449_512);
    }

    if (out.nr5g_sa_band_preference_available) {
        RLOGI("nr5g_sa_band_preference bits_1_64: 0x%016lX",
                out.nr5g_sa_band_preference.bits_1_64);
        RLOGI("nr5g_sa_band_preference bits_65_128: 0x%016lX",
                out.nr5g_sa_band_preference.bits_65_128);
        RLOGI("nr5g_sa_band_preference bits_129_192: 0x%016lX",
                out.nr5g_sa_band_preference.bits_129_192);
        RLOGI("nr5g_sa_band_preference bits_193_256: 0x%016lX",
                out.nr5g_sa_band_preference.bits_193_256);
        RLOGI("nr5g_sa_band_preference bits_257_320: 0x%016lX",
                out.nr5g_sa_band_preference.bits_257_320);
        RLOGI("nr5g_sa_band_preference bits_321_384: 0x%016lX",
                out.nr5g_sa_band_preference.bits_321_384);
        RLOGI("nr5g_sa_band_preference bits_385_448: 0x%016lX",
                out.nr5g_sa_band_preference.bits_385_448);
        RLOGI("nr5g_sa_band_preference bits_449_512: 0x%016lX",
                out.nr5g_sa_band_preference.bits_449_512);
    }

    if (out.nr5g_nsa_band_preference_available) {
        RLOGI("nr5g_nsa_band_preference bits_1_64: 0x%016lX",
                out.nr5g_nsa_band_preference.bits_1_64);
        RLOGI("nr5g_nsa_band_preference bits_65_128: 0x%016lX",
                out.nr5g_nsa_band_preference.bits_65_128);
        RLOGI("nr5g_nsa_band_preference bits_129_192: 0x%016lX",
                out.nr5g_nsa_band_preference.bits_129_192);
        RLOGI("nr5g_nsa_band_preference bits_193_256: 0x%016lX",
                out.nr5g_nsa_band_preference.bits_193_256);
        RLOGI("nr5g_nsa_band_preference bits_257_320: 0x%016lX",
                out.nr5g_nsa_band_preference.bits_257_320);
        RLOGI("nr5g_nsa_band_preference bits_321_384: 0x%016lX",
                out.nr5g_nsa_band_preference.bits_321_384);
        RLOGI("nr5g_nsa_band_preference bits_385_448: 0x%016lX",
                out.nr5g_nsa_band_preference.bits_385_448);
        RLOGI("nr5g_nsa_band_preference bits_449_512: 0x%016lX",
                out.nr5g_nsa_band_preference.bits_449_512);
    }

    if (out.nr5g_sa_nsa_mode_disable_available) {
        RLOGI("nr5g_SA_NSA_mode_disable: %d", out.nr5g_sa_nsa_mode_disable);
    }

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

    /* Needed for valgrind complain about thread stack memory */
    sleep(1);
end:

    return ret;
}
