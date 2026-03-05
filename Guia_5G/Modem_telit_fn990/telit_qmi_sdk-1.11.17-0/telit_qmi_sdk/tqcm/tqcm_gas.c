#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_gas.h"

int tqcm_gas_dms_set_usb_cfg_send(tqcm_client_context_t *ctx,
        gas_dms_usb_cfg_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_USB_CFG_SET);
}

int tqcm_gas_dms_get_usb_cfg_send(tqcm_client_context_t *ctx,
        gas_dms_usb_cfg_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_USB_CFG_GET);
}

int tqcm_gas_dms_active_fw_send(tqcm_client_context_t *ctx,
        gas_dms_active_fw_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_ACTIVE_FW);
}

int tqcm_gas_dms_get_fw_send(tqcm_client_context_t *ctx,
        gas_dms_get_fw_t *input,
        gas_dms_fw_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_GET_FW);
}

int tqcm_gas_dms_clear_fw_send(tqcm_client_context_t *ctx,
        gas_dms_get_fw_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_CLEAR_FW);
}

int tqcm_gas_dms_reset_info_ind_reg_send(tqcm_client_context_t *ctx,
        gas_dms_reset_info_indication_register_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_RESET_INFO_IND_REG);
}

int tqcm_gas_dms_get_reset_info_send(tqcm_client_context_t *ctx,
        gas_dms_reset_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_GET_RESET_INFO);
}

int tqcm_gas_dms_set_crash_action_send(tqcm_client_context_t *ctx,
        gas_dms_crash_action_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_SET_CRASH_ACTION);
}

int tqcm_gas_dms_get_crash_info_send(tqcm_client_context_t *ctx,
        gas_dms_get_crash_info_t *input,
        gas_dms_get_crash_info_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_GET_CRASH_INFO);
}

int tqcm_gas_dms_errgen_send(tqcm_client_context_t *ctx,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, NULL,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_ERRGEN);
}

int tqcm_gas_dms_get_host_fwver_send(tqcm_client_context_t *ctx,
        gas_dms_versions_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_GET_HOST_FWVER);
}

int tqcm_gas_dms_get_oem_fwver_send(tqcm_client_context_t *ctx,
        gas_dms_get_oem_fwver_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_GET_OEM_FWVER);
}

int tqcm_gas_dms_get_crash_action_send(tqcm_client_context_t *ctx,
        gas_dms_crash_action_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_GET_CRASH_ACTION);
}

int tqcm_gas_dms_set_usb_switch_send(tqcm_client_context_t *ctx,
        gas_dms_usb_switch_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_SET_USB_SWITCH);
}

int tqcm_gas_dms_get_usb_switch_send(tqcm_client_context_t *ctx,
        gas_dms_usb_switch_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_GET_USB_SWITCH);
}

int tqcm_gas_dms_set_usb_ext_send(tqcm_client_context_t *ctx,
        gas_dms_usb_ext_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_SET_USB_EXT);
}

int tqcm_gas_dms_get_usb_ext_send(tqcm_client_context_t *ctx,
        gas_dms_usb_ext_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_DMS_GET_USB_EXT);
}

int tqcm_gas_psm_get_wdisa_cfg_send(tqcm_client_context_t *ctx,
        gas_psm_wdisa_cfg_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_PSM_GET_WDISA_CFG);
}

int tqcm_gas_psm_set_wdisa_cfg_send(tqcm_client_context_t *ctx,
        gas_psm_wdisa_cfg_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_PSM_SET_WDISA_CFG);
}

int tqcm_gas_log_set_redirect_send(tqcm_client_context_t *ctx,
        gas_log_redirect_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_GAS,
            QMI_GAS_LOG_SET_REDIRECT);
}

QMI_SDK_error_t tqcm_gas_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_GAS_DMS_USB_CFG_SET:
            ret = telit_gas_dms_usb_cfg_set_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GAS_DMS_USB_CFG_GET:
            ret = telit_gas_dms_usb_cfg_get_pack(req_ctx, req, req_len);
            break;
        case QMI_GAS_DMS_ACTIVE_FW:
            ret = telit_gas_dms_active_fw_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GAS_DMS_GET_FW:
            ret = telit_gas_dms_get_fw_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GAS_DMS_CLEAR_FW:
            ret = telit_gas_dms_clear_fw_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GAS_DMS_RESET_INFO_IND_REG:
            ret = telit_gas_dms_reset_info_ind_reg_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GAS_DMS_GET_RESET_INFO:
            ret = telit_gas_dms_get_reset_info_pack(req_ctx, req, req_len);
            break;
        case QMI_GAS_DMS_SET_CRASH_ACTION:
            ret = telit_gas_dms_set_crash_action_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GAS_DMS_GET_CRASH_INFO:
            ret = telit_gas_dms_get_crash_info_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GAS_DMS_ERRGEN:
            ret = telit_gas_dms_errgen_pack(req_ctx, req, req_len);
            break;
        case QMI_GAS_DMS_GET_HOST_FWVER:
            ret = telit_gas_dms_get_host_fwver_pack(req_ctx, req, req_len);
            break;
        case QMI_GAS_DMS_GET_OEM_FWVER:
            ret = telit_gas_dms_get_oem_fwver_pack(req_ctx, req, req_len);
            break;
        case QMI_GAS_DMS_GET_CRASH_ACTION:
            ret = telit_gas_dms_get_crash_action_pack(req_ctx, req, req_len);
            break;
        case QMI_GAS_DMS_SET_USB_SWITCH:
            ret = telit_gas_dms_set_usb_switch_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GAS_DMS_GET_USB_SWITCH:
            ret = telit_gas_dms_get_usb_switch_pack(req_ctx, req, req_len);
            break;
        case QMI_GAS_DMS_SET_USB_EXT:
            ret = telit_gas_dms_set_usb_ext_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GAS_DMS_GET_USB_EXT:
            ret = telit_gas_dms_get_usb_ext_pack(req_ctx, req, req_len);
            break;
        case QMI_GAS_PSM_GET_WDISA_CFG:
            ret = telit_gas_psm_get_wdisa_cfg_pack(req_ctx, req, req_len);
            break;
        case QMI_GAS_PSM_SET_WDISA_CFG:
            ret = telit_gas_psm_set_wdisa_cfg_pack(req_ctx, req, req_len, input);
            break;
        case QMI_GAS_LOG_SET_REDIRECT:
            ret = telit_gas_log_set_redirect_pack(req_ctx, req, req_len, input);
            break;
        default:
            RLOGE("%s: GAS msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}

QMI_SDK_error_t tqcm_gas_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_GAS_DMS_USB_CFG_SET:
            ret = telit_gas_dms_usb_cfg_set_unpack(resp, resp_len);
            break;
        case QMI_GAS_DMS_USB_CFG_GET:
            ret = telit_gas_dms_usb_cfg_get_unpack(resp, resp_len, out);
            break;
        case QMI_GAS_DMS_ACTIVE_FW:
            ret = telit_gas_dms_active_fw_unpack(resp, resp_len);
            break;
        case QMI_GAS_DMS_GET_FW:
            ret = telit_gas_dms_get_fw_unpack(resp, resp_len, out);
            break;
        case QMI_GAS_DMS_CLEAR_FW:
            ret = telit_gas_dms_clear_fw_unpack(resp, resp_len);
            break;
        case QMI_GAS_DMS_RESET_INFO_IND_REG:
            ret = telit_gas_dms_reset_info_ind_reg_unpack(resp, resp_len);
            break;
        case QMI_GAS_DMS_GET_RESET_INFO:
            ret = telit_gas_dms_get_reset_info_unpack(resp, resp_len, out);
            break;
        case QMI_GAS_DMS_SET_CRASH_ACTION:
            ret = telit_gas_dms_set_crash_action_unpack(resp, resp_len);
            break;
        case QMI_GAS_DMS_GET_CRASH_INFO:
            ret = telit_gas_dms_get_crash_info_unpack(resp, resp_len, out);
            break;
        case QMI_GAS_DMS_ERRGEN:
            ret = telit_gas_dms_errgen_unpack(resp, resp_len);
            break;
        case QMI_GAS_DMS_GET_HOST_FWVER:
            ret = telit_gas_dms_get_host_fwver_unpack(resp, resp_len, out);
            break;
        case QMI_GAS_DMS_GET_OEM_FWVER:
            ret = telit_gas_dms_get_oem_fwver_unpack(resp, resp_len, out);
            break;
        case QMI_GAS_DMS_GET_CRASH_ACTION:
            ret = telit_gas_dms_get_crash_action_unpack(resp, resp_len, out);
            break;
        case QMI_GAS_DMS_SET_USB_SWITCH:
            ret = telit_gas_dms_set_usb_switch_unpack(resp, resp_len);
            break;
        case QMI_GAS_DMS_GET_USB_SWITCH:
            ret = telit_gas_dms_get_usb_switch_unpack(resp, resp_len, out);
            break;
        case QMI_GAS_DMS_SET_USB_EXT:
            ret = telit_gas_dms_set_usb_ext_unpack(resp, resp_len);
            break;
        case QMI_GAS_DMS_GET_USB_EXT:
            ret = telit_gas_dms_get_usb_ext_unpack(resp, resp_len, out);
            break;
        case QMI_GAS_PSM_GET_WDISA_CFG:
            ret = telit_gas_psm_get_wdisa_cfg_unpack(resp, resp_len, out);
            break;
        case QMI_GAS_PSM_SET_WDISA_CFG:
            ret = telit_gas_psm_set_wdisa_cfg_unpack(resp, resp_len);
            break;
        case QMI_GAS_LOG_SET_REDIRECT:
            ret = telit_gas_log_set_redirect_unpack(resp, resp_len);
            break;
        default:
            RLOGE("%s: GAS msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}
