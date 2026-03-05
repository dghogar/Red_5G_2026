#ifndef TELIT_SDK_CTL_H_
#define TELIT_SDK_CTL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_ctl.h
 * @brief CTL pack/unpack functions
 * Service ID: QMI_SVC_CONTROL (0)
 */

/**
 * @brief Enum to describe QMI CTL Message IDs
 */
typedef enum {
    /** 33 - 0x0021 Query the version of all services */
    QMI_CTL_GET_VERSION_INFO  = 0x0021,
    /** 34 - 0x0022 Request client id for a service */
    QMI_CTL_GET_CLIENT_ID     = 0x0022,
    /** 35 - 0x0023 Release an assigned client ID */
    QMI_CTL_RELEASE_CLIENT_ID = 0x0023,
} QMI_CTL_message_t;

#define QMI_CTL_MAX_SERVICE_LIST_LEN 256
/**
 * @brief QMI CTL service version info type
 *
 */
typedef struct {
    /** Service type of the client id, see #QMI_service_t for values */
    enum8_t qmi_svc_type;
    /** Major version number */
    uint16_t major_ver;
    /** Minor version number */
    uint16_t minor_ver;
} ctl_svc_version_t;

/**
 * @brief Pack QMI_CTL_GET_VERSION_INFO (33 - 0x0021) request
 *
 * This function packs a qmi request for querying the version of all qmi services supported by the modem
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_ctl_get_version_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len);

/**
 * @brief output type for QMI_CTL_GET_VERSION_INFO
 *
 */
typedef struct {
    /** service version list size */
    uint8_t service_version_list_size;
    /** array of #ctl_svc_version_t instances */
    ctl_svc_version_t service_version_list[QMI_CTL_MAX_SERVICE_LIST_LEN];
} ctl_get_version_info_t;

/**
 * @brief Unpack QMI_CTL_GET_VERSION_INFO (33 - 0x0021) request
 *
 * This function unpacks the result of a QMI_CTL_GET_VERSION_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #ctl_get_version_info_t
 */
QMI_SDK_error_t telit_ctl_get_version_info_unpack(uint8_t *rsp, uint16_t len, ctl_get_version_info_t *out);

/**
 * @brief input type for QMI_CTL_GET_CLIENT_ID
 *
 */
typedef struct {
    /** Service type of the client id, see #QMI_service_t for values */
    enum8_t qmi_svc_type;
} ctl_get_client_id_t;

/**
 * @brief Pack QMI_CTL_GET_CLIENT_ID (34 - 0x0022) request
 *
 * This function packs a qmi request for requesting a client ID for the specified QMI service type
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #ctl_get_client_id_t
 */
QMI_SDK_error_t telit_ctl_get_client_id_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        ctl_get_client_id_t *input);

/**
 * @brief output type for QMI_CTL_GET_CLIENT_ID and QMI_CTL_RELEASE_CLIENT_ID
 *
 */
typedef struct {
    /** Service type of the client id, see #QMI_service_t for values */
    enum8_t qmi_svc_type;
    /** Client ID */
    uint8_t client_id;
} ctl_get_client_id_info_t;

/**
 * @brief Unpack QMI_CTL_GET_CLIENT_ID (34 - 0x0022) request
 *
 * This function unpacks the result of a QMI_CTL_GET_CLIENT_ID request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #ctl_get_client_id_info_t
 */
QMI_SDK_error_t telit_ctl_get_client_id_unpack(uint8_t *rsp, uint16_t len, ctl_get_client_id_info_t *out);

/**
 * @brief Pack QMI_CTL_RELEASE_CLIENT_ID (35 - 0x0023) request
 *
 * This function packs a qmi request for releasing a previously assigned client ID
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #ctl_get_client_id_info_t
 */
QMI_SDK_error_t telit_ctl_release_client_id_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        ctl_get_client_id_info_t *input);

/**
 * @brief Unpack QMI_CTL_RELEASE_CLIENT_ID (35 - 0x0023) request
 *
 * This function unpacks the result of a QMI_CTL_RELEASE_CLIENT_ID request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out buffer to hold #ctl_get_client_id_info_t struct
 */
QMI_SDK_error_t telit_ctl_release_client_id_unpack(uint8_t *rsp, uint16_t len, ctl_get_client_id_info_t *out);

/**
 * @brief Add qmux header to a service payload
 *
 * This function adds a qmux header to the qmi message resulting of a service request
 *
 * @param[in] req_ctx request context
 * @param[in] client_id client id of the qmi request
 * @param[in] payload buffer holding the qmi request
 * @param[in] payloadlen payload len
 * @param[out] buffer buffer holding the qmi request and qmux header
 * @param[out] bufferLen buffer size
 */
QMI_SDK_error_t add_qmux_header(const pack_qmi_t *context,
        uint8_t client_id,
        const uint8_t *payload,
        uint16_t payloadlen,
        uint8_t *buffer,
        uint16_t *bufferLen);

/**
 * @brief Add qmux header to a service payload
 *
 * This function adds a qmux header to the qmi message resulting of a service request
 *
 * @param[in] rsp qmux response
 * @param[in] rsp_len qmux response len
 * @param[out] payload buffer without qmux header
 * @param[out] payloadlen payload len
 */
QMI_SDK_error_t skip_qmux_header(const uint8_t *rsp,
        uint16_t rsp_len,
        uint8_t **payload,
        uint16_t *payload_len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_CTL_H_ */
