#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <netinet/in.h>

#include <telit_sdk.h>
#include <telit_sdk_wds.h>
#include <tqcm.h>
#include <tqcm_wds.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_wds;
    uint16_t sdk_error;
    wds_set_event_report_info_t input = { 0 };

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

    ret = tqcm_init_client_context(QMI_SVC_WDS, &ctx, &ctx_wds, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_wds.client_id);

    memset(&input, 0, sizeof (input));
    input.report_data_bearer_tech = QMI_IND_ACTION_SET;
    input.report_current_data_bearer_tech = QMI_IND_ACTION_SET;
    input.report_dormancy_status = QMI_IND_ACTION_SET;
    input.report_mip_status = QMI_IND_ACTION_SET;
    input.transfer_stats_available = true;
    input.transfer_stats.stats_period = 2;
    input.transfer_stats.stats_mask = (0x00000001 |
            0x00000002 |
            0x00000004 |
            0x00000008 |
            0x00000010 |
            0x00000020 |
            0x00000040 |
            0x00000080 |
            0x00000100 |
            0x00000200);
    input.report_data_system_status = QMI_IND_ACTION_SET;
    input.report_data_bearer_tech_ext = QMI_IND_ACTION_SET;
    ret = tqcm_wds_set_event_report_send(&ctx_wds, &input, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_wds) != 0) {
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
