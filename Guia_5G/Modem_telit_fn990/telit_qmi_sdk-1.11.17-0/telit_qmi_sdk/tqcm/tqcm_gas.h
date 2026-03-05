#ifndef TQCM_GAS_H_
#define TQCM_GAS_H_

/**
 * @file tqcm_gas.h
 * @brief Telit GAS requests
 */

#include <telit_sdk.h>
#include <telit_sdk_gas.h>
#include "tqcm.h"

/**
 * @brief Send a QMI_GAS_DMS_USB_CFG_SET request
 *
 * This function sends a QMI_GAS_DMS_USB_CFG_SET request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #gas_dms_usb_cfg_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_set_usb_cfg_send(tqcm_client_context_t *ctx,
        gas_dms_usb_cfg_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_USB_CFG_GET request
 *
 * This function sends a QMI_GAS_DMS_USB_CFG_GET request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[out] output #gas_dms_usb_cfg_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_get_usb_cfg_send(tqcm_client_context_t *ctx,
        gas_dms_usb_cfg_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_ACTIVE_FW request
 *
 * This function sends a QMI_GAS_DMS_ACTIVE_FW request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #gas_dms_active_fw_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_active_fw_send(tqcm_client_context_t *ctx,
        gas_dms_active_fw_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_GET_FW request
 *
 * This function sends a QMI_GAS_DMS_GET_FW request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #gas_dms_get_fw_t
 * @param[out] out #gas_dms_fw_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_get_fw_send(tqcm_client_context_t *ctx,
        gas_dms_get_fw_t *input,
        gas_dms_fw_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_CLEAR_FW request
 *
 * This function sends a QMI_GAS_DMS_CLEAR_FW request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #gas_dms_get_fw_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_clear_fw_send(tqcm_client_context_t *ctx,
        gas_dms_get_fw_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_RESET_INFO_IND_REG request
 *
 * This function sends a QMI_GAS_DMS_RESET_INFO_IND_REG request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #gas_dms_reset_info_indication_register_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_reset_info_ind_reg_send(tqcm_client_context_t *ctx,
        gas_dms_reset_info_indication_register_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_GET_RESET_INFO request
 *
 * This function sends a QMI_GAS_DMS_GET_RESET_INFO request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[out] output #gas_dms_reset_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_get_reset_info_send(tqcm_client_context_t *ctx,
        gas_dms_reset_info_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_SET_CRASH_ACTION request
 *
 * This function sends a QMI_GAS_DMS_SET_CRASH_ACTION request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #gas_dms_crash_action_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_set_crash_action_send(tqcm_client_context_t *ctx,
        gas_dms_crash_action_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_GET_CRASH_INFO request
 *
 * This function sends a QMI_GAS_DMS_GET_CRASH_INFO request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #gas_dms_get_crash_info_t
 * @param[out] out #gas_dms_get_crash_info_resp_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_get_crash_info_send(tqcm_client_context_t *ctx,
        gas_dms_get_crash_info_t *input,
        gas_dms_get_crash_info_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_ERRGEN request
 *
 * This function sends a QMI_GAS_DMS_ERRGEN request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_errgen_send(tqcm_client_context_t *ctx,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_GET_HOST_FWVER request
 *
 * This function sends a QMI_GAS_DMS_GET_HOST_FWVER request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[out] output #gas_dms_versions_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_get_host_fwver_send(tqcm_client_context_t *ctx,
        gas_dms_versions_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_GET_OEM_FWVER request
 *
 * This function sends a QMI_GAS_DMS_GET_OEM_FWVER request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[out] output #gas_dms_get_oem_fwver_resp_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_get_oem_fwver_send(tqcm_client_context_t *ctx,
        gas_dms_get_oem_fwver_resp_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_GET_CRASH_ACTION request
 *
 * This function sends a QMI_GAS_DMS_GET_CRASH_ACTION request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[out] output #gas_dms_crash_action_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_get_crash_action_send(tqcm_client_context_t *ctx,
        gas_dms_crash_action_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_SET_USB_SWITCH request
 *
 * This function sends a QMI_GAS_DMS_SET_USB_SWITCH request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #gas_dms_usb_switch_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_set_usb_switch_send(tqcm_client_context_t *ctx,
        gas_dms_usb_switch_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_GET_USB_SWITCH request
 *
 * This function sends a QMI_GAS_DMS_GET_USB_SWITCH request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[out] output #gas_dms_usb_switch_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_get_usb_switch_send(tqcm_client_context_t *ctx,
        gas_dms_usb_switch_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_SET_USB_EXT request
 *
 * This function sends a QMI_GAS_DMS_SET_USB_EXT request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #gas_dms_usb_ext_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_set_usb_ext_send(tqcm_client_context_t *ctx,
        gas_dms_usb_ext_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_DMS_GET_USB_EXT request
 *
 * This function sends a QMI_GAS_DMS_GET_USB_EXT request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[out] output #gas_dms_usb_ext_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_dms_get_usb_ext_send(tqcm_client_context_t *ctx,
        gas_dms_usb_ext_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_PSM_GET_WDISA_CFG request
 *
 * This function sends a QMI_GAS_PSM_GET_WDISA_CFG request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[out] output #gas_psm_wdisa_cfg_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_psm_get_wdisa_cfg_send(tqcm_client_context_t *ctx,
        gas_psm_wdisa_cfg_t *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_PSM_SET_WDISA_CFG request
 *
 * This function sends a QMI_GAS_PSM_SET_WDISA_CFG request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #gas_psm_wdisa_cfg_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_psm_set_wdisa_cfg_send(tqcm_client_context_t *ctx,
        gas_psm_wdisa_cfg_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

/**
 * @brief Send a QMI_GAS_LOG_SET_REDIRECT request
 *
 * This function sends a QMI_GAS_LOG_SET_REDIRECT request.
 * - For @b synchronous behavior set #tqcm_callback_t to NULL.
 * - For @b asynchronous behavior set #tqcm_callback_t to a valid function and the token for linking
 *   the request to the response. The callback will be invoked once the response is available,
 *   with the response as the callback argument (#response_item_t *).
 *
 * @param[in]  ctx a valid #tqcm_client_context_t
 * @param[in]  input #gas_log_redirect_info_t
 * @param[out] sdk_error QMI protocol error if the request does not succeed when synchronous
 *             (see #QMI_protocol_error_t)
 * @param[in]  cb NULL for synchronous behavior, a valid #tqcm_callback_t value for asynchronous behavior
 * @param[in]  token asynchronous response identifier, otherwise 0
 * @return     0 (synchronous behavior) or EINPROGRESS (asynchronous behavior) for success,
 *             otherwise an error defined in <errno.h>
 */
int tqcm_gas_log_set_redirect_send(tqcm_client_context_t *ctx,
        gas_log_redirect_info_t *input,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token);

QMI_SDK_error_t tqcm_gas_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint16_t msg_id);

QMI_SDK_error_t tqcm_gas_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint16_t msg_id);

#endif  /* TQCM_GAS_H_ */
