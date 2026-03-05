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
    nas_get_lte_cphy_ca_info_t out;
    uint16_t sdk_error;
    int i;

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

    ret = tqcm_nas_get_lte_cphy_ca_info_send(&ctx_nas, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    if (out.dl_bw_value_available) {
        RLOGI("dl_bandwidth: %d", out.dl_bw_value);
    }

    if (out.pcell_info_available) {
        RLOGI("pcell.band: %d", out.pcell_info.band);
        RLOGI("pcell.dl_bw_value: %d", out.pcell_info.dl_bw_value);
        RLOGI("pcell.freq: %d", out.pcell_info.freq);
        RLOGI("pcell.pci: %d", out.pcell_info.pci);
    }

    if (out.scell_ind_type_available) {
        RLOGI("scell_ind_type.freq: %d", out.scell_ind_type.freq);
        RLOGI("scell_ind_type.pci: %d", out.scell_ind_type.pci);
        RLOGI("scell_ind_type.scell_state: %d", out.scell_ind_type.scell_state);
    }

    for (i = 0; i < out.scell_info_list_size; i++) {
        RLOGI("scell[%d]band %d, ", i, out.scell_info_list[i].band);
        RLOGI("scell[%d]dl_bw_value %u ", i, out.scell_info_list[i].dl_bw_value);
        RLOGI("scell[%d]freq %u ", i, out.scell_info_list[i].freq);
        RLOGI("scell[%d]pci %u ", i, out.scell_info_list[i].pci);
        RLOGI("scell[%d]scell_idx %u ", i, out.scell_info_list[i].scell_idx);
        RLOGI("scell[%d]scell_state %u ", i, out.scell_info_list[i].scell_state);
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
