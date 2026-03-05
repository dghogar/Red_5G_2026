#ifndef TELIT_SDK_PRIVATE_WDA_H
#define TELIT_SDK_PRIVATE_WDA_H

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_WDA_SET_DATA_FORMAT (32 - 0x0020)
 * QMI_WDA_GET_DATA_FORMAT (33 - 0x0021)
 */

/* Structure to describe generic qos format */
typedef struct {
    uint8_t qos_format;
} QMI_WDA_generic_qos_format_t;

/* Structure to describe generic link protocol */
typedef struct {
    uint32_t link_prot;
} QMI_WDA_generic_link_protocol_t;

/* Structure to describe generic data aggregation protocol */
typedef struct {
    uint32_t data_aggregation_protocol;
} QMI_WDA_generic_data_aggregation_protocol_t;

/* Structure to describe generic data aggregation max datagrams */
typedef struct {
    uint32_t data_aggregation_max_datagrams;
} QMI_WDA_generic_data_aggregation_max_datagrams_t;

/* Structure to describe generic data aggregation max datagram size */
typedef struct {
    uint32_t data_aggregation_max_datagram_size;
} QMI_WDA_generic_data_aggregation_max_datagram_size_t;

/* Structure to describe generic peripheral endpoint */
typedef struct {
    uint32_t ep_type;
    uint32_t iface_id;
} QMI_WDA_generic_peripheral_endpoint_t;

/* Structure to describe generic downlink minimum padding */
typedef struct {
    uint32_t dl_minimum_padding;
} QMI_WDA_generic_dl_minimum_padding_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_WDA_H */
