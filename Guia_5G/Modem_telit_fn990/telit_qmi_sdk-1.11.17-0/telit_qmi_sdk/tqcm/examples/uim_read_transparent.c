#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <telit_sdk.h>
#include <telit_sdk_uim.h>
#include <tqcm.h>
#include <tqcm_uim.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_uim;
    uim_read_file_t info;
    uim_read_file_info_t out;
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

    ret = tqcm_init_client_context(QMI_SVC_UIM, &ctx, &ctx_uim, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_uim.client_id);

    info.file_id = 0x6F07;
    info.file_path_len = 2 * sizeof(uint16_t);
    info.file_path[0] = 0x3F00;
    info.file_path[1] = 0x7f21;
    info.session_information.session_type = QMI_UIM_SESSION_TYPE_PRIMARY_GW_PROVISIONING;
    info.session_information.aid_len = 0;
    /* UNCOMMENT THIS FOR RESPONSE AS INDICATION */
    /*
       info.response_as_indication_available = true;
       info.indication_token = 0x00FFFFFF;
     */

    memset(&out, 0, sizeof(out));

    ret = tqcm_uim_read_transparent_send(&ctx_uim, &info, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    if (out.card_result_available) {
        RLOGI("Card result: sw1:%X, sw2:%X",
                out.card_result.sw1,
                out.card_result.sw2);
    }

    RLOGI("Read result: len: %u", out.read_result.len);
    if (out.read_result_available && out.read_result.len) {
        for (int i = 0; i < out.read_result.len; i++)
            RLOGI("0x%X ", out.read_result.content[i]);
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_uim) != 0) {
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
