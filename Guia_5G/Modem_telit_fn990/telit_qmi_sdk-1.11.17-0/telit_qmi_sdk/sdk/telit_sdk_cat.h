#ifndef TELIT_SDK_CAT_H_
#define TELIT_SDK_CAT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_cat.h
 * @brief CAT pack/unpack functions
 * Service ID: QMI_SVC_CAT (10)
 */

/**
 * @brief Enum to describe QMI CAT Message IDs
 */
typedef enum {
    /** 1 - 0x0001 Enable events report */
    QMI_CAT_SET_EVENT_REPORT         = 0x0001,
    /** 1 - 0x0001 Event report indication */
    QMI_CAT_EVENT_REPORT_IND         = 0x0001,

    /** 33 - 0x0021 Send a terminal response */
    QMI_CAT_SEND_TR                  = 0x0021,
    /** 34 - 0x0022 Send an envelope command */
    QMI_CAT_SEND_ENVELOPE_CMD        = 0x0022,

    /** 45 - 0x002D Set the configuration of the service */
    QMI_CAT_SET_CONFIGURATION        = 0x002D,
    /** 46 - 0x002E Get the configuration of the service */
    QMI_CAT_GET_CONFIGURATION        = 0x002E,
    /** 47 - 0x002F Retrieve a cached proactive command from the modem */
    QMI_CAT_GET_CACHED_PROACTIVE_CMD = 0x002F,
} QMI_CAT_message_t;

/**
 * @brief QMI CAT event report request bitmask
 *
 */
typedef enum {
    /** Display text */
    QMI_CAT_EVENT_DISPLAY_TEXT          = (1 << 0),
    /** Get Inkey */
    QMI_CAT_EVENT_GET_INKEY             = (1 << 1),
    /** Get Input */
    QMI_CAT_EVENT_GET_INPUT             = (1 << 2),
    /** Setup Menu */
    QMI_CAT_EVENT_SETUP_MENU            = (1 << 3),
    /** Select Item */
    QMI_CAT_EVENT_SELECT_ITEM           = (1 << 4),
    /** Send SMS */
    QMI_CAT_EVENT_SEND_SMS              = (1 << 5),

    /** End Proactive Session */
    QMI_CAT_EVENT_END_PROACTIVE_SESSION = (1 << 12),
} QMI_CAT_pc_event_report_req_t;

/**
 * @brief input type for QMI_CAT_SET_EVENT_REPORT
 *
 */
typedef struct {
    /** set if pc_evt_report_req_mask is available */
    bool pc_evt_report_req_mask_available;
    /** Event report request bitmask, see #QMI_CAT_pc_event_report_req_t */
    uint32_t pc_evt_report_req_mask;
    /** set if pc_full_func_evt_report_req_mask is available */
    bool pc_full_func_evt_report_req_mask_available;
    /** Full function event report request bitmask */
    uint32_t pc_full_func_evt_report_req_mask;
} cat_set_event_report_masks_t;

/**
 * @brief Pack QMI_CAT_SET_EVENT_REPORT (1 - 0x0001) request
 *
 * This function packs a qmi request for setting the card application
 * toolkit reporting conditions for the requesting control point.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #cat_set_event_report_masks_t
 */
QMI_SDK_error_t telit_cat_set_event_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        cat_set_event_report_masks_t *input);

/**
 * @brief Unpack QMI_CAT_SET_EVENT_REPORT (1 - 0x0001) request
 *
 * This function unpacks the result of a QMI_CAT_SET_EVENT_REPORT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #cat_set_event_report_masks_t
 */
QMI_SDK_error_t telit_cat_set_event_report_unpack(uint8_t *rsp, uint16_t len,
        cat_set_event_report_masks_t *out);

#define QMI_CAT_PROACTIVE_COMMAND_MAX_SZ 512

/**
 * @brief generic proactive command type
 *
 */
typedef struct {
    /** Proactive command reference id */
    uint32_t uim_ref_id;
    /** set if operating_mode is available */
    uint16_t cmd_sz;
    /** Proactive command, encoding could vary */
    uint8_t cmd[QMI_CAT_PROACTIVE_COMMAND_MAX_SZ];
} cat_generic_proactive_command_t;

/**
 * @brief input type for QMI_CAT_SEND_TR
 *
 */
typedef struct {
    /** Terminal response for the relevant proactive command, encoded as in ETSI
     * TS 102 223, Section 6.8. */
    cat_generic_proactive_command_t terminal_response;
} cat_send_tr_input_t;

#define QMI_CAT_TERMINAL_RESPONSE_MAX_SZ 256

/**
 * @brief generic terminal response type
 *
 */
typedef struct {
    /* Value of SW1 of the response, as defined in 3GPP TS 11.11 for ICC and ETSI TS
     * 102 221 for UICC. */
    uint8_t sw1;
    /* Value of SW2 of the response as defined in 3GPP TS 11.11 for ICC and ETSI TS
     * 102 221 for UICC. */
    uint8_t sw2;
    /** Size of tr_response */
    uint8_t tr_response_sz;
    /** Terminal response */
    uint8_t tr_response[QMI_CAT_TERMINAL_RESPONSE_MAX_SZ];
} cat_generic_terminal_response_t;

/**
 * @brief output type for QMI_CAT_SEND_TR
 *
 */
typedef struct {
    /** set if terminal_response is available */
    bool terminal_response_available;
    /** Terminal response */
    cat_generic_terminal_response_t terminal_response;
} cat_send_tr_info_t;

/**
 * @brief Pack QMI_CAT_SEND_TR (33 - 0x0021) request
 *
 * This function packs a qmi request for sending the terminal
 * response to the proactive commands coming from the card.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #cat_send_tr_input_t
 */
QMI_SDK_error_t telit_cat_send_tr_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        cat_send_tr_input_t *input);

/**
 * @brief Unpack QMI_CAT_SEND_TR (33 - 0x0021) request
 *
 * This function unpacks the result of a QMI_CAT_SEND_TR request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #cat_send_tr_info_t
 */
QMI_SDK_error_t telit_cat_send_tr_unpack(uint8_t *rsp, uint16_t len,
        cat_send_tr_info_t *out);

/**
 * @brief QMI CAT Envelope Command Types
 *
 */
typedef enum {
    /** Menu Selection */
    QMI_CAT_ENVELOPE_CMD_TYPE_MENU_SELECTION = 0x01,
} QMI_CAT_envelope_cmd_type_t;

#define QMI_CAT_ENVELOPE_COMMAND_MAX_SZ 512

/**
 * @brief input type for QMI_CAT_SEND_ENVELOPE_CMD
 *
 */
typedef struct {
    /** Envelope command type, see #QMI_CAT_envelope_cmd_type_t */
    uint16_t env_cmd_type;
    /** Size of env_cmd */
    uint16_t env_cmd_sz;
    /** Encoded envelope response, as defined in ETSI TS 102 223, Section 7. */
    uint8_t env_cmd[QMI_CAT_ENVELOPE_COMMAND_MAX_SZ];
} cat_send_envelope_cmd_input_t;

/**
 * @brief output type for QMI_CAT_SEND_ENVELOPE_CMD
 *
 */
typedef struct {
    /** set if envelope_response is available */
    bool envelope_response_available;
    /** Terminal response */
    cat_generic_terminal_response_t envelope_response;
} cat_send_envelope_cmd_info_t;

/**
 * @brief Pack QMI_CAT_SEND_ENVELOPE_CMD (34 - 0x0022) request
 *
 * This function packs a qmi request for sending an envelope
 * command to the card.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #cat_send_envelope_cmd_input_t
 */
QMI_SDK_error_t telit_cat_send_envelope_cmd_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        cat_send_envelope_cmd_input_t *input);

/**
 * @brief Unpack QMI_CAT_SEND_ENVELOPE_CMD (34 - 0x0022) request
 *
 * This function unpacks the result of a QMI_CAT_SEND_ENVELOPE_CMD request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #cat_send_envelope_cmd_info_t
 */
QMI_SDK_error_t telit_cat_send_envelope_cmd_unpack(uint8_t *rsp, uint16_t len,
        cat_send_envelope_cmd_info_t *out);

/**
 * @brief QMI CAT Configuration Modes
 *
 */
typedef enum {
    /** Disabled */
    QMI_CAT_MODE_DISABLED   = 0x00,

    /** Android */
    QMI_CAT_MODE_ANDROID    = 0x02,

    /** Custom raw */
    QMI_CAT_MODE_CUSTOM_RAW = 0x05,
} QMI_CAT_configuration_mode_t;

/**
 * @brief input type for QMI_CAT_SET_CONFIGURATION, outpus type for QMI_CAT_GET_CONFIGURATION
 *
 */
typedef struct {
    /** set if cat_config_mode is available
     *  cat_config_mode is mandatory for QMI_CAT_SET_CONFIGURATION, while
     *  optional for QMI_CAT_GET_CONFIGURATION */
    bool cat_config_mode_available;
    /** Service configuration mode, see #QMI_CAT_configuration_mode_t */
    enum8_t cat_config_mode;
} cat_configuration_mode_t;

/**
 * @brief Pack QMI_CAT_SET_CONFIGURATION (45 - 0x002D) request
 *
 * This function packs a qmi request for changing the configuration
 * of the service.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #cat_configuration_mode_t
 */
QMI_SDK_error_t telit_cat_set_configuration_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        cat_configuration_mode_t *input);

/**
 * @brief Unpack QMI_CAT_SET_CONFIGURATION (45 - 0x002D) request
 *
 * This function unpacks the result of a QMI_CAT_SET_CONFIGURATION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_cat_set_configuration_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_CAT_GET_CONFIGURATION (46 - 0x002E) request
 *
 * This function packs a qmi request for getting the configuration
 * of the service.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_cat_get_configuration_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len);

/**
 * @brief Unpack QMI_CAT_GET_CONFIGURATION (46 - 0x002E) request
 *
 * This function unpacks the result of a QMI_CAT_GET_CONFIGURATION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #cat_configuration_mode_t
 */
QMI_SDK_error_t telit_cat_get_configuration_unpack(uint8_t *rsp, uint16_t len,
        cat_configuration_mode_t *out);

/**
 * @brief QMI CAT Cached Proactive Command Identifier
 *
 */
typedef enum {
    /** Setup menu */
    QMI_CAT_CACHED_COMMAND_ID_SETUP_MENU       = 0x01,
    /** Setup event list */
    QMI_CAT_CACHED_COMMAND_ID_SETUP_EVENT_LIST = 0x02,
    /** Setup idle mode text */
    QMI_CAT_CACHED_COMMAND_ID_SETUP_IDLE_TEXT  = 0x03,
} QMI_CAT_cached_proactive_cmd_id_t;

/**
 * @brief input type for QMI_CAT_GET_CACHED_PROACTIVE_CMD
 *
 */
typedef struct {
    /** ID of the proactive command, see #QMI_CAT_cached_proactive_cmd_id_t */
    QMI_CAT_cached_proactive_cmd_id_t command_id;
} cat_cached_proactive_cmd_input_t;

/**
 * @brief output type for QMI_CAT_GET_CACHED_PROACTIVE_CMD
 *
 */
typedef struct {
    /** set if setup_menu_event is available */
    bool setup_menu_event_available;
    /** Setup Menu proactive command, encoded as in ETSI TS 102 223, Section 6.6.7. */
    cat_generic_proactive_command_t setup_menu_event;
    /** set if setup_event_list_raw_event is available */
    bool setup_event_list_raw_event_available;
    /** Setup Menu proactive command, encoded as in ETSI TS 102 223, Section 6.6.16. */
    cat_generic_proactive_command_t setup_event_list_raw_event;
    /** set if setup_idle_mode_text_event is available */
    bool setup_idle_mode_text_event_available;
    /** Setup Menu proactive command, encoded as in ETSI TS 102 223, Section 6.6.22. */
    cat_generic_proactive_command_t setup_idle_mode_text_event;
} cat_cached_proactive_cmd_info_t;

/**
 * @brief Pack QMI_CAT_GET_CACHED_PROACTIVE_CMD (47 - 0x002F) request
 *
 * This function packs a qmi request for retrieving a cached proactive
 * command from the modem.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #cat_cached_proactive_cmd_input_t
 */
QMI_SDK_error_t telit_cat_get_cached_proactive_cmd_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        cat_cached_proactive_cmd_input_t *input);

/**
 * @brief Unpack QMI_CAT_GET_CACHED_PROACTIVE_CMD (47 - 0x002F) request
 *
 * This function unpacks the result of a QMI_CAT_GET_CACHED_PROACTIVE_CMD request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #cat_cached_proactive_cmd_info_t
 */
QMI_SDK_error_t telit_cat_get_cached_proactive_cmd_unpack(uint8_t *rsp, uint16_t len,
        cat_cached_proactive_cmd_info_t *out);

/**
 * @brief QMI CAT Response Type
 *
 */
typedef enum {
    /** Terminal response */
    QMI_CAT_RESPONSE_TYPE_TERMINAL_RESPONSE  = 0x00,
    /** Event confirmation */
    QMI_CAT_RESPONSE_TYPE_EVENT_CONFIRMATION = 0x01,
} QMI_CAT_response_type_t;

/**
 * @brief QMI CAT Session end Type
 *
 */
typedef enum {
    /** End proactive session command received from the card */
    QMI_CAT_SESSION_END_TYPE_FROM_CARD = 0x01,
    /** End proactive session internal to the ME */
    QMI_CAT_SESSION_END_TYPE_FROM_ME   = 0x02,
} QMI_CAT_session_end_type_t;

/**
 * @brief output type for QMI_CAT_EVENT_REPORT_IND indication
 *
 */
typedef struct {
    /** set if display_text_event is available */
    bool display_text_event_available;
    /** Display Text proactive command, encoded as in ETSI TS 102 223, Section 6.6.1. */
    cat_generic_proactive_command_t display_text_event;
    /** set if get_inkey_event is available */
    bool get_inkey_event_available;
    /** Get Inkey proactive command, encoded as in ETSI TS 102 223, Section 6.6.2. */
    cat_generic_proactive_command_t get_inkey_event;
    /** set if get_input_event is available */
    bool get_input_event_available;
    /** Get Input proactive command, encoded as in ETSI TS 102 223, Section 6.6.3. */
    cat_generic_proactive_command_t get_input_event;
    /** set if setup_menu_event is available */
    bool setup_menu_event_available;
    /** Setup Menu proactive command, encoded as in ETSI TS 102 223, Section 6.6.7. */
    cat_generic_proactive_command_t setup_menu_event;
    /** set if select_item_event is available */
    bool select_item_event_available;
    /** Setup Menu proactive command, encoded as in ETSI TS 102 223, Section 6.6.8. */
    cat_generic_proactive_command_t select_item_event;
    /** set if send_sms_event is available */
    bool send_sms_event_available;
    /** Send SMS proactive command, encoded as in ETSI TS 102 223, Section 6.6.9. */
    cat_generic_proactive_command_t send_sms_event;
    /** set if proactive_session_end_type is available */
    bool proactive_session_end_type_available;
    /** Proactive session end type, see #QMI_CAT_session_end_type_t */
    enum8_t proactive_session_end_type;
    /** set if slot is available */
    bool slot_available;
    /** Slot index */
    uint8_t slot;
    /** set if rsp_type is available */
    bool rsp_type_available;
    /** Action that the control point is expected to perform after receiving
     *  and processing the indication, see #QMI_CAT_response_type_t */
    enum32_t rsp_type;
} cat_ind_event_report_t;

/**
 * @brief Unpack cat indications
 *
 * This function unpacks the result of the following cat indications
 *
 * QMI_CAT_EVENT_REPORT_IND (1)
 *
 * @param[in] msg_id indication id
 * @param[in] rsp indication
 * @param[in] len indication length
 * @param[out] out type according to the following
 *      QMI_CAT_EVENT_REPORT_IND -> #cat_ind_event_report_t
 * @param[in] out_len buffer size
 */
QMI_SDK_error_t telit_cat_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len,
        uint8_t *out, uint16_t out_len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_CAT_H_ */
