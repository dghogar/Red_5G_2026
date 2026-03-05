#ifndef TELIT_SDK_PRIVATE_DMS_H_
#define TELIT_SDK_PRIVATE_DMS_H_

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_NAS_SET_EVENT_REPORT (2 - 0x0002)
 */

/* Structure to describe request TLV 0x10 for QMI_NAS_EVENT_REG */
typedef struct {
    int8_t report_signal_strength;
    uint8_t number_of_thresholds;

    /* This array must be the size specified by number_of_thresholds */
    /* int8_t tresholds[number_of_thresholds]; */
} QMI_NAS_EVENT_REG_request_0x10_t;

/*****************************************************************************
 * QMI_NAS_INDICATION_REGISTER (3 - 0x0003)
 */

/* Structure to describe indication TLV 0x13 for QMI_NAS_INDICATION_REGISTER */
typedef struct {
    uint8_t req_serving_system;
} QMI_NAS_INDICATION_REGISTER_request_0x13_t;

/* Structure to describe indication TLV 0x17 for QMI_NAS_INDICATION_REGISTER */
typedef struct {
    uint8_t reg_network_time;
} QMI_NAS_INDICATION_REGISTER_request_0x17_t;

/* Structure to describe indication TLV 0x18 for QMI_NAS_INDICATION_REGISTER */
typedef struct {
    uint8_t req_sys_info;
} QMI_NAS_INDICATION_REGISTER_request_0x18_t;

/* Structure to describe indication TLV 0x19 for QMI_NAS_INDICATION_REGISTER */
typedef struct {
    uint8_t req_sig_info;
} QMI_NAS_INDICATION_REGISTER_request_0x19_t;

/* Structure to describe indication TLV 0x22 for QMI_NAS_INDICATION_REGISTER */
typedef struct {
    uint8_t reg_operator_name_data;
} QMI_NAS_INDICATION_REGISTER_request_0x22_t;

/* Structure to describe indication TLV 0x23 for QMI_NAS_INDICATION_REGISTER */
typedef struct {
    uint8_t reg_csp_plmn_mode_bit;
} QMI_NAS_INDICATION_REGISTER_request_0x23_t;

/* Structure to describe indication TLV 0x4F for QMI_NAS_INDICATION_REGISTER */
typedef struct {
    uint8_t req_reg_nr5g_time_sync_pulse_report;
} QMI_NAS_INDICATION_REGISTER_request_0x4F_t;

/* Structure to describe indication TLV 0x50 for QMI_NAS_INDICATION_REGISTER */
typedef struct {
    uint8_t reg_nr5g_lost_sync_frame;
} QMI_NAS_INDICATION_REGISTER_request_0x50_t;

/* Structure to describe indication TLV 0x5E for QMI_NAS_INDICATION_REGISTER */
typedef struct {
    uint8_t reg_nr5g_utc_time_update;
} QMI_NAS_INDICATION_REGISTER_request_0x5E_t;

/*****************************************************************************
 * QMI_NAS_GET_SIGNAL_STRENGTH (32 - 0x0020)
 */

/* Structure to describe request TLV 0x10 for QMI_NAS_GET_SIGNAL_STRENGTH */
typedef struct {
    uint16_t signal_strength_req_mask;
} QMI_NAS_GET_SIGNAL_STRENGTH_request_0x10_t;

/* Structure to describe response TLV 0x01 for QMI_NAS_GET_SIGNAL_STRENGTH */
typedef struct {
    int8_t signal_strength_dBm;
    uint8_t radio_interface;
} QMI_NAS_GET_SIGNAL_STRENGTH_response_0x01_t;

/* Structure to describe generic signal strength instance */
typedef struct {
    int8_t signal_strength_dBm;
    uint8_t radio_interface;
} signal_strength_instance_t;

/* Structure to describe response TLV 0x10 for QMI_NAS_GET_SIGNAL_STRENGTH */
typedef struct {
    uint16_t number_of_instances;

    /* This array must be the size specified by number_of_instances */
    /* signal_strangth_instance_t signal_strength_list[number_of_instances]; */
} QMI_NAS_GET_SIGNAL_STRENGTH_response_0x10_t;

/* Structure to describe generic ecio instance */
typedef struct ecio {
    uint8_t ECIO;
    uint8_t radio_interface;
} ecio_instance_t;

/* Structure to describe response TLV 0x12 for QMI_NAS_GET_SIGNAL_STRENGTH */
typedef struct {
    uint16_t number_of_instances;

    /* This array must be the size specified by number_of_instances */
    /* ecio_instance_t ecio_list[number_of_instances]; */
} QMI_NAS_GET_SIGNAL_STRENGTH_response_0x12_t;

/* Structure to describe response TLV 0x13 for QMI_NAS_GET_SIGNAL_STRENGTH */
typedef struct {
    uint32_t IO;
} QMI_NAS_GET_SIGNAL_STRENGTH_response_0x13_t;

/* Structure to describe response TLV 0x14 for QMI_NAS_GET_SIGNAL_STRENGTH */
typedef struct {
    uint8_t SINR;
} QMI_NAS_GET_SIGNAL_STRENGTH_response_0x14_t;

/* Structure to describe generic error instance */
typedef struct {
    uint16_t error_rate;
    uint8_t radio_interface;
} error_instance_t;

/* Structure to describe response TLV 0x15 for QMI_NAS_GET_SIGNAL_STRENGTH */
typedef struct {
    uint16_t number_of_instances;

    /* This array must be the size specified by number_of_instances */
    /* error_instance_t error_list[number_of_instances]; */
} QMI_NAS_GET_SIGNAL_STRENGTH_response_0x15_t;

/* Structure to describe response TLV 0x16 for QMI_NAS_GET_SIGNAL_STRENGTH */
typedef struct {
    int8_t RSRQ;
    uint8_t radio_interface;
} QMI_NAS_GET_SIGNAL_STRENGTH_response_0x16_t;

/* Structure to describe response TLV 0x17 for QMI_NAS_GET_SIGNAL_STRENGTH */
typedef struct {
    int16_t LTESNR;
} QMI_NAS_GET_SIGNAL_STRENGTH_response_0x17_t;

/* Structure to describe response TLV 0x18 for QMI_NAS_GET_SIGNAL_STRENGTH */
typedef struct {
    int16_t LTERSRP;
} QMI_NAS_GET_SIGNAL_STRENGTH_response_0x18_t;

/*****************************************************************************
 * QMI_NAS_PERFORM_NETWORK_SCAN (33 - 0x0021)
 */

/* Structure to describe request TLV 0x10 for QMI_NAS_SCAN_NETS */
typedef struct {
    uint8_t network_scan_req_mask;
} QMI_NAS_SCAN_NETS_request_0x10_t;

/* Structure to describe generic network instance */
typedef struct {
    uint16_t mcc;
    uint16_t mnc;
    uint8_t network_status;
    uint8_t description_len;

    /* This array must be the size specified by description_length */
    /* char description[description_len]; */
} network_info_instance_t;

/* Structure to describe generic rat instance */
typedef struct {
    uint16_t mcc;
    uint16_t mnc;
    uint8_t rat;
} rat_info_instance_t;

/* Structure to describe generic pcs instance */
typedef struct {
    uint16_t mcc;
    uint16_t mnc;
    uint8_t mcn_includes_pcs;
} pcs_digit_instance_t;

/* Structure to describe response TLV 0x10 for QMI_NAS_SCAN_NETS */
typedef struct {
    uint16_t number_of_instances;

    /* This array must be the size specified by number_of_instances */
    /* network_info_instance_t network_info_list[number_of_instances]; */
} QMI_NAS_SCAN_NETS_response_0x10_t;

/* Structure to describe response TLV 0x11 for QMI_NAS_SCAN_NETS */
typedef struct {
    uint16_t rat_info_list_size;

    /* This array must be the size specified by rat_info_list_len */
    /* rat_info_instance_t rat_info_list[rat_info_list_len]; */
} QMI_NAS_SCAN_NETS_response_0x11_t;

/* Structure to describe response TLV 0x12 for QMI_NAS_SCAN_NETS */
typedef struct {
    uint16_t pcs_digit_list_size;

    /* This array must be the size specified by pcs_digit_list_len */
    /* pcs_digit_instance_t pcs_digit_list[pcs_digit_list_len]; */
} QMI_NAS_SCAN_NETS_response_0x12_t;

/* Structure to describe request TLV 0x13 for QMI_NAS_SCAN_NETS */
typedef struct {
    uint32_t scan_result;
} QMI_NAS_SCAN_NETS_response_0x13_t;

/*****************************************************************************
 * QMI_NAS_GET_SERVING_SYSTEM (36 - 0x0024)
 */

/* Structure to describe generic roaming indicator instance */
typedef struct {
    uint8_t radio_if;
    uint8_t roaming_indicator;
} roaming_indicator_instance_t;

/* Structure to describe response TLV 0x01 for NAS_GET_SS_INFO */
typedef struct {
    uint8_t registration_state;
    uint8_t CS_attach_state;
    uint8_t PS_attach_state;
    uint8_t registered_network;
    uint8_t number_of_radio_interfaces_in_use;

    /* This array must be the size specified by number_of_radio_interfaces_in_use */
    /* uint8_t radio_interface[number_of_radio_interfaces_in_use]; */
} QMI_NAS_GET_SS_INFO_response_0x01_t;

/* Structure to describe response TLV 0x10 for NAS_GET_SS_INFO */
typedef struct {
    uint8_t roaming_indicator;
} QMI_NAS_GET_SS_INFO_response_0x10_t;

/* Structure to describe response TLV 0x11 for NAS_GET_SS_INFO */
typedef struct {
    uint8_t number_of_data_capabilities;

    /* This array must be the size specified by number_of_data_capabilities */
    /* uint8_t data_capability[number_of_data_capabilities]; */
} QMI_NAS_GET_SS_INFO_response_0x11_t;

/* Structure to describe response TLV 0x12 for NAS_GET_SS_INFO */
typedef struct {
    uint16_t mobile_country_code;
    uint16_t mobile_network_code;
    uint8_t description_len;

    /* This array must be the size specified by description_len */
    /* uint8_t description[description_len]; */
} QMI_NAS_GET_SS_INFO_response_0x12_t;

/* Structure to describe response TLV 0x13 for NAS_GET_SS_INFO */
typedef struct {
    uint16_t system_ID;
    uint16_t network_ID;
} QMI_NAS_GET_SS_INFO_response_0x13_t;

/* Structure to describe response TLV 0x14 for NAS_GET_SS_INFO */
typedef struct {
    uint16_t base_station_ID;
    int32_t latitude;
    int32_t longitude;
} QMI_NAS_GET_SS_INFO_response_0x14_t;

/* Structure to describe response TLV 0x15 for NAS_GET_SS_INFO */
typedef struct {
    uint8_t num_instances;

    /* This arrays must be the size specified by num_instances */
    /* roaming_indicator_instance_t roaming_indicator_list[num_instances]; */
} QMI_NAS_GET_SS_INFO_response_0x15_t;

/* Structure to describe response TLV 0x16 for NAS_GET_SS_INFO */
typedef struct {
    uint8_t roaming_indicator;
} QMI_NAS_GET_SS_INFO_response_0x16_t;

/* Structure to describe response TLV 0x17 for NAS_GET_SS_INFO */
typedef struct {
    uint8_t leapseconds;
    int8_t local_time_offset;
    uint8_t daylight_savings_in_effect;
} QMI_NAS_GET_SS_INFO_response_0x17_t;

/* Structure to describe response TLV 0x18 for NAS_GET_SS_INFO */
typedef struct {
    uint8_t protocol_revision;
} QMI_NAS_GET_SS_INFO_response_0x18_t;

/* Structure to describe response TLV 0x1A for NAS_GET_SS_INFO */
typedef struct {
    int8_t _3gpp_time_zone;
} QMI_NAS_GET_SS_INFO_response_0x1A_t;

/* Structure to describe response TLV 0x1B for NAS_GET_SS_INFO */
typedef struct {
    uint8_t daylight_savings_adjustment;
} QMI_NAS_GET_SS_INFO_response_0x1B_t;

/* Structure to describe response TLV 0x1C for NAS_GET_SS_INFO */
typedef struct {
    uint16_t location_area_code;
} QMI_NAS_GET_SS_INFO_response_0x1C_t;

/* Structure to describe response TLV 0x1D for NAS_GET_SS_INFO */
typedef struct {
    uint32_t cell_ID;
} QMI_NAS_GET_SS_INFO_response_0x1D_t;

/* Structure to describe response TLV 0x1E for NAS_GET_SS_INFO */
typedef struct {
    uint8_t concurrent_service;
} QMI_NAS_GET_SS_INFO_response_0x1E_t;

/* Structure to describe response TLV 0x1F for NAS_GET_SS_INFO */
typedef struct {
    uint8_t PRL_indicator;
} QMI_NAS_GET_SS_INFO_response_0x1F_t;

/* Structure to describe response TLV 0x20 for NAS_GET_SS_INFO */
typedef struct {
    uint8_t DTM;
} QMI_NAS_GET_SS_INFO_response_0x20_t;

/* Structure to describe response TLV 0x21 for NAS_GET_SS_INFO */
typedef struct {
    uint8_t service_status;
    uint8_t service_caps;
    uint8_t hdr_srv_status;
    uint8_t hdr_hybrid;
    uint8_t is_sys_forbidden;
} QMI_NAS_GET_SS_INFO_response_0x21_t;

/* Structure to describe response TLV 0x22 for NAS_GET_SS_INFO */
typedef struct {
    uint16_t MCC;
    uint8_t imsi_11_12;
} QMI_NAS_GET_SS_INFO_response_0x22_t;

/* Structure to describe response TLV 0x23 for NAS_GET_SS_INFO */
typedef struct {
    uint8_t hdr_personality;
} QMI_NAS_GET_SS_INFO_response_0x23_t;

/* Structure to describe response TLV 0x24 for NAS_GET_SS_INFO */
typedef struct {
    uint16_t track_area_code;
} QMI_NAS_GET_SS_INFO_response_0x24_t;

/* Structure to describe response TLV 0x25 for NAS_GET_SS_INFO */
typedef struct {
    uint32_t CS_call_bar_status;
    uint32_t PS_call_bar_status;
} QMI_NAS_GET_SS_INFO_response_0x25_t;

/*****************************************************************************
 * QMI_NAS_GET_HOME_NETWORK (37 - 0x0025)
 */

/* Structure to describe response TLV 0x01 for QMI_NAS_GET_HOME_INFO */
typedef struct {
    uint16_t MCC;
    uint16_t MNC;
    uint8_t description_len;

    /* This array must be the size specified by description_len */
    /* uint8_t description[description_len]; */
} QMI_NAS_GET_HOME_INFO_response_0x01_t;

/* Structure to describe response TLV 0x10 for QMI_NAS_GET_HOME_INFO */
typedef struct {
    uint16_t sid;
    uint16_t nid;
} QMI_NAS_GET_HOME_INFO_response_0x10_t;

/*****************************************************************************
 * QMI_NAS_GET_TECHNOLOGY_PREFERENCE (43 - 0x002B)
 */

/* Structure to describe response TLV 0x01 for QMI_NAS_GET_TECHNOLOGY_PREFERENCE */
typedef struct {
    uint16_t technology_preference;
    uint8_t technology_preference_duration;
} QMI_NAS_GET_NETWORK_PREFERENCE_response_0x01_t;

/* Structure to describe response TLV 0x10 for QMI_NAS_GET_TECHNOLOGY_PREFERENCE */
typedef struct {
    uint16_t persistent;
} QMI_NAS_GET_NETWORK_PREFERENCE_response_0x10_t;

/*****************************************************************************
 * QMI_NAS_GET_RF_BAND_INFO (49 - 0x0031)
 */

/* Structure to describe generic rf info instance */
typedef struct {
    uint8_t radio_interface;
    uint16_t active_band_class;
    uint16_t active_channel;
} QMI_NAS_GET_RF_INFO_instance_t;

/* Structure to describe generic rf info extended instance */
typedef struct
{
    uint8_t radio_interface;
    uint16_t active_band_class;
    uint32_t active_channel;
} QMI_NAS_GET_RF_INFO_extended_instance_t;

/* Structure to describe generic rf info list TLVs (0x01 and 0x11) for QMI_NAS_GET_RF_INFO */
typedef struct {
    uint8_t num_instances;

    /* QMI_NAS_GET_RF_INFO_instance_t instances[num_instances]; */
    /* OR */
    /* QMI_NAS_GET_RF_INFO_extended_instance_t instances[num_instances]; */
} QMI_NAS_GET_RF_INFO_response_t;

/*****************************************************************************
 * QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE (51 - 0x0033)
 * QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE (52 - 0x0034)
 */

/* Structure to describe response TLV 0x10 for QMI_NAS_GET_SYS_SELECT_PREF */
typedef struct {
    uint8_t emergency_mode_on;
} QMI_NAS_generic_emergency_mode_t;

/* Structure to describe generic mode preference */
typedef struct {
    uint16_t mode_pref;
} QMI_NAS_generic_mode_preference_t;

/* Structure to describe generic roaming preference */
typedef struct {
    uint16_t roam_pref;
} QMI_NAS_generic_roaming_preference_t;

/* Structure to describe generic band preference */
typedef struct {
    uint64_t band_pref;
} QMI_NAS_generic_band_preference_t;

/* Structure to describe generic LTE band preference */
typedef struct {
    uint64_t lte_band_pref;
} QMI_NAS_generic_LTE_band_preference_t;

/* Structure to describe response TLV 0x16 for QMI_NAS_GET_SYS_SELECT_PREF */
typedef struct {
    uint8_t net_sel_pref;
} QMI_NAS_GET_SYS_SELECT_PREF_response_0x16_t;

/* Structure to describe generic service domain preference */
typedef struct {
    uint32_t srv_domain_pref;
} QMI_NAS_generic_service_domain_preference_t;

/* Structure to describe generic GSM/WCDMA acquisition order preference */
typedef struct {
    uint32_t gw_acq_order_pref;
} QMI_NAS_generic_gw_acq_order_pref_t;

/* Structure to describe request TLV 0x16 for QMI_NAS_GET_SYS_SELECT_PREF */
typedef struct {
    uint8_t net_sel_pref;
    uint16_t mcc;
    uint16_t mnc;
} QMI_NAS_SET_SYS_SELECT_PREF_request_0x16_t;

/* Structure to describe request TLV 0x17 for QMI_NAS_GET_SYS_SELECT_PREF */
typedef struct {
    uint8_t change_duration;
} QMI_NAS_SET_SYS_SELECT_PREF_request_0x17_t;

/* Structure to describe request TLV 0x1A for QMI_NAS_GET_SYS_SELECT_PREF */
typedef struct {
    uint8_t mnc_includes_pcs_digit;
} QMI_NAS_SET_SYS_SELECT_PREF_request_0x1A_t;

/* Structure to describe request TLV 0x1E for QMI_NAS_GET_SYS_SELECT_PREF */
typedef struct {
    uint8_t acq_order_size;

    /* This array must be the size specified by acq_order_size */
    /* uint8_t acq_order[acq_order_size]; */
} QMI_NAS_SET_SYS_SELECT_PREF_request_0x1E_t;

/* Structure to describe request TLV 0x1F for QMI_NAS_SET_SYS_SELECT_PREF */
typedef struct {
    uint32_t srv_reg_restriction;
} QMI_NAS_SET_SYS_SELECT_PREF_request_0x1F_t;

/* Structure to describe request TLV 0x20 for QMI_NAS_GET_SYS_SELECT_PREF */
typedef struct {
    uint16_t mcc;
    uint16_t mnc;
    uint8_t mnc_includes_pcs_digit;
    uint32_t id;
    uint8_t rat;
} QMI_NAS_SET_SYS_SELECT_PREF_request_0x20_t;

/* Structure to describe request TLV 0x22 for QMI_NAS_GET_SYS_SELECT_PREF */
typedef struct {
    uint8_t rat;
} QMI_NAS_SET_SYS_SELECT_PREF_request_0x22_t;

/* Structure to describe generic lte bands extended */
typedef struct {
    uint64_t bits_1_64;
    uint64_t bits_65_128;
    uint64_t bits_129_192;
    uint64_t bits_193_256;
} QMI_NAS_generic_lte_bands_extended_t;

/* Structure to describe generic nr5g bands */
typedef struct {
    uint64_t bits_1_64;
    uint64_t bits_65_128;
    uint64_t bits_129_192;
    uint64_t bits_193_256;
    uint64_t bits_257_320;
    uint64_t bits_321_384;
    uint64_t bits_385_448;
    uint64_t bits_449_512;
} QMI_NAS_generic_nr5g_bands_t;

/* Structure to describe generic sa nsa mode disable TLV for QMI_NAS_SET_SYS_SELECT_PREF */
typedef struct {
    uint32_t nr5g_sa_nsa_mode_disable;
} QMI_NAS_SET_SYS_SELECT_PREF_generic_sa_nsa_mode_disable_t;

/*****************************************************************************
 * QMI_NAS_GET_CELL_LOCATION_INFO (67 - 0x0043)
 */

/* Structure to describe generic nmr cell info instance */
typedef struct {
    uint32_t nmr_cell_id;
    uint8_t nmr_plmn[3];
    uint16_t nmr_lac;
    uint16_t nmr_arfcn;
    uint8_t nmr_bsic;
    uint16_t nmr_rx_lev;
} nmr_cell_info_instance_t;

/* Structure to describe response TLV 0x10 for QMI_NAS_GET_CELL_LOC_INFO */
typedef struct {
    uint32_t cell_id;
    uint8_t plmn[3];
    uint16_t lac;
    uint16_t arfcn;
    uint8_t bsic;
    uint32_t timing_advance;
    uint16_t rx_lev;
    uint8_t nmr_cell_info_size;

    /* This array must be the size specified by nmr_cell_info_size */
    /* nmr_cell_info_instance_t nmr_cell_info[nmr_cell_info_size]; */
} QMI_NAS_GET_CELL_LOC_INFO_response_0x10_t;

/* Structure to describe generic umts cell info instance */
typedef struct {
    uint16_t umts_uarfcn;
    uint16_t umts_psc;
    int16_t umts_rscp;
    int16_t umts_ecio;
} umts_instance_t;

/* Structure to describe generic geran cell info instance */
typedef struct {
    uint16_t geran_arfcn;
    uint8_t geran_bsic_ncc;
    uint8_t geran_bsic_bcc;
    int16_t geran_rssi;
} geran_instance_t;

/* Structure to describe response TLV 0x11 for QMI_NAS_GET_CELL_LOC_INFO */
typedef struct {
    uint16_t cell_id;
    uint8_t plmn[3];
    uint16_t lac;
    uint16_t uarfcn;
    uint16_t psc;
    int16_t rscp;
    int16_t ecio;
    uint8_t umts_insts_size;
    /* This array must be the size specified by umts_insts_size */
    /* umts_instance_t umts_insts[umts_insts_size]; */

    /* uint8_t geran_insts_len; */
    /* This array must be the size specified by geran_insts_len */
    /* geran_instance_t geran_insts[geran_insts_len]; */
} QMI_NAS_GET_CELL_LOC_INFO_response_0x11_t;

/* Structure to describe generic cell params instance */
typedef struct {
    uint16_t pci;
    int16_t rsrq;
    int16_t rsrp;
    int16_t rssi;
    int16_t srxlev;
} cell_params_instance_t;

/* Structure to describe response TLV 0x13 for QMI_NAS_GET_CELL_LOC_INFO */
typedef struct {
    uint8_t ue_in_idle;
    uint8_t plmn[3];
    uint16_t tac;
    uint32_t global_cell_id;
    uint16_t earfcn;
    uint16_t serving_cell_id;
    uint8_t cell_resel_priority;
    uint8_t s_non_intra_search;
    uint8_t thresh_serving_low;
    uint8_t s_intra_search;
    uint8_t cell_intra_freq_params_size;

    /* This array must be the size specified by cell_intra_freq_params_size */
    /* cell_params_instance_t cell_intra_freq_params[cell_intra_freq_params_size]; */
} QMI_NAS_GET_CELL_LOC_INFO_response_0x13_t;

/* Structure to describe generic inter-freqs cell instance */
typedef struct {
    uint16_t earfcn;
    uint8_t threshX_low;
    uint8_t threshX_high;
    uint8_t cell_resel_priority;
    uint8_t cell_inter_freq_params_size;

    /* This array must be the size specified by cell_inter_freq_params_size */
    /* cell_params_instance_t cell_inter_freq_params[cell_inter_freq_params_size]; */
} inter_freqs_instance_t;

/* Structure to describe response TLV 0x14 for QMI_NAS_GET_CELL_LOC_INFO */
typedef struct
{
    uint8_t ue_in_idle;
    uint8_t inter_freqs_size;

    /* This array must be the size specified by inter_freqs_size */
    /* inter_freqs_instance_t inter_freqs[inter_freqs_size]; */
} QMI_NAS_GET_CELL_LOC_INFO_response_0x14_t;

/* Structure to describe generic gsm cell instance */
typedef struct {
    uint16_t arfcn;
    uint8_t band_1900;
    uint8_t cell_id_valid;
    uint8_t bsic_id;
    int16_t rssi;
    int16_t srxlev;
} gsm_cell_instance_t;

/* Structure to describe generic lte, gsm cell instance */
typedef struct {
    uint8_t cell_resel_priority;
    uint8_t thresh_gsm_high;
    uint8_t thresh_gsm_low;
    uint8_t ncc_permitted;
    uint8_t gsm_cells_size;

    /* This array must be the size specified by gsm_cells_size */
    /* gsm_cell_instance_t gsm_cells[gsm_cells_size]; */
} lte_gsm_cell_instance_t;

/* Structure to describe response TLV 0x15 for QMI_NAS_GET_CELL_LOC_INFO */
typedef struct {
    uint8_t ue_in_idle;
    uint8_t lte_gsm_cells_size;

    /* This array must be the size specified by lte_gsm_cells_size */
    /* lte_gsm_cell_instance_t lte_gsm_cells[lte_gsm_cells_size]; */
} QMI_NAS_GET_CELL_LOC_INFO_response_0x15_t;

/* Structure to describe generic wcdma cell instance */
typedef struct {
    uint16_t psc;
    int16_t cpich_rscp;
    int16_t cpich_ecno;
    int16_t srxlev;
} wcdma_cell_instance_t;

/* Structure to describe generic lte, wcdma cell instance */
typedef struct {
    uint16_t uarfcn;
    uint8_t cell_resel_priority;
    uint16_t thresh_Xhigh;
    uint16_t threshXlow;
    uint8_t wcdma_cells_size;

    /* This array must be the size specified by wcdma_cells_size */
    /* wcdma_cell_instance_t wcdma_cells[wcdma_cells_size]; */
} lte_wcdma_cell_instance_t;

/* Structure to describe response TLV 0x16 for QMI_NAS_GET_CELL_LOC_INFO */
typedef struct {
    uint8_t ue_in_idle;
    uint8_t lte_wcdma_cells_size;

    /* This array must be the size specified by lte_wcdma_cells_size */
    /* lte_wcdma_cell_instance_t lte_wcdma_cells[lte_wcdma_cells_size]; */
} QMI_NAS_GET_CELL_LOC_INFO_response_0x16_t;

/* Structure to describe response TLV 0x17 for QMI_NAS_GET_CELL_LOC_INFO */
typedef struct {
    uint32_t umts_cell_id;
} QMI_NAS_GET_CELL_LOC_INFO_response_0x17_t;

/* Structure to describe generic umts, lte, nbr cell instance */
typedef struct {
    uint16_t earfcn;
    uint16_t pci;
    uint32_t rsrp;
    uint32_t rsrq;
    int16_t srxlev;
    uint8_t cell_is_tdd;
} umts_lte_nbr_cell_instance_t;

/* Structure to describe response TLV 0x18 for QMI_NAS_GET_CELL_LOC_INFO */
typedef struct {
    uint32_t wcdma_rrc_state;
    uint8_t umts_lte_nbr_cell_size;

    /* This array must be the size specified by umts_lte_nbr_cell_size */
    /* lte_wcdma_cell_instance_t umts_lte_nbr_cells[umts_lte_nbr_cell_size]; */
} QMI_NAS_GET_CELL_LOC_INFO_response_0x18_t;

/* Structure to describe response TLV 0x2E for QMI_NAS_GET_CELL_LOC_INFO */
typedef struct {
    uint32_t nr5g_arfcn;
} QMI_NAS_GET_CELL_LOC_INFO_response_0x2E_t;

/* Structure to describe response TLV 0x2F for QMI_NAS_GET_CELL_LOC_INFO */
typedef struct {
    uint8_t plmn[3];
    uint8_t tac[3];
    uint64_t global_cell_id;
    uint16_t pci;
    int16_t rsrq;
    int16_t rsrp;
    int16_t snr;
} QMI_NAS_GET_CELL_LOC_INFO_response_0x2F_t;

/*****************************************************************************
 * QMI_NAS_NR5G_RRC_UTC_TIME_UPDATE_IND (335 - 0x0149)
 * QMI_NAS_GET_NR5G_RRC_UTC_TIME (336 - 0x0150)
 */

/* Structure to describe QMI_NAS_GET_NR5G_RRC_UTC_TIME response TLV 0x10 */
typedef struct {
    uint8_t utc_sib9_acquired;
} QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x10_t;

/* Structure to describe QMI_NAS_GET_NR5G_RRC_UTC_TIME response TLV 0x11 */
typedef struct {
    uint8_t r16_timing_info;
} QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x11_t;

/* Structure to describe QMI_NAS_GET_NR5G_RRC_UTC_TIME response TLV 0x12 */
typedef struct {
    uint32_t day_light_saving_time;
} QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x12_t;

/* Structure to describe QMI_NAS_GET_NR5G_RRC_UTC_TIME response TLV 0x13 */
typedef struct {
    int16_t leap_seconds;
} QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x13_t;

/* Structure to describe QMI_NAS_GET_NR5G_RRC_UTC_TIME response TLV 0x14 */
typedef struct {
    int16_t local_time_offset;
} QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x14_t;

/* Structure to describe QMI_NAS_GET_NR5G_RRC_UTC_TIME response TLV 0x15 */
typedef struct {
    uint32_t ref_days;
    uint32_t ref_seconds;
    uint16_t ref_milliseconds;
    uint32_t ref_ten_nano_seconds;
} QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x15_t;

/* Structure to describe QMI_NAS_GET_NR5G_RRC_UTC_TIME response TLV 0x16 */
typedef struct {
    uint32_t uncertainty;
} QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x16_t;

/* Structure to describe QMI_NAS_GET_NR5G_RRC_UTC_TIME response TLV 0x17 */
typedef struct {
    uint32_t time_info_type;
} QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x17_t;

/* Structure to describe QMI_NAS_GET_NR5G_RRC_UTC_TIME response TLV 0x18 */
typedef struct {
    uint32_t reference_sfn;
} QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x18_t;

/* Structure to describe QMI_NAS_GET_NR5G_RRC_UTC_TIME response TLV 0x19 */
typedef struct {
    uint64_t abs_time;
} QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x19_t;

/* Structure to describe QMI_NAS_GET_NR5G_RRC_UTC_TIME response TLV 0x1A */
typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t millisecond;
    uint8_t day_of_week;
} QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x1A_t;

/*****************************************************************************
 * QMI_NAS_GET_PLMN_NAME (68 - 0x0044)
 */

/* Structure to describe request TLV 0x01 for QMI_NAS_GET_PLMN_NAME */
typedef struct {
    uint16_t mcc;
    uint16_t mnc;
} QMI_NAS_GET_PLMN_NAME_request_0x01_t;

/* Structure to describe request TLV 0x11 for QMI_NAS_GET_PLMN_NAME */
typedef struct {
    uint8_t mnc_includes_pcs_digit;
} QMI_NAS_GET_PLMN_NAME_request_0x11_t;

/* Structure to describe response TLV 0x10 for QMI_NAS_GET_PLMN_NAME */
typedef struct {
    uint8_t spn_enc;
    uint8_t spn_len;

    /* The following are only indicated since are variable in size
       uint8_t spn[spn_len];
       uint8_t plmn_short_name_enc;
       uint8_t plmn_short_name_ci;
       uint8_t plmn_short_spare_bits;
       uint8_t plmn_short_name_len;
       uint8_t plmn_short_name[plmn_short_name_len];
       uint8_t plmn_long_name_enc;
       uint8_t plmn_long_name_ci;
       uint8_t plmn_long_spare_bits;
       uint8_t plmn_long_name_len;
       uint8_t plmn_long_name[plmn_long_name_len]; */
} QMI_NAS_GET_PLMN_NAME_response_0x10_t;

/*****************************************************************************
 * QMI_NAS_NETWORK_TIME_IND (76 - 0x004C)
 */

/* Structure to describe indication TLV 0x01 for QMI_NAS_NETWORK_TIME_IND */
typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t day_of_week;
} QMI_NAS_NETWORK_TIME_IND_0x01_t;

/* Structure to describe indication TLV 0x10 for QMI_NAS_NETWORK_TIME_IND */
typedef struct {
    int8_t time_zone;
} QMI_NAS_NETWORK_TIME_IND_0x10_t;

/* Structure to describe indication TLV 0x11 for QMI_NAS_NETWORK_TIME_IND */
typedef struct {
    uint8_t daylt_sav_adj;
} QMI_NAS_NETWORK_TIME_IND_0x11_t;

/* Structure to describe indication TLV 0x12 for QMI_NAS_NETWORK_TIME_IND */
typedef struct {
    enum8_t radio_if;
} QMI_NAS_NETWORK_TIME_IND_0x12_t;

/*****************************************************************************
 * QMI_NAS_GET_SYS_INFO (77 - 0x004D)
 */

/* Structure to describe response TLV 0x11 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t service_status;
    uint8_t preferred_data_path;
} QMI_NAS_GET_SYSTEM_INFO_response_0x11_t;

/* Structure to describe response TLV 0x12, 0x13, and 0x14 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t service_status;
    uint8_t true_service_status;
    uint8_t preferred_data_path;
} QMI_NAS_generic_service_status_info_t;

/* Structure to describe response TLV 0x16 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t srv_domain_valid;
    uint8_t srv_domain;
    uint8_t srv_capability_valid;
    uint8_t srv_capability;
    uint8_t roam_status_valid;
    uint8_t roam_status;
    uint8_t is_sys_forbidden_valid;
    uint8_t is_sys_forbidden;
    uint8_t is_sys_prl_match_valid;
    uint8_t is_sys_prl_match;
    uint8_t hdr_personality_valid;
    uint8_t hdr_personality;
    uint8_t hdr_active_prot_valid;
    uint8_t hdr_active_prot;
    uint8_t is856_sys_id_valid;
    uint8_t is856_sys_id[16];
} QMI_NAS_GET_SYSTEM_INFO_response_0x16_t;

/* Structure to describe response TLV 0x17 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t srv_domain_valid;
    uint8_t srv_domain;
    uint8_t srv_capability_valid;
    uint8_t srv_capability;
    uint8_t roam_status_valid;
    uint8_t roam_status;
    uint8_t is_sys_forbidden_valid;
    uint8_t is_sys_forbidden;
    uint8_t lac_valid;
    uint16_t lac;
    uint8_t cell_id_valid;
    uint32_t cell_id;
    uint8_t reg_reject_info_valid;
    uint8_t reject_srv_domain;
    uint8_t rej_cause;
    uint8_t network_id_valid;
    uint8_t mcc[3];
    uint8_t mnc[3];
    uint8_t egprs_supp_valid;
    uint8_t egprs_supp;
    uint8_t dtm_supp_valid;
    uint8_t dtm_supp;
} QMI_NAS_GET_SYSTEM_INFO_response_0x17_t;

/* Structure to describe response TLV 0x18 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t srv_domain_valid;
    uint8_t srv_domain;
    uint8_t srv_capability_valid;
    uint8_t srv_capability;
    uint8_t roam_status_valid;
    uint8_t roam_status;
    uint8_t is_sys_forbidden_valid;
    uint8_t is_sys_forbidden;
    uint8_t lac_valid;
    uint16_t lac;
    uint8_t cell_id_valid;
    uint32_t cell_id;
    uint8_t reg_reject_info_valid;
    uint8_t reject_srv_domain;
    uint8_t rej_cause;
    uint8_t network_id_valid;
    uint8_t mcc[3];
    uint8_t mnc[3];
    uint8_t hs_call_status_valid;
    uint8_t hs_call_status;
    uint8_t hs_ind_valid;
    uint8_t hs_ind;
    uint8_t psc_valid;
    uint16_t psc;
} QMI_NAS_GET_SYSTEM_INFO_response_0x18_t;

/* Structure to describe response TLV 0x19 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t srv_domain_valid;
    uint8_t srv_domain;
    uint8_t srv_capability_valid;
    uint8_t srv_capability;
    uint8_t roam_status_valid;
    uint8_t roam_status;
    uint8_t is_sys_forbidden_valid;
    uint8_t is_sys_forbidden;
    uint8_t lac_valid;
    uint16_t lac;
    uint8_t cell_id_valid;
    uint32_t cell_id;
    uint8_t reg_reject_info_valid;
    uint8_t reject_srv_domain;
    uint8_t rej_cause;
    uint8_t network_id_valid;
    uint8_t mcc[3];
    uint8_t mnc[3];
    int8_t tac_valid;
    uint16_t tac;
} QMI_NAS_GET_SYSTEM_INFO_response_0x19_t;

/* Structure to describe response TLV 0x1B for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint16_t reg_prd;
} QMI_NAS_GET_SYSTEM_INFO_response_0x1B_t;

/* Structure to describe response TLV 0x1C and 0x1D for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint16_t geo_sys_idx;
    uint32_t cell_broadcast_cap;
} QMI_NAS_generic_additional_system_info_t;

/* Structure to describe response TLV 0x1E for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint16_t geo_sys_idx;
} QMI_NAS_GET_SYSTEM_INFO_response_0x1E_t;

/* Structure to describe response TLV 0x1F and 0x20 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint32_t cs_bar_status;
    uint32_t ps_bar_status;
} QMI_NAS_generic_call_barring_info_t;

/* Structure to describe response TLV 0x21 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t voice_support_on_lte;
} QMI_NAS_GET_SYSTEM_INFO_response_0x21_t;

/* Structure to describe response TLV 0x22 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t gsm_cipher_domain;
} QMI_NAS_GET_SYSTEM_INFO_response_0x22_t;

/* Structure to describe response TLV 0x23 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t wcdma_cipher_domain;
} QMI_NAS_GET_SYSTEM_INFO_response_0x23_t;

/* Structure to describe response TLV 0x4B for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t srv_domain_valid;
    uint8_t srv_domain;
    uint8_t srv_capability_valid;
    uint8_t srv_capability;
    uint8_t roam_status_valid;
    uint8_t roam_status;
    uint8_t is_sys_forbidden_valid;
    uint8_t is_sys_forbidden;
    uint8_t lac_valid;
    uint16_t lac;
    uint8_t cell_id_valid;
    uint32_t cell_id;
    uint8_t reg_reject_info_valid;
    uint8_t reject_srv_domain;
    uint8_t rej_cause;
    uint8_t network_id_valid;
    uint8_t mcc[3];
    uint8_t mnc[3];
    int8_t tac_valid;
    uint16_t tac;
} QMI_NAS_GET_SYSTEM_INFO_response_0x4B_t;

/* Structure to describe response TLV 0x4C for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    int32_t nr5g_cell_status;
} QMI_NAS_GET_SYSTEM_INFO_response_0x4C_t;

/* Structure to describe response TLV 0x4E for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t endc_availability;
} QMI_NAS_GET_SYSTEM_INFO_response_0x4E_t;

/* Structure to describe response TLV 0x4F for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t restrict_dcnr;
} QMI_NAS_GET_SYSTEM_INFO_response_0x4F_t;

/* Structure to describe response TLV 0x50 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t tac[3];
} QMI_NAS_GET_SYSTEM_INFO_response_0x50_t;

/* Structure to describe response TLV 0x54 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint16_t nr5g_pci;
} QMI_NAS_GET_SYSTEM_INFO_response_0x54_t;

/* Structure to describe response TLV 0x55 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t plmn_infolist_r15_availability;
} QMI_NAS_GET_SYSTEM_INFO_response_0x55_t;

/* Structure to describe response TLV 0x5E for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint64_t nr5g_cell_id;
} QMI_NAS_GET_SYSTEM_INFO_response_0x5E_t;

/* Structure to describe response TLV 0x5F for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint8_t radio_if;
    uint8_t mcc[3];
    uint8_t mnc[3];
} QMI_NAS_GET_SYSTEM_INFO_response_0x5F_t;

/* Structure to describe response TLV 0x60 for QMI_NAS_GET_SYSTEM_INFO */
typedef struct {
    uint32_t nr5g_arfcn;
} QMI_NAS_GET_SYSTEM_INFO_response_0x60_t;

/*****************************************************************************
 * QMI_NAS_GET_SIG_INFO (79 - 0x004F)
 */

/* Structure to describe response TLV 0x12 for QMI_NAS_GET_SIGNAL_INFO */
typedef struct {
    int8_t RSSI;
} QMI_NAS_GET_SIGNAL_INFO_response_0x12_t;

/* Structure to describe response TLV 0x13 for QMI_NAS_GET_SIGNAL_INFO */
typedef struct {
    int8_t RSSI;
    uint16_t ECIO;
} QMI_NAS_GET_SIGNAL_INFO_response_0x13_t;

/* Structure to describe response TLV 0x14 for QMI_NAS_GET_SIGNAL_INFO */
typedef struct {
    int8_t RSSI;
    int8_t RSRQ;
    int16_t RSRP;
    int16_t SNR;
} QMI_NAS_GET_SIGNAL_INFO_response_0x14_t;

/* Structure to describe response TLV 0x17 for QMI_NAS_GET_SIGNAL_INFO */
typedef struct {
    int16_t RSRP;
    int16_t SNR;
} QMI_NAS_GET_SIGNAL_INFO_response_0x17_t;

/*****************************************************************************
 * QMI_NAS_GET_TX_RX_INFO (90 - 0x005A)
 */

/* Structure to describe request TLV 0x01 for QMI_NAS_GET_TX_RX_INFO */
typedef struct {
    uint8_t radio_if;
} QMI_NAS_GET_TX_RX_INFO_request_0x01_t;

/* Structure to describe TLV 0x10 and 0x11 for QMI_NAS_GET_TX_RX_INFO response */
typedef struct {
    uint8_t is_radio_tuned;
    int32_t rx_pwr;
    int32_t ecio;
    int32_t rscp;
    int32_t rsrp;
    uint32_t phase;
} QMI_NAS_GET_TX_RX_INFO_response_0x10_0x11_t;

/* Structure to describe TLV 0x12 for QMI_NAS_GET_TX_RX_INFO response */
typedef struct {
    uint8_t is_in_traffic;
    int32_t tx_pwr;
} QMI_NAS_GET_TX_RX_INFO_response_0x12_t;

/*****************************************************************************
 * QMI_NAS_CONFIG_SIG_INFO2 (108 - 0x006C)
 */

/* Structure to describe generic threshold list for QMI_NAS_CONFIG_SIG_INFO2 */
typedef struct {
    uint8_t threshold_list_size;

    /* This array must be the size specified by threshold_list_size */
    /* int16_t threshold_list[threshold_list_size]; */
} QMI_NAS_generic_threshold_list_t;

/* Structure to describe generic delta for QMI_NAS_CONFIG_SIG_INFO2 */
typedef struct {
    uint16_t delta;
} QMI_NAS_generic_delta_t;

/* Structure to describe generic signal rate for QMI_NAS_CONFIG_SIG_INFO2 */
typedef struct {
    uint8_t rpt_rate;
    uint8_t avg_period;
} QMI_NAS_generic_signal_rate_t;

/* Structure to describe generic hysteresis list item for QMI_NAS_CONFIG_SIG_INFO2 */
typedef struct {
    uint32_t signal;
    uint16_t delta;
} QMI_NAS_generic_hysteresis_item_t;

/* Structure to describe generic hysteresis list for QMI_NAS_CONFIG_SIG_INFO2 */
typedef struct {
    uint8_t hysteresis_list_size;

    /* This array must be the size specified by hysteresis_list_size */
    /* QMI_NAS_generic_hysteresis_item_t hysteresis_list[hysteresis_list_size]; */
} QMI_NAS_generic_hysteresis_list_t;

/* Structure to describe generic hysteresis timer for QMI_NAS_CONFIG_SIG_INFO2 */
typedef struct {
    uint16_t hysteresis_timer;
} QMI_NAS_generic_hysteresis_timer_t;

/*****************************************************************************
 * QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING (112 - 0x0070)
 */

/* Structure to describe request TLV 0x01 for QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING */
typedef struct {
    uint64_t limit_sys_info_chg_rpt;
} QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING_request_0x01_t;


/*****************************************************************************
 * QMI_NAS_GET_NETWORK_TIME (125 - 0x007D)
 */
/* Structure to describe response TLV 0x10 and 0x11 for QMI_NAS_GET_NETWORK_TIME */
typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t day_of_week;
    int8_t time_zone;
    uint8_t daylt_sav_adj;
    uint8_t radio_if;
} QMI_NAS_generic_time_info_t;

/*****************************************************************************
 * QMI_NAS_GET_LTE_CPHY_CA_INFO (172 - 0x00AC)
 */

/* Structure to describe response TLV 0x10 for QMI_NAS_GET_LTE_CPHY_CA_INFO */
typedef struct {
    uint16_t pci;
    uint16_t freq;
    uint32_t scell_state;
} QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x10_t;

/* Structure to describe response TLV 0x11 for QMI_NAS_GET_LTE_CPHY_CA_INFO */
typedef struct {
    uint32_t cphy_ca_dl_bandwidth;
} QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x11_t;

/* Structure to describe response TLV 0x13 for QMI_NAS_GET_LTE_CPHY_CA_INFO */
typedef struct {
    uint16_t pci;
    uint16_t freq;
    uint32_t cphy_ca_dl_bandwidth;
    uint16_t band;
} QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x13_t;

/* Structure to describe generic scell info instance */
typedef struct {
    uint16_t pci;
    uint16_t freq;
    uint32_t cphy_ca_dl_bandwidth;
    uint16_t band;
    uint32_t scell_state;
    uint8_t scell_idx;
} cphy_scell_info_instance_t;

/* Structure to describe response TLV 0x15 for QMI_NAS_GET_LTE_CPHY_CA_INFO */
typedef struct {
    uint8_t cphy_scell_info_list_size;

    /* This array must be the size specified by cphy_scell_info_list_size */
    /* cphy_scell_info_instance_t cphy_scell_info_list[cphy_scell_info_list_size]; */
} QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x15_t;

/*****************************************************************************
 * QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND
 */

/* Structure to describe indication TLV 0x10 for QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND */
typedef struct {
    uint64_t gps_time;
} QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x10_t;

/* Structure to describe indication TLV 0x11 for QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND */
typedef struct {
    uint32_t sfn;
} QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x11_t;

/* Structure to describe indication TLV 0x12 for QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND */
typedef struct {
    int32_t nta;
} QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x12_t;

/* Structure to describe indication TLV 0x13 for QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND */
typedef struct {
    uint32_t nta_offset;
} QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x13_t;

/* Structure to describe indication TLV 0x14 for QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND */
typedef struct {
    uint8_t leapseconds;
} QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x14_t;

/* Structure to describe indication TLV 0x15 for QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND */
typedef struct {
    uint64_t utc_time;
} QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x15_t;

/*****************************************************************************
 * QMI_NAS_NR5G_LOST_FRAME_SYNC_IND
 */

/** Structure to describe indication TLV 0x10 for QMI_NAS_NR5G_LOST_FRAME_SYNC_IND */
typedef struct {
    enum32_t nr5g_sync_lost_reason;
} QMI_NAS_NR5G_LOST_FRAME_SYNC_IND_0x10_t;

/*****************************************************************************
 * QMI_NAS_SET_NR5G_SYNC_PULSE_GEN
 */

/* Structure to describe request TLV 0x01 for QMI_NAS_SET_NR5G_SYNC_PULSE_GEN */
typedef struct {
    uint32_t pulse_period;
} QMI_NAS_SET_NR5G_SYNC_PULSE_GEN_0x01_t;

/* Structure to describe request TLV 0x010 for QMI_NAS_SET_NR5G_SYNC_PULSE_GEN */
typedef struct {
    uint32_t start_sfn;
} QMI_NAS_SET_NR5G_SYNC_PULSE_GEN_0x10_t;

/* Structure to describe request TLV 0x011 for QMI_NAS_SET_NR5G_SYNC_PULSE_GEN */
typedef struct {
    uint32_t report_period;
} QMI_NAS_SET_NR5G_SYNC_PULSE_GEN_0x11_t;

/*****************************************************************************
 * QMI NAS indications
 */

/* Structure to describe indication TLV 0x10 for QMI_NAS_EVENT_IND */
typedef struct {
    int8_t signal_strength_dBm;
    uint8_t radio_interface;
} QMI_NAS_EVENT_IND_response_0x10_t;

/* Structure to describe indication TLV 0x12 for QMI_NAS_EVENT_IND */
typedef struct {
    uint8_t service_domain;
    uint16_t reject_cause;
} QMI_NAS_EVENT_IND_response_0x12_t;

/*****************************************************************************
 * QMI_GMS_NAS_GET_MODEM_STATUS (770 - 0x0302)
 */

/* Structure to describe response TLV 0x03 for QMI_GMS_NAS_GET_MODEM_STATUS */
typedef struct {
    int8_t temperature;
    uint8_t modem_mode;
    uint8_t system_mode;
    uint8_t ims_reg_state;
    uint8_t ps_state;
} QMI_GMS_NAS_GET_MODEM_STATUS_response_0x03_t;

/* Structure to describe response TLV 0x10 for QMI_GMS_NAS_GET_MODEM_STATUS */
typedef struct {
    uint8_t band;
    uint8_t bandwidth;
    uint16_t RXChan;
    uint16_t TXChan;
    uint8_t emmState;
    uint8_t emmSubState;
    uint8_t emmConnState;
} QMI_GMS_NAS_GET_MODEM_STATUS_response_0x10_t;

/*****************************************************************************
 * QMI_NAS_SWITCH_ANTENNA_LTE (21849 - 0x5559)
 */

/* Structure to describe request TLV 0x01 for QMI_NAS_SWITCH_ANTENNA_LTE and QMI_NAS_SWITCH_ANTENNA_WCDMA */
typedef struct {
    uint8_t antenna;
} QMI_NAS_SWITCH_ANTENNA_generic_request_0x01_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_NAS_H_ */
