#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_dpm.h"

int tqcm_dpm_open_port_send(tqcm_client_context_t *ctx,
        dpm_open_port_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_DPM,
            QMI_DPM_OPEN_PORT);
}

QMI_SDK_error_t tqcm_dpm_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_DPM_OPEN_PORT:
            ret = telit_dpm_open_port_pack(req_ctx, req, req_len, input);
            break;
        default:
            RLOGE("%s: DPM msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}

QMI_SDK_error_t tqcm_dpm_unpack(void *resp,
        uint16_t resp_len,
        void *out __unused,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_DPM_OPEN_PORT:
            ret = telit_dpm_open_port_unpack(resp, resp_len);
            break;
        default:
            RLOGE("%s: DPM msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}
