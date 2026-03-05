#ifndef TQCM_ATR_H_
#define TQCM_ATR_H_

/**
 * @file tqcm_atr.h
 * @brief Telit ATR requests
 */

#include <telit_sdk.h>
#include <telit_sdk_atr.h>
#include "tqcm.h"

/**
 * @brief Send a QMI_ATR_CMD_SEND request
 *
 * This function sends a QMI_ATR_CMD_SEND request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #atr_cmd_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_atr_cmd_send_send(tqcm_client_context_t *ctx,
        atr_cmd_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

QMI_SDK_error_t tqcm_atr_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id);

QMI_SDK_error_t tqcm_atr_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id);

#endif  /* TQCM_ATR_H_ */
