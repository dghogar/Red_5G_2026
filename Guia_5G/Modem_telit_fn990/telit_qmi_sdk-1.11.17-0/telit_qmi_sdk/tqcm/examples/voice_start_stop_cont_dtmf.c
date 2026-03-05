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
    voice_cont_dtmf_input_t input;
    voice_dtmf_response_t out;
    uint16_t sdk_error;

    telit_set_log_level(LOG_INFO);

    if (argc == 3) {
        dev = default_rmnet_dev;
        input.call_id = atoi(argv[1]);
        memcpy(&input.digit, &argv[2][0], 1);
    } else if (argc == 4) {
        dev = argv[1];
        input.call_id = atoi(argv[2]);
        memcpy(&input.digit, &argv[3][0], 1);
    } else {
        RLOGI("Call ID and DTMF char needed");
        goto end;
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

    /* NOTE: a voice call should be in place for this function to work */
    RLOGI("Starting \"%c\" to call %d",
            input.digit, input.call_id);

    ret = tqcm_voice_start_cont_dtmf_send(&ctx_voice,
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
    }

    if (out.call_id_available) {
        RLOGI("Call id %u", out.call_id);
    }

    sleep(2);

    RLOGI("Stopping \"%c\" to call %d",
            input.digit, input.call_id);

    ret = tqcm_voice_stop_cont_dtmf_send(&ctx_voice,
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
    }

    if (out.call_id_available) {
        RLOGI("Call id %u", out.call_id);
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
