#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_wms.h"

int tqcm_wms_set_event_report_send(tqcm_client_context_t *ctx,
        wms_set_event_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_WMS,
            QMI_WMS_SET_EVENT_REPORT);
}

int tqcm_wms_raw_send_send(tqcm_client_context_t *ctx,
        wms_raw_send_t *input,
        wms_raw_send_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WMS,
            QMI_WMS_RAW_SEND);
}

int tqcm_wms_raw_write_send(tqcm_client_context_t *ctx,
        wms_raw_write_t *input,
        wms_raw_write_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WMS,
            QMI_WMS_RAW_WRITE);
}

int tqcm_wms_raw_read_send(tqcm_client_context_t *ctx,
        wms_raw_read_t *input,
        wms_raw_read_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WMS,
            QMI_WMS_RAW_READ);
}

int tqcm_wms_modify_tag_send(tqcm_client_context_t *ctx,
        wms_modify_tag_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_WMS,
            QMI_WMS_MODIFY_TAG);
}

int tqcm_wms_delete_send(tqcm_client_context_t *ctx,
        wms_delete_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_WMS,
            QMI_WMS_DELETE);
}

int tqcm_wms_list_messages_send(tqcm_client_context_t *ctx,
        wms_list_messages_t *input,
        wms_list_messages_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WMS,
            QMI_WMS_LIST_MESSAGES);
}

int tqcm_wms_set_routes_send(tqcm_client_context_t *ctx,
        wms_set_routes_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_WMS,
            QMI_WMS_SET_ROUTES);
}

int tqcm_wms_get_smsc_address_send(tqcm_client_context_t *ctx,
        wms_smsc_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_WMS,
            QMI_WMS_GET_SMSC_ADDR);
}

int tqcm_wms_set_smsc_address_send(tqcm_client_context_t *ctx,
        wms_smsc_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_WMS,
            QMI_WMS_SET_SMSC_ADDR);
}

int tqcm_wms_send_ack_send(tqcm_client_context_t *ctx,
        wms_send_ack_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_WMS,
            QMI_WMS_SEND_ACK);
}

QMI_SDK_error_t tqcm_wms_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_WMS_SET_EVENT_REPORT:
            ret = telit_wms_set_event_report_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WMS_RAW_SEND:
            ret = telit_wms_raw_send_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WMS_RAW_WRITE:
            ret = telit_wms_raw_write_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WMS_RAW_READ:
            ret = telit_wms_raw_read_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WMS_MODIFY_TAG:
            ret = telit_wms_modify_tag_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WMS_DELETE:
            ret = telit_wms_delete_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WMS_LIST_MESSAGES:
            ret = telit_wms_list_messages_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WMS_SET_ROUTES:
            ret = telit_wms_set_routes_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WMS_GET_SMSC_ADDR:
            ret = telit_wms_get_smsc_address_pack(req_ctx, req, req_len);
            break;
        case QMI_WMS_SET_SMSC_ADDR:
            ret = telit_wms_set_smsc_address_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WMS_SEND_ACK:
            ret = telit_wms_send_ack_pack(req_ctx, req, req_len, input);
            break;
        default:
            RLOGE("%s: WMS msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}

QMI_SDK_error_t tqcm_wms_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_WMS_SET_EVENT_REPORT:
            ret = telit_wms_set_event_report_unpack(resp, resp_len);
            break;
        case QMI_WMS_RAW_SEND:
            ret = telit_wms_raw_send_unpack(resp, resp_len, out);
            break;
        case QMI_WMS_RAW_WRITE:
            ret = telit_wms_raw_write_unpack(resp, resp_len, out);
            break;
        case QMI_WMS_RAW_READ:
            ret = telit_wms_raw_read_unpack(resp, resp_len, out);
            break;
        case QMI_WMS_MODIFY_TAG:
            ret = telit_wms_set_smsc_address_unpack(resp, resp_len);
            break;
        case QMI_WMS_DELETE:
            ret = telit_wms_delete_unpack(resp, resp_len);
            break;
        case QMI_WMS_LIST_MESSAGES:
            ret = telit_wms_list_messages_unpack(resp, resp_len, out);
            break;
        case QMI_WMS_SET_ROUTES:
            ret = telit_wms_set_routes_unpack(resp, resp_len);
            break;
        case QMI_WMS_GET_SMSC_ADDR:
            ret = telit_wms_get_smsc_address_unpack(resp, resp_len, out);
            break;
        case QMI_WMS_SET_SMSC_ADDR:
            ret = telit_wms_set_smsc_address_unpack(resp, resp_len);
            break;
        case QMI_WMS_SEND_ACK:
            ret = telit_wms_send_ack_unpack(resp, resp_len);
            break;
        default:
            RLOGE("%s: WMS msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}
