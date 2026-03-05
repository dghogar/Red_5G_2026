#ifndef TQCM_CAT_H_
#define TQCM_CAT_H_

/**
 * @file tqcm_cat.h
 * @brief Telit CAT requests
 */

#include <telit_sdk.h>
#include <telit_sdk_cat.h>
#include "tqcm.h"

/**
 * @brief Send a QMI_CAT_SET_EVENT_REPORT request
 *
 * This function sends a QMI_CAT_SET_EVENT_REPORT request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #cat_set_event_report_masks_t
 * @param[out] out #cat_set_event_report_masks_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_cat_set_event_report_send(tqcm_client_context_t *ctx,
        cat_set_event_report_masks_t *input,
        cat_set_event_report_masks_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_CAT_SEND_TR request
 *
 * This function sends a QMI_CAT_SEND_TR request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #cat_send_tr_input_t
 * @param[out] out #cat_send_tr_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_cat_send_tr_send(tqcm_client_context_t *ctx,
        cat_send_tr_input_t *input,
        cat_send_tr_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_CAT_SEND_ENVELOPE_CMD request
 *
 * This function sends a QMI_CAT_SEND_ENVELOPE_CMD request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #cat_send_envelope_cmd_input_t
 * @param[out] out #cat_send_envelope_cmd_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_cat_send_envelope_cmd_send(tqcm_client_context_t *ctx,
        cat_send_envelope_cmd_input_t *input,
        cat_send_envelope_cmd_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_CAT_SET_CONFIGURATION request
 *
 * This function sends a QMI_CAT_SET_CONFIGURATION request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #cat_configuration_mode_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_cat_set_configuration_send(tqcm_client_context_t *ctx,
        cat_configuration_mode_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_CAT_GET_CONFIGURATION request
 *
 * This function sends a QMI_CAT_GET_CONFIGURATION request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  out #cat_configuration_mode_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_cat_get_configuration_send(tqcm_client_context_t *ctx,
        cat_configuration_mode_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_CAT_GET_CACHED_PROACTIVE_CMD request
 *
 * This function sends a QMI_CAT_GET_CACHED_PROACTIVE_CMD request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #cat_cached_proactive_cmd_input_t
 * @param[out] out #cat_cached_proactive_cmd_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_cat_get_cached_proactive_cmd_send(tqcm_client_context_t *ctx,
        cat_cached_proactive_cmd_input_t *input,
        cat_cached_proactive_cmd_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

QMI_SDK_error_t tqcm_cat_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id);

QMI_SDK_error_t tqcm_cat_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id);

#endif  /* TQCM_CAT_H_ */
