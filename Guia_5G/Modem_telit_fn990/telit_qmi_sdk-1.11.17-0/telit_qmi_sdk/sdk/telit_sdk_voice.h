#ifndef TELIT_SDK_VOICE_H
#define TELIT_SDK_VOICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_voice.h
 * @brief VOICE pack/unpack functions
 * Service ID: QMI_SVC_VOICE (9 - 0x0009)
 */

/**
 * @brief Enum to describe QMI VOICE Message IDs
 */
typedef enum {
    /** 3 - 0x0003 Set the registration state for different QMI_VOICE indications */
    QMI_VOICE_INDICATION_REGISTER               = 0x0003,

    /** 32 - 0x0020 Originate a voice call */
    QMI_VOICE_DIAL_CALL                         = 0x0020,
    /** 33 - 0x0021 Ends a voice call */
    QMI_VOICE_END_CALL                          = 0x0021,
    /** 34 - 0x0022 Answer an incoming voice call */
    QMI_VOICE_ANSWER_CALL                       = 0x0022,

    /** 40 - 0x0028 Send a burst Dual-Tone Multifrequency (DTMF) */
    QMI_VOICE_BURST_DTMF                        = 0x0028,
    /** 41 - 0x0029 Start a continuous Dual-Tone Multifrequency (DTMF) */
    QMI_VOICE_START_CONT_DTMF                   = 0x0029,
    /** 42 - 0x002A Stop a continuous Dual-Tone Multifrequency (DTMF) */
    QMI_VOICE_STOP_CONT_DTMF                    = 0x002A,

    /** 46 - 0x002E Indicate a change in the call information */
    QMI_VOICE_ALL_CALL_STATUS_IND               = 0x002E,
    /** 47 - 0x002F Get all call info */
    QMI_VOICE_GET_ALL_CALL_INFO                 = 0x002F,

    /** 49 - 0x0031 Manage calls by using supplementary services */
    QMI_VOICE_MANAGE_CALLS                      = 0x0031,
    /** 50 - 0x0032 Used for supplementary service notifications */
    QMI_VOICE_SUPS_NOTIFICATION_IND             = 0x0032,
    /** 51 - 0x0033 Manage all call-independent supplementary services */
    QMI_VOICE_SET_SUPS_SERVICE                  = 0x0033,
    /** 52 - 0x0034 Query the status of call waiting supplementary service */
    QMI_VOICE_GET_CALL_WAITING                  = 0x0034,
    /** 53 - 0x0035 Query the status of call barring supplementary service */
    QMI_VOICE_GET_CALL_BARRING                  = 0x0035,
    /** 54 - 0x0036 Query the status of the calling line identification presentation supplementary service */
    QMI_VOICE_GET_CLIP                          = 0x0036,
    /** 55 - 0x0037 Query the status of the calling line identification restriction supplementary service */
    QMI_VOICE_GET_CLIR                          = 0x0037,
    /** 56 - 0x0038 Query the status of call forwarding supplementary service */
    QMI_VOICE_GET_CALL_FORWARDING               = 0x0038,
    /** 57 - 0x0039 Set a call barring password */
    QMI_VOICE_SET_CALL_BARRING_PASSWORD         = 0x0039,

    /** 66 - 0x0042 Notify clients about the modem-originated supplementary service requests and the responses received from the network */
    QMI_VOICE_SUPS_IND                          = 0x0042,

    /** 85 - 0x0055 Notify clients about conference information */
    QMI_VOICE_CONFERENCE_INFO_IND               = 0x0055,
    /** 86 - 0x0056 Notify clients about a new join in a conference */
    QMI_VOICE_CONFERENCE_JOIN_IND               = 0x0056,
    /** 87 - 0x0057 Notify clients about updated participants in a conference */
    QMI_VOICE_CONFERENCE_PARTICIPANT_UPDATE_IND = 0x0057,
} QMI_VOICE_message_t;

/**
 * @brief input type for QMI_VOICE_INDICATION_REGISTER
 *
 */
typedef struct {
    /** Action for DTMF Events */
    QMI_indication_action_t reg_dtmf_events;
    /** Action for Voice Privacy Events */
    QMI_indication_action_t reg_voice_privacy_events;
    /** Action for Supplementary Service Notification Events */
    QMI_indication_action_t supps_notification_events;
    /** Action for Call Notification Events */
    QMI_indication_action_t call_events;
    /** Action for Handover Events */
    QMI_indication_action_t handover_events;
    /** Action for Speech Codec Events */
    QMI_indication_action_t speech_events;
    /** Action for USSD Notification Events */
    QMI_indication_action_t ussd_notification_events;
    /** Action for Sups Events */
    QMI_indication_action_t sups_events;
    /** Action for Modification Events */
    QMI_indication_action_t modification_events;
    /** Action for UUS Events */
    QMI_indication_action_t uus_events;
    /** Action for AOC Events */
    QMI_indication_action_t aoc_events;
    /** Action for Conference Events */
    QMI_indication_action_t conference_events;
    /** Action for Extended Burst Type International Information Events */
    QMI_indication_action_t ext_brst_intl_events;
    /** Action for MT Page Miss Information Event */
    QMI_indication_action_t page_miss_events;
    /** Action for Call Control Result Information Event */
    QMI_indication_action_t cc_result_events;
    /** Action for Conference Participants Event */
    QMI_indication_action_t conf_participants_events;
    /** Action for TTY Info Events */
    QMI_indication_action_t tty_info_events;
    /** Action for E911 Call Origination Failure Events */
    QMI_indication_action_t orig_fail_events;
    /** Action for Videoshare Status Events */
    QMI_indication_action_t vs_status_events;
    /** Action for Audio RAT Change Events */
    QMI_indication_action_t audio_rat_change_events;
    /** Action for Additional Call Information Events */
    QMI_indication_action_t additional_call_info_events;
    /** Action for eCall Status Events */
    QMI_indication_action_t ecall_status_events;
    /** Action for Call Reestablishment Status */
    QMI_indication_action_t call_reestab_status_events;
    /** Action for VICE (VoLTE over Internet Connected Endpoint) Dialog Info */
    QMI_indication_action_t vice_dialog_event;
} voice_indications_t;

/**
 * @brief Pack QMI_VOICE_INDICATION_REGISTER (3 - 0x0003) request
 *
 * This function packs a qmi request to set the registration state for
 * different QMI_VOICE indications for the requesting control point.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #voice_indications_t
 */
QMI_SDK_error_t telit_voice_indication_register_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_indications_t *input);

/**
 * @brief Unpack QMI_VOICE_INDICATION_REGISTER (3 - 0x0003) request
 *
 * This function unpacks the result of a QMI_VOICE_INDICATION_REGISTER request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_voice_indication_register_unpack(uint8_t *rsp, uint16_t len);

#define QMI_VOICE_MAX_CALL_NUMBER_LEN 81

/**
 * @brief QMI VOICE CLIR type
 *
 */
typedef enum {
    /** Suppression */
    QMI_VOICE_CLIR_SUPPRESSION = 0x01,
    /** Invocation */
    QMI_VOICE_CLIR_INVOCATION  = 0x02,
} QMI_VOICE_CLIR_type_t;

/**
 * @brief input type for QMI_VOICE_DIAL_CALL request
 *
 */
typedef struct {
    /** Length of calling number */
    uint8_t calling_number_len;
    /** Number to be dialed in ASCII string. Length range: 1 to 81 */
    char calling_number[QMI_VOICE_MAX_CALL_NUMBER_LEN + 1];
    /** set if temporary CLIR should be used */
    bool clir_type_available;
    /** CLIR in temporary mode, see #QMI_VOICE_CLIR_type_t */
    enum8_t clir_type;
} voice_call_number_t;

/**
 * @brief QMI VOICE reject causes
 *
 */
typedef enum {
    /** User is busy */
    QMI_VOICE_REJECT_CAUSE_USER_BUSY     = 0x01,
    /** User has rejected the call */
    QMI_VOICE_REJECT_CAUSE_USER_REJECT   = 0x02,
    /** Call was rejected due to a low battery */
    QMI_VOICE_REJECT_CAUSE_LOW_BATTERY   = 0x03,
    /** Call was rejected because the number was blacklisted */
    QMI_VOICE_REJECT_CAUSE_BLACKLISTED   = 0x04,
    /** Call was rejected due to a dead battery */
    QMI_VOICE_REJECT_CAUSE_DEAD_BATTERY  = 0x05,
    /** Call was rejected because the call was an unwanted robo call */
    QMI_VOICE_REJECT_CAUSE_UNWANTED_CALL = 0x07,
} QMI_VOICE_reject_cause_t;

/**
 * @brief input and output type for QMI_VOICE_DIAL_CALL, QMI_VOICE_END_CALL and
 * QMI_VOICE_ANSWER_CALL request and response
 *
 */
typedef struct {
    /** set if optional call_id is available */
    bool call_id_available;
    /** Unique call identifier for the dialed call.
     * Mandatory for QMI_VOICE_END_CALL request */
    uint8_t call_id;
    /** set if  optional end_reason is available */
    bool end_reason_available;
    /** Cause for ending the call, see #QMI_VOICE_reject_cause_t */
    uint16_t end_reason;
} voice_call_t;

/**
 * @brief Pack QMI_VOICE_DIAL_CALL (32 - 0x0020) request
 *
 * This function packs a qmi request for originating a voice call
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #voice_call_number_t
 */
QMI_SDK_error_t telit_voice_dial_call_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_call_number_t *input);

/**
 * @brief Unpack QMI_VOICE_DIAL_CALL (32 - 0x0020) request
 *
 * This function unpacks the result of a QMI_VOICE_DIAL_CALL request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_call_t
 */
QMI_SDK_error_t telit_voice_dial_call_unpack(uint8_t *rsp, uint16_t len,
        voice_call_t *out);

/**
 * @brief Pack QMI_VOICE_END_CALL (33 - 0x0021) request
 *
 * This function packs a qmi request to end a voice call
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #voice_call_t
 */
QMI_SDK_error_t telit_voice_end_call_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_call_t *input);

/**
 * @brief Unpack QMI_VOICE_END_CALL (33 - 0x0021) request
 *
 * This function unpacks the result of a QMI_VOICE_END_CALL request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_call_t
 */
QMI_SDK_error_t telit_voice_end_call_unpack(uint8_t *rsp, uint16_t len,
        voice_call_t *out);

/**
 * @brief input type for QMI_VOICE_ANSWER_CALL request
 *
 */
typedef struct {
    /** Unique call identifier for the dialed call. */
    uint8_t call_id;
    /** Reject Incoming Call if true */
    bool reject_call;
    /** set if  optional end_reason is available */
    bool reject_cause_available;
    /** Cause for rejecting the incoming call, see #QMI_VOICE_reject_cause_t */
    enum32_t reject_cause;
} voice_answer_call_t;

/**
 * @brief Pack QMI_VOICE_ANSWER_CALL (34 - 0x0022) request
 *
 * This function packs a qmi request to answer a voice call
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #voice_answer_call_t
 */
QMI_SDK_error_t telit_voice_answer_call_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_answer_call_t *input);

/**
 * @brief Unpack QMI_VOICE_ANSWER_CALL (34 - 0x0022) request
 *
 * This function unpacks the result of a QMI_VOICE_ANSWER_CALL request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_call_t
 */
QMI_SDK_error_t telit_voice_answer_call_unpack(uint8_t *rsp, uint16_t len,
        voice_call_t *out);

#define QMI_VOICE_MAX_DTMF_BUFFER_SIZE 100

/**
 * @brief QMI VOICE DTMF pulse width values
 *
 */
typedef enum {
    /** 95ms */
    QMI_VOICE_DTMF_ONLENGTH_95MS  = 0x0,
    /** 150ms */
    QMI_VOICE_DTMF_ONLENGTH_150MS = 0x1,
    /** 200ms */
    QMI_VOICE_DTMF_ONLENGTH_200MS = 0x2,
    /** 250ms */
    QMI_VOICE_DTMF_ONLENGTH_250MS = 0x3,
    /** 300ms */
    QMI_VOICE_DTMF_ONLENGTH_300MS = 0x4,
    /** 350ms */
    QMI_VOICE_DTMF_ONLENGTH_350MS = 0x5,
    /** SMS Tx special pulse width */
    QMI_VOICE_DTMF_ONLENGTH_SMS   = 0x6,
} QMI_VOICE_DTMF_pulse_width_t;

/**
 * @brief QMI VOICE DTMF interdigit intervals
 *
 */
typedef enum {
    /** 60ms */
    QMI_VOICE_DTMF_OFFLENGTH_60MS  = 0x0,
    /** 100ms */
    QMI_VOICE_DTMF_OFFLENGTH_100MS = 0x1,
    /** 150ms */
    QMI_VOICE_DTMF_OFFLENGTH_150MS = 0x2,
    /** 200ms */
    QMI_VOICE_DTMF_OFFLENGTH_200MS = 0x3,
} QMI_VOICE_DTMF_interdigit_interval_t;

/**
 * @brief QMI VOICE dtmf lengths type
 *
 */
typedef struct {
    /** DTMF pulse width, see #QMI_VOICE_DTMF_pulse_width_t */
    enum8_t dtmf_onlength;
    /** DTMF interdigit interval, see #QMI_VOICE_DTMF_interdigit_interval_t */
    enum8_t dtmf_offlength;
} voice_dtmf_lengths_t;

/**
 * @brief input type for QMI_VOICE_BURST_DTMF request
 *
 */
typedef struct {
    /** Call ID associated with the current call */
    uint8_t call_id;
    /** len of digit_buffer */
    uint8_t digit_len;
    /** DTMF digit buffer in ASCII string */
    char digit[QMI_VOICE_MAX_DTMF_BUFFER_SIZE];
    /** set if optional dtmf_lengths is available */
    bool dtmf_lengths_available;
    /** see #voice_dtmf_lengths_t */
    voice_dtmf_lengths_t dtmf_lengths;
} voice_burst_dtmf_input_t;

/**
 * @brief output type for QMI_VOICE_BURST_DTMF request
 *
 */
typedef struct {
    /** set if  optional call_id is available */
    bool call_id_available;
    /** Call ID associated with the current call */
    uint8_t call_id;
} voice_dtmf_response_t;

/**
 * @brief Pack QMI_VOICE_BURST_DTMF (40 - 0x0028) request
 *
 * This function packs a qmi request to sends a burst Dual-Tone Multifrequency (DTMF)
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #voice_burst_dtmf_input_t
 */
QMI_SDK_error_t telit_voice_burst_dtmf_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_burst_dtmf_input_t *input);

/**
 * @brief Unpack QMI_VOICE_BURST_DTMF (40 - 0x0028) request
 *
 * This function unpacks the result of a QMI_VOICE_BURST_DTMF request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_dtmf_response_t
 */
QMI_SDK_error_t telit_voice_burst_dtmf_unpack(uint8_t *rsp, uint16_t len,
        voice_dtmf_response_t *out);

/**
 * @brief input type for QMI_VOICE_START_CONT_DTMF and QMI_VOICE_STOP_CONT_DTMF requests
 *
 */
typedef struct {
    /** Call ID associated with the current call, 0xFF for the active call */
    uint8_t call_id;
    /** DTMF digit in ASCII (needed only in QMI_VOICE_START_CONT_DTMF) */
    uint8_t digit;
} voice_cont_dtmf_input_t;

/**
 * @brief Pack QMI_VOICE_START_CONT_DTMF (41 - 0x0029) request
 *
 * This function packs a qmi request to start a continuous Dual-Tone Multifrequency (DTMF)
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #voice_cont_dtmf_input_t
 */
QMI_SDK_error_t telit_voice_start_cont_dtmf_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_cont_dtmf_input_t *input);

/**
 * @brief Unpack QMI_VOICE_START_CONT_DTMF (41 - 0x0029) request
 *
 * This function unpacks the result of a QMI_VOICE_START_CONT_DTMF request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_dtmf_response_t
 */
QMI_SDK_error_t telit_voice_start_cont_dtmf_unpack(uint8_t *rsp, uint16_t len,
        voice_dtmf_response_t *out);

/**
 * @brief Pack QMI_VOICE_STOP_CONT_DTMF (42 - 0x002A) request
 *
 * This function packs a qmi request to stop a continuous Dual-Tone Multifrequency (DTMF)
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #voice_cont_dtmf_input_t
 */
QMI_SDK_error_t telit_voice_stop_cont_dtmf_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_cont_dtmf_input_t *input);

/**
 * @brief Unpack QMI_VOICE_STOP_CONT_DTMF (42 - 0x002A) request
 *
 * This function unpacks the result of a QMI_VOICE_STOP_CONT_DTMF request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_dtmf_response_t
 */
QMI_SDK_error_t telit_voice_stop_cont_dtmf_unpack(uint8_t *rsp, uint16_t len,
        voice_dtmf_response_t *out);

/**
 * @brief Pack QMI_VOICE_GET_ALL_CALL_INFO (47 - 0x002F) request
 *
 * This function packs a qmi request for for getting info about all current calls
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_voice_get_all_call_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len);

/**
 * @brief QMI VOICE call states
 *
 */
typedef enum {
    /** Originating */
    QMI_VOICE_CALL_STATE_ORIGINATION    = 0x01,
    /** Incoming */
    QMI_VOICE_CALL_STATE_INCOMING       = 0x02,
    /** Conversation */
    QMI_VOICE_CALL_STATE_CONVERSATION   = 0x03,
    /** Call is originating, but waiting for call control to complete */
    QMI_VOICE_CALL_STATE_CC_IN_PROGRESS = 0x04,
    /** Alerting */
    QMI_VOICE_CALL_STATE_ALERTING       = 0x05,
    /** Hold */
    QMI_VOICE_CALL_STATE_HOLD           = 0x06,
    /** Waiting */
    QMI_VOICE_CALL_STATE_WAITING        = 0x07,
    /** Disconnecting */
    QMI_VOICE_CALL_STATE_DISCONNECTING  = 0x08,
    /** End */
    QMI_VOICE_CALL_STATE_END            = 0x09,
    /** MT call is in setup state in 3GPP */
    QMI_VOICE_CALL_STATE_SETUP          = 0x0A,
} QMI_VOICE_call_state_t;

/**
 * @brief QMI VOICE call directions
 *
 */
typedef enum {
    /** Mobile originated */
    QMI_VOICE_CALL_DIRECTION_MO = 0x01,
    /** Mobile terminated */
    QMI_VOICE_CALL_DIRECTION_MT = 0x02,
} QMI_VOICE_call_direction_t;

/**
 * @brief QMI VOICE call types
 *
 */
typedef enum {
    /** Voice */
    QMI_VOICE_CALL_TYPE_VOICE         = 0x00,
    /** Voice over IP */
    QMI_VOICE_CALL_TYPE_VOICE_IP      = 0x02,
    /** Videotelephony call over IP */
    QMI_VOICE_CALL_TYPE_VT            = 0x03,
    /** Videoshare */
    QMI_VOICE_CALL_TYPE_VIDEOSHARE    = 0x04,
    /** Test call type */
    QMI_VOICE_CALL_TYPE_TEST          = 0x05,
    /** OTAPA */
    QMI_VOICE_CALL_TYPE_OTAPA         = 0x06,
    /** OTASP */
    QMI_VOICE_CALL_TYPE_STD_OTASP     = 0x07,
    /** Nonstandard OTASP */
    QMI_VOICE_CALL_TYPE_NON_STD_OTASP = 0x08,
    /** Emergency */
    QMI_VOICE_CALL_TYPE_EMERGENCY     = 0x09,
    /** Supplementary service */
    QMI_VOICE_CALL_TYPE_SUPS          = 0x0A,
    /** Emergency VoIP */
    QMI_VOICE_CALL_TYPE_EMERGENCY_IP  = 0x0B,
    /** Emergency videotelephony call over IP */
    QMI_VOICE_CALL_TYPE_EMERGENCY_VT  = 0x0D,
} QMI_VOICE_call_type_t;

/**
 * @brief QMI VOICE call modes
 *
 */
typedef enum {
    /** No service */
    QMI_VOICE_CALL_MODE_NO_SRV  = 0x0,
    /** CDMA */
    QMI_VOICE_CALL_MODE_CDMA    = 0x1,
    /** GSM */
    QMI_VOICE_CALL_MODE_GSM     = 0x2,
    /** UMTS */
    QMI_VOICE_CALL_MODE_UMTS    = 0x3,
    /** LTE */
    QMI_VOICE_CALL_MODE_LTE     = 0x4,
    /** TD-SCDMA */
    QMI_VOICE_CALL_MODE_TDS     = 0x5,
    /** Unknown */
    QMI_VOICE_CALL_MODE_UNKNOWN = 0x6,
    /** WLAN */
    QMI_VOICE_CALL_MODE_WLAN    = 0x7,
    /** NR5G */
    QMI_VOICE_CALL_MODE_NR5G    = 0x8,
} QMI_VOICE_call_mode_t;

/**
 * @brief QMI VOICE call ALS lines
 *
 */
typedef enum {
    /** Line 1 (default) */
    QMI_VOICE_CALL_ALS_LINE1 = 0x0,
    /** Line 2 */
    QMI_VOICE_CALL_ALS_LINE2 = 0x1,
} QMI_VOICE_call_als_line_t;

/**
 * @brief QMI VOICE voice call info type
 *
 */
typedef struct {
    /** Unique call identifier for the call. */
    uint8_t call_id;
    /** Call state, see #QMI_VOICE_call_state_t */
    uint8_t call_state;
    /** Call type, see #QMI_VOICE_call_type_t */
    uint8_t call_type;
    /** Direction, see #QMI_VOICE_call_direction_t */
    uint8_t direction;
    /** Mode, see #QMI_VOICE_call_mode_t */
    uint8_t mode;
    /** Multiparty indicator. Values:
     * - 0x00 – FALSE
     * - 0x01 – TRUE */
    uint8_t is_mpty;
    /** ALS line indicator, see #QMI_VOICE_call_als_line_t */
    uint8_t als;
} voice_call_info_t;

/**
 * @brief QMI VOICE presentation indicators
 *
 */
typedef enum {
    /** Allowed presentation */
    QMI_VOICE_PRESENTATION_ALLOWED                           = 0x0,
    /** Restricted presentation */
    QMI_VOICE_PRESENTATION_RESTRICTED                        = 0x1,
    /** Unavailable presentation */
    QMI_VOICE_PRESENTATION_NUM_UNAVAILABLE                   = 0x2,
    /** Restricted name presentation */
    QMI_VOICE_PRESENTATION_NAME_NAME_PRESENTATION_RESTRICTED = 0x3,
    /** Payphone presentation (GSM/UMTS specific) */
    QMI_VOICE_PRESENTATION_PAYPHONE                          = 0x4,
} QMI_VOICE_presentation_indicator_t;

/**
 * @brief QMI VOICE Array Remote Party number type
 */
typedef struct {
    /** Unique call identifier for the dialed call. */
    uint8_t call_id;
    /** Presentation indicator, see #QMI_VOICE_presentation_indicator_t */
    uint8_t number_pi;
    /** len of the following number string */
    uint8_t number_len;
    /** Remote party number in ASCII characters */
    char number[MAX_STRING_SIZE];
} voice_array_remote_party_number_t;

/**
 * @brief QMI VOICE Array Remote Party name type
 */
typedef struct {
    /** Unique call identifier for the call. */
    uint8_t call_id;
    /** Name presentation indicator, QMI_VOICE_presentation_indicator_t */
    uint8_t name_pi;
    /** Refer to 3GPP TS 23.038 Section 5 for coding schemes */
    uint8_t coding;
    /** len of the following name string */
    uint8_t name_len;
    /** Caller name per the coding scheme */
    char name[MAX_STRING_SIZE];
} voice_array_remote_party_name_t;

/**
 * @brief QMI VOICE UUS types
 *
 */
typedef enum {
    /** Data */
    QMI_VOICE_UUS_TYPE_DATA          = 0x0,
    /** Type 1 implicit */
    QMI_VOICE_UUS_TYPE1_IMPLICIT     = 0x1,
    /** Type 1 required */
    QMI_VOICE_UUS_TYPE1_REQUIRED     = 0x2,
    /** Type 1 not required */
    QMI_VOICE_UUS_TYPE1_NOT_REQUIRED = 0x3,
    /** Type 2 required */
    QMI_VOICE_UUS_TYPE2_REQUIRED     = 0x4,
    /** Type 2 not required */
    QMI_VOICE_UUS_TYPE2_NOT_REQUIRED = 0x5,
    /** Type 3 required */
    QMI_VOICE_UUS_TYPE3_REQUIRED     = 0x6,
    /** Type 3 not required */
    QMI_VOICE_UUS_TYPE3_NOT_REQUIRED = 0x7,
} QMI_VOICE_uus_type_t;

/**
 * @brief QMI VOICE UUS data coding schemes
 *
 */
typedef enum {
    /** USP */
    QMI_VOICE_UUS_DCS_USP       = 0x1,
    /** OHLP */
    QMI_VOICE_UUS_DCS_OHLP      = 0x2,
    /** X244 */
    QMI_VOICE_UUS_DCS_X244      = 0x3,
    /** SMCF */
    QMI_VOICE_UUS_DCS_SMCF      = 0x4,
    /** IA5 */
    QMI_VOICE_UUS_DCS_IA5       = 0x5,
    /** RV12RD */
    QMI_VOICE_UUS_DCS_RV12RD    = 0x6,
    /** Q931UNCCM */
    QMI_VOICE_UUS_DCS_Q931UNCCM = 0x7,
} QMI_VOICE_uus_dcs_t;

/**
 * @brief QMI VOICE connected number screening indicators
 *
 */
typedef enum {
    /** Provided user is not screened */
    QMI_VOICE_SI_USER_PROVIDED_NOT_SCREENED    = 0x0,
    /** Provided user passed verification */
    QMI_VOICE_SI_USER_PROVIDED_VERIFIED_PASSED = 0x1,
    /** Provided user failed verification */
    QMI_VOICE_SI_USER_PROVIDED_VERIFIED_FAILED = 0x2,
    /** Provided network */
    QMI_VOICE_SI_NETWORK_PROVIDED              = 0x3,
} QMI_VOICE_screening_indicator_t;

/**
 * @brief QMI VOICE UUS Information type
 */
typedef struct {
    /** Unique call identifier for the call. */
    uint8_t call_id;
    /** UUS type, see #QMI_VOICE_uus_type_t */
    uint8_t uus_type;

    /** UUS data coding scheme, see #QMI_VOICE_uus_dcs_t */
    uint8_t uus_dcs;
    /** len of the uus_data string */
    uint8_t uus_data_len;
    /** UUS data encoded as per coding scheme */
    uint8_t uus_data[MAX_STRING_SIZE];
} voice_array_uus_info_t;

/**
 * @brief QMI VOICE Connected Party Number type
 */
typedef struct {
    /** Unique call identifier for the call. */
    uint8_t call_id;
    /** Presentation indicator; refer to 3GPP2 C.S0005-D Table 2.7.4.4-1 for valid values */
    uint8_t conn_num_pi;
    /** Connected number screening indicator, see #QMI_VOICE_screening_indicator_t */
    uint8_t conn_num_si;
    /** Connected number type, see #QMI_VOICE_number_type_t */
    uint8_t conn_num_type;
    /** Connected number plan, see #QMI_VOICE_number_type_t */
    uint8_t conn_num_plan;
    /** Length of the connected number */
    uint8_t conn_num_len;
    /** Connected number in ASCII characters */
    char conn_num[MAX_STRING_SIZE];
} voice_array_conn_party_number_t;

#define QMI_VOICE_MAX_CALL_INFO 8

/**
 * @brief output type for QMI_VOICE_GET_ALL_CALL_INFO request
 *
 */
typedef struct {
    /** number of valid #voice_call_info_t instances */
    uint8_t call_info_size;
    /** Call Information */
    voice_call_info_t call_info[QMI_VOICE_MAX_CALL_INFO];
    /** number of valid #voice_array_remote_party_number_t instances */
    uint8_t remote_party_number_size;
    voice_array_remote_party_number_t remote_party_number[QMI_VOICE_MAX_CALL_INFO]; /* remote party number info */
    /** number of valid #voice_array_remote_party_name_t instances */
    uint8_t remote_party_name_size;
    voice_array_remote_party_name_t remote_party_name[QMI_VOICE_MAX_CALL_INFO];     /* remote party name info */
    /** number of valid #voice_array_uus_info_t instances */
    uint8_t array_uus_size;
    voice_array_uus_info_t array_uus[QMI_VOICE_MAX_CALL_INFO];                      /* array UUS info */
    /** number of valid #voice_array_conn_party_number_t instances */
    uint8_t conn_party_number_size;
    voice_array_conn_party_number_t conn_party_number[QMI_VOICE_MAX_CALL_INFO];     /* conn party number info */
} voice_get_all_call_info_t;

/**
 * @brief Unpack QMI_VOICE_GET_ALL_CALL_INFO (47 - 0x002F) request
 *
 * This function unpacks the result of a QMI_VOICE_GET_ALL_CALL_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_get_all_call_info_t
 */
QMI_SDK_error_t telit_voice_get_all_call_info_unpack(uint8_t *rsp,
        uint16_t len,
        voice_get_all_call_info_t *out);

/**
 * @brief QMI VOICE supplementary service types
 *
 */
typedef enum {
    /** Release is held or waiting */
    QMI_VOICE_SUPS_TYPE_RELEASE_HELD_OR_WAITING               = 0x01,
    /** Release is active and accepting held or waiting */
    QMI_VOICE_SUPS_TYPE_RELEASE_ACTIVE_ACCEPT_HELD_OR_WAITING = 0x02,
    /** Hold is active and accepting waiting or held */
    QMI_VOICE_SUPS_TYPE_HOLD_ACTIVE_ACCEPT_WAITING_OR_HELD    = 0x03,
    /** Hold all calls except a specified one */
    QMI_VOICE_SUPS_TYPE_HOLD_ALL_EXCEPT_SPECIFIED_CALL        = 0x04,
    /** Make a conference call */
    QMI_VOICE_SUPS_TYPE_MAKE_CONFERENCE_CALL                  = 0x05,
    /** Explicit call transfer */
    QMI_VOICE_SUPS_TYPE_EXPLICIT_CALL_TRANSFER                = 0x06,
    /** Activate completion of calls to busy subscriber */
    QMI_VOICE_SUPS_TYPE_CCBS_ACTIVATION                       = 0x07,
    /** End all calls */
    QMI_VOICE_SUPS_TYPE_END_ALL_CALLS                         = 0x08,
    /** Release a specified call */
    QMI_VOICE_SUPS_TYPE_RELEASE_SPECIFIED_CALL                = 0x09,
    /** Put all active calls on local hold */
    QMI_VOICE_SUPS_TYPE_LOCAL_HOLD                            = 0x0A,
    /** Retrieve locally held calls */
    QMI_VOICE_SUPS_TYPE_LOCAL_UNHOLD                          = 0x0B,
} QMI_VOICE_sups_type_t;

/**
 * @brief input type for QMI_VOICE_MANAGE_CALLS request
 *
 */
typedef struct {
    /** Supplementary service type during the call, see #QMI_VOICE_sups_type_t */
    uint8_t sups_type;
    /** set if optional call_id is available */
    bool call_id_available;
    /** Applicable only for sups_type 0x04, 0x07, and 0x09 */
    uint8_t call_id;
    /** set if optional reject_cause is available */
    bool reject_cause_available;
    /** Cause for rejecting the call, see #QMI_VOICE_reject_cause_t */
    uint32_t reject_cause;
} voice_manage_call_request_t;

/**
 * @brief output type for QMI_VOICE_MANAGE_CALLS request
 *
 */
typedef struct {
    /** set if optional failure_cause is available */
    bool failure_cause_available;
    /** Supplementary services failure cause see table A-3 in Qualcomm voice docs */
    uint16_t failure_cause;
} voice_manage_call_response_t;

/**
 * @brief Pack QMI_VOICE_MANAGE_CALLS (49 - 0x0031) request
 *
 * This function manages the calls by using the supplementary
 * service applicable during the call (applicable only for 3GPP)
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #voice_manage_call_request_t
 */
QMI_SDK_error_t telit_voice_manage_calls_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_manage_call_request_t *input);

/**
 * @brief Unpack QMI_VOICE_MANAGE_CALLS (49 - 0x0031) request
 *
 * This function unpacks the result of a QMI_VOICE_MANAGE_CALLS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_manage_call_response_t
 */
QMI_SDK_error_t telit_voice_manage_calls_unpack(uint8_t *rsp, uint16_t len,
        voice_manage_call_response_t *out);

#define QMI_VOICE_BARRING_PASSWORD_MAX_LEN 5
#define QMI_VOICE_BARRING_NUMBERS_LIST_MAX_SIZE 10
#define QMI_VOICE_BARRING_NUMBER_MAX_LEN 82

/**
 * @brief QMI VOICE call barring number type
 *
 */
typedef struct {
    /** Service status, see #QMI_VOICE_service_status_t, only for QMI_VOICE_GET_CALL_BARRING */
    enum8_t active_status;
    /** Length of barred number */
    uint8_t number_len;
    /** Call barring number as an ASCII string.
     * Length range: 1 to 81 */
    char number[QMI_VOICE_BARRING_NUMBER_MAX_LEN];
} voice_call_barring_number_t;

/**
 * @brief QMI VOICE voice call forwarding time type
 *
 */
typedef struct {
    /** Year */
    uint16_t year;
    /** Month. Range: 1 to 12. 1 is January and 12 is December */
    uint8_t month;
    /** Day. Range: 1 to 31 */
    uint8_t day;
    /** Hour. Range: 0 to 23 */
    uint8_t hour;
    /** Minute. Range: 0 to 59 */
    uint8_t minute;
    /** Second. Range: 0 to 59 */
    uint8_t second;
    /** Time zone. Offset from Universal time, i.e., the difference
     * between local time and Universal time,
     * in increments of 15 min (signed value). */
    int8_t time_zone;
} voice_call_forwarding_time_t;

/**
 * @brief QMI VOICE number types
 *
 */
typedef enum {
    /** Unknown */
    QMI_VOICE_NUM_TYPE_UNKNOWN            = 0x0,
    /** International */
    QMI_VOICE_NUM_TYPE_INTERNATIONAL      = 0x1,
    /** National */
    QMI_VOICE_NUM_TYPE_NATIONAL           = 0x2,
    /** Network-speciﬁc */
    QMI_VOICE_NUM_TYPE_NETWORK_SPECIFIC   = 0x3,
    /** Subscriber */
    QMI_VOICE_NUM_TYPE_SUBSCRIBER         = 0x4,
    /** Reserved */
    QMI_VOICE_NUM_TYPE_RESERVED           = 0x5,
    /** Abbreviated */
    QMI_VOICE_NUM_TYPE_ABBREVIATED        = 0x6,
    /** Reserved extension */
    QMI_VOICE_NUM_TYPE_RESERVED_EXTENSION = 0x7,
} QMI_VOICE_number_type_t;

/**
 * @brief QMI VOICE number plans
 *
 */
typedef enum {
    /** Unknown */
    QMI_VOICE_NUM_PLAN_UNKNOWN            = 0x0,
    /** ISDN */
    QMI_VOICE_NUM_PLAN_ISDN               = 0x1,
    /** Data */
    QMI_VOICE_NUM_PLAN_DATA               = 0x3,
    /** Telex */
    QMI_VOICE_NUM_PLAN_TELEX              = 0x4,
    /** National */
    QMI_VOICE_NUM_PLAN_NATIONAL           = 0x8,
    /** Private */
    QMI_VOICE_NUM_PLAN_PRIVATE            = 0x9,
    /** Reserved cordless telephony system */
    QMI_VOICE_NUM_PLAN_RESERVED_CTS       = 0xB,
    /** Reserved extension */
    QMI_VOICE_NUM_PLAN_RESERVED_EXTENSION = 0xF,
} QMI_VOICE_number_plan_t;

/**
 * @brief QMI VOICE number type and plan type
 *
 */
typedef struct {
    /** Number type, see #QMI_VOICE_number_type_t */
    enum8_t num_type;
    /** Number plan, see #QMI_VOICE_number_plan_t */
    enum8_t num_plan;
} voice_call_num_type_and_plan_t;

/**
 * @brief QMI VOICE supplementary service operations
 *
 */
typedef enum {
    /** Activate */
    QMI_VOICE_SERVICE_ACTIVATE          = 0x1,
    /** Deactivate */
    QMI_VOICE_SERVICE_DEACTIVATE        = 0x2,
    /** Register */
    QMI_VOICE_SERVICE_REGISTER          = 0x3,
    /** Erase */
    QMI_VOICE_SERVICE_ERASE             = 0x4,
    /** Interrogate */
    QMI_VOICE_SERVICE_INTERROGATE       = 0x5,
    /** Register password */
    QMI_VOICE_SERVICE_REGISTER_PASSWORD = 0x6,
    /** USSD */
    QMI_VOICE_SERVICE_USSD              = 0x7,
} QMI_VOICE_service_operation_t;

/**
 * @brief QMI VOICE COLR status values
 *
 */
typedef enum {
    /** COLR presentation is not restricted */
    QMI_VOICE_COLR_PRESENTATION_NOT_RESTRICTED = 0x0,
    /** COLR presentation is restricted */
    QMI_VOICE_COLR_PRESENTATION_RESTRICTED     = 0x1,
} QMI_VOICE_COLR_status_t;

/**
 * @brief QMI VOICE supplementary services reasons
 *
 */
typedef enum {
    /** Unconditional call forwarding */
    QMI_VOICE_REASON_FWDREASON_UNCONDITIONAL  = 0x01,
    /** Forward when the mobile is busy */
    QMI_VOICE_REASON_FWDREASON_MOBILEBUSY     = 0x02,
    /** Forward when there is no reply */
    QMI_VOICE_REASON_FWDREASON_NOREPLY        = 0x03,
    /** Forward when the call is unreachable */
    QMI_VOICE_REASON_FWDREASON_UNREACHABLE    = 0x04,
    /** All forwarding */
    QMI_VOICE_REASON_FWDREASON_ALLFORWARDING  = 0x05,
    /** All conditional forwarding */
    QMI_VOICE_REASON_FWDREASON_ALLCONDITIONAL = 0x06,
    /** Barr all outgoing */
    QMI_VOICE_REASON_BARR_ALLOUTGOING         = 0x07,
    /** Barr outgoing international */
    QMI_VOICE_REASON_BARR_OUTGOINGINT         = 0x08,
    /** Barr outgoing external to home */
    QMI_VOICE_REASON_BARR_OUTGOINGINTEXTOHOME = 0x09,
    /** Barr all incoming */
    QMI_VOICE_REASON_BARR_ALLINCOMING         = 0x0A,
    /** Barr roaming incoming */
    QMI_VOICE_REASON_BARR_INCOMINGROAMING     = 0x0B,
    /** Barr all calls */
    QMI_VOICE_REASON_BARR_ALLBARRING          = 0x0C,
    /** Barr all outgoing calls */
    QMI_VOICE_REASON_BARR_ALLOUTGOINGBARRING  = 0x0D,
    /** Barr all incoming calls */
    QMI_VOICE_REASON_BARR_ALLINCOMINGBARRING  = 0x0E,
    /** Call waiting */
    QMI_VOICE_REASON_CALLWAITING              = 0x0F,
    /** Calling line identiﬁcation presentation */
    QMI_VOICE_REASON_CLIP                     = 0x10,
    /** Connected line identiﬁcation presentation */
    QMI_VOICE_REASON_COLP                     = 0x12,
    /** Connected line identiﬁcation restriction */
    QMI_VOICE_REASON_COLR                     = 0x13,
    /** Calling name presentation */
    QMI_VOICE_REASON_CNAP                     = 0x14,
    /** Barr incoming calls from registered and activated numbers */
    QMI_VOICE_REASON_BARR_INCOMINGNUMBER      = 0x15,
    /** Barr incoming calls from anonymous numbers */
    QMI_VOICE_REASON_BARR_INCOMINGANONYMOUS   = 0x16,
} QMI_VOICE_sups_service_reason_t;

/**
 * @brief input type for QMI_VOICE_SET_SUPS_SERVICE request
 *
 */
typedef struct {
    /** Service, see #QMI_VOICE_service_operation_t */
    enum8_t voice_service;
    /** Reason, see #QMI_VOICE_sups_service_reason_t */
    enum8_t reason;
    /** set if  optional service_class is available */
    bool service_class_available;
    /** Service class, see #QMI_VOICE_service_class_t */
    uint8_t service_class;
    /** Length of password string */
    uint8_t password_len;
    /** Call Barring Password. Is required if call barring is provisioned using a password.
     * Password consists of 4 ASCII digits. Range: 0000 to 999 */
    char password[QMI_VOICE_BARRING_PASSWORD_MAX_LEN];
    /** Length of number string */
    uint8_t number_len;
    /** Call forwarding number to be registered with
     * the network; ASCII string */
    char number[MAX_STRING_SIZE];
    /** set if  optional timer_value is available */
    bool timer_value_available;
    /** Call Forwarding No Reply Timer in seconds */
    uint8_t timer_value;
    /** set if  optional num_type_and_plan is available */
    bool num_type_and_plan_available;
    /** Call Forwarding Number Type and Plan */
    voice_call_num_type_and_plan_t num_type_and_plan;
    /** set if  optional service_class_ext is available */
    bool service_class_ext_available;
    /** Extended service class; see Table A-7 in Voice service spec for more information. */
    enum32_t service_class_ext;
    /** Size of call_barring_numbers_list */
    uint8_t call_barring_numbers_list_size;
    /** List of call barring numbers
     * to be activated/deactivated or registered
     * with/erased from the network */
    voice_call_barring_number_t call_barring_numbers_list[QMI_VOICE_BARRING_NUMBERS_LIST_MAX_SIZE];
    /** set if  optional colr_pi is available */
    bool colr_pi_available;
    /** COLR Presentation Information, see #QMI_VOICE_COLR_status_t */
    enum32_t colr_pi;
    /** set if  optional call_forwarding_start_time is available */
    bool call_forwarding_start_time_available;
    /** Call Forwarding Start Time */
    voice_call_forwarding_time_t call_forwarding_start_time;
    /** set if  optional call_forwarding_end_time is available */
    bool call_forwarding_end_time_available;
    /** Call Forwarding end Time */
    voice_call_forwarding_time_t call_forwarding_end_time;
} voice_set_sup_service_input_t;

/**
 * @brief output type for QMI_VOICE_SET_SUPS_SERVICE
 *
 */
typedef struct {
    /** set if optional failure_cause is available */
    bool failure_cause_available;
    /** Supplementary services failure cause see table A-3 in Qualcomm voice docs */
    enum16_t failure_cause;
    /** set if  optional call_id is available */
    bool call_id_available;
    /** Call ID of the voice call that resulted from call
     * control; ID is present when cc_result_type is
     * present and is Voice */
    uint8_t call_id;
} voice_set_sup_service_response_t;

/**
 * @brief Pack QMI_VOICE_SET_SUPS_SERVICE (51 - 0x0033) request
 *
 * This function packs a qmi request to manage
 * all call-independent supplementary services, such as activation,
 * deactivation, registration, and erasure.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #voice_set_sup_service_input_t
 */
QMI_SDK_error_t telit_voice_set_sups_service_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_set_sup_service_input_t *input);

/**
 * @brief Unpack QMI_VOICE_SET_SUPS_SERVICE (51 - 0x0033) request
 *
 * This function unpacks the result of a QMI_VOICE_SET_SUPS_SERVICE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_set_sup_service_response_t
 */
QMI_SDK_error_t telit_voice_set_sups_service_unpack(uint8_t *rsp, uint16_t len,
        voice_set_sup_service_response_t *out);

/**
 * @brief input type for QMI_VOICE_GET_CALL_WAITING, QMI_VOICE_GET_CALL_BARRING and QMI_GET_CALL_FORWARDING
 *
 */
typedef struct {
    /** Reason, see #QMI_VOICE_sups_service_reason_t, not used in QMI_VOICE_GET_CALL_WAITING */
    uint8_t reason;
    /** set if  optional service_class is available */
    bool service_class_available;
    /** Service class, see #QMI_VOICE_service_class_t */
    uint8_t service_class;
    /** set if optional service_class_ext is available */
    bool service_class_ext_available;
    /** Extended service class; see Table A-7 in Voice service spec for more information. */
    enum32_t service_class_ext;
} voice_get_call_service_input_t;

/**
 * @brief output type for QMI_VOICE_GET_CALL_WAITING request
 *
 */
typedef struct {
    /** set if optional service_class is available */
    bool service_class_available;
    /** Service class, see #QMI_VOICE_service_class_t */
    uint8_t service_class;
    /** set if optional failure_cause is available */
    bool failure_cause_available;
    /** Supplementary services failure cause see table A-3 in Qualcomm voice docs */
    enum16_t failure_cause;
    /** set if  optional call_id is available */
    bool call_id_available;
    /** Call ID of the voice call that resulted from call
     * control; ID is present when cc_result_type is
     * present and is Voice */
    uint8_t call_id;
} voice_get_call_waiting_response_t;

/**
 * @brief Pack QMI_VOICE_GET_CALL_WAITING (52 - 0x0034) request
 *
 * This function packs a qmi request to query the status
 * of call waiting supplementary service
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #voice_get_call_service_input_t
 */
QMI_SDK_error_t telit_voice_get_call_waiting_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_get_call_service_input_t *input);

/**
 * @brief Unpack QMI_VOICE_GET_CALL_WAITING (52 - 0x0034) request
 *
 * This function unpacks the result of a QMI_VOICE_GET_CALL_WAITING request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_get_call_waiting_response_t
 */
QMI_SDK_error_t telit_voice_get_call_waiting_unpack(uint8_t *rsp, uint16_t len,
        voice_get_call_waiting_response_t *out);

/**
 * @brief Pack QMI_VOICE_GET_CALL_BARRING (53 - 0x0035) request
 *
 * This function packs a qmi request to query the status
 * of call barring supplementary service
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #voice_get_call_service_input_t
 */
QMI_SDK_error_t telit_voice_get_call_barring_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_get_call_service_input_t *input);

#define QMI_VOICE_MAX_CALL_BARRING_NUMBER_LIST 10

/**
 * @brief QMI VOICE call barring service
 *
 */
typedef struct {
    /** Extended service class, see table A-7 in Qualcomm voice docs */
    enum32_t service_class_ext;
    /** Size of call_barring_number_list */
    uint8_t call_barring_number_list_size;
    /** Call barring numbers list */
    voice_call_barring_number_t call_barring_number_list[QMI_VOICE_MAX_CALL_BARRING_NUMBER_LIST];
} voice_call_barring_service_t;

#define QMI_VOICE_MAX_CALL_BARRING_SERVICE 10

/**
 * @brief output type for QMI_VOICE_GET_CALL_BARRING and QMI_VOICE_SET_CALL_BARRING_PASSWORD requests
 *
 */
typedef struct {
    /** set if  optional service_class is available */
    bool service_class_available;
    /** Service class, see #QMI_VOICE_service_class_t */
    uint8_t service_class;
    /** set if optional failure_cause is available */
    bool failure_cause_available;
    /** Supplementary services failure cause see table A-3 in Qualcomm voice docs */
    enum16_t failure_cause;
    /** set if  optional call_id is available */
    bool call_id_available;
    /** Call ID of the voice call that resulted from call
     * control; ID is present when cc_result_type is
     * present and is Voice */
    uint8_t call_id;
    /** Size of call_barring_service */
    uint8_t call_barring_service_size;
    /** Array of #voice_call_barring_service_t instances */
    voice_call_barring_service_t call_barring_service[QMI_VOICE_MAX_CALL_BARRING_SERVICE];
} voice_call_barring_t;

/**
 * @brief Unpack QMI_VOICE_GET_CALL_BARRING (53 - 0x0035) request
 *
 * This function unpacks the result of a QMI_VOICE_GET_CALL_BARRING request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_call_barring_t
 */
QMI_SDK_error_t telit_voice_get_call_barring_unpack(uint8_t *rsp, uint16_t len,
        voice_call_barring_t *out);

/**
 * @brief QMI VOICE service provision status
 *
 */
typedef enum {
    /** Not provisioned */
    QMI_VOICE_PROVISION_STATUS_NOT_PROVISIONED         = 0x00,
    /** Permanently provisioned */
    QMI_VOICE_PROVISION_STATUS_PROVISIONED_PERMANENT   = 0x01,
    /** Restricted presentation */
    QMI_VOICE_PROVISION_STATUS_PRESENTATION_RESTRICTED = 0x02,
    /** Allowed presentation */
    QMI_VOICE_PROVISION_STATUS_PRESENTATION_ALLOWED    = 0x03,
} QMI_VOICE_service_provision_status_t;

/**
 * @brief QMI VOICE provision status type
 *
 */
typedef struct {
    /** Service status, see #QMI_VOICE_service_status_t */
    enum8_t active_status;
    /** Provision status, see #QMI_VOICE_service_provision_status_t */
    enum8_t provision_status;
} voice_provision_status_t;

/**
 * @brief output type for QMI_VOICE_GET_CLIP and QMI_VOICE_GET_CLIR request
 *
 */
typedef struct {
    /** set if optional calling line response is available */
    bool cl_response_available;
    /** Calling line response, see #voice_provision_status_t */
    voice_provision_status_t cl_response;
    /** set if optional failure_cause is available */
    bool failure_cause_available;
    /** Supplementary services failure cause see table A-3 in Qualcomm voice docs */
    enum16_t failure_cause;
} voice_get_calling_line_response_t;

/**
 * @brief Pack QMI_VOICE_GET_CLIP (54 - 0x0036) request
 *
 * This function packs a qmi request to query the status of
 * the calling line identification presentation supplementary service
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_voice_get_clip_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_VOICE_GET_CLIP (54 - 0x0036) request
 *
 * This function unpacks the result of a QMI_VOICE_GET_CLIP request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_get_calling_line_response_t
 */
QMI_SDK_error_t telit_voice_get_clip_unpack(uint8_t *rsp, uint16_t len,
        voice_get_calling_line_response_t *out);

/**
 * @brief Pack QMI_VOICE_GET_CLIR (55 - 0x0037) request
 *
 * This function packs a qmi request to query the status of
 * the calling line identification restriction supplementary service
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_voice_get_clir_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_VOICE_GET_CLIR (55 - 0x0037) request
 *
 * This function unpacks the result of a QMI_VOICE_GET_CLIR request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_get_calling_line_response_t
 */
QMI_SDK_error_t telit_voice_get_clir_unpack(uint8_t *rsp, uint16_t len,
        voice_get_calling_line_response_t *out);

/**
 * @brief QMI VOICE supplementary service status values
 *
 */
typedef enum {
    /** Inactive */
    QMI_VOICE_ACTIVE_STATUS_INACTIVE = 0x00,
    /** Active */
    QMI_VOICE_ACTIVE_STATUS_ACTIVE   = 0x01,
} QMI_VOICE_service_status_t;

/**
 * @brief QMI VOICE call service classes
 *
 */
typedef enum {
    /** None */
    QMI_VOICE_CLASS_NONE               = 0x00,
    /** Voice */
    QMI_VOICE_CLASS_VOICE              = 0x01,
    /** Data */
    QMI_VOICE_CLASS_DATA               = 0x02,
    /** Fax */
    QMI_VOICE_CLASS_FAX                = 0x04,
    /** SMS */
    QMI_VOICE_CLASS_SMS                = 0x08,
    /** Data circuit sync */
    QMI_VOICE_CLASS_DATA_CIRCUIT_SYNC  = 0x10,
    /** Data circuit async */
    QMI_VOICE_CLASS_DATA_CIRCUIT_ASYNC = 0x20,
    /** Packet access */
    QMI_VOICE_CLASS_PACKET_ACCESS      = 0x40,
    /** Pad access */
    QMI_VOICE_CLASS_PAD_ACCESS         = 0x80,
} QMI_VOICE_service_class_t;

/**
 * @brief QMI VOICE call forwarding type
 *
 */
typedef struct {
    /** Service status, see #QMI_VOICE_service_status_t */
    enum8_t service_status;
    /** Service Class, see #QMI_VOICE_service_class_t */
    uint8_t service_class;
    /** length of number string */
    uint8_t number_len;
    /** Call forwarding number in ASCII characters */
    char number[MAX_STRING_SIZE];
    /** No reply timer value in seconds; a value of 0
     * indicates that no_reply_timer is ignored. */
    uint8_t no_reply_timer;
} voice_call_forwarding_info_t;

#define QMI_VOICE_MAX_CALL_FW_INFO 10

/**
 * @brief output type for QMI_VOICE_GET_CALL_FORWARDING request
 *
 */
typedef struct {
    /** Size of call_forwarding_info */
    uint8_t call_forwarding_info_size;
    /** Array of #voice_call_forwarding_info_t instances */
    voice_call_forwarding_info_t call_forwarding_info[QMI_VOICE_MAX_CALL_FW_INFO];
    /** set if optional failure_cause is available */
    bool failure_cause_available;
    /** Supplementary services failure cause see table A-3 in Qualcomm voice docs */
    enum16_t failure_cause;
    /** set if  optional call_id is available */
    bool call_id_available;
    /** Call ID of the voice call that resulted from call
     * control; ID is present when cc_result_type is
     * present and is Voice */
    uint8_t call_id;
} voice_call_forwarding_t;

/**
 * @brief Pack QMI_VOICE_GET_CALL_FORWARDING (56 - 0x0038) request
 *
 * This function packs a qmi request to query the status of
 * call forwarding supplementary service
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #voice_get_call_service_input_t
 */
QMI_SDK_error_t telit_voice_get_call_forwarding_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_get_call_service_input_t *input);

/**
 * @brief Unpack QMI_VOICE_GET_CALL_FORWARDING (56 - 0x0038) request
 *
 * This function unpacks the result of a QMI_VOICE_GET_CALL_FORWARDING request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_call_forwarding_t
 */
QMI_SDK_error_t telit_voice_get_call_forwarding_unpack(uint8_t *rsp, uint16_t len,
        voice_call_forwarding_t *out);

#define CALL_BARRING_PASSWORD_LEN 4

/**
 * @brief input type for QMI_VOICE_SET_CALL_BARRING_PASSWORD request
 *
 */
typedef struct {
    /** Reason, see #QMI_VOICE_sups_service_reason_t */
    enum8_t reason;
    /** Old password, consisting of 4 ASCII digits NULL-terminated */
    char old_password[CALL_BARRING_PASSWORD_LEN + 1];
    /** New password, consisting of 4 ASCII digits NULL-terminated */
    char new_password[CALL_BARRING_PASSWORD_LEN + 1];
} voice_call_barring_input_t;

/**
 * @brief Pack QMI_VOICE_SET_CALL_BARRING_PASSWORD (57 - 0x0039) request
 *
 * This function packs a qmi request to set a call barring password
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #voice_call_barring_input_t
 */
QMI_SDK_error_t telit_voice_set_call_barring_password_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_call_barring_input_t *input);

/**
 * @brief Unpack QMI_VOICE_SET_CALL_BARRING_PASSWORD (57 - 0x0039) request
 *
 * This function unpacks the result of a QMI_VOICE_SET_CALL_BARRING_PASSWORD request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #voice_call_barring_t
 */
QMI_SDK_error_t telit_voice_set_call_barring_password_unpack(uint8_t *rsp, uint16_t len,
        voice_call_barring_t *out);

/**
 * @brief output type for QMI_VOICE_ALL_CALL_STATUS_IND
 *
 */
typedef struct {
    /** number of valid #voice_call_info_t instances */
    uint8_t call_info_size;
    /** Call Information */
    voice_call_info_t call_info[QMI_VOICE_MAX_CALL_INFO];
} voice_all_call_status_ind_t;

#define QMI_VOICE_MAX_CONF_XML_LEN 2048

/**
 * @brief output type for QMI_VOICE_CONFERENCE_INFO_IND
 *
 */
typedef struct {
    /** length of conference_xml */
    uint16_t conference_xml_len;
    /** Conference XML is a part of an XML ﬁle that
     * is passed as a UTF-8 string. The conference description
     * consists of up to 2048 UTF-8 characters. Length range: 1 to 2048 */
    uint8_t conference_xml[QMI_VOICE_MAX_CONF_XML_LEN];
    /** Sequence number of this indication.
     * Sequence number 0 indicates that this indication is the start of a new update.
     * The sequence number increments for each successive indication of an update. */
    uint32_t sequence;
    /** set if optional failure_cause is available */
    bool total_size_available;
    /** Total size of the document being passed.
     * This is included in the ﬁrst indication of an update, i.e., the indication with sequence number 0.
     * The client has received the last indication of an update when the received size is equal to the total size. */
    enum16_t total_size;
    /** set if  optional call_id is available */
    bool call_id_available;
    /** Call ID of the call for which the conference info indication is sent. */
    uint8_t call_id;
} voice_conference_info_ind_t;

#define QMI_VOICE_MAX_CONF_URI_NAME_LEN 128
#define QMI_VOICE_MAX_CONF_URI_DESCRIPTION_LEN 64

/**
 * @brief output type for QMI_VOICE_CONFERENCE_PARTICIPANT_UPDATE_IND
 *
 */
typedef struct {
    /** length of uri_name string */
    uint8_t uri_name_len;
    /** URI name (128 UTF-16 characters) */
    uint16_t uri_name[QMI_VOICE_MAX_CONF_URI_NAME_LEN];
    /** length of uri_description string */
    uint8_t uri_description_len;
    /** URI description (128 UTF-16 characters) */
    uint16_t uri_description[QMI_VOICE_MAX_CONF_URI_DESCRIPTION_LEN];
} voice_partecipant_info_t;

/**
 * @brief output type for QMI_VOICE_CONFERENCE_JOIN_IND
 *
 */
typedef struct {
    /** Call ID of the conference */
    uint8_t call_id;
    /** see voice_partecipant_info_t */
    voice_partecipant_info_t partecipant_info;
} voice_conference_join_info_t;

/**
 * @brief output type for QMI_VOICE_SUPS_IND
 *
 */
typedef struct {
    /** Service type, see #QMI_VOICE_service_operation_t */
    enum8_t service_type;
    /** Indicates whether the supplementary service
     * data is modified by the card (SIM/USIM) as partù
     * of the call control:
     *
     * 0 – FALSE
     * 1 – TRUE */
    uint8_t is_modified_by_call_control;
    /** set if  optional call_id is available */
    bool call_id_available;
    /** Call identifier of the voice call that has been modified to a
     * supplementary service as a result of call control */
    uint8_t call_id;
} voice_supplementary_service_info_t;

/**
 * @brief QMI VOICE supplementary service notification types
 *
 */
typedef enum {
    /** MO call is being forwarded to another user */
    QMI_VOICE_NOTIFICATION_TYPE_OUTGOING_CALL_IS_FORWARDED = 0x01,
    /** MO call is waiting at the called user */
    QMI_VOICE_NOTIFICATION_TYPE_OUTGOING_CALL_IS_WAITING   = 0x02,
    /** Outgoing call is a CUG call */
    QMI_VOICE_NOTIFICATION_TYPE_OUTGOING_CUG_CALL          = 0x03,
    /** Outgoing calls are barred */
    QMI_VOICE_NOTIFICATION_TYPE_OUTGOING_CALLS_BARRED      = 0x04,
    /** Outgoing call is deflected */
    QMI_VOICE_NOTIFICATION_TYPE_OUTGOING_CALL_IS_DEFLECTED = 0x05,
    /** Incoming call is a CUG call */
    QMI_VOICE_NOTIFICATION_TYPE_INCOMING_CUG_CALL          = 0x06,
    /** Incoming calls are barred */
    QMI_VOICE_NOTIFICATION_TYPE_INCOMING_CALLS_BARRED      = 0x07,
    /** Incoming call is a forwarded call */
    QMI_VOICE_NOTIFICATION_TYPE_INCOMING_FORWARDED_CALL    = 0x08,
    /** Incoming call is a deflected call */
    QMI_VOICE_NOTIFICATION_TYPE_INCOMING_DEFLECTED_CALL    = 0x09,
    /** Incoming call is forwarded to another user */
    QMI_VOICE_NOTIFICATION_TYPE_INCOMING_CALL_IS_FORWARDED = 0x0A,
    /** Unconditional call forwarding is active */
    QMI_VOICE_NOTIFICATION_TYPE_UNCOND_CALL_FORWARD_ACTIVE = 0x0B,
    /** Conditional call forwarding is active */
    QMI_VOICE_NOTIFICATION_TYPE_COND_CALL_FORWARD_ACTIVE   = 0x0C,
    /** CLIR suppression is rejected */
    QMI_VOICE_NOTIFICATION_TYPE_CLIR_SUPPRESSION_REJECTED  = 0x0D,
    /** Call is put on hold at the remote party */
    QMI_VOICE_NOTIFICATION_TYPE_CALL_IS_ON_HOLD            = 0x0E,
    /** Call is retrieved at the remote party from the hold state */
    QMI_VOICE_NOTIFICATION_TYPE_CALL_IS_RETRIEVED          = 0x0F,
    /** Call is in a conference */
    QMI_VOICE_NOTIFICATION_TYPE_CALL_IS_IN_MPTY            = 0x10,
    /** Incoming call is an explicit call transfer */
    QMI_VOICE_NOTIFICATION_TYPE_INCOMING_CALL_IS_ECT       = 0x11,
} QMI_VOICE_sups_notification_type_t;

/**
 * @brief output type for QMI_VOICE_SUPS_NOTIFICATION_IND
 *
 */
typedef struct {
    /** Unique identifier of the call for which the
     * notification is applicable */
    uint8_t call_id;
    /** Notification type, see #QMI_VOICE_sups_notification_type_t */
    enum8_t notification_type;
} voice_sups_notification_t;

/**
 * @brief Unpack voice indications
 *
 * This function unpacks the result of the following voice indications
 *
 * QMI_VOICE_ALL_CALL_STATUS_IND (0x002E)
 *
 * @param[in] msg_id indication id
 * @param[in] rsp indication
 * @param[in] len indication length
 * @param[out] out type according to the following
 *      QMI_VOICE_ALL_CALL_STATUS_IND               (0x002E) -> #voice_all_call_status_ind_t
 *      QMI_VOICE_SUPS_NOTIFICATION_IND             (0x0032) -> #voice_sups_notification_t
 *      QMI_VOICE_CONFERENCE_INFO_IND               (0x0055) -> #voice_conference_info_ind_t
 *      QMI_VOICE_CONFERENCE_JOIN_IND               (0x0056) -> #voice_conference_join_info_t
 *      QMI_VOICE_CONFERENCE_PARTICIPANT_UPDATE_IND (0x0057) -> #voice_partecipant_info_t
 *      QMI_VOICE_SUPS_IND                          (0x0042) -> #voice_supplementary_service_info_t

 * @param[in] out_len buffer size
 */
QMI_SDK_error_t telit_voice_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len,
        uint8_t *out, uint16_t out_len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_VOICE_H */
