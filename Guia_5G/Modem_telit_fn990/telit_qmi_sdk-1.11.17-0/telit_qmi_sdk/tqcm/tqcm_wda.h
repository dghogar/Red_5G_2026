#ifndef TQCM_WDA_H_
#define TQCM_WDA_H_

/**
 * @file tqcm_wda.h
 * @brief WDA requests
 */

#include <telit_sdk.h>
#include <telit_sdk_wda.h>
#include "tqcm.h"

/**
 * @brief Send a QMI_WDA_SET_DATA_FORMAT request
 *
 * This function sends a QMI_WDA_SET_DATA_FORMAT request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #wda_data_format_t
 * @param[out] out #wda_data_format_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_wda_set_data_format_send(tqcm_client_context_t *ctx,
        wda_data_format_t *input,
        wda_data_format_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_WDA_GET_DATA_FORMAT request
 *
 * This function sends a QMI_WDA_GET_DATA_FORMAT request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #wda_get_data_format_t
 * @param[out] out #wda_data_format_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_wda_get_data_format_send(tqcm_client_context_t *ctx,
        wda_get_data_format_t *input,
        wda_data_format_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

QMI_SDK_error_t tqcm_wda_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id);

QMI_SDK_error_t tqcm_wda_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id);

#endif  /* TQCM_WDA_H_ */
