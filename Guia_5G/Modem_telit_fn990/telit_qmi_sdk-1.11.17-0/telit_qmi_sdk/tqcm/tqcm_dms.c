#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_dms.h"

int tqcm_dms_set_event_report_send(tqcm_client_context_t *ctx,
        dms_set_event_report_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_SET_EVENT_REPORT);
}

int tqcm_dms_get_device_cap_send(tqcm_client_context_t *ctx,
        dms_device_caps_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_GET_DEVICE_CAP);
}

int tqcm_dms_get_device_mfr_send(tqcm_client_context_t *ctx,
        dms_get_manufacturer_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_GET_DEVICE_MFR);
}

int tqcm_dms_get_device_model_id_send(tqcm_client_context_t *ctx,
        dms_get_model_id_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_GET_DEVICE_MODEL_ID);
}

int tqcm_dms_get_device_rev_id_send(tqcm_client_context_t *ctx,
        dms_get_fw_revision_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_GET_DEVICE_REV_ID);
}

int tqcm_dms_get_msisdn_send(tqcm_client_context_t *ctx,
        dms_voice_number_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_GET_MSISDN);
}

int tqcm_dms_get_device_serial_numbers_send(tqcm_client_context_t *ctx,
        dms_get_serial_numbers_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_GET_DEVICE_SERIAL_NUMBERS);
}

int tqcm_dms_get_device_hardware_rev_send(tqcm_client_context_t *ctx,
        dms_get_revision_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_GET_DEVICE_HARDWARE_REV);
}

int tqcm_dms_get_operating_mode_send(tqcm_client_context_t *ctx,
        dms_power_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_GET_OPERATING_MODE);
}

int tqcm_dms_set_operating_mode_send(tqcm_client_context_t *ctx,
        dms_power_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_SET_OPERATING_MODE);
}

int tqcm_dms_get_time_send(tqcm_client_context_t *ctx,
        dms_get_time_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_GET_TIME);
}

int tqcm_dms_get_band_capability_send(tqcm_client_context_t *ctx,
        dms_band_capabilities_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_GET_BAND_CAPABILITY);
}

int tqcm_dms_trigger_modem_activity_info_calculation_send(tqcm_client_context_t *ctx,
        dms_modem_activity_info_calculation_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION);
}

int tqcm_dms_get_modem_activity_info_send(tqcm_client_context_t *ctx,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, NULL,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_GET_MODEM_ACTIVITY_INFO);
}

int tqcm_dms_modem_device_power_info_send(tqcm_client_context_t *ctx,
        dms_modem_device_power_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_DEVICE_POWER_INFO);
}

int tqcm_dms_send_at_command_send(tqcm_client_context_t *ctx,
        dms_at_command_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_DMS,
            QMI_DMS_SEND_AT_COMMAND);
}

QMI_SDK_error_t tqcm_dms_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_DMS_SET_EVENT_REPORT:
            ret = telit_dms_set_event_report_pack(req_ctx, req, req_len, input);
            break;
        case QMI_DMS_GET_DEVICE_CAP:
            ret = telit_dms_get_device_cap_pack(req_ctx, req, req_len);
            break;
        case QMI_DMS_GET_DEVICE_MFR:
            ret = telit_dms_get_device_mfr_pack(req_ctx, req, req_len);
            break;
        case QMI_DMS_GET_DEVICE_MODEL_ID:
            ret = telit_dms_get_device_model_id_pack(req_ctx, req, req_len);
            break;
        case QMI_DMS_GET_DEVICE_REV_ID:
            ret = telit_dms_get_device_rev_id_pack(req_ctx, req, req_len);
            break;
        case QMI_DMS_GET_MSISDN:
            ret = telit_dms_get_msisdn_pack(req_ctx, req, req_len);
            break;
        case QMI_DMS_GET_DEVICE_SERIAL_NUMBERS:
            ret = telit_dms_get_device_serial_numbers_pack(req_ctx, req, req_len);
            break;
        case QMI_DMS_GET_DEVICE_HARDWARE_REV:
            ret = telit_dms_get_device_hardware_rev_pack(req_ctx, req, req_len);
            break;
        case QMI_DMS_GET_OPERATING_MODE:
            ret = telit_dms_get_operating_mode_pack(req_ctx, req, req_len);
            break;
        case QMI_DMS_SET_OPERATING_MODE:
            ret = telit_dms_set_operating_mode_pack(req_ctx, req, req_len, input);
            break;
        case QMI_DMS_GET_TIME:
            ret = telit_dms_get_time_pack(req_ctx, req, req_len);
            break;
        case QMI_DMS_GET_BAND_CAPABILITY:
            ret = telit_dms_get_band_capability_pack(req_ctx, req, req_len);
            break;
        case QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION:
            ret = telit_dms_trigger_modem_activity_info_calculation_pack(req_ctx, req, req_len, input);
            break;
        case QMI_DMS_GET_MODEM_ACTIVITY_INFO:
            ret = telit_dms_get_modem_activity_info_pack(req_ctx, req, req_len);
            break;
        case QMI_DMS_DEVICE_POWER_INFO:
            ret = telit_dms_device_power_info_pack(req_ctx, req, req_len, input);
            break;
        case QMI_DMS_SEND_AT_COMMAND:
            ret = telit_dms_send_at_command_pack(req_ctx, req, req_len, input);
            break;
        default:
            RLOGE("%s: DMS msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}

QMI_SDK_error_t tqcm_dms_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_DMS_SET_EVENT_REPORT:
            ret = telit_dms_set_event_report_unpack(resp, resp_len);
            break;
        case QMI_DMS_GET_DEVICE_CAP:
            ret = telit_dms_get_device_cap_unpack(resp, resp_len, out);
            break;
        case QMI_DMS_GET_DEVICE_MFR:
            ret = telit_dms_get_device_mfr_unpack(resp, resp_len, out);
            break;
        case QMI_DMS_GET_DEVICE_MODEL_ID:
            ret = telit_dms_get_device_model_id_unpack(resp, resp_len, out);
            break;
        case QMI_DMS_GET_DEVICE_REV_ID:
            ret = telit_dms_get_device_rev_id_unpack(resp, resp_len, out);
            break;
        case QMI_DMS_GET_MSISDN:
            ret = telit_dms_get_msisdn_unpack(resp, resp_len, out);
            break;
        case QMI_DMS_GET_DEVICE_SERIAL_NUMBERS:
            ret = telit_dms_get_device_serial_numbers_unpack(resp, resp_len, out);
            break;
        case QMI_DMS_GET_DEVICE_HARDWARE_REV:
            ret = telit_dms_get_device_hardware_rev_unpack(resp, resp_len, out);
            break;
        case QMI_DMS_GET_OPERATING_MODE:
            ret = telit_dms_get_operating_mode_unpack(resp, resp_len, out);
            break;
        case QMI_DMS_SET_OPERATING_MODE:
            ret = telit_dms_set_operating_mode_unpack(resp, resp_len);
            break;
        case QMI_DMS_GET_TIME:
            ret = telit_dms_get_time_unpack(resp, resp_len, out);
            break;
        case QMI_DMS_GET_BAND_CAPABILITY:
            ret = telit_dms_get_band_capability_unpack(resp, resp_len, out);
            break;
        case QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION:
            ret = telit_dms_trigger_modem_activity_info_calculation_unpack(resp, resp_len);
            break;
        case QMI_DMS_GET_MODEM_ACTIVITY_INFO:
            ret = telit_dms_get_modem_activity_info_unpack(resp, resp_len);
            break;
        case QMI_DMS_DEVICE_POWER_INFO:
            ret = telit_dms_device_power_info_unpack(resp, resp_len);
            break;
        case QMI_DMS_SEND_AT_COMMAND:
            ret = telit_dms_send_at_command_unpack(resp, resp_len);
            break;
        default:
            RLOGE("%s: DMS msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}
