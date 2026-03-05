#ifndef _TELIT_SDK_WMS_H_
#define _TELIT_SDK_WMS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_wms.h
 * @brief WMS pack/unpack functions
 * Service ID: QMI_SVC_WMS (5)
 */

/**
 * @brief Enum to describe QMI WMS Message IDs
 */
typedef enum {
    /** 01 - 0x0001 Enable events report */
    QMI_WMS_SET_EVENT_REPORT = 0x0001,
    /** 01 - 0x0001 Events report indication */
    QMI_WMS_EVENT_REPORT_IND = 0x0001,

    /** 32 - 0x0020 Send a raw message */
    QMI_WMS_RAW_SEND         = 0x0020,
    /** 33 - 0x0021 Write a raw message to the device */
    QMI_WMS_RAW_WRITE        = 0x0021,
    /** 34 - 0x0022 Read a raw message from the device */
    QMI_WMS_RAW_READ         = 0x0022,
    /** 35 - 0x0023 Modify message tag on the device */
    QMI_WMS_MODIFY_TAG       = 0x0023,
    /** 36 - 0x0024 Delete message by index/tag/memory */
    QMI_WMS_DELETE           = 0x0024,

    /** 49 - 0x0031 List all messages */
    QMI_WMS_LIST_MESSAGES    = 0x0031,
    /** 50 - 0x0032 Set routes for message memory storage */
    QMI_WMS_SET_ROUTES       = 0x0032,

    /** 52 - 0x0034 Get SMSC address */
    QMI_WMS_GET_SMSC_ADDR    = 0x0034,
    /** 53 - 0x0035 Set SMSC address */
    QMI_WMS_SET_SMSC_ADDR    = 0x0035,

    /** 55 - 0x0037 Send ACK */
    QMI_WMS_SEND_ACK         = 0x0037,

    /** 63 - 0x003f Indicate that the SMS storage is full */
    QMI_WMS_MEMORY_FULL_IND  = 0x003f,
} QMI_WMS_message_t;

/**
 * @brief input type for QMI_WMS_SET_EVENT_REPORT
 */
typedef struct {
    /** report new MT messages: 0x00 - Disable, 0x01 - Enable*/
    bool report_mt_message;
    /** true if report_mt_message is set */
    bool report_mt_message_available;
    /** report MO SMS call control: 0x00 - Disable, 0x01 - Enable*/
    bool report_call_control_info;
    /** true if report_call_control_info is set */
    bool report_call_control_info_available;
    /** report new MWI messages: 0x00 - Disable, 0x01 - Enable*/
    bool report_mwi_message;
    /** true if report_mwi_message is set */
    bool report_mwi_message_available;
} wms_set_event_t;

/**
 * @brief Pack QMI_WMS_SET_EVENT_REPORT (1 - 0x0001) request
 *
 * This function packs a qmi request to set event report
 *
 * @param req_ctx request context
 * @param req request
 * @param len request len
 * @param input #wms_set_event_t
 */
QMI_SDK_error_t telit_wms_set_event_report_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_set_event_t *input);

/**
 * @brief Unpack QMI_WMS_SET_EVENT_REPORT (1 - 0x0001) request
 *
 * This function unpacks the result of a QMI_WMS_SET_EVENT_REPORT request
 *
 * @param rsp response
 * @param len response len
 */
QMI_SDK_error_t telit_wms_set_event_report_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI WMS service options
 */
typedef enum {
    /** Choose the best service option while setting up the DC */
    QMI_WMS_SO_AUTO = 0x00,
    /** Service option 6 */
    QMI_WMS_SO_6    = 0x06,
    /** Service option 14 */
    QMI_WMS_SO_14   = 0x0E,
} QMI_WMS_service_option_t;

#define QMI_WMS_FOLLOW_ON_DC_ON 0x01
typedef struct {
    /** Force the message to be sent on CDMA:
       - 0x00: do not care about the channel on which the message is sent
       - 0x01: request to send the message over the dedicated channel */
    uint8_t force;
    /** see #QMI_WMS_service_option_t */
    enum8_t service_option;
} wms_force_on_dc_t;

/**
 * @brief QMI WMS message formats
 */
typedef enum {
    /** CDMA */
    QMI_WMS_MESSAGE_FORMAT_CDMA = 0x00,
    /** GW PP */
    QMI_WMS_MESSAGE_FORMAT_GWPP = 0x06,
    /** MWI */
    QMI_WMS_MESSAGE_FORMAT_MWI  = 0x08,
} QMI_WMS_message_format_t;

#define QMI_WMS_MAX_RAW_DATA_LEN    (896)

/**
 * @brief input type for QMI_WMS_RAW_SEND
 *
 */
typedef struct {
    /** see #QMI_WMS_message_format_t */
    enum8_t format;
    /** length of the following array containing the SMS in PDU format */
    uint16_t raw_data_size;
    /** Array containing the SMS in PDU format */
    uint8_t raw_data[QMI_WMS_MAX_RAW_DATA_LEN];
    /** optional, see #wms_force_on_dc_t */
    wms_force_on_dc_t force_on_dc;
    /** true if force and/or service_option are set */
    bool force_on_dc_available;
    /** optional flag to request to not disconnect the CDMA dedicated
       channel after the send operation is completed.
          values:
       - 0x01 - FOLLOW_ON_DC_ON (do not disconnect the DC
       after the send operation).
          Any value other than 0x01 in this field is threaded as
       an absence of this TLV */
    uint8_t follow;
    /** true if follow is set */
    bool follow_available;
    /** optional. Keeps the GW SMS link open for the specified
       number of seconds; can be enabled if more messages
       are expected to follow. */
    uint8_t gsm_wcdma_link_timer;
    /** true if gsm_wcdma_link_timer is set */
    bool gsm_wcdma_link_timer_available;
    /** Optional. Indicates whether the message is to be sent on IMS
          values
       - 0x00 - message is not to be sent on IMS
       - 0x01 - message is to be sent on IMS */
    uint8_t sms_on_ims;
    /** true if sms_on_ims is set */
    bool sms_on_ims_available;
    /** Optional. Indicates this message is a retry.
          values
       -  0x01 - WMS_MESSAGE_IS_A_RETRY
          Note: any value other than 0x01 in this field is
       treaded as absence of this TLV */
    uint8_t retry_message;
    /** true if retry_message is set */
    bool retry_message_available;
    /** Optional. Message ID to be used in the retry message.
       This TLV is valid only if Retry Message TLV is specified and set to 0x01 */
    uint32_t retry_message_id;
    /** true if retry_message_id is set */
    bool retry_message_id_available;
    /** Optional. Indicates whether to keep the link control enabled
       until the option is modified by the client
          values
       - 0x00 - enable link control one so that the lower layer
       keeps the line up for a specified time until the next
       MO SMS is requested or the timer expires
       - 0x01 - always enable link control
       Note, This TLV is valid only if the Link Control TLV is
       specified and is set to a valid timer value */
    uint8_t link_enable_mode;
    /** true if link_enable_mode is set */
    bool link_enable_mode_available;
} wms_raw_send_t;

/**
 * @brief Pack QMI_WMS_RAW_SEND (32 - 0x20) request
 *
 * This function packs a qmi request to send a new message in its raw format.
 *
 * @param req_ctx request context
 * @param req request
 * @param len in request buffer size, out request len
 * @param input #wms_raw_send_t
 */
QMI_SDK_error_t telit_wms_raw_send_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_raw_send_t *input);

/**
 * @brief QMI WMS GW RP cause per 3GPP TS 24.011 Section 8.2.5.4
 */
typedef enum {
    /** Unassigned (unallocated) number */
    QMI_WMS_RP_CAUSE_UNASSIGNED_NUMBER                    = 0x01,
    /** Operator determined barring */
    QMI_WMS_RP_CAUSE_OPERATOR_DETERMINED_BARRING          = 0x08,
    /** Call barred */
    QMI_WMS_RP_CAUSE_CALL_BARRED                          = 0x0A,
    /** Reserved */
    QMI_WMS_RP_CAUSE_RESERVED                             = 0x0B,
    /** Short message transfer rejected */
    QMI_WMS_RP_CAUSE_SMS_TRANSFER_REJECTED                = 0x15,
    /** Memory capacity exceeded */
    QMI_WMS_RP_CAUSE_MEMORY_CAP_EXCEEDED                  = 0x16,
    /** Destination out of order */
    QMI_WMS_RP_CAUSE_DESTINATION_OUT_OF_ORDER             = 0x1B,
    /** Unidentified subscriber */
    QMI_WMS_RP_CAUSE_UNIDENTIFIED_SUBSCRIBER              = 0x1C,
    /** Facility rejected */
    QMI_WMS_RP_CAUSE_FACILITY_REJECTED                    = 0x1D,
    /** Unknown subscriber */
    QMI_WMS_RP_CAUSE_UNKNOWN_SUBSCRIBER                   = 0x1E,
    /** Network out of order */
    QMI_WMS_RP_CAUSE_NETWORK_OUT_OF_ORDER                 = 0x26,
    /** Temporary failure */
    QMI_WMS_RP_CAUSE_TEMPORARY_FAILURE                    = 0x29,
    /** Congestion */
    QMI_WMS_RP_CAUSE_CONGESTION                           = 0x2A,
    /** Resources unavailable, unspecified */
    QMI_WMS_RP_CAUSE_RESOURCES_UNAVAILABLE                = 0x2F,
    /** Requested facility not subscribed */
    QMI_WMS_RP_CAUSE_REQUESTED_FACILITY_NOT_SUBSCRIBED    = 0x32,
    /** Requested facility not implemented */
    QMI_WMS_RP_CAUSE_REQUESTED_FACILITY_NOT_IMPLEMENTED   = 0x45,
    /** Invalid short message transfer reference value */
    QMI_WMS_RP_CAUSE_INVALID_SMS_TRANSFER_REFERENCE_VALUE = 0x51,
    /** Semantically incorrect message */
    QMI_WMS_RP_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE       = 0x5F,
    /** Invalid mandatory information */
    QMI_WMS_RP_CAUSE_INVALID_MANDATORY_INFO               = 0x60,
    /** Message type nonexistent or not implemented */
    QMI_WMS_RP_CAUSE_MESSAGE_TYPE_NOT_IMPLEMENTED         = 0x61,
    /** Message not compatible with short message protocol state */
    QMI_WMS_RP_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_SMS      = 0x62,
    /** Information element nonexistent or not implemented */
    QMI_WMS_RP_CAUSE_INFO_ELEMENT_NOT_IMPLEMENTED         = 0x63,
    /** Protocol error, unspecified */
    QMI_WMS_RP_CAUSE_PROTOCOL_ERROR                       = 0x6F,
    /** Interworking, unspecified */
    QMI_WMS_RP_CAUSE_INTERWORKING                         = 0x7F,
} QMI_WMS_RP_cause_t;

/**
 * @brief QMI WMS GW TP Cause per 3GPP TS 23.040 Section 9.2.3.22
 */
typedef enum {
    /** Telematic interworking not suppored */
    QMI_WMS_TP_CAUSE_INTERWORKING_NOT_SUPPORTED         = 0x80,
    /** Short message type 0 not supported */
    QMI_WMS_TP_CAUSE_SHORT_MESSAGE_TYPE_0_NOT_SUPPORTED = 0x81,
    /** Cannot replace short message */
    QMI_WMS_TP_CAUSE_SHORT_MESSAGE_CANNOT_BE_REPLACED   = 0x82,
    /** Unspecified TP-PID error */
    QMI_WMS_TP_CAUSE_UNSPECIFIED_PID_ERROR              = 0x8F,
    /** Data coding scheme (alphabet) not supported */
    QMI_WMS_TP_CAUSE_DCS_NOT_SUPPORTED                  = 0x90,
    /** Message class not supported */
    QMI_WMS_TP_CAUSE_MESSAGE_CLASS_NOT_SUPPORTED        = 0x91,
    /** Unspecified TP-DCS error */
    QMI_WMS_TP_CAUSE_UNSPECIFIED_DCS_ERROR              = 0x9F,
    /** Command cannot be actioned */
    QMI_WMS_TP_CAUSE_COMMAND_CANNOT_BE_ACTIONED         = 0xA0,
    /** Command unsupported */
    QMI_WMS_TP_CAUSE_COMMAND_UNSUPPORTED                = 0xA1,
    /** Unspecified TP-command error */
    QMI_WMS_TP_CAUSE_UNSPECIFIED_COMMAND_ERROR          = 0xAF,
    /** TPDU not supported */
    QMI_WMS_TP_CAUSE_TPDU_NOT_SUPPORTED                 = 0xB0,
    /** SC busy */
    QMI_WMS_TP_CAUSE_SC_BUSY                            = 0xC0,
    /** No SC subscription */
    QMI_WMS_TP_CAUSE_NO_SC_SUBSCRIPTION                 = 0xC1,
    /** SC system failure */
    QMI_WMS_TP_CAUSE_SC_SYS_FAILURE                     = 0xC2,
    /** Invalid SME address */
    QMI_WMS_TP_CAUSE_INVALID_SME_ADDRESS                = 0xC3,
    /** Destination SME barred */
    QMI_WMS_TP_CAUSE_DESTINATION_SME_BARRED             = 0xC4,
    /** SM rejected-duplicate SM */
    QMI_WMS_TP_CAUSE_SM_REJECTED_OR_DUPLICATED          = 0xC5,
    /** TP-VPF not supported */
    QMI_WMS_TP_CAUSE_TP_VPF_NOT_SUPPORTED               = 0xC6,
    /** TP-VP not supported */
    QMI_WMS_TP_CAUSE_TP_VP_NOT_SUPPORTED                = 0xC7,
    /** SIM SMS storage full */
    QMI_WMS_TP_CAUSE_SIM_SMS_STORAGE_FULL               = 0xD0,
    /** No SMS storage capability in SIM */
    QMI_WMS_TP_CAUSE_NO_SMS_STORAGE_CAP_IN_SIM          = 0xD1,
    /** Error in MS */
    QMI_WMS_TP_CAUSE_MS_ERROR                           = 0xD2,
    /** Memory capacity exceeded */
    QMI_WMS_TP_CAUSE_MEMORY_CAP_EXCEEDED                = 0xD3,
    /** SIM application toolkit busy */
    QMI_WMS_TP_CAUSE_SIM_APP_TOOLKIT_BUSY               = 0xD4,
    /** SIM data download error */
    QMI_WMS_TP_CAUSE_SIM_DATA_DOWNLOAD_ERROR            = 0xD5,
    /** Unspecified error cause */
    QMI_WMS_TP_CAUSE_UNSPECIFIED_ERROR                  = 0xFF,
} QMI_WMS_TP_cause_t;

/**
 * @brief QMI WMS message delivery failure types
 */
typedef enum {
    /** Temporary delivery failure */
    QMI_WMS_MESSAGE_DELIVERY_FAILURE_TEMPORARY = 0x00,
    /** Permanent delivery failure */
    QMI_WMS_MESSAGE_DELIVERY_FAILURE_PERMANENT = 0x01,
} QMI_WMS_message_delivery_failure_type_t;

/**
 * @brief QMI WMS Cause Code per 3GPP2 N.S0005-0 Section 6.5.2.125
 */
typedef enum {
    /** The SMS Destination Address is valid but is not currently allocated to an SMS terminal; the MIN associated with a valid destination address is not known to its HLR */
    QMI_WMS_TL_CAUSE_CODE_ADDR_VACANT                = 0x00,
    /** The SMS Destination Address is invalid */
    QMI_WMS_TL_CAUSE_CODE_ADDR_TRANSLATION_FAILURE   = 0x01,
    /** The network transmission failed due to lack of a network resource or link capacity */
    QMI_WMS_TL_CAUSE_CODE_NETWORK_RESOURCE_SHORTAGE  = 0x02,
    /** A network node failed, a link failed, or a required operation failed */
    QMI_WMS_TL_CAUSE_CODE_NETWORK_FAILURE            = 0x03,
    /** The SMS_TeleserviceIdentifer is not known, is not supported, or is not authorized by an addressed functional entity */
    QMI_WMS_TL_CAUSE_CODE_INVALID_TELESERVICE_ID     = 0x04,
    /** A network problem other than identifed above */
    QMI_WMS_TL_CAUSE_CODE_NETWORK_OTHER              = 0x05,
    /** The addressed MS-based SME is known, but it does not respond to a page; SMS notifcation is not pending */
    QMI_WMS_TL_CAUSE_CODE_NO_PAGE_RESPONSE           = 0x20,
    /** The destination MS-based SME is SMS capable, but is currently engaged in a call, a service, or a call mode that precludes the use of SMS, or the destination SME is congested. This value can only be used between the MSC and the MC when allowed by bilateral agreement. SMS Notifcation is not pending. */
    QMI_WMS_TL_CAUSE_CODE_DEST_BUSY                  = 0x21,
    /** The destination SME does not acknowledge receipt of the SMS delivery. This value may be used when Terminal Busy and No Page Response are not appropriate. SMS notifcation is not pending. */
    QMI_WMS_TL_CAUSE_CODE_NO_ACK                     = 0x22,
    /** A required terminal resource (memory, etc.) is not available to process this message; SMS notifcation is not pending */
    QMI_WMS_TL_CAUSE_CODE_DEST_RESOURCE_SHORTAGE     = 0x23,
    /** Delivery is not currently possible (e.g., No page response, Destination busy, No acknowledgment, Destination out of service, Other terminal problem), but SMS notifcation is pending */
    QMI_WMS_TL_CAUSE_CODE_SMS_DELIVERY_POSTPONED     = 0x24,
    /** The addressed destination is out of service for an extended period of time (e.g., MS sleep, inactive, power off); SMS notifcation is not pending */
    QMI_WMS_TL_CAUSE_CODE_DEST_OUT_OF_SERV           = 0x25,
    /** The MS-based SME is no longer at the temporary SMS routing address. The message sender should not reuse the temporary SMS routing address. SMS notifcation is not pending. */
    QMI_WMS_TL_CAUSE_CODE_DEST_NOT_AT_ADDR           = 0x26,
    /** A terminal problem other than described above; SMS notifcation is not pending */
    QMI_WMS_TL_CAUSE_CODE_DEST_OTHER                 = 0x27,
    /** There is no channel available or there is radio congestion at this time */
    QMI_WMS_TL_CAUSE_CODE_RADIO_IF_RESOURCE_SHORTAGE = 0x40,
    /** The MS for an MS-based SME is operating in a mode that does not support SMS at this time */
    QMI_WMS_TL_CAUSE_CODE_RADIO_IF_INCOMPATIBILITY   = 0x41,
    /** A radio interface problem to an MS-based SME other than described above */
    QMI_WMS_TL_CAUSE_CODE_RADIO_IF_OTHER             = 0x42,
    /** The size of a parameter or feld is not what is expected */
    QMI_WMS_TL_CAUSE_CODE_ENCODING                   = 0x60,
    /** The originating MIN is not recognized, the originating address is not allowed for the originating MIN, the ESN does not match the originating MIN, the origination is not authorized, the originating address is not recognized, etc. */
    QMI_WMS_TL_CAUSE_CODE_SMS_ORIG_DENIED            = 0x61,
    /** The destination is not authorized to receive the SMS message, the MC refused the message, the destination SME refused the message, the destination is not authorized for a required supplementary service, etc. */
    QMI_WMS_TL_CAUSE_CODE_SMS_TERM_DENIED            = 0x62,
    /** The originating supplementary service is not known or supported, the sender is not authorized for an originating supplementary service, etc. */
    QMI_WMS_TL_CAUSE_CODE_SUPP_SERV_NOT_SUPP         = 0x63,
    /** SMS is not supported by an addressed functional entity */
    QMI_WMS_TL_CAUSE_CODE_SMS_NOT_SUPP               = 0x64,
    /** An optional parameter that is required for a particular function */
    QMI_WMS_TL_CAUSE_CODE_MISSING_EXPECTED_PARAM     = 0x65,
    /** A parameter is missing that is mandatory for a particular message */
    QMI_WMS_TL_CAUSE_CODE_MISSING_MAND_PARAM         = 0x66,
    /** A known parameter has an unknown or unsupported value */
    QMI_WMS_TL_CAUSE_CODE_UNRECOGNIZED_PARAM_VAL     = 0x67,
    /** A known parameter has a known but unexpected value */
    QMI_WMS_TL_CAUSE_CODE_UNEXPECTED_PARAM_VAL       = 0x68,
    /** The User Data size is too large for access technology, transport network, or call mode,etc.;theUserDatasizeisnotwhat is expected for the indicated teleservice */
    QMI_WMS_TL_CAUSE_CODE_USER_DATA_SIZE_ERR         = 0x69,
    /** Other general problems */
    QMI_WMS_TL_CAUSE_CODE_GENERAL_OTHER              = 0x6A,
} QMI_WMS_cause_code_t;

/**
 * @brief QMI WMS error classes
 */
typedef enum {
    /** Temporary error */
    QMI_WMS_ERROR_CLASS_TEMPORARY = 0x00,
    /** Permanent error */
    QMI_WMS_ERROR_CLASS_PERMANENT = 0x01,
} QMI_WMS_error_class_t;

/**
 * @brief QMI WMS gw cause information type
 */
typedef struct {
    /** see #QMI_WMS_RP_cause_t */
    enum16_t rp_cause;
    /** see #QMI_WMS_TP_cause_t */
    enum8_t tp_cause;
} wms_gw_cause_info_t;

/**
 * @brief output type for QMI_WMS_RAW_SEND
 *
 */
typedef struct {
    /** ID of the sent message. This value is valid only on a SUCCESSFUL response */
    uint16_t message_id;
    /** optional, see #QMI_WMS_cause_code_t */
    enum16_t cause_code;
    /** true if cause_code is set */
    bool cause_code_available;
    /** optional, see #QMI_WMS_error_class_t */
    enum8_t error_class;
    /** true if error_class is set */
    bool error_class_available;
    /** optional, see #wms_gw_cause_info_t struct */
    wms_gw_cause_info_t gw_cause_info;
    /** true if gw_cause_info is set */
    bool gw_cause_info_available;
    /** optional, see #QMI_WMS_message_delivery_failure_type_t */
    enum8_t message_delivery_failure_type;
    /** true if message_delivery_failure_type is set */
    bool message_delivery_failure_type_available;
} wms_raw_send_resp_t;

/**
 * @brief Unpack QMI_WMS_RAW_SEND (32 - 0x20) request
 *
 * This function unpacks the result of a QMI_WMS_RAW_SEND request
 *
 * @param[in]  rsp response
 * @param[in]  len response len
 * @param[out] out #wms_raw_send_resp_t
 */
QMI_SDK_error_t telit_wms_raw_send_unpack(uint8_t *rsp, uint16_t len, wms_raw_send_resp_t *out);

/**
 * @brief QMI WMS message tag types
 */
typedef enum {
    /** Mobile terminated read */
    QMI_WMS_TAG_TYPE_MT_READ     = 0x00,
    /** Mobile terminated not read */
    QMI_WMS_TAG_TYPE_MT_NOT_READ = 0x01,
    /** Mobile originated sent */
    QMI_WMS_TAG_TYPE_MO_SENT     = 0x02,
    /** Mobile originated not sent */
    QMI_WMS_TAG_TYPE_MO_NOT_SENT = 0x03,
} QMI_WMS_tag_type_t;

/**
 * @brief QMI WMS memory storage types
 */
typedef enum {
    /** UIM */
    QMI_WMS_STORAGE_UIM = 0x00,
    /** NV */
    QMI_WMS_STORAGE_NV  = 0x01,
} QMI_WMS_storage_type_t;

/**
 * @brief input type for QMI_WMS_RAW_WRITE
 *
 */
typedef struct {
    /** see #QMI_WMS_storage_type_t */
    enum8_t storage_type;
    /** see #QMI_WMS_message_format_t */
    enum8_t format;
    /** length of the following array containing the SMS in PDU format */
    uint16_t raw_data_size;
    /** Array containing the SMS in PDU format */
    uint8_t raw_data[QMI_WMS_MAX_RAW_DATA_LEN];
    /** optional, see #QMI_WMS_tag_type_t */
    enum8_t tag_type;
    /** true if message tag is set */
    bool tag_type_available;
} wms_raw_write_t;

/**
 * @brief Pack QMI_WMS_RAW_WRITE (33 - 0x21) request
 *
 * This function packs a qmi request to store a message in the device memory.
 *
 * @param req_ctx request context
 * @param req request
 * @param len in request buffer size, out request len
 * @param input #wms_raw_write_t
 */
QMI_SDK_error_t telit_wms_raw_write_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_raw_write_t *input);

/**
 * @brief output type for QMI_WMS_RAW_WRITE
 *
 */
typedef struct {
    uint32_t memory_index;
} wms_raw_write_resp_t;

/**
 * @brief Unpack QMI_WMS_RAW_WRITE (33 - 0x21) request
 *
 * This function unpacks the result of a QMI_WMS_RAW_WRITE request
 *
 * @param[in]  rsp response
 * @param[in]  len response len
 * @param[out] out #wms_raw_write_resp_t
 */
QMI_SDK_error_t telit_wms_raw_write_unpack(uint8_t *rsp, uint16_t len, wms_raw_write_resp_t *out);

/**
 * @brief QMI WMS message modes
 */
typedef enum {
    /** 3GPP2 message mode */
    QMI_WMS_MESSAGE_MODE_CDMA = 0x00,
    /** 3GPP message mode */
    QMI_WMS_MESSAGE_MODE_GW   = 0x01,
} QMI_WMS_message_mode_t;

/**
 * @brief input type for QMI_WMS_RAW_READ
 */
typedef struct {
    /** see #QMI_WMS_storage_type_t */
    enum8_t storage_type;
    uint32_t memory_index;
    /** optional, see #QMI_WMS_message_mode_t */
    enum8_t message_mode;
    /** true if message_mode is set */
    bool message_mode_available;
    /** optional, Indicates whether the message is to be read from IMS.
          values:
       - 0x00 - message is not to be read from IMS
       - 0x01 - message is to be read from IMS
          Note: this TLV is deprecated from minor version 9 */
    uint8_t sms_on_ims;
    /** true if sms_on_ims is set */
    bool sms_on_ims_available;
} wms_raw_read_t;

/**
 * @brief Pack QMI_WMS_RAW_READ (34 - 0x22) request
 *
 * This function packs a qmi request for reading a message from the device memory storage
 *
 * @param req_ctx request context
 * @param req request
 * @param len request len
 * @param input #wms_raw_read_t
 */
QMI_SDK_error_t telit_wms_raw_read_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_raw_read_t *input);

/**
 * @brief output type for QMI_WMS_RAW_READ
 */
typedef struct {
    /** see #QMI_WMS_tag_type_t */
    enum8_t message_tag;
    /** see #QMI_WMS_message_format_t */
    enum8_t format;
    /** length of the following array containing the SMS in PDU format */
    uint16_t raw_data_size;
    /** Array containing the SMS in PDU format */
    uint8_t raw_data[QMI_WMS_MAX_RAW_DATA_LEN];
} wms_raw_read_resp_t;

/**
 * @brief Unpack QMI_WMS_RAW_READ (34 - 0x22) request
 *
 * This function unpacks the result of a QMI_WMS_RAW_READ request
 *
 * @param rsp response
 * @param len response len
 * @param out #wms_raw_read_resp_t
 */
QMI_SDK_error_t telit_wms_raw_read_unpack(uint8_t *rsp, uint16_t len, wms_raw_read_resp_t *out);

/**
 * @brief QMI WMS message tag type
 */
typedef struct {
    /** see #QMI_WMS_storage_type_t */
    enum8_t storage_type;
    /** Memory index */
    uint32_t storage_index;
    /** see #QMI_WMS_tag_type_t */
    enum8_t tag_type;
} wms_message_tag_t;

/**
 * @brief input type for QMI_WMS_MODIFY_TAG
 */
typedef struct {
    /** see #wms_message_tag_t */
    wms_message_tag_t message_tag;
    /** optional, see #QMI_WMS_message_mode_t */
    enum8_t message_mode;
    /** true if message_mode is set */
    bool message_mode_available;
} wms_modify_tag_t;

/**
 * @brief pack QMI_WMS_MODIFY_TAG (35 - 0x23) request
 *
 * This function packs a qmi request for modifying a tag of a message in device memory
 *
 * @param req_ctx request context
 * @param req request
 * @param len request len
 * @param input #wms_modify_tag_t
 */
QMI_SDK_error_t telit_wms_modify_tag_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_modify_tag_t *input);

/**
 * @brief Unpack QMI_WMS_MODIFY_TAG (35 - 0x23) request
 *
 * This function unpacks the result of a QMI_WMS_MODIFY_TAG request
 *
 * @param rsp response
 * @param len response len
 */
QMI_SDK_error_t telit_wms_modify_tag_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_WMS_DELETE
 */
typedef struct {
    /** see #QMI_WMS_storage_type_t */
    enum8_t storage_type;
    /** memory index */
    uint32_t memory_index;
    /** true if memory_index is set */
    bool memory_index_available;
    /** optional, see #QMI_WMS_tag_type_t */
    enum8_t tag_type;
    /** true if message tag is set */
    bool tag_type_available;
    /** optional, see #QMI_WMS_message_mode_t */
    enum8_t message_mode;
    /** true if message_mode is set */
    bool message_mode_available;
} wms_delete_t;

/**
 * @brief pack QMI_WMS_DELETE (36 - 0x24) request
 *
 * This function packs a qmi request for deleting a message from the device memory storage
 *
 * @param req_ctx request context
 * @param req request
 * @param len request len
 * @param input #wms_delete_t
 */
QMI_SDK_error_t telit_wms_delete_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len, wms_delete_t *input);

/**
 * @brief Unpack QMI_WMS_DELETE (36 - 0x24) request
 *
 * This function unpacks the result of a QMI_WMS_DELETE request
 *
 * @param rsp response
 * @param len response len
 */
QMI_SDK_error_t telit_wms_delete_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_WMS_LIST_MESSAGES
 */
typedef struct {
    /** see #QMI_WMS_storage_type_t */
    enum8_t storage_type;
    /** optional, see #QMI_WMS_tag_type_t */
    enum8_t requested_tag;
    /** true if message tag is set */
    bool requested_tag_available;
    /** optional, see #QMI_WMS_message_mode_t */
    enum8_t message_mode;
    /** true if message_mode is set */
    bool message_mode_available;
} wms_list_messages_t;

/**
 * @brief pack QMI_WMS_LIST_MESSAGES (49 - 0x31) request
 *
 * This function packs a qmi request for listing all the message from the device memory storage
 *
 * @param req_ctx request context
 * @param req request
 * @param len request len
 * @param input #wms_list_messages_t
 */
QMI_SDK_error_t telit_wms_list_messages_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_list_messages_t *input);

/**
 * @brief QMI WMS message data instance type
 */
typedef struct {
    /** Message index */
    uint32_t message_index;
    /** see #QMI_WMS_tag_type_t */
    enum8_t tag_type;
} wms_message_instance_t;

#define QMI_WMS_MAX_NUMBER_OF_MESSAGES (1024)

/**
 * @brief output type for QMI_WMS_LIST_MESSAGES
 */
typedef struct {
    /** message_list array size */
    uint32_t message_list_size;
    /** array of #wms_message_instance_t instances */
    wms_message_instance_t message_list[QMI_WMS_MAX_NUMBER_OF_MESSAGES];
} wms_list_messages_resp_t;

/**
 * @brief pack QMI_WMS_LIST_MESSAGES (49 - 0x31) response
 *
 * This function unpacks the result of a QMI_WMS_LIST_MESSAGES request
 *
 * @param rsp response
 * @param len response len
 * @param out #wms_list_messages_resp_t
 */
QMI_SDK_error_t telit_wms_list_messages_unpack(uint8_t *rsp, uint16_t len, wms_list_messages_resp_t *out);

/**
 * @brief QMI WMS message classes
 */
typedef enum {
    /** Class 0 */
    QMI_WMS_MESSAGE_CLASS_0    = 0x00,
    /** Class 1 */
    QMI_WMS_MESSAGE_CLASS_1    = 0x01,
    /** Class 2 */
    QMI_WMS_MESSAGE_CLASS_2    = 0x02,
    /** Class 3 */
    QMI_WMS_MESSAGE_CLASS_3    = 0x03,
    /** Class none */
    QMI_WMS_MESSAGE_CLASS_NONE = 0x04,
    /** Class CDMA */
    QMI_WMS_MESSAGE_CLASS_CDMA = 0x05,
} QMI_WMS_message_class_t;

/**
 * @brief QMI WMS receipt actions
 */
typedef enum {
    /** Discard incoming messages without notifications */
    QMI_WMS_RECEIPT_ACTION_DISCARD          = 0x00,
    /** Store incoming messages to the specified location and send notifications to the registered clients */
    QMI_WMS_RECEIPT_ACTION_STORE_AND_NOTIFY = 0x01,
    /** Transfer incoming messages to the client and expect the client to send an ACK to the network */
    QMI_WMS_RECEIPT_ACTION_TRANSFER_ONLY    = 0x02,
    /** Transfer incoming messages to the client and send an ACK to the network */
    QMI_WMS_RECEIPT_ACTION_TRANSFER_AND_ACK = 0x03,
} QMI_WMS_receipt_action_t;

/**
 * @brief QMI WMS route type
 *
 */
typedef struct {
    /** Message type matching this route (only 0x00 - Point to point supported) */
    uint8_t message_type;
    /** Message class, see #QMI_WMS_message_class_t */
    uint8_t message_class;
    /** If the action is store, where to store the incoming message, see #QMI_WMS_storage_type_t */
    uint8_t route_storage;
    /** Action to be taken on receipt of a message matching the specified type and class for this route, see #QMI_WMS_receipt_action_t */
    uint8_t receipt_action;
} wms_route_t;

#define QMI_WMS_MAX_ROUTES_LEN 10

/**
 * @brief input type for QMI_WMS_SET_ROUTES
 *
 */
typedef struct {
    /** Size of routes array */
    uint16_t routes_size;
    /** Array of wms_route_t items */
    wms_route_t routes[QMI_WMS_MAX_ROUTES_LEN];
    /** true if transfer_ind is set */
    bool transfer_ind_available;
    /** Status reports are transferred to the client if 1, otherwise 0 */
    uint8_t transfer_ind;
} wms_set_routes_t;

/**
 * @brief pack QMI_WMS_SET_ROUTES (50 - 0x32) request
 *
 * This function packs a qmi request for setting the desired action upon WMS message receipt for the specified message routes
 *
 * @param req_ctx request context
 * @param req request
 * @param len request len
 * @param input #wms_set_routes_t
 */
QMI_SDK_error_t telit_wms_set_routes_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_set_routes_t *input);

/**
 * @brief unpack QMI_WMS_SET_ROUTES (50 - 0x32) request
 *
 * This function unpacks the result of a QMI_WMS_SET_ROUTES request
 *
 * @param rsp response
 * @param len response len
 */
QMI_SDK_error_t telit_wms_set_routes_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief pack QMI_WMS_GET_SMSC_ADDRESS (52 - 0x34) request
 *
 * This function packs a qmi request for querying the currently configured SMSC address
 *
 * @param req_ctx request context
 * @param req request
 * @param len request len
 */
QMI_SDK_error_t telit_wms_get_smsc_address_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

#define QMI_WMS_MAX_SMSC_ADDRESS_TYPE_LEN 4
#define QMI_WMS_MAX_SMSC_ADDRESS_LEN 22

/**
 * @brief QMI WMS smsc address type
 *
 */
typedef struct {
    /** length of smsc_address_type, 3 if set, otherwise 0 */
    uint8_t smsc_address_type_len;
    /** Type of SMSC address given in ASCII digits (must be three digits long, with leading zeros used as placeholders) */
    char smsc_address_type[QMI_WMS_MAX_SMSC_ADDRESS_TYPE_LEN];
    /** length of smsc_address */
    uint8_t smsc_address_len;
    /** SMSC address given in ASCII digits (can be prefixed with +) */
    char smsc_address[QMI_WMS_MAX_SMSC_ADDRESS_LEN];
} wms_smsc_t;

/**
 * @brief unpack QMI_WMS_GET_SMSC_ADDRESS (52 - 0x34) response
 *
 * This function unpacks the result of a QMI_WMS_GET_SMSC_ADDRESS request
 *
 * @param rsp response
 * @param len response len
 * @param out #wms_smsc_t
 */
QMI_SDK_error_t telit_wms_get_smsc_address_unpack(uint8_t *rsp, uint16_t len, wms_smsc_t *out);

/**
 * @brief pack QMI_WMS_SET_SMSC_ADDRESS (53 - 0x35) request
 *
 * This function packs a qmi request for setting the SMSC address used when storing or saving SMS messages
 *
 * @param req_ctx request context
 * @param req request
 * @param len request len
 * @param input #wms_smsc_t
 */
QMI_SDK_error_t telit_wms_set_smsc_address_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_smsc_t *input);

/**
 * @brief unpack QMI_WMS_SET_SMSC_ADDRESS (50 - 0x32) request
 *
 * This function unpacks the result of a QMI_WMS_SET_SMSC_ADDRESS request
 *
 * @param rsp response
 * @param len response len
 */
QMI_SDK_error_t telit_wms_set_smsc_address_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI WMS send_ack data type
 *
 */
typedef struct {
    /** Transaction ID of the message for which ACK is to be sent */
    uint32_t transaction_id;
    /** Message protocol, see #QMI_WMS_message_mode_t */
    enum8_t message_protocol;
    /** Indicates whether the MT message processed successfully, 0x00 failure, 0x01 success */
    uint8_t success;
} wms_send_ack_t;

/**
 * @brief pack QMI_WMS_SEND_ACK (55 - 0x37) request
 *
 * This function packs a qmi request for sending an ack to the network for a MT message
 *
 * @param req_ctx request context
 * @param req request
 * @param len request len
 * @param input #wms_send_ack_t
 */
QMI_SDK_error_t telit_wms_send_ack_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_send_ack_t *input);

/**
 * @brief Unpack QMI_WMS_SEND_ACK (55 - 0x37) request
 *
 * This function unpacks the result of a QMI_WMS_SEND_ACK request
 *
 * @param rsp response
 * @param len response len
 */
QMI_SDK_error_t telit_wms_send_ack_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI WMS notification types
 */
typedef enum {
    /** Primary */
    QMI_WMS_NOTIFICATION_PRIMARY        = 0x00,
    /** Secondary GSM */
    QMI_WMS_NOTIFICATION_SECONDARY_GSM  = 0x01,
    /** Secondary UMTS */
    QMI_WMS_NOTIFICATION_SECONDARY_UMTS = 0x02,
} QMI_WMS_notification_type_t;

/**
 * @brief QMI WMS broadcast SMS RAT Information Options
 */
typedef enum {
    /** Broadcast SMS received over GSM */
    QMI_WMS_BC_RAT_GSM_RADIO_ACCESS   = 0x00,
    /** Broadcast SMS received over WCDMA */
    QMI_WMS_BC_RAT_WCDMA_RADIO_ACCESS = 0x01,
    /** Broadcast SMS received over LTE */
    QMI_WMS_BC_RAT_LTE_RADIO_ACCESS   = 0x02,
    /** Broadcast SMS received over CDMA */
    QMI_WMS_BC_RAT_CDMA_RADIO_ACCESS  = 0x03,
    /** Broadcast SMS received over NR5G */
    QMI_WMS_BC_RAT_NR5G_RADIO_ACCESS  = 0x04,
} QMI_WMS_BC_RAT_info_t;

/**
 * @brief QMI WMS report MT message type
 */
typedef struct {
    /** see #QMI_WMS_storage_type_t */
    enum8_t storage_type;
    /** Memory index */
    uint32_t storage_index;
} wms_report_mt_message;

/**
 * @brief QMI WMS ack actions
 */
typedef enum {
    /** Send ACK */
    QMI_WMS_ACK_INDICATOR_SEND_ACK        = 0x00,
    /** Do not send ACK */
    QMI_WMS_ACK_INDICATOR_DO_NOT_SEND_ACK = 0x01,
} QMI_WMS_ack_action_t;

/**
 * @brief QMI WMS transfer route MT message type
 */
typedef struct {
    /** Parameter to indicate id ACK needs to be sent by the control point, see #QMI_WMS_ack_action_t */
    enum8_t ack_indicator;
    /** transaction ID of the message */
    uint32_t transaction_id;
    /** see #QMI_WMS_message_format_t */
    enum8_t format;
    /** length of the following raw data */
    uint16_t size;
    uint8_t data[QMI_WMS_MAX_RAW_DATA_LEN];  /** < raw data message */
} wms_transfer_route_mt_message;

/**
 * @brief QMI WMS notification type
 */
typedef struct {
    /** see #QMI_WMS_notification_type_t */
    enum8_t type;
    /** length of the following raw data */
    uint16_t size;
    uint8_t data[QMI_WMS_MAX_RAW_DATA_LEN];  /** < raw data message */
} wms_notification_type_t;

/**
 * @brief QMI WMS ETWS PLMN information
 */
typedef struct {
    /** 16-bit integer representation of MCC */
    uint16_t mobile_country_code;
    /** 16-bit integer representation of MNC */
    uint16_t mobile_network_code;
} wms_etws_plmn_information;

#define SMSC_ADDRESS_DATA_MAX_LEN 128
/**
 * @brief QMI WMS smsc address type
 */
typedef struct {
    /** length of the smsc address */
    uint8_t len;
    /** smsc address */
    uint8_t data[SMSC_ADDRESS_DATA_MAX_LEN];
} wms_smsc_address_t;

/**
 * @brief output type for QMI_WMS_EVENT_REPORT_IND
 */
typedef struct {
    /** see #wms_report_mt_message */
    wms_report_mt_message mt_message;
    /** true if mt_message is set */
    bool mt_message_set;
    /** see #wms_transfer_route_mt_message */
    wms_transfer_route_mt_message transfer_route_mt_message;
    /** true if transfer_route_mt_message is set */
    bool transfer_route_mt_message_set;
    /** optional, see #QMI_WMS_message_mode_t */
    enum8_t message_mode;
    /** true if message_mode is set */
    bool message_mode_set;
    /** see #wms_notification_type_t */
    wms_notification_type_t notification_type;
    /** true if notification_type is set */
    bool notification_type_set;
    /** see #wms_etws_plmn_information */
    wms_etws_plmn_information etws_plmn_info;
    /** true if etws_plmn_info is set */
    bool etws_plmn_info_set;
    /** see #wms_smsc_address_t */
    wms_smsc_address_t smsc_address;
    /** true if smsc_address is set */
    bool smsc_address_set;
    /** Optional. Indicates whether the massage is to be sent on IMS
          values
       - 0x00 - message is not to be sent on IMS
       - 0x01 - message is to be sent on IMS */
    uint8_t sms_on_ims;
    /** true if sms_on_ims is set */
    bool sms_on_ims_set;
    /** see #QMI_WMS_BC_RAT_info_t */
    enum32_t bc_sms_rat_info;
    /** true if sms_on_ims is set */
    bool bc_sms_rat_info_set;
} wms_ind_event_report_t;

/**
 * @brief output type for QMI_WMS_MEMORY_FULL_IND
 */
typedef struct {
    /** memory storage, see #QMI_WMS_storage_type_t */
    enum8_t storage_type;
    /** message mode, see #QMI_WMS_message_mode_t */
    enum8_t message_mode;
} wms_memory_full_info_t;

/**
 * @brief Unpack WMS indications
 *
 * This function unpacks the result of the following WMS indications
 *
 * QMI_WMS_EVENT_REPORT_IND (1)
 *
 * @param[in] msg_id indication id
 * @param[in] rsp indication
 * @param[in] len indication length
 * @param[out] out type according to the following
 *      QMI_WMS_EVENT_REPORT_IND (1)          -> #wms_ind_event_report_t
 *      QMI_WMS_MEMORY_FULL_IND  (63)         -> #wms_memory_full_info_t
 * @param[in] out_len buffer size
 */
QMI_SDK_error_t telit_wms_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len,
        uint8_t *out, uint16_t out_len);

#ifdef __cplusplus
}
#endif

#endif  /* _TELIT_SDK_WMS_H_ */
