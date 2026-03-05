#ifndef TELIT_SDK_QOS_H_
#define TELIT_SDK_QOS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_qos.h
 * @brief QOS pack/unpack functions
 * Service ID: QMI_SVC_QOS (4)
 */

/**
 * @brief Enum to describe QMI QOS Message IDs
 */
typedef enum {
    /** 01 - 0x0001 Set event reporting conditions */
    QMI_QOS_SET_EVENT_REPORT      = 0x0001,
    /** 01 - 0x0001 Flow and filter information indication */
    QMI_QOS_EVENT_REPORT_IND      = 0x0001,

    /** 41 - 0x0029 Primary flow specific indication */
    QMI_QOS_PRIMARY_QOS_EVENT_IND = 0x0029,

    /** 43 - 0x002B Bind a control point to a muxed data port */
    QMI_QOS_BIND_DATA_PORT        = 0x002B,

    /** 47 - 0x002F Set the registration state for different indications */
    QMI_QOS_INDICATION_REGISTER   = 0x002F,

    /** 49 - 0x0031 Indicate a change of state in a QoS flow */
    QMI_QOS_GLOBAL_QOS_FLOW_IND   = 0x0031,

    /** 51 - 0x0033 Retrieve the QoS flow and filter parameters that are in effect */
    QMI_QOS_GET_QOS_INFO          = 0x0033,
} QMI_QOS_message_t;

/**
 * @brief input type for QMI_QOS_SET_EVENT_REPORT
 *
 */
typedef struct {
    /** Action for global flow reporting, see enum #QMI_indication_action_t */
    QMI_indication_action_t global_flow_reporting;
} qos_set_event_report_info_t;

/**
 * @brief Pack QMI_QOS_SET_EVENT_REPORT (1 - 0x0001) request
 *
 * This function packs a qmi request for setting the event reporting
 * conditions for the requesting control point.
 *
 * The following event reportings can be activated:
 *
 * Global flow reporting
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #qos_set_event_report_info_t
 */
QMI_SDK_error_t telit_qos_set_event_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        qos_set_event_report_info_t *input);

/**
 * @brief Unpack QMI_QOS_SET_EVENT_REPORT (1 - 0x0001) request
 *
 * This function unpacks the result of a QMI_QOS_SET_EVENT_REPORT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_qos_set_event_report_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI QOS peripheral endpoint type
 *
 */
typedef enum {
    /** High-speed universal serial bus */
    QMI_QOS_DATA_EP_TYPE_HSUSB = 0x02,
    /** PCIe endpoint type */
    QMI_QOS_DATA_EP_TYPE_PCIE  = 0x03,
} QMI_QOS_peripheral_endpoint_type_t;

/**
 * @brief QMI QOS Peripheral endpoint specification type
 *
 */
typedef struct {
    /** Peripheral endpoint type, see #QMI_QOS_peripheral_endpoint_type_t */
    enum32_t ep_type;
    /** Peripheral interface number */
    uint32_t iface_id;
} peripheral_endopint_t;

/**
 * @brief input type for QMI_QOS_BIND_DATA_PORT
 *
 */
typedef struct {
    /** Set if endpoint_id should be considered */
    bool endpoint_id_available;
    /** The peripheral endpoint (physical data channel)
     * to which the client binds, see #peripheral_endopint_t */
    peripheral_endopint_t endpoint_id;
    /** Set if mux_id should be considered */
    bool mux_id_available;
    /** The Mux ID of the logical data channel
     * to which the client binds, default value is 0,
     * supported mux id values 1 and 2 */
    uint8_t mux_id;
    /** Set if data_port should be considered */
    bool data_port_available;
    /** SIO data port to which the client binds */
    uint16_t data_port;
} qos_bind_data_port_t;

/**
 * @brief Pack QMI_QOS_BIND_DATA_PORT (43 - 0x002B) request
 *
 * This function packs a qmi request for binding a control point to a muxed data port
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #qos_bind_data_port_t
 */
QMI_SDK_error_t telit_qos_bind_data_port_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        qos_bind_data_port_t *input);

/**
 * @brief Unpack QMI_QOS_BIND_DATA_PORT (43 - 0x002B) request
 *
 * This function unpacks the result of a QMI_QOS_BIND_DATA_PORT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_qos_bind_data_port_unpack(uint8_t *rsp, uint16_t len);

#define QMI_QOS_FLOW_INFO_MAX_SIZE 4
/**
 * @brief QMI QOS ip flow packet error rate type
 *
 */
typedef struct {
    /** Factor m in calculating packet error rate iwht formula: E = m*10^(-p) */
    uint16_t error_rate_multiplier;
    /** Factor p in calculating packet error rate iwht formula: E = m*10^(-p) */
    uint16_t error_rate_exponent;
} qos_ip_flow_packet_error_rate_t;

/**
 * @brief QMI QOS ip flow data rate token bucket type
 *
 */
typedef struct {
    /** Maximum rate at which data can be transmitted when the token bucket is full (bits per second) */
    uint32_t peak_rate;
    /** Rate at which tokens will be put in the token bucket (bits per second); a token is required to be present in the bucket to send a byte of data */
    uint32_t token_rate;
    /** Maximum number of tokens that can be accumulated at any instance (bytes): controls the size of the burst that is allowed at any given time */
    uint32_t bucket_size;
} qos_ip_flow_data_rate_token_bucket_t;

/**
 * @brief QMI QOS ip flow data rate minimum and maximum type
 *
 */
typedef struct {
    /** Maximum required data rate (bits per second) */
    uint32_t data_rate_max;
    /** Guaranteed data rate (bits per second) */
    uint32_t guaranteed_data_rate;
} qos_ip_flow_data_rate_min_max_t;

/**
 * @brief QMI QOS traffic classes
 *
 */
typedef enum {
    /** Conversation */
    QMI_QOS_CONVERSATION = 0,
    /** Streaming */
    QMI_QOS_STREAMING    = 1,
    /** Interactive */
    QMI_QOS_INTERACTIVE  = 2,
    /** Background */
    QMI_QOS_BACKGROUND   = 3,
} QMI_QOS_traffic_class_t;

/**
 * @brief QMI QOS BER values
 *
 */
typedef enum {
    /** 5*10^-2 */
    QMI_QOS_BER_0 = 0,
    /** 1*10^-2 */
    QMI_QOS_BER_1 = 1,
    /** 5*10^-3 */
    QMI_QOS_BER_2 = 2,
    /** 4*10^-3 */
    QMI_QOS_BER_3 = 3,
    /** 1*10^-3 */
    QMI_QOS_BER_4 = 4,
    /** 1*10^-4 */
    QMI_QOS_BER_5 = 5,
    /** 1*10^-5 */
    QMI_QOS_BER_6 = 6,
    /** 1*10^-6 */
    QMI_QOS_BER_7 = 7,
    /** 6*10^-8 */
    QMI_QOS_BER_8 = 8,
} QMI_QOS_BER_t;

/**
 * @brief QMI QOS relative traffic handling priorities
 *
 */
typedef enum {
    /** Relative traffic handling priority 1 */
    QMI_QOS_TRAFFIC_HANDLING_PRIORITY_1 = 0,
    /** Relative traffic handling priority 2 */
    QMI_QOS_TRAFFIC_HANDLING_PRIORITY_2 = 1,
    /** Relative traffic handling priority 3 */
    QMI_QOS_TRAFFIC_HANDLING_PRIORITY_3 = 2,
} QMI_QOS_traffic_handling_priority_t;

/**
 * @brief QMI QOS flow spec type
 *
 */
typedef struct {
    /** Integer that uniquely identifies each flow instance. A unique index must be assigned by the control point to every flow spec instance */
    uint8_t ip_flow_index;
    /** Set if ip_flow_traffic_class should be considered */
    bool ip_flow_traffic_class_available;
    /** Integer that designates the requested traffic class, see #QMI_QOS_traffic_class_t */
    enum8_t ip_flow_traffic_class;
    /** Set if ip_flow_data_rate_min_max should be considered */
    bool ip_flow_data_rate_min_max_available;
    qos_ip_flow_data_rate_min_max_t ip_flow_data_rate_min_max;           /** Instance of #qos_ip_flow_data_rate_min_max_t */
    /** Set if ip_flow_data_rate_token_bucket should be considered */
    bool ip_flow_data_rate_token_bucket_available;
    qos_ip_flow_data_rate_token_bucket_t ip_flow_data_rate_token_bucket; /** Instance of #qos_ip_flow_data_rate_token_bucket_t */
    /** Set if ip_flow_latency should be considered */
    bool ip_flow_latency_available;
    /** Maximum delay (in milliseconds) that can be tolerated by an IP packet during the transfer through the wireless link */
    uint32_t ip_flow_latency;
    bool ip_flow_jitter_available;  /** Set if ip_flow_jitter should be considered */
    /** Difference between the maximum and minimum latency (in milliseconds) that can be tolerated by an IP packet during the transfer through the wireless link */
    uint32_t ip_flow_jitter;
    /** Set if ip_flow_packet_error_rate should be considered */
    bool ip_flow_packet_error_rate_available;
    qos_ip_flow_packet_error_rate_t ip_flow_packet_error_rate;  /** Instance of #qos_ip_flow_packet_error_rate_t */
    /** Set if ip_flow_minimum_policed_packet_size should be considered */
    bool ip_flow_minimum_policed_packet_size_available;
    /** Integer that defines the minimum packet size (in bytes) that will be policed for QoS guarantees. Any IP packets that are smaller than the minimum specified policed size may not receive the requested QoS */
    uint32_t ip_flow_minimum_policed_packet_size;
    /** Set if ip_flow_maximum_allowed_packet_size should be considered */
    bool ip_flow_maximum_allowed_packet_size_available;
    /** Integer that defines the maximum packet size (in bytes) allowed in the IP flow. Any IP packets greater in size than this value are not queued for transmission */
    uint32_t ip_flow_maximum_allowed_packet_size;
    /** Set if ip_flow_3gpp_residual_bit_error_rate should be considered */
    bool ip_flow_3gpp_residual_bit_error_rate_available;
    /** Undetected BER for each IP flow in the delivered packets according to the following values, see #QMI_QOS_BER_t */
    uint8_t ip_flow_3gpp_residual_bit_error_rate;
    /** Set if ip_flow_3gpp_traffic_handling_priority should be considered */
    bool ip_flow_3gpp_traffic_handling_priority_available;
    /** Defines the relative priority of the flow, see #QMI_QOS_traffic_handling_priority_t */
    uint8_t ip_flow_3gpp_traffic_handling_priority;
    /** Set if ip_flow_3gpp_im_cn_flag should be considered */
    bool ip_flow_3gpp_im_cn_flag_available;
    /** IM CN subsystem signaling flag, according to the following values
       - 0x00 - false
       - 0x01 - true
     */
    uint8_t ip_flow_3gpp_im_cn_flag;
    /** Set if ip_flow_3gpp_sig_ind should be considered */
    bool ip_flow_3gpp_sig_ind_available;
    /** IP flow 3gpp signaling indication, according to the following values
       - 0x00 - false
       - 0x01 - true
     */
    uint8_t ip_flow_3gpp_sig_ind;
    /** Set if lte_qos_class_identifier should be considered */
    bool lte_qos_class_identifier_available;
    /** QoS class identifier (3GPP TS 23.203), according to the following values
       - 0 - requests the network to assign the appropriate QCI value
       - 1-4 - associated with guaranteed bitrates
       - 5-9 - associated with nonguaranteed bitrates, so the values specified as guaranteed and maximum bitrates are ignored
     */
    uint8_t lte_qos_class_identifier;
} qos_flow_spec_t;

/**
 * @brief QMI QOS IPv4 address type
 *
 */
typedef struct {
    /** IPv4 address */
    uint32_t addr;
    /** IPv4 subnet mask */
    uint32_t mask;
} qos_ipv4_addr_t;

/**
 * @brief QMI QOS type of service type
 *
 */
typedef struct {
    /** value */
    uint8_t val;
    /** mask */
    uint8_t mask;
} qos_ipv4_tos_t;

/**
 * @brief QMI QOS IPv6 address with prefix length type
 *
 */
typedef struct {
    /** IPv6 address */
    uint16_t addr[IPV6_ADDR_SIZE];
    /** IPv6 prefix length */
    uint8_t prefix_len;
} qos_ipv6_addr_t;

/**
 * @brief QMI QOS traffic classes
 *
 */
typedef struct {
    /** value */
    uint8_t val;
    /** mask */
    uint8_t mask;
} qos_ipv6_traffic_class_t;

/**
 * @brief QMI QOS TCP port info type
 *
 */
typedef struct {
    /** value */
    uint16_t port;
    /** range, a range of 0 means that only the specified port is valid */
    uint16_t range;
} qos_port_info_t;

/**
 * @brief QMI QOS high layer protocols
 *
 */
typedef enum {
    /** None */
    QMI_QOS_NONE   = 0x00,
    /** ICMP */
    QMI_QOS_ICMP   = 0x01,
    /** TCP */
    QMI_QOS_TCP    = 0x06,
    /** UDP */
    QMI_QOS_UDP    = 0x11,
    /** ESP */
    QMI_QOS_ESP    = 0x32,
    /** Authentication header */
    QMI_QOS_AH     = 0x33,
    /** Internet Control Messaging Protocol for IPV6 */
    QMI_QOS_ICMPV6 = 0x3A,
} QMI_QOS_high_layer_protocol_t;

/**
 * @brief QMI QOS filter spec type
 *
 */
typedef struct {
    /** Integer that uniquely identifies each filter instance */
    uint8_t ip_filter_index;
    /** Identifies whether the filter is associated with IPv4 or IPv6, see #QMI_ip_family_t */
    uint8_t ip_filter_version;
    /** Set if ipv4_filter_src_addr should be considered */
    bool ipv4_filter_src_addr_available;
    /** Source IPv4 address */
    qos_ipv4_addr_t ipv4_filter_src_addr;
    /** Set if ipv4_filter_dest_addr should be considered */
    bool ipv4_filter_dest_addr_available;
    /** Destination IPv4 address */
    qos_ipv4_addr_t ipv4_filter_dest_addr;
    /** Set if ipv4_filter_next_header_protocol is available */
    bool ipv4_filter_next_header_protocol_available;
    /** If available, only IP packets belonging to the specified higher layer protocol are considered when filtering, see #QMI_QOS_high_layer_protocol_t */
    uint8_t ipv4_filter_next_header_protocol;
    /** Set if ipv4_filter_tos is available */
    bool ipv4_filter_tos_available;
    /** Instance of #qos_ipv4_tos_t */
    qos_ipv4_tos_t ipv4_filter_tos;
    /** Set if ipv6_filter_src_addr should be considered */
    bool ipv6_filter_src_addr_available;
    /** Source IPv6 address */
    qos_ipv6_addr_t ipv6_filter_src_addr;
    /** Set if ipv6_filter_dest_addr should be considered */
    bool ipv6_filter_dest_addr_available;
    /** Destination IPv6 address */
    qos_ipv6_addr_t ipv6_filter_dest_addr;
    /** Set if ipv6_filter_traffic_class should be considered */
    bool ipv6_filter_traffic_class_available;
    /** Instance of #qos_ipv6_traffic_class_t */
    qos_ipv6_traffic_class_t ipv6_filter_traffic_class;
    /** Set if ipv6_filter_flow_label is available */
    bool ipv6_filter_flow_label_available;
    /** IPv6 flow label filter */
    uint32_t ipv6_filter_flow_label;
    /** Set if tcp_filter_src_port is available */
    bool tcp_filter_src_port_available;
    /** Instance of #qos_port_info_t */
    qos_port_info_t tcp_filter_src_port;
    /** Set if tcp_filter_dest_port is available */
    bool tcp_filter_dest_port_available;
    /** Instance of #qos_port_info_t */
    qos_port_info_t tcp_filter_dest_port;
    /** Set if udp_filter_src_port is available */
    bool udp_filter_src_port_available;
    /** Instance of #qos_port_info_t */
    qos_port_info_t udp_filter_src_port;
    /** Set if udp_filter_dest_port is available */
    bool udp_filter_dest_port_available;
    /** Instance of #qos_port_info_t */
    qos_port_info_t udp_filter_dest_port;
    /** Set if esp_filter_security_policy_index is available */
    bool esp_filter_security_policy_index_available;
    /** Security policy index to uniquely identify each IP flow for filtering encrypted packets for encapsulating security payload */
    uint32_t esp_filter_security_policy_index;
    /** Set if filter_precedence is available */
    bool filter_precedence_available;
    /** Specifies the order in which filters are applied; a lower numerical value has a higher precedence (only applies to network-initiated QoS) */
    uint16_t filter_precedence;
    /** Set if filter_id is available */
    bool filter_id_available;
    /** Unique identifier for each filter, assigned by the modem (only applies to network-initiated QoS) */
    uint16_t filter_id;
    /** Set if transport_prot_filter_src_port is available */
    bool transport_prot_filter_src_port_available;
    /** Instance of #qos_port_info_t */
    qos_port_info_t transport_prot_filter_src_port;
    /** Set if transport_prot_filter_dest_port is available */
    bool transport_prot_filter_dest_port_available;
    /** Instance of #qos_port_info_t */
    qos_port_info_t transport_prot_filter_dest_port;
} qos_filter_spec_t;

/**
 * @brief QMI QOS flow states
 *
 */
typedef enum {
    /** Flow activated */
    QMI_QOS_FLOW_ACTIVATED = 0x01,
    /** Flow modified */
    QMI_QOS_FLOW_MODIFIED  = 0x02,
    /** Flow deleted */
    QMI_QOS_FLOW_DELETED   = 0x03,
    /** Flow suspended */
    QMI_QOS_FLOW_SUSPENDED = 0x04,
    /** Flow enabled */
    QMI_QOS_FLOW_ENABLED   = 0x05,
    /** Flow disabled */
    QMI_QOS_FLOW_DISABLED  = 0x06,
} QMI_QOS_flow_state_t;

/**
 * @brief QMI QOS flow state type
 *
 */
typedef struct {
    /** QoS identifier */
    uint32_t identifier;
    /** Indicates if the flow considered is newly added or already existing */
    uint8_t new_flow;
    /** Indicates the status change on the flow, see #QMI_QOS_flow_state_t */
    uint8_t state_change;
} qos_flow_state_t;

/**
 * @brief QMI QOS flow type
 *
 */
typedef enum {
    /** UE-initiated QOS flow */
    QMI_QOS_FLOW_UE_INITIATED_QOS      = 0,
    /** Network-initiated QOS flow */
    QMI_QOS_FLOW_NETWORK_INITIATED_QOS = 1,
} QMI_QOS_flow_type_t;

/**
 * @brief QMI QOS flow info type
 *
 */
typedef struct {
    /** Instance of #qos_flow_state_t */
    qos_flow_state_t flow_state;
    /** Set if tx_qos_flow_granted should be considered */
    bool tx_qos_flow_granted_available;
    /** Instance of #qos_flow_spec_t */
    qos_flow_spec_t tx_qos_flow_granted;
    /** Set if rx_qos_flow_granted should be considered */
    bool rx_qos_flow_granted_available;
    /** Instance of #qos_flow_spec_t */
    qos_flow_spec_t rx_qos_flow_granted;
    /** Set if tx_qos_filter should be considered */
    bool tx_qos_filter_available;
    /** Instance of #qos_filter_spec_t */
    qos_filter_spec_t tx_qos_filter;
    /** Set if rx_qos_filter should be considered */
    bool rx_qos_filter_available;
    /** Instance of #qos_filter_spec_t */
    qos_filter_spec_t rx_qos_filter;
    /** Set if qos_flow_type should be considered */
    bool qos_flow_type_available;
    /** Indicates whether the QoS flow type is network-initiated or UE-initiated, see #QMI_QOS_flow_type_t */
    uint8_t qos_flow_type;
    /** Set if bearer_id should be considered */
    bool bearer_id_available;
    /** Bearer ID of the activated flow */
    uint8_t bearer_id;
    /** Set if flow_control_sequence_number should be considered */
    bool flow_control_sequence_number_available;
    /** The sequence number of each flow enable and disable event. Whenever the flow is disabled, the sequence number is incremented. The sequence number remains the same when the flow is enabled */
    uint16_t flow_control_sequence_number;
} qos_flow_info_t;

/**
 * @brief QMI QOS ip global flow data rate minimum and maximum type
 *
 */
typedef struct {
    /** Maximum required data rate (bits per second) */
    uint64_t data_rate_max;
    /** Guaranteed data rate (bits per second) */
    uint64_t guaranteed_data_rate;
} qos_ip_global_flow_data_rate_min_max_t;

/**
 * @brief QMI QOS global flow spec type
 *
 */
typedef struct {
    /** Set if ip_flow_traffic_class should be considered */
    bool ip_flow_traffic_class_available;
    /** Integer that designates the requested traffic class, see see #QMI_QOS_traffic_class_t */
    uint32_t ip_flow_traffic_class;
    /** Set if ip_flow_data_rate_min_max should be considered */
    bool ip_flow_data_rate_min_max_available;
    qos_ip_global_flow_data_rate_min_max_t ip_flow_data_rate_min_max;    /** Instance of #qos_ip_global_flow_data_rate_min_max_t */
    /** Set if ip_flow_data_rate_token_bucket should be considered */
    bool ip_flow_data_rate_token_bucket_available;
    qos_ip_flow_data_rate_token_bucket_t ip_flow_data_rate_token_bucket; /** Instance of #qos_ip_flow_data_rate_token_bucket_t */
    /** Set if ip_flow_latency should be considered */
    bool ip_flow_latency_available;
    /** Maximum delay (in milliseconds) that can be tolerated by an IP packet during the transfer through the wireless link */
    uint32_t ip_flow_latency;
    /** Set if ip_flow_jitter should be considered */
    bool ip_flow_jitter_available;
    /** Difference between the maximum and minimum latency (in milliseconds) that can be tolerated by an IP packet during the transfer through the wireless link */
    uint32_t ip_flow_jitter;
    /** Set if ip_flow_packet_error_rate should be considered */
    bool ip_flow_packet_error_rate_available;
    /** Instance of #qos_ip_flow_packet_error_rate_t */
    qos_ip_flow_packet_error_rate_t ip_flow_packet_error_rate;
    /** Set if ip_flow_minimum_policed_packet_size should be considered */
    bool ip_flow_minimum_policed_packet_size_available;
    /** Integer that defines the minimum packet size (in bytes) that will be policed for QoS guarantees. Any IP packets that are smaller than the minimum specified policed size may not receive the requested QoS */
    uint32_t ip_flow_minimum_policed_packet_size;
    /** Set if ip_flow_maximum_allowed_packet_size should be considered */
    bool ip_flow_maximum_allowed_packet_size_available;
    /** Integer that defines the maximum packet size (in bytes) allowed in the IP flow. Any IP packets greater in size than this value are not queued for transmission */
    uint32_t ip_flow_maximum_allowed_packet_size;
    /** Set if ip_flow_3gpp_residual_bit_error_rate should be considered */
    bool ip_flow_3gpp_residual_bit_error_rate_available;
    /** Undetected BER for each IP flow in the delivered packets, see #QMI_QOS_BER_t */
    uint32_t ip_flow_3gpp_residual_bit_error_rate;
    /** Set if ip_flow_3gpp_traffic_handling_priority should be considered */
    bool ip_flow_3gpp_traffic_handling_priority_available;
    /** Defines the relative priority of the flow, see #QMI_QOS_traffic_handling_priority_t */
    uint32_t ip_flow_3gpp_traffic_handling_priority;
    /** Set if ip_flow_3gpp_im_cn_flag should be considered */
    bool ip_flow_3gpp_im_cn_flag_available;
    /** IM CN subsystem signaling flag, according to the following values
       - 0x00 - false
       - 0x01 - true
     */
    uint8_t ip_flow_3gpp_im_cn_flag;
    /** Set if ip_flow_3gpp_sig_ind should be considered */
    bool ip_flow_3gpp_sig_ind_available;
    /** IP flow 3gpp signaling indication, according to the following values
       - 0x00 - false
       - 0x01 - true
     */
    uint8_t ip_flow_3gpp_sig_ind;
    /** Set if lte_qos_class_identifier should be considered */
    bool lte_qos_class_identifier_available;
    /** QoS class identifier (3GPP TS 23.203), according to the following values
       - 0 - requests the network to assign the appropriate QCI value
       - 1-4 - associated with guaranteed bitrates
       - 5-9 - associated with nonguaranteed bitrates, so the values specified as guaranteed and maximum bitrates are ignored
     */
    uint32_t lte_qos_class_identifier;
} qos_global_flow_spec_t;

/**
 * @brief QMI QOS global flow states
 *
 */
typedef enum {
    /** Flow activated */
    QMI_QOS_GLOBAL_FLOW_ACTIVATED = 0x00,
    /** Flow modified */
    QMI_QOS_GLOBAL_FLOW_MODIFIED  = 0x01,
    /** Flow deleted */
    QMI_QOS_GLOBAL_FLOW_DELETED   = 0x02,
    /** Flow suspended */
    QMI_QOS_GLOBAL_FLOW_SUSPENDED = 0x03,
    /** Flow enabled */
    QMI_QOS_GLOBAL_FLOW_ENABLED   = 0x04,
    /** Flow disabled */
    QMI_QOS_GLOBAL_FLOW_DISABLED  = 0x05,
} QMI_QOS_global_flow_state_t;

/**
 * @brief QMI QOS global flow state type
 *
 */
typedef struct {
    /** QoS identifier */
    uint32_t identifier;
    /** Indicates if the flow considered is newly added or already existing */
    uint8_t new_flow;
    /** Indicates the status change on the flow, see #QMI_QOS_global_flow_state_t */
    uint32_t state_change;
} qos_global_flow_state_t;

/**
 * @brief QMI QOS global filter spec type
 *
 */
typedef struct {
    /** Identifies whether the filter is associated with IPv4 or IPv6, see #QMI_ip_family_t */
    uint8_t ip_filter_version;
    /** Set if ipv4_filter_src_addr should be considered */
    bool ipv4_filter_src_addr_available;
    /** Source IPv4 address */
    qos_ipv4_addr_t ipv4_filter_src_addr;
    /** Set if ipv4_filter_dest_addr should be considered */
    bool ipv4_filter_dest_addr_available;
    /** Destination IPv4 address */
    qos_ipv4_addr_t ipv4_filter_dest_addr;
    /** Set if ipv4_filter_tos is available */
    bool ipv4_filter_tos_available;
    /** Instance of #qos_ipv4_tos_t */
    qos_ipv4_tos_t ipv4_filter_tos;
    /** Set if ipv6_filter_src_addr should be considered */
    bool ipv6_filter_src_addr_available;
    /** Source IPv6 address */
    qos_ipv6_addr_t ipv6_filter_src_addr;
    /** Set if ipv6_filter_dest_addr should be considered */
    bool ipv6_filter_dest_addr_available;
    /** Destination IPv6 address */
    qos_ipv6_addr_t ipv6_filter_dest_addr;
    /** Set if ipv6_filter_traffic_class should be considered */
    bool ipv6_filter_traffic_class_available;
    /** Instance of #qos_ipv6_traffic_class_t */
    qos_ipv6_traffic_class_t ipv6_filter_traffic_class;
    /** Set if ipv6_filter_flow_label is available */
    bool ipv6_filter_flow_label_available;
    /** IPv6 flow label filter */
    uint32_t ipv6_filter_flow_label;
    /** Set if filter_xport_protocol is available */
    bool filter_xport_protocol_available;
    /** Specify the higher layer protocol considered when filtering, see #QMI_QOS_high_layer_protocol_t */
    uint32_t filter_xport_protocol;
    /** Set if tcp_filter_src_port is available */
    bool tcp_filter_src_port_available;
    /** Instance of #qos_port_info_t */
    qos_port_info_t tcp_filter_src_port;
    /** Set if tcp_filter_dest_port is available */
    bool tcp_filter_dest_port_available;
    /** Instance of #qos_port_info_t */
    qos_port_info_t tcp_filter_dest_port;
    /** Set if udp_filter_src_port is available */
    bool udp_filter_src_port_available;
    /** Instance of #qos_port_info_t */
    qos_port_info_t udp_filter_src_port;
    /** Set if udp_filter_dest_port is available */
    bool udp_filter_dest_port_available;
    /** Instance of #qos_port_info_t */
    qos_port_info_t udp_filter_dest_port;
    /** Set if icmp_filter_type is available */
    bool icmp_filter_type_available;
    /** ICMP type */
    uint8_t icmp_filter_type;
    /** Set if icmp_filter_code is available */
    bool icmp_filter_code_available;
    /** ICMP code */
    uint8_t icmp_filter_code;
    /** Set if esp_filter_spi_index is available */
    bool esp_filter_spi_index_available;
    /** Security parameter index for IPSec */
    uint32_t esp_filter_spi_index;
    /** Set if ah_filter_spi_index is available */
    bool ah_filter_spi_index_available;
    /** Security parameter index */
    uint32_t ah_filter_spi_index;
    /** Set if ah_filter_id is available */
    bool ah_filter_id_available;
    /** Unique identifier for each filter, assigned by the modem (only applies to network-initiated QoS) */
    uint16_t ah_filter_id;
    /** Set if ah_filter_precedence is available */
    bool ah_filter_precedence_available;
    /** Specifies the order in which filters are applied; a lower numerical value has a higher precedence (only applies to network-initiated QoS) */
    uint16_t ah_filter_precedence;
} qos_global_filter_spec_t;

#define QMI_QOS_FILTER_MAX_SIZE 5
/**
 * @brief QMI QOS global flow info type
 *
 */
typedef struct {
    /** Instance of #qos_global_flow_state_t */
    qos_global_flow_state_t flow_state;
    /** Set if tx_qos_flow_granted should be considered */
    bool tx_qos_flow_granted_available;
    /** Instance of #qos_global_flow_spec_t */
    qos_global_flow_spec_t tx_qos_flow_granted;
    /** Set if rx_qos_flow_granted should be considered */
    bool rx_qos_flow_granted_available;
    /** Instance of #qos_global_flow_spec_t */
    qos_global_flow_spec_t rx_qos_flow_granted;
    /** tx_qos_filter array size */
    uint8_t tx_qos_filter_size;
    /** Array of #qos_global_filter_spec_t */
    qos_global_filter_spec_t tx_qos_filter[QMI_QOS_FILTER_MAX_SIZE];
    /** rx_qos_filter array size */
    uint8_t rx_qos_filter_size;
    /** Array of #qos_global_filter_spec_t */
    qos_global_filter_spec_t rx_qos_filter[QMI_QOS_FILTER_MAX_SIZE];
    /** Set if qos_flow_type should be considered */
    bool qos_flow_type_available;
    /** Indicates whether the QoS flow type is network-initiated or UE-initiated, see #QMI_QOS_flow_type_t */
    uint32_t qos_flow_type;
    /** Set if bearer_id should be considered */
    bool bearer_id_available;
    /** Bearer ID of the activated flow */
    uint8_t bearer_id;
    /** Set if flow_control_sequence_number should be considered */
    bool flow_control_sequence_number_available;
    /** The sequence number of each flow enable and disable event. Whenever the flow is disabled, the sequence number is incremented. The sequence number remains the same when the flow is enabled */
    uint16_t flow_control_sequence_number;
    /** Set if nr5g_tx_qos_class_identifier should be considered */
    bool nr5g_tx_qos_class_identifier_available;
    uint32_t nr5g_tx_qos_class_identifier;  /* 5G Tx QoS Class Identifier */
    /** Set if nr5g_rx_qos_class_identifier should be considered */
    bool nr5g_rx_qos_class_identifier_available;
    uint32_t nr5g_rx_qos_class_identifier;  /* 5G Rx QoS Class Identifier */
} qos_global_flow_info_t;

/**
 * @brief input type for QMI_QOS_INDICATION_REGISTER
 *
 */
typedef struct {
    /** Action for global flow reporting, see enum #QMI_indication_action_t */
    QMI_indication_action_t global_qos_flows;
} qos_indication_register_info_t;

/**
 * @brief Pack QMI_QOS_INDICATION_REGISTER (47 - 0x002F) request
 *
 * This function packs a qmi request for setting the registration state
 * for different indications for the requesting control point.
 *
 * The following event reportings can be activated:
 *
 * Global QOS flows
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #qos_indication_register_info_t
 */
QMI_SDK_error_t telit_qos_indication_register_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        qos_indication_register_info_t *input);

/**
 * @brief Unpack QMI_QOS_INDICATION_REGISTER (47 - 0x002F) request
 *
 * This function unpacks the result of a QMI_QOS_INDICATION_REGISTER request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_qos_indication_register_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI QOS event report info type
 *
 */
typedef struct {
    /** size of flow_info */
    uint8_t flow_info_size;
    /** Array of #qos_flow_info_t instances */
    qos_flow_info_t flow_info[QMI_QOS_FLOW_INFO_MAX_SIZE];
} qos_event_report_info_t;

/**
 * @brief QMI QOS operation result values
 *
 */
typedef enum {
    /** Operation succeeded */
    QMI_QOS_OPERATION_SUCCESS = 0x0001,
    /** Operation failed */
    QMI_QOS_OPERATION_FAILURE = 0x0002,
} QMI_QOS_operation_result_t;

/**
 * @brief input type for QMI_QOS_GET_QOS_INFO
 *
 */
typedef struct {
    /** Identifier for the QoS flow/instance that has been negotiated and that is being queried */
    uint32_t qos_id;
} qos_get_qos_t;

/**
 * @brief Pack QMI_QOS_GET_QOS_INFO (51 - 0x0033) request
 *
 * This function packs a qmi request for retrieving the QoS flow and
 * filter parameters that are in effect for the specified QoS flow.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #qos_get_qos_t
 */
QMI_SDK_error_t telit_qos_get_qos_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        qos_get_qos_t *input);

/** No parameters */
#define QMI_QOS_IP_FLOW_MASK_NONE                   0x0000000000000000
/** Traffic class */
#define QMI_QOS_IP_FLOW_MASK_TRF_CLASS              0x0000000000000001
/** Data rate min/max */
#define QMI_QOS_IP_FLOW_MASK_DATA_RATE_MIN_MAX      0x0000000000000002
/** Data rate token bucket */
#define QMI_QOS_IP_FLOW_MASK_DATA_RATE_TOK_BUCKET   0x0000000000000004
/** Latency */
#define QMI_QOS_IP_FLOW_MASK_LATENCY                0x0000000000000008
/** Jitter */
#define QMI_QOS_IP_FLOW_MASK_JITTER                 0x0000000000000010
/** Packet error rate */
#define QMI_QOS_IP_FLOW_MASK_PKT_ERR_RATE           0x0000000000000020
/** Minimum policed packet size */
#define QMI_QOS_IP_FLOW_MASK_MIN_POLICED_PKT_SIZE   0x0000000000000040
/** Maximum allowed packet size */
#define QMI_QOS_IP_FLOW_MASK_MAX_ALLOWED_PKT_SIZE   0x0000000000000080
/** 3GPP residual BER */
#define QMI_QOS_IP_FLOW_MASK_3GPP_RES_BER           0x0000000000000100
/** 3GPP traffic handling priority */
#define QMI_QOS_IP_FLOW_MASK_3GPP_TRF_PRI           0x0000000000000200
/** 3GPP2 profile ID */
#define QMI_QOS_IP_FLOW_MASK_3GPP2_PROFILE_ID       0x0000000000000400
/** 3GPP2 flow priority */
#define QMI_QOS_IP_FLOW_MASK_3GPP2_FLOW_PRIORITY    0x0000000000000800
/** 3GPP IM CN subsystem signaling flag */
#define QMI_QOS_IP_FLOW_MASK_3GPP_IM_CN             0x0000000000001000
/** 3GPP signaling indication */
#define QMI_QOS_IP_FLOW_MASK_3GPP_SIG_IND           0x0000000000002000
/** LTE QOS class identifier */
#define QMI_QOS_IP_FLOW_MASK_LTE_QCI                0x0000000000004000
/** 5G QOS class identifier */
#define QMI_QOS_IP_FLOW_MASK_5G_QCI                 0x0000000000008000
/** Averaging window */
#define QMI_QOS_IP_FLOW_MASK_AVG_WINDOW             0x0000000000010000

/**
 * @brief QMI QOS granted flow specification type
 *
 */
typedef struct {
    /** Mask of parameters that are valid for this flow, see QMI_QOS_IP_FLOW_MASK_* constants */
    uint64_t flow_valid_params;
    /** Requested traffic class, see #QMI_QOS_traffic_class_t */
    uint32_t ip_flow_trf_cls;
    /** Maximum required data rate (bps) */
    uint64_t data_rate_max;
    /** Minimum guaranteed data rate (bps) */
    uint64_t guaranteed_rate;
    /** Maximum rate at which data can be transmitted when the token bucket is full (bps) */
    uint32_t peak_rate;
    /** Rate at which tokens are put in the token bucket (bps) */
    uint32_t token_rate;
    /** Maximum number of tokens that can be accumulated at any instance (bytes) */
    uint32_t bucket_size;
    /** Maximum delay in milliseconds that can be tolerated by an IP packet during transfer through the wireless link */
    uint32_t ip_flow_latency;
    /** Difference between the maximum and minimum latency (in milliseconds) */
    uint32_t ip_flow_jitter;
    /** Factor m in calculating packet error rate: E = m*10**(-p) */
    uint16_t ip_flow_pkt_error_rate_multiplier;
    /** Factor p in calculating packet error rate: E = m*10**(-p) */
    uint16_t ip_flow_pkt_error_rate_exponent;
    /** Integer that defines the minimum packet size (in bytes) that will be policed for QoS guarantees. Any IP packets that are smaller than the minimum specified policed size may not receive the requested QoS */
    uint32_t ip_flow_min_policed_packet_size;
    /** Integer that defines the maximum packet size (in bytes) allowed in the IP flow. Any IP packets greater than the maximum allowed packet size are not queued for transmission */
    uint32_t ip_flow_max_allowed_packet_size;
    /** Indicates the undetected BER for each IP flow in the delivered packets. Applies only to 3GPP networks, see #QMI_QOS_BER_t */
    uint32_t ip_flow_3gpp_residual_bit_error_rate;
    /** Defines the relative proprity of the flow, see #QMI_QOS_traffic_handling_priority_t */
    uint32_t ip_flow_3gpp_traffic_handling_priority;
    /** A profile ID is shorthand for a defined set of QoS flow parameters specified by the network: to be present while requesting QoS for a CDMA device */
    uint16_t ip_flow_3gpp2_profile_id;
    /** Flow priority used by the network in case of contention between flows with the same QoS (CDMA) */
    uint8_t ip_flow_3gpp2_flow_priority;
    /** IM CN subsystem signaling flag. This parameter applies only to 3GPP networks, according to the following values:
       - 0  False
       - 1 - True */
    bool ip_flow_3gpp_im_cn_flag;
    /** Applies only to 3GPP networks, according to the following values:
       - 0 - False
       - 1 - True */
    bool ip_flow_3gpp_sig_ind;
    /** QoS class identifier, according to the following values:
       - 0 - requests the network to assign the appropriate QCI value
       - 1/4, 65, 66 - associated with guaranteed bitrates
       - 5/9, 69, 70 - associated with non guaranteed bitrates */
    uint32_t ip_flow_lte_qci;
} qos_granted_flow_t;

/** No parameters */
#define QMI_QOS_IPV4_FILTER_MASK_NONE       0x0000000000000000
/** IPv4 source address */
#define QMI_QOS_IPV4_FILTER_MASK_SRC_ADDR   0x0000000000000001
/** IPv4 destination address */
#define QMI_QOS_IPV4_FILTER_MASK_DEST_ADDR  0x0000000000000002
/** IPv4 type of service */
#define QMI_QOS_IPV4_FILTER_MASK_TOS        0x0000000000000004

/** No parameters */
#define QMI_QOS_IPV6_FILTER_MASK_NONE           0x0000000000000000
/** IPv6 source address */
#define QMI_QOS_IPV6_FILTER_MASK_SRC_ADDR       0x0000000000000001
/** IPv6 destination address */
#define QMI_QOS_IPV6_FILTER_MASK_DEST_ADDR      0x0000000000000002
/** IPv6 traffic class */
#define QMI_QOS_IPV6_FILTER_MASK_TRAFFIC_CLASS  0x0000000000000004
/** IPv6 flow label */
#define QMI_QOS_IPV6_FILTER_MASK_FLOW_LABEL     0x0000000000000008

/** No parameters */
#define QMI_QOS_PORT_INFO_FILTER_MASK_NONE      0x0000000000000000
/** Source port */
#define QMI_QOS_PORT_INFO_FILTER_MASK_SRC_PORT  0x0000000000000001
/** Destination port */
#define QMI_QOS_PORT_INFO_FILTER_MASK_DEST_PORT 0x0000000000000002

/** No parameters */
#define QMI_QOS_ICMP_FILTER_MASK_NONE       0x0000000000000000
/** Message type */
#define QMI_QOS_ICMP_FILTER_MASK_MSG_TYPE   0x0000000000000001
/** Message code */
#define QMI_QOS_ICMP_FILTER_MASK_MSG_CODE   0x0000000000000000

/** No parameters */
#define QMI_QOS_IPSEC_FILTER_MASK_NONE      0x0000000000000000
/** Security parameter index */
#define QMI_QOS_IPSEC_FILTER_MASK_SPI       0x0000000000000001

/**
 * @brief QMI QOS granted flow specification type
 *
 */
typedef struct {
    /** 0x04 for IPV4, 0x06 for IPV6 */
    uint8_t ip_version;
    /** This mask denotes which parameters contain valid values related to ipv4, see QMI_QOS_IPV4_FILTER_MASK_* constants */
    uint64_t valid_params_ipv4;
    /** IPv4 address source */
    qos_ipv4_addr_t ipv4_addr_src;
    /** IPv4 address destination */
    qos_ipv4_addr_t ipv4_addr_dest;
    /** IPv4 type of service */
    qos_ipv4_tos_t ipv4_tos;
    /** This mask denotes which parameters contain valid values related to ipv6, see QMI_QOS_IPV6_FILTER_MASK_* constants */
    uint64_t valid_params_ipv6;
    /** IPv6 address source */
    qos_ipv6_addr_t ipv6_addr_src;
    /** IPv6 address destination */
    qos_ipv6_addr_t ipv6_addr_dest;
    /** IPv6 traffic class */
    qos_ipv6_traffic_class_t ipv6_traffic_class;
    /** IPv6 flow label */
    uint32_t ipv6_flow_label;
    /** Denotes the transport protocol, see #QMI_QOS_high_layer_protocol_t */
    uint32_t xport_protocol;
    /** This mask denotes which parameters contain valid values, see QMI_QOS_PORT_INFO_FILTER_MASK_* constants */
    uint64_t valid_params_tcp;
    /** Source port */
    qos_port_info_t tcp_port_src;
    /** Destination port */
    qos_port_info_t tcp_port_dest;
    /** This mask denotes which parameters contain valid values, see QMI_QOS_PORT_INFO_FILTER_MASK_* constants */
    uint64_t valid_params_udp;
    /** Source port */
    qos_port_info_t udp_port_src;
    /** Destination port */
    qos_port_info_t udp_port_dest;
    /** This mask denotes which parameters contain valid values, see QMI_QOS_ICMP_FILTER_MASK_* constants */
    uint64_t valid_params_icmp;
    /** ICMP type */
    uint8_t icmp_type;
    /** ICMP code */
    uint8_t icmp_code;
    /** This mask denotes which parameters contain valid values, see QMI_QOS_IPSEC_FILTER_MASK_* constants */
    uint64_t valid_params_esp;
    /** Security parameter index for IPSec */
    uint32_t esp_spi;
    /** This mask denotes which parameters contain valid values, see QMI_QOS_IPSEC_FILTER_MASK_* constants */
    uint64_t valid_params_ah;
    /** Security parameter index for IPSec */
    uint32_t ah_spi;
    /** Unique identifier for each filter */
    uint16_t filter_id;
    /** Specifies the order in which filters are applied: a lower numerical value has a higher precedence */
    uint16_t filter_precedence;
} qos_info_filter_spec_t;

/**
 * @brief QMI QOS current flow status
 *
 */
typedef enum {
    /** Activated */
    QMI_QOS_STATUS_ACTIVATED = 0x01,
    /** Suspended */
    QMI_QOS_STATUS_SUSPENDED = 0x02,
} QMI_QOS_flow_status_t;

/**
 * @brief output type for QMI_QOS_GET_QOS_INFO
 *
 */
typedef struct {
    /** Set if flow_status should be considered */
    bool flow_status_available;
    /** Current flow status, see #QMI_QOS_flow_status_t */
    uint8_t flow_status;
    /** Set if tx_qos_granted_flow should be considered */
    bool tx_qos_granted_flow_available;
    /** Tx QoS granted flow, instance of #qos_granted_flow_t */
    qos_granted_flow_t tx_qos_granted_flow;
    /** Set if rx_qos_granted_flow should be considered */
    bool rx_qos_granted_flow_available;
    /** Rx QoS granted flow, instance of #qos_granted_flow_t */
    qos_granted_flow_t rx_qos_granted_flow;
    /** size of tx_qos_filter_specs */
    uint8_t tx_qos_filter_specs_size;
    /** Array of #qos_info_filter_spec_t instances */
    qos_info_filter_spec_t tx_qos_filter_specs[QMI_QOS_FILTER_MAX_SIZE];
    /** size of rx_qos_filter_specs */
    uint8_t rx_qos_filter_specs_size;
    /** Array of #qos_info_filter_spec_t instances */
    qos_info_filter_spec_t rx_qos_filter_specs[QMI_QOS_FILTER_MAX_SIZE];
    /** Set if ext_error_info should be considered */
    bool ext_error_info_available;
    /** Sata services error value returned by the IOCTL/lower layers */
    uint16_t ext_error_info;
} qos_get_qos_info_t;

/**
 * @brief Unpack QMI_QOS_GET_QOS_INFO (51 - 0x0033) request
 *
 * This function unpacks the result of a QMI_QOS_GET_QOS_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #qos_get_qos_info_t
 */
QMI_SDK_error_t telit_qos_get_qos_info_unpack(uint8_t *rsp, uint16_t len, qos_get_qos_info_t *out);

/**
 * @brief QMI QOS primary event info type
 *
 */
typedef struct {
    /** Event that causes the indication, see #QMI_QOS_operation_result_t */
    uint16_t event;
} qos_primary_event_info_t;

/**
 * Unpack qos indications
 * @param[in] msg_id indication id
 * @param[in] rsp indication
 * @param[in] len indication length
 * @param[out] out type according to the following
 *      QMI_QOS_EVENT_REPORT_IND      (1 - 0x0001)    -> #qos_event_report_info_t
 *      QMI_QOS_PRIMARY_QOS_EVENT_IND (41 - 0x0029)   -> #qos_primary_event_info_t
 *      QMI_QOS_GLOBAL_FLOW_IND       (49 - 0x0031)   -> #qos_global_flow_info_t
 * @param[in] out_len buffer size
 */
QMI_SDK_error_t telit_qos_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len,
        uint8_t *out, uint16_t out_len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_QOS_H_ */
