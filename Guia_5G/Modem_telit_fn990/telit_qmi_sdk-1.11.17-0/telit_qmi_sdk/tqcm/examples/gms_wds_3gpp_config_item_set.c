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

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_gms;
    uint16_t sdk_error;
    wds_3gpp_config_item_t in = {
        .lte_attach_profile_available = true,
        .lte_attach_profile = 2,
        .profile_list_size = 2,
        .profile_list = {3, 4},
        .default_PDN_enabled_available = true,
        .default_PDN_enabled = QMI_GMS_DEFAULT_PDN_STATE_ENABLED,
        .release_3gpp_available = true,
        .release_3gpp = QMI_GMS_3GPP_RELEASE_7,
        .attach_LTE_profile_list_size = 4,
        .attach_LTE_profile_list = {1, 2, 4, 8}
    };

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

    ret = tqcm_gms_wds_3gpp_config_item_set_send(&ctx_gms,
            &in,
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
        RLOGI("GMS 3GPP config item set success\n");
    }

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
