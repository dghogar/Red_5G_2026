#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_voice.h"

int tqcm_voice_indication_register_send(tqcm_client_context_t *ctx,
        voice_indications_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_INDICATION_REGISTER);
}

int tqcm_voice_dial_call_send(tqcm_client_context_t *ctx,
        voice_call_number_t *input,
        voice_call_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_DIAL_CALL);
}

int tqcm_voice_end_call_send(tqcm_client_context_t *ctx,
        voice_call_t *input,
        voice_call_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_END_CALL);
}

int tqcm_voice_answer_call_send(tqcm_client_context_t *ctx,
        voice_answer_call_t *input,
        voice_call_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_ANSWER_CALL);
}

int tqcm_voice_burst_dtmf_send(tqcm_client_context_t *ctx,
        voice_burst_dtmf_input_t *input,
        voice_dtmf_response_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_BURST_DTMF);
}

int tqcm_voice_start_cont_dtmf_send(tqcm_client_context_t *ctx,
        voice_cont_dtmf_input_t *input,
        voice_dtmf_response_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_START_CONT_DTMF);
}

int tqcm_voice_stop_cont_dtmf_send(tqcm_client_context_t *ctx,
        voice_cont_dtmf_input_t *input,
        voice_dtmf_response_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_STOP_CONT_DTMF);
}

int tqcm_voice_get_all_call_info_send(tqcm_client_context_t *ctx,
        voice_get_all_call_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_GET_ALL_CALL_INFO);
}

int tqcm_voice_manage_calls_send(tqcm_client_context_t *ctx,
        voice_manage_call_request_t *input,
        voice_manage_call_response_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_MANAGE_CALLS);
}

int tqcm_voice_set_sups_service_send(tqcm_client_context_t *ctx,
        voice_set_sup_service_input_t *input,
        voice_set_sup_service_response_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_SET_SUPS_SERVICE);
}

int tqcm_voice_get_call_waiting_send(tqcm_client_context_t *ctx,
        voice_get_call_service_input_t *input,
        voice_get_call_waiting_response_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_GET_CALL_WAITING);
}

int tqcm_voice_get_call_barring_send(tqcm_client_context_t *ctx,
        voice_get_call_service_input_t *input,
        voice_call_barring_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_GET_CALL_BARRING);
}

int tqcm_voice_get_clip_send(tqcm_client_context_t *ctx,
        voice_get_calling_line_response_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_GET_CLIP);
}

int tqcm_voice_get_clir_send(tqcm_client_context_t *ctx,
        voice_get_calling_line_response_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, NULL, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_GET_CLIR);
}

int tqcm_voice_get_call_forwarding_send(tqcm_client_context_t *ctx,
        voice_get_call_service_input_t *input,
        voice_call_forwarding_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_GET_CALL_FORWARDING);
}

int tqcm_voice_set_call_barring_password_send(tqcm_client_context_t *ctx,
        voice_call_barring_input_t *input,
        voice_call_barring_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, out,
            sdk_error, cb, token,
            QMI_SVC_VOICE,
            QMI_VOICE_SET_CALL_BARRING_PASSWORD);
}

QMI_SDK_error_t tqcm_voice_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_VOICE_INDICATION_REGISTER:
            ret = telit_voice_indication_register_pack(req_ctx, req, req_len, input);
            break;
        case QMI_VOICE_DIAL_CALL:
            ret = telit_voice_dial_call_pack(req_ctx, req, req_len, input);
            break;
        case QMI_VOICE_END_CALL:
            ret = telit_voice_end_call_pack(req_ctx, req, req_len, input);
            break;
        case QMI_VOICE_ANSWER_CALL:
            ret = telit_voice_answer_call_pack(req_ctx, req, req_len, input);
            break;
        case QMI_VOICE_BURST_DTMF:
            ret = telit_voice_burst_dtmf_pack(req_ctx, req, req_len, input);
            break;
        case QMI_VOICE_START_CONT_DTMF:
            ret = telit_voice_start_cont_dtmf_pack(req_ctx, req, req_len, input);
            break;
        case QMI_VOICE_STOP_CONT_DTMF:
            ret = telit_voice_stop_cont_dtmf_pack(req_ctx, req, req_len, input);
            break;
        case QMI_VOICE_GET_ALL_CALL_INFO:
            ret = telit_voice_get_all_call_info_pack(req_ctx, req, req_len);
            break;
        case QMI_VOICE_MANAGE_CALLS:
            ret = telit_voice_manage_calls_pack(req_ctx, req, req_len, input);
            break;
        case QMI_VOICE_SET_SUPS_SERVICE:
            ret = telit_voice_set_sups_service_pack(req_ctx, req, req_len, input);
            break;
        case QMI_VOICE_GET_CALL_WAITING:
            ret = telit_voice_get_call_waiting_pack(req_ctx, req, req_len, input);
            break;
        case QMI_VOICE_GET_CALL_BARRING:
            ret = telit_voice_get_call_barring_pack(req_ctx, req, req_len, input);
            break;
        case QMI_VOICE_GET_CLIP:
            ret = telit_voice_get_clip_pack(req_ctx, req, req_len);
            break;
        case QMI_VOICE_GET_CLIR:
            ret = telit_voice_get_clir_pack(req_ctx, req, req_len);
            break;
        case QMI_VOICE_GET_CALL_FORWARDING:
            ret = telit_voice_get_call_forwarding_pack(req_ctx, req, req_len, input);
            break;
        case QMI_VOICE_SET_CALL_BARRING_PASSWORD:
            ret = telit_voice_set_call_barring_password_pack(req_ctx, req, req_len, input);
            break;
        default:
            RLOGE("%s: VOICE msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}

QMI_SDK_error_t tqcm_voice_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_VOICE_INDICATION_REGISTER:
            ret = telit_voice_indication_register_unpack(resp, resp_len);
            break;
        case QMI_VOICE_DIAL_CALL:
            ret = telit_voice_dial_call_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_END_CALL:
            ret = telit_voice_end_call_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_ANSWER_CALL:
            ret = telit_voice_answer_call_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_BURST_DTMF:
            ret = telit_voice_burst_dtmf_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_START_CONT_DTMF:
            ret = telit_voice_start_cont_dtmf_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_STOP_CONT_DTMF:
            ret = telit_voice_stop_cont_dtmf_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_GET_ALL_CALL_INFO:
            ret = telit_voice_get_all_call_info_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_MANAGE_CALLS:
            ret = telit_voice_manage_calls_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_SET_SUPS_SERVICE:
            ret = telit_voice_set_sups_service_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_GET_CALL_WAITING:
            ret = telit_voice_get_call_waiting_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_GET_CALL_BARRING:
            ret = telit_voice_get_call_barring_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_GET_CLIP:
            ret = telit_voice_get_clip_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_GET_CLIR:
            ret = telit_voice_get_clir_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_GET_CALL_FORWARDING:
            ret = telit_voice_get_call_forwarding_unpack(resp, resp_len, out);
            break;
        case QMI_VOICE_SET_CALL_BARRING_PASSWORD:
            ret = telit_voice_set_call_barring_password_unpack(resp, resp_len, out);
            break;
        default:
            RLOGE("%s: VOICE msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}
