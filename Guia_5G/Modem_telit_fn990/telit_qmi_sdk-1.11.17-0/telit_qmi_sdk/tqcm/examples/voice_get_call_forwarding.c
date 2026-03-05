#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_voice.h>
#include <tqcm.h>
#include <tqcm_voice.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_voice;
    voice_get_call_service_input_t input;
    voice_call_forwarding_t out;
    uint16_t sdk_error;
    uint8_t i = 0;

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

    ret = tqcm_init_client_context(QMI_SVC_VOICE,
            &ctx,
            &ctx_voice,
            NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_voice.client_id);

    input.reason = QMI_VOICE_REASON_FWDREASON_UNCONDITIONAL;

    ret = tqcm_voice_get_call_forwarding_send(&ctx_voice,
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

        if (out.failure_cause_available) {
            RLOGI("failure_cause %d", out.failure_cause);
        }

        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    for (i = 0; i < out.call_forwarding_info_size; i++) {
        RLOGI("service_status[%u] %d", i, out.call_forwarding_info[i].service_status);
        RLOGI("service_class[%u] %d", i, out.call_forwarding_info[i].service_class);
        RLOGI("number_len[%u] %d", i, out.call_forwarding_info[i].number_len);
        RLOGI("number[%u] %s", i, out.call_forwarding_info[i].number);
        RLOGI("no_reply_timer[%u] %d", i, out.call_forwarding_info[i].no_reply_timer);
    }

    if (out.call_id_available) {
        RLOGI("call ID %d", out.call_id);
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_voice) != 0) {
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
