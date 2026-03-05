#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_wda.h"

int tqcm_wda_set_data_format_send(tqcm_client_context_t *ctx,
        wda_data_format_t *input,
        wda_data_format_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WDA,
            QMI_WDA_SET_DATA_FORMAT);
}

int tqcm_wda_get_data_format_send(tqcm_client_context_t *ctx,
        wda_get_data_format_t *input,
        wda_data_format_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WDA,
            QMI_WDA_GET_DATA_FORMAT);
}

QMI_SDK_error_t tqcm_wda_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_WDA_SET_DATA_FORMAT:
            ret = telit_wda_set_data_format_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDA_GET_DATA_FORMAT:
            ret = telit_wda_get_data_format_pack(req_ctx, req, req_len, input);
            break;
        default:
            RLOGE("%s: WDA msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}

QMI_SDK_error_t tqcm_wda_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_WDA_SET_DATA_FORMAT:
            ret = telit_wda_set_data_format_unpack(resp, resp_len, out);
            break;
        case QMI_WDA_GET_DATA_FORMAT:
            ret = telit_wda_get_data_format_unpack(resp, resp_len, out);
            break;
        default:
            RLOGE("%s: WDA msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}
