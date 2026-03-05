#ifndef TELIT_SDK_DPM_H_
#define TELIT_SDK_DPM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_dpm.h
 * @brief DPM pack/unpack functions
 * Service ID: QMI_SVC_DPM (47)
 */

/**
 * @brief Enum to describe QMI DPM Message IDs
 */
typedef enum {
    /** 32 - 0x0020 Configure the different modem data paths */
    QMI_DPM_OPEN_PORT = 0x0020,
} QMI_DPM_message_t;

/**
 * @brief DPM peripheral endpoint
 *
 */
typedef struct {
    /** peripheral endpoint type, only 0x03 supported */
    uint32_t ep_type;
    /** peripheral interface number, only 4 supported */
    uint32_t iface_id;
} dpm_peripheral_endpoint_t;

/**
 * @brief DPM pair type
 *
 */
typedef struct {
    /** consumer pipe number, only 1 supported */
    uint32_t consumer_pipe_num;
    /** producer pipe number, only 14 supported */
    uint32_t producer_pipe_num;
} dpm_pair_type_t;

/**
 * @brief DPM port details
 *
 */
typedef struct {
    /** peripheral endpoint details, see #dpm_peripheral_endpoint_t */
    dpm_peripheral_endpoint_t peripheral_endpoint;
    /** pair type details, see #dpm_pair_type_t */
    dpm_pair_type_t pair_type;
} dpm_port_details_t;

#define QMI_DPM_MAX_PORTS 32

/**
 * @brief input type for QMI_DPM_OPEN_PORT
 *
 */
typedef struct {
    /** hardware_data_port list size, only 1 supported */
    uint8_t hardware_data_port_list_size;
    /** array of #dpm_port_details_t */
    dpm_port_details_t hardware_data_port_list[QMI_DPM_MAX_PORTS];
} dpm_open_port_t;

/**
 * @brief Pack QMI_DPM_OPEN_PORT (32 - 0x0020) request
 *
 * This function packs a qmi request for configuring the modem
 * data paths
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #dpm_open_port_t
 */
QMI_SDK_error_t telit_dpm_open_port_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        dpm_open_port_t *input);

/**
 * @brief Unpack QMI_DPM_OPEN_PORT (32 - 0x0020) request
 *
 * This function unpacks the result of a QMI_DPM_OPEN_PORT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_dpm_open_port_unpack(uint8_t *rsp, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_DPM_H_ */
