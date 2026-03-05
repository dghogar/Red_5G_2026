#ifndef TQCM_FOTA_H_
#define TQCM_FOTA_H_

/**
 * @file tqcm_fota.h
 * @brief Telit FOTA requests
 */

#include <telit_sdk.h>
#include <telit_sdk_fota.h>
#include "tqcm.h"

/**
 * @brief Send a QMI_FOTA_OEM_FTP_SET_SERVER_INFO request
 *
 * This function sends a QMI_FOTA_OEM_FTP_SET_SERVER_INFO request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #fota_server_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_fota_oem_ftp_set_server_info_send(tqcm_client_context_t *ctx,
        fota_server_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_FOTA_OEM_FTP_GET_SERVER_INFO request
 *
 * This function sends a QMI_FOTA_OEM_FTP_GET_SERVER_INFO request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #fota_server_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_fota_oem_ftp_get_server_info_send(tqcm_client_context_t *ctx,
        fota_server_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_FOTA_OEM_FTP_START_SESSION request
 *
 * This function sends a QMI_FOTA_OEM_FTP_START_SESSION request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #fota_session_type_t
 * @param[out] out #fota_session_status_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_fota_oem_ftp_start_session_send(tqcm_client_context_t *ctx,
        fota_session_type_t *input,
        fota_session_status_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_FOTA_OEM_FTP_STOP_SESSION request
 *
 * This function sends a QMI_FOTA_OEM_FTP_STOP_SESSION request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #fota_session_type_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_fota_oem_ftp_stop_session_send(tqcm_client_context_t *ctx,
        fota_session_type_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_FOTA_OEM_FTP_SET_SETTINGS request
 *
 * This function sends a QMI_FOTA_OEM_FTP_SET_SETTINGS request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #fota_settings_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_fota_oem_ftp_set_settings_send(tqcm_client_context_t *ctx,
        fota_settings_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_FOTA_OEM_FTP_GET_SETTINGS request
 *
 * This function sends a QMI_FOTA_OEM_FTP_GET_SETTINGS request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #fota_settings_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_fota_oem_ftp_get_settings_send(tqcm_client_context_t *ctx,
        fota_settings_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_FOTA_OEM_FTP_SET_EVENT_REPORT request
 *
 * This function sends a QMI_FOTA_OEM_FTP_SET_EVENT_REPORT request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #fota_set_event_report_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_fota_oem_ftp_set_event_report_send(tqcm_client_context_t *ctx,
        fota_set_event_report_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_FOTA_OEM_FTP_SEND_SELECTION request
 *
 * This function sends a QMI_FOTA_OEM_FTP_SEND_SELECTION request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #fota_selection_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_fota_oem_ftp_send_selection_send(tqcm_client_context_t *ctx,
        fota_selection_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_FOTA_OEM_ATT_GET_HOST_ODIS request
 *
 * This function sends a QMI_FOTA_OEM_ATT_GET_HOST_ODIS request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[out] out #fota_host_odis_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_fota_oem_att_get_host_odis_send(tqcm_client_context_t *ctx,
        fota_host_odis_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_FOTA_OEM_ATT_SET_HOST_ODIS request
 *
 * This function sends a QMI_FOTA_OEM_ATT_SET_HOST_ODIS request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #fota_host_odis_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_fota_oem_att_set_host_odis_send(tqcm_client_context_t *ctx,
        fota_host_odis_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_FOTA_OEM_FTP_GET_SESSION_INFO request
 *
 * This function sends a QMI_FOTA_OEM_FTP_GET_SESSION_INFO request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking the request to the response. The callback will be invoked once the response is available, with the response as the callback argument (#response_item_t *).
 *
 * @attention memory for the output struct with synchronous behavior should be allocated by the caller
 *
 * @param[in] ctx a valid #tqcm_client_context_t
 * @param[in] input #fota_session_type_t
 * @param[out] out #fota_session_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous (see #QMI_protocol_error_t)
 * @param[in] cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in] token asynchronous response identifier, otherwise 0
 * @return 0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success, otherwise an error defined in <errno.h>
 */
int tqcm_fota_oem_ftp_get_session_info_send(tqcm_client_context_t *ctx,
        fota_session_type_t *input,
        fota_session_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

QMI_SDK_error_t tqcm_fota_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id);

QMI_SDK_error_t tqcm_fota_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id);

#endif  /* TQCM_FOTA_H_ */
