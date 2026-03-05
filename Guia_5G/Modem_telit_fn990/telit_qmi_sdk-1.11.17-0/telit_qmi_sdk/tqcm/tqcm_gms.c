#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_gms.h"

int tqcm_gms_wds_3gpp_config_item_set_send(tqcm_client_context_t *ctx,
        wds_3gpp_config_item_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_WDS_3GPP_CONFIG_ITEM_SET);
}

int tqcm_gms_wds_3gpp_config_item_get_send(tqcm_client_context_t *ctx,
        wds_3gpp_config_item_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_WDS_3GPP_CONFIG_ITEM_GET);
}

int tqcm_gms_wds_create_profile_send(tqcm_client_context_t *ctx,
        wds_profile_t *input,
        wds_profile_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_WDS_CREATE_PROFILE);
}

int tqcm_gms_wds_preserved_apn_backup_send(tqcm_client_context_t *ctx,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, NULL,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_WDS_PRESERVED_APN_BACKUP);
}

int tqcm_gms_dms_get_dying_gasp_config_send(tqcm_client_context_t *ctx,
        dms_dying_gasp_cfg_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_DMS_GET_DYING_GASP_CFG);
}

int tqcm_gms_dms_set_dying_gasp_config_send(tqcm_client_context_t *ctx,
        dms_dying_gasp_cfg_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_DMS_SET_DYING_GASP_CFG);
}

int tqcm_gms_dms_get_dying_gasp_stat_send(tqcm_client_context_t *ctx,
        dms_dying_gasp_stat_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_DMS_GET_DYING_GASP_STAT);
}

int tqcm_gms_dms_clr_dying_gasp_stat_send(tqcm_client_context_t *ctx,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, NULL,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_DMS_CLR_DYING_GASP_STAT);
}

int tqcm_gms_dms_get_cust_features_send(tqcm_client_context_t *ctx,
        gms_dms_get_cust_features_v2_t *input,
        gms_dms_get_cust_features_v2_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_DMS_GET_CUST_FEATURES);
}

int tqcm_gms_dms_set_cust_features_send(tqcm_client_context_t *ctx,
        gms_dms_set_cust_features_v2_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_DMS_SET_CUST_FEATURES);
}

int tqcm_gms_dms_get_pc_info_send(tqcm_client_context_t *ctx,
        gms_dms_get_pc_info_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_DMS_GET_PC_INFO);
}

int tqcm_gms_dms_get_mtu_send(tqcm_client_context_t *ctx,
        gms_dms_get_mtu_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_DMS_GET_MTU);
}

int tqcm_gms_dms_set_mtu_send(tqcm_client_context_t *ctx,
        gms_dms_set_mtu_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_DMS_SET_MTU);
}

int tqcm_gms_dms_fwswitch_info_ex_send(tqcm_client_context_t *ctx,
        gms_dms_fwswitch_info_ex_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_DMS_FWSWITCH_INFO_EX);
}

int tqcm_gms_nas_modem_status_send(tqcm_client_context_t *ctx,
        nas_modem_status_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_NAS_MODEM_STATUS);
}

int tqcm_gms_nas_get_ltecqi_info_send(tqcm_client_context_t *ctx,
        gms_nas_lte_cqi_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_NAS_GET_LTECQI_INFO);
}

int tqcm_gms_nas_get_lte_scc_rx_info_send(tqcm_client_context_t *ctx,
        gms_nas_lte_scc_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_NAS_GET_LTE_SCC_RX_INFO);
}

int tqcm_gms_nas_ind_register_send(tqcm_client_context_t *ctx,
        gms_nas_indication_register_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_NAS_IND_REGISTER);
}

int tqcm_gms_nas_get_band_cap_send(tqcm_client_context_t *ctx,
        gms_nas_band_capability_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_NAS_GET_BAND_CAP);
}

int tqcm_gms_nas_get_endc_ca_info_send(tqcm_client_context_t *ctx,
        gms_nas_endc_ca_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_NAS_GET_ENDC_CA_INFO);
}

int tqcm_gms_nas_get_nr5g_ca_info_send(tqcm_client_context_t *ctx,
        gms_nas_nr5g_ca_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_NAS_GET_NR5G_CA_INFO);
}

int tqcm_gms_nas_set_mccblock_send(tqcm_client_context_t *ctx,
        gms_nas_mccblock_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_NAS_SET_MCCBLOCK);
}

int tqcm_gms_nas_get_mccblock_send(tqcm_client_context_t *ctx,
        gms_nas_mccblock_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_NAS_GET_MCCBLOCK);
}

int tqcm_gms_loc_get_auto_start_send(tqcm_client_context_t *ctx,
        gms_loc_get_auto_start_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_LOC_GET_AUTO_START);
}

int tqcm_gms_loc_set_auto_start_send(tqcm_client_context_t *ctx,
        gms_loc_set_auto_start_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_LOC_SET_AUTO_START);
}

int tqcm_gms_loc_set_nmea_type_send(tqcm_client_context_t *ctx,
        gms_loc_set_nmea_type_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_LOC_SET_NMEA_TYPE);
}

int tqcm_gms_loc_set_gnss_session_send(tqcm_client_context_t *ctx,
        gms_loc_gnss_session_mode_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_LOC_SET_GNSS_SESSION);
}

int tqcm_gms_loc_get_gnss_session_send(tqcm_client_context_t *ctx,
        gms_loc_gnss_session_mode_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_LOC_GET_GNSS_SESSION);
}

int tqcm_gms_loc_get_acq_position_send(tqcm_client_context_t *ctx,
        gms_loc_acquired_position_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GMS,
            QMI_GMS_LOC_GET_ACQ_POSITION);
}

QMI_SDK_error_t tqcm_gms_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    switch (msg_id) {
        case QMI_GMS_WDS_3GPP_CONFIG_ITEM_SET:
            ret = telit_gms_wds_3gpp_config_item_set_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GMS_WDS_3GPP_CONFIG_ITEM_GET:
            ret = telit_gms_wds_3gpp_config_item_get_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_WDS_CREATE_PROFILE:
            ret = telit_gms_wds_create_profile_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GMS_WDS_PRESERVED_APN_BACKUP:
            ret = telit_gms_wds_preserved_apn_backup_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_DMS_GET_DYING_GASP_CFG:
            ret = telit_gms_dms_get_dying_gasp_cfg_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_DMS_SET_DYING_GASP_CFG:
            ret = telit_gms_dms_set_dying_gasp_cfg_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GMS_DMS_GET_DYING_GASP_STAT:
            ret = telit_gms_dms_get_dying_gasp_stat_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_DMS_CLR_DYING_GASP_STAT:
            ret = telit_gms_dms_clr_dying_gasp_stat_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_DMS_GET_CUST_FEATURES:
            ret = telit_gms_dms_get_cust_features_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GMS_DMS_SET_CUST_FEATURES:
            ret = telit_gms_dms_set_cust_features_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GMS_DMS_GET_PC_INFO:
            ret = telit_gms_dms_get_pc_info_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_DMS_GET_MTU:
            ret = telit_gms_dms_get_mtu_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_DMS_SET_MTU:
            ret = telit_gms_dms_set_mtu_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GMS_DMS_FWSWITCH_INFO_EX:
            ret = telit_gms_dms_fwswitch_info_ex_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_NAS_MODEM_STATUS:
            ret = telit_gms_nas_modem_status_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_NAS_GET_LTECQI_INFO:
            ret = telit_gms_nas_get_ltecqi_info_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_NAS_GET_LTE_SCC_RX_INFO:
            ret = telit_gms_nas_get_lte_scc_rx_info_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_NAS_IND_REGISTER:
            ret = telit_gms_nas_ind_register_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GMS_NAS_GET_BAND_CAP:
            ret = telit_gms_nas_get_band_cap_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_NAS_GET_ENDC_CA_INFO:
            ret = telit_gms_nas_get_endc_ca_info_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_NAS_GET_NR5G_CA_INFO:
            ret = telit_gms_nas_get_nr5g_ca_info_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_NAS_SET_MCCBLOCK:
            ret = telit_gms_nas_set_mccblock_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GMS_NAS_GET_MCCBLOCK:
            ret = telit_gms_nas_get_mccblock_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_LOC_GET_AUTO_START:
            ret = telit_gms_loc_get_auto_start_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_LOC_SET_AUTO_START:
            ret = telit_gms_loc_set_auto_start_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GMS_LOC_SET_NMEA_TYPE:
            ret = telit_gms_loc_set_nmea_type_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GMS_LOC_SET_GNSS_SESSION:
            ret = telit_gms_loc_set_gnss_session_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GMS_LOC_GET_GNSS_SESSION:
            ret = telit_gms_loc_get_gnss_session_pack(req_ctx, req, req_len);
            break;
        case QMI_GMS_LOC_GET_ACQ_POSITION:
            ret = telit_gms_loc_get_acq_position_pack(req_ctx, req, req_len);
            break;
        default:
            RLOGE("%s: GMS msg_id %u not found", __FUNCTION__, msg_id);
    }
    return ret;
}

QMI_SDK_error_t tqcm_gms_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    switch (msg_id) {
        case QMI_GMS_WDS_3GPP_CONFIG_ITEM_SET:
            ret = telit_gms_wds_3gpp_config_item_set_unpack(resp, resp_len);
            break;
        case QMI_GMS_WDS_3GPP_CONFIG_ITEM_GET:
            ret = telit_gms_wds_3gpp_config_item_get_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_WDS_CREATE_PROFILE:
            ret = telit_gms_wds_create_profile_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_WDS_PRESERVED_APN_BACKUP:
            ret = telit_gms_wds_preserved_apn_backup_unpack(resp, resp_len);
            break;
        case QMI_GMS_DMS_GET_DYING_GASP_CFG:
            ret = telit_gms_dms_get_dying_gasp_cfg_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_DMS_SET_DYING_GASP_CFG:
            ret = telit_gms_dms_set_dying_gasp_cfg_unpack(resp, resp_len);
            break;
        case QMI_GMS_DMS_GET_DYING_GASP_STAT:
            ret = telit_gms_dms_get_dying_gasp_stat_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_DMS_CLR_DYING_GASP_STAT:
            ret = telit_gms_dms_clr_dying_gasp_stat_unpack(resp, resp_len);
            break;
        case QMI_GMS_DMS_GET_CUST_FEATURES:
            ret = telit_gms_dms_get_cust_features_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_DMS_SET_CUST_FEATURES:
            ret = telit_gms_dms_set_cust_features_unpack(resp, resp_len);
            break;
        case QMI_GMS_DMS_GET_PC_INFO:
            ret = telit_gms_dms_get_pc_info_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_DMS_GET_MTU:
            ret = telit_gms_dms_get_mtu_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_DMS_SET_MTU:
            ret = telit_gms_dms_set_mtu_unpack(resp, resp_len);
            break;
        case QMI_GMS_DMS_FWSWITCH_INFO_EX:
            ret = telit_gms_dms_fwswitch_info_ex_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_NAS_MODEM_STATUS:
            ret = telit_gms_nas_modem_status_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_NAS_GET_LTECQI_INFO:
            ret = telit_gms_nas_get_ltecqi_info_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_NAS_GET_LTE_SCC_RX_INFO:
            ret = telit_gms_nas_get_lte_scc_rx_info_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_NAS_IND_REGISTER:
            ret = telit_gms_nas_ind_register_unpack(resp, resp_len);
            break;
        case QMI_GMS_NAS_GET_BAND_CAP:
            ret = telit_gms_nas_get_band_cap_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_NAS_GET_ENDC_CA_INFO:
            ret = telit_gms_nas_get_endc_ca_info_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_NAS_GET_NR5G_CA_INFO:
            ret = telit_gms_nas_get_nr5g_ca_info_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_NAS_SET_MCCBLOCK:
            ret = telit_gms_nas_set_mccblock_unpack(resp, resp_len);
            break;
        case QMI_GMS_NAS_GET_MCCBLOCK:
            ret = telit_gms_nas_get_mccblock_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_LOC_GET_AUTO_START:
            ret = telit_gms_loc_get_auto_start_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_LOC_SET_AUTO_START:
            ret = telit_gms_loc_set_auto_start_unpack(resp, resp_len);
            break;
        case QMI_GMS_LOC_SET_NMEA_TYPE:
            ret = telit_gms_loc_set_nmea_type_unpack(resp, resp_len);
            break;
        case QMI_GMS_LOC_SET_GNSS_SESSION:
            ret = telit_gms_loc_set_gnss_session_unpack(resp, resp_len);
            break;
        case QMI_GMS_LOC_GET_GNSS_SESSION:
            ret = telit_gms_loc_get_gnss_session_unpack(resp, resp_len, out);
            break;
        case QMI_GMS_LOC_GET_ACQ_POSITION:
            ret = telit_gms_loc_get_acq_position_unpack(resp, resp_len, out);
            break;
        default:
            RLOGE("%s: GMS msg_id %u not found", __FUNCTION__, msg_id);
    }
    return ret;
}
