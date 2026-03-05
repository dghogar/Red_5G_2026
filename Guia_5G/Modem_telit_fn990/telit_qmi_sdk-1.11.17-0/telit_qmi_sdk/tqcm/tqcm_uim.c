#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_uim.h"

int tqcm_uim_read_transparent_send(tqcm_client_context_t *ctx,
        uim_read_file_t *input,
        uim_read_file_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_READ_TRANSPARENT);
}

int tqcm_uim_read_record_send(tqcm_client_context_t *ctx,
        uim_read_record_t *input,
        uim_read_record_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_READ_RECORD);
}

int tqcm_uim_get_file_attributes_send(tqcm_client_context_t *ctx,
        uim_read_file_t *input,
        uim_read_file_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_GET_FILE_ATTRIBUTES);
}

int tqcm_uim_set_pin_protection_send(tqcm_client_context_t *ctx,
        uim_pin_protection_info_t *input,
        uim_pin_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_SET_PIN_PROTECTION);
}

int tqcm_uim_verify_pin_send(tqcm_client_context_t *ctx,
        uim_verify_pin_info_t *input,
        uim_pin_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_VERIFY_PIN);
}

int tqcm_uim_unblock_pin_send(tqcm_client_context_t *ctx,
        uim_unblock_pin_info_t *input,
        uim_pin_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_UNBLOCK_PIN);
}

int tqcm_uim_change_pin_send(tqcm_client_context_t *ctx,
        uim_change_pin_info_t *input,
        uim_pin_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_CHANGE_PIN);
}

int tqcm_uim_event_reg_send(tqcm_client_context_t *ctx,
        uim_event_reg_info_t *input,
        uim_event_reg_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_EVENT_REG);
}

int tqcm_uim_get_card_status_send(tqcm_client_context_t *ctx,
        uim_card_status_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_GET_CARD_STATUS);
}

int tqcm_uim_power_down_send(tqcm_client_context_t *ctx,
        uim_power_slot_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_POWER_DOWN);
}

int tqcm_uim_power_up_send(tqcm_client_context_t *ctx,
        uim_power_slot_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_POWER_UP);
}

int tqcm_uim_change_provisioning_session_send(tqcm_client_context_t *ctx,
        uim_change_provisioning_session_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_CHANGE_PROVISIONING_SESSION);
}

int tqcm_uim_send_apdu_send(tqcm_client_context_t *ctx,
        uim_send_apdu_info_t *input,
        uim_send_apdu_response_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_SEND_APDU);
}

int tqcm_uim_logical_channel_send(tqcm_client_context_t *ctx,
        uim_logical_channel_info_t *input,
        uim_logical_channel_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_LOGICAL_CHANNEL);
}

int tqcm_uim_switch_slot_send(tqcm_client_context_t *ctx,
        uim_switch_slot_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_SWITCH_SLOT);
}

int tqcm_uim_get_slots_status_send(tqcm_client_context_t *ctx,
        uim_slot_status_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_UIM,
            QMI_UIM_GET_SLOTS_STATUS);
}

QMI_SDK_error_t tqcm_uim_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_UIM_READ_TRANSPARENT:
            ret = telit_uim_read_transparent_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_READ_RECORD:
            ret = telit_uim_read_record_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_GET_FILE_ATTRIBUTES:
            ret = telit_uim_get_file_attributes_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_SET_PIN_PROTECTION:
            ret = telit_uim_set_pin_protection_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_VERIFY_PIN:
            ret = telit_uim_verify_pin_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_UNBLOCK_PIN:
            ret = telit_uim_unblock_pin_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_CHANGE_PIN:
            ret = telit_uim_change_pin_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_EVENT_REG:
            ret = telit_uim_event_reg_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_GET_CARD_STATUS:
            ret = telit_uim_get_card_status_pack(req_ctx, req, req_len);
            break;
        case QMI_UIM_POWER_DOWN:
            ret = telit_uim_power_down_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_POWER_UP:
            ret = telit_uim_power_up_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_CHANGE_PROVISIONING_SESSION:
            ret = telit_uim_change_provisioning_session_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_SEND_APDU:
            ret = telit_uim_send_apdu_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_LOGICAL_CHANNEL:
            ret = telit_uim_logical_channel_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_SWITCH_SLOT:
            ret = telit_uim_switch_slot_pack(req_ctx, req, req_len, input);
            break;
        case QMI_UIM_GET_SLOTS_STATUS:
            ret = telit_uim_get_slots_status_pack(req_ctx, req, req_len);
            break;
        default:
            RLOGE("%s: UIM msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}

QMI_SDK_error_t tqcm_uim_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_UIM_READ_TRANSPARENT:
            ret = telit_uim_read_transparent_unpack(resp, resp_len, out);
            break;
        case QMI_UIM_READ_RECORD:
            ret = telit_uim_read_record_unpack(resp, resp_len, out);
            break;
        case QMI_UIM_GET_FILE_ATTRIBUTES:
            ret = telit_uim_get_file_attributes_unpack(resp, resp_len, out);
            break;
        case QMI_UIM_SET_PIN_PROTECTION:
            ret = telit_uim_set_pin_protection_unpack(resp, resp_len, out);
            break;
        case QMI_UIM_VERIFY_PIN:
            ret = telit_uim_verify_pin_unpack(resp, resp_len, out);
            break;
        case QMI_UIM_UNBLOCK_PIN:
            ret = telit_uim_unblock_pin_unpack(resp, resp_len, out);
            break;
        case QMI_UIM_CHANGE_PIN:
            ret = telit_uim_change_pin_unpack(resp, resp_len, out);
            break;
        case QMI_UIM_EVENT_REG:
            ret = telit_uim_event_reg_unpack(resp, resp_len, out);
            break;
        case QMI_UIM_GET_CARD_STATUS:
            ret = telit_uim_get_card_status_unpack(resp, resp_len, out);
            break;
        case QMI_UIM_POWER_DOWN:
            ret = telit_uim_power_down_unpack(resp, resp_len);
            break;
        case QMI_UIM_POWER_UP:
            ret = telit_uim_power_up_unpack(resp, resp_len);
            break;
        case QMI_UIM_CHANGE_PROVISIONING_SESSION:
            ret = telit_uim_change_provisioning_session_unpack(resp, resp_len);
            break;
        case QMI_UIM_SEND_APDU:
            ret = telit_uim_send_apdu_unpack(resp, resp_len, out);
            break;
        case QMI_UIM_LOGICAL_CHANNEL:
            ret = telit_uim_logical_channel_unpack(resp, resp_len, out);
            break;
        case QMI_UIM_SWITCH_SLOT:
            ret = telit_uim_switch_slot_unpack(resp, resp_len);
            break;
        case QMI_UIM_GET_SLOTS_STATUS:
            ret = telit_uim_get_slots_status_unpack(resp, resp_len, out);
            break;
        default:
            RLOGE("%s: UIM msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}
