#ifndef TELIT_SDK_WDS_H_
#define TELIT_SDK_WDS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_wds.h
 * @brief WDS pack/unpack functions
 * Service ID: QMI_SVC_WDS (1)
 */

/**
 * @brief Enum to describe QMI WDS Message IDs
 */
typedef enum {
    /** 01 - 0x0001 Enable events report */
    QMI_WDS_SET_EVENT_REPORT          = 0x0001,
    /** 01 - 0x0001 Events report indication */
    QMI_WDS_EVENT_REPORT_IND          = 0x0001,

    /** 03 - 0x0003 Sets the registration state for different indications */
    QMI_WDS_INDICATION_REGISTER       = 0x0003,

    /** 32 - 0x0020 Start data connection */
    QMI_WDS_START_NETWORK_INTERFACE   = 0x0020,
    /** 33 - 0x0021 Stop data connection */
    QMI_WDS_STOP_NETWORK_INTERFACE    = 0x0021,
    /** 34 - 0x0022 Get data connection status */
    QMI_WDS_GET_PKT_SRVC_STATUS       = 0x0022,
    /** 34 - 0x0022 Data connection status indication */
    QMI_WDS_PKT_SRVC_STATUS_IND       = 0x0022,

    /** 36 - 0x0024 Get data connection statistics */
    QMI_WDS_GET_PKT_STATISTICS        = 0x0024,

    /** 39 - 0x0027 Create a data connection profile */
    QMI_WDS_CREATE_PROFILE            = 0x0027,
    /** 40 - 0x0028 Modify a data connection profile */
    QMI_WDS_MODIFY_PROFILE_SETTINGS   = 0x0028,
    /** 41 - 0x0029 Delete a profile */
    QMI_WDS_DELETE_PROFILE            = 0x0029,
    /** 42 - 0x002A Retrieve the configured profiles list */
    QMI_WDS_GET_PROFILE_LIST          = 0x002A,
    /** 43 - 0x002B Get profile' setting */
    QMI_WDS_GET_PROFILE_SETTINGS      = 0x002B,

    /** 45 - 0x002D Get the runtime data session settings */
    QMI_WDS_GET_RUNTIME_SETTINGS      = 0x002D,

    /** 53 - 0x0035 Get the duration of data session */
    QMI_WDS_GET_CALL_DURATION         = 0x0035,

    /** 73 - 0x0049 Retrieve the default profile number */
    QMI_WDS_GET_DEFAULT_PROFILE_NUM   = 0x0049,

    /** 77 - 0x004D Set the client IP family preference */
    QMI_WDS_SET_CLIENT_IP_FAMILY_PREF = 0x004D,

    /** 147 - 0x0093 Set the LTE attach PDN list */
    QMI_WDS_SET_LTE_ATTACH_PDN_LIST   = 0x0093,
    /** 148 - 0x0094 Get the LTE attach PDN list */
    QMI_WDS_GET_LTE_ATTACH_PDN_LIST   = 0x0094,

    /** 162 - 0x00A2 Bind mux data port */
    QMI_WDS_BIND_MUX_DATA_PORT        = 0x00A2,

    /** 185 - 0x00B9 Indicates an APN change during a data call */
    QMI_WDS_APN_PARAM_INFO_CHANGE_IND = 0x00B9,
} QMI_WDS_message_t;

/**
 * @brief QMI WDS profile extended errors
 *
 */
typedef enum {
    /** General failure */
    QMI_WDS_DS_PROFILE_REG_RESULT_FAIL                   = 1,
    /** Request contains an invalid profile handle */
    QMI_WDS_DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL         = 2,
    /** Invalid operation was requested */
    QMI_WDS_DS_PROFILE_REG_RESULT_ERR_INVAL_OP           = 3,
    /** Request contains an invalid technology type */
    QMI_WDS_DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE = 4,
    /** Request contains an invalid profile number */
    QMI_WDS_DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  = 5,
    /** Request contains an invalid profile identifier */
    QMI_WDS_DS_PROFILE_REG_RESULT_ERR_INVAL_IDENT        = 6,
    /** Request contains an invalid argument other than profile number and profile identifier */
    QMI_WDS_DS_PROFILE_REG_RESULT_ERR_INVAL              = 7,
    /** Profile registry has not been initialized yet */
    QMI_WDS_DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED     = 8,
    /** Request contains a parameter with invalid length */
    QMI_WDS_DS_PROFILE_REG_RESULT_ERR_LEN_INVALID        = 9,
    /** End of the profile lsit was reached while searching for the requested profile */
    QMI_WDS_DS_PROFILE_REG_RESULT_LIST_END               = 10,
    /** Request contains an invalid subscription identifier */
    QMI_WDS_DS_PROFILE_REG_RESULT_ERR_INVAL_SUBS_ID      = 11,
    /** Request contains an invalid profile family */
    QMI_WDS_DS_PROFILE_REG_INVAL_PROFILE_FAMILY          = 12,
    /** Version mismatch */
    QMI_WDS_DS_PROFILE_REG_PROFILE_VERSION_MISMATCH      = 13,
    /** Out of memory */
    QMI_WDS_REG_RESULT_ERR_OUT_OF_MEMORY                 = 14,
    /** File access error */
    QMI_WDS_DS_PROFILE_REG_RESULT_ERR_FILE_ACCESS        = 15,
    /** End of field */
    QMI_WDS_DS_PROFILE_REG_RESULT_ERR_EOF                = 16,
    /** A valid flag is not set */
    QMI_WDS_REG_RESULT_ERR_VALID_FLAG_NOT_SET            = 17,
    /** Out of profiles */
    QMI_WDS_REG_RESULT_ERR_OUT_OF_PROFILES               = 18,
    /** No emergency PDN support */
    QMI_WDS_REG_RESULT_NO_EMERGENCY_PDN_SUPPORT          = 19,
    /** Request contains an invalid 3GPP profile family */
    QMI_WDS_DS_PROFILE_REG_3GPP_INVAL_PROFILE_FAMILY     = 4097,
    /** Error was encountered while accessing the 3GPP profiles */
    QMI_WDS_DS_PROFILE_REG_3GPP_ACCESS_ERR               = 4098,
    /** Specified 3GPP profile does not have a valid context */
    QMI_WDS_DS_PROFILE_REG_3GPP_CONTEXT_NOT_DEFINED      = 4099,
    /** Specified 3GPP profile is marked invalid */
    QMI_WDS_DS_PROFILE_REG_3GPP_VALID_FLAG_NOT_SET       = 4100,
    /** Specified 3GPP profile is marked read-only */
    QMI_WDS_DS_PROFILE_REG_3GPP_READ_ONLY_FLAG_SET       = 4101,
    /** Creation of a new 3GPP profile failed because of the limit of 16 profiles has already been reached */
    QMI_WDS_DS_PROFILE_REG_3GPP_ERR_OUT_OF_PROFILES      = 4102,
} QMI_WDS_DS_profile_ext_error_t;

/**
 * @brief QMI WDS generic extended error type
 *
 */
typedef struct {
    /** error codes from the data session profile, see #QMI_WDS_DS_profile_ext_error_t */
    enum16_t extended_error_code;
} wds_profile_ext_err_t;

/**
 * @brief QMI WDStransfer statistics mask
 *
 */
typedef enum {
    /** Tx packets OK */
    QMI_WDS_STATS_TX_PACKETS_OK      = (1 << 0),
    /** Rx packets OK */
    QMI_WDS_STATS_RX_PACKETS_OK      = (1 << 1),
    /** Tx packets errors */
    QMI_WDS_STATS_TX_PACKETS_ERROR   = (1 << 2),
    /** Rx packets errors */
    QMI_WDS_STATS_RX_PACKETS_ERROR   = (1 << 3),
    /** Tx overflows */
    QMI_WDS_STATS_TX_OVERFLOWS       = (1 << 4),
    /** Rx overflows */
    QMI_WDS_STATS_RX_OVERFLOWS       = (1 << 5),
    /** Tx bytes OK */
    QMI_WDS_STATS_TX_BYTES_OK        = (1 << 6),
    /** Rx bytes OK */
    QMI_WDS_STATS_RX_BYTES_OK        = (1 << 7),
    /** Tx packets dropped */
    QMI_WDS_STATS_TX_PACKETS_DROPPED = (1 << 8),
    /** Rx packets dropped */
    QMI_WDS_STATS_RX_PACKETS_DROPPED = (1 << 9),
} QMI_WDS_stats_mask_t;

/**
 * @brief QMI WDS transfer statistic indicator type
 *
 */
typedef struct {
    /** Period between transfer statistics reports, according to the following values:
       - 0 - Do not report
       - Other - Period between reports (in seconds)
     */
    uint8_t stats_period;
    /** Packet statistics mask, see #QMI_WDS_stats_mask_t */
    uint32_t stats_mask;
} wds_set_event_report_transfer_stats_t;

/**
 * @brief input type for QMI_WDS_SET_EVENT_REPORT
 *
 */
typedef struct {
    /** Action for data bearer technology indicator event, see #QMI_indication_action_t */
    QMI_indication_action_t report_data_bearer_tech;
    /** Action for dormancy status indicator event, see #QMI_indication_action_t */
    QMI_indication_action_t report_dormancy_status;
    /** Action for MIP status indicator event, see #QMI_indication_action_t */
    QMI_indication_action_t report_mip_status;
    /** Action for current data bearer technology indicator event, see #QMI_indication_action_t */
    QMI_indication_action_t report_current_data_bearer_tech;
    /** set if transfer_stats should be considered */
    bool transfer_stats_available;
    /** #wds_set_event_report_transfer_stats_t instance */
    wds_set_event_report_transfer_stats_t transfer_stats;
    /** Action for data system status change indicator event, see #QMI_indication_action_t */
    QMI_indication_action_t report_data_system_status;
    /** Action for data bearer technology extended indicator event, see #QMI_indication_action_t */
    QMI_indication_action_t report_data_bearer_tech_ext;
    /** Action for data call status change indicator event, see #QMI_indication_action_t */
    QMI_indication_action_t report_data_call_status_change;
} wds_set_event_report_info_t;

/**
 * @brief Pack QMI_WDS_SET_EVENT_REPORT (1 - 0x0001) request
 *
 * This function packs a qmi request for setting the wireless data
 * connection state reporting conditions for the requesting control point.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_set_event_report_info_t
 */
QMI_SDK_error_t telit_wds_set_event_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        wds_set_event_report_info_t *input);

/**
 * @brief Unpack QMI_WDS_SET_EVENT_REPORT (1 - 0x0001) request
 *
 * This function unpacks the result of a QMI_WDS_SET_EVENT_REPORT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_wds_set_event_report_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_WDS_INDICATION_REGISTER
 *
 */
typedef struct {
    /** Action for APN Parameter Change Information, see #QMI_indication_action_t */
    QMI_indication_action_t report_apn_param_change_info;
} wds_indication_register_info_t;

/**
 * @brief Pack QMI_WDS_INDICATION_REGISTER (3 - 0x0003) request
 *
 * This function packs a qmi request for setting the registration state
 * for different QMI_WDS indications for the requesting control point.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] in request buffer size, out request len
 * @param[in] input #wds_indication_register_t
 */
QMI_SDK_error_t telit_wds_indication_register_pack(pack_qmi_t *req_ctx,
        void *req, uint16_t *len,
        wds_indication_register_info_t *input);

/**
 * @brief Unpack QMI_WDS_INDICATION_REGISTER (3 - 0x0003) request
 *
 * This function unpacks the result of a QMI_WDS_INDICATION_REGISTER request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_wds_indication_register_unpack(uint8_t *rsp, uint16_t len);


/**
 * @brief QMI WDS profile extended errors
 *
 */
typedef enum {
    /** Unspecified */
    QMI_WDS_VCER_TYPE_UNSPECIFIED          = 0x00,
    /** Mobile IP */
    QMI_WDS_VCER_TYPE_MOBILE_IP            = 0x01,
    /** Internal */
    QMI_WDS_VCER_TYPE_INTERNAL             = 0x02,
    /** Call manager defined */
    QMI_WDS_VCER_TYPE_CALL_MANAGER_DEFINED = 0x03,
    /** 3gpp specification defined */
    QMI_WDS_VCER_TYPE_3GPP_SPEC_DEFINED    = 0x06,
    /** ppp */
    QMI_WDS_VCER_TYPE_PPP                  = 0x07,
    /** EHRPD */
    QMI_WDS_VCER_TYPE_EHRPD                = 0x08,
    /** IPv6 */
    QMI_WDS_VCER_TYPE_IPV6                 = 0x09,
    /** Handoff */
    QMI_WDS_VCER_TYPE_HANDOFF              = 0x0c,
} QMI_WDS_call_end_reason_type_t;

/**
 * @brief QMI WDS call end reasons
 *
 */
typedef enum {
    /** Reason unspecified */
    QMI_WDS_CALL_END_REASON_UNSPECIFIED                         = 1,
    /** Client ended the call */
    QMI_WDS_CALL_END_REASON_CLIENT_END                          = 2,
    /** Phone has no service */
    QMI_WDS_CALL_END_REASON_NO_SRV                              = 3,
    /** Call ended abnormally */
    QMI_WDS_CALL_END_REASON_FADE                                = 4,
    /** Received release from BS; no reason given */
    QMI_WDS_CALL_END_REASON_REL_NORMAL                          = 5,
    /** Access attempt already in progress; SD2.0 only */
    QMI_WDS_CALL_END_REASON_ACC_IN_PROG                         = 6,
    /** Access failure for reason other than the above */
    QMI_WDS_CALL_END_REASON_ACC_FAIL                            = 7,
    /** Call rejected because of redirection or handoff */
    QMI_WDS_CALL_END_REASON_REDIR_OR_HANDOFF                    = 8,
    /** Call failed because close is in progress */
    QMI_WDS_CALL_END_REASON_CLOSE_IN_PROGRESS                   = 9,
    /** Authentication failed */
    QMI_WDS_CALL_END_REASON_AUTH_FAILED                         = 10,
    /** Call ended because of an internal error */
    QMI_WDS_CALL_END_REASON_INTERNAL_CALL_END                   = 11,

    /** Call origination request failed (WCDMA/GSM only) */
    QMI_WDS_CALL_END_REASON_CONF_FAILED                         = 1000,
    /** Client rejected the incoming call (WCDMA/GSM only) */
    QMI_WDS_CALL_END_REASON_INCOM_REJ                           = 1001,
    /** Phone has no service (WCDMA/GSM only) */
    QMI_WDS_CALL_END_REASON_NO_GW_SRV                           = 1002,
    /** Network ended the call, look in cc_cause (WCDMA/GSM only) */
    QMI_WDS_CALL_END_REASON_NETWORK_END                         = 1003,
    /** LLC or SNDCP failure */
    QMI_WDS_CALL_END_REASON_LLC_SNDCP_FAILURE                   = 1004,
    /** Insufficient resources */
    QMI_WDS_CALL_END_REASON_INSUFFICIENT_RESOURCES              = 1005,
    /** Service option temporarily out of order */
    QMI_WDS_CALL_END_REASON_OPTION_TEMP_OOO                     = 1006,
    /** NSAPI already used */
    QMI_WDS_CALL_END_REASON_NSAPI_ALREADY_USED                  = 1007,
    /** Regular PDP context deactivation */
    QMI_WDS_CALL_END_REASON_REGULAR_DEACTIVATION                = 1008,
    /** Network failure */
    QMI_WDS_CALL_END_REASON_NETWORK_FAILURE                     = 1009,
    /** Reactivation requested */
    QMI_WDS_CALL_END_REASON_UMTS_REATTACH_REQ                   = 1010,
    /** Protocol error, unspecified */
    QMI_WDS_CALL_END_REASON_PROTOCOL_ERROR                      = 1011,
    /** Operator-determined barring */
    QMI_WDS_CALL_END_REASON_OPERATOR_DETERMINED_BARRING         = 1012,
    /** Unknown or missing access point name */
    QMI_WDS_CALL_END_REASON_UNKNOWN_APN                         = 1013,
    /** Unknown PDP address or PDP type */
    QMI_WDS_CALL_END_REASON_UNKNOWN_PDP                         = 1014,
    /** Activation rejected by GGSN */
    QMI_WDS_CALL_END_REASON_GGSN_REJECT                         = 1015,
    /** Activation rejected, unspecified */
    QMI_WDS_CALL_END_REASON_ACTIVATION_REJECT                   = 1016,
    /** Service option not supported */
    QMI_WDS_CALL_END_REASON_OPTION_NOT_SUPPORTED                = 1017,
    /** Requested service option not subscribed */
    QMI_WDS_CALL_END_REASON_OPTION_UNSUBSCRIBED                 = 1018,
    /** QoS not accepted */
    QMI_WDS_CALL_END_REASON_QOS_NOT_ACCEPTED                    = 1019,
    /** Semantic errod in the TFT operation */
    QMI_WDS_CALL_END_REASON_TFT_SEMANTIC_ERROR                  = 1020,
    /** Syntactical error in the TFT operation */
    QMI_WDS_CALL_END_REASON_TFT_SYNTAX_ERROR                    = 1021,
    /** Unknown PDP context */
    QMI_WDS_CALL_END_REASON_UNKNOWN_PDP_CONTEXT                 = 1022,
    /** Semantic errors in packet filters */
    QMI_WDS_CALL_END_REASON_FILTER_SEMANTIC_ERROR               = 1023,
    /** Syntactical error in packet filters */
    QMI_WDS_CALL_END_REASON_FILTER_SYNTAX_ERROR                 = 1024,
    /** PDP context without TFT already activated */
    QMI_WDS_CALL_END_REASON_PDP_WITHOUT_ACTIVE_TFT              = 1025,
    /** Invalid transaction identifier value */
    QMI_WDS_CALL_END_REASON_INVALID_TRANSACTION_ID              = 1026,
    /** Semantically incorrect message */
    QMI_WDS_CALL_END_REASON_MESSAGE_INCORRECT_SEMANTIC          = 1027,
    /** Invalid mandatory information */
    QMI_WDS_CALL_END_REASON_INVALID_MANDATORY_INFO              = 1028,
    /** Message type nonexistent or not implemented */
    QMI_WDS_CALL_END_REASON_MESSAGE_TYPE_UNSUPPORTED            = 1029,
    /** Message not compatible with state */
    QMI_WDS_CALL_END_REASON_MSG_TYPE_NONCOMPATIBLE_STATE        = 1030,
    /** Information element nonexistent or not implemented */
    QMI_WDS_CALL_END_REASON_UNKNOWN_INFO_ELEMENT                = 1031,
    /** Conditional IE error */
    QMI_WDS_CALL_END_REASON_CONDITIONAL_IE_ERROR                = 1032,
    /** Message not compatible with protocol state */
    QMI_WDS_CALL_END_REASON_MSG_AND_PROTOCOL_STATE_UNCOMPATIBLE = 1033,
    /** APN restriction value incompatible with active PDP context */
    QMI_WDS_CALL_END_REASON_APN_TYPE_CONFLICT                   = 1034,
    /** No GPRS context present */
    QMI_WDS_CALL_END_REASON_NO_GPRS_CONTEXT                     = 1035,
    /** Requested feature not supported */
    QMI_WDS_CALL_END_REASON_FEATURE_NOT_SUPPORTED               = 1036,
} QMI_WDS_call_end_reason_t;

/**
 * @brief QMI WDS verbose call end reason type
 *
 */
typedef struct {
    /** call end reason type, see #QMI_WDS_call_end_reason_type_t */
    uint16_t call_end_reason_type;
    /** reason the call ended, see Qualcomm document Appendix B */
    uint16_t call_end_reason;
} wds_verbose_call_end_reason_t;

/**
 * @brief output type for QMI_WDS_START_NETWORK_INTERFACE
 *
 */
typedef struct {
    /** set if pkt_data_handle is available */
    bool pkt_data_handle_available;
    /** packet data handle, identifying the call instance providing packet service */
    uint32_t pkt_data_handle;
    /** set if call_end_reason is available */
    bool call_end_reason_available;
    /** reason the call ended, see #QMI_WDS_call_end_reason_t */
    enum16_t call_end_reason;
    /** set if verbose_call_end_reason is available */
    bool verbose_call_end_reason_available;
    /** verbose call end reason */
    wds_verbose_call_end_reason_t verbose_call_end_reason;
    bool mux_id_available; /** set if mux_id is available */
    uint8_t mux_id;        /** mux id of the rmnet instance where a data call is already present */
} wds_start_network_resp_t;

/**
 * @brief QMI WDS profile types
 *
 */
typedef enum {
    QMI_WDS_PROFILE_TYPE_3GPP  = 0,
    QMI_WDS_PROFILE_TYPE_3GPP2 = 1,
} QMI_WDS_profile_type_t;

/**
 * @brief QMI WDS profile id information type
 *
 */
typedef struct {
    /** technology preference, see #QMI_WDS_profile_type_t */
    enum8_t type;
    /** index of the configured profile on which data call parameters
     * are based, if index is 0, the TLV may be missing */
    uint8_t index;
} wds_settings_profile_id_t;

/**
 * @brief input type for QMI_WDS_START_NETWORK_INTERFACE
 *
 */
typedef struct {
    /** set if technology_preference should be considered */
    bool technology_preference_available;
    /** technology preference, see #QMI_WDS_profile_type_t */
    enum8_t technology_preference;
    /** set if profile_index_3gpp should be considered */
    bool profile_index_3gpp_available;
    /** index of the 3gpp configured profile on which data call parameters are based */
    uint8_t profile_index_3gpp;
    /** set if authentication_pref should be considered */
    bool authentication_pref_available;
    /** bitmap indicating the algorithm preference, according to the following values:
          Bit 0 - PAP preference:
       - 0 - PAP is never performed
       - 1 - PAP may be performed
          Bit 1 - CHAP preference:
       - 0 - CHAP is never performed
       - 1 - CHAP may be performed
          If more than one bit is set, the device decides which authentication procedure is performed while setting up the data session.
     */
    uint8_t authentication_pref;
    /** username string length */
    uint8_t username_len;
    /** username to use during data network authentication */
    char username[MAX_STRING_SIZE];
    /** password string length */
    uint8_t password_len;
    /** password to use during data network authentication */
    char password[MAX_STRING_SIZE];
    /** apn string length */
    uint8_t apn_len;
    /** APN to be used if missing in the indicated profile */
    char apn[MAX_STRING_SIZE];
    /** set if ip_family_preference should be considered */
    bool ip_family_preference_available;
    /** IP family preference, see #QMI_ip_family_t */
    enum8_t ip_family_preference;
    /** set if disallow_in_roaming should be considered */
    bool disallow_in_roaming_available;
    /** Manage roaming according to the following values:
     * - 0, allow the call if the device is in roaming
     * - 1, disallow the call if the device is in roaming
     */
    uint8_t disallow_in_roaming;
    /** set if bring_up_by_apn_name should be considered */
    bool bring_up_by_apn_name_available;
    /** To be used with FN980: 1 if device should attempt to bring up a call with the APN name, 0 otherwise */
    uint8_t bring_up_by_apn_name;
} wds_start_network_info_t;

/**
 * @brief Pack QMI_WDS_START_NETWORK_INTERFACE (32 - 0x0020) request
 *
 * This function packs a qmi request for activating a packet data
 * session (if not already started) on behalf of the requesting
 * control point
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_start_network_info_t
 */
QMI_SDK_error_t telit_wds_start_network_interface_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_start_network_info_t *input);

/**
 * @brief Unpack QMI_WDS_START_NETWORK_INTERFACE (32 - 0x0020) request
 *
 * This function unpacks the result of a QMI_WDS_START_NETWORK_INTERFACE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wds_start_network_resp_t struct
 */
QMI_SDK_error_t telit_wds_start_network_interface_unpack(uint8_t *rsp, uint16_t len,
        wds_start_network_resp_t *out);

/**
 * @brief input type for QMI_WDS_STOP_NETWORK_INTERFACE
 *
 */
typedef struct {
    /** packet data handle, identifying the call instance providing packet service */
    uint32_t pkt_data_handle;
} wds_stop_network_info_t;

/**
 * @brief Pack QMI_WDS_STOP_NETWORK_INTERFACE (33 - 0x0021) request
 *
 * This function packs a qmi request for deactivating a packet data
 * session (unless in use by other control point) on behalf of the
 * requesting control point
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_stop_network_info_t
 */
QMI_SDK_error_t telit_wds_stop_network_interface_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_stop_network_info_t *input);

/**
 * @brief Unpack QMI_WDS_STOP_NETWORK_INTERFACE (33 - 0x0021) request
 *
 * This function unpacks the result of a QMI_WDS_STOP_NETWORK_INTERFACE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_wds_stop_network_interface_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI WDS data connection status
 *
 */
typedef enum {
    /** Disconnected */
    QMI_WDS_CONNECTION_STATUS_DISCONNECTED   = 0x01,
    /** Connected */
    QMI_WDS_CONNECTION_STATUS_CONNECTED      = 0x02,
    /** Suspended */
    QMI_WDS_CONNECTION_STATUS_SUSPENDED      = 0x03,
    /** Authenticating */
    QMI_WDS_CONNECTION_STATUS_AUTHENTICATING = 0x04,
} QMI_WDS_data_connection_status_t;

/**
 * @brief output type for QMI_WDS_GET_PKT_SRVC_STATUS
 *
 */
typedef struct {
    /** connection status, see #QMI_WDS_data_connection_status_t */
    enum8_t conn_status;
} wds_get_packet_service_status_t;

/**
 * @brief Pack QMI_WDS_GET_PKT_SRVC_STATUS (34 - 0x0022) request
 *
 * This function packs a qmi request for querying the current packet
 * data connection status
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_wds_get_pkt_srvc_status_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_WDS_GET_PKT_SRVC_STATUS (34 - 0x0022) request
 *
 * This function unpacks the result of a QMI_WDS_GET_PKT_STATUS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wds_get_packet_service_status_t
 */
QMI_SDK_error_t telit_wds_get_pkt_srvc_status_unpack(uint8_t *rsp, uint16_t len,
        wds_get_packet_service_status_t *out);

/**
 * @brief QMI WDS packet statistics mask
 *
 */
typedef struct {
    /** Packet statistics mask, see #QMI_WDS_stats_mask_t */
    uint32_t stats_mask;
} wds_get_packet_statistics_mask_t;

/**
 * @brief output type for QMI_WDS_GET_PKT_STATISTICS
 *
 */
typedef struct {
    /** Set if tx_out_count is available */
    bool tx_out_count_available;
    /** number of packets transmitted without errors */
    uint32_t tx_out_count;
    /** Set if rx_out_count is available */
    bool rx_out_count_available;
    /** number of packets received without errors */
    uint32_t rx_out_count;
    /** Set if tx_err_count is available */
    bool tx_err_count_available;
    /** number of outgoing packets with framing errors */
    uint32_t tx_err_count;
    /** Set if rx_err_count is available */
    bool rx_err_count_available;
    /** number of incoming packets with framing errors */
    uint32_t rx_err_count;
    /** Set if tx_ofl_count is available */
    bool tx_ofl_count_available;
    /** number of packets droppedbecause tx buffer overflowed */
    uint32_t tx_ofl_count;
    /** Set if rx_ofl_count is available */
    bool rx_ofl_count_available;
    /** number of packets droppedbecause rx buffer overflowed */
    uint32_t rx_ofl_count;
    /** Set if tx_ok_bytes_count is available */
    bool tx_ok_bytes_count_available;
    /** number of bytes transmitted without errors */
    uint64_t tx_ok_bytes_count;
    /** Set if rx_ok_bytes_count is available */
    bool rx_ok_bytes_count_available;
    /** number of bytes received without errors */
    uint64_t rx_ok_bytes_count;
    /** Set if last_call_tx_ok_bytes_count is available */
    bool last_call_tx_ok_bytes_count_available;
    /** number of bytes transmitted without errors during the last data call (0 if no call was made earlier) */
    uint64_t last_call_tx_ok_bytes_count;
    /** Set if last_call_rx_ok_bytes_count is available */
    bool last_call_rx_ok_bytes_count_available;
    /** number of bytes received without errors during the last data call (0 if no call was made earlier) */
    uint64_t last_call_rx_ok_bytes_count;
    /** Set if tx_dropped_count is available */
    bool tx_dropped_count_available;
    /** number of outgoing packets dropped */
    uint32_t tx_dropped_count;
    /** Set if rx_dropped_count is available */
    bool rx_dropped_count_available;
    /** number of incoming packets dropped */
    uint32_t rx_dropped_count;
} wds_get_packet_statistics_t;

/**
 * @brief Pack QMI_WDS_GET_PKT_STATISTICS (36 - 0x0024) request
 *
 * This function packs a qmi request for retrieving the packet
 * data transfer statistics from the start of the current data
 * session
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_get_packet_statistics_mask_t
 */
QMI_SDK_error_t telit_wds_get_pkt_statistics_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_get_packet_statistics_mask_t *input);

/**
 * @brief Unpack QMI_WDS_GET_PKT_STATISTICS (36 - 0x0024) request
 *
 * This function unpacks the result of a QMI_WDS_GET_PKT_STATISTICS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wds_get_packet_statistics_t
 */
QMI_SDK_error_t telit_wds_get_pkt_statistics_unpack(uint8_t *rsp, uint16_t len,
        wds_get_packet_statistics_t *out);

/**
 * @brief QMI WDS PDP Types
 *
 */
typedef enum {
    QMI_WDS_PDP_TYPE_IPV4   = 0x00,
    QMI_WDS_PDP_TYPE_PPP    = 0x01,
    QMI_WDS_PDP_TYPE_IPV6   = 0x02,
    QMI_WDS_PDP_TYPE_IPV4V6 = 0x03,
    QMI_WDS_PDP_TYPE_NON_IP = 0x04,
} QMI_WDS_PDP_type_t;

/**
 * @brief QMI WDS PDP Header Compression Types
 *
 */
typedef enum {
    QMI_WDS_PDP_HDR_COMPR_TYPE_OFF               = 0x00,
    QMI_WDS_PDP_HDR_COMPR_TYPE_MANUFACTURER_PREF = 0x01,
    QMI_WDS_PDP_HDR_COMPR_TYPE_RFC_1144          = 0x02,
    QMI_WDS_PDP_HDR_COMPR_TYPE_RFC_2507          = 0x03,
    QMI_WDS_PDP_HDR_COMPR_TYPE_RFC_3095          = 0x04,
} QMI_WDS_PDP_header_compression_type_t;

/**
 * @brief QMI WDS PDP Data Compression Types
 *
 */
typedef enum {
    QMI_WDS_PDP_DATA_COMPR_TYPE_OFF               = 0x00,
    QMI_WDS_PDP_DATA_COMPR_TYPE_MANUFACTURER_PREF = 0x01,
    QMI_WDS_PDP_DATA_COMPR_TYPE_V42BIS            = 0x02,
    QMI_WDS_PDP_DATA_COMPR_TYPE_V44               = 0x03,
} QMI_WDS_PDP_data_compression_type_t;

/**
 * @brief QMI WDS PDP access control values
 *
 */
typedef enum {
    QMI_WDS_PDP_ACCESS_CONTROL_NONE       = 0x00,
    QMI_WDS_PDP_ACCESS_CONTROL_REJECT     = 0x01,
    QMI_WDS_PDP_ACCESS_CONTROL_PERMISSION = 0x02,
} QMI_WDS_PDP_access_control_t;

/**
 * @brief QMI WDS address allocation preferences
 *
 */
typedef enum {
    QMI_WDS_ADDR_ALLOC_PREF_NAS_SIGNALING = 0,
    QMI_WDS_ADDR_ALLOC_PREF_DHCP          = 1,
} QMI_WDS_addr_alloc_pref_t;

#define IPV6_ADDR_MAX_SIZE 16

/**
 * @brief QMI WDS address traffic classes
 *
 */
typedef enum {
    QMI_WDS_TRAFFIC_CLASS_SUBSCRIBED     = 0x00,
    QMI_WDS_TRAFFIC_CLASS_CONVERSATIONAL = 0x01,
    QMI_WDS_TRAFFIC_CLASS_STREAMING      = 0x02,
    QMI_WDS_TRAFFIC_CLASS_INTERACTIVE    = 0x03,
    QMI_WDS_TRAFFIC_CLASS_BACKGROUND     = 0x04,
} QMI_WDS_traffic_class_t;

/**
 * @brief QMI WDS QOS delivery order
 *
 */
typedef enum {
    QMI_WDS_QOS_DELIVERY_ORDER_SUBSCRIBE = 0x00,
    QMI_WDS_QOS_DELIVERY_ORDER_ON        = 0x01,
    QMI_WDS_QOS_DELIVERY_ORDER_OFF       = 0x02,
} QMI_WDS_QOS_delivery_order_t;

/**
 * @brief QOS delivery of erroneous SDUs
 *
 */
typedef enum {
    QMI_WDS_QOS_DELIVERY_ERRONEOUS_SDUS_SUBSCRIBE    = 0x00,
    QMI_WDS_QOS_DELIVERY_ERRONEOUS_SDUS_NO_DETECTION = 0x01,
    QMI_WDS_QOS_DELIVERY_ERRONEOUS_SDUS_YES          = 0x02,
    QMI_WDS_QOS_DELIVERY_ERRONEOUS_SDUS_NO           = 0x03,
} QMI_WDS_QOS_delivery_erroneous_SDU_t;

#define QMI_WDS_MAX_STRING_SIZE_PROFILE 64

/**
 * @brief QMI WDS SDU error ratio values
 *
 */
typedef enum {
    /** Subscribe */
    QMI_WDS_SDU_ERROR_RATIO_SUBSCRIBE = 0,
    /** 1 * 10^2 */
    QMI_WDS_SDU_ERROR_RATIO_1         = 1,
    /** 7 * 10^3 */
    QMI_WDS_SDU_ERROR_RATIO_2         = 2,
    /** 1 * 10^3 */
    QMI_WDS_SDU_ERROR_RATIO_3         = 3,
    /** 1 * 10^4 */
    QMI_WDS_SDU_ERROR_RATIO_4         = 4,
    /** 1 * 10^5 */
    QMI_WDS_SDU_ERROR_RATIO_5         = 5,
    /** 1 * 10^6 */
    QMI_WDS_SDU_ERROR_RATIO_6         = 6,
    /** 1 * 10^1 */
    QMI_WDS_SDU_ERROR_RATIO_7         = 7,
} QMI_WDS_SDU_error_ratio_t;

/**
 * @brief QMI WDS residual BER values
 *
 */
typedef enum {
    /** Subscribe */
    QMI_WDS_RESIDUAL_BER_SUBSCRIBE = 0,
    /** 5 * 10^2 */
    QMI_WDS_RESIDUAL_BER_1         = 1,
    /** 1 * 10^2 */
    QMI_WDS_RESIDUAL_BER_2         = 2,
    /** 5 * 10^3 */
    QMI_WDS_RESIDUAL_BER_3         = 3,
    /** 4 * 10^3 */
    QMI_WDS_RESIDUAL_BER_4         = 4,
    /** 1 * 10^3 */
    QMI_WDS_RESIDUAL_BER_5         = 5,
    /** 1 * 10^4 */
    QMI_WDS_RESIDUAL_BER_6         = 6,
    /** 1 * 10^5 */
    QMI_WDS_RESIDUAL_BER_7         = 7,
    /** 1 * 10^6 */
    QMI_WDS_RESIDUAL_BER_8         = 8,
    /** 6 * 10^8 */
    QMI_WDS_RESIDUAL_BER_9         = 9,
} QMI_WDS_residual_BER_t;

/**
 * @brief QMI WDS UMTS qos type
 *
 */
typedef struct {
    /** Traffic class, see #QMI_WDS_traffic_class_t */
    enum8_t traffic_class;
    /** Maximum uplink bitrate in bits per second */
    uint32_t max_uplink_bitrate;
    /** Maximum downlink bitrate in bits per second */
    uint32_t max_downlink_bitrate;
    /** Guaranteed uplink bitrate in bits per second */
    uint32_t guaranteed_uplink_bitrate;
    /** Guaranteed downlink bitrate in bits per second */
    uint32_t guaranteed_downlink_bitrate;
    /** QOS delivery order, see #QMI_WDS_QOS_delivery_order_t */
    enum8_t qos_delivery_order;
    /** Maximum SDU size */
    uint32_t max_sdu_size;
    /** Target value for the fraction of SDUs lost or detected as erroneous, see #QMI_WDS_SDU_error_ratio_t */
    uint8_t sdu_error_ratio;
    /** Target value for the undetected bit error ratio in the delivered SDUs, see #QMI_WDS_residual_BER_t */
    uint8_t residual_bit_error_ratio;
    /** Delivery of erroneous SDUs, see #QMI_WDS_QOS_delivery_erroneous_SDU_t */
    enum8_t delivery_erroneous_SDUs;
    /** Indicates the targeted time between a request to transfer an SDU at one SAP to its delivery at the other SAP, in milliseconds. If the parameter is 0 the subscribed value is requested */
    uint32_t transfer_delay;
    /** Specifies the relative importance for handling of SDUs that belong to the UMTS bearer, compared to the SDUs of other bearers. If the parameter is 0 the subscribed value is requested */
    uint32_t traffic_handling_priority;
    /** Signaling indication flag, according to the following values:
       - 0 - signaling indication off
       - 1 - signaling indication on
          Considered only for UMTS QoS with signaling indication flag
     */
    bool sig_ind;
} wds_umts_qos_t;

/**
 * @brief QMI WDS GPRS QOS type
 *
 */
typedef struct {
    /** Precedence class */
    uint32_t precedence_class;
    /** Delay class */
    uint32_t delay_class;
    /** Reliability class */
    uint32_t reliability_class;
    /** Peak throughput class */
    uint32_t peak_throughput_class;
    /** Mean throughput class */
    uint32_t mean_throughput_class;
} wds_gprs_qos_t;

/**
 * @brief QMI WDS traffic flow template parameters type
 *
 */
typedef struct {
    /** Filter identifier */
    uint8_t filter_id;
    /** Evaluation precedence index */
    uint8_t eval_id;
    /** IP version number, see #QMI_ip_family_t */
    enum8_t ip_version;
    /** Source IP according to the following rules:
     *
     * IPv4 - Fill the first 4 bytes
     * IPv6 - Fill all the 16 bytes
     *
     */
    uint8_t source_ip[IPV6_ADDR_MAX_SIZE];
    /** Mask value for the source address */
    uint8_t source_ip_mask;
    /** Next header/protocol value */
    uint8_t next_header;
    /** Start value for the destination port range */
    uint16_t dest_port_range_start;
    /** End value for the destination port range */
    uint16_t dest_port_range_end;
    /** Start value for the source port range */
    uint16_t src_port_range_start;
    /** End value for the source port range */
    uint16_t src_port_range_end;
    /** IPSec security parameter index */
    uint32_t ipsec_spi;
    /** TOS mask (traffic class for IPv6 */
    uint16_t tos_mask;
    /** Flow label */
    uint32_t flow_label;
} wds_tft_parameters_t;

/**
 * @brief QMI WDS 3GPP LTE QOS parameters type
 *
 */
typedef struct {
    /** The requested QOS must be specified using the QOS class identifier according to the following values:
       - 0 - Requests the network to assign the appropriate QCI value
       - 1 to 4 - Associated with guaranteed bitrates
       - 5 to 9 - Associated with non guaranteed bitrates, the values specified as guaranteed and maximum bitrates are ignored
     */
    uint8_t qci;
    /** Guaranteed DL bitrate */
    uint32_t g_dl_bit_rate;
    /** Maximum DL bitrate */
    uint32_t max_dl_bit_rate;
    /** Guaranteed UL bitrate */
    uint32_t g_ul_bit_rate;
    /** Maximum UL bitrate */
    uint32_t max_ul_bit_rate;
} wds_3gpp_lte_qos_parameters_t;

#define QMI_WDS_IPV4_PCSCF_ADDR_LIST_MAX_SIZE 8

/**
 * @brief QMI WDS ipv6 address with prefix length type
 *
 */
typedef struct {
    uint16_t addr[IPV6_ADDR_SIZE];
    uint8_t prefix_len;
} wds_ipv6_addr_with_prefix_t;

#define QMI_DOMAIN_NAME_LIST_MAX_SIZE 6
#define QMI_DOMAIN_NAME_MAX_SIZE 120

/**
 * @brief QMI WDS domain name type
 *
 */
typedef struct {
    /** Domain name length */
    uint16_t domain_name_len;
    /** Domain name */
    char domain_name[QMI_DOMAIN_NAME_MAX_SIZE];
} wds_domain_name_t;

#define QMI_WDS_FQDN_LIST_MAX_SIZE 6

/**
 * @brief QMI WDS PCSCF FQDN list type
 *
 */
typedef struct {
    /** FQDN string length */
    uint16_t fqdn_len;
    /** FQDN string */
    char fqdn[QMI_WDS_MAX_STRING_SIZE_PROFILE];
} wds_pcscf_fqdn_item_t;

/**
 * @brief QMI WDS technology names
 *
 */
typedef enum {
    /** CDMA */
    QMI_WDS_TECHNOLOGY_NAME_CDMA              = 0x8001,
    /** UMTS */
    QMI_WDS_TECHNOLOGY_NAME_UMTS              = 0x8004,
    /** WLAN Local Break-out */
    QMI_WDS_TECHNOLOGY_NAME_WLAN_LOCAL_BRKOUT = 0x8020,
    /** IWLAN S2B */
    QMI_WDS_TECHNOLOGY_NAME_IWLAN_S2B         = 0x8021,
    /** EPC */
    QMI_WDS_TECHNOLOGY_NAME_EPC               = 0x8880,
    /** EMBMS */
    QMI_WDS_TECHNOLOGY_NAME_EMBMS             = 0x8882,
    /** Non-IP */
    QMI_WDS_TECHNOLOGY_NAME_NON_IP            = 0x8884,
    /** Modem link local */
    QMI_WDS_TECHNOLOGY_NAME_MODEM_LINK_LOCAL  = 0x8888,
} QMI_WDS_technology_name_t;

#define QMI_WDS_PCSCF_IPV6_LIST_MAX_SIZE 4

/**
 * @brief QMI WDS data connection profile details type
 *
 */
typedef struct {
    /** Profile name length */
    uint8_t profile_name_len;
    /** Profile name */
    char profile_name[QMI_WDS_MAX_STRING_SIZE_PROFILE];
    /** set if pdp_type should be considered */
    bool pdp_type_available;
    /** type of data payload exchanged over the airlink when the packet data session is established with this profile, see #QMI_WDS_PDP_type_t */
    enum8_t pdp_type;
    /** set if pdp_hdr_comp_type_available should be considered */
    bool pdp_hdr_comp_type_available;
    /** PDP header compression type, see #QMI_WDS_PDP_header_compression_type_t */
    enum8_t pdp_hdr_comp_type;
    /** set if pdp_data_comp_type should be considered */
    bool pdp_data_comp_type_available;
    /** PDP data compression type, see #QMI_WDS_PDP_data_compression_type_t */
    enum8_t pdp_data_comp_type;
    /** apn length */
    uint8_t apn_len;
    /** Context acess point node name */
    char apn[QMI_WDS_MAX_STRING_SIZE_PROFILE];
    /** set if pri_dns_ipv4_addr_pref should be considered */
    bool pri_dns_ipv4_addr_pref_available;
    /** Primary dns value that can be used as a preference during negotiation with network */
    uint32_t pri_dns_ipv4_addr_pref;
    /** set if sec_dns_ipv4_addr_pref should be considered */
    bool sec_dns_ipv4_addr_pref_available;
    /** Secondary dns value that can be used as a preference during negotiation with network */
    uint32_t sec_dns_ipv4_addr_pref;
    /** set if umts_req_qos should be considered */
    bool umts_req_qos_available;
    /** #wds_umts_qos_t instance */
    wds_umts_qos_t umts_req_qos;
    /** set if umts_min_qos should be considered */
    bool umts_min_qos_available;
    /** #wds_umts_qos_t instance */
    wds_umts_qos_t umts_min_qos;
    /** set if gprs_req_qos should be considered */
    bool gprs_req_qos_available;
    /** #wds_gprs_qos_t instance */
    wds_gprs_qos_t gprs_req_qos;
    /** set if gprs_min_qos should be considered */
    bool gprs_min_qos_available;
    /** #wds_gprs_qos_t instance */
    wds_gprs_qos_t gprs_min_qos;
    /** Username length */
    uint8_t username_len;
    /** Username used during data network authentication */
    char username[QMI_WDS_MAX_STRING_SIZE_PROFILE];
    /** Password length */
    uint8_t password_len;
    /** Password used during data network authentication */
    char password[QMI_WDS_MAX_STRING_SIZE_PROFILE];
    /** set if authentication_pref should be considered */
    bool authentication_pref_available;
    /** A bitmap that indicates the algorithm preference, according to the following values:
          Bit 0 - PAP preference:
       - 0 - PAP is never performed
       - 1 - PAP may be performed
          Bit 1 - CHAP preference:
       - 0 - CHAP is never performed
       - 1 - CHAP may be performed
          All the other bits must be set to zero by the client. If more than one bit is set, the device decides which authentication procedure is performed while setting up the data session.
     */
    uint8_t authentication_pref;
    /** set if ipv4_addr_pref should be considered */
    bool ipv4_addr_pref_available;
    /** IPV4 address value that can be used as a preference during negotiation with network */
    uint32_t ipv4_addr_pref;
    /** set if ipv4_gateway_addr is available (only in get runtime settings functions) */
    bool ipv4_gateway_addr_available;
    /** IPV4 gateway address */
    uint32_t ipv4_gateway_addr;
    /** set if ipv4_subnet_mask_addr is available (only in get runtime settings functions) */
    bool ipv4_subnet_mask_available;
    /** IPV4 subnet mask */
    uint32_t ipv4_subnet_mask_addr;
    /** Possible values:
       - true - request PCSFC address using PCO
       - false - do not request
     */
    bool pcscf_addr_using_pco;
    /** pcscf_ipv4_address list length  (only in get runtime settings functions) */
    uint8_t pcscf_ipv4_address_size;
    /** List of PCSCF IPV4 addresses */
    uint32_t pcscf_ipv4_address[QMI_WDS_IPV4_PCSCF_ADDR_LIST_MAX_SIZE];
    /** fqdn list length  (only in get runtime settings functions) */
    uint8_t fqdn_size;
    /** List of #wds_pcscf_fqdn_item_t instances */
    wds_pcscf_fqdn_item_t fqdn[QMI_WDS_FQDN_LIST_MAX_SIZE];
    /** set if ipv6_addr is available (only in get runtime settings functions) */
    bool ipv6_addr_available;
    /** #wds_ipv6_addr_with_prefix_t instance */
    wds_ipv6_addr_with_prefix_t ipv6_addr;
    /** set if ipv6_gw_addr is available (only in get runtime settings functions) */
    bool ipv6_gw_addr_available;
    /** #wds_ipv6_addr_with_prefix_t instance */
    wds_ipv6_addr_with_prefix_t ipv6_gw_addr;
    /** set if pri_dns_ipv6_addr is available (only in get runtime settings functions) */
    bool pri_dns_ipv6_addr_available;
    /** Primary dns ipv6 address */
    uint16_t pri_dns_ipv6_addr[IPV6_ADDR_SIZE];
    /** set if sec_dns_ipv6_addr is available (only in get runtime settings functions) */
    bool sec_dns_ipv6_addr_available;
    /** Secondary dns ipv6 value */
    uint16_t sec_dns_ipv6_addr[IPV6_ADDR_SIZE];
    /** set if MTU is available (only in get runtime settings functions) */
    bool mtu_available;
    /** MTU */
    uint32_t mtu;
    /** domain name list length (only in get runtime settings functions) */
    uint8_t domain_name_list_size;
    /** Domain name list */
    wds_domain_name_t domain_name_list[QMI_DOMAIN_NAME_LIST_MAX_SIZE];
    /** set if ip_family is available (only in get runtime settings functions) */
    bool ip_family_available;
    /** IP family, see #QMI_ip_family_t */
    enum8_t ip_family;
    /** set if pdp_access_control_flag_available should be considered */
    bool pdp_access_control_flag_available;
    /** PDP access control, see #QMI_WDS_PDP_access_control_t */
    enum8_t pdp_access_control_flag;
    /** Possible values:
       - true - request PCSFC address using DHCP
       - false - do not request
     */
    bool pcscf_addr_using_dhcp;
    /** Possible values:
       - true - request IM CN flag for this profile
       - false - do not request
     */
    bool im_cn_flag;
    /** set if technology_name is available (only in get runtime settings functions) */
    bool technology_name_available;
    /** Technology on which current packet data session is in progress, see #QMI_WDS_technology_name_t */
    uint16_t technology_name;
    /** set if tft_id1 is available */
    bool tft_id1_available;
    /** #wds_tft_parameters_t instance */
    wds_tft_parameters_t tft_id1;
    /** set if tft_id2 is available */
    bool tft_id2_available;
    /** #wds_tft_parameters_t instance */
    wds_tft_parameters_t tft_id2;
    /** Set if pdp_context should be considered */
    bool pdp_context_available;
    /** PDP context number */
    uint8_t pdp_context;
    /** PDP context seconday flag, according to the following values:
       - true - This is secondary profile
       - false - This is not secondary profile
     */
    bool secondary_flag;
    /** Set if primary_id should be considered */
    bool primary_id_available;
    /** PDP context number primary ID */
    uint8_t primary_id;
    /** set if ipv6_addr_pref should be considered */
    bool ipv6_addr_pref_available;
    /** IPV6 address value that can be used as a preference during negotiation with network */
    uint16_t ipv6_addr_pref[IPV6_ADDR_SIZE];
    /** set if umts_req_qos_signaling should be considered */
    bool umts_req_qos_signaling_available;
    /** #wds_umts_qos_t instance */
    wds_umts_qos_t umts_req_qos_signaling;
    /** set if umts_min_qos_signaling should be considered */
    bool umts_min_qos_signaling_available;
    /** #wds_umts_qos_t instance */
    wds_umts_qos_t umts_min_qos_signaling;
    /** set if pri_dns_ipv6_addr_pref should be considered */
    bool pri_dns_ipv6_addr_pref_available;
    /** Primary dns ipv6 value that can be used as a preference during negotiation with network */
    uint16_t pri_dns_ipv6_addr_pref[IPV6_ADDR_SIZE];
    /** set if sec_dns_ipv6_addr_pref should be considered (only in profile creation/modification functions) */
    uint8_t sec_dns_ipv6_addr_pref_available;
    /** Secondary dns ipv6 value that can be used as a preference during negotiation with network */
    uint16_t sec_dns_ipv6_addr_pref[IPV6_ADDR_SIZE];
    /** Set if addr_allocation_pref should be considered */
    bool addr_allocation_pref_available;
    /** Address allocation preference, see #QMI_WDS_addr_alloc_pref_t */
    enum8_t addr_allocation_pref;
    /** Set if lte_qos_params should be considered */
    bool lte_qos_params_available;
    /** #wds_3gpp_lte_qos_parameters_t instance */
    wds_3gpp_lte_qos_parameters_t lte_qos_params;
    /** If true disables the use of this profile for making data calls, making the call to fail locally */
    bool apn_disabled_flag;
    /** set if pdn_inactivity_timeout should be considered */
    bool pdn_inactivity_timeout_available;
    /** Duration of the inactivity timer in seconds before disconnecting the call 0 means an infinite value. */
    uint32_t pdn_inactivity_timeout;
    /** set if apn_class should be considered */
    bool apn_class_available;
    /** An opaque numeric identifier representing the APN in the profile */
    uint8_t apn_class;
    /** If set the user can make emergency calls using this profile */
    bool support_emergency_calls;
    /** pcscf_ipv6_address list length (only in get runtime settings functions) */
    uint8_t pcscf_ipv6_addr_list_sz;
    /** PCSCF IPv6 server address (in network byte order) */
    uint16_t pcscf_ipv6_addr_list[QMI_WDS_PCSCF_IPV6_LIST_MAX_SIZE][IPV6_ADDR_SIZE];
} wds_settings_profile_info_t;

/**
 * @brief QMI WDS data connection profile type
 *
 */
typedef struct {
    /** set if profile_id is available or should be considered as input. NOTE: mandatory for telit_wds_create_profile */
    bool profile_id_available;
    /** #wds_settings_profile_id_t instance */
    wds_settings_profile_id_t profile_id;
    /** set if profile_settings is available or should be considered as input */
    bool profile_settings_available;
    /** #wds_settings_profile_info_t instance */
    wds_settings_profile_info_t profile_settings;
    /** set if extended_error_code is available */
    bool extended_error_code_available;
    /** instance of #wds_profile_ext_err_t */
    wds_profile_ext_err_t extended_error_code;
} wds_profile_t;

/**
 * @brief Pack QMI_WDS_CREATE_PROFILE (39 - 0x0027) request
 *
 * This function packs a qmi request for creating a configured
 * profile with specified settings
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_profile_t
 */
QMI_SDK_error_t telit_wds_create_profile_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_profile_t *input);

/**
 * @brief QMI WDS data connection profile related response type
 *
 */
typedef struct {
    /** set if profile_id is available */
    bool profile_id_available;
    /** #wds_settings_profile_id_t instance */
    wds_settings_profile_id_t profile_id;
    /** set if extended_error_code is available */
    bool extended_error_code_available;
    /** extended error code, see #QMI_WDS_DS_profile_ext_error_t */
    enum16_t extended_error_code;
} wds_profile_resp_t;

/**
 * @brief Unpack QMI_WDS_CREATE_PROFILE (39 - 0x0027) request
 *
 * This function unpacks the result of a QMI_WDS_CREATE_PROFILE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wds_profile_resp_t struct
 */
QMI_SDK_error_t telit_wds_create_profile_unpack(uint8_t *rsp, uint16_t len, wds_profile_resp_t *out);

/**
 * @brief Pack QMI_WDS_MODIFY_PROFILE_SETTINGS (40 - 0x0028) request
 *
 * This function packs a qmi request for modifying a configured
 * profile with specified settings
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_profile_t
 */
QMI_SDK_error_t telit_wds_modify_profile_settings_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_profile_t *input);

/**
 * @brief Unpack QMI_WDS_MODIFY_PROFILE_SETTINGS (40 - 0x0028) request
 *
 * This function unpacks the result of a QMI_WDS_MODIFY_PROFILE_SETTINGS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wds_profile_ext_err_t
 */
QMI_SDK_error_t telit_wds_modify_profile_settings_unpack(uint8_t *rsp, uint16_t len,
        wds_profile_ext_err_t *out);

/**
 * @brief Pack QMI_WDS_DELETE_PROFILE (41 - 0x0029) request
 *
 * This function packs a qmi request for deleting a
 * configured profile
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_settings_profile_id_t
 */
QMI_SDK_error_t telit_wds_delete_profile_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wds_settings_profile_id_t *input);

/**
 * @brief Unpack QMI_WDS_DELETE_PROFILE (41 - 0x0029) request
 *
 * This function unpacks the result of a QMI_WDS_DELETE_PROFILE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wds_profile_ext_err_t
 */
QMI_SDK_error_t telit_wds_delete_profile_unpack(uint8_t *rsp, uint16_t len, wds_profile_ext_err_t *out);

#define QMI_WDS_PROFILE_LIST_MAX_ELEMENTS 30

/**
 * @brief QMI WDS profile list element data type
 *
 */
typedef struct {
    /** Identifies the technology type of the profile, see #QMI_WDS_profile_type_t */
    enum8_t profile_type;
    /** Profile number identifying the profile */
    uint8_t profile_index;
    /** profile_name string length */
    uint8_t profile_name_len;
    /** profile description */
    char profile_name[MAX_STRING_SIZE];
} wds_profile_list_element_t;

/**
 * @brief QMI WDS profile list data type
 *
 */
typedef struct {
    /** profile_list size */
    uint8_t profile_list_size;
    /** see #wds_profile_list_element_t */
    wds_profile_list_element_t profile_list[QMI_WDS_PROFILE_LIST_MAX_ELEMENTS];
    /** set if extended_error_code is available */
    bool extended_error_code_available;
    /** instance of #wds_profile_ext_err_t */
    wds_profile_ext_err_t extended_error_code;
} wds_profile_list_t;

/**
 * @brief QMI WDS search type
 *
 */
typedef enum {
    /** Search for APN name */
    QMI_WDS_PROFILE_SEARCH_TYPE_APN_NAME = 0x0001,
} QMI_WDS_search_type_t;

/**
 * @brief QMI WDS profile type data type
 *
 */
typedef struct {
    /** set if profile_type is available */
    bool profile_type_available;
    /** Identifies the technology type of the profile,
       see #QMI_WDS_profile_type_t */
    enum8_t profile_type;
    /** set if search_type is available */
    bool search_type_available;
    /** Identifies the type of search, see #QMI_WDS_search_type_t */
    enum32_t search_type;
    /** apn_name string length */
    uint8_t apn_name_len;
    /** apn name */
    char apn_name[MAX_STRING_SIZE];
} wds_profile_list_input_t;

/**
 * @brief Pack QMI_WDS_GET_PROFILE_LIST (42 - 0x002A) request
 *
 * This function retrieves a list of configured profiles present on the wireless device
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_profile_list_input_t
 */
QMI_SDK_error_t telit_wds_get_profile_list_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_profile_list_input_t *input);

/**
 * @brief Unpack QMI_WDS_GET_PROFILE_LIST (42 - 0x002A request)
 *
 * This function unpacks the result of a QMI_WDS_GET_PROFILE_LIST request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wds_profile_list_t struct
 */
QMI_SDK_error_t telit_wds_get_profile_list_unpack(uint8_t *rsp,
        uint16_t len,
        wds_profile_list_t *out);

/**
 * @brief Pack QMI_WDS_GET_PROFILE_SETTINGS (43 - 0x002B) request
 *
 * This function packs a qmi request for retrieving the settings
 * from a configured profile
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_settings_profile_id_t
 */
QMI_SDK_error_t telit_wds_get_profile_settings_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_settings_profile_id_t *input);

/**
 * @brief Unpack QMI_WDS_GET_PROFILE_SETTINGS (43 - 0x002B) request
 *
 * This function unpacks the result of a QMI_WDS_GET_PROFILE_SETTINGS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out buffer to hold #wds_profile_t struct
 */
QMI_SDK_error_t telit_wds_get_profile_settings_unpack(uint8_t *rsp, uint16_t len, wds_profile_t *out);

/**
 * @brief QMI WDS pdp type info type
 *
 */
typedef struct {
    /** type of data payload exchanged over the airlink when the packet data session is established with this profile, see #QMI_WDS_PDP_type_t */
    enum8_t type;
} wds_settings_pdp_type_t;

/**
 * @brief QMI WDS requested settings
 *
 */
typedef enum {
    /** Profile identifier */
    QMI_WDS_SETTING_PROFILE_ID                = (1 << 0),
    /** Profile name */
    QMI_WDS_SETTING_PROFILE_NAME              = (1 << 1),
    /** PDP type */
    QMI_WDS_SETTING_PDP_TYPE                  = (1 << 2),
    /** APN name */
    QMI_WDS_SETTING_APN_NAME                  = (1 << 3),
    /** DNS address */
    QMI_WDS_SETTING_DNS_ADDRESS               = (1 << 4),
    /** UMTS/GPRS granted QOS */
    QMI_WDS_SETTING_UMTS_GPRS_GRANTED_QOS     = (1 << 5),
    /** Username */
    QMI_WDS_SETTING_USERNAME                  = (1 << 6),
    /** Authentication protocol */
    QMI_WDS_SETTING_AUTH_PROTOCOL             = (1 << 7),
    /** IP address */
    QMI_WDS_SETTING_IP_ADDRESS                = (1 << 8),
    /** Gateway information (address and subnet mask) */
    QMI_WDS_SETTING_GATEWAY_INFO              = (1 << 9),
    /** PCSCF address using a PCO flag */
    QMI_WDS_SETTING_PCSCF_ADDRESS             = (1 << 10),
    /** PCSCF server address list */
    QMI_WDS_SETTING_PCSCF_SERVER_ADDRESS_LIST = (1 << 11),
    /** PCSCF domain name list */
    QMI_WDS_SETTING_PCSCF_DOMAIN_NAME_LIST    = (1 << 12),
    /** MTU */
    QMI_WDS_SETTING_MTU                       = (1 << 13),
    /** Domain name list */
    QMI_WDS_SETTING_DOMAIN_NAME_LIST          = (1 << 14),
    /** IP family */
    QMI_WDS_SETTING_IP_FAMILY                 = (1 << 15),
    /** IM_CM flag */
    QMI_WDS_SETTING_IM_CM_FLAG                = (1 << 16),
    /** Technology name */
    QMI_WDS_SETTING_TECHNOLOGY_NAME           = (1 << 17),
    /** Operator reserved PCO */
    QMI_WDS_SETTING_OPERATOR_RESERVED_PCO     = (1 << 18),
} QMI_WDS_requested_setting_t;

/**
 * @brief input type for QMI_WDS_GET_RUNTIME_SETTINGS
 *
 */
typedef struct {
    /** Requested settings mask, see #QMI_WDS_requested_setting_t */
    uint32_t requested_settings;
} wds_runtime_settings_info_t;

/**
 * @brief Pack QMI_WDS_GET_RUNTIME_SETTINGS (45 - 0x002D) request
 *
 * This function packs a qmi request for retrieving the packet data
 * session settings currently in use
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_runtime_settings_info_t
 */
QMI_SDK_error_t telit_wds_get_runtime_settings_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_runtime_settings_info_t *input);

/**
 * @brief Unpack QMI_WDS_GET_RUNTIME_SETTINGS (45 - 0x002D request)
 *
 * This function unpacks the result of a QMI_WDS_GET_RUNTIME_SETTINGS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wds_profile_t
 */
QMI_SDK_error_t telit_wds_get_runtime_settings_unpack(uint8_t *rsp, uint16_t len, wds_profile_t *out);

/**
 * @brief output type for QMI_WDS_GET_CALL_DURATION
 *
 */
typedef struct {
    /** call duration in milliseconds */
    uint64_t call_duration;
} wds_duration_t;

/**
 * @brief Pack QMI_WDS_GET_CALL_DURATION (53 - 0x0035) request
 *
 * This function packs a qmi request for querying the duration
 * of the current call
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_wds_get_call_duration_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_WDS_GET_CALL_DURATION (53 - 0x0035) request
 *
 * This function unpacks the result of a QMI_WDS_GET_CALL_DURATION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wds_duration_t
 */
QMI_SDK_error_t telit_wds_get_call_duration_unpack(uint8_t *rsp, uint16_t len, wds_duration_t *out);

/**
 * @brief QMI WDS profile family
 *
 */
typedef enum {
    /** Embedded */
    QMI_WDS_PROFILE_FAMILY_EMBEDDED = 0,
    /** Tethered */
    QMI_WDS_PROFILE_FAMILY_TETHERED = 1,
} QMI_WDS_profile_family_t;

/**
 * @brief input type for QMI_WDS_GET_DEFAULT_PROFILE_NUM
 *
 */
typedef struct {
    /** Profile type, see #QMI_WDS_profile_type_t */
    enum8_t profile_type;
    /** Profile family, see #QMI_WDS_profile_family_t */
    enum8_t profile_family;
} wds_default_profile_input_t;

/**
 * @brief output type for QMI_WDS_GET_DEFAULT_PROFILE_NUM
 *
 */
typedef struct {
    /** Default profile index */
    uint8_t profile_index;
} wds_default_profile_out_t;

/**
 * @brief Pack QMI_WDS_GET_DEFAULT_PROFILE_NUM (73 - 0x0049) request
 *
 * This function packs a qmi request for getting the default profile
 * number
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_default_profile_input_t
 */
QMI_SDK_error_t telit_wds_get_default_profile_num_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_default_profile_input_t *input);

/**
 * @brief Unpack QMI_WDS_GET_DEFAULT_PROFILE_NUM (73 - 0x0049) request
 *
 * This function unpacks the result of a QMI_WDS_GET_DEFAULT_PROFILE_NUM request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out buffer to hold #wds_default_profile_out_t
 */
QMI_SDK_error_t telit_wds_get_default_profile_num_unpack(uint8_t *rsp,
        uint16_t len, wds_default_profile_out_t *out);

/**
 * @brief input type for QMI_WDS_SET_CLIENT_IP_FAMILY_PREF
 *
 */
typedef struct {
    /** IP family preference, see #QMI_ip_family_t */
    enum8_t ip_preference;
} wds_ip_family_t;

/**
 * @brief Pack QMI_WDS_SET_CLIENT_IP_FAMILY_PREF (77 - 0x004D) request
 *
 * This function packs a qmi request for setting the control
 * point IP preference
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_ip_family_t
 */
QMI_SDK_error_t telit_wds_set_client_ip_family_pref_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_ip_family_t *input);

/**
 * @brief Unpack QMI_WDS_SET_CLIENT_IP_FAMILY_PREF (77 - 0x004D) request
 *
 * This function unpacks the result of a QMI_WDS_SET_CLIENT_IP_FAMILY_PREF request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_wds_set_client_ip_family_pref_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI WDS attach PDN list action type
 *
 */
typedef enum {
    /** No action */
    QMI_WDS_ATTACH_PDN_LIST_ACTION                          = 0x01,
    /** PD detach or PDN disconnect based on modem configuration */
    QMI_WDS_ATTACH_PDN_LIST_ACTION_DETACH_OR_PDN_DISCONNECT = 0x02,
} QMI_WDS_attach_pdn_list_action_t;

#define QMI_ATTACH_PDN_LIST_MAX_SIZE 4

/**
 * @brief input type for QMI_WDS_SET_LTE_ATTACH_PDN_LIST
 *
 */
typedef struct {
    /** attach_pdn_list size */
    uint8_t attach_pdn_list_size;
    /** List of profile IDs to attach to, listed in order of decreasing priority */
    uint16_t attach_pdn_list[QMI_ATTACH_PDN_LIST_MAX_SIZE];
    /** set if action is available */
    bool action_available;
    /** action to be executed, see #QMI_WDS_attach_pdn_list_action_t */
    enum32_t action;
} wds_attach_pdn_list_t;

/**
 * @brief Pack QMI_WDS_SET_LTE_ATTACH_PDN_LIST (147 - 0x0093) request
 *
 * This function packs a QMI request to set the LTE attach PDN list specified
 * by the control point
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_attach_pdn_list_t
 */
QMI_SDK_error_t telit_wds_set_lte_attach_pdn_list_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_attach_pdn_list_t *input);

/**
 * @brief Unpack QMI_WDS_SET_LTE_ATTACH_PDN_LIST (147 - 0x0093) request
 *
 * This function unpacks the result of a QMI_WDS_SET_LTE_ATTACH_PDN_LIST request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_wds_set_lte_attach_pdn_list_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief output type for QMI_WDS_GET_LTE_ATTACH_PDN_LIST
 *
 */
typedef struct {
    /** attach_pdn_list size */
    uint8_t attach_pdn_list_size;
    /** List of profile IDs to attach to, listed in order of decreasing priority */
    uint16_t attach_pdn_list[QMI_ATTACH_PDN_LIST_MAX_SIZE];
    /** pending attach_pdn_list size */
    uint8_t pending_attach_pdn_list_size;
    /** List of profile IDs to attach to, listed in order of decreasing priority */
    uint16_t pending_attach_pdn_list[QMI_ATTACH_PDN_LIST_MAX_SIZE];
} wds_get_attach_pdn_list_t;

/**
 * @brief Pack QMI_WDS_GET_LTE_ATTACH_PDN_LIST (148 - 0x0094) request
 *
 * This function packs a QMI request to get the LTE attach PDN list specified
 * by the control point
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_wds_get_lte_attach_pdn_list_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_WDS_GET_LTE_ATTACH_PDN_LIST (148 - 0x0094) request
 *
 * This function unpacks the result of a QMI_WDS_GET_LTE_ATTACH_PDN_LIST request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wds_get_attach_pdn_list_t
 */
QMI_SDK_error_t telit_wds_get_lte_attach_pdn_list_unpack(uint8_t *rsp, uint16_t len,
        wds_get_attach_pdn_list_t *out);

/**
 * @brief QMI WDS peripheral endpoint type
 *
 */
typedef enum {
    /** High-speed universal serial bus */
    QMI_WDS_DATA_EP_TYPE_HSUSB = 0x02,
    /** PCIe endpoint type */
    QMI_WDS_DATA_EP_TYPE_PCIE  = 0x03,
} QMI_WDS_peripheral_endpoint_type_t;

/**
 * @brief WDS peripheral endpoint
 *
 */
typedef struct {
    /** peripheral endpoint type, see #QMI_WDS_peripheral_endpoint_type_t */
    enum32_t ep_type;
    /** peripheral interface number */
    uint32_t iface_id;
} wds_peripheral_endpoint_t;

/**
 * @brief input type for QMI_WDS_BIND_MUX_DATA_PORT
 *
 */
typedef struct {
    /** set if peripheral_endpoint is available */
    bool peripheral_endpoint_available;
    /** peripheral_endpoint, see #wds_peripheral_endpoint_t */
    wds_peripheral_endpoint_t peripheral_endpoint;
    /** set if mux_id is available */
    bool mux_id_available;
    /** MUX ID of the logical data channel to which the client binds, range 1 - 127 */
    uint8_t mux_id;
} wds_bind_mux_data_port_t;

/**
 * @brief Pack QMI_WDS_BIND_MUX_DATA_PORT (162 - 0x00A2) request
 *
 * This function packs a qmi request for binding a control point to a muxed data port
 * IMPORTANT: this command must be sent immediately after the client ID is assigned.
 * If other control messages are sent before the bind request, the service might return
 * a QMI_ERR_NO_EFFECT error and ignore the bind request.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_bind_mux_data_port_t
 */
QMI_SDK_error_t telit_wds_bind_mux_data_port_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_bind_mux_data_port_t *input);

/**
 * @brief Unpack QMI_WDS_BIND_MUX_DATA_PORT (162 - 0x00A2) request
 *
 * This function unpacks the result of a QMI_WDS_BIND_MUX_DATA_PORT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_wds_bind_mux_data_port_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI WDS Data Bearer Technologies
 *
 */
typedef enum {
    /** CDMA 2000 1X */
    QMI_WDS_DATA_BEARER_TECH_CDMA20001X                 = 0x01,
    /** CDMA 2000 HRPD (1xEV-DO) */
    QMI_WDS_DATA_BEARER_TECH_CDMA20001X_EVDO_REV0       = 0x02,
    /** GSM */
    QMI_WDS_DATA_BEARER_TECH_GPRS                       = 0x03,
    /** UMTS */
    QMI_WDS_DATA_BEARER_TECH_WCDMA                      = 0x04,
    /** CDMA 2000 HRPD (1xEV-DO Rev. A) */
    QMI_WDS_DATA_BEARER_TECH_CDMA20001X_EVDO_REVA       = 0x05,
    /** EDGE */
    QMI_WDS_DATA_BEARER_TECH_EGPRS                      = 0x06,
    /** HSDPA and WCDMA */
    QMI_WDS_DATA_BEARER_TECH_HSDPA_WCDMA                = 0x07,
    /** WCDMA and HSUPA */
    QMI_WDS_DATA_BEARER_TECH_WCDMA_HSUPA                = 0x08,
    /** HSDPA and HSUPA */
    QMI_WDS_DATA_BEARER_TECH_HSDPA_HSUPA                = 0x09,
    /** LTE */
    QMI_WDS_DATA_BEARER_TECH_LTE                        = 0x0A,
    /** CDMA2000 EHRPD */
    QMI_WDS_DATA_BEARER_TECH_CDMA2000_EHRPD             = 0x0B,
    /** HSDPA+ and WCDMA */
    QMI_WDS_DATA_BEARER_TECH_HSDPA_PLUS_WCDMA           = 0x0C,
    /** HSDPA+ and HSUPA */
    QMI_WDS_DATA_BEARER_TECH_HSDPA_PLUS_HSUPA           = 0x0D,
    /** DC HSDPA+ and WCDMA */
    QMI_WDS_DATA_BEARER_TECH_DUAL_CELL_HSDPA_PLUS_WCDMA = 0x0E,
    /** DC HSDPA+ and HSUPA */
    QMI_WDS_DATA_BEARER_TECH_DUAL_CELL_HSDPA_PLUS_HSUPA = 0x0F,
    /** HSDPA+ and 64QAM */
    QMI_WDS_DATA_BEARER_TECH_HSDPA_PLUS_64QAM           = 0x10,
    /** HSDPA+, 64QAM and HSUPA */
    QMI_WDS_DATA_BEARER_TECH_HSDPA_PLUS_64QAM_HSUPA     = 0x11,
    /** TD-SCDMA */
    QMI_WDS_DATA_BEARER_TECH_TDSCDMA                    = 0x12,
    /** TD-SCDMA and HSDPA */
    QMI_WDS_DATA_BEARER_TECH_TDSCDMA_HSDPA              = 0x13,
    /** TD-SCDMA and HSUPA */
    QMI_WDS_DATA_BEARER_TECH_TDSCDMA_HSUPA              = 0x14,
    /** IWLAN S2B */
    QMI_WDS_DATA_BEARER_TECH_IWLAN_S2B                  = 0x15,
    /** Unknown */
    QMI_WDS_DATA_BEARER_TECH_UNKNOWN                    = 0xFF,
} QMI_WDS_data_bearer_technology_t;

/**
 * @brief QMI WDS system network types
 *
 */
typedef enum {
    /** 3GPP */
    QMI_WDS_SYSTEM_NETWORK_TYPE_3GPP  = 0x00,
    /** 3GPP2 */
    QMI_WDS_SYSTEM_NETWORK_TYPE_3GPP2 = 0x01,
} QMI_WDS_system_network_type_t;

/**
 * @brief QMI WDS network types
 *
 */
typedef enum {
    /** Traffic channel dormant */
    QMI_WDS_DORMANCY_STATUS_TRAFFIC_CHANNEL_DORMANT = 0x01,
    /** Traffic channel active */
    QMI_WDS_DORMANCY_STATUS_TRAFFIC_CHANNEL_ACTIVE  = 0x02,
} QMI_WDS_dormancy_status_t;

/**
 * @brief QMI WDS requested settings
 *
 */
typedef enum {
    /** The field should be ignored */
    QMI_WDS_DATA_BEARER_RAT_UNKNOWN       = 0,
    /** WCDMA */
    QMI_WDS_DATA_BEARER_RAT_WCDMA         = (1 << 0),
    /** GPRS */
    QMI_WDS_DATA_BEARER_RAT_GPRS          = (1 << 1),
    /** HSDPA */
    QMI_WDS_DATA_BEARER_RAT_HSDPA         = (1 << 2),
    /** HSUPA */
    QMI_WDS_DATA_BEARER_RAT_HSUPA         = (1 << 3),
    /** EDGE */
    QMI_WDS_DATA_BEARER_RAT_EDGE          = (1 << 4),
    /** LTE */
    QMI_WDS_DATA_BEARER_RAT_LTE           = (1 << 5),
    /** HSDPA+ */
    QMI_WDS_DATA_BEARER_RAT_HSDPA_PLUS    = (1 << 6),
    /** DC HSDPA+ */
    QMI_WDS_DATA_BEARER_RAT_DC_HSDPA_PLUS = (1 << 7),
    /** 64 QAM */
    QMI_WDS_DATA_BEARER_RAT_64QAM         = (1 << 8),
    /** TD-SCDMA */
    QMI_WDS_DATA_BEARER_RAT_TDSCDMA       = (1 << 9),
    /** LTE FDD */
    QMI_WDS_DATA_BEARER_RAT_LTE_FDD       = (1 << 10),
    /** LTE TDD */
    QMI_WDS_DATA_BEARER_RAT_LTE_TDD       = (1 << 11),
    /** Null bearer */
    QMI_WDS_DATA_BEARER_RAT_NULL          = (1 << 15),
} QMI_WDS_data_bearer_rat_t;

/**
 * @brief QMI WDS current data bearer technology type
 *
 */
typedef struct {
    /** Current network type of data bearer, see #QMI_network_type_t */
    enum8_t current_nw;
    /** RAT mask to indicate the type of technology, see #QMI_WDS_data_bearer_rat_t */
    uint32_t rat_mask;
    /** Service option or type of application (always 0x00) */
    uint32_t so_mask;
} wds_current_data_bearer_tech_t;

/**
 * @brief QMI WDS Radio Access Technologies
 *
 */
typedef enum {
    /** No bearer  */
    QMI_WDS_RAT_NULL     = 0x01,
    /** WCDMA */
    QMI_WDS_RAT_WCDMA    = 0x02,
    /** GERAN */
    QMI_WDS_RAT_GERAN    = 0x03,
    /** LTE */
    QMI_WDS_RAT_LTE      = 0x04,
    /** TD-SCDMA */
    QMI_WDS_RAT_TD_SCDMA = 0x05,
    /** WLAN */
    QMI_WDS_RAT_WLAN     = 0x06,
    /** 5G */
    QMI_WDS_RAT_5G       = 0x07,
} QMI_WDS_RAT_t;

/**
 * @brief QMI WDS service option values
 *
 */
typedef enum {
    /** The field should be ignored */
    QMI_WDS_DATA_SO_UNKNOWN               = 0,
    /** WCDMA */
    QMI_WDS_DATA_SO_WCDMA                 = (1 << 0),
    /** HSDPA */
    QMI_WDS_DATA_SO_HSDPA                 = (1 << 1),
    /** HSUPA */
    QMI_WDS_DATA_SO_HSUPA                 = (1 << 2),
    /** HSDPA+ */
    QMI_WDS_DATA_SO_HSDPA_PLUS            = (1 << 3),
    /** DC HSDPA+ */
    QMI_WDS_DATA_SO_DC_HSDPA_PLUS         = (1 << 4),
    /** 64 QAM */
    QMI_WDS_DATA_SO_64_QAM                = (1 << 5),
    /** HSPA */
    QMI_WDS_DATA_SO_HSPA                  = (1 << 6),
    /** GPRS */
    QMI_WDS_DATA_SO_GPRS                  = (1 << 7),
    /** EDGE */
    QMI_WDS_DATA_SO_EDGE                  = (1 << 8),
    /** GSM */
    QMI_WDS_DATA_SO_GSM                   = (1 << 9),
    /** S2B */
    QMI_WDS_DATA_SO_S2B                   = (1 << 10),
    /** LTE limited service */
    QMI_WDS_DATA_SO_LTE_LIMITED_SERVICE   = (1 << 11),
    /** LTE FDD */
    QMI_WDS_DATA_SO_LTE_FDD               = (1 << 12),
    /** LTE TDD */
    QMI_WDS_DATA_SO_LTE_TDD               = (1 << 13),
    /** TD-SCDMA */
    QMI_WDS_DATA_SO_TDSCDMA               = (1 << 14),
    /** DC HSUPA */
    QMI_WDS_DATA_SO_DC_HSUPA              = (1 << 15),
    /** LTE CA DL */
    QMI_WDS_DATA_SO_LTE_CA_DL             = (1 << 16),
    /** LTE CA UL */
    QMI_WDS_DATA_SO_LTE_CA_UL             = (1 << 17),
    /** 4.5G */
    QMI_WDS_DATA_SO_LTE_ADVANCED_PRO      = (1 << 18),
    /** 4.5G Plus */
    QMI_WDS_DATA_SO_LTE_ADVANCED_PRO_PLUS = (1 << 19),
} QMI_WDS_service_option_t;

/**
 * @brief QMI WDS data call status
 *
 */
typedef enum {
    /** Data call status unknown */
    QMI_WDS_DATA_CALL_UNKNOWN    = 0x00,
    /** Data call activated */
    QMI_WDS_DATA_CALL_ACTIVATED  = 0x01,
    /** Data call terminated */
    QMI_WDS_DATA_CALL_TERMINATED = 0x02,
} QMI_WDS_data_call_status_t;

/**
 * @brief QMI WDS data bearer technology extended type
 *
 */
typedef struct {
    /** Technology type, see #QMI_WDS_system_network_type_t */
    enum32_t technology;
    /** RAT value, see #QMI_WDS_RAT_t */
    uint32_t rat_value;
    /** Service option or type of application, see #QMI_WDS_service_option_t */
    uint64_t so_mask;
} wds_data_bearer_tech_ext_t;

/**
 * @brief QMI WDS network info type
 *
 */
typedef struct {
    /** Current network type of data bearer, see #QMI_WDS_system_network_type_t */
    enum8_t network;
    /** RAT mask to indicate the type of technology, see #QMI_WDS_data_bearer_rat_t */
    uint32_t rat_mask;
    /** Service option or type of application (always 0x00) */
    uint32_t so_mask;
} wds_network_info_t;

#define QMI_WDS_MAX_NETWORK_INFO_SIZE 16
/**
 * @brief QMI WDS data system status type
 *
 */
typedef struct {
    /** Preferred network type, see #QMI_WDS_system_network_type_t */
    enum8_t preferred_network;
    /** Length of network_info array */
    uint8_t network_info_size;
    /** array of #wds_network_info_t instances */
    wds_network_info_t network_info[QMI_WDS_MAX_NETWORK_INFO_SIZE];
} wds_data_system_status_t;

/**
 * @brief output type for QMI_WDS_EVENT_REPORT_IND
 *
 */
typedef struct {
    /** set if current_data_bearer_tech is available */
    bool current_data_bearer_tech_available;
    /** #wds_current_data_bearer_tech_t instance */
    wds_current_data_bearer_tech_t current_data_bearer_tech;
    /** set if tx_ok_count is available */
    bool tx_ok_count_available;
    /** Number of packet transmitted without error */
    uint32_t tx_ok_count;
    /** set if rx_ok_count is available */
    bool rx_ok_count_available;
    /** Number of packet received without error */
    uint32_t rx_ok_count;
    /** set if tx_ok_bytes_count is available */
    bool tx_ok_bytes_count_available;
    /** Number of bytes transmitted without error */
    uint64_t tx_ok_bytes_count;
    /** set if rx_ok_bytes_count is available */
    bool rx_ok_bytes_count_available;
    /** Number of bytes received without error */
    uint64_t rx_ok_bytes_count;
    /** set if mip_status is available */
    bool mip_status_available;
    /** Status of the last MIP call (or attempt) according to the following values:
       - 0x00 - Success
       - 0 - Error code as defined in RFC2002
     */
    uint8_t mip_status;
    /** set if dormancy_status is available */
    bool dormancy_status_available;
    /** Dormancy status, see #QMI_WDS_dormancy_status_t */
    enum8_t dormancy_status;
    /** set if data_bearer_tech is available */
    bool data_bearer_tech_available;
    /** Data bearer technology, see #QMI_WDS_data_bearer_technology_t */
    enum8_t data_bearer_tech;
    /** set if data_system_status is available */
    bool data_system_status_available;
    /** #wds_data_system_status_t instance */
    wds_data_system_status_t data_system_status;
    /** set if data_bearer_tech_ext is available */
    bool data_bearer_tech_ext_available;
    /** #wds_data_bearer_tech_ext_t instance */
    wds_data_bearer_tech_ext_t data_bearer_tech_ext;
    /** set if data_call_status is available */
    bool data_call_status_available;
    /** data call status, see #QMI_WDS_data_call_status_t */
    enum8_t data_call_status;
} wds_ind_event_report_t;

/**
 * @brief QMI WDS service session state info type
 *
 */
typedef struct {
    /** current link status, see #QMI_WDS_data_connection_status_t */
    enum8_t connection_status;
    /** indicates whether the network interface on the host must be reconfigured */
    bool reconfiguration_required;
    /** set if call_end_reason is available */
    bool call_end_reason_available;
    /** call end reason, see #QMI_WDS_call_end_reason_t */
    enum16_t call_end_reason;
    /** set if call_end_reason_verbose is available */
    bool call_end_reason_verbose_available;
    /** #wds_verbose_call_end_reason_t instance */
    wds_verbose_call_end_reason_t call_end_reason_verbose;
    /** set if ip_family is available */
    bool ip_family_available;
    /** ip family of the packet data connection, see #QMI_ip_family_t */
    uint8_t ip_family;
    /** set if tech_name is available */
    bool tech_name_available;
    /** technology name according to the following values, see #QMI_WDS_technology_name_t */
    uint16_t tech_name;
    /** set if bearer_id is available */
    bool bearer_id_available;
    /** bearer id of the packet data connection */
    uint8_t bearer_id;
} wds_ind_session_state_t;

/**
 * @brief QMI WDS APN string change type
 *
 */
typedef struct {
    /** old apn string length */
    uint8_t old_apn_len;
    /** old APN string */
    char old_apn[MAX_STRING_SIZE];
    /** new apn string length */
    uint8_t new_apn_len;
    /** new APN string */
    char new_apn[MAX_STRING_SIZE];
} wds_ind_apn_string_info_t;

/**
 * @brief QMI WDS APN string change type
 *
 */
typedef struct {
    /** old PDP info, see #QMI_WDS_PDP_type_t */
    QMI_WDS_PDP_type_t old_pdp_type;
    /** new PDP info, see #QMI_WDS_PDP_type_t */
    QMI_WDS_PDP_type_t new_pdp_type;
} wds_ind_pdp_type_info_t;

/**
 * @brief QMI WDS APN info change type
 *
 */
typedef struct {
    /** set if apn_string_info is available */
    bool apn_string_info_available;
    /** Old and New APN String Information
     * Array containing old and new APN
     * string information. The first value of the
     * array points to an old APN string; the
     * second value of the array points to a new
     * APN string, see #wds_ind_apn_string_info_t */
    wds_ind_apn_string_info_t apn_string_info;
    /** set if pdp_type_info is available */
    bool pdp_type_info_available;
    /** Container to store the old and new PDN
     * IP type. The first value of the array
     * points to the old PDN IP type; the
     * second value of the array points to the
     * new PDN IP type, see #wds_ind_pdp_type_info_t */
    wds_ind_pdp_type_info_t pdp_type_info;
} wds_ind_apn_change_t;

/**
 * @brief Unpack wds indications
 *
 * This function unpacks the result of the following nas indications
 *
 * QMI_WDS_EVENT_REPORT_IND (1 - 0x0001)
 * QMI_WDS_PKT_SRVC_STATUS_IND (34 - 0x0022)
 * QMI_WDS_APN_PARAM_INFO_CHANGE_IND (185 - 0x00B9)
 *
 * @param[in] msg_id indication id
 * @param[in] rsp indication
 * @param[in] len indication length
 * @param[out] out type according to the following
 *      QMI_WDS_EVENT_REPORT_IND            -> #wds_ind_event_report_t
 *      QMI_WDS_PKT_SRVC_STATUS_IND         -> #wds_ind_session_state_t
 *      QMI_WDS_APN_PARAM_INFO_CHANGE_IND   -> #wds_ind_apn_change_t
 * @param[in] out_len buffer size
 */
QMI_SDK_error_t telit_wds_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len,
        uint8_t *out, uint16_t out_len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_WDS_H_ */
