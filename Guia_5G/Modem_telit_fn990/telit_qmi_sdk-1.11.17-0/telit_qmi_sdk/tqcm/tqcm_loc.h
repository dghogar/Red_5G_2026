#ifndef TQCM_LOC_H
#define TQCM_LOC_H

/**
 * @file tqcm_loc.h
 * @brief LOC requests
 */

#include <telit_sdk.h>
#include <telit_sdk_loc.h>
#include "tqcm.h"

/**
 * @brief Send a QMI_LOC_REG_EVENTS request
 *
 * This function sends a QMI_LOC_REG_EVENTS request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #loc_event_register_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_loc_reg_events_send(tqcm_client_context_t *ctx,
        loc_event_register_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_LOC_START request
 *
 * This function sends a QMI_LOC_START request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #loc_start_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_loc_start_send(tqcm_client_context_t *ctx,
        loc_start_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_LOC_STOP request
 *
 * This function sends a QMI_LOC_STOP request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #loc_stop_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_loc_stop_send(tqcm_client_context_t *ctx,
        loc_stop_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_LOC_INJECT_UTC_TIME request
 *
 * This function sends a QMI_LOC_INJECT_UTC_TIME request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #loc_utc_time_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_loc_inject_utc_time_send(tqcm_client_context_t *ctx,
        loc_utc_time_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_LOC_INJECT_POSITION request
 *
 * This function sends a QMI_LOC_INJECT_POSITION request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #loc_position_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_loc_inject_position_send(tqcm_client_context_t *ctx,
        loc_position_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_LOC_SET_NMEA_TYPES request
 *
 * This function sends a QMI_LOC_SET_NMEA_TYPES request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #loc_nmea_types_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_loc_set_nmea_types_send(tqcm_client_context_t *ctx,
        loc_nmea_types_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_LOC_DELETE_ASSIST_DATA request
 *
 * This function sends a QMI_LOC_DELETE_ASSIST_DATA request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #loc_delete_assist_data_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_loc_delete_assist_data_send(tqcm_client_context_t *ctx,
        loc_delete_assist_data_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_LOC_SET_OPERATION_MODE request
 *
 * This function sends a QMI_LOC_SET_OPERATION_MODE request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #loc_operation_mode_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_loc_set_operation_mode_send(tqcm_client_context_t *ctx,
        loc_operation_mode_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_LOC_REGISTER_MASTER_CLIENT request
 *
 * This function sends a QMI_LOC_REGISTER_MASTER_CLIENT request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #loc_master_client_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_loc_register_master_client_send(tqcm_client_context_t *ctx,
        loc_master_client_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

QMI_SDK_error_t tqcm_loc_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id);

QMI_SDK_error_t tqcm_loc_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id);

#endif  /* TQCM_LOC_H */
