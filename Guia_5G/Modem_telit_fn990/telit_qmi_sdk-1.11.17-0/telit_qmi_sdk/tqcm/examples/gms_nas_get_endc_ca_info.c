#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <telit_sdk.h>
#include <telit_sdk_gms.h>
#include <tqcm.h>
#include <tqcm_gms.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

static void print_output(gms_nas_endc_ca_info_t *output)
{
    if (output->pcc_ca_info_available) {
        RLOGI("pcc band_class: %u", output->pcc_ca_info.band_class);
        RLOGI("pcc channel: %u", output->pcc_ca_info.channel);
        RLOGI("pcc dl bandwidth: %u", output->pcc_ca_info.dl_bw);
        RLOGI("pcc pci: %u", output->pcc_ca_info.pci);
        RLOGI("pcc rsrp: %d", output->pcc_ca_info.rsrp);
        RLOGI("pcc rssi: %d", output->pcc_ca_info.rssi);
        RLOGI("pcc rsrq: %d", output->pcc_ca_info.rsrq);
        RLOGI("pcc sinr: %d", output->pcc_ca_info.sinr);
        RLOGI("pcc tac: %u", output->pcc_ca_info.tac);
    }

    if (output->scc_0_ca_info_available) {
        RLOGI("scc 0 band_class: %u", output->scc_0_ca_info.band_class);
        RLOGI("scc 0 channel: %u", output->scc_0_ca_info.channel);
        RLOGI("scc 0 dl bandwidth: %u", output->scc_0_ca_info.dl_bw);
        RLOGI("scc 0 pci: %u", output->scc_0_ca_info.pci);
        RLOGI("scc 0 rsrp: %d", output->scc_0_ca_info.rsrp);
        RLOGI("scc 0 rssi: %d", output->scc_0_ca_info.rssi);
        RLOGI("scc 0 rsrq: %d", output->scc_0_ca_info.rsrq);
        RLOGI("scc 0 sinr: %d", output->scc_0_ca_info.sinr);
        RLOGI("scc 0 state: %u", output->scc_0_ca_info.state);
    }

    if (output->scc_1_ca_info_available) {
        RLOGI("scc 1 band_class: %u", output->scc_1_ca_info.band_class);
        RLOGI("scc 1 channel: %u", output->scc_1_ca_info.channel);
        RLOGI("scc 1 dl bandwidth: %u", output->scc_1_ca_info.dl_bw);
        RLOGI("scc 1 pci: %u", output->scc_1_ca_info.pci);
        RLOGI("scc 1 rsrp: %d", output->scc_1_ca_info.rsrp);
        RLOGI("scc 1 rssi: %d", output->scc_1_ca_info.rssi);
        RLOGI("scc 1 rsrq: %d", output->scc_1_ca_info.rsrq);
        RLOGI("scc 1 sinr: %d", output->scc_1_ca_info.sinr);
        RLOGI("scc 1 state: %u", output->scc_1_ca_info.state);
    }

    if (output->scc_2_ca_info_available) {
        RLOGI("scc 2 band_class: %u", output->scc_2_ca_info.band_class);
        RLOGI("scc 2 channel: %u", output->scc_2_ca_info.channel);
        RLOGI("scc 2 dl bandwidth: %u", output->scc_2_ca_info.dl_bw);
        RLOGI("scc 2 pci: %u", output->scc_2_ca_info.pci);
        RLOGI("scc 2 rsrp: %d", output->scc_2_ca_info.rsrp);
        RLOGI("scc 2 rssi: %d", output->scc_2_ca_info.rssi);
        RLOGI("scc 2 rsrq: %d", output->scc_2_ca_info.rsrq);
        RLOGI("scc 2 sinr: %d", output->scc_2_ca_info.sinr);
        RLOGI("scc 2 state: %u", output->scc_2_ca_info.state);
    }

    if (output->scc_3_ca_info_available) {
        RLOGI("scc 3 band_class: %u", output->scc_3_ca_info.band_class);
        RLOGI("scc 3 channel: %u", output->scc_3_ca_info.channel);
        RLOGI("scc 3 dl bandwidth: %u", output->scc_3_ca_info.dl_bw);
        RLOGI("scc 3 pci: %u", output->scc_3_ca_info.pci);
        RLOGI("scc 3 rsrp: %d", output->scc_3_ca_info.rsrp);
        RLOGI("scc 3 rssi: %d", output->scc_3_ca_info.rssi);
        RLOGI("scc 3 rsrq: %d", output->scc_3_ca_info.rsrq);
        RLOGI("scc 3 sinr: %d", output->scc_3_ca_info.sinr);
        RLOGI("scc 3 state: %u", output->scc_3_ca_info.state);
    }

    if (output->scc_4_ca_info_available) {
        RLOGI("scc 4 band_class: %u", output->scc_4_ca_info.band_class);
        RLOGI("scc 4 channel: %u", output->scc_4_ca_info.channel);
        RLOGI("scc 4 dl bandwidth: %u", output->scc_4_ca_info.dl_bw);
        RLOGI("scc 4 pci: %u", output->scc_4_ca_info.pci);
        RLOGI("scc 4 rsrp: %d", output->scc_4_ca_info.rsrp);
        RLOGI("scc 4 rssi: %d", output->scc_4_ca_info.rssi);
        RLOGI("scc 4 rsrq: %d", output->scc_4_ca_info.rsrq);
        RLOGI("scc 4 sinr: %d", output->scc_4_ca_info.sinr);
        RLOGI("scc 4 state: %u", output->scc_4_ca_info.state);
    }

    if (output->cc_5g_info_available) {
        RLOGI("5G CC endc_band: %d", output->cc_5g_info.endc_band);
        RLOGI("5G CC endc_bandwidth: %d", output->cc_5g_info.endc_bandwidth);
        RLOGI("5G CC endc_channel: %d", output->cc_5g_info.endc_channel);
        RLOGI("5G CC endc_ul_channel: %d", output->cc_5g_info.endc_ul_channel);
        RLOGI("5G CC endc_pci: %d", output->cc_5g_info.endc_pci);
        RLOGI("5G CC endc_rsrp: %d", output->cc_5g_info.endc_rsrp);
        RLOGI("5G CC endc_rssi: %d", output->cc_5g_info.endc_rssi);
        RLOGI("5G CC endc_rsrq: %d", output->cc_5g_info.endc_rsrq);
        RLOGI("5G CC endc_sinr: %d", output->cc_5g_info.endc_sinr);
    }
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_gms;
    gms_nas_endc_ca_info_t output = {0};
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

    ret = tqcm_init_client_context(QMI_SVC_GMS,
            &ctx,
            &ctx_gms,
            NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_gms.client_id);

    ret = tqcm_gms_nas_get_endc_ca_info_send(&ctx_gms,
            &output,
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

    print_output(&output);

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_gms) != 0) {
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
