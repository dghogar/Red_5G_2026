#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_atr.h"

int tqcm_atr_cmd_send_send(tqcm_client_context_t *ctx,
        atr_cmd_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_ATR,
            QMI_ATR_CMD_SEND);
}

QMI_SDK_error_t tqcm_atr_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_ATR_CMD_SEND:
            ret = telit_atr_cmd_send_pack(req_ctx, req, req_len, input);
            break;
        default:
            RLOGE("%s: ATR msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}

QMI_SDK_error_t tqcm_atr_unpack(void *resp,
        uint16_t resp_len,
        void *out __unused,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_ATR_CMD_SEND:
            ret = telit_atr_cmd_send_unpack(resp, resp_len);
            break;
        default:
            RLOGE("%s: ATR msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}
