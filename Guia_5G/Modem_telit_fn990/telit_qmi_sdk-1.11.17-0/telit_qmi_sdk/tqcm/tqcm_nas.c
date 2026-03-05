#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_nas.h"

int tqcm_nas_set_event_report_send(tqcm_client_context_t *ctx,
        nas_set_event_report_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_SET_EVENT_REPORT);
}

int tqcm_nas_indication_register_send(tqcm_client_context_t *ctx,
        nas_indication_register_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_INDICATION_REGISTER);
}

int tqcm_nas_get_signal_strength_send(tqcm_client_context_t *ctx,
        nas_signal_strength_request_t *input,
        nas_signal_strength_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_SIGNAL_STRENGTH);
}

int tqcm_nas_perform_network_scan_send(tqcm_client_context_t *ctx,
        nas_network_scan_mask_t *input,
        nas_network_scan_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_PERFORM_NETWORK_SCAN);
}

int tqcm_nas_get_serving_system_send(tqcm_client_context_t *ctx,
        nas_serving_system_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_SERVING_SYSTEM);
}

int tqcm_nas_get_home_network_send(tqcm_client_context_t *ctx,
        nas_home_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_HOME_NETWORK);
}

int tqcm_nas_get_technology_preference_send(tqcm_client_context_t *ctx,
        nas_get_network_preference_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_TECHNOLOGY_PREFERENCE);
}

int tqcm_nas_get_rf_band_info_send(tqcm_client_context_t *ctx,
        nas_rf_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_RF_BAND_INFO);
}

int tqcm_nas_set_system_selection_preference_send(tqcm_client_context_t *ctx,
        nas_set_system_selection_preference_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE);
}

int tqcm_nas_get_system_selection_preference_send(tqcm_client_context_t *ctx,
        nas_system_selection_pref_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE);
}

int tqcm_nas_get_cell_location_info_send(tqcm_client_context_t *ctx,
        nas_cell_location_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_CELL_LOCATION_INFO);
}

int tqcm_nas_get_plmn_name_send(tqcm_client_context_t *ctx,
        nas_plmn_info_t *input,
        nas_plmn_names_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_PLMN_NAME);
}

int tqcm_nas_get_sys_info_send(tqcm_client_context_t *ctx,
        nas_system_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_SYS_INFO);
}

int tqcm_nas_get_sig_info_send(tqcm_client_context_t *ctx,
        nas_signal_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_SIG_INFO);
}

int tqcm_nas_get_tx_rx_info_send(tqcm_client_context_t *ctx,
        nas_radio_if_t *input,
        nas_tx_rx_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_TX_RX_INFO);
}

int tqcm_nas_config_sig_info2_send(tqcm_client_context_t *ctx,
        nas_config_sig2_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_CONFIG_SIG_INFO2);
}

int tqcm_nas_limit_sys_info_ind_reporting_send(tqcm_client_context_t *ctx,
        nas_limit_sys_info_ind_reporting_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING);
}

int tqcm_nas_get_network_time_send(tqcm_client_context_t *ctx,
        nas_network_time_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_NETWORK_TIME);
}

int tqcm_nas_get_lte_cphy_ca_info_send(tqcm_client_context_t *ctx,
        nas_get_lte_cphy_ca_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_LTE_CPHY_CA_INFO);
}

int tqcm_nas_set_nr5g_sync_pulse_gen_send(tqcm_client_context_t *ctx,
        nas_set_nr5g_sync_pulse_gen_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_SET_NR5G_SYNC_PULSE_GEN);
}

int tqcm_nas_get_nr5g_rrc_utc_time_send(tqcm_client_context_t *ctx,
        nas_nr5g_rrc_utc_time_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_NAS,
            QMI_NAS_GET_NR5G_RRC_UTC_TIME);
}

QMI_SDK_error_t tqcm_nas_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_NAS_SET_EVENT_REPORT:
            ret = telit_nas_set_event_report_pack(req_ctx, req, req_len, input);
            break;
        case QMI_NAS_INDICATION_REGISTER:
            ret = telit_nas_indication_register_pack(req_ctx, req, req_len, input);
            break;
        case QMI_NAS_GET_SIGNAL_STRENGTH:
            ret = telit_nas_get_signal_strength_pack(req_ctx, req, req_len, input);
            break;
        case QMI_NAS_PERFORM_NETWORK_SCAN:
            ret = telit_nas_perform_network_scan_pack(req_ctx, req, req_len, input);
            break;
        case QMI_NAS_GET_SERVING_SYSTEM:
            ret = telit_nas_get_serving_system_pack(req_ctx, req, req_len);
            break;
        case QMI_NAS_GET_HOME_NETWORK:
            ret = telit_nas_get_home_network_pack(req_ctx, req, req_len);
            break;
        case QMI_NAS_GET_TECHNOLOGY_PREFERENCE:
            ret = telit_nas_get_technology_preference_pack(req_ctx, req, req_len);
            break;
        case QMI_NAS_GET_RF_BAND_INFO:
            ret = telit_nas_get_rf_band_info_pack(req_ctx, req, req_len);
            break;
        case QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE:
            ret = telit_nas_set_system_selection_preference_pack(req_ctx, req, req_len, input);
            break;
        case QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE:
            ret = telit_nas_get_system_selection_preference_pack(req_ctx, req, req_len);
            break;
        case QMI_NAS_GET_CELL_LOCATION_INFO:
            ret = telit_nas_get_cell_location_info_pack(req_ctx, req, req_len);
            break;
        case QMI_NAS_GET_PLMN_NAME:
            ret = telit_nas_get_plmn_name_pack(req_ctx, req, req_len, input);
            break;
        case QMI_NAS_GET_SYS_INFO:
            ret = telit_nas_get_sys_info_pack(req_ctx, req, req_len);
            break;
        case QMI_NAS_GET_SIG_INFO:
            ret = telit_nas_get_sig_info_pack(req_ctx, req, req_len);
            break;
        case QMI_NAS_GET_TX_RX_INFO:
            ret = telit_nas_get_tx_rx_info_pack(req_ctx, req, req_len, input);
            break;
        case QMI_NAS_CONFIG_SIG_INFO2:
            ret = telit_nas_config_sig_info2_pack(req_ctx, req, req_len, input);
            break;
        case QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING:
            ret = telit_nas_limit_sys_info_ind_reporting_pack(req_ctx, req, req_len, input);
            break;
        case QMI_NAS_GET_NETWORK_TIME:
            ret = telit_nas_get_network_time_pack(req_ctx, req, req_len);
            break;
        case QMI_NAS_GET_LTE_CPHY_CA_INFO:
            ret = telit_nas_get_lte_cphy_ca_info_pack(req_ctx, req, req_len);
            break;
        case QMI_NAS_SET_NR5G_SYNC_PULSE_GEN:
            ret = telit_nas_set_nr5g_sync_pulse_gen_pack(req_ctx, req, req_len, input);
            break;
        case QMI_NAS_GET_NR5G_RRC_UTC_TIME:
            ret = telit_nas_get_nr5g_rrc_utc_time_pack(req_ctx, req, req_len);
            break;
        default:
            RLOGE("%s: NAS msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}

QMI_SDK_error_t tqcm_nas_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_NAS_SET_EVENT_REPORT:
            ret = telit_nas_set_event_report_unpack(resp, resp_len);
            break;
        case QMI_NAS_INDICATION_REGISTER:
            ret = telit_nas_indication_register_unpack(resp, resp_len);
            break;
        case QMI_NAS_GET_SIGNAL_STRENGTH:
            ret = telit_nas_get_signal_strength_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_PERFORM_NETWORK_SCAN:
            ret = telit_nas_perform_network_scan_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_GET_SERVING_SYSTEM:
            ret = telit_nas_get_serving_system_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_GET_HOME_NETWORK:
            ret = telit_nas_get_home_network_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_GET_TECHNOLOGY_PREFERENCE:
            ret = telit_nas_get_technology_preference_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_GET_RF_BAND_INFO:
            ret = telit_nas_get_rf_band_info_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE:
            ret = telit_nas_set_system_selection_preference_unpack(resp, resp_len);
            break;
        case QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE:
            ret = telit_nas_get_system_selection_preference_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_GET_CELL_LOCATION_INFO:
            ret = telit_nas_get_cell_location_info_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_GET_PLMN_NAME:
            ret = telit_nas_get_plmn_name_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_GET_SYS_INFO:
            ret = telit_nas_get_sys_info_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_GET_SIG_INFO:
            ret = telit_nas_get_sig_info_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_GET_TX_RX_INFO:
            ret = telit_nas_get_tx_rx_info_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_CONFIG_SIG_INFO2:
            ret = telit_nas_config_sig_info2_unpack(resp, resp_len);
            break;
        case QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING:
            ret = telit_nas_limit_sys_info_ind_reporting_unpack(resp, resp_len);
            break;
        case QMI_NAS_GET_NETWORK_TIME:
            ret = telit_nas_get_network_time_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_GET_LTE_CPHY_CA_INFO:
            ret = telit_nas_get_lte_cphy_ca_info_unpack(resp, resp_len, out);
            break;
        case QMI_NAS_SET_NR5G_SYNC_PULSE_GEN:
            ret = telit_nas_set_nr5g_sync_pulse_gen_unpack(resp, resp_len);
            break;
        case QMI_NAS_GET_NR5G_RRC_UTC_TIME:
            ret = telit_nas_get_nr5g_rrc_utc_time_unpack(resp, resp_len, out);
            break;
        default:
            RLOGE("%s: NAS msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}
