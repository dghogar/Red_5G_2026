#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_fota.h"

int tqcm_fota_oem_ftp_set_server_info_send(tqcm_client_context_t *ctx,
        fota_server_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_FOTA,
            QMI_FOTA_OEM_FTP_SET_SERVER_INFO);
}

int tqcm_fota_oem_ftp_get_server_info_send(tqcm_client_context_t *ctx,
        fota_server_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_FOTA,
            QMI_FOTA_OEM_FTP_GET_SERVER_INFO);
}

int tqcm_fota_oem_ftp_start_session_send(tqcm_client_context_t *ctx,
        fota_session_type_t *input,
        fota_session_status_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_FOTA,
            QMI_FOTA_OEM_FTP_START_SESSION);
}

int tqcm_fota_oem_ftp_stop_session_send(tqcm_client_context_t *ctx,
        fota_session_type_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_FOTA,
            QMI_FOTA_OEM_FTP_STOP_SESSION);
}

int tqcm_fota_oem_ftp_set_settings_send(tqcm_client_context_t *ctx,
        fota_settings_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_FOTA,
            QMI_FOTA_OEM_FTP_SET_SETTINGS);
}

int tqcm_fota_oem_ftp_get_settings_send(tqcm_client_context_t *ctx,
        fota_settings_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_FOTA,
            QMI_FOTA_OEM_FTP_GET_SETTINGS);
}

int tqcm_fota_oem_ftp_set_event_report_send(tqcm_client_context_t *ctx,
        fota_set_event_report_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_FOTA,
            QMI_FOTA_OEM_FTP_SET_EVENT_REPORT);
}

int tqcm_fota_oem_ftp_send_selection_send(tqcm_client_context_t *ctx,
        fota_selection_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_FOTA,
            QMI_FOTA_OEM_FTP_SEND_SELECTION);
}

int tqcm_fota_oem_att_get_host_odis_send(tqcm_client_context_t *ctx,
        fota_host_odis_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_FOTA,
            QMI_FOTA_OEM_ATT_GET_HOST_ODIS);
}

int tqcm_fota_oem_att_set_host_odis_send(tqcm_client_context_t *ctx,
        fota_host_odis_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_FOTA,
            QMI_FOTA_OEM_ATT_SET_HOST_ODIS);
}

int tqcm_fota_oem_ftp_get_session_info_send(tqcm_client_context_t *ctx,
        fota_session_type_t *input,
        fota_session_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_FOTA,
            QMI_FOTA_OEM_FTP_GET_SESSION_INFO);
}

QMI_SDK_error_t tqcm_fota_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_FOTA_OEM_FTP_SET_SERVER_INFO:
            ret = telit_fota_oem_ftp_set_server_info_pack(req_ctx, req, req_len, input);
            break;
        case QMI_FOTA_OEM_FTP_GET_SERVER_INFO:
            ret = telit_fota_oem_ftp_get_server_info_pack(req_ctx, req, req_len);
            break;
        case QMI_FOTA_OEM_FTP_START_SESSION:
            ret = telit_fota_oem_ftp_start_session_pack(req_ctx, req, req_len, input);
            break;
        case QMI_FOTA_OEM_FTP_STOP_SESSION:
            ret = telit_fota_oem_ftp_stop_session_pack(req_ctx, req, req_len, input);
            break;
        case QMI_FOTA_OEM_FTP_SET_SETTINGS:
            ret = telit_fota_oem_ftp_set_settings_pack(req_ctx, req, req_len, input);
            break;
        case QMI_FOTA_OEM_FTP_GET_SETTINGS:
            ret = telit_fota_oem_ftp_get_settings_pack(req_ctx, req, req_len);
            break;
        case QMI_FOTA_OEM_FTP_SET_EVENT_REPORT:
            ret = telit_fota_oem_ftp_set_event_report_pack(req_ctx, req, req_len, input);
            break;
        case QMI_FOTA_OEM_FTP_SEND_SELECTION:
            ret = telit_fota_oem_ftp_send_selection_pack(req_ctx, req, req_len, input);
            break;
        case QMI_FOTA_OEM_ATT_GET_HOST_ODIS:
            ret = telit_fota_oem_att_get_host_odis_pack(req_ctx, req, req_len);
            break;
        case QMI_FOTA_OEM_ATT_SET_HOST_ODIS:
            ret = telit_fota_oem_att_set_host_odis_pack(req_ctx, req, req_len, input);
            break;
        case QMI_FOTA_OEM_FTP_GET_SESSION_INFO:
            ret = telit_fota_get_session_info_pack(req_ctx, req, req_len, input);
            break;
        default:
            RLOGE("%s: FOTA msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}

QMI_SDK_error_t tqcm_fota_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_FOTA_OEM_FTP_SET_SERVER_INFO:
            ret = telit_fota_oem_ftp_set_server_info_unpack(resp, resp_len);
            break;
        case QMI_FOTA_OEM_FTP_GET_SERVER_INFO:
            ret = telit_fota_oem_ftp_get_server_info_unpack(resp, resp_len, out);
            break;
        case QMI_FOTA_OEM_FTP_START_SESSION:
            ret = telit_fota_oem_ftp_start_session_unpack(resp, resp_len, out);
            break;
        case QMI_FOTA_OEM_FTP_STOP_SESSION:
            ret = telit_fota_oem_ftp_stop_session_unpack(resp, resp_len);
            break;
        case QMI_FOTA_OEM_FTP_SET_SETTINGS:
            ret = telit_fota_oem_ftp_set_settings_unpack(resp, resp_len);
            break;
        case QMI_FOTA_OEM_FTP_GET_SETTINGS:
            ret = telit_fota_oem_ftp_get_settings_unpack(resp, resp_len, out);
            break;
        case QMI_FOTA_OEM_FTP_SET_EVENT_REPORT:
            ret = telit_fota_oem_ftp_set_event_report_unpack(resp, resp_len);
            break;
        case QMI_FOTA_OEM_FTP_SEND_SELECTION:
            ret = telit_fota_oem_ftp_send_selection_unpack(resp, resp_len);
            break;
        case QMI_FOTA_OEM_ATT_GET_HOST_ODIS:
            ret = telit_fota_oem_att_get_host_odis_unpack(resp, resp_len, out);
            break;
        case QMI_FOTA_OEM_ATT_SET_HOST_ODIS:
            ret = telit_fota_oem_att_set_host_odis_unpack(resp, resp_len);
            break;
        case QMI_FOTA_OEM_FTP_GET_SESSION_INFO:
            ret = telit_fota_get_session_info_unpack(resp, resp_len, out);
            break;
        default:
            RLOGE("%s: FOTA msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}
