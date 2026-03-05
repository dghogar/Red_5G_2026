#ifndef TELIT_SDK_PRIVATE_QOS_H_
#define TELIT_SDK_PRIVATE_QOS_H_

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_QOS_SET_EVENT_REPORT (1 - 0x0001)
 */

/* Structure to describe request TLV 0x10 for QMI_QOS_SET_EVENT_REPORT */
typedef struct {
    uint8_t global_flow_reporting;
} QMI_QOS_SET_EVENT_REPORT_request_0x10_t;

/*****************************************************************************
 * QMI_QOS_BIND_DATA_PORT (43 - 0x002B)
 */

/* Structure to describe request TLV 0x10 for QOS_BIND_DATA_PORT */
typedef struct {
    uint32_t ep_type;
    uint32_t iface_id;
} QMI_QOS_BIND_DATA_PORT_request_0x10_t;

/* Structure to describe request TLV 0x11 for QOS_BIND_DATA_PORT */
typedef struct {
    uint8_t mux_id;
} QMI_QOS_BIND_DATA_PORT_request_0x11_t;

/* Structure to describe request TLV 0x12 for QOS_BIND_DATA_PORT */
typedef struct {
    uint16_t data_port;
} QMI_QOS_BIND_DATA_PORT_request_0x12_t;

/*****************************************************************************
 * QMI_QOS_INDICATION_REGISTER (47 - 0x002F)
 */

/* Structure to describe request TLV 0x10 for QMI_INDICATION_REGISTER */
typedef struct {
    uint8_t global_qos_flows;
} QMI_INDICATION_REGISTER_request_0x10_t;

/*****************************************************************************
 * QMI_QOS_GET_QOS_INFO (51 - 0x0033)
 */

/* Structure to describe request TLV 0x01 for QMI_QOS_GET_QOS_INFO */
typedef struct {
    uint32_t qos_id;
} QMI_QOS_GET_QOS_INFO_request_0x01_t;

/* Structure to describe response TLV 0x10 for QMI_QOS_GET_QOS_INFO */
typedef struct {
    uint8_t flow_status;
} QMI_QOS_GET_QOS_INFO_response_0x10_t;

/*****************************************************************************
 * QMI QOS indications
 */

/* Structure to describe indication TLV 0x01 for QMI_QOS_PRIMARY_QOS_EVENT_IND */
typedef struct {
    uint16_t event;
} QMI_QOS_PRIMARY_QOS_EVENT_indication_0x01_t;

/* Structure to describe generic QoS flow state */
typedef struct {
    uint32_t identifier;
    uint8_t new_flow;
    uint8_t state_change;
} QMI_QOS_generic_flow_state_t;

/* Structure to describe generic QoS global flow state */
typedef struct {
    uint32_t identifier;
    uint8_t new_flow;
    uint32_t state_change;
} QMI_QOS_generic_global_flow_state_t;

/* Structure to describe generic QoS flow type */
typedef struct {
    uint8_t qos_flow_type;
} QMI_QOS_generic_flow_type_t;

/* Structure to describe global QoS flow type */
typedef struct {
    uint32_t qos_flow_type;
} QMI_QOS_global_flow_type_t;

/* Structure to describe generic QoS flow bearer id */
typedef struct {
    uint8_t bearer_id;
} QMI_QOS_generic_flow_bearer_id_t;

/* Structure to describe generic QoS flow sequence number */
typedef struct {
    uint16_t flow_control_sequence_number;
} QMI_QOS_generic_flow_seq_num_t;

/* Structure to describe generic ip flow index */
typedef struct {
    uint8_t ip_flow_index;
} QMI_QOS_generic_ip_flow_index_t;

/* Structure to describe generic ip flow traffic class */
typedef struct {
    uint8_t ip_flow_traffic_class;
} QMI_QOS_generic_ip_flow_traffic_class_t;

/* Structure to describe generic ip flow data rate min and max */
typedef struct {
    uint32_t data_rate_max;
    uint32_t guaranteed_data_rate;
} QMI_QOS_generic_ip_flow_data_rate_min_max_t;

/* Structure to describe generic ip flow data rate token bucket */
typedef struct {
    uint32_t peak_rate;
    uint32_t token_rate;
    uint32_t bucket_size;
} QMI_QOS_generic_ip_flow_data_rate_token_bucket_t;

/* Structure to describe generic ip flow latency */
typedef struct {
    uint32_t ip_flow_latency;
} QMI_QOS_generic_ip_flow_latency_t;

/* Structure to describe generic ip flow jitter */
typedef struct {
    uint32_t ip_flow_jitter;
} QMI_QOS_generic_ip_flow_jitter_t;

/* Structure to describe generic ip flow packet error rate */
typedef struct {
    uint16_t error_rate_multiplier;
    uint16_t error_rate_exponent;
} QMI_QOS_generic_ip_flow_packet_error_rate_t;

/* Structure to describe generic ip flow minimum policed packet size */
typedef struct {
    uint32_t ip_flow_minimum_policed_packet_size;
} QMI_QOS_generic_ip_flow_min_policed_pkt_sz_t;

/* Structure to describe generic ip flow maximum allowed packet size */
typedef struct {
    uint32_t ip_flow_maximum_allowed_packet_size;
} QMI_QOS_generic_ip_flow_max_allowed_pkt_sz_t;

/* Structure to describe generic ip flow 3gpp residual ber */
typedef struct {
    uint8_t ip_flow_3gpp_residual_bit_error_rate;
} QMI_QOS_generic_ip_flow_3gpp_residual_ber_t;

/* Structure to describe generic ip flow 3gpp traffic handling priority */
typedef struct {
    uint8_t ip_flow_3gpp_traffic_handling_priority;
} QMI_QOS_generic_ip_flow_3gpp_traffic_handling_pri_t;

/* Structure to describe generic ip flow 3gpp im cn flag */
typedef struct {
    uint8_t ip_flow_3gpp_im_cn_flag;
} QMI_QOS_generic_ip_flow_3gpp_im_cn_flag_t;

/* Structure to describe generic ip flow 3gpp signaling indication */
typedef struct {
    uint8_t ip_flow_3gpp_sig_ind;
} QMI_QOS_generic_ip_flow_3gpp_sig_ind_t;

/* Structure to describe generic lte QCI */
typedef struct {
    uint8_t lte_qos_class_identifier;
} QMI_QOS_generic_lte_qci_t;

/* Structure to describe generic ip filter index */
typedef struct {
    uint8_t ip_filter_index;
} QMI_QOS_generic_ip_filter_index_t;

/* Structure to describe generic ip filter version */
typedef struct {
    uint8_t ip_filter_version;
} QMI_QOS_generic_ip_filter_version_t;

/* Structure to describe generic ipv4 address */
typedef struct {
    uint32_t addr;
    uint32_t mask;
} QMI_QOS_generic_ipv4_address_t;

/* Structure to describe generic ipv4 filter next header protocol */
typedef struct {
    uint8_t ipv4_filter_next_header_protocol;
} QMI_QOS_generic_ipv4_next_hdr_proto_t;

/* Structure to describe generic ipv4 filter type of service */
typedef struct {
    uint8_t val;
    uint8_t mask;
} QMI_QOS_generic_ipv4_tos_t;

/* Structure to describe generic ipv6 address */
typedef struct {
    uint16_t addr[8];
    uint8_t prefix_len;
} QMI_QOS_generic_ipv6_address_t;

/* Structure to describe generic ipv6 filter traffic class */
typedef struct {
    uint8_t val;
    uint8_t mask;
} QMI_QOS_generic_ipv6_filter_traffic_class_t;

/* Structure to describe generic ipv6 filter flow label */
typedef struct {
    uint32_t ipv6_filter_flow_label;
} QMI_QOS_generic_ipv6_filter_flow_label_t;

/* Structure to describe generic filter port */
typedef struct {
    uint16_t port;
    uint16_t range;
} QMI_QOS_generic_filter_port_t;

/* Structure to describe generic filter ESP security policy index */
typedef struct {
    uint32_t esp_filter_security_policy_index;
} QMI_QOS_generic_filter_esp_t;

/* Structure to describe generic filter precedence */
typedef struct {
    uint16_t filter_precedence;
} QMI_QOS_generic_filter_precedence_t;

/* Structure to describe generic filter id */
typedef struct {
    uint16_t filter_id;
} QMI_QOS_generic_filter_id_t;

/* Structure to describe generic QoS global flow spec */
typedef struct {
    uint64_t flow_valid_params;
    uint32_t ip_flow_traffic_class;
    uint64_t data_rate_max;
    uint64_t guaranteed_data_rate;
    uint32_t peak_rate;
    uint32_t token_rate;
    uint32_t bucket_size;
    uint32_t ip_flow_latency;
    uint32_t ip_flow_jitter;
    uint16_t error_rate_multiplier;
    uint16_t error_rate_exponent;
    uint32_t ip_flow_minimum_policed_packet_size;
    uint32_t ip_flow_maximum_allowed_packet_size;
    uint32_t ip_flow_3gpp_residual_bit_error_rate;
    uint32_t ip_flow_3gpp_traffic_handling_priority;
    uint16_t ip_flow_3gpp2_profile_id;   /* NOT USED */
    uint8_t ip_flow_3gpp2_flow_priority; /* NOT USED */
    uint8_t ip_flow_3gpp_im_cn_flag;
    uint8_t ip_flow_3gpp_sig_ind;
    uint32_t lte_qos_class_identifier;
} QMI_QOS_generic_global_flow_spec_t;

/* Structure to describe single QoS global filter spec */
typedef struct {
    uint8_t ip_filter_version;
    uint64_t ipv4_valid_params;
    uint32_t ipv4_filter_src_addr;
    uint32_t ipv4_filter_src_mask;
    uint32_t ipv4_filter_dest_addr;
    uint32_t ipv4_filter_dest_mask;
    uint8_t ipv4_tos_val;
    uint8_t ipv4_tos_mask;
    uint64_t ipv6_valid_params;
    uint16_t ipv6_filter_src_addr[8];
    uint8_t ipv6_filter_src_prefix_len;
    uint16_t ipv6_filter_dest_addr[8];
    uint8_t ipv6_filter_dest_prefix_len;
    uint8_t ipv6_tos_val;
    uint8_t ipv6_tos_mask;
    uint32_t ipv6_filter_flow_label;
    uint32_t ipv6_filter_xport_protocol;
    uint64_t tcp_valid_params;
    uint16_t tcp_src_port;
    uint16_t tcp_src_range;
    uint16_t tcp_dest_port;
    uint16_t tcp_dest_range;
    uint64_t udp_valid_params;
    uint16_t udp_src_port;
    uint16_t udp_src_range;
    uint16_t udp_dest_port;
    uint16_t udp_dest_range;
    uint64_t icmp_valid_params;
    uint8_t icmp_type;
    uint8_t icmp_code;
    uint64_t esp_valid_params;
    uint32_t esp_spi;
    uint64_t ah_valid_params;
    uint32_t ah_spi;
    uint16_t ah_filter_id;
    uint16_t ah_filter_precedence;
} QMI_QOS_generic_single_filter_spec_t;

/* Structure to describe generic QoS global filter spec */
typedef struct {
    uint8_t qos_filters_size;
    /* QMI_QOS_generic_single_filter_spec_t filter[tx_qos_filters_size]; */
} QMI_QOS_generic_global_filter_spec_t;

typedef struct {
    uint64_t flow_valid_params;
    uint32_t ip_flow_trf_cls;
    uint64_t data_rate_max;
    uint64_t guaranteed_rate;
    uint32_t peak_rate;
    uint32_t token_rate;
    uint32_t bucket_size;
    uint32_t ip_flow_latency;
    uint32_t ip_flow_jitter;
    uint16_t ip_flow_pkt_error_rate_multiplier;
    uint16_t ip_flow_pkt_error_rate_exponent;
    uint32_t ip_flow_min_policed_packet_size;
    uint32_t ip_flow_max_allowed_packet_size;
    uint32_t ip_flow_3gpp_residual_bit_error_rate;
    uint32_t ip_flow_3gpp_traffic_handling_priority;
    uint16_t ip_flow_3gpp2_profile_id;
    uint8_t ip_flow_3gpp2_flow_priority;
    uint8_t ip_flow_3gpp_im_cn_flag;
    uint8_t ip_flow_3gpp_sig_ind;
    uint32_t ip_flow_lte_qci;
} QMI_QOS_generic_granted_flow_t;

typedef struct {
    uint8_t ip_version;
    uint64_t valid_params_ipv4;
    uint32_t ipv4_addr_src;
    uint32_t ipv4_mask_src;
    uint32_t ipv4_addr_dest;
    uint32_t ipv4_mask_dest;
    uint8_t ipv4_tos_val;
    uint8_t ipv4_tos_mask;
    uint64_t valid_params_ipv6;
    uint16_t ipv6_addr_src[8];
    uint8_t ipv6_prefix_len_src;
    uint16_t ipv6_addr_dest[8];
    uint8_t ipv6_prefix_len_dest;
    uint8_t ipv6_traffic_class_val;
    uint8_t ipv6_traffic_class_mask;
    uint32_t ipv6_flow_label;
    uint32_t xport_protocol;
    uint64_t valid_params_tcp;
    uint16_t port_src_tcp;
    uint16_t range_src_tcp;
    uint16_t port_dest_tcp;
    uint16_t range_dest_tcp;
    uint64_t valid_params_udp;
    uint16_t port_src_udp;
    uint16_t range_src_udp;
    uint16_t port_dest_udp;
    uint16_t range_dest_udp;
    uint64_t valid_params_icmp;
    uint8_t icmp_type;
    uint8_t icmp_code;
    uint64_t valid_params_esp;
    uint32_t esp_spi;
    uint64_t valid_params_ah;
    uint32_t ah_spi;
    uint16_t filter_id;
    uint16_t filter_precedence;
} QMI_QOS_generic_filter_spec_t;

typedef struct {
    uint8_t qos_filter_size;
    /* QMI_QOS_generic_filter_spec_t arr[qos_filter_size] */
} QMI_QOS_generic_filter_specs_t;

typedef struct {
    uint32_t qci;
} QMI_QOS_generic_nr5g_qci_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_QOS_H_ */
