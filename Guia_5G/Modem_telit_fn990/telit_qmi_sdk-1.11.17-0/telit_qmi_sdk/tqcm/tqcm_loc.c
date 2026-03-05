#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_loc.h"

int tqcm_loc_reg_events_send(tqcm_client_context_t *ctx,
        loc_event_register_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_LOC,
            QMI_LOC_REG_EVENTS);
}

int tqcm_loc_start_send(tqcm_client_context_t *ctx,
        loc_start_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_LOC,
            QMI_LOC_START);
}

int tqcm_loc_stop_send(tqcm_client_context_t *ctx,
        loc_stop_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_LOC,
            QMI_LOC_STOP);
}

int tqcm_loc_inject_utc_time_send(tqcm_client_context_t *ctx,
        loc_utc_time_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_LOC,
            QMI_LOC_INJECT_UTC_TIME);
}

int tqcm_loc_inject_position_send(tqcm_client_context_t *ctx,
        loc_position_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_LOC,
            QMI_LOC_INJECT_POSITION);
}

int tqcm_loc_set_nmea_types_send(tqcm_client_context_t *ctx,
        loc_nmea_types_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_LOC,
            QMI_LOC_SET_NMEA_TYPES);
}

int tqcm_loc_delete_assist_data_send(tqcm_client_context_t *ctx,
        loc_delete_assist_data_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_LOC,
            QMI_LOC_DELETE_ASSIST_DATA);
}

int tqcm_loc_set_operation_mode_send(tqcm_client_context_t *ctx,
        loc_operation_mode_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_LOC,
            QMI_LOC_SET_OPERATION_MODE);
}

int tqcm_loc_register_master_client_send(tqcm_client_context_t *ctx,
        loc_master_client_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token)
{
    return tqcm_generic_request(ctx, input, NULL,
            sdk_error, cb, token,
            QMI_SVC_LOC,
            QMI_LOC_REGISTER_MASTER_CLIENT);
}

QMI_SDK_error_t tqcm_loc_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_LOC_REG_EVENTS:
            ret = telit_loc_reg_events_pack(req_ctx, req, req_len, input);
            break;
        case QMI_LOC_START:
            ret = telit_loc_start_pack(req_ctx, req, req_len, input);
            break;
        case QMI_LOC_STOP:
            ret = telit_loc_stop_pack(req_ctx, req, req_len, input);
            break;
        case QMI_LOC_INJECT_UTC_TIME:
            ret = telit_loc_inject_utc_time_pack(req_ctx, req, req_len, input);
            break;
        case QMI_LOC_INJECT_POSITION:
            ret = telit_loc_inject_position_pack(req_ctx, req, req_len, input);
            break;
        case QMI_LOC_DELETE_ASSIST_DATA:
            ret = telit_loc_delete_assist_data_pack(req_ctx, req, req_len, input);
            break;
        case QMI_LOC_SET_OPERATION_MODE:
            ret = telit_loc_set_operation_mode_pack(req_ctx, req, req_len, input);
            break;
        case QMI_LOC_SET_NMEA_TYPES:
            ret = telit_loc_set_nmea_types_pack(req_ctx, req, req_len, input);
            break;
        case QMI_LOC_REGISTER_MASTER_CLIENT:
            ret = telit_loc_reg_master_client_pack(req_ctx, req, req_len, input);
            break;
        default:
            RLOGE("%s: LOC msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}

QMI_SDK_error_t tqcm_loc_unpack(void *resp,
        uint16_t resp_len,
        void *out __unused,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (msg_id) {
        case QMI_LOC_REG_EVENTS:
            ret = telit_loc_reg_events_unpack(resp, resp_len);
            break;
        case QMI_LOC_START:
            ret = telit_loc_start_unpack(resp, resp_len);
            break;
        case QMI_LOC_STOP:
            ret = telit_loc_stop_unpack(resp, resp_len);
            break;
        case QMI_LOC_INJECT_UTC_TIME:
            ret = telit_loc_inject_utc_time_unpack(resp, resp_len);
            break;
        case QMI_LOC_INJECT_POSITION:
            ret = telit_loc_inject_position_unpack(resp, resp_len);
            break;
        case QMI_LOC_SET_NMEA_TYPES:
            ret = telit_loc_set_nmea_types_unpack(resp, resp_len);
            break;
        case QMI_LOC_DELETE_ASSIST_DATA:
            ret = telit_loc_delete_assist_data_unpack(resp, resp_len);
            break;
        case QMI_LOC_SET_OPERATION_MODE:
            ret = telit_loc_set_operation_mode_unpack(resp, resp_len);
            break;
        case QMI_LOC_REGISTER_MASTER_CLIENT:
            ret = telit_loc_reg_master_client_unpack(resp, resp_len);
            break;
        default:
            RLOGE("%s: LOC msg_id %u not found", __FUNCTION__, msg_id);
    }

    return ret;
}
