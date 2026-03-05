#ifndef TELIT_SDK_PRIVATE_TDM_H
#define TELIT_SDK_PRIVATE_TDM_H

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_TMD_GET_MITIGATION_DEVICE_LIST (32 - 0x0020)
 */

typedef struct
{
    uint8_t mitigation_dev_id_len;
    /* uint8_t mitigation_dev_id[mitigation_dev_id_len]; */
    uint8_t max_mitigation_level;
} tmd_mitigation_device_instance_t;

/* Structure to describe TLV 0x10 QMI_TMD_GET_MITIGATION_DEVICE_LIST response */
typedef struct {
    uint8_t mitigation_device_list_size;
    /* tmd_mitigation_device_instance_t mitigation_devices[mitigation_device_list_size]; */
} QMI_TMD_GET_MITIGATION_DEVICE_LIST_response_0x10_t;

/*****************************************************************************
 * QMI_TMD_GET_MITIGATION_LEVEL (34 - 0x0022)
 */

/* Structure to describe TLV 0x01 for QMI_TMD_GET_MITIGATION_LEVEL request */
typedef struct {
    uint8_t mitigation_dev_id_len;
    /* uint8_t mitigation_dev_id[mitigation_dev_id_len]; */
} QMI_TMD_GET_MITIGATION_LEVEL_request_0x01_t;

/* Structure to describe TLV 0x10 for QMI_TMD_GET_MITIGATION_LEVEL response */
typedef struct {
    uint8_t current_mitigation_level;
} QMI_TMD_GET_MITIGATION_LEVEL_response_0x10_t;

/* Structure to describe TLV 0x11 for QMI_TMD_GET_MITIGATION_LEVEL response */
typedef struct {
    uint8_t requested_mitigation_level;
} QMI_TMD_GET_MITIGATION_LEVEL_response_0x11_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_TDM_H */
