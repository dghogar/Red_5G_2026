#ifndef TELIT_SDK_TMD_H
#define TELIT_SDK_TMD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_tmd.h
 * @brief TMD pack/unpack functions
 * Service ID: QMI_SVC_TMD (24 - 0x0018)
 */

/**
 * @brief Enum to describe QMI TMD Message types
 */
typedef enum {
    /** 32 - 0x0020 Get mitigation devices list */
    QMI_TMD_GET_MITIGATION_DEVICE_LIST = 0x0020,

    /** 34 - 0x0022 Get the thermal mitigation level for the mitigation device */
    QMI_TMD_GET_MITIGATION_LEVEL       = 0x0022,
} QMI_TMD_message_t;

#define QMI_TMD_MAX_MITIGATION_STRING_SIZE 128

/**
 * @brief input type for QMI_TMD_GET_MITIGATION_LEVEL request
 *
 */
typedef struct {
    /** mitigation_dev_id string length */
    uint8_t mitigation_dev_id_len;
    /** Mitigation device ID. */
    char mitigation_dev_id[QMI_TMD_MAX_MITIGATION_STRING_SIZE + 1];
    /** set if max_mitigation_level is available */
    bool max_mitigation_level_available;
    /** Maximum valid mitigation level.
     *
     * Valid range: 0 to max_mitigation_level.
     * Only used in QMI_TMD_GET_MITIGATION_DEVICE_LIST */
    uint8_t max_mitigation_level;
} tmd_mitigation_device_t;

#define QMI_TMD_MAX_MITIGATION_DEVICES 12

/**
 * @brief output type for QMI_TMD_GET_MITIGATION_DEVICE_LIST request
 *
 */
typedef struct {
    /** length of mitigation_devices */
    uint8_t mitigation_device_list_size;
    /** array of #tmd_mitigation_device_t instances */
    tmd_mitigation_device_t mitigation_devices[QMI_TMD_MAX_MITIGATION_DEVICES];
} tmd_mitigation_device_list_t;

/**
 * @brief Pack QMI_TMD_GET_MITIGATION_DEVICE_LIST (32 - 0x0020) request
 *
 * This function packs a qmi request for for getting the list of
 * mitigation devices from the remote endpoint
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_tmd_get_mitigation_device_list_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_TMD_GET_MITIGATION_DEVICE_LIST (32 - 0x0020) request
 *
 * This function unpacks the result of a QMI_TMD_GET_MITIGATION_DEVICE_LIST request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #tmd_mitigation_device_list_t
 */
QMI_SDK_error_t telit_tmd_get_mitigation_device_list_unpack(uint8_t *rsp, uint16_t len,
        tmd_mitigation_device_list_t *out);

/**
 * @brief output type for QMI_TMD_GET_MITIGATION_LEVEL request
 *
 */
typedef struct {
    /** set if current_mitigation_level
     * is available */
    bool current_mitigation_level_available;
    /** Current thermal mitigation level. */
    uint8_t current_mitigation_level;
    /** set if requested_mitigation_level
     * is available */
    bool requested_mitigation_level_available;
    /** The requested thermal mitigation level
     * from the client. The default is zero if the
     * client has not previously set the
     * mitigation level. */
    uint8_t requested_mitigation_level;
} tmd_mitigation_level_t;

/**
 * @brief Pack QMI_TMD_GET_MITIGATION_LEVEL (34 - 0x0022) request
 *
 * This function packs a qmi request for for getting the
 * thermal mitigation level for the mitigation device
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #tmd_mitigation_device_t
 */
QMI_SDK_error_t telit_tmd_get_mitigation_level_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        tmd_mitigation_device_t *input);

/**
 * @brief Unpack QMI_TMD_GET_MITIGATION_LEVEL (34 - 0x0022) request
 *
 * This function unpacks the result of a QMI_TMD_GET_MITIGATION_LEVEL request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #tmd_mitigation_level_t
 */
QMI_SDK_error_t telit_tmd_get_mitigation_level_unpack(uint8_t *rsp, uint16_t len,
        tmd_mitigation_level_t *out);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_TMD_H */
