#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_gas.h>
#include <tqcm.h>
#include <tqcm_gas.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_gas;
    uint16_t sdk_error;
    gas_dms_fw_info_t out;
    gas_dms_get_fw_t input;

    memset(&out, 0, sizeof(out));
    memset(&input, 0, sizeof(input));

    input.operate_mode = 1;

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

    ret = tqcm_init_client_context(QMI_SVC_GAS,
            &ctx,
            &ctx_gas,
            NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_gas.client_id);

    ret = tqcm_gas_dms_get_fw_send(&ctx_gas,
            &input,
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
    } else {
        uint8_t i;
        RLOGI("Success");

        RLOGI("Operate mode %u", out.operate_mode);
        RLOGI("Available firmware %u", out.available_firmware);

        for (i = 0; i < out.available_firmware; i++) {
            RLOGI("Firmware %u:", out.fw_info[i].index);
            RLOGI("  model       : (%u) '%s'",
                    out.fw_info[i].model_len,
                    out.fw_info[i].model);
            RLOGI("  carrier_str : (%u) '%s'",
                    out.fw_info[i].carrier_str_len,
                    out.fw_info[i].carrier_str);
            RLOGI("  sku         : (%u) '%s'",
                    out.fw_info[i].sku_len,
                    out.fw_info[i].sku);
            RLOGI("  carrier name: (%u) '%s'",
                    out.fw_info[i].carrier_name.name_len,
                    out.fw_info[i].carrier_name.name);
            RLOGI("  version     : (%u) '%s'",
                    out.fw_info[i].version.versions_len,
                    out.fw_info[i].version.versions);
            RLOGI("  PRI revision: (%u) '%s'",
                    out.fw_info[i].pri_rev.pri_rev_len,
                    out.fw_info[i].pri_rev.pri_rev);
        }

        if (out.auto_sim_status_available) {
            RLOGI("auto sim: %s", out.auto_sim_status ? "ENABLED" : "DISABLED");
        }
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_gas) != 0) {
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
