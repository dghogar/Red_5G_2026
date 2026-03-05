#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_wds.h"

int tqcm_wds_set_event_report_send(tqcm_client_context_t *ctx,
        wds_set_event_report_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_SET_EVENT_REPORT);
}

int tqcm_wds_indication_register_send(tqcm_client_context_t *ctx,
        wds_indication_register_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_INDICATION_REGISTER);
}

int tqcm_wds_start_network_interface_send(tqcm_client_context_t *ctx,
        wds_start_network_info_t *input,
        wds_start_network_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_START_NETWORK_INTERFACE);
}

int tqcm_wds_stop_network_interface_send(tqcm_client_context_t *ctx,
        wds_stop_network_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_STOP_NETWORK_INTERFACE);
}

int tqcm_wds_get_pkt_srvc_status_send(tqcm_client_context_t *ctx,
        wds_get_packet_service_status_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_GET_PKT_SRVC_STATUS);
}

int tqcm_wds_get_pkt_statistics_send(tqcm_client_context_t *ctx,
        wds_get_packet_statistics_mask_t *input,
        wds_get_packet_statistics_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_GET_PKT_STATISTICS);
}

int tqcm_wds_create_profile_send(tqcm_client_context_t *ctx,
        wds_profile_t *input,
        wds_profile_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_CREATE_PROFILE);
}

int tqcm_wds_modify_profile_settings_send(tqcm_client_context_t *ctx,
        wds_profile_t *input,
        wds_profile_ext_err_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_MODIFY_PROFILE_SETTINGS);
}

int tqcm_wds_delete_profile_send(tqcm_client_context_t *ctx,
        wds_settings_profile_id_t *input,
        wds_profile_ext_err_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_DELETE_PROFILE);
}

int tqcm_wds_get_profile_list_send(tqcm_client_context_t *ctx,
        wds_profile_list_input_t *input,
        wds_profile_list_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_GET_PROFILE_LIST);
}

int tqcm_wds_get_profile_settings_send(tqcm_client_context_t *ctx,
        wds_settings_profile_id_t *input,
        wds_profile_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_GET_PROFILE_SETTINGS);
}

int tqcm_wds_get_runtime_settings_send(tqcm_client_context_t *ctx,
        wds_runtime_settings_info_t *input,
        wds_profile_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_GET_RUNTIME_SETTINGS);
}

int tqcm_wds_get_call_duration_send(tqcm_client_context_t *ctx,
        wds_duration_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_GET_CALL_DURATION);
}

int tqcm_wds_get_default_profile_num_send(tqcm_client_context_t *ctx,
        wds_default_profile_input_t *input,
        wds_default_profile_out_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_GET_DEFAULT_PROFILE_NUM);
}

int tqcm_wds_set_client_ip_family_pref_send(tqcm_client_context_t *ctx,
        wds_ip_family_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_SET_CLIENT_IP_FAMILY_PREF);
}

int tqcm_wds_set_lte_attach_pdn_list_send(tqcm_client_context_t *ctx,
        wds_attach_pdn_list_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_SET_LTE_ATTACH_PDN_LIST);
}

int tqcm_wds_get_lte_attach_pdn_list_send(tqcm_client_context_t *ctx,
        wds_get_attach_pdn_list_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_GET_LTE_ATTACH_PDN_LIST);
}

int tqcm_wds_bind_mux_data_port_send(tqcm_client_context_t *ctx,
        wds_bind_mux_data_port_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_WDS,
            QMI_WDS_BIND_MUX_DATA_PORT);
}

QMI_SDK_error_t tqcm_wds_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_WDS_SET_EVENT_REPORT:
            ret = telit_wds_set_event_report_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_INDICATION_REGISTER:
            ret = telit_wds_indication_register_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_START_NETWORK_INTERFACE:
            ret = telit_wds_start_network_interface_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_STOP_NETWORK_INTERFACE:
            ret = telit_wds_stop_network_interface_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_GET_PKT_SRVC_STATUS:
            ret = telit_wds_get_pkt_srvc_status_pack(req_ctx, req, req_len);
            break;
        case QMI_WDS_GET_PKT_STATISTICS:
            ret = telit_wds_get_pkt_statistics_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_CREATE_PROFILE:
            ret = telit_wds_create_profile_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_MODIFY_PROFILE_SETTINGS:
            ret = telit_wds_modify_profile_settings_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_DELETE_PROFILE:
            ret = telit_wds_delete_profile_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_GET_PROFILE_LIST:
            ret = telit_wds_get_profile_list_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_GET_PROFILE_SETTINGS:
            ret = telit_wds_get_profile_settings_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_GET_RUNTIME_SETTINGS:
            ret = telit_wds_get_runtime_settings_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_GET_CALL_DURATION:
            ret = telit_wds_get_call_duration_pack(req_ctx, req, req_len);
            break;
        case QMI_WDS_GET_DEFAULT_PROFILE_NUM:
            ret = telit_wds_get_default_profile_num_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_SET_CLIENT_IP_FAMILY_PREF:
            ret = telit_wds_set_client_ip_family_pref_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_SET_LTE_ATTACH_PDN_LIST:
            ret = telit_wds_set_lte_attach_pdn_list_pack(req_ctx, req, req_len, input);
            break;
        case QMI_WDS_GET_LTE_ATTACH_PDN_LIST:
            ret = telit_wds_get_lte_attach_pdn_list_pack(req_ctx, req, req_len);
            break;
        case QMI_WDS_BIND_MUX_DATA_PORT:
            ret = telit_wds_bind_mux_data_port_pack(req_ctx, req, req_len, input);
            break;
        default:
            RLOGE("%s: WDS msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}

QMI_SDK_error_t tqcm_wds_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_WDS_SET_EVENT_REPORT:
            ret = telit_wds_set_event_report_unpack(resp, resp_len);
            break;
        case QMI_WDS_INDICATION_REGISTER:
            ret = telit_wds_indication_register_unpack(resp, resp_len);
            break;
        case QMI_WDS_START_NETWORK_INTERFACE:
            ret = telit_wds_start_network_interface_unpack(resp, resp_len, out);
            break;
        case QMI_WDS_STOP_NETWORK_INTERFACE:
            ret = telit_wds_stop_network_interface_unpack(resp, resp_len);
            break;
        case QMI_WDS_GET_PKT_SRVC_STATUS:
            ret = telit_wds_get_pkt_srvc_status_unpack(resp, resp_len, out);
            break;
        case QMI_WDS_GET_PKT_STATISTICS:
            ret = telit_wds_get_pkt_statistics_unpack(resp, resp_len, out);
            break;
        case QMI_WDS_CREATE_PROFILE:
            ret = telit_wds_create_profile_unpack(resp, resp_len, out);
            break;
        case QMI_WDS_MODIFY_PROFILE_SETTINGS:
            ret = telit_wds_modify_profile_settings_unpack(resp, resp_len, out);
            break;
        case QMI_WDS_DELETE_PROFILE:
            ret = telit_wds_delete_profile_unpack(resp, resp_len, out);
            break;
        case QMI_WDS_GET_PROFILE_LIST:
            ret = telit_wds_get_profile_list_unpack(resp, resp_len, out);
            break;
        case QMI_WDS_GET_PROFILE_SETTINGS:
            ret = telit_wds_get_profile_settings_unpack(resp, resp_len, out);
            break;
        case QMI_WDS_GET_RUNTIME_SETTINGS:
            ret = telit_wds_get_runtime_settings_unpack(resp, resp_len, out);
            break;
        case QMI_WDS_GET_CALL_DURATION:
            ret = telit_wds_get_call_duration_unpack(resp, resp_len, out);
            break;
        case QMI_WDS_GET_DEFAULT_PROFILE_NUM:
            ret = telit_wds_get_default_profile_num_unpack(resp, resp_len, out);
            break;
        case QMI_WDS_SET_CLIENT_IP_FAMILY_PREF:
            ret = telit_wds_set_client_ip_family_pref_unpack(resp, resp_len);
            break;
        case QMI_WDS_SET_LTE_ATTACH_PDN_LIST:
            ret = telit_wds_set_lte_attach_pdn_list_unpack(resp, resp_len);
            break;
        case QMI_WDS_GET_LTE_ATTACH_PDN_LIST:
            ret = telit_wds_get_lte_attach_pdn_list_unpack(resp, resp_len, out);
            break;
        case QMI_WDS_BIND_MUX_DATA_PORT:
            ret = telit_wds_bind_mux_data_port_unpack(resp, resp_len);
            break;
        default:
            RLOGE("%s: WDS msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}
