#ifndef TQCM_DMS_H_
#define TQCM_DMS_H_

/**
 * @file tqcm_dms.h
 * @brief DMS requests
 */

#include <telit_sdk.h>
#include <telit_sdk_dms.h>
#include "tqcm.h"

/**
 * @brief Send a QMI_DMS_SET_EVENT_REPORT request
 *
 * This function sends a QMI_DMS_SET_EVENT_REPORT request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #dms_set_event_report_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_set_event_report_send(tqcm_client_context_t *ctx,
        dms_set_event_report_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_GET_DEVICE_CAP request
 *
 * This function sends a QMI_DMS_GET_DEVICE_CAP request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #dms_device_caps_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_get_device_cap_send(tqcm_client_context_t *ctx,
        dms_device_caps_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_GET_DEVICE_MFR request
 *
 * This function sends a QMI_DMS_GET_DEVICE_MFR request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #dms_get_manufacturer_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_get_device_mfr_send(tqcm_client_context_t *ctx,
        dms_get_manufacturer_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_GET_DEVICE_MODEL_ID request
 *
 * This function sends a QMI_DMS_GET_DEVICE_MODEL_ID request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #dms_get_model_id_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_get_device_model_id_send(tqcm_client_context_t *ctx,
        dms_get_model_id_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_GET_DEVICE_REV_ID request
 *
 * This function sends a QMI_DMS_GET_DEVICE_REV_ID request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #dms_get_fw_revision_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_get_device_rev_id_send(tqcm_client_context_t *ctx,
        dms_get_fw_revision_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_GET_MSISDN request
 *
 * This function sends a QMI_DMS_GET_MSISDN request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #dms_voice_number_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_get_msisdn_send(tqcm_client_context_t *ctx,
        dms_voice_number_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_GET_DEVICE_SERIAL_NUMBERS request
 *
 * This function sends a QMI_DMS_GET_DEVICE_SERIAL_NUMBERS request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #dms_get_serial_numbers_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_get_device_serial_numbers_send(tqcm_client_context_t *ctx,
        dms_get_serial_numbers_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_GET_DEVICE_HARDWARE_REV request
 *
 * This function sends a QMI_DMS_GET_DEVICE_HARDWARE_REV request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #dms_get_revision_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_get_device_hardware_rev_send(tqcm_client_context_t *ctx,
        dms_get_revision_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_GET_OPERATING_MODE request
 *
 * This function sends a QMI_DMS_GET_OPERATING_MODE request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #dms_power_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_get_operating_mode_send(tqcm_client_context_t *ctx,
        dms_power_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_SET_OPERATING_MODE request
 *
 * This function sends a QMI_DMS_SET_OPERATING_MODE request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #dms_power_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_set_operating_mode_send(tqcm_client_context_t *ctx,
        dms_power_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_GET_TIME request
 *
 * This function sends a QMI_DMS_GET_TIME request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #dms_get_time_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_get_time_send(tqcm_client_context_t *ctx,
        dms_get_time_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_GET_BAND_CAPABILITY request
 *
 * This function sends a QMI_DMS_GET_BAND_CAPABILITY request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #dms_band_capabilities_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_get_band_capability_send(tqcm_client_context_t *ctx,
        dms_band_capabilities_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION request
 *
 * This function sends a QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #dms_modem_activity_info_calculation_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_trigger_modem_activity_info_calculation_send(tqcm_client_context_t *ctx,
        dms_modem_activity_info_calculation_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_GET_MODEM_ACTIVITY_INFO request
 *
 * This function sends a QMI_DMS_GET_MODEM_ACTIVITY_INFO request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_get_modem_activity_info_send(tqcm_client_context_t *ctx,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_DEVICE_POWER_INFO request
 *
 * This function sends a QMI_DMS_DEVICE_POWER_INFO request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #dms_modem_device_power_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_modem_device_power_info_send(tqcm_client_context_t *ctx,
        dms_modem_device_power_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_DMS_SEND_AT_COMMAND request
 *
 * This function sends a QMI_DMS_SEND_AT_COMMAND request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #dms_at_command_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_dms_send_at_command_send(tqcm_client_context_t *ctx,
        dms_at_command_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

QMI_SDK_error_t tqcm_dms_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id);

QMI_SDK_error_t tqcm_dms_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id);

#endif  /* TQCM_DMS_H_ */
