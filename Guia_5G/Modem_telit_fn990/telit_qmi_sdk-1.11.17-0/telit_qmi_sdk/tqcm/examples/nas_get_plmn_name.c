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
    nas_plmn_names_t out;
    nas_plmn_info_t input;
    uint16_t sdk_error;

    telit_set_log_level(LOG_INFO);

    input.mcc = 222;
    input.mnc = 1;

    input.mnc_includes_pcs_digit_available = true;
    input.mnc_includes_pcs_digit = false;

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

    ret = tqcm_nas_get_plmn_name_send(&ctx_nas, &input, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    if (out.eons_plmn_name_available) {
        RLOGI("spn_encoding %d", out.eons_plmn_name.name.spn_encoding);
        if (out.eons_plmn_name.name.spn_name_len > 0) {
            RLOGI("spn_name %s", out.eons_plmn_name.name.spn_name);
        }

        RLOGI("short_name_encoding %d", out.eons_plmn_name.name_short.short_name_encoding);
        RLOGI("short_name_country_initials %d", out.eons_plmn_name.name_short.short_name_country_initials);
        RLOGI("short_name_spare_bits %d", out.eons_plmn_name.name_short.short_name_spare_bits);
        if (out.eons_plmn_name.name_short.short_name_len > 0) {
            RLOGI("short_name %s", out.eons_plmn_name.name_short.short_name);
        }

        RLOGI("long_name_encoding %d", out.eons_plmn_name.name_long.long_name_encoding);
        RLOGI("long_name_country_initials %d", out.eons_plmn_name.name_long.long_name_country_initials);
        RLOGI("long_name_long_bits %d", out.eons_plmn_name.name_long.long_name_long_bits);
        if (out.eons_plmn_name.name_long.long_name_len > 0) {
            RLOGI("long_name %s", out.eons_plmn_name.name_long.long_name);
        }
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
