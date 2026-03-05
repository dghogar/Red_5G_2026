#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_uim.h>
#include <telit_sdk_test_helpers.h>
#include <tqcm.h>
#include <tqcm_uim.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_uim;
    uim_logical_channel_info_t input = {0};
    uim_logical_channel_resp_t out;
    uint16_t sdk_error;
    char *aid = NULL;

    telit_set_log_level(LOG_INFO);

    if (argc == 1) {
        RLOGE("AID is required");
        ret = EXIT_FAILURE;
        goto end;
    } else if (argc == 2) {
        dev = default_rmnet_dev;
        aid = argv[1];
    } else {
        dev = argv[1];
        aid = argv[2];
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

    input.aid_len = ascii_to_hex(aid, strlen(aid), input.aid);
    input.slot = 1;

    ret = tqcm_uim_logical_channel_send(&ctx_uim,
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

    if (out.channel_id_available) {
        uint8_t *response_bytes = NULL;
        size_t sz = 0;

        RLOGI("Channel id = %u", out.channel_id);
        if (out.select_response_sz) {
            sz += out.select_response_sz;
            response_bytes = malloc(++sz);
            if (response_bytes) {
                response_bytes[0] = out.channel_id;
                for (size_t i = 1; i < sz; i++)
                    response_bytes[i] = out.select_response[i - 1];
            }
        } else if (out.long_select_response_sz) {
            sz += out.long_select_response_sz;
            response_bytes = malloc(++sz);
            if (response_bytes) {
                response_bytes[0] = out.channel_id;
                for (size_t i = 1; i < sz; i++)
                    response_bytes[i] = out.long_select_response[i - 1];
            }
        }

        if (sz && response_bytes)
            telit_print_buffer(response_bytes, sz, LOG_DEBUG);
        free(response_bytes);

        sleep(1);
        RLOGI("Closing channel");
        memset(&input, 0, sizeof(input));
        input.slot = 1;
        input.channel_id_to_close = out.channel_id;
        input.channel_id_to_close_available = true;

        ret = tqcm_uim_logical_channel_send(&ctx_uim,
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
    } else
        RLOGE("No channel id available");

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
