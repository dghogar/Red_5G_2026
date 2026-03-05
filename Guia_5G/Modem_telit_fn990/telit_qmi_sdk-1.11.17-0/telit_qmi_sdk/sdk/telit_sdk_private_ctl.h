#ifndef TELIT_SDK_PRIVATE_CTL_H_
#define TELIT_SDK_PRIVATE_CTL_H_

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_CTL_GET_VERSION_INFO (33 - 0x0021)
 */

/* Structure to describe generic service version */
typedef struct {
    uint8_t qmi_svc_type;
    uint16_t major_ver;
    uint16_t minor_ver;
} QMI_CTL_generic_service_version_t;

/* Structure to describe QMI_CTL_GET_VERSION_INFO response TLV 0x01 */
typedef struct {
    uint8_t service_version_list_size;
    /* QMI_CTL_generic_service_version_t list[service_version_list_len] */
} QMI_CTL_GET_VERSION_INFO_response_0x01_t;

/*****************************************************************************
 * QMI_CTL_GET_CLIENT_ID (34 - 0x0022)
 * QMI_CTL_RELEASE_CLIENT_ID (35 - 0x0023)
 */

/* Structure to describe generic service type */
typedef struct {
    uint8_t qmi_svc_type;
} QMI_CTL_generic_svc_type_t;

/* Structure to describe generic client id tlv */
typedef struct {
    uint8_t qmi_svc_type;
    uint8_t client_id;
} QMI_CTL_generic_client_id_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_CTL_H_ */

