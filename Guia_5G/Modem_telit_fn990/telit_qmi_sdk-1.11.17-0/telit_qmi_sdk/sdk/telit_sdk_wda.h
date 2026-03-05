#ifndef TELIT_SDK_WDA_H_
#define TELIT_SDK_WDA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_wda.h
 * @brief WDA pack/unpack functions
 * Service ID: QMI_SVC_WDA (26)
 */

/**
 * @brief Enum to describe QMI WDA Message IDs
 */
typedef enum {
    /** 32 - 0x0020 Indicate to the service the data format used by the client */
    QMI_WDA_SET_DATA_FORMAT = 0x0020,
    /** 33 - 0x0021 Get the current data format settings of the client */
    QMI_WDA_GET_DATA_FORMAT = 0x0021,
} QMI_WDA_message_t;

/**
 * @brief QMI WDA link protocol
 *
 */
typedef enum {
    /** Raw-IP mode */
    QMI_WDA_LINK_PROTOCOL_IP = 0x02,
} QMI_WDA_link_protocol_t;

/**
 * @brief QMI WDA data aggregation protocol
 *
 */
typedef enum {
    /** Data aggregation is disabled */
    QMI_WDA_DATA_AGG_DISABLED = 0x00,

    /** QMAP is enabled */
    QMI_WDA_DATA_AGG_QMAP     = 0x05,
} QMI_WDA_data_aggregation_protocol_t;

/**
 * @brief QMI WDA peripheral endpoint type
 *
 */
typedef enum {
    /** High-speed universal serial bus */
    QMI_WDA_DATA_EP_TYPE_HSUSB = 0x02,
    /** PCIe endpoint type */
    QMI_WDA_DATA_EP_TYPE_PCIE  = 0x03,
} QMI_WDA_peripheral_endpoint_type_t;

/**
 * @brief WDA peripheral endpoint
 *
 */
typedef struct {
    /** peripheral endpoint type, see #QMI_WDA_peripheral_endpoint_type_t */
    enum32_t ep_type;
    /** peripheral interface number */
    uint32_t iface_id;
} wda_peripheral_endpoint_t;

/**
 * @brief input type for QMI_WDA_SET_DATA_FORMAT, output for QMI_WDA_GET_DATA_FORMAT
 *
 */
typedef struct {
    /** set if qos_data_format is available */
    bool qos_format_available;
    /** 1 if QOS flow header is present, otherwise 0 */
    uint8_t qos_format;
    /** set if link_prot is available */
    bool link_prot_available;
    /** link protocol used by the client, see #QMI_WDA_link_protocol_t */
    enum32_t link_prot;
    /** set if ul_data_aggregation_protocol is available */
    bool ul_data_aggregation_protocol_available;
    /** uplink data aggregation protocol, see #QMI_WDA_data_aggregation_protocol_t */
    enum32_t ul_data_aggregation_protocol;
    /** set if dl_data_aggregation_protocol is available */
    bool dl_data_aggregation_protocol_available;
    /** downlink data aggregation protocol, see #QMI_WDA_data_aggregation_protocol_t */
    enum32_t dl_data_aggregation_protocol;
    /** set if dl_data_aggregation_max_datagrams is available */
    bool dl_data_aggregation_max_datagrams_available;
    /** Maximum number of datagrams in a single aggregated packet on downlink */
    uint32_t dl_data_aggregation_max_datagrams;
    /** set if dl_data_aggregation_max_size is available */
    bool dl_data_aggregation_max_size_available;
    /** Maximum size in bytes if a single aggregated packet allowed on downlink when data aggregation is enabled */
    uint32_t dl_data_aggregation_max_size;
    /** set if ul_data_aggregation_max_datagrams is available */
    bool ul_data_aggregation_max_datagrams_available;
    /** Maximum number of datagrams in a single aggregated packet on uplink */
    uint32_t ul_data_aggregation_max_datagrams;
    /** set if ul_data_aggregation_max_size is available */
    bool ul_data_aggregation_max_size_available;
    /** Maximum size in bytes if a single aggregated packet allowed on uplink when data aggregation is enabled */
    uint32_t ul_data_aggregation_max_size;
    /** set if peripheral_endpoint is available */
    bool peripheral_endpoint_available;
    /** peripheral endpoint, see #wda_peripheral_endpoint_t */
    wda_peripheral_endpoint_t peripheral_endpoint;
    /** set if dl_minimum_padding is available */
    bool dl_minimum_padding_available;
    /** Minimum padding bytes to add between aggregated downlink QMAP packets, valid values 0-64, must be
     * 4-bytes aligned. Default is 0 */
    uint32_t dl_minimum_padding;
} wda_data_format_t;

/**
 * @brief Pack QMI_WDA_SET_DATA_FORMAT (32 - 0x0020) request
 *
 * This function packs a qmi request for indicating to the service
 * the data format used by the client
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wda_data_format_t
 */
QMI_SDK_error_t telit_wda_set_data_format_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        wda_data_format_t *input);

/**
 * @brief Unpack QMI_WDA_SET_DATA_FORMAT (32 - 0x0020) request
 *
 * This function unpacks the result of a QMI_WDA_SET_DATA_FORMAT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wda_data_format_t
 */
QMI_SDK_error_t telit_wda_set_data_format_unpack(uint8_t *rsp, uint16_t len, wda_data_format_t *out);

/**
 * @brief input type for QMI_WDA_GET_DATA_FORMAT
 *
 */
typedef struct {
    /** set if peripheral_endpoint is available */
    bool peripheral_endpoint_available;
    /** peripheral endpoint, see #wda_peripheral_endpoint_t */
    wda_peripheral_endpoint_t peripheral_endpoint;
} wda_get_data_format_t;

/**
 * @brief Pack QMI_WDA_GET_DATA_FORMAT (33 - 0x0021) request
 *
 * This function packs a qmi request for getting the current
 * data format settings of the client
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wda_get_data_format_t
 */
QMI_SDK_error_t telit_wda_get_data_format_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        wda_get_data_format_t *input);

/**
 * @brief Unpack QMI_WDA_GET_DATA_FORMAT (33 - 0x0021) request
 *
 * This function unpacks the result of a QMI_WDA_GET_DATA_FORMAT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wda_data_format_t
 */
QMI_SDK_error_t telit_wda_get_data_format_unpack(uint8_t *rsp, uint16_t len, wda_data_format_t *out);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_WDA_H_ */
