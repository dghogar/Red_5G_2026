#ifndef TQCM_UIM_H_
#define TQCM_UIM_H_

/**
 * @file tqcm_uim.h
 * @brief UIM requests
 */

#include <telit_sdk.h>
#include <telit_sdk_uim.h>
#include "tqcm.h"

/**
 * @brief Send a QMI_UIM_READ_TRANSPARENT request
 *
 * This function sends a QMI_UIM_READ_TRANSPARENT request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_read_file_t
 * @param[out] out #uim_read_file_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_read_transparent_send(tqcm_client_context_t *ctx,
        uim_read_file_t *input,
        uim_read_file_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_READ_RECORD request
 *
 * This function sends a QMI_UIM_READ_RECORD request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_read_record_t
 * @param[out] out #uim_read_record_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_read_record_send(tqcm_client_context_t *ctx,
        uim_read_record_t *input,
        uim_read_record_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_GET_FILE_ATTRIBUTES request
 *
 * This function sends a QMI_UIM_GET_FILE_ATTRIBUTES request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_read_file_t
 * @param[out] out #uim_read_file_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_get_file_attributes_send(tqcm_client_context_t *ctx,
        uim_read_file_t *input,
        uim_read_file_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_SET_PIN_PROTECTION request
 *
 * This function sends a QMI_UIM_SET_PIN_PROTECTION request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_pin_protection_info_t
 * @param[out] out #uim_pin_resp_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_set_pin_protection_send(tqcm_client_context_t *ctx,
        uim_pin_protection_info_t *input,
        uim_pin_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_VERIFY_PIN request
 *
 * This function sends a QMI_UIM_VERIFY_PIN request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_verify_pin_info_t
 * @param[out] out #uim_pin_resp_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_verify_pin_send(tqcm_client_context_t *ctx,
        uim_verify_pin_info_t *input,
        uim_pin_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_UNBLOCK_PIN request
 *
 * This function sends a QMI_UIM_UNBLOCK_PIN request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_unblock_pin_info_t
 * @param[out] out #uim_pin_resp_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_unblock_pin_send(tqcm_client_context_t *ctx,
        uim_unblock_pin_info_t *input,
        uim_pin_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_CHANGE_PIN request
 *
 * This function sends a QMI_UIM_CHANGE_PIN request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_change_pin_info_t
 * @param[out] out #uim_pin_resp_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_change_pin_send(tqcm_client_context_t *ctx,
        uim_change_pin_info_t *input,
        uim_pin_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_EVENT_REG request
 *
 * This function sends a QMI_UIM_EVENT_REG request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_event_reg_info_t
 * @param[out] out #uim_event_reg_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_event_reg_send(tqcm_client_context_t *ctx,
        uim_event_reg_info_t *input,
        uim_event_reg_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_GET_CARD_STATUS request
 *
 * This function sends a QMI_UIM_GET_CARD_STATUS request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #uim_card_status_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_get_card_status_send(tqcm_client_context_t *ctx,
        uim_card_status_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_POWER_DOWN request
 *
 * This function sends a QMI_UIM_POWER_DOWN request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_power_slot_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_power_down_send(tqcm_client_context_t *ctx,
        uim_power_slot_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_POWER_UP request
 *
 * This function sends a QMI_UIM_POWER_UP request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_power_slot_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_power_up_send(tqcm_client_context_t *ctx,
        uim_power_slot_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_CHANGE_PROVISIONING_SESSION request
 *
 * This function sends a QMI_UIM_CHANGE_PROVISIONING_SESSION request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_change_provisioning_session_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_change_provisioning_session_send(tqcm_client_context_t *ctx,
        uim_change_provisioning_session_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_SEND_APDU request
 *
 * This function sends a QMI_UIM_SEND_APDU request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_send_apdu_info_t
 * @param[out] out #uim_send_apdu_response_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_send_apdu_send(tqcm_client_context_t *ctx,
        uim_send_apdu_info_t *input,
        uim_send_apdu_response_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_LOGICAL_CHANNEL request
 *
 * This function sends a QMI_UIM_LOGICAL_CHANNEL request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_logical_channel_info_t
 * @param[out] out #uim_logical_channel_resp_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_logical_channel_send(tqcm_client_context_t *ctx,
        uim_logical_channel_info_t *input,
        uim_logical_channel_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_SWITCH_SLOT request
 *
 * This function sends a QMI_UIM_SWITCH_SLOT request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #uim_switch_slot_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_switch_slot_send(tqcm_client_context_t *ctx,
        uim_switch_slot_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_UIM_GET_SLOTS_STATUS request
 *
 * This function sends a QMI_UIM_GET_SLOTS_STATUS request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #uim_slot_status_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_uim_get_slots_status_send(tqcm_client_context_t *ctx,
        uim_slot_status_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

QMI_SDK_error_t tqcm_uim_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id);

QMI_SDK_error_t tqcm_uim_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id);

#endif  /* TQCM_UIM_H_ */
