#ifndef TELIT_SDK_NAS_H_
#define TELIT_SDK_NAS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_nas.h
 * @brief NAS pack/unpack functions
 * Service ID: QMI_SVC_NAS (3)
 */

/**
 * @brief Enum to describe QMI NAS Message IDs
 */
typedef enum {
    /** 02 - 0x0002 Enable events report */
    QMI_NAS_SET_EVENT_REPORT                = 0x0002,
    /** 02 - 0x0002 Events report indication */
    QMI_NAS_EVENT_REPORT_IND                = 0x0002,
    /** 03 - 0x0003 Set registration report conditions for indications */
    QMI_NAS_INDICATION_REGISTER             = 0x0003,

    /** 32 - 0x0020 Get the signal strength */
    QMI_NAS_GET_SIGNAL_STRENGTH             = 0x0020,
    /** 33 - 0x0021 Scan for visible networks */
    QMI_NAS_PERFORM_NETWORK_SCAN            = 0x0021,

    /** 36 - 0x0024 Get info about the current serving system */
    QMI_NAS_GET_SERVING_SYSTEM              = 0x0024,
    /** 36 - 0x0024 Current serving system info indication */
    QMI_NAS_SERVING_SYSTEM_IND              = 0x0024,
    /** 37 - 0x0025 Get info about home network */
    QMI_NAS_GET_HOME_NETWORK                = 0x0025,

    /** 43 - 0x002B Get the technology preference */
    QMI_NAS_GET_TECHNOLOGY_PREFERENCE       = 0x002B,

    /** 49 - 0x0031 Get the SS radio/band channel info */
    QMI_NAS_GET_RF_BAND_INFO                = 0x0031,

    /** 51 - 0x0033 Set system selection preference */
    QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE = 0x0033,
    /** 52 - 0x0034 Get system selection preference */
    QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE = 0x0034,

    /** 67 - 0x0043 Get cell location information */
    QMI_NAS_GET_CELL_LOCATION_INFO          = 0x0043,
    /** 68 - 0x0044 Get operator name for specified network */
    QMI_NAS_GET_PLMN_NAME                   = 0x0044,

    /** 76 - 0x004C Network time indication */
    QMI_NAS_NETWORK_TIME_IND                = 0x004C,
    /** 77 - 0x004D Get system info */
    QMI_NAS_GET_SYS_INFO                    = 0x004D,
    /** 77 - 0x004D System info indication */
    QMI_NAS_SYS_INFO_IND                    = 0x004E,

    /** 79 - 0x004F Get signal info */
    QMI_NAS_GET_SIG_INFO                    = 0x004F,

    /** 81 - 0x0051 Signal info indication */
    QMI_NAS_SIG_INFO_IND                    = 0x0051,

    /** 90 - 0x005A Get detailed TX/RX information */
    QMI_NAS_GET_TX_RX_INFO                  = 0x005A,

    /** 108 - 0x006C Set the signal strength reporting thresholds */
    QMI_NAS_CONFIG_SIG_INFO2                = 0x006C,

    /** 112 - 0x0070 Limit the reporting of SYS_INFO_IND to only when some fields change */
    QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING    = 0x0070,

    /** 125 - 0x007D Retrieves the latest time change reported by the network */
    QMI_NAS_GET_NETWORK_TIME                = 0x007D,

    /** 172 - 0x00AC Get the previous carrier aggregation event information */
    QMI_NAS_GET_LTE_CPHY_CA_INFO            = 0x00AC,

    /** 288 - 0x0120 Informs the control point about the parameters related to
     * pulse generation on NR5G */
    QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND = 0x0120,

    /** 289 - 0x0121 Informs the control point that frame sync is lost on NR5G */
    QMI_NAS_NR5G_LOST_FRAME_SYNC_IND        = 0x0121,

    /** 290 - 0x0122 Requests to start or stop the sync pulse generation on NR5G */
    QMI_NAS_SET_NR5G_SYNC_PULSE_GEN         = 0x0122,

    /** 335 - 0x0149 Informs the clients about the configured TDD DL-UL pattern */
    QMI_NAS_NR5G_RRC_UTC_TIME_UPDATE_IND    = 0x0149,

    /** 336 - 0x0150 Retrieves the NR5G SIB9 UTC time from the UE */
    QMI_NAS_GET_NR5G_RRC_UTC_TIME           = 0x0150,

    /** 21849 - 0x5559 Switch LTE antenna (Foxconn) */
    QMI_NAS_SWITCH_ANTENNA_LTE              = 0x5559,
    /** 21850 - 0x5559 Switch WCDMA antenna (Foxconn) */
    QMI_NAS_SWITCH_ANTENNA_WCDMA            = 0x555A,
} QMI_NAS_message_t;

#define QMI_NAS_SET_EVENT_REPORT_MAX_THRESHOLD 6

/**
 * @brief input type for QMI_NAS_SET_EVENT_REPORT request
 *
 */
typedef struct {
    uint8_t threshold_size;  /* threshold sequence size. Maximum size: 6 */
    /** A sequence of thresholds delimiting signal strength Var bands. Each threshold specifies the signal strength (in dBm) at which an event report indication will be sent to the requesting control point. Valid values: -128 dBm to + 127 dBm */
    int8_t threshold[QMI_NAS_SET_EVENT_REPORT_MAX_THRESHOLD];
} nas_set_event_report_info_t;

/**
 * @brief Pack QMI_NAS_SET_EVENT_REPORT (2 - 0x0002) request
 *
 * This function packs a qmi request for setting the NAS state
 * reporting conditions for the requesting control point.
 *
 * The following event reportings are activated:
 *
 * Signal strength
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #nas_set_event_report_info_t
 */
QMI_SDK_error_t telit_nas_set_event_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_set_event_report_info_t *input);

/**
 * @brief Unpack QMI_NAS_SET_EVENT_REPORT (2 - 0x0002) request
 *
 * This function unpacks the result of a QMI_NAS_SET_EVENT_REPORT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_nas_set_event_report_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_NAS_INDICATION_REGISTER request
 *
 */
typedef struct {
    /** Action for serving system event */
    QMI_indication_action_t serving_system;
    /** Action for serving network time */
    QMI_indication_action_t reg_network_time;
    /** Action for system info event */
    QMI_indication_action_t sys_info;
    /** Action for signal strength event */
    QMI_indication_action_t sig_info;
    /** Action for operator name data indication */
    QMI_indication_action_t reg_operator_name_data;
    /** Action for CSP PLMN mode bit indication */
    QMI_indication_action_t reg_csp_plmn_mode_bit;
    /** Action for NR5G time sync pulse report indication */
    QMI_indication_action_t reg_nr5g_time_sync_pulse_report;
    /** Action for NR5G lost sync frame indication */
    QMI_indication_action_t reg_nr5g_lost_sync_frame;
    /** Action for NR5G UTC Time Update Indication */
    QMI_indication_action_t reg_nr5g_utc_time_update;
} nas_indication_register_t;

/**
 * @brief Pack QMI_NAS_INDICATION_REGISTER (3 - 0x0003) request
 *
 * This function packs a qmi request for setting the NAS
 * indications.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #nas_indication_register_t
 */
QMI_SDK_error_t telit_nas_indication_register_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_indication_register_t *input);

/**
 * @brief Unpack QMI_NAS_INDICATION_REGISTER (3 - 0x0003) request
 *
 * This function unpacks the result of a QMI_NAS_INDICATION_REGISTER request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_nas_indication_register_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI NAS radio interfaces
 *
 */
typedef enum {
    /** No service */
    QMI_NAS_RADIO_INTERFACE_NO_SERVICE    = 0x00,
    /** CDMA 2000 1X */
    QMI_NAS_RADIO_INTERFACE_CDMA2000_1X   = 0x01,
    /** CDMA 2000 HRPD */
    QMI_NAS_RADIO_INTERFACE_CDMA2000_HRPD = 0x02,
    /** AMPS */
    QMI_NAS_RADIO_INTERFACE_AMPS          = 0x03,
    /** GSM */
    QMI_NAS_RADIO_INTERFACE_GSM           = 0x04,
    /** UMTS */
    QMI_NAS_RADIO_INTERFACE_UMTS          = 0x05,
    /** LTE */
    QMI_NAS_RADIO_INTERFACE_LTE           = 0x08,
    /** TD-SCDMA */
    QMI_NAS_RADIO_INTERFACE_TDSCDMA       = 0x09,
    /** NR5G */
    QMI_NAS_RADIO_INTERFACE_NR5G          = 0x0C,
} QMI_NAS_radio_interface_t;

#define QMI_NAS_SIGNAL_STRENGTH_MAX_LIST_ELEMENTS 18

/**
 * @brief QMI NAS signal strength list element type
 *
 */
typedef struct {
    /** Received signal strength in dBm */
    int8_t signal_strength;
    /** Radio interface technology, see #QMI_NAS_radio_interface_t */
    enum8_t radio_if;
} nas_signal_strength_list_element_t;

/**
 * @brief QMI NAS ecio list element type
 *
 */
typedef struct {
    /** ECIO value representing negative 0.5 dB increments, i.e. 2 means -1 dB, 14 means -7 dB */
    uint8_t ecio;
    /** Radio interface technology, see #QMI_NAS_radio_interface_t */
    enum8_t radio_if;
} nas_ecio_list_element_t;

/**
 * @brief QMI NAS error rate list element type
 *
 */
typedef struct {
    /** Error rate value corresponds to the RAT that is currently registered.
     * For GSM the error rate reported is Bit Error Rate
     * For WCDMA the error rate reported is Block Error Rate
     */
    uint16_t error_rate;
    /** Radio interface technology, see #QMI_NAS_radio_interface_t */
    enum8_t radio_if;
} nas_error_rate_list_element_t;

/**
 * @brief QMI NAS rsrq info type
 *
 */
typedef struct {
    /** RSRQ value in dB (signed integer value) */
    int8_t rsrq;
    /** Radio interface technology, see #QMI_NAS_radio_interface_t */
    enum8_t radio_if;
} nas_rsrq_information_t;

/**
 * @brief QMI NAS SINR levels
 *
 */
typedef enum {
    /** -9 dB */
    QMI_NAS_SINR_LEVEL_0 = 0,
    /** -6 dB */
    QMI_NAS_SINR_LEVEL_1 = 1,
    /** -4.5 dB */
    QMI_NAS_SINR_LEVEL_2 = 2,
    /** -3 dB */
    QMI_NAS_SINR_LEVEL_3 = 3,
    /** -2 dB */
    QMI_NAS_SINR_LEVEL_4 = 4,
    /** +1 dB */
    QMI_NAS_SINR_LEVEL_5 = 5,
    /** +3 dB */
    QMI_NAS_SINR_LEVEL_6 = 6,
    /** +6 dB */
    QMI_NAS_SINR_LEVEL_7 = 7,
    /** +9 dB */
    QMI_NAS_SINR_LEVEL_8 = 8,
} QMI_NAS_SINR_level_t;

/**
 * @brief output type for QMI_NAS_GET_SIGNAL_STRENGTH
 *
 */
typedef struct {
    /** signal_strength_list size */
    uint16_t signal_strength_list_size;
    /** see #nas_signal_strength_list_element_t */
    nas_signal_strength_list_element_t signal_strength_list[QMI_NAS_SIGNAL_STRENGTH_MAX_LIST_ELEMENTS];
    /** ecio_list size */
    uint16_t ecio_list_size;
    /** see #nas_ecio_list_element_t */
    nas_ecio_list_element_t ecio_list[QMI_NAS_SIGNAL_STRENGTH_MAX_LIST_ELEMENTS];
    /** set if io is available */
    bool io_available;
    /** Received IO in dBm. IO is only applicable for 1xEV-DO */
    uint32_t io;
    /** set if sinr is available */
    bool sinr_available;
    /** SINR level. SINR is only applicable for 1xEV-DO, see #QMI_NAS_SINR_level_t */
    enum8_t sinr;
    /** error_rate_list size */
    uint16_t error_rate_list_size;
    /** see #nas_error_rate_list_element_t */
    nas_error_rate_list_element_t error_rate_list[QMI_NAS_SIGNAL_STRENGTH_MAX_LIST_ELEMENTS];
    /** set if rsrq_info is available */
    bool rsrq_info_available;
    /** see #nas_rsrq_information_t */
    nas_rsrq_information_t rsrq_info;
    /** set if lte_snr is available */
    bool lte_snr_available;
    /** LTE SNR level as a scaled integer in units of 0.1 dB */
    int16_t lte_snr;
    /** set il lte_rsrp is available */
    bool lte_rsrp_available;
    /** Current LTE RSRP in dBm */
    int16_t lte_rsrp;
} nas_signal_strength_t;

/**
 * @brief QMI NAS signal strength mask values
 *
 */
typedef enum {
    /** RSSI Information bit */
    QMI_NAS_REQUEST_SIG_INFO_RSSI_MASK       = (1 << 0),
    /** ECIO Information bit */
    QMI_NAS_REQUEST_SIG_INFO_ECIO_MASK       = (1 << 1),
    /** IO Information bit */
    QMI_NAS_REQUEST_SIG_INFO_IO_MASK         = (1 << 2),
    /** SINR Information bit */
    QMI_NAS_REQUEST_SIG_INFO_SINR_MASK       = (1 << 3),
    /** ERROR RATE Information bit */
    QMI_NAS_REQUEST_SIG_INFO_ERROR_RATE_MASK = (1 << 4),
    /** RSRQ Information bit */
    QMI_NAS_REQUEST_SIG_INFO_RSRQ_MASK       = (1 << 5),
    /** LTE SNR information bit */
    QMI_NAS_REQUEST_SIG_INFO_LTE_SNR_MASK    = (1 << 6),
    /** LTE RSRP Information bit */
    QMI_NAS_REQUEST_SIG_INFO_LTE_RSRP_MASK   = (1 << 7),
} QMI_NAS_signal_strength_mask_t;

/**
 * @brief input type for QMI_NAS_GET_SIGNAL_STRENGTH
 *
 */
typedef struct {
    /** Bitmask for requesting information, see #QMI_NAS_signal_strength_mask_t */
    uint16_t signal_strength_req_mask;
} nas_signal_strength_request_t;

/**
 * @brief Pack QMI_NAS_GET_SIGNAL_STRENGTH (32 - 0x0020) request
 *
 * This function packs a qmi request for requesting the current
 * signal strength as measured by the device
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #nas_signal_strength_request_t
 */
QMI_SDK_error_t telit_nas_get_signal_strength_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        nas_signal_strength_request_t *input);

/**
 * @brief Unpack QMI_NAS_GET_SIGNAL_STRENGTH (32 - 0x0020) request
 *
 * This function unpacks the result of a QMI_NAS_GET_SIGNAL_STRENGTH request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_signal_strength_t
 */
QMI_SDK_error_t telit_nas_get_signal_strength_unpack(uint8_t *rsp, uint16_t len, nas_signal_strength_t *out);

#define QMI_NAS_MAX_NETWORK_DESCRIPTION_LENGTH 100
#define QMI_NAS_SCAN_NETS_MAX_LIST_ELEMENTS 16

/**
 * @brief QMI NAS scan network request mask values
 *
 */
typedef enum {
    /** RSSI Information bit */
    QMI_NAS_SCAN_GSM     = (1 << 0),
    /** ECIO Information bit */
    QMI_NAS_SCAN_UMTS    = (1 << 1),
    /** IO Information bit */
    QMI_NAS_SCAN_LTE     = (1 << 2),
    /** SINR Information bit */
    QMI_NAS_SCAN_TDSCDMA = (1 << 3),
} QMI_NAS_network_scan_mask_t;

/**
 * @brief QMI NAS network type to scan
 *
 */
typedef struct {
    /** Bitmask for network type, see #QMI_NAS_network_scan_mask_t */
    uint8_t network_scan_req_mask;
} nas_network_scan_mask_t;

/**
 * @brief QMI NAS network in-use values
 *
 */
typedef enum {
    /** Unknown */
    QMI_NAS_NETWORK_IN_USE_STATUS_UNKNOWN         = 0,
    /** Current serving */
    QMI_NAS_NETWORK_IN_USE_STATUS_CURRENT_SERVING = 1,
    /** Available */
    QMI_NAS_NETWORK_IN_USE_STATUS_AVAILABLE       = 2,
} QMI_NAS_in_use_states_t;

/**
 * @brief QMI NAS roaming states
 *
 */
typedef enum {
    /** Unknown */
    QMI_NAS_NETWORK_ROAMING_STATUS_UNKNOWN = 0,
    /** Home */
    QMI_NAS_NETWORK_ROAMING_STATUS_HOME    = 1,
    /** Roaming */
    QMI_NAS_NETWORK_ROAMING_STATUS_ROAM    = 2,
} QMI_NAS_roaming_states_t;

/**
 * @brief QMI NAS forbidden states
 *
 */
typedef enum {
    /** Unknown */
    QMI_NAS_NETWORK_FORBIDDEN_STATUS_UNKNOWN       = 0,
    /** Forbidden */
    QMI_NAS_NETWORK_FORBIDDEN_STATUS_FORBIDDEN     = 1,
    /** Not forbidden */
    QMI_NAS_NETWORK_FORBIDDEN_STATUS_NOT_FORBIDDEN = 2,
} QMI_NAS_forbidden_states_t;

/**
 * @brief QMI NAS preferred states
 *
 */
typedef enum {
    /** Unknown */
    QMI_NAS_NETWORK_PREFERRED_STATUS_UNKNOWN       = 0,
    /** Preferred */
    QMI_NAS_NETWORK_PREFERRED_STATUS_PREFERRED     = 1,
    /** Not preferred */
    QMI_NAS_NETWORK_PREFERRED_STATUS_NOT_PREFERRED = 2,
} QMI_NAS_preferred_states_t;

/**
 * @brief QMI NAS network info from scan type
 *
 */
typedef struct {
    /** A 16-bit integer representation of MCC. Range: 0 to 999 */
    uint16_t mobile_country_code;
    /** A 16-bit integer representation of MNC. Range: 0 to 999 */
    uint16_t mobile_network_code;
    /** Status of the network identified by MCC and MNC preceding it bits 0-1, see #QMI_NAS_in_use_states_t */
    enum8_t in_use_status;
    /** Status of the network identified by MCC and MNC preceding it bits 2-3, see #QMI_NAS_roaming_states_t */
    enum8_t roaming_status;
    /** Status of the network identified by MCC and MNC preceding it bits 4-5, see #QMI_NAS_forbidden_states_t */
    enum8_t forbidden_status;
    /** Status of the network identified by MCC and MNC preceding it bits 6-7, see #QMI_NAS_preferred_states_t */
    enum8_t preferred_status;
    /** Length of the following optional string containing the network name or description.*/
    uint8_t description_len;
    /** An optional string containing the network name or description.*/
    char description[QMI_NAS_MAX_NETWORK_DESCRIPTION_LENGTH];
} nas_3gpp_network_info_t;

/**
 * @brief QMI NAS radio access technologies
 *
 */
typedef enum {
    /** Radio access technology GERAN */
    QMI_NAS_RAT_GERAN   = 0x00,
    /** Radio access technology UMTS */
    QMI_NAS_RAT_UMTS    = 0x01,
    /** Radio access technology LTE */
    QMI_NAS_RAT_LTE     = 0x02,
    /** Radio access technology TD-SCDMA */
    QMI_NAS_RAT_TDSCDMA = 0x03,
} QMI_NAS_radio_access_technology_t;

/**
 * @brief QMI NAS nas rat info type
 *
 */
typedef struct {
    /** Mobile Country Code */
    uint16_t MCC;
    /** Mobile Network Code */
    uint16_t MNC;
    /** Radio Access Technology, see #QMI_NAS_radio_access_technology_t */
    enum8_t RAT;
} nas_rat_info_t;

/**
 * @brief QMI NAS pcs digit type
 *
 */
typedef struct {
    /** Mobile Country Code */
    uint16_t MCC;
    /** Mobile Network Code */
    uint16_t MNC;
    /** this field is use to interpret the length of corresponding MNC reported
     * 0x01 - MNC is a three-digit value
     * 0x00 - MNC is a two-digit value
     */
    uint8_t includes_pcs_digit;
} nas_pcs_digit_t;

/**
 * @brief QMI NAS scan result values
 *
 */
typedef enum {
    /** Network scan was successful */
    QMI_NAS_SCAN_SUCCESS    = 0x00,
    /** Network scan was aborted */
    QMI_NAS_SCAN_AS_ABORT   = 0x01,
    /** Network scan did not complete due to a radio link failure recovery in progress */
    QMI_NAS_SCAN_REJ_IN_RLF = 0x02,
} QMI_NAS_scan_result_t;

/**
 * @brief output type for QMI_NAS_PERFORM_NETWORK_SCAN
 *
 */
typedef struct {
    /** network_3gpp_info_list size */
    uint16_t network_3gpp_list_size;
    /** array of #nas_3gpp_network_info_t instances */
    nas_3gpp_network_info_t network_3gpp_info_list[QMI_NAS_SCAN_NETS_MAX_LIST_ELEMENTS];
    /** rat_list size */
    uint16_t rat_list_size;
    /** array of #nas_rat_info_t instances */
    nas_rat_info_t rat_list[QMI_NAS_SCAN_NETS_MAX_LIST_ELEMENTS];
    /** pcs_digit_list size */
    uint16_t pcs_digit_list_size;
    /** array of #nas_pcs_digit_t instances */
    nas_pcs_digit_t pcs_digit_list[QMI_NAS_SCAN_NETS_MAX_LIST_ELEMENTS];
    /** true if optional scan_result is set */
    bool scan_result_available;
    /** scan result, see #QMI_NAS_scan_result_t */
    enum32_t scan_result;
} nas_network_scan_t;

/**
 * @brief Pack QMI_NAS_PERFORM_NETWORK_SCAN (33 - 0x0021) request
 *
 * This function packs a qmi request to perform a scan of visible networks.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #nas_network_scan_mask_t, if NULL the scan is performed using the currently set preference
 */
QMI_SDK_error_t telit_nas_perform_network_scan_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_network_scan_mask_t *input);

/**
 * @brief Unpack QMI_NAS_PERFORM_NETWORK_SCAN (33 - 0x0021) request
 *
 * This function unpacks the result of a QMI_NAS_PERFORM_NETWORK_SCAN request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_network_scan_t struct
 */
QMI_SDK_error_t telit_nas_perform_network_scan_unpack(uint8_t *rsp, uint16_t len,
        nas_network_scan_t *out);

#define QMI_NAS_SERVING_SYSTEM_MAX_DATA_CAPS 32
#define QMI_NAS_SERVING_SYSTEM_MAX_RADIO_IF 32
#define QMI_NAS_SERVING_SYSTEM_MAX_ROAMING_IND 32

/**
 * @brief QMI NAS registration states
 *
 */
typedef enum {
    /** Not registered and not searching */
    QMI_NAS_NOT_REGISTERED           = 0x00,
    /** Registered with a network */
    QMI_NAS_REGISTERED               = 0x01,
    /** Not registered and searching */
    QMI_NAS_NOT_REGISTERED_SEARCHING = 0x02,
    /** Registration denied */
    QMI_NAS_REGISTRATION_DENIED      = 0x03,
    /** Registration state unknown */
    QMI_NAS_REGISTRATION_UNKNOWN     = 0x04,
} QMI_NAS_registration_state_t;

/**
 * @brief QMI NAS attach states
 *
 */
typedef enum {
    /** Unknown or not applicable */
    QMI_NAS_UNKNOWN  = 0x00,
    /** Attached */
    QMI_NAS_ATTACHED = 0x01,
    /** Detached */
    QMI_NAS_DETACHED = 0x02,
} QMI_NAS_attach_state_t;

/**
 * @brief QMI NAS serving system type
 *
 */
typedef struct {
    /** Registration state of the mobile, see #QMI_NAS_registration_state_t */
    enum8_t reg_state;
    /** Circuit-switched domain attach state of the mobile, see #QMI_NAS_attach_state_t */
    enum8_t cs_attach_state;
    /** Packet-switched domain attach state of the mobile, see #QMI_NAS_attach_state_t */
    enum8_t ps_attach_state;
    /** Selected Network - Type of selected radio access network, see #QMI_network_type_t */
    enum8_t sel_network;
    /** In Use Radio Interfaces Number */
    uint8_t radio_interface_size;
    /** Radio Interface currently in use, see #QMI_NAS_radio_interface_t */
    enum8_t radio_interface[QMI_NAS_SERVING_SYSTEM_MAX_RADIO_IF];
} nas_serving_system_t;

/**
 * @brief QMI NAS current PLMN type
 *
 */
typedef struct {
    /** A 16-bit integer representation of MCC. Range: 0 to 999 */
    uint16_t mcc;
    /** A 16-bit integer representation of MNC. Range: 0 to 999 */
    uint16_t mnc;
    /** length of description string */
    uint8_t net_descr_len;
    /** An optional string containing the network name or description */
    char network_name[MAX_STRING_SIZE];
} nas_current_PLMN_t;

/**
 * @brief QMI NAS roaming indicator values
 *
 */
typedef enum {
    /** Roaming */
    QMI_NAS_ROAMING_IND_ON  = 0x00,
    /** Home */
    QMI_NAS_ROAMING_IND_OFF = 0x01,
} QMI_NAS_roaming_indicator_t;

/**
 * @brief QMI NAS Roaming Indicator Instance type
 *
 */
typedef struct {
    /** Radio Interface currently in use, see #QMI_NAS_radio_interface_t */
    enum8_t radio_interface;
    /** Roaming Indicator, see #QMI_NAS_roaming_indicator_t */
    enum8_t roam_indicator;
} nas_roaming_indicator_instance_t;

/**
 * @brief QMI NAS daylight saving indicator values
 *
 */
typedef enum {
    /** daylight saving not in effect */
    QMI_NAS_DAYLIGHT_SAVING_INDICATOR_OFF = 0x00,
    /** daylight saving in effect */
    QMI_NAS_DAYLIGHT_SAVING_INDICATOR_ON  = 0x01,
} QMI_NAS_daylight_saving_indicator_t;

/**
 * @brief QMI NAS daylight saving adjustment
 *
 */
typedef enum {
    /** No adjustment for Daylight Saving Time */
    QMI_NAS_DAYLIGHT_SAVING_NONE = 0x00,
    /** 1 hr adjustment for Daylight Saving Time */
    QMI_NAS_DAYLIGHT_SAVING_1H   = 0x01,
    /** 2 hr adjustment for Daylight Saving Time */
    QMI_NAS_DAYLIGHT_SAVING_2H   = 0x02,
} QMI_NAS_daylight_saving_adjustment_t;

/**
 * @brief QMI NAS 3GPP2 TimeZone parameters type
 *
 */
typedef struct {
    /** Number of leap seconds since the start of CDMA system time */
    uint8_t leap_seconds;
    /** Offset of system time in units of 30 minutes */
    int8_t local_time_offset;
    /** Day Light Savings Indicator, see #QMI_NAS_daylight_saving_indicator_t */
    enum8_t daylight_saving;
} nas_3gpp2_time_zone_t;

/**
 * @brief QMI NAS service states
 *
 */
typedef enum {
    /** No service */
    QMI_NAS_SERVICE_STATUS_NONE             = 0x00,
    /** Limited service */
    QMI_NAS_SERVICE_STATUS_LIMITED          = 0x01,
    /** Service available*/
    QMI_NAS_SERVICE_STATUS_AVAILABLE        = 0x02,
    /** Limited regional service */
    QMI_NAS_SERVICE_STATUS_LIMITED_REGIONAL = 0x03,
    /** MS in power save or deep sleep */
    QMI_NAS_SERVICE_STATUS_MS_POWER_SAVE    = 0x04,
} QMI_NAS_service_state_t;

/**
 * @brief QMI NAS system service capabilities
 *
 */
typedef enum {
    /** No service */
    QMI_NAS_SYSTEM_SERVICE_CAPABILITY_NO_SERVICE = 0x00,
    /** Circuit-switched only */
    QMI_NAS_SYSTEM_SERVICE_CAPABILITY_CS_ONLY    = 0x01,
    /** Packet-switched only */
    QMI_NAS_SYSTEM_SERVICE_CAPABILITY_PS_ONLY    = 0x02,
    /** Circuit-switched and packet-switched */
    QMI_NAS_SYSTEM_SERVICE_CAPABILITY_CS_AND_PS  = 0x03,
    /** MS found the right system, but not yet registered/attached */
    QMI_NAS_SYSTEM_SERVICE_CAPABILITY_CAMPED     = 0x04,
    /** Unknown */
    QMI_NAS_SYSTEM_SERVICE_CAPABILITY_UNKNOWN    = 0xFF,
} QMI_NAS_system_service_capability_t;

/**
 * @brief QMI NAS detailed service information type
 *
 */
typedef struct {
    /** Service status, see #QMI_NAS_service_state_t */
    enum8_t service_status;
    /** System service capability, see #QMI_NAS_system_service_capability_t */
    enum8_t service_caps;
    /** HDR Service status, see #QMI_NAS_service_state_t */
    enum8_t hdr_srv_status;
    /** HDR hybrid information */
    uint8_t hdr_hybrid;
    /** Forbidden system information */
    uint8_t is_sys_forbidden;
} nas_detail_svc_info_t;

/**
 * @brief QMI NAS CDMA system information extension type
 *
 */
typedef struct {
    /** Mobile Country Code */
    uint16_t MCC;
    /** IMSI_11_12 */
    uint8_t imsi_11_12;
} nas_cdma_sys_info_ext_t;

/**
 * @brief QMI NAS call barring status values
 *
 */
typedef enum {
    /** access type is unknown */
    QMI_NAS_CELL_ACCESS_UNKNOWN     = -1,
    /** Cell access is allowed for normal calls only */
    QMI_NAS_CELL_ACCESS_NORMAL_ONLY = 0x00,
    /** Cell access is allowed for emergency calls only */
    QMI_NAS_CELL_ACCESS_EMERGENCY   = 0x01,
    /** Cell access is not allowed for any call type */
    QMI_NAS_CELL_ACCESS_NO_CALLS    = 0x02,
    /** Cell access is allowed for all call types */
    QMI_NAS_CELL_ACCESS_ALL_CALLS   = 0x03,
} QMI_NAS_call_barring_status_t;

/**
 * @brief QMI NAS Call Barring Status type
 *
 */
typedef struct {
    /** Call Barring Status for circuit-switched calls, see #QMI_NAS_call_barring_status_t */
    enum32_t cs_bar_status;
    /** Call Barring Status for packet-switched calls, see #QMI_NAS_call_barring_status_t */
    enum32_t ps_bar_status;
} nas_call_bar_status_t;

/**
 * @brief QMI NAS CDMA system type
 *
 */
typedef struct {
    /** CDMA System ID */
    uint16_t cdma_system_id;
    /** CDMA Network ID */
    uint16_t cdma_network_id;
} nas_cdma_system_t;

/**
 * @brief QMI NAS base station type
 *
 */
typedef struct {
    /** CDMA Base Station ID */
    uint16_t cdma_base_station_id;
    /** CDMA Base Station Latitude */
    int32_t cdma_base_station_latitude;
    /** CDMA Base Station Longitude */
    int32_t cdma_base_station_longitude;
} nas_base_station_t;

/**
 * @brief QMI NAS HDR personality information values
 *
 */
typedef enum {
    /** HDR personality Unknown */
    QMI_NAS_HDR_PERSONALITY_UNKNOWN = 0x00,
    /** HDR personality HRPD */
    QMI_NAS_HDR_PERSONALITY_HRPD    = 0x02,
    /** HDR personality eHRPD */
    QMI_NAS_HDR_PERSONALITY_EHRPD   = 0x03,
} QMI_NAS_HDR_personality_t;

/**
 * @brief QMI NAS data capabilities values
 *
 */
typedef enum {
    /** GPRS */
    QMI_NAS_DATA_CAPABILITIES_GPRS          = 0x01,
    /** EDGE */
    QMI_NAS_DATA_CAPABILITIES_EDGE          = 0x02,
    /** HSDPA */
    QMI_NAS_DATA_CAPABILITIES_HSDPA         = 0x03,
    /** HSUPA */
    QMI_NAS_DATA_CAPABILITIES_HSUPA         = 0x04,
    /** WCDMA */
    QMI_NAS_DATA_CAPABILITIES_WCDMA         = 0x05,
    /** CD;A */
    QMI_NAS_DATA_CAPABILITIES_CDMA          = 0x06,
    /** EV-DO REV 0 */
    QMI_NAS_DATA_CAPABILITIES_EVDO_REV_0    = 0x07,
    /** EV-DO REV A */
    QMI_NAS_DATA_CAPABILITIES_EVDO_REV_A    = 0x08,
    /** GSM */
    QMI_NAS_DATA_CAPABILITIES_GSM           = 0x09,
    /** EV-DO REV B */
    QMI_NAS_DATA_CAPABILITIES_EVDO_REV_B    = 0x0A,
    /** LTE */
    QMI_NAS_DATA_CAPABILITIES_LTE           = 0x0B,
    /** HSDPA+ */
    QMI_NAS_DATA_CAPABILITIES_HSDPA_PLUS    = 0x0C,
    /** DC-HSDPA+ */
    QMI_NAS_DATA_CAPABILITIES_DC_HSDPA_PLUS = 0x0D,
} QMI_NAS_data_service_capability_t;

/**
 * @brief output type for QMI_NAS_GET_SERVING_SYSTEM request
 *
 */
typedef struct {
    /** ServingSystem, see #nas_serving_system_t */
    nas_serving_system_t serving_system;
    /** true if optional roam_indicator_value is set */
    bool roam_indicator_value_available;
    /** Optional parameter indicating Roaming Indicator value */
    uint8_t roam_indicator_value;
    /** data_caps size */
    uint8_t data_caps_size;
    /** Array of #QMI_NAS_data_service_capability_t instances */
    enum8_t data_caps[QMI_NAS_SERVING_SYSTEM_MAX_DATA_CAPS];
    /** true if optional current_PLMN is set */
    bool current_PLMN_available;
    /** Current PLMN, see nas_current_PLMN_t */
    nas_current_PLMN_t current_PLMN;
    /** true if optional cdma_system is set */
    bool cdma_system_available;
    /** Optional parameter indicating CDMA System IDs */
    nas_cdma_system_t cdma_system;
    /** true if optional base_station is set */
    bool base_station_available;
    /** Optional parameter indicating CDMA Base Station info */
    nas_base_station_t base_station;
    /** roaming_indicators size */
    uint8_t roaming_indicators_size;
    /** array of #nas_roaming_indicator_instance_t */
    nas_roaming_indicator_instance_t roaming_indicators[QMI_NAS_SERVING_SYSTEM_MAX_ROAMING_IND];
    /** true if optional default_roam_ind is set */
    bool default_roam_ind_available;
    /** Default Roaming Indicator, see #QMI_NAS_roaming_indicator_t */
    enum8_t default_roam_ind;
    /** true if optional time_zone_3gpp2 is set */
    bool time_zone_3gpp2_available;
    /** 3GPP2 Time Zone, see #nas_3gpp2_time_zone_t */
    nas_3gpp2_time_zone_t time_zone_3gpp2;
    /** true if optional CDMA_p_rev is set */
    bool CDMA_p_rev_available;
    /** CDMA protocol revision in use */
    uint8_t CDMA_p_rev;
    /** true if optional time_zone_3gpp is set */
    bool time_zone_3gpp_available;
    /** Offset from Universal time */
    int8_t time_zone_3gpp;
    /** true if optional network_DST_adjustment_3gpp is set */
    bool network_DST_adjustment_3gpp_available;
    /** 3GPP network daylight, see #QMI_NAS_daylight_saving_adjustment_t */
    enum8_t network_DST_adjustment_3gpp;
    /** true if optional location_area_code is set */
    bool location_area_code_available;
    /** A 16-bit integer representation of MCC. Range: 0 to 999 */
    uint16_t location_area_code;
    /** true if optional cell_id is set */
    bool cell_id_available;
    /** A 32-bit integer representation of cell ID */
    uint32_t cell_id;
    /** true if optional conc_svc_info is set */
    bool conc_svc_info_available;
    /** concurrent service information */
    uint8_t conc_svc_info;
    /** true if optional PRL_ind is set */
    bool PRL_ind_available;
    /** 3GPP2 PRL Indicator */
    uint8_t PRL_ind;
    /** true if optional DTM_ind is set */
    bool DTM_ind_available;
    /** Dual Transfer Mode */
    uint8_t DTM_ind;
    /** true if optional detailed_svc_info is set */
    bool detailed_svc_info_available;
    /** Detailed service information, see #nas_detail_svc_info_t */
    nas_detail_svc_info_t detailed_svc_info;
    /** true if optional CDMA_system_info_ext is set */
    bool CDMA_system_info_ext_available;
    /** CDMA System Info Ext, see #nas_cdma_sys_info_ext_t */
    nas_cdma_sys_info_ext_t CDMA_system_info_ext;
    /** true if optional hdr_personality is set */
    bool hdr_personality_available;
    /** HDR Personality Information, see #QMI_NAS_HDR_personality_t */
    enum8_t hdr_personality;
    /** true if optional track_area_code is set */
    bool track_area_code_available;
    /** Tracking area code information for LTE */
    uint16_t track_area_code;
    /** true if optional call_bar_status is set */
    bool call_bar_status_available;
    /** Call Barring Status, see #nas_call_bar_status_t */
    nas_call_bar_status_t call_bar_status;
} nas_serving_system_info_t;

/**
 * @brief Pack QMI_NAS_GET_SERVING_SYSTEM (36 - 0x0024) request
 *
 * This function packs a qmi request for querying information
 * regarding the system that currently provides service.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_nas_get_serving_system_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_NAS_GET_SERVING_SYSTEM (36 - 0x0024) request
 *
 * This function unpacks the result of a QMI_NAS_GET_SERVING_SYSTEM request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_serving_system_info_t
 */
QMI_SDK_error_t telit_nas_get_serving_system_unpack(uint8_t *rsp, uint16_t len,
        nas_serving_system_info_t *out);

/**
 * @brief QMI NAS Home System ID type
 *
 */
typedef struct {
    /** optional System ID */
    uint16_t sid;
    /** optional Network ID */
    uint16_t nid;
} nas_home_system_id_t;

/**
 * @brief output type for QMI_NAS_GET_HOME_NETWORK request
 *
 */
typedef struct {
    /** A 16-bit integer representation of MCC. Range: 0 to 999 */
    uint16_t mcc;
    /** A 16-bit integer representation of MNC. Range: 0 to 999 */
    uint16_t mnc;
    /** name string length */
    uint8_t name_len;
    /** An optional string containing the network name or description */
    char name[MAX_STRING_SIZE];
    /** true if optional System ID and Network ID are set */
    bool sid_nid_available;
    /** see #nas_home_system_id_t */
    nas_home_system_id_t home_system_id;
} nas_home_info_t;

/**
 * @brief Pack QMI_NAS_GET_HOME_NETWORK (37 - 0x0025) request
 *
 * This function packs a qmi request for retrieving information
 * about the home network of the device
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_nas_get_home_network_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_NAS_GET_HOME_NETWORK (37 - 0x0025) request
 *
 * This function unpacks the result of a QMI_NAS_GET_HOME_NETWORK request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_home_info_t
 */
QMI_SDK_error_t telit_nas_get_home_network_unpack(uint8_t *rsp, uint16_t len, nas_home_info_t *out);

/**
 * @brief QMI NAS tech preference durations
 *
 */
typedef enum {
    /** Preference is used permanently */
    QMI_NAS_TECH_PREF_DURATION_PERMANENT       = 0x00,
    /** Preference is used until the next device power cycle */
    QMI_NAS_TECH_PREF_DURATION_POWER_CYCLE     = 0x01,
    /** Preference is used until the end of the next call ora a power cycle */
    QMI_NAS_TECH_PREF_DURATION_1_CALL          = 0x02,
    /** Preference is used until the end of the next call, a specified time, or a power cycle */
    QMI_NAS_TECH_PREF_DURATION_1_CALL_OR_TIME  = 0x03,
    /** Preference is used until the end of the next call */
    QMI_NAS_TECH_PREF_DURATION_1_CALL_INTERNAL = 0x04,
} QMI_NAS_tech_pref_duration_t;

/**
 * @brief output type for QMI_NAS_GET_TECHNOLOGY_PREFERENCE request
 *
 */
typedef struct {
    /** Bitmask representing the radio technology preference set.
     * No bits set indicates to the device to automatically
     * determine the technology to use
     *
     * Values:
     * - Bit 0 - Technology is 3GPP2
     * - Bit 1 - Technology is 3GPP
     *
     * Any combination of the following may be returned:
     * - Bit 2 - Analog - AMPS if 3GPP2, GSM if 3GPP
     * - Bit 3 - Digital - CDMA if 3GPP2, WCDMA if 3GPP
     * - Bit 4 - HDR
     * - Bit 5 - LTE
     * - Bits 6 to 15 - Reserved
     * - 0xffffffff - invalid data. */
    uint16_t technology_preference;
    /** Duration of the active preference, see #QMI_NAS_tech_pref_duration_t */
    enum8_t technology_preference_duration;
    /** set if persistent is available */
    bool persistent_available;
    /** Persistent technology preference, same as
     * nas_get_technology_preference_t->technology_preference
     *
     * - 0xffffffff - invalid data.  */
    uint16_t persistent;
} nas_get_network_preference_t;

/**
 * @brief Pack QMI_NAS_GET_TECHNOLOGY_PREFERENCE (43 - 0x002B) request
 *
 * This function packs a qmi request for retrieving the technology
 * preference
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_nas_get_technology_preference_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_NAS_GET_TECHNOLOGY_PREFERENCE (43 - 0x002B) request
 *
 * This function unpacks the result of a QMI_NAS_GET_TECHNOLOGY_PREFERENCE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_get_network_preference_t
 */
QMI_SDK_error_t telit_nas_get_technology_preference_unpack(uint8_t *rsp, uint16_t len,
        nas_get_network_preference_t *out);

#define QMI_NAS_MAX_RF_INFO_INSTANCES 128

/**
 * @brief QMI NAS band classes
 *
 */
typedef enum {
    /** BC 0 */
    QMI_NAS_ACTIVE_BAND_BC_0                      = 0,
    /** BC 1 */
    QMI_NAS_ACTIVE_BAND_BC_1                      = 1,

    /** BC 3 */
    QMI_NAS_ACTIVE_BAND_BC_3                      = 3,
    /** BC 4 */
    QMI_NAS_ACTIVE_BAND_BC_4                      = 4,
    /** BC 5 */
    QMI_NAS_ACTIVE_BAND_BC_5                      = 5,
    /** BC 6 */
    QMI_NAS_ACTIVE_BAND_BC_6                      = 6,
    /** BC 7 */
    QMI_NAS_ACTIVE_BAND_BC_7                      = 7,
    /** BC 8 */
    QMI_NAS_ACTIVE_BAND_BC_8                      = 8,
    /** BC 9 */
    QMI_NAS_ACTIVE_BAND_BC_9                      = 9,
    /** BC 10 */
    QMI_NAS_ACTIVE_BAND_BC_10                     = 10,
    /** BC 11 */
    QMI_NAS_ACTIVE_BAND_BC_11                     = 11,
    /** BC 12 */
    QMI_NAS_ACTIVE_BAND_BC_12                     = 12,
    /** BC 13 */
    QMI_NAS_ACTIVE_BAND_BC_13                     = 13,
    /** BC 14 */
    QMI_NAS_ACTIVE_BAND_BC_14                     = 14,
    /** BC 15 */
    QMI_NAS_ACTIVE_BAND_BC_15                     = 15,
    /** BC 16 */
    QMI_NAS_ACTIVE_BAND_BC_16                     = 16,
    /** BC 17 */
    QMI_NAS_ACTIVE_BAND_BC_17                     = 17,
    /** BC 18 */
    QMI_NAS_ACTIVE_BAND_BC_18                     = 18,
    /** BC 19 */
    QMI_NAS_ACTIVE_BAND_BC_19                     = 19,

    /** GSM 450 */
    QMI_NAS_ACTIVE_BAND_GSM_450                   = 40,
    /** GSM 480 */
    QMI_NAS_ACTIVE_BAND_GSM_480                   = 41,
    /** GSM 750 */
    QMI_NAS_ACTIVE_BAND_GSM_750                   = 42,
    /** GSM 850 */
    QMI_NAS_ACTIVE_BAND_GSM_850                   = 43,
    /** GSM 900 Extended */
    QMI_NAS_ACTIVE_BAND_GSM_900_EXTENDED          = 44,
    /** GSM 900 Primary */
    QMI_NAS_ACTIVE_BAND_GSM_900_PRIMARY           = 45,
    /** GSM 900 Railways */
    QMI_NAS_ACTIVE_BAND_GSM_900_RAILWAYS          = 46,
    /** GSM 1800 */
    QMI_NAS_ACTIVE_BAND_GSM_1800                  = 47,
    /** GSM 1900 */
    QMI_NAS_ACTIVE_BAND_GSM_1900                  = 48,

    /** WCDMA 2100 */
    QMI_NAS_ACTIVE_BAND_WCDMA_2100                = 80,
    /** WCDMA PCS 1900 */
    QMI_NAS_ACTIVE_BAND_WCDMA_PCS_1900            = 81,
    /** WCDMA DCS 1800 */
    QMI_NAS_ACTIVE_BAND_WCDMA_DCS_1800            = 82,
    /** WCDMA 1700 US */
    QMI_NAS_ACTIVE_BAND_WCDMA_1700_US             = 83,
    /** WCDMA 850 */
    QMI_NAS_ACTIVE_BAND_WCDMA_850                 = 84,
    /** WCDMA 800 */
    QMI_NAS_ACTIVE_BAND_WCDMA_800                 = 85,
    /** WCDMA 2600 */
    QMI_NAS_ACTIVE_BAND_WCDMA_2600                = 86,
    /** WCDMA 900 */
    QMI_NAS_ACTIVE_BAND_WCDMA_900                 = 87,
    /** WCDMA 1700 Japan */
    QMI_NAS_ACTIVE_BAND_WCDMA_1700_JAPAN          = 88,

    /** WCDMA 1500 Japan */
    QMI_NAS_ACTIVE_BAND_WCDMA_1500_JAPAN          = 90,
    /** WCDMA 850 Japan */
    QMI_NAS_ACTIVE_BAND_WCDMA_850_JAPAN           = 91,

    /** E-UTRA Operating Band 1 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_1   = 120,
    /** E-UTRA Operating Band 2 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_2   = 121,
    /** E-UTRA Operating Band 3 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_3   = 122,
    /** E-UTRA Operating Band 4 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_4   = 123,
    /** E-UTRA Operating Band 5 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_5   = 124,
    /** E-UTRA Operating Band 6 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_6   = 125,
    /** E-UTRA Operating Band 7 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_7   = 126,
    /** E-UTRA Operating Band 8 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_8   = 127,
    /** E-UTRA Operating Band 9 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_9   = 128,
    /** E-UTRA Operating Band 10 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_10  = 129,
    /** E-UTRA Operating Band 11 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_11  = 130,
    /** E-UTRA Operating Band 12 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_12  = 131,
    /** E-UTRA Operating Band 13 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_13  = 132,
    /** E-UTRA Operating Band 14 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_14  = 133,
    /** E-UTRA Operating Band 17 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_17  = 134,
    /** E-UTRA Operating Band 33 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_33  = 135,
    /** E-UTRA Operating Band 34 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_34  = 136,
    /** E-UTRA Operating Band 35 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_35  = 137,
    /** E-UTRA Operating Band 36 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_36  = 138,
    /** E-UTRA Operating Band 37 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_37  = 139,
    /** E-UTRA Operating Band 38 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_38  = 140,
    /** E-UTRA Operating Band 39 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_39  = 141,
    /** E-UTRA Operating Band 40 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_40  = 142,
    /** E-UTRA Operating Band 18 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_18  = 143,
    /** E-UTRA Operating Band 19 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_19  = 144,
    /** E-UTRA Operating Band 20 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_20  = 145,
    /** E-UTRA Operating Band 21 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_21  = 146,
    /** E-UTRA Operating Band 24 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_24  = 147,
    /** E-UTRA Operating Band 25 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_25  = 148,
    /** E-UTRA Operating Band 41 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_41  = 149,
    /** E-UTRA Operating Band 42 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_42  = 150,
    /** E-UTRA Operating Band 43 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_43  = 151,
    /** E-UTRA Operating Band 23 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_23  = 152,
    /** E-UTRA Operating Band 26 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_26  = 153,
    /** E-UTRA Operating Band 32 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_32  = 154,
    /** E-UTRA Operating Band 125 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_125 = 155,
    /** E-UTRA Operating Band 126 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_126 = 156,
    /** E-UTRA Operating Band 127 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_127 = 157,
    /** E-UTRA Operating Band 28 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_28  = 158,
    /** E-UTRA Operating Band 29 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_29  = 159,
    /** E-UTRA Operating Band 30 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_30  = 160,
    /** E-UTRA Operating Band 66 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_66  = 161,
    /** E-UTRA Operating Band 250 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_250 = 162,
    /** E-UTRA Operating Band 46 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_46  = 163,
    /** E-UTRA Operating Band 27 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_27  = 164,
    /** E-UTRA Operating Band 31 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_31  = 165,
    /** E-UTRA Operating Band 71 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_71  = 166,
    /** E-UTRA Operating Band 47 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_47  = 167,
    /** E-UTRA Operating Band 48 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_48  = 168,
    /** E-UTRA Operating Band 67 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_67  = 169,
    /** E-UTRA Operating Band 68 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_68  = 170,
    /** E-UTRA Operating Band 49 */
    QMI_NAS_ACTIVE_BAND_E_UTRA_OPERATING_BAND_49  = 171,

    /** TD-SCDMA Band A */
    QMI_NAS_ACTIVE_BAND_TDSCDMA_BAND_A            = 200,
    /** TD-SCDMA Band B */
    QMI_NAS_ACTIVE_BAND_TDSCDMA_BAND_B            = 201,
    /** TD-SCDMA Band C */
    QMI_NAS_ACTIVE_BAND_TDSCDMA_BAND_C            = 202,
    /** TD-SCDMA Band D */
    QMI_NAS_ACTIVE_BAND_TDSCDMA_BAND_D            = 203,
    /** TD-SCDMA Band E */
    QMI_NAS_ACTIVE_BAND_TDSCDMA_BAND_E            = 204,
    /** TD-SCDMA Band F */
    QMI_NAS_ACTIVE_BAND_TDSCDMA_BAND_F            = 205,

    /** NR5G Band 1 */
    QMI_NAS_ACTIVE_BAND_NR5G_BAND_1               = 250,
    /** NR5G Band 2 */
    QMI_NAS_ACTIVE_BAND_NR5G_BAND_2               = 251,
} QMI_NAS_band_class_t;

/**
 * @brief QMI NAS rf info instance type
 *
 */
typedef struct {
    /** Radio interface currently in use, see #QMI_NAS_radio_interface_t */
    enum8_t radio_if;
    /** Active band class, see #QMI_NAS_band_class_t */
    enum16_t active_band;
    /** Active channel. If the channel is not relevant to the technology, 0 is returned */
    uint16_t active_channel;
} nas_rf_info_instance_t;

/**
 * @brief QMI NAS rf info extended type
 *
 */
typedef struct {
    /** Radio interface currently in use, see #QMI_NAS_radio_interface_t */
    enum8_t radio_if;
    /** Active band class, see #QMI_NAS_band_class_t */
    enum16_t active_band;
    /** Active channel. If the channel is not relevant to the technology, 0 is returned */
    uint32_t active_channel;
} nas_rf_info_extended_t;

/**
 * @brief output type for QMI_NAS_GET_RF_BAND_INFO request
 *
 */
typedef struct {
    /** rf_info_instance size */
    uint8_t rf_info_instances_size;
    /** Array of #nas_rf_info_instance_t instances */
    nas_rf_info_instance_t rf_info_instance[QMI_NAS_MAX_RF_INFO_INSTANCES];
    /** rf_info_extended size */
    uint8_t rf_info_extended_size;
    /** Array of #nas_rf_info_extended_t instances */
    nas_rf_info_extended_t rf_info_extended[QMI_NAS_MAX_RF_INFO_INSTANCES];
} nas_rf_info_t;

/**
 * @brief Pack QMI_NAS_GET_RF_BAND_INFO (49 - 0x0031) request
 *
 * This function packs a qmi request for retrieving current
 * RF band information
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_nas_get_rf_band_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_NAS_GET_RF_BAND_INFO (49 - 0x0031) request
 *
 * This function unpacks the result of a QMI_NAS_GET_RF_BAND_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_rf_info_t
 */
QMI_SDK_error_t telit_nas_get_rf_band_info_unpack(uint8_t *rsp, uint16_t len, nas_rf_info_t *out);

/**
 * @brief QMI NAS service domains
 *
 */
typedef enum {
    /** No service */
    QMI_NAS_SYS_SRV_DOMAIN_NO_SRV = 0x00,
    /** Circuit-switched */
    QMI_NAS_SYS_SRV_DOMAIN_CS     = 0x01,
    /** Packet-switched */
    QMI_NAS_SYS_SRV_DOMAIN_PS     = 0x02,
    /** Circuit-switched and packet-switched */
    QMI_NAS_SYS_SRV_DOMAIN_CS_PS  = 0x03,
    /** Camped */
    QMI_NAS_SYS_SRV_DOMAIN_CAMPED = 0x04,
} QMI_NAS_service_domain_t;

/**
 * @brief QMI NAS change duration values
 *
 */
typedef enum {
    /** Remains active until the next device power-cycle */
    QMI_NAS_POWER_CYCLE = 0x00,
    /** Remains active through power cycles until changed by the client */
    QMI_NAS_PERMANENT   = 0x01,
} QMI_NAS_change_duration_t;

/**
 * @brief QMI NAS roaming preferences
 *
 */
typedef enum {
    /** Acquire only systems for which the roaming indicator is off */
    QMI_NAS_ROAMING_PREF_OFF          = 0x01,
    /** Acquire a system as long as its roaming indicator is not off */
    QMI_NAS_ROAMING_PREF_NOT_OFF      = 0x02,
    /** Acquire only systems for which the roaming indicator is off or solid on, i.e. not flashing (CDMA only) */
    QMI_NAS_ROAMING_PREF_NOT_FLASHING = 0x03,
    /** Acquire systems, regardless of their roaming indicator */
    QMI_NAS_ROAMING_PREF_ANY          = 0xFF,
} QMI_NAS_roaming_preference_t;

/**
 * @brief QMI NAS network selection
 *
 */
typedef enum {
    /** Device registers according to its provisioning */
    QMI_NAS_NET_SEL_PREF_AUTOMATIC = 0x00,
    /** Device registers to specified network */
    QMI_NAS_NET_SEL_PREF_MANUAL    = 0x01,
} QMI_NAS_network_selection_t;


/**
 * @brief QMI NAS service domain preferences
 *
 */
typedef enum {
    /** Circuit-switched only */
    QMI_NAS_SRV_DOMAIN_PREF_CS_ONLY                  = 0x00,
    /** Packet-switched only */
    QMI_NAS_SRV_DOMAIN_PREF_PS_ONLY                  = 0x01,
    /** Circuit-switched and packet-switched */
    QMI_NAS_SRV_DOMAIN_PREF_CS_PS                    = 0x02,
    /** Packet-switched attach */
    QMI_NAS_SRV_DOMAIN_PREF_PS_ATTACH                = 0x03,
    /** Packet-switched detach */
    QMI_NAS_SRV_DOMAIN_PREF_PS_DETACH                = 0x04,
    /** Packet-switched preferred */
    QMI_NAS_SRV_DOMAIN_PREF_PS_DETACH_NO_PREF_CHANGE = 0x05,
    /** Packet-switched detach with no change in the service domain preference */
    QMI_NAS_SRV_DOMAIN_PREF_ON_DEMAND_PS_ATTACH      = 0x06,
    /** Packet-switched detach where PS service loss is done forcibly by the modem */
    QMI_NAS_SRV_DOMAIN_PREF_FORCE_PS_DETACH          = 0x07,
} QMI_NAS_service_domain_preference_t;

/**
 * @brief QMI NAS acquisition order
 *
 */
typedef enum {
    /** Automatic */
    QMI_NAS_GW_ACQ_ORDER_PREF_AUTOMATIC = 0x00,
    /** GSM then WCDMA */
    QMI_NAS_GW_ACQ_ORDER_PREF_GSM_WCDMA = 0x01,
    /** WCDMA then GSM */
    QMI_NAS_GW_ACQ_ORDER_PREF_WCDMA_GSM = 0x02,
} QMI_NAS_acquisition_order_t;

/**
 * @brief QMI NAS registration restrictions
 *
 */
typedef enum {
    /** Device follows the normal registration process */
    QMI_NAS_SRV_REG_RESTRICTION_UNRESTRICTED = 0x00,
    /** Device camps on the network according to its provisioning, but does not register */
    QMI_NAS_SRV_REG_RESTRICTION_CAMPED_ONLY  = 0x01,
    /** Device selects the network for limited service */
    QMI_NAS_SRV_REG_RESTRICTION_LIMITED      = 0x02,
} QMI_NAS_registration_restriction_t;

/**
 * @brief QMI NAS CSG ID type
 *
 */
typedef struct nas_csg_id {
    /** A 16-bit integer representation of CSG MCC */
    uint16_t mcc;
    /** A 16-bit integer representation of CSG MNC */
    uint16_t mnc;
    /** true if MNC PCS Digit Include Status */
    bool mnc_includes_pcs_digit;
    /** Closed subscriber group identifier. */
    uint32_t id;
    /** Radio interface technology of the CSG network, see #QMI_NAS_radio_interface_t */
    enum8_t rat;
} nas_csg_id_t;

#define QMI_NAS_ACQ_ORDER_SIZE 255

/**
 * @brief QMI NAS Acquisition Order Preference type
 *
 */
typedef struct nas_acq_order_pref {
    /** Number of sets of the following elements */
    uint8_t acq_order_size;
    /** Acquisition order preference to be set, see #QMI_NAS_radio_interface_t */
    enum8_t acq_order[QMI_NAS_ACQ_ORDER_SIZE];
} nas_acq_order_pref_t;

/**
 * @brief QMI NAS Network Selection Preference type
 *
 */
typedef struct {
    /** Optional parameter indicating network selection preference, see #QMI_NAS_network_selection_t */
    enum8_t net_sel_pref;
    /** A 16-bit integer representation of MCC. Range: 0 to 999 */
    uint16_t mobile_country_code;
    /** A 16-bit integer representation of MNC. Range: 0 to 999 */
    uint16_t mobile_network_code;
} nas_net_sel_pref_t;

/**
 * @brief QMI NAS LTE Band Preference Extended type
 *
 */
typedef struct {
    /** Bits 1 to 64 of the 256-bit LTE E-UTRA Operating Band bitmask */
    uint64_t bits_1_64;
    /** Bits 65 to 128 of the 256-bit LTE E-UTRA Operating Band bitmask */
    uint64_t bits_65_128;
    /** Bits 129 to 192 of the 256-bit LTE E-UTRA Operating Band bitmask */
    uint64_t bits_129_192;
    /** Bits 193 to 256 of the 256-bit LTE E-UTRA Operating Band bitmask */
    uint64_t bits_193_256;
} nas_lte_band_pref_ext_t;

/**
 * @brief QMI NAS NR5G Band Preference type
 *
 */
typedef struct {
    /** Bits 1 to 64 of the 256-bit NR5G Operating Band bitmask */
    uint64_t bits_1_64;
    /** Bits 65 to 128 of the 256-bit NR5G Operating Band bitmask */
    uint64_t bits_65_128;
    /** Bits 129 to 192 of the 256-bit NR5G Operating Band bitmask */
    uint64_t bits_129_192;
    /** Bits 193 to 256 of the 256-bit NR5G Operating Band bitmask */
    uint64_t bits_193_256;
    /** Bits 257 to 320 of the 256-bit NR5G Operating Band bitmask */
    uint64_t bits_257_320;
    /** Bits 321 to 384 of the 256-bit NR5G Operating Band bitmask */
    uint64_t bits_321_384;
    /** Bits 385 to 448 of the 256-bit NR5G Operating Band bitmask */
    uint64_t bits_385_448;
    /** Bits 449 to 512 of the 256-bit NR5G Operating Band bitmask */
    uint64_t bits_449_512;
} nas_nr5g_band_pref_t;

/**
 * @brief QMI NAS radio technology mode preference type
 *
 */
typedef enum {
    /** CDMA2000 1x */
    QMI_NAS_RAT_MODE_PREF_CDMA2000_1X   = (1 << 0),
    /** CDMA2000 HRPD(1xEV-DO) */
    QMI_NAS_RAT_MODE_PREF_CDMA2000_HRPD = (1 << 1),
    /** GSM */
    QMI_NAS_RAT_MODE_PREF_GSM           = (1 << 2),
    /** UMTS */
    QMI_NAS_RAT_MODE_PREF_UMTS          = (1 << 3),
    /** LTE */
    QMI_NAS_RAT_MODE_PREF_LTE           = (1 << 4),
    /** TD-SCDMA */
    QMI_NAS_RAT_MODE_PREF_TDSCDMA       = (1 << 5),
    /** NR5G */
    QMI_NAS_RAT_MODE_PREF_NR5G          = (1 << 6),
} QMI_NAS_RAT_mode_preference_mask_t;

/**
 * @brief Enum to describe NR5G SA/NSA MODE Disable Setting
 *
 */
typedef enum {
    /** None */
    QMI_NAS_NR5G_DISABLE_MODE_NONE = 0,
    /** Disable NR5G SA mode */
    QMI_NAS_NR5G_DISABLE_MODE_SA   = 1,
    /** Disable NR5G NSA mode */
    QMI_NAS_NR5G_DISABLE_MODE_NSA  = 2,
} QMI_NAS_NR5G_SA_NSA_mode_disable_setting_t;

/**
 * @brief Constants to describe QMI NAS E-UTRA bands
 *
 */
/** E-UTRA operating band 1 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_1     (1 << 0)
/** E-UTRA operating band 2 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_2     (1 << 1)
/** E-UTRA operating band 3 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_3     (1 << 2),
/** E-UTRA operating band 4 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_4     (1 << 3),
/** E-UTRA operating band 5 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_5     (1 << 4),
/** E-UTRA operating band 6 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_6     (1 << 5),
/** E-UTRA operating band 7 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_7     (1 << 6),
/** E-UTRA operating band 8 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_8     (1 << 7),
/** E-UTRA operating band 9 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_9     (1 << 8),
/** E-UTRA operating band 10 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_10    (1 << 9),
/** E-UTRA operating band 11 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_11    (1 << 10),
/** E-UTRA operating band 12 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_12    (1 << 11),
/** E-UTRA operating band 13 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_13    (1 << 12),
/** E-UTRA operating band 14 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_14    (1 << 13),
/* Bits 14 to 15 skipped */
/** E-UTRA operating band 17 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_17    (1 << 16),
/** E-UTRA operating band 18 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_18    (1 << 17),
/** E-UTRA operating band 19 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_19    (1 << 18),
/** E-UTRA operating band 20 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_20    (1 << 19),
/** E-UTRA operating band 21 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_21    (1 << 20),
/* Bit 22 skipped */
/** E-UTRA operating band 23 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_23    (1 << 22),
/** E-UTRA operating band 24 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_24    (1 << 23),
/** E-UTRA operating band 25 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_25    (1 << 24),
/** E-UTRA operating band 26 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_26    (1 << 25),
/* Bits 27 to 28 skipped */
/** E-UTRA operating band 28 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_28    (1 << 27),
/** E-UTRA operating band 29 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_29    (1 << 28),
/* The following two bitfields are messed... */
/** E-UTRA operating band 32 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_32    (1 << 29),
/** E-UTRA operating band 30 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_30    (1 << 31),
/* Things look again normal */
/** E-UTRA operating band 33 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_33    (1 << 32),
/** E-UTRA operating band 34 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_34    (1 << 33),
/** E-UTRA operating band 35 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_35    (1 << 34),
/** E-UTRA operating band 36 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_36    (1 << 35),
/** E-UTRA operating band 37 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_37    (1 << 36),
/** E-UTRA operating band 38 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_38    (1 << 37),
/** E-UTRA operating band 39 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_39    (1 << 38),
/** E-UTRA operating band 40 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_40    (1 << 39),
/** E-UTRA operating band 41 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_41    (1 << 40),
/** E-UTRA operating band 42 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_42    (1 << 41),
/** E-UTRA operating band 43 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_43    (1 << 42),
/* Bits 43 to 44 skipped */
/** E-UTRA operating band 46 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_46    (1 << 45),
/** E-UTRA operating band 47 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_47    (1 << 46),
/** E-UTRA operating band 48 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_48    (1 << 47),
/** E-UTRA operating band 49 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_49    (1 << 48),
/* Bits 49 to 59 skipped */
/** E-UTRA operating band 125 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_125   (1 << 125),
/** E-UTRA operating band 126 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_126   (1 << 126),
/** E-UTRA operating band 127 */
#define QMI_NAS_E_UTRA_OPERATING_BAND_127   (1 << 127),

/**
 * @brief input type for QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE
 *
 */
typedef struct {
    /** true if optional emergency_mode_on is set */
    bool emergency_mode_available;
    /** Optional parameter specifying the emergency Mode state */
    bool emergency_mode_on;
    /** true if optional mode_pref is set */
    bool mode_pref_available;
    /** - Bit Mask indicating the radio technology mode preference, see #QMI_NAS_RAT_mode_preference_mask_t */
    uint16_t mode_pref;
    /** true if optional band_pref is set */
    bool band_pref_available;
    /** - Bit mask representing the band preference, see QMI_BAND_* constants */
    uint64_t band_pref;
    /** true if optional roam_pref is set */
    bool roam_pref_available;
    /** Optional parameter indicating the roaming Preference, see #QMI_NAS_roaming_preference_t */
    enum16_t roam_pref;
    /** true if optional lte_band_pref is set */
    bool lte_band_pref_available;
    /** - Bit mask representing the LTE band preference, see QMI_NAS_E_UTRA_OPERATING_BAND_* constants */
    uint64_t lte_band_pref;
    /** true if optional net_sel_pref is set */
    bool net_sel_pref_available;
    /** Optional parameter indicating network selection preference, see #nas_net_sel_pref_t */
    nas_net_sel_pref_t network_selection_preference;
    /** true if optional chg_duration is set */
    bool chg_duration_available;
    /** Duration of the change, see #QMI_NAS_change_duration_t */
    enum8_t chg_duration;
    /** true if MNC PCS Digit Include Status */
    bool mnc_inc_pcs_dig_stat;
    /** true if optional srv_domain_pref is set */
    bool srv_domain_pref_available;
    /** Optional parameter indicating Service domain preference, see #QMI_NAS_service_domain_preference_t */
    enum32_t srv_domain_pref;
    /** true if optional gw_acq_order_pref is set */
    bool gw_acq_order_pref_available;
    /** GSM/WCDMA acquisition order preference, see #QMI_NAS_acquisition_order_t */
    enum32_t gw_acq_order_pref;
    /** true if optional acq_order_pref is set */
    bool acq_order_pref_available;
    /** Acquisition Order Preference, see #nas_acq_order_pref_t */
    nas_acq_order_pref_t acq_order_pref;
    /** true if optional srv_reg_restriction is set */
    bool srv_reg_restriction_available;
    /** Registration restriction preference, see #QMI_NAS_registration_restriction_t */
    enum32_t srv_reg_restriction;
    /** true if optional csg_id is set */
    bool csg_id_available;
    /** CSG ID, see nas_csg_id_t */
    nas_csg_id_t csg_id;
    /** true if optional rat is set */
    bool rat_available;
    /** Radio Access Technology, see #QMI_NAS_radio_interface_t */
    enum8_t rat;
    /** true if optional lte_band_preference_extended is set */
    bool lte_band_preference_extended_available;
    /** LTE band preference extended, see #nas_lte_band_pref_ext_t */
    nas_lte_band_pref_ext_t lte_band_preference_extended;
    /** true if optional nr5g_band_preference is set */
    bool nr5g_band_preference_available;
    /** NR5G band preference, the preference is set to both NR5G SA and NR5G NSA, see #nas_nr5g_band_pref_t */
    nas_nr5g_band_pref_t nr5g_band_preference;
    /**< true if optional nr5g_sa_nsa_mode_disable is set */
    bool nr5g_sa_nsa_mode_disable_available;
    /**< Disable NR5G SA/NSA mode setting. If this field is not present, it is considered as no change,
     * see #QMI_NAS_NR5G_SA_NSA_mode_disable_setting_t */
    enum32_t nr5g_sa_nsa_mode_disable;
    /** true if optional nr5g_sa_band_preference is set */
    bool nr5g_sa_band_preference_available;
    /** NR5G SA band preference, see #nas_nr5g_band_pref_t */
    nas_nr5g_band_pref_t nr5g_sa_band_preference;
    /** true if optional nr5g_nsa_band_preference is set */
    bool nr5g_nsa_band_preference_available;
    /** NR5G NSA band preference, see #nas_nr5g_band_pref_t */
    nas_nr5g_band_pref_t nr5g_nsa_band_preference;
} nas_set_system_selection_preference_t;

/**
 * @brief Pack QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE (51 - 0x0033) request
 *
 * This function packs a qmi request to set system selection preferences.
 *
 * @param[in] req_ctx request context
 * @param[out] out request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #nas_set_system_selection_preference_t
 */
QMI_SDK_error_t telit_nas_set_system_selection_preference_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_set_system_selection_preference_t *input);

/**
 * @brief Unpack QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE (51 - 0x0033) request
 *
 * This function unpacks the result of a QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_nas_set_system_selection_preference_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief output type for QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE
 *
 */
typedef struct {
    /** true if optional emergency_mode_on is set */
    bool emergency_mode_available;
    /** Optional parameter specifying the emergency Mode state */
    bool emergency_mode_on;
    /** true if optional mode_pref is set */
    bool mode_pref_available;
    /** - Bit Mask indicating the radio technology mode preference, see #QMI_NAS_RAT_mode_preference_mask_t */
    uint16_t mode_pref;
    /** true if optional band_pref is set */
    bool band_pref_available;
    /** - Bit mask representing the band preference, see QMI_BAND_* constants */
    uint64_t band_pref;
    /** true if optional roam_pref is set */
    bool roam_pref_available;
    /** Optional parameter indicating the roaming Preference, see #QMI_NAS_roaming_preference_t */
    enum16_t roam_pref;
    /** true if optional lte_band_pref is set */
    bool lte_band_pref_available;
    /** - Bit mask representing the LTE band preference, see QMI_NAS_E_UTRA_OPERATING_BAND_* constants */
    uint64_t lte_band_pref;
    /** true if optional net_sel_pref is set */
    bool net_sel_pref_available;
    /** Optional parameter indicating network selection preference, see #QMI_NAS_network_selection_t */
    enum8_t net_sel_pref;
    /** true if optional srv_domain_pref is set */
    bool srv_domain_pref_available;
    /** Optional parameter indicating Service domain preference, see #QMI_NAS_service_domain_preference_t */
    enum32_t srv_domain_pref;
    /** true if optional gw_acq_order_pref is set */
    bool gw_acq_order_pref_available;
    /** Optional parameter indicating GSM/WCDMA Acquisition, see #QMI_NAS_acquisition_order_t */
    enum32_t gw_acq_order_pref;
    /** true if optional lte_band_preference_extended is set */
    bool lte_band_preference_extended_available;
    /** LTE band preference extended, see #nas_lte_band_pref_ext_t */
    nas_lte_band_pref_ext_t lte_band_preference_extended;
    /** true if optional nr5g_band_preference is set */
    bool nr5g_band_preference_available;
    /** NR5G band preference, see #nas_nr5g_band_pref_t */
    nas_nr5g_band_pref_t nr5g_band_preference;
    /**< true if optional nr5g_sa_nsa_mode_disable is set */
    bool nr5g_sa_nsa_mode_disable_available;
    /**< Disable NR5G SA/NSA mode setting, see #QMI_NAS_NR5G_SA_NSA_mode_disable_setting_t */
    enum32_t nr5g_sa_nsa_mode_disable;
    /**< true if optional nr5g_sa_band_preference is set */
    bool nr5g_sa_band_preference_available;
    /**< NR5G SA band preference, see #nas_nr5g_band_pref_t */
    nas_nr5g_band_pref_t nr5g_sa_band_preference;
    /**< true if optional nr5g_nsa_band_preference is set */
    bool nr5g_nsa_band_preference_available;
    /**< NR5G NSA band preference, see nas_nr5g_band_pref_t */
    nas_nr5g_band_pref_t nr5g_nsa_band_preference;
} nas_system_selection_pref_t;

/**
 * @brief Pack QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE (52 - 0x0034) request
 *
 * This function packs a qmi request for querying the different
 * system selection preferences of the device
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_nas_get_system_selection_preference_pack(pack_qmi_t *req_ctx, uint8_t *req,
        uint16_t *len);

/**
 * @brief Unpack QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE (52 - 0x0034) request
 *
 * This function unpacks the result of a QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_system_selection_pref_t
 */
QMI_SDK_error_t telit_nas_get_system_selection_preference_unpack(uint8_t *rsp, uint16_t len,
        nas_system_selection_pref_t *out);

#define QMI_NAS_MAX_CELLS 9
#define QMI_NAS_PLMN_LEN 3

/**
 * @brief QMI NAS NR5G cell instance type
 *
 */
typedef struct {
    /** PLMN ID coded as octet 3, 4, and 5 in 3GPP TS 24.008 Section 10.5.1.3. */
    uint8_t plmn[QMI_NAS_PLMN_LEN];
    /** NR5G tracking area code, with tac[0] the most significant byte */
    uint8_t tac[3];
    /** NR5G global cell ID */
    uint64_t global_cell_id;
    /** Physical cell ID */
    uint16_t pci;
    /** Current RSRQ in units of 0.1 dB as measured by L1 */
    int16_t rsrq;
    /** RSRP in units of 0.1 dB as measured by L1 */
    int16_t rsrp;
    /** SNR in units of 0.1 dB as measured by L1 */
    int16_t snr;
} nas_nr5g_cell_info_t;

/**
 * @brief QMI NAS UMTS, LTE, NBR cell instance type
 *
 */
typedef struct {
    /** E-UTRA absolute RF channel number of the detected cell. */
    uint16_t earfcn;
    /** hysical cell ID of the detected cell. */
    uint16_t pci;
    /** Current received signal strength indication (float, in dBm) */
    uint32_t rsrp;
    /** Current reference signal received quality (float, in dB) */
    uint32_t rsrq;
    /** Cell selection Rx level (Srxlev) value of the detected cell in linear scale */
    int16_t srxlev;
    /** TRUE if the cell is TDD; FALSE if the cell is FDD */
    bool cell_is_tdd;
} nas_umts_lte_nbr_cell_info_t;

/**
 * @brief QMI NAS WCDMA Info - LTE Neighbor Cell Info Set type
 *
 */
typedef struct {
    /** WCDMA RRC states, see wcdma_rrc_states_t */
    enum32_t wcdma_rrc_state;
    /** Number of umts_lte_nbr_cells */
    uint8_t umts_lte_nbr_cell_size;
    /** Array of #nas_umts_lte_nbr_cell_info_t instances */
    nas_umts_lte_nbr_cell_info_t umts_lte_nbr_cells[QMI_NAS_MAX_CELLS];
} nas_wcdma_info_lte_neighbor_cell_t;

/**
 * @brief QMI NAS WCDMA cell instance type
 *
 */
typedef struct {
    /** Primary scrambling code. Range: 0 to 511. */
    uint16_t psc;
    /** Absolute power level (in 1/10 dBm) of the common
     * pilot channel as received by the UE */
    int16_t cpich_rscp;
    /** CPICH Ec/No; ratio (in 1/10 dB) of the received
     * energy per PN chip for the CPICH to the total
     * received power spectral density at the UE antenna
     * connector. */
    int16_t cpich_ecno;
    /** Cell selection Rx level (Srxlev) value. Range: -128
     * to 128. (This field is only valid when ue_in_idle is
     * TRUE.) */
    int16_t srxlev;
} nas_wcdma_cell_info_t;

/**
 * @brief QMI NAS LTE, WCDMA cell instance type
 *
 */
typedef struct {
    /** WCDMA layer frequency. Range: 0 to 16383. */
    uint16_t uarfcn;
    /** Cell reselection priority. Range: 0 to 7 */
    uint8_t cell_resel_priority;
    /** Reselection low threshold. Range: 0 to 31. */
    uint16_t thresh_Xhigh;
    /** Reselection high threshold. Range: 0 to 31 */
    uint16_t threshXlow;
    /** Number of wcdma_cells */
    uint8_t wcdma_cells_size;
    /** Array of #nas_wcdma_cell_info_t instances */
    nas_wcdma_cell_info_t wcdma_cells[QMI_NAS_MAX_CELLS];
} nas_lte_wcdma_cell_info_t;

/**
 * @brief QMI NAS WCDMA Info - LTE Neighbor Cell Info Set type
 *
 */
typedef struct {
    /** TRUE if the UE is in Idle mode; otherwise FALSE. */
    bool ue_in_idle;
    /** Number of lte_wcdma_cells */
    uint8_t lte_wcdma_cells_size;
    /** Array of #nas_lte_wcdma_cell_info_t instances */
    nas_lte_wcdma_cell_info_t lte_wcdma_cells[QMI_NAS_MAX_CELLS];
} nas_lte_info_neighboring_wcdma_t;

/**
 * @brief QMI NAS GSM cell instance type
 *
 */
typedef struct {
    /** GSM frequency being reported. Range: 0 to 1023. */
    uint16_t arfcn;
    /** Band indicator for the GSM ARFCN */
    bool band_1900;
    /** Flag indicating whether the base station identity
     * code ID is valid. */
    bool cell_id_valid;
    /** Base station identity code ID, including base station
     * color code and network color code. */
    uint8_t bsic_id;
    /** Measured RSSI value in 1/10 dB. */
    int16_t rssi;
    /** Cell selection Rx level (Srxlev) value. */
    int16_t srxlev;
} nas_gsm_cell_info_t;

/**
 * @brief QMI NAS LTE, GSM cell instance type
 *
 */
typedef struct {
    /** Priority of this frequency group. Range: 0 to 7. */
    uint8_t cell_resel_priority;
    /** Reselection threshold for high priority layers */
    uint8_t thresh_gsm_high;
    /** Reselection threshold for low priority layers */
    uint8_t thresh_gsm_low;
    /** Bitmask specifying whether a neighbor with a
     * specific network color code is to be reported. */
    uint8_t ncc_permitted;
    /** Number of gsm_cells */
    uint8_t gsm_cells_size;
    /** Array of #nas_gsm_cell_info_t instances */
    nas_gsm_cell_info_t gsm_cells[QMI_NAS_MAX_CELLS];
} nas_lte_gsm_cell_info_t;

/**
 * @brief QMI NAS LTE neighboring GSM cell type
 *
 */
typedef struct {
    /** TRUE if the UE is in Idle mode; otherwise FALSE. */
    bool ue_in_idle;
    /** Number of lte_gsm_cells */
    uint8_t lte_gsm_cells_size;
    /** Array of #nas_lte_gsm_cell_info_t instances */
    nas_lte_gsm_cell_info_t lte_gsm_cells[QMI_NAS_MAX_CELLS];
} nas_lte_info_neighboring_gsm_t;

/**
 * @brief QMI NAS cell parameters type
 *
 */
typedef struct {
    /** Physical cell ID. Range: 0 to 503. */
    uint16_t pci;
    /** Current RSRQ in 1/10 dB as measured by L1. */
    int16_t rsrq;
    /** Current RSRP in 1/10 dBm as measured by L1 */
    int16_t rsrp;
    /** Current RSSI in 1/10 dBm as measured by L1 */
    int16_t rssi;
    /** Cell selection Rx level (Srxlev) value. */
    int16_t srxlev;
} nas_cell_params_t;

/**
 * @brief QMI NAS inter-frequencies information type
 *
 */
typedef struct {
    /** E-UTRA absolute radio frequency channel number. */
    uint16_t earfcn;
    /** Cell Srxlev low threshold. Range: 0 to 31 */
    uint8_t threshX_low;
    /** Cell Srxlev high threshold. Range: 0 to 31 */
    uint8_t threshX_high;
    /** Cell reselection priority. Range: 0 to 7 */
    uint8_t cell_resel_priority;
    /** Number of cell_inter_freq_params */
    uint8_t cell_inter_freq_params_size;
    /** Array of #nas_cell_params_t instances */
    nas_cell_params_t cell_inter_freq_params[QMI_NAS_MAX_CELLS];
} nas_inter_freqs_info_t;

/**
 * @brief QMI NAS LTE inter-frequencies information type
 *
 */
typedef struct {
    /** TRUE if the UE is in Idle mode; otherwise FALSE. */
    bool ue_in_idle;
    /** Number of inter_freqs */
    uint8_t inter_freqs_size;
    /** Array of #nas_inter_freqs_info_t instances */
    nas_inter_freqs_info_t inter_freqs[QMI_NAS_MAX_CELLS];
} nas_lte_info_interfreq_t;

/**
 * @brief QMI NAS LTE intra-frequencies information type
 *
 */
typedef struct {
    /** TRUE if the UE is in Idle mode; otherwise FALSE. */
    bool ue_in_idle;
    /** PLMN ID coded as octet 3, 4, and 5 in 3GPP TS 24.008 Section 10.5.1.3. */
    uint8_t plmn[QMI_NAS_PLMN_LEN];
    /** Tracking area code. */
    uint16_t tac;
    /** Global cell ID in the system information block. */
    uint32_t global_cell_id;
    /** E-UTRA absolute radio frequency channel number
     * of the serving cell. Range: 0 to 65535. */
    uint16_t earfcn;
    /** LTE serving cell ID. Range: 0 to 503 */
    uint16_t serving_cell_id;
    /** Priority for serving frequency. Range: 0 to 7 */
    uint8_t cell_resel_priority;
    /** S non-intra search threshold to control
     * non-intrafrequency searches */
    uint8_t s_non_intra_search;
    /** Serving cell low threshold. Range: 0 to 31 */
    uint8_t thresh_serving_low;
    /** S intra search threshold. Range: 0 to 31 */
    uint8_t s_intra_search;
    /** Number of cell_intra_freq_params */
    uint8_t cell_intra_freq_params_size;
    /** Array of #nas_cell_params_t instances */
    nas_cell_params_t cell_intra_freq_params[QMI_NAS_MAX_CELLS];
} nas_lte_info_intrafreq_t;

/**
 * @brief QMI NAS UMTS cell parameters type
 *
 */
typedef struct {
    /** UTRA absolute RF channel number. */
    uint16_t umts_uarfcn;
    /** Primary scrambling code. */
    uint16_t umts_psc;
    /** Received signal code power */
    int16_t umts_rscp;
    /** ECIO; the received energy per chip divided by the
     * power density in the band measured in dBm on the
     * primary CPICH channel of the neighbor/monitored
     * cell. */
    int16_t umts_ecio;
} nas_umts_inst_info_t;

/**
 * @brief QMI NAS GERAN cell parameters type
 *
 */
typedef struct {
    /** Absolute RF channel number. */
    uint16_t geran_arfcn;
    /** Base station identity code network color code */
    uint8_t geran_bsic_ncc;
    /** Base station identity code base station color code */
    uint8_t geran_bsic_bcc;
    /** Received signal strength indicator */
    int16_t geran_rssi;
} nas_geran_inst_info_t;

/**
 * @brief QMI NAS set of UMTS cells type
 *
 */
typedef struct {
    /** Cell ID */
    uint16_t cell_id;
    /** MCC/MNC information coded as octet 3, 4, and 5 in
     * 3GPP TS 24.008 Section 10.5.1.3. */
    uint8_t plmn[QMI_NAS_PLMN_LEN];
    /** Location area code. */
    uint16_t lac;
    /** UTRA absolute RF channel number. */
    uint16_t uarfcn;
    /** Primary scrambling code. */
    uint16_t psc;
    /** Received signal code power */
    int16_t rscp;
    /** ECIO; the received energy per chip divided by the
     * power density in the band measured in dBm on the
     * primary CPICH channel of the serving cell. */
    int16_t ecio;
    /** Number of umts_insts */
    uint8_t umts_insts_size;
    /** Array of #nas_umts_inst_info_t instances */
    nas_umts_inst_info_t umts_insts[QMI_NAS_MAX_CELLS];
    /** Number of geran_insts */
    uint8_t geran_insts_size;
    /** Array of #nas_geran_inst_info_t instances */
    nas_geran_inst_info_t geran_insts[QMI_NAS_MAX_CELLS];
} nas_umts_info_t;

/**
 * @brief QMI NAS NMR cell parameters type
 *
 */
typedef struct {
    /** Cell ID */
    uint32_t nmr_cell_id;
    /** MCC/MNC information coded as octet 3, 4, and 5 in
     * 3GPP TS 24.008 Section 10.5.1.3. */
    uint8_t nmr_plmn[QMI_NAS_PLMN_LEN];
    /** Location area code. */
    uint16_t nmr_lac;
    /** Absolute RF channel number. */
    uint16_t nmr_arfcn;
    /** Base station identity code. */
    uint8_t nmr_bsic;
    /** Cell Rx measurement. */
    uint16_t nmr_rx_lev;
} nas_nmr_cell_info_t;

/**
 * @brief QMI NAS set of GERAN cells type
 *
 */
typedef struct {
    /** Cell ID */
    uint32_t cell_id;
    /** MCC/MNC information coded as octet 3, 4, and 5 in
     * 3GPP TS 24.008 Section 10.5.1.3. */
    uint8_t plmn[QMI_NAS_PLMN_LEN];
    /** Location area code. */
    uint16_t lac;
    /** Absolute RF channel number. */
    uint16_t arfcn;
    /** Base station identity code. */
    uint8_t bsic;
    /** Measured delay (in bit periods; 1 bit period = 48/13
     * microsecond) of an access burst transmission on telit_helper_get_resp_ctx(
     * RACH or PRACH to the expected signal from an
     * MS at zero distance under static channel conditions. */
    uint32_t timing_advance;
    /** Serving cell Rx measurement. */
    uint16_t rx_lev;
    /** Number of nmr_cell_info */
    uint8_t nmr_cell_info_size;
    /** Array of #nas_nmr_cell_info_t instances */
    nas_nmr_cell_info_t nmr_cell_info[QMI_NAS_MAX_CELLS];
} nas_geran_info_t;

/**
 * @brief output type for QMI_NAS_GET_CELL_LOCATION_INFO
 *
 */
typedef struct {
    /** true if optional geran_info is set */
    bool geran_info_available;
    /** GERAN Info, see #nas_geran_info_t */
    nas_geran_info_t geran_info;
    /** true if optional umts_info is set */
    bool umts_info_available;
    /** UMTS Info, see #nas_umts_info_t */
    nas_umts_info_t umts_info;
    /** true if optional lte_info_intrafreq is set */
    bool lte_info_intrafreq_available;
    /** LTE Info - Intrafrequency, see #nas_lte_info_intrafreq_t */
    nas_lte_info_intrafreq_t lte_info_intrafreq;
    /** true if optional lte_info_interfreq is set */
    bool lte_info_interfreq_available;
    /** LTE Info - Interfrequency, see #nas_lte_info_interfreq_t */
    nas_lte_info_interfreq_t lte_info_interfreq;
    /** true if optional lte_info_neighboring_gsm is set */
    bool lte_info_neighboring_gsm_available;
    /** LTE Info - Neighboring GSM, see #nas_lte_info_neighboring_gsm_t */
    nas_lte_info_neighboring_gsm_t lte_info_neighboring_gsm;
    /** true if optional lte_info_neighboring_wcdma is set */
    bool lte_info_neighboring_wcdma_available;
    /** LTE Info - Neighboring WCDMA, see #nas_lte_info_neighboring_wcdma_t */
    nas_lte_info_neighboring_wcdma_t lte_info_neighboring_wcdma;
    /** true if optional umts_cell_id is set */
    bool umts_cell_id_available;
    /** UMTS Cell ID */
    uint32_t umts_cell_id;
    /** true if optional wcdma_info_lte_neighbor_cell is set */
    bool wcdma_info_lte_neighbor_cell_available;
    /** WCDMA Info - LTE Neighbor Cell Info Set, see #nas_wcdma_info_lte_neighbor_cell_t */
    nas_wcdma_info_lte_neighbor_cell_t wcdma_info_lte_neighbor_cell;
    /** true if optional nr5g_arfcn is set */
    bool nr5g_arfcn_available;
    /** NR5G ARFCN of the serving cell */
    uint32_t nr5g_arfcn;
    /** true if optional nr5g_serving_cell is set */
    bool nr5g_serving_cell_available;
    /** NR5G serving cell information */
    nas_nr5g_cell_info_t nr5g_serving_cell;
} nas_cell_location_info_t;

/**
 * @brief Pack QMI_NAS_GET_CELL_LOCATION_INFO (67 - 0x0043) request
 *
 * This function packs a qmi request for querying the
 * cell location-related information.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_nas_get_cell_location_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_NAS_GET_CELL_LOCATION_INFO (67 - 0x0043) request
 *
 * This function unpacks the result of a QMI_NAS_GET_CELL_LOCATION_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_cell_location_info_t
 */
QMI_SDK_error_t telit_nas_get_cell_location_info_unpack(uint8_t *rsp, uint16_t len,
        nas_cell_location_info_t *out);

/**
 * @brief QMI NAS PLMN name encoding schemes
 *
 */
typedef enum {
    /** SMS default 7-bit coded alphabet as defined in 3GPP TS 23.038 with bit 8 set to 0 */
    QMI_NAS_CODING_SCHEME_CELL_BROADCAST_GSM = 0x00,
    /** UCS2 (16 bit, little endian) 3GPP TS 23.038 */
    QMI_NAS_CODING_SCHEME_UCS2               = 0x01,
} QMI_NAS_PLMN_name_encoding_scheme_t;

/**
 * @brief QMI NAS PLMN name country initials
 *
 */
typedef enum {
    /** Letters for the country's initials not added */
    QMI_NAS_PLMN_NAME_COUNTRY_INITIALS_NONE    = 0x00,
    /** Letters for the country's initials added */
    QMI_NAS_PLMN_NAME_COUNTRY_INITIALS_ADD     = 0x01,
    /** Letters for the country's initials not specified */
    QMI_NAS_PLMN_NAME_COUNTRY_INITIALS_UNKNOWN = 0xFF,
} QMI_NAS_PLMN_name_country_initials_t;

/**
 * @brief QMI NAS PLMN name spare bits
 *
 */
typedef enum {
    /** No information about the number of spare bits in octet n */
    QMI_NAS_PLMN_NAME_SPARE_BITS_UNKNOWN      = 0x00,
    /** Bit 8 is spare and set to 0 in octet n */
    QMI_NAS_PLMN_NAME_SPARE_BITS_BIT8         = 0x01,
    /** Bit 7 to 8 are spare and set to 0 in octet n */
    QMI_NAS_PLMN_NAME_SPARE_BITS_BIT7_TO_BIT8 = 0x02,
    /** Bit 6 to 8 are spare and set to 0 in octet n */
    QMI_NAS_PLMN_NAME_SPARE_BITS_BIT6_TO_BIT8 = 0x03,
    /** Bit 5 to 8 are spare and set to 0 in octet n */
    QMI_NAS_PLMN_NAME_SPARE_BITS_BIT5_TO_BIT8 = 0x04,
    /** Bit 4 to 8 are spare and set to 0 in octet n */
    QMI_NAS_PLMN_NAME_SPARE_BITS_BIT4_TO_BIT8 = 0x05,
    /** Bit 3 to 8 are spare and set to 0 in octet n */
    QMI_NAS_PLMN_NAME_SPARE_BITS_BIT3_TO_BIT8 = 0x06,
    /** Bit 2 to 8 are spare and set to 0 in octet n */
    QMI_NAS_PLMN_NAME_SPARE_BITS_BIT2_TO_BIT8 = 0x07,
} QMI_NAS_PLMN_name_spare_bits_t;

/**
 * @brief input type for QMI_NAS_GET_PLMN_NAME
 *
 */
typedef struct {
    /** A 16-bit integer representation of MCC. Range: 0 to 999 */
    uint16_t mcc;
    /** A 16-bit integer representation of MNC. Range: 0 to 999 */
    uint16_t mnc;
    /** set if mnc_includes_pcs_digit is available */
    bool mnc_includes_pcs_digit_available;
    /** This field is used to interpret the length of the
     * corresponding MNC reported in the PLMN TLV (0x01).
     *
     * Values:
     *
     * TRUE – MNC is a three-digit value; e.g., a reported
     *        value of 90 corresponds to an MNC value of 090
     *
     * FALSE – MNC is a two-digit value; e.g., a reported
     *         value of 90 corresponds to an MNC value of 90
     *
     * If this TLV is not present, an MNC smaller than 100
     * is assumed to be a two-digit value, and an MNC
     * greater than or equal to 100 is assumed to be a
     * three-digit value. */
    bool mnc_includes_pcs_digit;
} nas_plmn_info_t;

/**
 * @brief QMI NAS PLMN name type
 *
 */
typedef struct {
    /** Coding scheme for the service provider name, see #QMI_NAS_PLMN_name_encoding_scheme_t */
    enum8_t spn_encoding;
    /** Service provider name length */
    uint8_t spn_name_len;
    /** Service provider name */
    char spn_name[MAX_STRING_SIZE];
} nas_plmn_name_t;

/**
 * @brief QMI NAS PLMN short name info type
 *
 */
typedef struct {
    /** Coding scheme for the service provider name, see #QMI_NAS_PLMN_name_encoding_scheme_t */
    enum8_t short_name_encoding;
    /** Indicates wether the country initials are to be added in the plmn short name, see #QMI_NAS_PLMN_name_country_initials_t */
    enum8_t short_name_country_initials;
    /** Spare bits in the plmn short name, see #QMI_NAS_PLMN_name_spare_bits_t */
    enum8_t short_name_spare_bits;
    /** Service provider short name length */
    uint8_t short_name_len;
    /** Service provider short name */
    char short_name[MAX_STRING_SIZE];
} nas_plmn_name_short_t;

/**
 * @brief QMI NAS PLMN long name info type
 *
 */
typedef struct {
    /** Coding scheme for the service provider name, see #QMI_NAS_PLMN_name_encoding_scheme_t */
    enum8_t long_name_encoding;
    /** Indicates wether the country initials are to be added in the plmn long name, see #QMI_NAS_PLMN_name_country_initials_t */
    enum8_t long_name_country_initials;
    /** Spare bits in the plmn long name, see #QMI_NAS_PLMN_name_spare_bits_t */
    enum8_t long_name_long_bits;
    /** Service provider long name length */
    uint8_t long_name_len;
    /** Service provider long name */
    char long_name[MAX_STRING_SIZE];
} nas_plmn_name_long_t;

/**
 * @brief QMI NAS 3GPP EONS type
 *
 */
typedef struct {
    /** #nas_plmn_name_t instance */
    nas_plmn_name_t name;
    /** #nas_plmn_name_short_t instance */
    nas_plmn_name_short_t name_short;
    /** #nas_plmn_name_long_t instance */
    nas_plmn_name_long_t name_long;
} nas_3gpp_eons_plmn_t;

/**
 * @brief output type for QMI_NAS_GET_PLMN_NAME
 *
 */
typedef struct {
    /** true if optional eons_plmn_name is set */
    bool eons_plmn_name_available;
    /** 3GPP EONS PLMN Name, see #nas_3gpp_eons_plmn_t */
    nas_3gpp_eons_plmn_t eons_plmn_name;
} nas_plmn_names_t;

/**
 * @brief Pack QMI_NAS_GET_PLMN_NAME (68 - 0x0044) request
 *
 * This function packs a qmi request for querying the operator
 * names for a specific network
 *
 * @param[in] req_ctx request context
 * @param[in,out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #nas_plmn_info_t
 */
QMI_SDK_error_t telit_nas_get_plmn_name_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_plmn_info_t *input);

/**
 * @brief Unpack QMI_NAS_GET_PLMN_NAME (68 - 0x0044) request
 *
 * This function unpacks the result of a QMI_NAS_GET_PLMN_NAME request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_plmn_names_t
 */
QMI_SDK_error_t telit_nas_get_plmn_name_unpack(uint8_t *rsp, uint16_t len, nas_plmn_names_t *out);

/**
 * @brief QMI NAS preferred data path values
 *
 */
typedef enum {
    /** Not preferred */
    QMI_NAS_DATA_PATH_NOT_PREFERRED = 0,
    /** Preferred */
    QMI_NAS_DATA_PATH_PREFERRED     = 1,
} QMI_NAS_preferred_data_path_t;

/**
 * @brief QMI NAS HDR service status type
 *
 */
typedef struct {
    /** Service status of the system, see #QMI_NAS_service_state_t */
    enum8_t service_status;
    /** Whether the RAT is the preferred data path, see #QMI_NAS_preferred_data_path_t */
    enum8_t preferred_data_path;
} nas_hdr_service_status_info_t;

/**
 * @brief QMI NAS GSM Service Status Info type
 *
 */
typedef struct {
    /** Service status of the system, see #QMI_NAS_service_state_t */
    enum8_t service_status;
    /** True service status of the system, see #QMI_NAS_service_state_t  */
    enum8_t true_service_status;
    /** Whether the RAT is the preferred data path, see #QMI_NAS_preferred_data_path_t */
    enum8_t preferred_data_path;
} nas_service_status_info_t;

/**
 * @brief QMI NAS system forbidden status
 *
 */
typedef enum {
    /** Not forbidden */
    QMI_NAS_SYSTEM_NOT_FORBIDDEN = 0x00,
    /** Forbidden */
    QMI_NAS_SYSTEM_FORBIDDEN     = 0x01,
} QMI_NAS_system_forbidden_status_t;

/**
 * @brief QMI NAS HDR active protocol
 *
 */
typedef enum {
    /** None */
    QMI_NAS_HDR_PROT_NONE     = 0x00,
    /** HDR Rel 0 */
    QMI_NAS_HDR_PROT_HDR_REL0 = 0x02,
    /** HDR Rel A */
    QMI_NAS_HDR_PROT_RELA     = 0x03,
    /** HDR Rel B */
    QMI_NAS_HDR_PROT_RELB     = 0x04,
} QMI_NAS_HDR_active_protocol_t;

#define SYS_ID_SIZE 16

/**
 * @brief QMI NAS roaming status
 *
 */
typedef enum {
    /** Off */
    QMI_NAS_SYS_ROAM_STATUS_OFF                 = 0x00,
    /** On */
    QMI_NAS_SYS_ROAM_STATUS_ON                  = 0x01,
    /** Blinking */
    QMI_NAS_SYS_ROAM_STATUS_BLINK               = 0x02,
    /** Out of the neighborhood */
    QMI_NAS_SYS_ROAM_STATUS_OUT_OF_NEIGHBORHOOD = 0x03,
    /** Out of the building */
    QMI_NAS_SYS_ROAM_STATUS_OUT_OF_BLDG         = 0x04,
    /** Preferred system */
    QMI_NAS_SYS_ROAM_STATUS_PREF_SYS            = 0x05,
    /** Available system */
    QMI_NAS_SYS_ROAM_STATUS_AVAIL_SYS           = 0x06,
    /** Alliance partner */
    QMI_NAS_SYS_ROAM_STATUS_ALLIANCE_PARTNER    = 0x07,
    /** Premium partner */
    QMI_NAS_SYS_ROAM_STATUS_PREMIUM_PARTNER     = 0x08,
    /** Full service */
    QMI_NAS_SYS_ROAM_STATUS_FULL_SVC            = 0x09,
    /** Partial service */
    QMI_NAS_SYS_ROAM_STATUS_PARTIAL_SVC         = 0x0A,
    /** Banner is on */
    QMI_NAS_SYS_ROAM_STATUS_BANNER_ON           = 0x0B,
    /** Banner is off */
    QMI_NAS_SYS_ROAM_STATUS_BANNER_OFF          = 0x0C,
} QMI_NAS_roam_status_t;

/**
 * @brief QMI NAS HDR System Info type
 *
 */
typedef struct {
    /** Indicates whether the service domain is valid. */
    bool srv_domain_available;
    /** Service domain registered on the system, see #QMI_NAS_service_domain_t */
    enum8_t srv_domain;
    /** Indicates whether the service capability is valid. */
    bool srv_capability_available;
    /** Current system’s service capability, see #QMI_NAS_service_domain_t */
    enum8_t srv_capability;
    /** Indicates whether the roaming status is valid. */
    bool roam_status_available;
    /** Current roaming status, see #QMI_NAS_roam_status_t */
    enum8_t roam_status;
    /** Indicates whether the forbidden system is valid. */
    bool is_sys_forbidden_available;
    /** Whether the system is forbidden, see #QMI_NAS_system_forbidden_status_t */
    enum8_t is_sys_forbidden;
    /** Indicates whether the system PRL match is valid */
    bool is_sys_prl_match_available;
    /** Indicates whether the system is in a PRL */
    bool is_sys_prl_match;
    /** Indicates whether the HDR personality is valid */
    bool hdr_personality_available;
    /** HDR personality information, see #QMI_NAS_HDR_personality_t */
    enum8_t hdr_personality;
    /** Indicates whether the HDR active protocol revision information is valid */
    bool hdr_active_prot_available;
    /** HDR active protocol revision information, see #QMI_NAS_HDR_active_protocol_t */
    enum8_t hdr_active_prot;
    /** Indicates whether the IS-856 system ID is valid */
    bool is856_sys_id_available;
    /** IS-856 system ID */
    uint8_t is856_sys_id[SYS_ID_SIZE];
} nas_HDR_sys_info_t;

/**
 * @brief QMI NAS GSM system info type
 *
 */
typedef struct {
    /** Indicates whether the service domain is valid */
    bool srv_domain_available;
    /** Service domain registered on the system, see #QMI_NAS_system_service_capability_t */
    enum8_t srv_domain;
    /** Indicates whether theservice capability is valid */
    bool srv_capability_available;
    /** Current system’s service capability, see #QMI_NAS_system_service_capability_t */
    enum8_t srv_capability;
    /** Indicates whether the roaming status is valid */
    bool roam_status_available;
    /** GSM current roaming status, see #QMI_NAS_roam_status_t */
    enum8_t roam_status;
    /** Indicates whether the forbidden system is valid */
    bool is_sys_forbidden_available;
    /** Indicates Whether the system is forbidden, see #QMI_NAS_system_forbidden_status_t */
    enum8_t is_sys_forbidden;
    /** Indicates whether the location area code is valid */
    bool lac_available;
    /** Location area code */
    uint16_t lac;
    /** Indicates whether the cell ID is valid */
    bool cell_id_available;
    /** cell ID */
    uint32_t cell_id;
    /** Indicates whether the registration reject information is valid */
    bool reg_reject_info_available;
    /** Type of service domain in which the registration is rejected, see #QMI_NAS_system_service_capability_t */
    enum8_t reject_srv_domain;
    /** Reject cause */
    uint8_t rej_cause;
    /** Indicates whether the network ID is valid */
    bool network_id_available;
    /** MCC digits in ASCII characters */
    char mcc[3 + 1];
    /** MNC digits in ASCII characters. Unused byte is set to 0xFF. In the case of two-digit MNC values,
     * the third (unused) digit is set to 0xFF */
    char mnc[3 + 1];
    /** Indicates whether the EGPRS support is valid */
    bool egprs_supp_available;
    /** EGPRS support indication
     * 0x00 – Not available
     * 0x01 – Available
     */
    uint8_t egprs_supp;
    /** Indicates whether the Dual Transfer mode support is valid */
    bool dtm_supp_available;
    /** Dual Transfer mode support indication
     * 0x00 – Not available
     * 0x01 – Available
     */
    uint8_t dtm_supp;
} nas_sys_info_gsm_t;

/**
 * @brief QMI NAS high-speed call status
 *
 */
typedef enum {
    /** HSDPA and HSUPA are unsupported */
    QMI_NAS_SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL          = 0x00,
    /** HSDPA is supported */
    QMI_NAS_SYS_HS_IND_HSDPA_SUPP_CELL                  = 0x01,
    /** HSUPA is supported */
    QMI_NAS_SYS_HS_IND_HSUPA_SUPP_CELL                  = 0x02,
    /** HSDPA and HSUPA are supported */
    QMI_NAS_SYS_HS_IND_HSDPA_HSUPA_SUPP_CELL            = 0x03,
    /** HSDPA+ is supported */
    QMI_NAS_SYS_HS_IND_HSDPA_PLUS_SUPP_CELL             = 0x04,
    /** HSDPA+ and HSUPA are supported */
    QMI_NAS_SYS_HS_IND_HSDPA_PLUS_HSUPA_SUPP_CELL       = 0x05,
    /** Dual-cell HSDPA+ is supported */
    QMI_NAS_SYS_HS_IND_DC_HSDPA_PLUS_SUPP_CELL          = 0x06,
    /** Dual-cell HSDPA+ and HSUPA are supported */
    QMI_NAS_SYS_HS_IND_DC_HSDPA_PLUS_HSUPA_SUPP_CELL    = 0x07,
    /** Dual-cell HSDPA+, 64 QAM, and HSUPA are supported */
    QMI_NAS_SYS_HS_IND_HSDPA_PLUS_64QAM_HSUPA_SUPP_CELL = 0x08,
    /** Dual-cell HSDPA+ and 64 QAM are supported */
    QMI_NAS_SYS_HS_IND_HSDPA_PLUS_64QAM_SUPP_CELL       = 0x09,
    /** Dual-cell HSUPA is supported */
    QMI_NAS_SYS_HS_IND_DC_HSDPA_PLUS_DC_HSUPA_SUPP_CELL = 0x0A,
} QMI_NAS_high_speed_call_status_t;

/**
 * @brief QMI NAS WCDMA system info type
 *
 */
typedef struct {
    /** Indicates whether the service domain is valid */
    bool srv_domain_available;
    /** Service domain registered on the system, see #QMI_NAS_system_service_capability_t */
    enum8_t srv_domain;
    /** Indicates whether theservice capability is valid */
    bool srv_capability_available;
    /** Current system’s service capability, see #QMI_NAS_system_service_capability_t */
    enum8_t srv_capability;
    /** Indicates whether the roaming status is valid */
    bool roam_status_available;
    /** GSM current roaming status, see #QMI_NAS_roam_status_t */
    enum8_t roam_status;
    /** Indicates whether the forbidden system is valid */
    bool is_sys_forbidden_available;
    /** Indicates Whether the system is forbidden, see #QMI_NAS_system_forbidden_status_t */
    enum8_t is_sys_forbidden;
    /** Indicates whether the location area code is valid */
    bool lac_available;
    /** Location area code */
    uint16_t lac;
    /** Indicates whether the cell ID is valid */
    bool cell_id_available;
    /** cell ID */
    uint32_t cell_id;
    /** Indicates whether the registration reject information is valid */
    bool reg_reject_info_available;
    /** Type of service domain in which the registration is rejected, see #QMI_NAS_system_service_capability_t */
    enum8_t reject_srv_domain;
    /** Reject cause */
    uint8_t rej_cause;
    /** Indicates whether the network ID is valid */
    bool network_id_available;
    /** MCC digits in ASCII characters */
    char mcc[3 + 1];
    /** MNC digits in ASCII characters. Unused byte is set to 0xFF. In the case of two-digit MNC values,
     * the third (unused) digit is set to 0xFF */
    char mnc[3 + 1];
    /** Indicates whether the high-speed call status is valid */
    bool hs_call_status_available;
    /** Call status on high speed, see #QMI_NAS_high_speed_call_status_t */
    enum8_t hs_call_status;
    /** Indicates whether the he high-speed service indication is valid */
    bool hs_ind_available;
    /** High-speed service indication, see #QMI_NAS_high_speed_call_status_t */
    enum8_t hs_ind;
    /** Indicates whether the primary scrambling code is valid */
    bool psc_available;
    /** primary scrambling code */
    uint16_t psc;
} nas_sys_info_wcdma_t;

/**
 * @brief QMI NAS LTE system info type
 *
 */
typedef struct {
    /** Indicates whether the service domain is valid */
    bool srv_domain_available;
    /** Service domain registered on the system, see #QMI_NAS_system_service_capability_t */
    enum8_t srv_domain;
    /** Indicates whether theservice capability is valid */
    bool srv_capability_available;
    /** Current system’s service capability, see #QMI_NAS_system_service_capability_t */
    enum8_t srv_capability;
    /** Indicates whether the roaming status is valid */
    bool roam_status_available;
    /** GSM current roaming status, see #QMI_NAS_roam_status_t */
    enum8_t roam_status;
    /** Indicates whether the forbidden system is valid */
    bool is_sys_forbidden_available;
    /** Indicates Whether the system is forbidden, see #QMI_NAS_system_forbidden_status_t */
    enum8_t is_sys_forbidden;
    /** Indicates whether the location area code is valid */
    bool lac_available;
    /** Location area code */
    uint16_t lac;
    /** Indicates whether the cell ID is valid */
    bool cell_id_available;
    /** cell ID */
    uint32_t cell_id;
    /** Indicates whether the registration reject information is valid */
    bool reg_reject_info_available;
    /** Type of service domain in which the registration is rejected, see #QMI_NAS_system_service_capability_t */
    enum8_t reject_srv_domain;
    /** Reject cause */
    uint8_t rej_cause;
    /** Indicates whether the network ID is valid */
    bool network_id_available;
    /** MCC digits in ASCII characters */
    char mcc[3 + 1];
    /** MNC digits in ASCII characters. Unused byte is set to 0xFF. In the case of two-digit MNC values,
     * the third (unused) digit is set to 0xFF */
    char mnc[3 + 1];
    /** Indicates whether the tracking area code is valid */
    bool tac_available;
    /** tracking area code */
    uint16_t tac;
} nas_sys_info_lte_t;

/**
 * @brief QMI NAS NR5G system info type
 *
 */
typedef struct {
    /** Indicates whether the service domain is valid */
    bool srv_domain_available;
    /** Service domain registered on the system, see #QMI_NAS_system_service_capability_t */
    enum8_t srv_domain;
    /** Indicates whether theservice capability is valid */
    bool srv_capability_available;
    /** Current system’s service capability, see #QMI_NAS_system_service_capability_t */
    enum8_t srv_capability;
    /** Indicates whether the roaming status is valid */
    bool roam_status_available;
    /** GSM current roaming status, see #QMI_NAS_roam_status_t */
    enum8_t roam_status;
    /** Indicates whether the forbidden system is valid */
    bool is_sys_forbidden_available;
    /** Indicates Whether the system is forbidden, see #QMI_NAS_system_forbidden_status_t */
    enum8_t is_sys_forbidden;
    /** Indicates whether the location area code is valid */
    bool lac_available;
    /** Location area code */
    uint16_t lac;
    /** Indicates whether the cell ID is valid */
    bool cell_id_available;
    /** cell ID */
    uint32_t cell_id;
    /** Indicates whether the registration reject information is valid */
    bool reg_reject_info_available;
    /** Type of service domain in which the registration is rejected, see #QMI_NAS_system_service_capability_t */
    enum8_t reject_srv_domain;
    /** Reject cause */
    uint8_t rej_cause;
    /** Indicates whether the network ID is valid */
    bool network_id_available;
    /** MCC digits in ASCII characters */
    char mcc[3 + 1];
    /** MNC digits in ASCII characters. Unused byte is set to 0xFF. In the case of two-digit MNC values,
     * the third (unused) digit is set to 0xFF */
    char mnc[3 + 1];
    /** Indicates whether the tracking area code is valid */
    bool tac_available;
    /** tracking area code */
    uint16_t tac;
} nas_sys_info_nr5g_t;

/**
 * @brief QMI NAS Cell Broadcast Caps
 *
 */
typedef enum {
    /** Cell broadcast support is unknown */
    QMI_NAS_CELL_BROADCAST_CAP_UNKNOWN = 0x00,
    /** Cell broadcast is not supported */
    QMI_NAS_CELL_BROADCAST_CAP_OFF     = 0x01,
    /** Cell broadcast is supported */
    QMI_NAS_CELL_BROADCAST_CAP_ON      = 0x02,
} QMI_NAS_cell_broadcast_cap_t;

/**
 * @brief QMI NAS additional system info type
 *
 */
typedef struct {
    /** System table index */
    uint16_t geo_sys_idx;
    /** Cell broadcast capability of the serving system, see #QMI_NAS_cell_broadcast_cap_t */
    enum32_t cell_broadcast_cap;
} nas_sys_info_add_t;

typedef QMI_NAS_call_barring_status_t QMI_NAS_cell_access_status_t;

/**
 * @brief QMI NAS NR5G system info type
 *
 */
typedef struct {
    /** Radio interface technology, see #QMI_NAS_radio_interface_t */
    enum8_t radio_if;
    /** MCC digits in ASCII characters */
    char mcc[3 + 1];
    /** MNC digits in ASCII characters. Unused byte is set to 0xFF. In the case of two-digit MNC values,
     *  the third (unused) digit is set to 0xFF. */
    char mnc[3 + 1];
} nas_sys_info_plmn_t;

/**
 * @brief output type for QMI_NAS_GET_SYS_INFO
 *
 */
typedef struct {
    /** true if optional hdr_service_status_info is set */
    bool hdr_service_status_info_available;
    /** HDR service status, see #nas_hdr_service_status_info_t */
    nas_hdr_service_status_info_t hdr_service_status_info;
    /** true if optional gsm_service_status_info is set */
    bool gsm_service_status_info_available;
    /** GSM Service Status Info, see #nas_service_status_info_t */
    nas_service_status_info_t gsm_service_status_info;
    /** true if optional wcdma_service_status_info is set */
    bool wcdma_service_status_info_available;
    /** WCDMA Service Status Info, see #nas_service_status_info_t */
    nas_service_status_info_t wcdma_service_status_info;
    /** true if optional lte_service_status_info is set */
    bool lte_service_status_info_available;
    /** LTE Service Status Info, see #nas_service_status_info_t */
    nas_service_status_info_t lte_service_status_info;
    /** true if optional nr5g_service_status_info is set */
    bool nr5g_service_status_info_available;
    /** NR5G Service Status Info, see #nas_service_status_info_t */
    nas_service_status_info_t nr5g_service_status_info;
    /** true if optional HDR_sys_info is set */
    bool HDR_sys_info_available;
    /** HDR System Info, see #nas_HDR_sys_info_t */
    nas_HDR_sys_info_t HDR_sys_info;
    /** true if optional gsm_sys_info is set */
    bool gsm_sys_info_available;
    /** GSM System Info, see #nas_sys_info_gsm_t */
    nas_sys_info_gsm_t gsm_sys_info;
    /** true if optional wcdma_sys_info is set */
    bool wcdma_sys_info_available;
    /** WCDMA System Info, see #nas_sys_info_wcdma_t */
    nas_sys_info_wcdma_t wcdma_sys_info;
    /** true if optional lte_sys_info is set */
    bool lte_sys_info_available;
    /** LTE System Info, see #nas_sys_info_lte_t */
    nas_sys_info_lte_t lte_sys_info;
    /** true if optional nr5g_sys_info is set */
    bool nr5g_sys_info_available;
    /** NR5G System Info, see #nas_sys_info_nr5g_t */
    nas_sys_info_nr5g_t nr5g_sys_info;
    /** true if optional nr5g_cell_status is set */
    bool nr5g_cell_status_available;
    /** NR5G Cell Access Status Info, see #QMI_NAS_cell_access_status_t */
    enum32s_t nr5g_cell_status;
    /** true if optional endc_availability is set */
    bool endc_availability_available;
    /** Availability of Dual connectivity of E-UTRA with NR5G */
    bool endc_availability;
    /** true if optional restrict_dcnr is set */
    bool restrict_dcnr_available;
    /** DCNR restriction Info */
    bool restrict_dcnr;
    /** true if optional nr5g_tac is set */
    bool nr5g_tac_available;
    /** Additional NR5G System Info - NR5G Tracking Area Code (nr5g_tac[0] is the most significant byte)*/
    uint8_t nr5g_tac[3];
    /** true if optional nr5g_pci is set */
    bool nr5g_pci_available;
    /** Additional NR5G System Info - NR5G physical cell ID (only present in SA or NSA mode) */
    uint16_t nr5g_pci;
    /** true if optional plmn_infolist_r15__availability is set */
    bool plmn_infolist_r15_availability_available;
    /** Additional LTE System Info - PLMN Info List R15 availability */
    bool plmn_infolist_r15_availability;
    /** true if optional nr5g_cell_id is set */
    bool nr5g_cell_id_available;
    /** NR5G Cell ID (only present in SA mode) */
    uint64_t nr5g_cell_id;
    /** true if optional nr5g_arfcn is set */
    bool nr5g_arfcn_available;
    /** NR5G ARFCN of the serving cell (present in both SA and NSA mode) */
    uint32_t nr5g_arfcn;
    /** true if optional add_hdr_sys_info is set */
    bool add_hdr_sys_info_available;
    /** Additional HDR System Info */
    uint16_t add_hdr_sys_info;
    /** true if optional add_gsm_sys_info is set */
    bool add_gsm_sys_info_available;
    /** Additional GSM System Info, see #nas_sys_info_add_t */
    nas_sys_info_add_t add_gsm_sys_info;
    /** true if optional add_wcdma_sys_info is set */
    bool add_wcdma_sys_info_available;
    /** Additional WCDMA System Info, see #nas_sys_info_add_t */
    nas_sys_info_add_t add_wcdma_sys_info;
    /** true if optional lte_geo_sys_idx is set */
    bool lte_geo_sys_idx_available;
    /** System table index referencing the beginning of the geo in which the current serving system is present; when not known 0xFFFF is used */
    uint16_t lte_geo_sys_idx;
    /** true if optional gsm_call_barring_sys_info is set */
    bool gsm_call_barring_sys_info_available;
    /** GSM Call Barring System Info */
    nas_call_bar_status_t gsm_call_barring_sys_info;
    /** true if optional wcdma_call_barring_sys_info is set */
    bool wcdma_call_barring_sys_info_available;
    /** WCDMA Call Barring System Info, see #nas_call_bar_status_t */
    nas_call_bar_status_t wcdma_call_barring_sys_info;
    /** true if voice is supported */
    bool lte_voice_support_sys_info;
    /** true if optional gsm_cipher_domain_sys_info is set */
    bool gsm_cipher_domain_sys_info_available;
    /** GSM Cipher Domain Sys Info */
    uint8_t gsm_cipher_domain_sys_info;
    /** true if optional wcdma_cipher_domain_sys_info is set */
    bool wcdma_cipher_domain_sys_info_available;
    /** WCDMA Cipher Domain Sys Info */
    uint8_t wcdma_cipher_domain_sys_info;
    /** true if optional sys_info_plmn is set */
    bool sys_info_plmn_available;
    /** Primary PLMN information */
    nas_sys_info_plmn_t sys_info_plmn;
} nas_system_info_t;

/**
 * @brief Pack QMI_NAS_GET_SYS_INFO (77 - 0x004D) request
 *
 * This function packs a qmi request for querying system information
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_nas_get_sys_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_NAS_GET_SYS_INFO (77 - 0x004D) request
 *
 * This function unpacks the result of a QMI_NAS_GET_SYS_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_system_info_t
 */
QMI_SDK_error_t telit_nas_get_sys_info_unpack(uint8_t *rsp, uint16_t len, nas_system_info_t *out);

/**
 * @brief QMI NAS GSM signal info type
 *
 */
typedef struct {
    int8_t rssi;  /** GSM signal strength RSSI in DBm. A value of -125 dBm or lower is used to indicate no signal */
} nas_signal_info_gsm_t;

/**
 * @brief QMI NAS WCDMA signal info type
 *
 */
typedef struct {
    /** WCDMA signal strength RSSI in DBm. A value of -125 dBm or lower is used to indicate no signal. For UMTS this indicates forward link pilot Ec */
    int8_t rssi;
    /** ECIO value representing negative 0.5 dB increments, i.e., 2 means -1 dB (14 means -7 dB, 63 means -31.5 dB) */
    uint16_t ecio;
} nas_signal_info_wcdma_t;

/**
 * @brief QMI NAS LTE signal info type
 *
 */
typedef struct {
    /** LTE signal strength RSSI in DBm. A value of -125 dBm or lower is used to indicate no signal */
    int8_t rssi;
    /** RSRQ value in dB as measured by L1. Range: -3 to -20 (-3 means -3 dB, -20 means -20 dB) */
    int8_t rsrq;
    /** RSRP value in dBm as measured by L1. Range: -44 to -140 (-44 means -44 dBm, -140 means -140 dBm) */
    int16_t rsrp;
    /** SNR level as scaled integer in units of 0.1 dB */
    int16_t snr;
} nas_signal_info_lte_t;

/**
 * @brief QMI NAS NR5G signal info type
 *
 */
typedef struct {
    /** RSRP value in dBm as measured by L1. Range: -44 to -140 (-44 means -44 dBm, -140 means -140 dBm). -32768 indicates that modem is not in EN-DC connected mode */
    int16_t rsrp;
    /** SNR level as scaled integer in units of 0.1 dB. -32768 indicates that modem is not in EN-DC connected mode */
    int16_t snr;
} nas_signal_info_nr5g_t;

/**
 * @brief output type for QMI_NAS_GET_SIG_INFO
 *
 */
typedef struct {
    /** #nas_signal_info_gsm_t instance */
    nas_signal_info_gsm_t signal_info_gsm;
    /** #nas_signal_info_wcdma_t instance */
    nas_signal_info_wcdma_t signal_info_wcdma;
    /** #nas_signal_info_lte_t instance */
    nas_signal_info_lte_t signal_info_lte;
    /** #nas_signal_info_nr5g_t instance */
    nas_signal_info_nr5g_t signal_info_nr5g;
    /** set if nas_signal_info_t#signal_info_gsm is available */
    bool signal_info_gsm_set;
    /** set if nas_signal_info_t#signal_info_wcdma is available */
    bool signal_info_wcdma_set;
    /** set if nas_signal_info_t#signal_info_lte is available */
    bool signal_info_lte_set;
    /** set if nas_signal_info_t#signal_info_nr5g is available */
    bool signal_info_nr5g_set;
} nas_signal_info_t;

/**
 * @brief Pack QMI_NAS_GET_SIG_INFO (79 - 0x004F) request
 *
 * This function packs a qmi request for querying signal info
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_nas_get_sig_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_NAS_GET_SIG_INFO (79 - 0x004F) request
 *
 * This function unpacks the result of a QMI_NAS_GET_SIG_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_signal_info_t
 */
QMI_SDK_error_t telit_nas_get_sig_info_unpack(uint8_t *rsp, uint16_t len, nas_signal_info_t *out);

/**
 * @brief input type for QMI_NAS_GET_TX_RX_INFO
 *
 */
typedef struct {
    /** Radio interface from which to get the information, see #QMI_NAS_radio_interface_t */
    uint8_t radio_if;
} nas_radio_if_t;

/**
 * @brief QMI NAS Rx chain info type
 *
 */
typedef struct {
    /** Whether Rx is tuned to a channel:
     * • 0x00 – Not tuned
     * • 0x01 – Tuned
     *
     * If the radio is tuned, instantaneous values are set for
     * the signal information fields below. If the radio is
     * not tuned, or is delayed or invalid, the values are set
     * depending on each technology. */
    bool is_radio_tuned;
    /** Rx power value in 1/10 dbm resolution. */
    int32_t rx_pwr;
    /** ECIO in 1/10 dB; valid for CDMA, HDR, GSM, WCDMA, and LTE. */
    int32_t ecio;
    /** Received signal code power in 1/10 dbm; valid for WCDMA. */
    int32_t rscp;
    /** Current reference signal received power in 1/10 dbm; valid for LTE. */
    int32_t rsrp;
    /** Phase in 1/100 degrees; valid for LTE.
     * When the phase is unknown, 0xFFFFFFFF is used. */
    uint32_t phase;
} nas_rx_chain_info_t;

/**
 * @brief QMI NAS Tx info type
 *
 */
typedef struct {
    /** Whether the device is in traffic. The tx_pwr field is
     * only meaningful when in the device is in traffic. If it
     * is not in traffic, tx_pwr is invalid. */
    bool is_in_traffic;
    /** Tx power value in 1/10 dbm. */
    int32_t tx_pwr;
} nas_tx_info_t;

/**
 * @brief output type for QMI_NAS_GET_TX_RX_INFO
 *
 */
typedef struct {
    /** set if rx_chain_0_info is available */
    bool rx_chain_0_info_available;
    /** Rx Chain 0 Info */
    nas_rx_chain_info_t rx_chain_0_info;
    /** set if rx_chain_1_info is available */
    bool rx_chain_1_info_available;
    /** Rx Chain 1 Info */
    nas_rx_chain_info_t rx_chain_1_info;
    /** set if tx_info is available */
    bool tx_info_available;
    /** Tx Info */
    nas_tx_info_t tx_info;
} nas_tx_rx_info_t;

/**
 * @brief Pack QMI_NAS_GET_TX_RX_INFO (90 - 0x005A) request
 *
 * This function retrieves the detailed Tx/Rx information
 *
 * @param[in] req_ctx request context
 * @param[in,out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #nas_radio_if_t
 */
QMI_SDK_error_t telit_nas_get_tx_rx_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_radio_if_t *input);

/**
 * @brief Unpack QMI_NAS_GET_TX_RX_INFO (90 - 0x005A) request
 *
 * This function unpacks the result of a QMI_NAS_GET_TX_RX_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_tx_rx_info_t
 */
QMI_SDK_error_t telit_nas_get_tx_rx_info_unpack(uint8_t *rsp, uint16_t len, nas_tx_rx_info_t *out);

/**
 * @brief QMI NAS report rate
 *
 */
typedef enum {
    /** Report using the default configuration */
    QMI_NAS_REPORT_RATE_DEFAULT = 0x00,
    /** Report every 1 second */
    QMI_NAS_REPORT_RATE_1       = 0x01,
    /** Report every 2 second */
    QMI_NAS_REPORT_RATE_2       = 0x02,
    /** Report every 3 second */
    QMI_NAS_REPORT_RATE_3       = 0x03,
    /** Report every 4 second */
    QMI_NAS_REPORT_RATE_4       = 0x04,
    /** Report every 5 second */
    QMI_NAS_REPORT_RATE_5       = 0x05,
    /** Report every 10 second */
    QMI_NAS_REPORT_RATE_10      = 0x0A,
    /** Report every 20 second */
    QMI_NAS_REPORT_RATE_20      = 0x14,
    /** Report every 30 second */
    QMI_NAS_REPORT_RATE_30      = 0x1E,
} QMI_NAS_report_rate_t;

/**
 * @brief QMI NAS average period
 *
 */
typedef enum {
    /** Average using the default configuration */
    QMI_NAS_AVERAGE_PERIOD_DEFAULT = 0x00,
    /** Average period of 1 second */
    QMI_NAS_AVERAGE_PERIOD_1       = 0x01,
    /** Average period of 2 seconds */
    QMI_NAS_AVERAGE_PERIOD_2       = 0x02,
    /** Average period of 3 seconds */
    QMI_NAS_AVERAGE_PERIOD_3       = 0x03,
    /** Average period of 4 seconds */
    QMI_NAS_AVERAGE_PERIOD_4       = 0x04,
    /** Average period of 5 seconds */
    QMI_NAS_AVERAGE_PERIOD_5       = 0x05,
    /** Average period of 6 seconds */
    QMI_NAS_AVERAGE_PERIOD_6       = 0x06,
    /** Average period of 7 seconds */
    QMI_NAS_AVERAGE_PERIOD_7       = 0x07,
    /** Average period of 8 seconds */
    QMI_NAS_AVERAGE_PERIOD_8       = 0x08,
    /** Average period of 9 seconds */
    QMI_NAS_AVERAGE_PERIOD_9       = 0x09,
    /** Average period of 10 seconds */
    QMI_NAS_AVERAGE_PERIOD_10      = 0x0A,
    /** Average period of 20 seconds */
    QMI_NAS_AVERAGE_PERIOD_20      = 0x14,
    /** Average period 30 seconds */
    QMI_NAS_AVERAGE_PERIOD_30      = 0x1E,
} QMI_NAS_average_period_t;

/**
 * @brief QMI NAS signal reporting info type
 *
 */
typedef struct {
    /** Rate on how often the signal must be checked for reporting, see #QMI_NAS_report_rate_t */
    enum8_t rpt_rate;
    /** Averaging period to use for the LTE signal, see #QMI_NAS_average_period_t */
    enum8_t avg_period;
} nas_signal_report_info_t;

/**
 * @brief QMI NAS signal strength type
 *
 */
typedef enum {
    /** RSSI */
    QMI_NAS_SS_RSSI = 0x00,
    /** ECIO */
    QMI_NAS_SS_ECIO = 0x01,
    /** SINR */
    QMI_NAS_SS_SINR = 0x02,
    /** IO */
    QMI_NAS_SS_IO   = 0x03,
    /** RSRQ */
    QMI_NAS_SS_RSRQ = 0x04,
    /** RSRP */
    QMI_NAS_SS_RSRP = 0x05,
    /** SNR */
    QMI_NAS_SS_SNR  = 0x06,
    /** RSCP */
    QMI_NAS_SS_RSCP = 0x07,
} QMI_NAS_signal_strength_type_t;

/**
 * @brief QMI NAS hysteresis info type
 *
 */
typedef struct {
    /** Type of signal strength, see #QMI_NAS_signal_strength_type_t */
    enum32_t signal;
    /** Hysteresis value (in units of 0.1 dBm) to use for each signal. A value of 0 disables the hysteresis for the signal strength type */
    uint16_t delta;
} nas_hysteresis_info_t;

#define QMI_NAS_THRESHOLD_LIST_MAX_SIZE 32
#define QMI_NAS_HYSTERESIS_LIST_MAX_SIZE 8

/**
 * @brief input type for QMI_NAS_CONFIG_SIG_INFO2
 *
 */
typedef struct {
    /** Size of gsm_rssi_thr_list */
    uint8_t gsm_rssi_thr_list_size;
    /** Array of GSM RSSI thresholds (in units of 0.1 dBm), range -111 to -48 (in dBm) */
    int16_t gsm_rssi_thr_list[QMI_NAS_THRESHOLD_LIST_MAX_SIZE];
    /** set if gsm_rssi_delta is available */
    bool gsm_rssi_delta_available;
    /** GSM RSSI delta (in units of 0.1 dBm) */
    uint16_t gsm_rssi_delta;
    /** Size of wcdma_rssi_thr_list */
    uint8_t wcdma_rssi_thr_list_size;
    /** Array of WCDMA RSSI thresholds (in units of 0.1 dBm), range -121 to 0 (in dBm) */
    int16_t wcdma_rssi_thr_list[QMI_NAS_THRESHOLD_LIST_MAX_SIZE];
    /** set if wcdma_rssi_delta is available */
    bool wcdma_rssi_delta_available;
    /** WCDMA RSSI delta (in units of 0.1 dBm) */
    uint16_t wcdma_rssi_delta;
    /** Size of wcdma_ecio_thr_list */
    uint8_t wcdma_ecio_thr_list_size;
    /** Array of WCDMA ECIO thresholds (in units of 0.1 dBm), range -31.5 to 0 (in dBm) */
    int16_t wcdma_ecio_thr_list[QMI_NAS_THRESHOLD_LIST_MAX_SIZE];
    /** set if wcdma_ecio_delta is available */
    bool wcdma_ecio_delta_available;
    /** WCDMA ECIO delta (in units of 0.1 dBm) */
    uint16_t wcdma_ecio_delta;
    /** Size of wcdma_rscp_thr_list */
    uint8_t wcdma_rscp_thr_list_size;
    /** Array of WCDMA RSCP thresholds (in units of 0.1 dBm), range -95 to -80 (in dBm) */
    int16_t wcdma_rscp_thr_list[QMI_NAS_THRESHOLD_LIST_MAX_SIZE];
    /** set if wcdma_rscp_delta is available */
    bool wcdma_rscp_delta_available;
    /** WCDMA RSCP delta (in units of 0.1 dBm) */
    uint16_t wcdma_rscp_delta;
    /** Size of lte_rssi_thr_list */
    uint8_t lte_rssi_thr_list_size;
    /** Array of LTE RSSI thresholds (in units of 0.1 dBm), range -120 to 0 (in dBm) */
    int16_t lte_rssi_thr_list[QMI_NAS_THRESHOLD_LIST_MAX_SIZE];
    /** set if lte_rssi_delta is available */
    bool lte_rssi_delta_available;
    /** LTE RSSI delta (in units of 0.1 dBm) */
    uint16_t lte_rssi_delta;
    /** Size of lte_snr_thr_list */
    uint8_t lte_snr_thr_list_size;
    /** Array of LTE SNR thresholds (in units of 0.1 dBm), range -20 to 30 (in dBm) */
    int16_t lte_snr_thr_list[QMI_NAS_THRESHOLD_LIST_MAX_SIZE];
    /** set if lte_snr_delta is available */
    bool lte_snr_delta_available;
    /** LTE SNR delta (in units of 0.1 dBm) */
    uint16_t lte_snr_delta;
    /** Size of lte_rsrq_thr_list */
    uint8_t lte_rsrq_thr_list_size;
    /** Array of LTE RSRQ thresholds (in units of 0.1 dBm), range -20 to -3 (in dBm) */
    int16_t lte_rsrq_thr_list[QMI_NAS_THRESHOLD_LIST_MAX_SIZE];
    /** set if lte_rsrq_delta is available */
    bool lte_rsrq_delta_available;
    /** LTE RSRQ delta (in units of 0.1 dBm) */
    uint16_t lte_rsrq_delta;
    /** Size of lte_rsrp_thr_list */
    uint8_t lte_rsrp_thr_list_size;
    /** Array of LTE RSRP thresholds (in units of 0.1 dBm), range -140 to -44 (in dBm) */
    int16_t lte_rsrp_thr_list[QMI_NAS_THRESHOLD_LIST_MAX_SIZE];
    /** set if lte_rsrp_delta is available */
    bool lte_rsrp_delta_available;
    /** LTE RSRP delta (in units of 0.1 dBm) */
    uint16_t lte_rsrp_delta;
    /** set if lte_signal_report_config is available */
    bool lte_signal_report_config_available;
    /** Set the signal reporting rate and averaging period information for LTE, see #nas_signal_report_info_t */
    nas_signal_report_info_t lte_signal_report_config;
    /** Size of nr5g_snr_thr_list */
    uint8_t nr5g_snr_thr_list_size;
    /** Array of NR5G SNR thresholds (in units of 0.1 dBm), range -230 to 400 (in dBm) */
    int16_t nr5g_snr_thr_list[QMI_NAS_THRESHOLD_LIST_MAX_SIZE];
    /** set if nr5g_snr_delta is available */
    bool nr5g_snr_delta_available;
    /** NR5G SNR delta (in units of 0.1 dBm) */
    uint16_t nr5g_snr_delta;
    /** Size of nr5g_rsrp_thr_list */
    uint8_t nr5g_rsrp_thr_list_size;
    /** Array of NR5G RSRP thresholds (in units of 0.1 dBm), range -1400 to -440 (in dBm) */
    int16_t nr5g_rsrp_thr_list[QMI_NAS_THRESHOLD_LIST_MAX_SIZE];
    /** set if nr5g_rsrp_delta is available */
    bool nr5g_rsrp_delta_available;
    /** NR5G RSRP delta (in units of 0.1 dBm) */
    uint16_t nr5g_rsrp_delta;
    /** set if nr5g_signal_report_config is available */
    bool nr5g_signal_report_config_available;
    /** Set the signal reporting rate and averaging period information for NR5G, see #nas_signal_report_info_t */
    nas_signal_report_info_t nr5g_signal_report_config;
    /** Size of nr5g_rsrq_thr_list */
    uint8_t nr5g_rsrq_thr_list_size;
    /** Array of NR5G RSRQ thresholds (in units of 0.1 dBm), range --430 to 200 (in dBm) */
    int16_t nr5g_rsrq_thr_list[QMI_NAS_THRESHOLD_LIST_MAX_SIZE];
    /** set if nr5g_rsrq_delta is available */
    bool nr5g_rsrq_delta_available;
    /** NR5G RSRQ delta (in units of 0.1 dBm) */
    uint16_t nr5g_rsrq_delta;
    /** Size of wcdma_hysteresis */
    uint8_t wcdma_hysteresis_list_size;
    /** Hysteresis values list for WCDMA */
    nas_hysteresis_info_t wcdma_hysteresis_list[QMI_NAS_HYSTERESIS_LIST_MAX_SIZE];
    /** Size of gsm_hysteresis */
    uint8_t gsm_hysteresis_list_size;
    /** Hysteresis values list for GSM */
    nas_hysteresis_info_t gsm_hysteresis_list[QMI_NAS_HYSTERESIS_LIST_MAX_SIZE];
    /** Size of lte_hysteresis */
    uint8_t lte_hysteresis_list_size;
    /** Hysteresis values list for LTE */
    nas_hysteresis_info_t lte_hysteresis_list[QMI_NAS_HYSTERESIS_LIST_MAX_SIZE];
    /** Size of nr5g_hysteresis */
    uint8_t nr5g_hysteresis_list_size;
    /** Hysteresis values list for NR5G */
    nas_hysteresis_info_t nr5g_hysteresis_list[QMI_NAS_HYSTERESIS_LIST_MAX_SIZE];
    /** set if hysteresis_timer is available */
    bool hysteresis_timer_available;
    /** Hysteresis timer (in units of milliseconds) to prevent multiple rapid sig info indication reports */
    uint16_t hysteresis_timer;
} nas_config_sig2_info_t;

/**
 * @brief Pack QMI_NAS_CONFIG_SIG_INFO2 (108 - 0x006C) request
 *
 * This function sets the signal strength reporting thresholds
 *
 * @param[in] req_ctx request context
 * @param[in,out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #nas_config_sig2_info_t
 */
QMI_SDK_error_t telit_nas_config_sig_info2_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_config_sig2_info_t *input);

/**
 * @brief Unpack QMI_NAS_CONFIG_SIG_INFO2 (108 - 0x006C) request
 *
 * This function unpacks the result of a QMI_NAS_CONFIG_SIG_INFO2 request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_nas_config_sig_info2_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI NAS sys info ind limit reporting mask values
 * this is not an enum, since the values are greater than uint32_t
 *
 */
/** Limit by srv_status changes */
#define QMI_NAS_LIMIT_BY_SRV_STATUS (0x1UL)
/** Limit by srv_domain changes */
#define QMI_NAS_LIMIT_BY_SRV_DOMAIN (0x2UL)

/** Limit by service RAT change */
#define QMI_NAS_LIMIT_BY_SRV_RAT_TECH_CHANGE (0x1000000000UL)

/**
 * @brief input type for QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING
 *
 */
typedef struct {
    /** Bitmask for limiting sys info change reporting, see defines QMI_NAS_LIMIT_BY_* */
    uint64_t limit_sys_info_chg_rpt;
} nas_limit_sys_info_ind_reporting_t;

/**
 * @brief Pack QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING (112 - 0x0070) request
 *
 * This function limits the reporting of QMI_NAS_SYS_INFO_IND to only when
 * certain fields have changed
 *
 * @param[in] req_ctx request context
 * @param[in,out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #nas_limit_sys_info_ind_reporting_t
 */
QMI_SDK_error_t telit_nas_limit_sys_info_ind_reporting_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_limit_sys_info_ind_reporting_t *input);

/**
 * @brief Unpack QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING (112 - 0x0070) request
 *
 * This function unpacks the result of a QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_nas_limit_sys_info_ind_reporting_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief output type for QMI_NAS_NETWORK_TIME_IND
 *
 */
typedef struct {
    /** Year */
    uint16_t year;
    /** Month */
    uint8_t month;
    /** Day */
    uint8_t day;
    /** Hour */
    uint8_t hour;
    /** Minute */
    uint8_t minute;
    /** Second */
    uint8_t second;
    /** Day of the week: 0 is Monday, 6 is Sunday */
    uint8_t day_of_week;
    /** set if time_zone is available */
    bool time_zone_available;
    /** Offset from Universal Time; the difference between local time and Universal time
     * in increments of 15 minutes */
    int8_t time_zone;
    /** set if daylt_sav_adj is available */
    bool daylt_sav_adj_available;
    /** Daylight saving adjustment in hours. Possible values: 0, 1 and 2. */
    uint8_t daylt_sav_adj;
    /** set if radio_if is available */
    bool radio_if_available;
    /** Radio interface technology, see #QMI_NAS_radio_interface_t */
    enum8_t radio_if;
} nas_network_time_t;

/**
 * @brief output type for QMI_NAS_GET_NETWORK_TIME
 *
 */
typedef struct {
    bool time_information_3gpp2_available;     /**< set if time_information_3gpp2 is available */
    nas_network_time_t time_information_3gpp2; /**< 3gpp2 network time, see #nas_network_time_t */
    bool time_information_3gpp_available;      /**< set if time_information_3gpp is available */
    nas_network_time_t time_information_3gpp;  /**< 3gpp network time, see #nas_network_time_t */
} nas_network_time_info_t;

/**
 * @brief Pack QMI_NAS_GET_NETWORK_TIME (125 - 0x007D) request
 *
 * This function packs a qmi request for retrieving the latest time change reported by the network.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_nas_get_network_time_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_NAS_GET_NETWORK_TIME (125 - 0x007D) request
 *
 * This function unpacks the result of a QMI_NAS_GET_NETWORK_TIME request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_network_time_t
 */
QMI_SDK_error_t telit_nas_get_network_time_unpack(uint8_t *rsp, uint16_t len, nas_network_time_info_t *out);

/**
 * @brief QMI NAS scell states
 *
 */
typedef enum {
    /** Deconfigured */
    QMI_NAS_LTE_CPHY_SCELL_STATE_DECONFIGURED           = 0x00,
    /** Configured and deactivated */
    QMI_NAS_LTE_CPHY_SCELL_STATE_CONFIGURED_DEACTIVATED = 0x01,
    /** Configured and activated */
    QMI_NAS_LTE_CPHY_SCELL_STATE_CONFIGURED_ACTIVATED   = 0x02,
} QMI_NAS_scell_state_t;

/**
 * @brief QMI NAS downlink bandwidth values
 *
 */
typedef enum {
    /** 1.4 MHz bandwidth */
    QMI_NAS_LTE_CPHY_CA_BW_NRB_6   = 0x00,
    /** 3 MHz bandwidth */
    QMI_NAS_LTE_CPHY_CA_BW_NRB_15  = 0x01,
    /** 5 MHz bandwidth */
    QMI_NAS_LTE_CPHY_CA_BW_NRB_25  = 0x02,
    /** 10 MHz bandwidth */
    QMI_NAS_LTE_CPHY_CA_BW_NRB_50  = 0x03,
    /** 15 MHz bandwidth */
    QMI_NAS_LTE_CPHY_CA_BW_NRB_75  = 0x04,
    /** 20 MHz bandwidth */
    QMI_NAS_LTE_CPHY_CA_BW_NRB_100 = 0x05,
} QMI_NAS_scell_dl_bw_t;

/**
 * @brief QMI NAS Physical Carrier Aggregation of Scell Indicator Type
 *
 */
typedef struct {
    /** Physical cell ID of the SCell Range. */
    uint16_t pci;
    /** Frequency of the absolute cell Range */
    uint16_t freq;
    /** Scell state, see #QMI_NAS_scell_state_t */
    enum32_t scell_state;
} nas_phy_ca_agg_scell_ind_type_t;

#define QMI_NAS_MAX_SCELL_INFO_INSTANCES 255

/**
 * @brief QMI NAS Scell Information Instance type
 *
 */
typedef struct {
    /** Physical cell ID of the SCell Range */
    uint16_t pci;
    /** Frequency of the absolute cell Range */
    uint16_t freq;
    /** Downlink Bandwidth, see #QMI_NAS_scell_dl_bw_t */
    enum32_t dl_bw_value;
    /** EARFCN value, due to QC limitation is 0 if EARFCN is more than 65535 */
    uint16_t band;
    /** Scell state, see #QMI_NAS_scell_state_t */
    enum32_t scell_state;
    /** Scell index. Range: 0 to 7 */
    uint8_t scell_idx;
} nas_phy_ca_agg_scell_info_instance_t;

/**
 * @brief QMI NAS Pcell Information type
 *
 */
typedef struct {
    /** Physical cell ID of the SCell Range */
    uint16_t pci;
    /** Frequency of the absolute cell Range */
    uint16_t freq;
    /** Downlink Bandwidth, see #QMI_NAS_scell_dl_bw_t */
    enum32_t dl_bw_value;
    /** Band value */
    uint16_t band;
} nas_phy_ca_agg_pcell_info_t;

/**
 * @brief output type for QMI_NAS_GET_LTE_CPHY_CA_INFO
 *
 */
typedef struct {
    /** true if optional scell_ind_type is set */
    bool scell_ind_type_available;
    /** see #nas_phy_ca_agg_scell_ind_type_t */
    nas_phy_ca_agg_scell_ind_type_t scell_ind_type;
    /** true if optional dl_bw_value is set */
    bool dl_bw_value_available;
    /** Downlink Bandwidth, see #QMI_NAS_scell_dl_bw_t */
    enum32_t dl_bw_value;
    /** scell_info_list size */
    uint8_t scell_info_list_size;
    /** array of #nas_phy_ca_agg_scell_info_instance_t */
    nas_phy_ca_agg_scell_info_instance_t scell_info_list[QMI_NAS_MAX_SCELL_INFO_INSTANCES];
    /** true if optional pcell_info is set */
    bool pcell_info_available;
    /** see #nas_phy_ca_agg_pcell_info_t */
    nas_phy_ca_agg_pcell_info_t pcell_info;
} nas_get_lte_cphy_ca_info_t;

/**
 * @brief Pack QMI_NAS_GET_LTE_CPHY_CA_INFO (172 - 0x00AC) request
 *
 * This function packs a qmi request for retrieving the
 * previous carrier aggregation event information
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_nas_get_lte_cphy_ca_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_NAS_GET_LTE_CPHY_CA_INFO (172 - 0x00AC) request
 *
 * This function unpacks the result of a QMI_NAS_GET_LTE_CPHY_CA_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_get_lte_cphy_ca_info_t
 */
QMI_SDK_error_t telit_nas_get_lte_cphy_ca_info_unpack(uint8_t *rsp, uint16_t len,
        nas_get_lte_cphy_ca_info_t *out);


typedef struct {
    /** Pulse Period. Sets the pulse generation periodicity. Range: 0 - 128. The
     * value is a multiple of 10 milliseconds. To stop pulse generation, set to
     * 0 */
    uint32_t pulse_period;
    /** true if optional start_sfn is set */
    bool start_sfn_available;
    /** Start SFN. Start of system frame number. Valid range: 0 - 1024 If this
     * TLV is not set or set to a value of 1024, the modem starts generating
     * pulse from the next available SFN. */
    uint32_t start_sfn;
    /** true if optional report_period is set */
    bool report_period_available;
    /** Report Period. Configures pulse generation indication periodicity.
     * Range: 0 - 128. The value is a multiple of 10 milliseconds. For example,
     * if the value is set to 1, the report is sent every 10 milliseconds. If
     * the value is set to 128, the indication is sent every 1280 milliseconds.
     * If this TLV is not set or the value is set to 0, this indication is
     * disabled */
    uint32_t report_period;
} nas_set_nr5g_sync_pulse_gen_t;

/**
 * @brief Pack QMI_NAS_SET_NR5G_SYNC_PULSE_GEN (290 - 0x0122) request
 *
 * This function packs a qmi request to start or stop the sync pulse generation
 * on NR5G.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #nas_set_nr5g_sync_pulse_gen_t
 */
QMI_SDK_error_t telit_nas_set_nr5g_sync_pulse_gen_pack(pack_qmi_t *req_ctx, void *req, uint16_t *len,
        nas_set_nr5g_sync_pulse_gen_t *input);

/**
 * @brief Unpack QMI_NAS_SET_NR5G_SYNC_PULSE_GEN (290 - 0x0122) request
 *
 * This function unpacks the result of a QMI_NAS_SET_NR5G_SYNC_PULSE_GEN request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_nas_set_nr5g_sync_pulse_gen_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief type to hold Reference Time Info
 *
 */
typedef struct {
    uint32_t ref_days;
    uint32_t ref_seconds;
    uint16_t ref_milliseconds;
    uint32_t ref_ten_nano_seconds;
} nas_reference_time_info_t;

/**
 * @brief type to hold Universal Time
 *
 */
typedef struct {
    /** Year */
    uint16_t year;
    /** Month. 1 is January and 12 is December */
    uint8_t month;
    /** Day. Range: 1 to 31 */
    uint8_t day;
    /** Hour. Range: 0 to 23 */
    uint8_t hour;
    /** Minute. Range: 0 to 59 */
    uint8_t minute;
    /** Second. Range: 0 to 59 */
    uint8_t second;
    /** Millisecond. Range: 0 to 999 */
    uint16_t millisecond;
    /** Day of the week. 0 is Monday and 6 is Sunday */
    uint8_t day_of_week;
} nas_universal_time_t;

/**
 * @brief output type for QMI_NAS_GET_NR5G_RRC_UTC_TIME (336 - 0x0150) respone
 *
 */
typedef struct {
    /** set if utc_sib9_acquired is available */
    bool utc_sib9_acquired_available;
    /** Flag to indicate UTC SIB9 acquisition */
    bool utc_sib9_acquired;
    /** set if r16_timing_info is available */
    bool r16_timing_info_available;
    /** R16 Timing Info Flag */
    bool r16_timing_info;
    /** set if dayLightSavingTime is set */
    bool day_light_saving_time_available;
    /** UTC daylight Saving Type */
    enum32_t day_light_saving_time;
    /** set if leapSeconds is set */
    bool leap_seconds_available;
    /** UTC Leap Seconds */
    int16_t leap_seconds;
    /** set if localTimeOffset is set */
    bool local_time_offset_available;
    /** UTC Local Time Offset */
    int16_t local_time_offset;
    /** set if Reference Time Info is set */
    bool reference_time_info_available;
    /** Reference Time Info */
    nas_reference_time_info_t reference_time_info;
    /** set if uncertainty is set */
    bool uncertainty_available;
    /** Uncertainity of reference time */
    uint32_t uncertainty;
    /** set if timeInfoType is set */
    bool time_info_type_available;
    /** Time Info Type */
    uint32_t time_info_type;
    /** set if referenceSFN is set*/
    bool reference_sfn_available;
    /** SFN frame boundary for which time is given */
    uint32_t reference_sfn;
    /** set if abs_time is set */
    bool abs_time_available;
    /** Absolute time in milliseconds since Jan 6, 1980 00:00:00 hr */
    uint64_t abs_time;
    /** set if Universal Time is set */
    bool universal_time_available;
    /** Universal Time */
    nas_universal_time_t universal_time;
} nas_nr5g_rrc_utc_time_t;

/**
 * @brief Pack QMI_NAS_GET_NR5G_RRC_UTC_TIME (336 - 0x0150) request
 *
 * This function packs a qmi request to retrieve the NR5G SIB9 UTC time from the UE.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_nas_get_nr5g_rrc_utc_time_pack(pack_qmi_t *req_ctx, void *req, uint16_t *len);

/**
 * @brief Unpack QMI_NAS_GET_NR5G_RRC_UTC_TIME (336 - 0x0150) request
 *
 * This function unpacks the result of a QMI_NAS_GET_NR5G_RRC_UTC_TIME request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_nr5g_rrc_utc_time
 */
QMI_SDK_error_t telit_nas_get_nr5g_rrc_utc_time_unpack(uint8_t *rsp, uint16_t len,
        nas_nr5g_rrc_utc_time_t *out);


/**
 * @brief input type for QMI_NAS_SWITCH_ANTENNA_LTE and QMI_NAS_SWITCH_ANTENNA_WCDMA
 *
 */
typedef struct {
    /** 0 for main and aux, 1 for main only, 2 for aux only */
    uint8_t antenna;
} nas_antenna_t;

/**
 * @brief Pack QMI_NAS_SWITCH_ANTENNA_LTE (21849 - 0x5559) request
 *
 * This function packs a qmi request for switching LTE antenna type
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #nas_antenna_t
 */
QMI_SDK_error_t telit_nas_switch_antenna_lte_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        nas_antenna_t *input);

/**
 * @brief Unpack QMI_NAS_SWITCH_ANTENNA_LTE (21849 - 0x5559) request
 *
 * This function unpacks the result of a QMI_NAS_SWITCH_ANTENNA_LTE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_nas_switch_antenna_lte_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_NAS_SWITCH_ANTENNA_WCDMA (21850 - 0x555a) request
 *
 * This function packs a qmi request for switching WCDMA antenna type
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #nas_antenna_t
 */
QMI_SDK_error_t telit_nas_switch_antenna_wcdma_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        nas_antenna_t *input);

/**
 * @brief Unpack QMI_NAS_SWITCH_ANTENNA_WCDMA (21850 - 0x555a) request
 *
 * This function unpacks the result of a QMI_NAS_SWITCH_ANTENNA_WCDMA request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_nas_switch_antenna_wcdma_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI NAS signal strength info type
 *
 */
typedef struct {
    /** Received signal strength in dBm, for UMTS is forward link pilot Ec, for GSM received signal strength, for LTE total received wideband power observed by the UE */
    int8_t strength;
    /** radio interface technology of the signal being measured, see #QMI_NAS_radio_interface_t */
    enum8_t radio_interface;
} nas_ind_event_report_signal_strength_t;

/**
 * @brief QMI NAS registration status info type
 *
 */
typedef struct {
    /** network service domain that was rejected, see #QMI_NAS_service_domain_t */
    enum8_t domain;
    /** reject cause, refer to 3GPP TS 24.008 sections 10.5.3.6 and 10.5.5.14, and 3GPP TS 24.301 section 9.9.3.9 */
    uint16_t reject_cause;
} nas_ind_event_report_registration_reject_t;

/**
 * @brief output type for QMI_NAS_EVENT_REPORT_IND
 *
 */
typedef struct {
    /** nas_ind_event_report_signal_strength_t instance */
    nas_ind_event_report_signal_strength_t signal_strength;
    /** nas_ind_event_report_registration_reject_t instance */
    nas_ind_event_report_registration_reject_t registration_reject;
    /** set if nas_ind_event_report_t#signal_strength is available */
    bool signal_strength_set;
    /** set if nas_ind_event_report_t#registration_reject is available */
    bool registration_reject_set;
} nas_ind_event_report_t;

/**
 * @brief output type for QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND (288 - 0x0120)
 *
 */
typedef struct {
    /** set if gps_time is available */
    bool gps_time_available;
    /** GPS Time. Indicates the GPS time at which the next pulse will be
     * generated */
    uint64_t gps_time;
    /** set if sfn is available */
    bool sfn_available;
    /** System Frame Number. Indicates the SFN at which the next pulse will be
     * generated. */
    uint32_t sfn;
    /** set if nta is available */
    bool nta_available;
    /** NTA. Indicates NTA value in Ts units. Ts = 1/(15,000*2048) seconds. */
    int32_t nta;
    /** set if nta_offset is available */
    bool nta_offset_available;
    /** NTA Offset. Indicates NTA offset value in Tc units. Tc = Ts/64. Ts =
     * 1/(15,000*2048) seconds. */
    uint32_t nta_offset;
    /** set if leapseconds is available */
    bool leapseconds_available;
    /** Leap Seconds. Indicates the number of leap seconds to get GPS time from
     * the UTC time. */
    uint8_t leapseconds;
    /** set if utc_time is available */
    bool utc_time_available;
    /** UTC Time. Indicates the UTC time at which the next pulse will be
     * generated. */
    uint64_t utc_time;
} nas_nr5g_time_sync_pulse_report_t;

typedef enum {
    /** Frame sync lost due to RLF */
    QMI_NAS_NR5G_LOST_FRAME_SYNC_RLF         = 0,
    /** Frame sync lost due to handover */
    QMI_NAS_NR5G_LOST_FRAME_SYNC_HANDOVER    = 1,
    /** Frame sync lost due to reselection */
    QMI_NAS_NR5G_LOST_FRAME_SYNC_RESELECTION = 2,
    /** Frame sync lost due to OOS */
    QMI_NAS_NR5G_LOST_FRAME_SYNC_OOS         = 3,
    /** Frame sync lost due to stale SIB 9 */
    QMI_NAS_NR5G_LOST_FRAME_SYNC_STALE_SIB9  = 4,
    /** Frame sync lost as SIB 9 is not available */
    QMI_NAS_NR5G_LOST_FRAME_SYNC_NO_SIB9     = 5
} QMI_NAS_nr5g_sync_lost_reason_t;

/**
 * @brief output type for QMI_NAS_NR5G_LOST_FRAME_SYNC_IND (289 - 0x0121)
 *
 */
typedef struct {
    /** set if nr5g_sync_lost_reason is available */
    bool nr5g_sync_lost_reason_available;
    /** Indicates the reason for which frame sync is lost on NR5G, see #QMI_NAS_nr5g_sync_lost_reason_t */
    enum32_t nr5g_sync_lost_reason;
} nas_nr5g_lost_frame_sync_t;

/**
 * @brief Unpack nas indications
 *
 * This function unpacks the result of the following nas indications
 *
 * QMI_NAS_EVENT_REPORT_IND (2)
 * QMI_NAS_GET_SERVING_SYSTEM (36)
 * QMI_NAS_NETWORK_TIME_IND (76)
 * QMI_NAS_SYS_INFO_IND (78)
 * QMI_NAS_SIG_INFO_IND (81)
 * QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND (288)
 * QMI_NAS_NR5G_LOST_FRAME_SYNC_IND (289)
 * QMI_NAS_NR5G_RRC_UTC_TIME_UPDATE_IND (335)
 *
 * @param[in] msg_id indication id
 * @param[in] rsp indication
 * @param[in] len indication length
 * @param[out] out type according to the following
 *      QMI_NAS_EVENT_REPORT_IND (2)                  -> #nas_ind_event_report_t
 *      QMI_NAS_GET_SERVING_SYSTEM (36)               -> #nas_serving_system_info_t
 *      QMI_NAS_NETWORK_TIME_IND (76)                 -> #nas_network_time_t
 *      QMI_NAS_SYS_INFO_IND (78)                     -> #nas_system_info_t
 *      QMI_NAS_SIG_INFO_IND (81)                     -> #nas_signal_info_t
 *      QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND (288) -> #nas_nr5g_time_sync_pulse_report_t
 *      QMI_NAS_NR5G_LOST_FRAME_SYNC_IND (289)        -> #nas_nr5g_lost_frame_sync_t
 *      QMI_NAS_NR5G_RRC_UTC_TIME_UPDATE_IND (335)    -> #nas_nr5g_rrc_utc_time_t
 * @param[in] out_len buffer size
 */
QMI_SDK_error_t telit_nas_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len,
        uint8_t *out, uint16_t out_len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_NAS_H_ */
