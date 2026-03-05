#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_cat.h"

int tqcm_cat_set_event_report_send(tqcm_client_context_t *ctx,
        cat_set_event_report_masks_t *input,
        cat_set_event_report_masks_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_CAT,
            QMI_CAT_SET_EVENT_REPORT);
}

int tqcm_cat_send_tr_send(tqcm_client_context_t *ctx,
        cat_send_tr_input_t *input,
        cat_send_tr_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_CAT,
            QMI_CAT_SEND_TR);
}

int tqcm_cat_send_envelope_cmd_send(tqcm_client_context_t *ctx,
        cat_send_envelope_cmd_input_t *input,
        cat_send_envelope_cmd_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_CAT,
            QMI_CAT_SEND_ENVELOPE_CMD);
}

int tqcm_cat_set_configuration_send(tqcm_client_context_t *ctx,
        cat_configuration_mode_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_CAT,
            QMI_CAT_SET_CONFIGURATION);
}

int tqcm_cat_get_configuration_send(tqcm_client_context_t *ctx,
        cat_configuration_mode_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_CAT,
            QMI_CAT_GET_CONFIGURATION);
}

int tqcm_cat_get_cached_proactive_cmd_send(tqcm_client_context_t *ctx,
        cat_cached_proactive_cmd_input_t *input,
        cat_cached_proactive_cmd_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_CAT,
            QMI_CAT_GET_CACHED_PROACTIVE_CMD);
}

QMI_SDK_error_t tqcm_cat_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_CAT_SET_EVENT_REPORT:
            ret = telit_cat_set_event_report_pack(req_ctx, req, req_len, input);
            break;
        case QMI_CAT_SEND_TR:
            ret = telit_cat_send_tr_pack(req_ctx, req, req_len, input);
            break;
        case QMI_CAT_SEND_ENVELOPE_CMD:
            ret = telit_cat_send_envelope_cmd_pack(req_ctx, req, req_len, input);
            break;
        case QMI_CAT_SET_CONFIGURATION:
            ret = telit_cat_set_configuration_pack(req_ctx, req, req_len, input);
            break;
        case QMI_CAT_GET_CONFIGURATION:
            ret = telit_cat_get_configuration_pack(req_ctx, req, req_len);
            break;
        case QMI_CAT_GET_CACHED_PROACTIVE_CMD:
            ret = telit_cat_get_cached_proactive_cmd_pack(req_ctx, req, req_len, input);
            break;
        default:
            RLOGE("%s: CAT msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}

QMI_SDK_error_t tqcm_cat_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_CAT_SET_EVENT_REPORT:
            ret = telit_cat_set_event_report_unpack(resp, resp_len, out);
            break;
        case QMI_CAT_SEND_TR:
            ret = telit_cat_send_tr_unpack(resp, resp_len, out);
            break;
        case QMI_CAT_SEND_ENVELOPE_CMD:
            ret = telit_cat_send_envelope_cmd_unpack(resp, resp_len, out);
            break;
        case QMI_CAT_SET_CONFIGURATION:
            ret = telit_cat_set_configuration_unpack(resp, resp_len);
            break;
        case QMI_CAT_GET_CONFIGURATION:
            ret = telit_cat_get_configuration_unpack(resp, resp_len, out);
            break;
        case QMI_CAT_GET_CACHED_PROACTIVE_CMD:
            ret = telit_cat_get_cached_proactive_cmd_unpack(resp, resp_len, out);
            break;
        default:
            RLOGE("%s: CAT msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}
