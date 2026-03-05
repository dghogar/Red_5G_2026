#ifndef TELIT_SDK_PRIVATE_WDS_H_
#define TELIT_SDK_PRIVATE_WDS_H_

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_WDS_SET_EVENT_REPORT (1 - 0x0001)
 */

/* Structure to describe QMI_WDS_SET_EVENT_REPORT request TLV 0x11 */
typedef struct {
    uint8_t stats_period;
    uint32_t stats_mask;
} QMI_WDS_SET_EVENT_REPORT_request_0x11_t;

/* Structure to describe QMI_WDS_SET_EVENT_REPORT request TLV 0x12 */
typedef struct {
    uint8_t report_data_bearer_tech;
} QMI_WDS_SET_EVENT_REPORT_request_0x12_t;

/* Structure to describe QMI_WDS_SET_EVENT_REPORT request TLV 0x13 */
typedef struct {
    uint8_t report_dormancy_status;
} QMI_WDS_SET_EVENT_REPORT_request_0x13_t;

/* Structure to describe QMI_WDS_SET_EVENT_REPORT request TLV 0x14 */
typedef struct {
    uint8_t report_mip_status;
} QMI_WDS_SET_EVENT_REPORT_request_0x14_t;

/* Structure to describe QMI_WDS_SET_EVENT_REPORT request TLV 0x15 */
typedef struct {
    uint8_t report_current_data_bearer_tech;
} QMI_WDS_SET_EVENT_REPORT_request_0x15_t;

/* Structure to describe QMI_WDS_SET_EVENT_REPORT request TLV 0x17 */
typedef struct {
    uint8_t report_data_call_status_change;
} QMI_WDS_SET_EVENT_REPORT_request_0x17_t;

/* Structure to describe QMI_WDS_SET_EVENT_REPORT request TLV 0x1A */
typedef struct {
    uint8_t report_data_system_status;
} QMI_WDS_SET_EVENT_REPORT_request_0x1A_t;

/* Structure to describe QMI_WDS_SET_EVENT_REPORT request TLV 0x1E */
typedef struct {
    uint8_t report_data_bearer_tech_ext;
} QMI_WDS_SET_EVENT_REPORT_request_0x1E_t;


/*****************************************************************************
 * QMI_WDS_INDICATION_REGISTER (3 - 0x0003)
 */

/* Structure to describe QMI_WDS_INDICATION_REGISTER request TLV 0x1E */
typedef struct {
    uint8_t report_apn_change_info;
} QMI_WDS_INDICATION_REGISTER_request_0x1E_t;

/*****************************************************************************
 * QMI_WDS_START_NETWORK_INTERFACE (32 - 0x0020)
 * QMI_WDS_STOP_NETWORK_INTERFACE (33 - 0x0021)
 */

/* Structure to describe generic authentication preference */
typedef struct {
    uint8_t authentication_pref;
} QMI_WDS_generic_authentication_pref_t;

/* Structure to describe generic ip family */
typedef struct {
    uint8_t ip_family;
} QMI_WDS_generic_ip_family_t;

/* Structure to describe generic profile index */
typedef struct {
    uint8_t profile_index;
} QMI_WDS_generic_profile_index_t;

/* Structure to describe QMI_WDS_START_NETWORK_INTERFACE request TLV 0x30 */
typedef struct {
    uint8_t technology_preference;
} QMI_WDS_START_NETWORK_INTERFACE_request_0x30_t;

/* Structure to describe QMI_WDS_START_NETWORK_INTERFACE request TLV 0x39 */
typedef struct {
    uint8_t disallow_in_roaming;
} QMI_WDS_START_NETWORK_INTERFACE_request_0x39_t;

/* Structure to describe QMI_WDS_START_NETWORK_INTERFACE request TLV 0x3C */
typedef struct {
    uint8_t bring_up_by_apn_name;
} QMI_WDS_START_NETWORK_INTERFACE_request_0x3C_t;

/* Structure to describe QMI_WDS generic packet data handle */
typedef struct {
    uint32_t pkt_data_handle;
} QMI_WDS_generic_pkt_data_handle_t;

/* Structure to describe generic call end reason */
typedef struct {
    uint16_t call_end_reason;
} QMI_WDS_generic_call_end_reason_t;

/* Structure to describe generic verbose call end reason */
typedef struct {
    uint16_t call_end_reason_type;
    uint16_t call_end_reason;
} QMI_WDS_generic_verbose_call_end_reason_t;

/* Structure to describe generic mux id TLV */
typedef struct {
    uint8_t mux_id;
} QMI_WDS_generic_mux_id_t;

/*****************************************************************************
 * QMI_WDS_GET_PKT_SRVC_STATUS (34 - 0x0022)
 */

/* Structure to describe QMI_WDS_GET_PKT_SRVC_STATUS TLV 0x01 */
typedef struct {
    uint8_t connection_status;
} QMI_WDS_GET_PKT_SRVC_STATUS_response_0x01_t;

/*****************************************************************************
 * QMI_WDS_GET_PKT_STATISTICS (36 - 0x0024)
 */

/* Structure to describe QMI_WDS_GET_PKT_STATISTICS request TLV 0x01 */
typedef struct {
    uint32_t stats_mask;
} QMI_WDS_GET_PKT_STATISTICS_request_0x01_t;

/* Structure to describe generic 32 bit value for packet statistics */
typedef struct {
    uint32_t value_32;
} QMI_WDS_generic_packet_stats_value_32_t;

/* Structure to describe generic 64 bit value for packet statistics */
typedef struct {
    uint64_t value_64;
} QMI_WDS_generic_packet_stats_value_64_t;

/*****************************************************************************
 * QMI_WDS_CREATE_PROFILE (39 - 0x0027)
 * QMI_WDS_MODIFY_PROFILE_SETTINGS (40 - 0x0028)
 * QMI_WDS_DELETE_PROFILE (41 - 0x0029)
 */

/* Structure to describe generic profile type */
typedef struct {
    uint8_t profile_type;
} QMI_WDS_generic_profile_type_0x01_t;

/* Structure to describe generic profile details TLV 0x01 */
typedef struct {
    uint8_t profile_type;
    uint8_t profile_index;
} QMI_WDS_generic_profile_details_0x01_t;

/* Structure to describe generic profile details TLV 0x11 */
typedef struct {
    uint8_t pdp_type;
} QMI_WDS_generic_profile_details_0x11_t;

/* Structure to describe generic profile details TLV 0x12 */
typedef struct {
    uint8_t pdp_hdr_comp_type;
} QMI_WDS_generic_profile_details_0x12_t;

/* Structure to describe generic profile details TLV 0x13 */
typedef struct {
    uint8_t pdp_data_comp_type;
} QMI_WDS_generic_profile_details_0x13_t;

/* Structure to describe generic profile details TLV 0x15 */
typedef struct {
    uint32_t pri_dns_ipv4_addr_pref;
} QMI_WDS_generic_profile_details_0x15_t;

/* Structure to describe generic profile details TLV 0x16 */
typedef struct {
    uint32_t sec_dns_ipv4_addr_pref;
} QMI_WDS_generic_profile_details_0x16_t;

/* Structure to describe generic umts qos info */
typedef struct {
    uint8_t traffic_class;
    uint32_t max_uplink_bitrate;
    uint32_t max_downlink_bitrate;
    uint32_t guaranteed_uplink_bitrate;
    uint32_t guaranteed_downlink_bitrate;
    uint8_t qos_delivery_order;
    uint32_t max_sdu_size;
    uint8_t sdu_error_ratio;
    uint8_t residual_bit_error_ratio;
    uint8_t delivery_erroneous_SDUs;
    uint32_t transfer_delay;
    uint32_t traffic_handling_priority;
} QMI_WDS_generic_umts_qos_t;

/* Structure to describe generic gprs qos info */
typedef struct {
    uint32_t precedence_class;
    uint32_t delay_class;
    uint32_t reliability_class;
    uint32_t peak_throughput_class;
    uint32_t mean_throughput_class;
} QMI_WDS_generic_gprs_qos_t;

/* Structure to describe generic ipv4 address */
typedef struct {
    uint32_t ipv4_addr;
} QMI_WDS_generic_ipv4_addr_t;

/* Structure to describe generic profile details TLV 0x1F */
typedef struct {
    uint8_t pcscf_addr_using_pco;
} QMI_WDS_generic_pcscf_addr_with_pco_t;

/* Structure to describe generic profile details TLV 0x20 */
typedef struct {
    uint8_t pdp_access_control_flag;
} QMI_WDS_generic_profile_details_0x20_t;

/* Structure to describe generic profile details TLV 0x21 */
typedef struct {
    uint8_t pcscf_addr_using_dhcp;
} QMI_WDS_generic_profile_details_0x21_t;

/* Structure to describe generic profile IM CN flag */
typedef struct {
    uint8_t im_cn_flag;
} QMI_WDS_generic_im_cn_flag_t;

/* Structure to describe generic traffic flow template parameters */
typedef struct {
    uint8_t filter_id;
    uint8_t eval_id;
    uint8_t ip_version;
    uint8_t source_ip[16];
    uint8_t source_ip_mask;
    uint8_t next_header;
    uint16_t dest_port_range_start;
    uint16_t dest_port_range_end;
    uint16_t src_port_range_start;
    uint16_t src_port_range_end;
    uint32_t ipsec_spi;
    uint16_t tos_mask;
    uint32_t flow_label;
} QMI_WDS_generic_tft_parameters_t;

/* Structure to describe generic profile details TLV 0x25 */
typedef struct {
    uint8_t pdp_context;
} QMI_WDS_generic_profile_details_0x25_t;

/* Structure to describe generic profile details TLV 0x26 */
typedef struct {
    uint8_t secondary_flag;
} QMI_WDS_generic_profile_details_0x26_t;

/* Structure to describe generic profile details TLV 0x27 */
typedef struct {
    uint8_t primary_id;
} QMI_WDS_generic_profile_details_0x27_t;

/* Structure to describe generic umts qos info with indication flag */
typedef struct {
    uint8_t traffic_class;
    uint32_t max_uplink_bitrate;
    uint32_t max_downlink_bitrate;
    uint32_t guaranteed_uplink_bitrate;
    uint32_t guaranteed_downlink_bitrate;
    uint8_t qos_delivery_order;
    uint32_t max_sdu_size;
    uint8_t sdu_error_ratio;
    uint8_t residual_bit_error_ratio;
    uint8_t delivery_erroneous_SDUs;
    uint32_t transfer_delay;
    uint32_t traffic_handling_priority;
    uint8_t sig_ind;
} QMI_WDS_generic_umts_qos_with_ind_t;

/* Structure to describe generic profile details TLV 0x2D */
typedef struct {
    uint8_t addr_allocation_pref;
} QMI_WDS_generic_profile_details_0x2D_t;

/* Structure to describe generic profile details TLV 0x2E */
typedef struct {
    uint8_t qci;
    uint32_t g_dl_bit_rate;
    uint32_t max_dl_bit_rate;
    uint32_t g_ul_bit_rate;
    uint32_t max_ul_bit_rate;
} QMI_WDS_generic_profile_details_0x2E_t;

/* Structure to describe generic profile details TLV 0x2F */
typedef struct {
    uint8_t apn_disabled_flag;
} QMI_WDS_generic_profile_details_0x2F_t;

/* Structure to describe generic profile details TLV 0x30 */
typedef struct {
    uint32_t pdn_inactivity_timeout;
} QMI_WDS_generic_profile_details_0x30_t;

/* Structure to describe generic profile details TLV 0x31 */
typedef struct {
    uint8_t apn_class;
} QMI_WDS_generic_profile_details_0x31_t;

/* Structure to describe generic profile details TLV 0x36 (QC) or 0x33 (GMS) */
typedef struct {
    uint8_t support_emergency_calls;
} QMI_WDS_generic_profile_details_emergency_t;

/* Structure to describe generic profile id */
typedef struct QMI_WDS_generic_profile_id {
    uint8_t profile_type;
    uint8_t profile_index;
} QMI_WDS_generic_profile_id_t;

/* Structure to describe generic response TLV 0xE0 */
typedef struct QMI_WDS_generic_resp_0xE0 {
    uint16_t extended_error_code;
} QMI_WDS_generic_resp_0xE0_t;

/*****************************************************************************
 * QMI_WDS_GET_PROFILE_LIST (42 - 0x002A)
 */

/* Structure to describe QMI_WDS_GET_PROFILE_LIST request TLV 0x11 */
typedef struct {
    uint32_t search_type;
} QMI_WDS_GET_PROFILE_LIST_request_0x11_t;

/* Structure to describe profile instance in profile list item */
typedef struct {
    uint8_t profile_type;
    uint8_t profile_index;
    uint8_t profile_name_len;
    /* This array must be the size specified by profile_name_len */
    /* char profile_name[profile_name_len]; */
} QMI_WDS_profile_list_instance_t;

/* Structure to describe QMI_WDS_GET_PROFILE_LIST response TLV 0x01 */
typedef struct {
    uint8_t profile_list_size;
    /* This array must be the size specified by profile_list_size */
    /* QMI_WDS_profile_list_instance_t profile_list[profile_list_size]; */
} QMI_WDS_GET_PROFILE_LIST_response_0x01_t;

/*****************************************************************************
 * QMI_WDS_GET_PROFILE_SETTINGS (43 - 0x002B)
 */

/* Structure to describe generic profile details TLV 0x28 */
typedef struct {
    uint8_t ipv6_addr_pref[16];
} QMI_WDS_generic_profile_details_0x28_t;

/* Structure to describe generic profile details TLV 0x2B */
typedef struct {
    uint8_t pri_dns_ipv6_addr_pref[16];
} QMI_WDS_generic_profile_details_0x2B_t;

/* Structure to describe generic profile details TLV 0x2C */
typedef struct {
    uint8_t sec_dns_ipv6_addr_pref[16];
} QMI_WDS_generic_profile_details_0x2C_t;

/*****************************************************************************
 * QMI_WDS_GET_RUNTIME_SETTINGS (45 - 0x002D)
 */

/* Structure to describe QMI_WDS_GET_RUNTIME_SETTINGS request TLV 0x10 */
typedef struct {
    uint32_t requested_settings;
} QMI_WDS_GET_RUNTIME_SETTINGS_request_0x10_t;

/* Structure to describe QMI_WDS_GET_RUNTIME_SETTINGS response TLV 0x23 */
typedef struct {
    uint8_t pcscf_ipv4_address_size;
    /* This array must be the size specified by pcscf_ipv4_address_size */
    /* uint32_t pcscf_ipv4_address[pcscf_ipv4_address_size]; */
} QMI_WDS_GET_RUNTIME_SETTINGS_response_0x23_t;

/* Structure to describe generic fqdn item */
typedef struct {
    uint16_t fqdn_len;
    /* This array must be the size specified by domain_name_len */
    /* char fqdn[fqdn_len]; */
} QMI_WDS_generic_fqdn_t;

/* Structure to describe QMI_WDS_GET_RUNTIME_SETTINGS response TLV 0x24 */
typedef struct {
    uint8_t fqdn_size;
    /* This array must be the size specified by fqdn_size */
    /* QMI_WDS_generic_fqdn_t arr[fqdn_size]; */
} QMI_WDS_GET_RUNTIME_SETTINGS_response_0x24_t;

/* Structure to describe generic ipv6 address with prefix */
typedef struct {
    /* NOTE: qmi specs define the following 16 * uint8_t, but seems to be wrong */
    uint16_t ipv6_addr[8];
    uint8_t ipv6_prefix_length;
} QMI_WDS_generic_ipv6_addr_with_prefix_t;

/* Structure to describe generic ipv6 address */
typedef struct {
    /* NOTE: qmi specs define the following 16 * uint8_t, but seems to be wrong */
    uint16_t ipv6_addr[8];
} QMI_WDS_generic_ipv6_addr_t;

/* Structure to describe QMI_WDS_GET_RUNTIME_SETTINGS response TLV 0x29 */
typedef struct {
    uint32_t mtu;
} QMI_WDS_GET_RUNTIME_SETTINGS_response_0x29_t;

/* Structure to describe generic domain name */
typedef struct {
    uint16_t domain_name_len;
    /* This array must be the size specified by domain_name_len */
    /* char domain_name[domain_name_len]; */
} QMI_WDS_generic_domain_name_t;

/* Structure to describe QMI_WDS_GET_RUNTIME_SETTINGS response TLV 0x2A */
typedef struct {
    uint8_t domain_name_list_size;
    /* This array must be the size specified by domain_name_list_size */
    /* QMI_WDS_generic_domain_name_t arr[domain_name_list_size]; */
} QMI_WDS_GET_RUNTIME_SETTINGS_response_0x2A_t;

/* Structure to describe QMI_WDS_GET_RUNTIME_SETTINGS response TLV 0x2E */
typedef struct {
    uint8_t pcscf_ipv6_addr_list_sz;
    /* This array must be the size specified by domain_name_list_size */
    /* QMI_WDS_generic_ipv6_addr_t arr[pcscf_ipv6_addr_list_sz]; */
} QMI_WDS_GET_RUNTIME_SETTINGS_response_0x2E_t;

/* Structure to describe generic technology name */
typedef struct {
    uint16_t technology_name;
} QMI_WDS_generic_tech_name_t;

/*****************************************************************************
 * QMI_WDS_GET_CALL_DURATION (53 - 0x0035)
 */

/* Structure to describe QMI_WDS_GET_CALL_DURATION response TLV 0x01 */
typedef struct {
    uint64_t call_duration;
} QMI_WDS_GET_CALL_DURATION_response_0x01_t;

/*****************************************************************************
 * QMI_WDS_GET_DEFAULT_PROFILE_NUM (73 - 0x0049)
 */

/* Structure to describe QMI_WDS_GET_DEFAULT_PROFILE_NUM request TLV 0x01 */
typedef struct {
    uint8_t profile_type;
    uint8_t profile_family;
} QMI_WDS_GET_DEFAULT_PROFILE_NUM_request_0x01_t;

/* Structure to describe QMI_WDS_GET_DEFAULT_PROFILE_NUM response TLV 0x01 */
typedef struct {
    uint8_t profile_index;
} QMI_WDS_GET_DEFAULT_PROFILE_NUM_response_0x01_t;

/*****************************************************************************
 * QMI_WDS_SET_CLIENT_IP_FAMILY_PREF (77 - 0x004D)
 */

/* Structure to describe QMI_WDS_SET_CLIENT_IP_FAMILY_PREF TLV 0x01 */
typedef struct {
    uint8_t ip_preference;
} QMI_WDS_SET_CLIENT_IP_FAMILY_PREF_request_0x01_t;

/*****************************************************************************
 * QMI_WDS_SET_LTE_ATTACH_PDN_LIST (147 - 0x0093)
 */

/* Structure to describe generic attach PDN list TLV */
typedef struct {
    uint8_t attach_pdn_list_size;
    /* This array must be the size specified by attach_pdn_list_size */
    /* uint16_t arr[attach_pdn_list_size]; */
} QMI_WDS_generic_attach_pdn_list_t;

/* Structure to describe QMI_WDS_SET_LTE_ATTACH_PDN_LIST TLV 0x10 */
typedef struct {
    uint32_t action;
} QMI_WDS_SET_LTE_ATTACH_PDN_LIST_request_0x10_t;

/*****************************************************************************
 * QMI_WDS_BIND_MUX_DATA_PORT (162 - 0x00A2)
 */

/* Structure to describe QMI_WDS_BIND_MUX_DATA_PORT request TLV 0x10 */
typedef struct {
    uint32_t ep_type;
    uint32_t iface_id;
} QMI_WDS_BIND_MUX_DATA_PORT_request_0x10_t;

/*****************************************************************************
 * QMI WDS indications
 */

/* Structure to describe QMI_WDS__EVENT_REPORT indication TLV 0x17 */
typedef struct {
    uint8_t data_bearer_tech;
} QMI_WDS_EVENT_REPORT_indication_0x17_t;

/* Structure to describe QMI_WDS_EVENT_REPORT indication TLV 0x18 */
typedef struct {
    uint8_t dormancy_status;
} QMI_WDS_EVENT_REPORT_indication_0x18_t;

/* Structure to describe QMI_WDS_EVENT_REPORT indication TLV 0x1B */
typedef struct {
    uint8_t mip_status;
} QMI_WDS_EVENT_REPORT_indication_0x1B_t;

/* Structure to describe QMI_WDS_EVENT_REPORT indication TLV 0x1D */
typedef struct {
    uint8_t current_nw;
    uint32_t rat_mask;
    uint32_t so_mask;
} QMI_WDS_EVENT_REPORT_indication_0x1D_t;

/* Structure to describe QMI_WDS_EVENT_REPORT indication TLV 0x1F */
typedef struct {
    uint8_t data_call_status;
} QMI_WDS_EVENT_REPORT_indication_0x1F_t;

/* Network info instance */
typedef struct {
    uint8_t network;
    uint32_t rat_mask;
    uint32_t so_mask;
} QMI_WDS_EVENT_REPORT_network_info_t;

/* Structure to describe QMI_WDS_EVENT_REPORT indication TLV 0x24 */
typedef struct {
    uint8_t preferred_network;
    uint8_t network_info_size;
    /* This array must be the size specified by network_info_size */
    /* QMI_WDS_SET_EVENT_REPORT_network_info_t network_info[network_info_size]; */
} QMI_WDS_EVENT_REPORT_indication_0x24_t;

/* Structure to describe QMI_WDS_EVENT_REPORT indication TLV 0x2A */
typedef struct {
    uint32_t technology;
    uint32_t rat_value;
    uint64_t so_mask;
} QMI_WDS_EVENT_REPORT_indication_0x2A_t;

/* Structure to describe QMI_WDS_PKT_SRVC_STATUS indication TLV 0x01 */
typedef struct {
    uint8_t connection_status;
    uint8_t reconfiguration_required;
} QMI_WDS_PKT_SRVC_STATUS_indication_0x01_t;

/* Structure to describe QMI_WDS_PKT_SRVC_STATUS indication TLV 0x14 */
typedef struct {
    uint8_t bearer_id;
} QMI_WDS_PKT_SRVC_STATUS_indication_0x14_t;

/* Structure to describe QMI_WDS_APN_PARAM_INFO_CHANGE indication TLV 0x10 */
typedef struct {
    uint8_t old_apn_string_len;
    /* This array must be the size specified by old_apn_string_len */
    /* char old_apn_string[old_apn_string_len]; */
    uint8_t new_apn_string_len;
    /* This array must be the size specified by new_apn_string_len */
    /* char new_apn_string[new_apn_string_len]; */
} QMI_WDS_APN_PARAM_INFO_CHANGE_indication_0x10_t;

/* Structure to describe QMI_WDS_APN_PARAM_INFO_CHANGE indication TLV 0x11 */
typedef struct {
    uint8_t old_pdp_type;
    uint8_t new_pdp_type;
} QMI_WDS_APN_PARAM_INFO_CHANGE_indication_0x11_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_WDS_H_ */
