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
    uim_pin_protection_info_t info;
    uim_pin_resp_t out;
    uint16_t sdk_error;
    char *pin;

    telit_set_log_level(LOG_INFO);

    if (argc < 2) {
        RLOGE("PIN should be provided");
        ret = EXIT_FAILURE;
        goto end;
    } else if (argc == 2) {
        dev = default_rmnet_dev;
        pin = argv[1];
    } else {
        dev = argv[1];
        pin = argv[2];
    }

    ret = tqcm_init(dev, &ctx);
    if (ret != 0) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_init_client_context(QMI_SVC_UIM,
            &ctx,
            &ctx_uim,
            NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_uim.client_id);

    memset(&info, 0, sizeof(info));
    info.session_information.session_type = QMI_UIM_SESSION_TYPE_PRIMARY_GW_PROVISIONING;
    info.session_information.aid_len = 0;
    info.pin_info.pin_id = QMI_UIM_PIN_ID_PIN1;
    info.pin_info.pin_operation = QMI_UIM_PIN_OPERATION_DISABLE;
    info.pin_info.pin_len = strlen(pin);
    strlcpy(info.pin_info.pin, pin, sizeof(info.pin_info.pin));
    /* UNCOMMENT THIS FOR RESPONSE AS INDICATION */
    /*
       info.response_as_indication_available = true;
       info.indication_token = 0x00FFFFFF;
     */

    memset(&out, 0, sizeof(out));

    ret = tqcm_uim_set_pin_protection_send(&ctx_uim, &info, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X, check if retries available", sdk_error);
            if (out.pin_retries_available) {
                RLOGE("pin verify left %u, unblock left %u",
                        out.retries_remaining.verify_left,
                        out.retries_remaining.unblock_left);
            }
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    if (out.encrypted_pin1_available) {
        for (int i = 0; i < out.encrypted_pin1.enc_pin1_len; i++)
            RLOGI("0x%X ", out.encrypted_pin1.enc_pin1[i]);
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
