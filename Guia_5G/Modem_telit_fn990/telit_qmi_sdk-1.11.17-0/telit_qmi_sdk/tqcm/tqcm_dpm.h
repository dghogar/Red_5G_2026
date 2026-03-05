#ifndef TQCM_DPM_H_
#define TQCM_DPM_H_

/**
 * @file tqcm_dpm.h
 * @brief DPM requests
 */

#include <telit_sdk.h>
#include <telit_sdk_dpm.h>
#include "tqcm.h"

/**
 * @brief Send a QMI_DPM_OPEN_PORT request
 *
 * This function sends a QMI_DPM_OPEN_PORT request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #dpm_open_port_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dpm_open_port_send(tqcm_client_context_t *ctx,
        dpm_open_port_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

QMI_SDK_error_t tqcm_dpm_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id);

QMI_SDK_error_t tqcm_dpm_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id);

#endif  /* TQCM_DPM_H_ */
