#ifndef TELIT_SDK_PRIVATE_GMS_H_
#define TELIT_SDK_PRIVATE_GMS_H_

#include <math.h>

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_GMS_WDS_3GPP_CONFIG_ITEM_SET (256 - 0x0100)
 * QMI_GMS_WDS_3GPP_CONFIG_ITEM_GET (257 - 0x0101)
 */

/* Structure to describe GMS_WDS_3GPP_CONFIG_ITEM lte attach profile tlv */
typedef struct {
    uint16_t lte_attach_profile;
} QMI_GMS_WDS_3GPP_CONFIG_generic_lte_profile_t;

/* Structure to describe QMI_GMS_WDS_3GPP_CONFIG profile list tlv */
typedef struct {
    uint16_t profile_list[5];
} QMI_GMS_WDS_3GPP_CONFIG_generic_profile_list_t;

/* Structure to describe QMI_GMS_WDS_3GPP_CONFIG default pdn enabled tlv */
typedef struct {
    uint8_t default_PDN_enabled;
} QMI_GMS_WDS_3GPP_CONFIG_generic_default_pdn_enabled_t;

/* Structure to describe QMI_GMS_WDS_3GPP_CONFIG 3gpp release tlv */
typedef struct {
    uint8_t release_3gpp;
} QMI_GMS_WDS_3GPP_CONFIG_generic_3gpp_release_t;

/* Structure to describe QMI_GMS_WDS_3GPP_CONFIG lte attach profile list tlv */
typedef struct {
    uint8_t attach_LTE_profile_list_size;
    /* This array must be the size specified by attach_LTE_profile_list_size */
    /* uint16_t attach_LTE_profile_list[attach_LTE_profile_list_size]; */
} QMI_GMS_WDS_3GPP_CONFIG_generic_lte_attach_profile_list_t;

/*****************************************************************************
 * QMI_GMS_WDS_CREATE_PROFILE (259 - 0x0103)
 */

/* Structure to describe QMI_GMS_WDS_CREATE_PROFILE response TLV 0x03 */
typedef struct {
    uint8_t profile_type;
} QMI_GMS_WDS_CREATE_PROFILE_response_0x03_t;

/* Structure to describe QMI_GMS_WDS_CREATE_PROFILE response TLV 0x04 */
typedef struct {
    uint8_t profile_index;
} QMI_GMS_WDS_CREATE_PROFILE_response_0x04_t;

/* Structure to describe QMI_GMS_WDS_CREATE_PROFILE response TLV 0x10 */
typedef struct QMI_WDS_generic_resp_0x10 {
    uint16_t extended_error_code;
} QMI_GMS_WDS_CREATE_PROFILE_response_0x10_t;

/*****************************************************************************
 * QMI_GMS_DMS_GET_DYING_GASP_CFG (512 - 0x0200)
 * QMI_GMS_DMS_SET_DYING_GASP_CFG (513 - 0x0201)
 */

/* Structure to describe generic dying gasp config item TLV 0x10 */
typedef struct {
    uint8_t sms_destination_number_len;
    /* This array must be the size specified by sms_destination_number_len */
    /* char sms_destination_number[sms_destination_number_len]; */
} QMI_GMS_DMS_SET_DYING_GASP_CFG_generic_cfg_0x10_t;

/* Structure to describe generic dying gasp config item TLV 0x11 */
typedef struct {
    uint8_t sms_content_len;
    /* This array must be the size specified by sms_destination_number_len */
    /* char sms_content[sms_content_len]; */
} QMI_GMS_DMS_SET_DYING_GASP_CFG_generic_cfg_0x11_t;

/*****************************************************************************
 * QMI_GMS_DMS_GET_DYING_GASP_STAT (514 - 0x0202)
 */

/* Structure to describe QMI_GMS_DMS_GET_DYING_GASP_STAT response TLV 0x03 */
typedef struct {
    uint32_t timestamp;
} QMI_GMS_DMS_GET_DYING_GASP_STAT_response_0x03_t;

/* Structure to describe QMI_GMS_DMS_GET_DYING_GASP_STAT response TLV 0x04 */
typedef struct {
    uint8_t sms_attempted;
} QMI_GMS_DMS_GET_DYING_GASP_STAT_response_0x04_t;

/*****************************************************************************
 * QMI_GMS_DMS_GET_CUST_FEATURES (516 - 0x0204)
 */

/* Structure to describe QMI_GMS_DMS_GET_CUST_FEATURES_V2 response TLV 0x03 */
typedef struct {
    uint8_t cust_id_len;
    /* This array must be the size specified by cust_id_len */
    /* char cust_id[cust_id_len]; */
} QMI_GMS_DMS_GET_CUST_FEATURES_V2_response_0x03_t;

/* Structure to describe QMI_GMS_DMS_GET_CUST_FEATURES_V2 response TLV 0x04 */
typedef struct {
    uint8_t cust_id_len;
    /* This array must be the size specified by cust_id_len */
    /* char cust_id[cust_id_len]; */
    uint8_t cust_value_len;
    uint8_t cust_value;
    uint16_t cust_attr;
} QMI_GMS_DMS_GET_CUST_FEATURES_V2_response_0x04_t;

/*****************************************************************************
 * QMI_GMS_DMS_SET_CUST_FEATURES (517 - 0x0205)
 */

/* Structure to describe QMI_GMS_DMS_SET_CUST_FEATURES request TLV 0x02 */
typedef struct {
    uint8_t value_length;
    /* uint8_t cust_value[value_length <= 8]; */
} QMI_GMS_DMS_SET_CUST_FEATURES_V2_request_0x02_t;

/*****************************************************************************
 * QMI_GMS_DMS_GET_PC_INFO (518 - 0x0206)
 */

/* Structure to describe QMI_GMS_DMS_GET_PC_INFO response TLV 0x03 */
typedef struct {
    uint8_t operating_mode;
} QMI_GMS_DMS_GET_PC_INFO_response_0x03_t;

/* Structure to describe QMI_GMS_DMS_GET_PC_INFO response TLV 0x10 */
typedef struct {
    uint32_t lpm_flag;
} QMI_GMS_DMS_GET_PC_INFO_response_0x10_t;

/* Structure to describe QMI_GMS_DMS_GET_PC_INFO response TLV 0x11 */
typedef struct {
    uint8_t w_disable;
} QMI_GMS_DMS_GET_PC_INFO_response_0x11_t;

/* Structure to describe QMI_GMS_DMS_GET_PC_INFO response TLV 0x12 */
typedef struct {
    uint8_t power_off_mode;
} QMI_GMS_DMS_GET_PC_INFO_response_0x12_t;

/* Structure to describe QMI_GMS_DMS_GET_PC_INFO response TLV 0x13 */
typedef struct {
    uint8_t persistent_lpm;
} QMI_GMS_DMS_GET_PC_INFO_response_0x13_t;

/*****************************************************************************
 * QMI_GMS_DMS_GET_MTU (519 - 0x0207)
 */

/* Structure to describe QMI_GMS_DMS_GET_MTU request TLV 0x03 */
typedef struct {
    uint8_t mtu_size_3gpp_available;
    uint16_t mtu_size_3gpp;
    uint8_t mtu_size_hrpd_available;
    uint16_t mtu_size_hrpd;
    uint8_t mtu_size_ehrpd_available;
    uint16_t mtu_size_ehrpd;
    uint8_t mtu_size_usb_available;
    uint16_t mtu_size_usb;
} QMI_GMS_DMS_GET_MTU_response_0x03_t;

/*****************************************************************************
 * QMI_GMS_DMS_SET_MTU (520 - 0x0208)
 */

/* Structure to describe QMI_GMS_DMS_SET_MTU request TLV 0x01 */
typedef struct {
    uint16_t size;
} QMI_GMS_DMS_SET_MTU_request_0x01_t;

/*****************************************************************************
 * QMI_GMS_DMS_FWSWITCH_INFO_EX (528 - 0x0210)
 */

/* Helper struct for parsing fwswitch instances */
typedef struct {
    uint8_t mno_id;
    uint8_t mno_name_len;
    /* char mno_name[mno_name_len]; */
    /* uint8_t mno_cfg_ver_len; */
    /* char mno_cfg_ver[mno_cfg_ver_len]; */
    /* uint8_t reserved_1; */
    /* uint8_t reserved_2; */
    /* uint16_t reserved_3; */
    /* uint32_t reserved_4; */
} QMI_GMS_DMS_FWSWITCH_INFO_EX_request_info_instance_t;

/* Structure to describe QMI_GMS_DMS_FWSWITCH_INFO_EX request TLV 0x03 */
typedef struct {
    uint8_t info_list_len;
    /* This array must be the size specified by info_list_len */
    /* char cust_id[info_list_len]; */
} QMI_GMS_DMS_FWSWITCH_INFO_EX_request_0x03_t;

/*****************************************************************************
 * QMI_GMS_NAS_MODEM_STATUS (770 - 0x0302)
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
    uint16_t rx_chan;
    uint16_t tx_chan;
    uint8_t emm_state;
    uint8_t emm_sub_state;
    uint8_t emm_conn_state;
} QMI_GMS_NAS_GET_MODEM_STATUS_response_0x10_t;


/* Structure to describe response TLV 0x11 for QMI_GMS_NAS_GET_MODEM_STATUS */
typedef struct {
    uint16_t band;
    uint16_t bandwidth;
    uint32_t rx_chan;
    uint32_t tx_chan;
    int16_t rssi;
    int16_t rsrp;
    int16_t rsrq;
    int16_t snr;
    uint16_t pci;
} QMI_GMS_NAS_GET_MODEM_STATUS_response_0x11_t;

/*****************************************************************************
 * QMI_GMS_NAS_GET_LTECQI_INFO (771 - 0x0303)
 */

/* Structure to describe QMI_GMS_NAS_GET_LTECQI_INFO response TLVs */
typedef struct {
    uint8_t cqi_0;
    uint8_t cqi_1;
} QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t;

/*****************************************************************************
 * QMI_GMS_NAS_GET_LTE_SCC_RX_INFO (772 - 0x0304)
 */

/* Structure to describe QMI_GMS_NAS_GET_LTE_SCC_RX_INFO response TLVs 0x10, 0x15, 0x20, 0x25 */
typedef struct {
    int16_t sinr;
    uint8_t num_instances;
} QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_info_t;

/* Structure to describe QMI_GMS_NAS_GET_LTE_SCC_RX_INFO esponse TLVs 0x11, 0x12, 0x13,
 * 0x14, 0x16, 0x17, 0x18, 0x19, 0x21, 0x22, 0x23, 0x24, 0x26, 0x27, 0x28, 0x29 */
typedef struct {
    uint8_t rx_chain_index;
    uint8_t is_radio_tuned;
    int32_t rx_power;
    int32_t rsrp;
    int32_t rsrq;
} QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t;

/*****************************************************************************
 * QMI_GMS_NAS_IND_REGISTER (773 - 0x0305)
 */

/* Structure to describe request TLV 0x01 for QMI_GMS_NAS_IND_REGISTER */
typedef struct {
    uint8_t lte_esm_ul;
} QMI_NAS_INDICATION_REGISTER_request_0x01_t;

/* Structure to describe request TLV 0x02 for QMI_GMS_NAS_IND_REGISTER */
typedef struct {
    uint8_t lte_esm_dl;
} QMI_NAS_INDICATION_REGISTER_request_0x02_t;

/* Structure to describe request TLV 0x03 for QMI_GMS_NAS_IND_REGISTER */
typedef struct {
    uint8_t lte_emm_ul;
} QMI_NAS_INDICATION_REGISTER_request_0x03_t;

/* Structure to describe request TLV 0x04 for QMI_GMS_NAS_IND_REGISTER */
typedef struct {
    uint8_t lte_emm_dl;
} QMI_NAS_INDICATION_REGISTER_request_0x04_t;

/* Structure to describe request TLV 0x05 for QMI_GMS_NAS_IND_REGISTER */
typedef struct {
    uint8_t gsm_umts_ul;
} QMI_NAS_INDICATION_REGISTER_request_0x05_t;

/* Structure to describe request TLV 0x06 for QMI_GMS_NAS_IND_REGISTER */
typedef struct {
    uint8_t gsm_umts_dl;
} QMI_NAS_INDICATION_REGISTER_request_0x06_t;

/* Structure to describe request TLV 0x07 for QMI_GMS_NAS_IND_REGISTER */
typedef struct {
    uint8_t rank_indicator;
} QMI_NAS_INDICATION_REGISTER_request_0x07_t;

/* Structure to describe request TLV 0x08 for QMI_GMS_NAS_IND_REGISTER */
typedef struct {
    uint8_t timer;
} QMI_NAS_INDICATION_REGISTER_request_0x08_t;

/*****************************************************************************
 * QMI_GMS_NAS_GET_BAND_CAP (777 - 0x0309)
 */

/* Structure to describe response TLV 0x03 for QMI_GMS_NAS_GET_BAND_CAP */
typedef struct {
    uint32_t variant;
    uint64_t band_pref;
    uint64_t bits_1_64;
    uint64_t bits_65_128;
    uint64_t bits_129_192;
    uint64_t bits_193_256;
} QMI_GMS_NAS_GET_BAND_CAP_response_0x03_t;

/*****************************************************************************
 * QMI_GMS_NAS_GET_ENDC_CA_INFO (783 - 0x030f)
 */

/* Structure to describe QMI_GMS_NAS_GET_ENDC_CA_INFO request TLV 0x10 */
typedef struct {
    uint8_t band_class;
    uint32_t channel;
    uint8_t dl_bw;
    uint16_t pci;
    int32_t rsrp;
    int32_t rssi;
    int32_t rsrq;
    int32_t sinr;
    uint16_t tac;
} QMI_GMS_NAS_GET_ENDC_CA_INFO_response_0x10_t;

/* Structure to describe QMI_GMS_NAS_GET_ENDC_CA_INFO request TLV 0x11, 0x12 and 0x13, 0x14, 0x15 */
typedef struct {
    uint8_t band_class;
    uint32_t channel;
    uint8_t dl_bw;
    uint16_t pci;
    int32_t rsrp;
    int32_t rssi;
    int32_t rsrq;
    int32_t sinr;
    uint32_t state;
} QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t;

/* Structure to describe QMI_GMS_NAS_GET_ENDC_CA_INFO request TLV 0x16 */
typedef struct {
    int32_t endc_band;
    uint16_t endc_bandwidth;
    int32_t endc_channel;
    int32_t endc_ul_channel;
    uint16_t endc_pci;
    int32_t endc_rsrp;
    int32_t endc_rssi;
    int32_t endc_rsrq;
    int32_t endc_sinr;
} QMI_GMS_NAS_GET_ENDC_CA_INFO_response_0x16_t;

/*****************************************************************************
 * QMI_GMS_NAS_GET_NR5G_CA_INFO (790 - 0x0316)
 */

/* Structure to describe QMI_GMS_NAS_GET_NR5G_CA_INFO request TLV 0x10 */
typedef struct {
    uint16_t band_class;
    uint32_t channel;
    uint16_t dl_bw;
    uint16_t pci;
    int32_t rsrp;
    int32_t rssi;
    int32_t rsrq;
    int32_t sinr;
    uint32_t tac;
} QMI_GMS_NAS_GET_NR5G_CA_INFO_response_0x10_t;

/* Structure to describe QMI_GMS_NAS_GET_NR5G_CA_INFO request TLV 0x11, 0x12, 0x13, 0x14 and 0x15 */
typedef struct {
    uint16_t band_class;
    uint32_t channel;
    uint16_t dl_bw;
    uint16_t pci;
    int32_t rsrp;
    int32_t rssi;
    int32_t rsrq;
    int32_t sinr;
    uint32_t state;
} QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t;

/*****************************************************************************
 * QMI_GMS_LOC_GET_AUTO_START (4097 - 0x1001)
 */

/* Structure to describe QMI_GMS_LOC_GET_AUTO_START response TLV 0x03 */
typedef struct {
    uint8_t function;
} QMI_GMS_LOC_GET_AUTO_START_response_0x03_t;

/* Structure to describe QMI_GMS_LOC_GET_AUTO_START response TLV 0x04 */
typedef struct {
    uint8_t function_reported;
} QMI_GMS_LOC_GET_AUTO_START_response_0x04_t;

/* Structure to describe QMI_GMS_LOC_GET_AUTO_START response TLV 0x05 */
typedef struct {
    uint8_t fix_type;
} QMI_GMS_LOC_GET_AUTO_START_response_0x05_t;

/* Structure to describe QMI_GMS_LOC_GET_AUTO_START response TLV 0x06 */
typedef struct {
    uint8_t fix_type_reported;
} QMI_GMS_LOC_GET_AUTO_START_response_0x06_t;

/* Structure to describe QMI_GMS_LOC_GET_AUTO_START response TLV 0x07 */
typedef struct {
    uint8_t max_time;
} QMI_GMS_LOC_GET_AUTO_START_response_0x07_t;

/* Structure to describe QMI_GMS_LOC_GET_AUTO_START response TLV 0x08 */
typedef struct {
    uint8_t max_time_reported;
} QMI_GMS_LOC_GET_AUTO_START_response_0x08_t;

/* Structure to describe QMI_GMS_LOC_GET_AUTO_START response TLV 0x09 */
typedef struct {
    uint32_t max_dist;
} QMI_GMS_LOC_GET_AUTO_START_response_0x09_t;

/* Structure to describe QMI_GMS_LOC_GET_AUTO_START response TLV 0x0A */
typedef struct {
    uint8_t max_dist_reported;
} QMI_GMS_LOC_GET_AUTO_START_response_0x0A_t;

/* Structure to describe QMI_GMS_LOC_GET_AUTO_START response TLV 0x0B */
typedef struct {
    uint32_t fix_rate;
} QMI_GMS_LOC_GET_AUTO_START_response_0x0B_t;

/* Structure to describe QMI_GMS_LOC_GET_AUTO_START response TLV 0x0C */
typedef struct {
    uint8_t fix_rate_reported;
} QMI_GMS_LOC_GET_AUTO_START_response_0x0C_t;

/*****************************************************************************
 * QMI_GMS_NAS_SET_MCCBLOCK (794 - 0x031A)
 * QMI_GMS_NAS_GET_MCCBLOCK (795 - 0x031B)
 */

/* Structure to describe QMI_GMS_NAS_SET_MCCBLOCK generic TLV */
typedef struct {
    uint16_t mcc;
} QMI_GMS_NAS_MCCBLOCK_generic_mcc_t;

/*****************************************************************************
 * QMI_GMS_LOC_SET_AUTO_START (4098 - 0x1002)
 */

/* Structure to describe QMI_GMS_LOC_SET_AUTO_START request TLV 0x01 */
typedef struct {
    uint8_t function;
} QMI_GMS_LOC_SET_AUTO_START_request_0x01_t;

/* Structure to describe QMI_GMS_LOC_SET_AUTO_START request TLV 0x02 */
typedef struct {
    uint8_t set_function;
} QMI_GMS_LOC_SET_AUTO_START_request_0x02_t;

/* Structure to describe QMI_GMS_LOC_SET_AUTO_START request TLV 0x03 */
typedef struct {
    uint8_t fix_type;
} QMI_GMS_LOC_SET_AUTO_START_request_0x03_t;

/* Structure to describe QMI_GMS_LOC_SET_AUTO_START request TLV 0x04 */
typedef struct {
    uint8_t set_fix_type;
} QMI_GMS_LOC_SET_AUTO_START_request_0x04_t;

/* Structure to describe QMI_GMS_LOC_SET_AUTO_START request TLV 0x05 */
typedef struct {
    uint8_t max_time;
} QMI_GMS_LOC_SET_AUTO_START_request_0x05_t;

/* Structure to describe QMI_GMS_LOC_SET_AUTO_START request TLV 0x06 */
typedef struct {
    uint8_t set_max_time;
} QMI_GMS_LOC_SET_AUTO_START_request_0x06_t;

/* Structure to describe QMI_GMS_LOC_SET_AUTO_START request TLV 0x07 */
typedef struct {
    uint32_t max_dist;
} QMI_GMS_LOC_SET_AUTO_START_request_0x07_t;

/* Structure to describe QMI_GMS_LOC_SET_AUTO_START request TLV 0x08 */
typedef struct {
    uint8_t set_max_dist;
} QMI_GMS_LOC_SET_AUTO_START_request_0x08_t;

/* Structure to describe QMI_GMS_LOC_SET_AUTO_START request TLV 0x09 */
typedef struct {
    uint32_t fix_rate;
} QMI_GMS_LOC_SET_AUTO_START_request_0x09_t;

/* Structure to describe QMI_GMS_LOC_SET_AUTO_START request TLV 0x0A */
typedef struct {
    uint8_t set_fix_rate;
} QMI_GMS_LOC_SET_AUTO_START_request_0x0A_t;

/*****************************************************************************
 * QMI_GMS_LOC_SET_NMEA_TYPE (4099 - 0x1003)
 */

/* Structure to describe QMI_GMS_LOC_SET_NMEA_TYPE request TLV 0x01 */
typedef struct {
    uint8_t nmea_mode_ind;
} QMI_GMS_LOC_SET_NMEA_TYPE_request_0x01_t;

/* Structure to describe QMI_GMS_LOC_SET_NMEA_TYPE request TLV 0x10 */
typedef struct {
    uint32_t nmea_type;
} QMI_GMS_LOC_SET_NMEA_TYPE_request_0x10_t;

/*****************************************************************************
 * QMI_GMS_LOC_SET_GNSS_SESSION (4103 - 0x1007)
 * QMI_GMS_LOC_GET_GNSS_SESSION (4104 - 0x1008)
 */

/* Structure to describe QMI_GMS_LOC generic gnss session mode tlv */
typedef struct {
    uint8_t gnss_session_mode;
} QMI_GMS_LOC_generic_gnss_session_mode_t;

/*****************************************************************************
 * QMI_GMS_LOC_GET_ACQ_POSITION (4105 - 0x1009)
 */

/* Structure to describe QMI_GMS_LOC_GET_ACQ_POSITION response TLV 0x03 */
typedef struct {
    uint64_t utc_time;
} QMI_GMS_LOC_GET_ACQ_POSITION_response_0x03_t;

/* Structure to describe QMI_GMS_LOC_GET_ACQ_POSITION response TLV 0x06 */
typedef struct {
    float_t horizontal_dop;
} QMI_GMS_LOC_GET_ACQ_POSITION_response_0x06_t;

/* Structure to describe QMI_GMS_LOC_GET_ACQ_POSITION response TLV 0x07 */
typedef struct {
    float_t altitude_wrt_sea_level;
} QMI_GMS_LOC_GET_ACQ_POSITION_response_0x07_t;

/* Structure to describe QMI_GMS_LOC_GET_ACQ_POSITION response TLV 0x08 */
typedef struct {
    uint8_t fix_type;
} QMI_GMS_LOC_GET_ACQ_POSITION_response_0x08_t;

/* Structure to describe QMI_GMS_LOC_GET_ACQ_POSITION response TLV 0x09 */
typedef struct {
    float_t heading;
} QMI_GMS_LOC_GET_ACQ_POSITION_response_0x09_t;

/* Structure to describe QMI_GMS_LOC_GET_ACQ_POSITION response TLV 0x0A */
typedef struct {
    float_t horizontal_speed_km;
} QMI_GMS_LOC_GET_ACQ_POSITION_response_0x0A_t;

/* Structure to describe QMI_GMS_LOC_GET_ACQ_POSITION response TLV 0x0B */
typedef struct {
    float_t horizontal_speed_kn;
} QMI_GMS_LOC_GET_ACQ_POSITION_response_0x0B_t;

/* Structure to describe QMI_GMS_LOC_GET_ACQ_POSITION response TLV 0x0D */
typedef struct {
    uint8_t num_gps_sv_in_view;
} QMI_GMS_LOC_GET_ACQ_POSITION_response_0x0D_t;

/*****************************************************************************
 * QMI GMS indications
 */

/* Structure to describe QMI_GMS_NAS_OTA_MSG_REPORT indication TLV 0x01 */
typedef struct {
    uint32_t msg_type;
} QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x01_t;

/* Structure to describe QMI_GMS_NAS_OTA_MSG_REPORT indication TLV 0x02 */
typedef struct {
    uint16_t data_size;
} QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x02_t;

/* Structure to describe QMI_GMS_NAS_OTA_MSG_REPORT indication TLV 0x04 */
typedef struct {
    uint8_t release;
} QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x04_t;

/* Structure to describe QMI_GMS_NAS_OTA_MSG_REPORT indication TLV 0x05 */
typedef struct {
    uint8_t major;
} QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x05_t;

/* Structure to describe QMI_GMS_NAS_OTA_MSG_REPORT indication TLV 0x06 */
typedef struct {
    uint8_t minor;
} QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x06_t;

/* Structure to describe QMI_GMS_NAS_OTA_MSG_REPORT indication TLV 0x07 */
typedef struct {
    uint64_t time;
} QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x07_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_GMS_H_ */
