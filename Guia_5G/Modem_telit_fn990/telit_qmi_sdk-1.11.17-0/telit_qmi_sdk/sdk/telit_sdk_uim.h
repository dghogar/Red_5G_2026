#ifndef TELIT_SDK_UIM_H_
#define TELIT_SDK_UIM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_uim.h
 * @brief UIM pack/unpack functions
 * Service ID: QMI_SVC_UIM (11)
 */

/**
 * @brief Enum to describe QMI UIM Message IDs
 */
typedef enum {
    /** 32 - 0x0020 Provide read access to a transparent file */
    QMI_UIM_READ_TRANSPARENT            = 0x0020,
    /** 32 - 0x0020 Read transparent response available indication */
    QMI_UIM_READ_TRANSPARENT_IND        = 0x0020,
    /** 33 - 0x0021 Provide read access to a specific record */
    QMI_UIM_READ_RECORD                 = 0x0021,
    /** 33 - 0x0021 Read record response available indication */
    QMI_UIM_READ_RECORD_IND             = 0x0021,

    /** 36 - 0x0024 Retrieve the file attributes for any EF or DF */
    QMI_UIM_GET_FILE_ATTRIBUTES         = 0x0024,
    /** 36 - 0x0024 Get file attributes response available indication */
    QMI_UIM_GET_FILE_ATTRIBUTES_IND     = 0x0024,
    /** 37 - 0x0025 Enable or disables the protection of the UIM */
    QMI_UIM_SET_PIN_PROTECTION          = 0x0025,
    /** 37 - 0x0025 Set pin protection response available indication */
    QMI_UIM_SET_PIN_PROTECTION_IND      = 0x0025,
    /** 38 - 0x0026 Verify pin */
    QMI_UIM_VERIFY_PIN                  = 0x0026,
    /** 38 - 0x0026 Verify pin response available indication */
    QMI_UIM_VERIFY_PIN_IND              = 0x0026,
    /** 39 - 0x0027 Unblock pin */
    QMI_UIM_UNBLOCK_PIN                 = 0x0027,
    /** 39 - 0x0027 Unblock pin response available indication */
    QMI_UIM_UNBLOCK_PIN_IND             = 0x0027,
    /** 40 - 0x0028 Change pin */
    QMI_UIM_CHANGE_PIN                  = 0x0028,
    /** 40 - 0x0028 Change pin response available indication */
    QMI_UIM_CHANGE_PIN_IND              = 0x0028,

    /** 46 - 0x002E Register for UIM indications */
    QMI_UIM_EVENT_REG                   = 0x002E,
    /** 47 - 0x002F Get card status */
    QMI_UIM_GET_CARD_STATUS             = 0x002F,
    /** 48 - 0x0030 Power down the card */
    QMI_UIM_POWER_DOWN                  = 0x0030,
    /** 49 - 0x0031 Power up the card */
    QMI_UIM_POWER_UP                    = 0x0031,
    /** 50 - 0x0032 Card status change indication */
    QMI_UIM_STATUS_CHANGE_IND           = 0x0032,

    /** 56 - 0x0038 Change the provisioning session */
    QMI_UIM_CHANGE_PROVISIONING_SESSION = 0x0038,

    /** 59 - 0x003B Send an APDU to the card */
    QMI_UIM_SEND_APDU                   = 0x003B,

    /** 63 - 0x003F Open/close a logical channel to an app on a UICC card */
    QMI_UIM_LOGICAL_CHANNEL             = 0x003F,

    /** 70 - 0x0046 Switch logical/physical slot */
    QMI_UIM_SWITCH_SLOT                 = 0x0046,
    /** 71 - 0x0047 Get slots status */
    QMI_UIM_GET_SLOTS_STATUS            = 0x0047,
    /** 72 - 0x0048 Get slots status change indication */
    QMI_UIM_GET_SLOTS_STATUS_IND        = 0x0048,
} QMI_UIM_message_t;

/**
 * @brief QMI UIM Session Types
 *
 */
typedef enum {
    /** GSM ICC or UICC application */
    QMI_UIM_SESSION_TYPE_PRIMARY_GW_PROVISIONING    = 0,
    /** CDMA ICC or CSIM application */
    QMI_UIM_SESSION_TYPE_PRIMARY_1X_PROVISIONING    = 1,
    /** GSM ICC or UICC application (dual standby) */
    QMI_UIM_SESSION_TYPE_SECONDARY_GW_PROVISIONING  = 2,
    /** CDMA ICC or CSIM application (dual standby) */
    QMI_UIM_SESSION_TYPE_SECONDARY_1X_PROVISIONING  = 3,
    /** Non provisioning application in slot 1 */
    QMI_UIM_SESSION_TYPE_NON_PROVISIONING_ON_SLOT_1 = 4,
    /** Non provisioning application in slot 2 */
    QMI_UIM_SESSION_TYPE_NON_PROVISIONING_ON_SLOT_2 = 5,
    /** Files not in any application of the card in slot 1 */
    QMI_UIM_SESSION_TYPE_CARD_ON_SLOT_1             = 6,
    /** Files not in any application of the card in slot 2 */
    QMI_UIM_SESSION_TYPE_CARD_ON_SLOT_2             = 7,
    /** Files available on the UICC card in slot 1 after opening a logical channel */
    QMI_UIM_SESSION_TYPE_LOGICAL_CHANNEL_ON_SLOT_1  = 8,
    /** Files available on the UICC card in slot 2 after opening a logical channel */
    QMI_UIM_SESSION_TYPE_LOGICAL_CHANNEL_ON_SLOT_2  = 9,
} QMI_UIM_session_type_t;

#define QMI_UIM_MAX_AID_LEN 60

/**
 * @brief QMI UIM session information type
 *
 */
typedef struct {
    /** Session type, see #QMI_UIM_session_type_t */
    enum8_t session_type;
    /** Application identifier length */
    uint8_t aid_len;
    /** Application identifier value or channel ID */
    uint8_t aid[QMI_UIM_MAX_AID_LEN];
} uim_session_information_t;

#define QMI_UIM_MAX_FILE_LENGTH 1856
/**
 * @brief input type for QMI_UIM_READ_TRANSPARENT
 *
 */
typedef struct {
    /** #uim_session_information_t instance */
    uim_session_information_t session_information;
    /** ID of the file to be read */
    uint16_t file_id;
    /** file path length (in bytes) */
    uint8_t file_path_len;
    /** file path, this value must be the complete path of the file, which is a sequence block of 2 bytes (e.g. 0x3F00, 0x7FFF) */
    uint16_t file_path[MAX_STRING_SIZE];
    /** set if indication_token should be considered */
    bool response_as_indication_available;
    /** Token used when response is returned as indication */
    uint32_t indication_token;
} uim_read_file_t;

/**
 * @brief QMI UIM result status codes type
 *
 */
typedef struct {
    /** SW1 status code received from the card */
    uint8_t sw1;
    /** SW2 status code received from the card */
    uint8_t sw2;
} uim_card_result_t;

/**
 * @brief QMI UIM read request results type
 *
 */
typedef struct {
    /** read request result size */
    uint16_t len;
    /**  content, sequence of bytes as read from the card */
    uint8_t content[QMI_UIM_MAX_FILE_LENGTH];
} uim_read_result_t;

/**
 * @brief output type for QMI_UIM_READ_TRANSPARENT
 *
 */
typedef struct {
    /** set if card_result is available */
    bool card_result_available;
    /** #uim_card_result_t instance */
    uim_card_result_t card_result;
    /** sef if read_result is available */
    bool read_result_available;
    /** #uim_read_result_t instance */
    uim_read_result_t read_result;
    /** set if indication_token is available */
    bool response_as_indication_available;
    /** Token used when response is returned as indication */
    uint32_t indication_token;
} uim_read_file_info_t;

/**
 * @brief Pack QMI_UIM_READ_TRANSPARENT (32 - 0x0020) request
 *
 * This function packs a qmi request for providing read access to
 * any transparent file in the card by the path
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_read_file_t
 */
QMI_SDK_error_t telit_uim_read_transparent_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_read_file_t *input);

/**
 * @brief Unpack QMI_UIM_READ_TRANSPARENT (32 - 0x0020) request
 *
 * This function unpacks the result of a QMI_UIM_READ_TRANSPARENT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #uim_read_file_info_t
 */
QMI_SDK_error_t telit_uim_read_transparent_unpack(uint8_t *rsp, uint16_t len, uim_read_file_info_t *out);

/**
 * @brief input type for QMI_UIM_READ_RECORD
 *
 */
typedef struct {
    /** #uim_session_information_t instance */
    uim_session_information_t session_information;
    /** ID of the file to be read */
    uint16_t file_id;
    /** file path length (in bytes) */
    uint8_t file_path_len;
    /** file path, this value must be the complete path of the file, which is a sequence block of 2 bytes (e.g. 0x3F00, 0x7FFF) */
    uint16_t file_path[MAX_STRING_SIZE];
    /** record number (starting from 1) */
    uint16_t record;
    /** length of the content to be read. The value 0 is used to read the complete record */
    uint16_t record_length;
    /** set if indication_token should be considered */
    bool response_as_indication_available;
    /** Token used when response is returned as indication */
    uint32_t indication_token;
} uim_read_record_t;

/**
 * @brief output type for QMI_UIM_READ_RECORD
 *
 */
typedef struct {
    /** set if card_result is available */
    bool card_result_available;
    /** #uim_card_result_t instance */
    uim_card_result_t card_result;
    /** sef if read_result is available */
    bool read_result_available;
    /** #uim_read_result_t instance */
    uim_read_result_t read_result;
    /** set if additional_read_result is available */
    bool additional_read_result_available;
    /** #uim_read_result_t instance */
    uim_read_result_t additional_read_result;
    /** set if indication_token is available */
    bool response_as_indication_available;
    /** Token used when response is returned as indication */
    uint32_t indication_token;
} uim_read_record_info_t;

/**
 * @brief Pack QMI_UIM_READ_RECORD (33 - 0x0021) request
 *
 * This function packs a qmi request for providing read access to
 * a specific record in a linear fixed or cyclic file in the card
 * and provides access by the path
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_read_record_t
 */
QMI_SDK_error_t telit_uim_read_record_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_read_record_t *input);

/**
 * @brief Unpack QMI_UIM_READ_RECORD (33 - 0x0021) request
 *
 * This function unpacks the result of a QMI_UIM_READ_RECORD request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #uim_read_record_info_t
 */
QMI_SDK_error_t telit_uim_read_record_unpack(uint8_t *rsp, uint16_t len, uim_read_record_info_t *out);

/**
 * @brief Pack QMI_UIM_GET_FILE_ATTRIBUTES (36 - 0x0024) request
 *
 * This function packs a qmi request for retrieving the file attributes
 * for any EF or DF in the card by the path
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_read_file_t
 */
QMI_SDK_error_t telit_uim_get_file_attributes_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_read_file_t *input);

/**
 * @brief Unpack QMI_UIM_GET_FILE_ATTRIBUTES (36 - 0x0024) request
 *
 * This function unpacks the result of a QMI_UIM_GET_FILE_ATTRIBUTES request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #uim_read_file_info_t
 */
QMI_SDK_error_t telit_uim_get_file_attributes_unpack(uint8_t *rsp, uint16_t len, uim_read_file_info_t *out);

/**
 * @brief QMI UIM encrypted PIN1 type
 *
 */
typedef struct {
    /** Encrypted PIN1 string length */
    uint8_t enc_pin1_len;
    /** Encrypted PIN1 */
    uint8_t enc_pin1[MAX_STRING_SIZE];
} uim_encrypted_pin1_t;

/**
 * @brief QMI UIM PIN IDs
 *
 */
typedef enum {
    /** PIN1 */
    QMI_UIM_PIN_ID_PIN1       = 0x01,
    /** PIN2 */
    QMI_UIM_PIN_ID_PIN2       = 0x02,
    /** Universal PIN */
    QMI_UIM_PIN_ID_UNIVERSAL  = 0x03,
    /** Hidden key */
    QMI_UIM_PIN_ID_HIDDEN_KEY = 0x04,
} QMI_UIM_pin_id_t;

/**
 * @brief QMI UIM PIN operations
 *
 */
typedef enum {
    /** Enable */
    QMI_UIM_PIN_OPERATION_DISABLE = 0x00,
    /** Disable */
    QMI_UIM_PIN_OPERATION_ENABLE  = 0x01,
} QMI_UIM_pin_operation_t;

#define QMI_UIM_MAX_PIN_LEN 255

/**
 * @brief QMI UIM pin information type
 *
 */
typedef struct {
    /** PIN ID to be changed, see #QMI_UIM_pin_id_t */
    enum8_t pin_id;
    /** Operation to be applied to PIN, see #QMI_UIM_pin_operation_t */
    enum8_t pin_operation;
    /** PIN value length */
    uint8_t pin_len;
    /** PIN */
    char pin[QMI_UIM_MAX_PIN_LEN];
} uim_set_pin_protection_pin_information_t;

/**
 * @brief input type for QMI_UIM_SET_PIN_PROTECTION
 *
 */
typedef struct {
    /** #uim_session_information_t instance */
    uim_session_information_t session_information;
    /** #uim_set_pin_protection_pin_information_t instance */
    uim_set_pin_protection_pin_information_t pin_info;
    /** set if pin_key_ref_id should be considered */
    bool key_reference_available;
    /** PIN key reference ID, valid values are from 1 to 8 as specified in ETSI TS 102 221, used only for PIN1 and PIN2 */
    uint8_t pin_key_ref_id;
    /** set if indication_token should be considered */
    bool response_as_indication_available;
    /** Token used when response is returned as indication */
    uint32_t indication_token;
} uim_pin_protection_info_t;

/**
 * @brief Pack QMI_UIM_SET_PIN_PROTECTION (37 - 0x0025) request
 *
 * This function packs a qmi request for enabling/disabling the
 * protection of the UIM contents by a specific PIN
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_pin_protection_info_t
 */
QMI_SDK_error_t telit_uim_set_pin_protection_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_pin_protection_info_t *input);

/**
 * @brief QMI UIM retries remaining type
 *
 */
typedef struct {
    /** Number of remaining attempts to verify the PIN */
    uint8_t verify_left;
    /** Number of remaining attempts to unblock the PIN */
    uint8_t unblock_left;
} uim_retries_remaining_t;

/**
 * @brief output type for QMI_UIM_PIN requests
 *
 */
typedef struct {
    /** set if retries_remaining is available */
    bool pin_retries_available;
    /** #uim_retries_remaining_t instance */
    uim_retries_remaining_t retries_remaining;
    /** set if encrypted_pin1 is available */
    bool encrypted_pin1_available;
    /** #uim_encrypted_pin1_t instance */
    uim_encrypted_pin1_t encrypted_pin1;
    /** set if indication_token is available */
    bool response_as_indication_available;
    /** Token used when response is returned as indication */
    uint32_t indication_token;
} uim_pin_resp_t;

/**
 * @brief Unpack QMI_UIM_SET_PIN_PROTECTION (37 - 0x0025) request
 *
 * This function unpacks the result of a QMI_UIM_SET_PIN_PROTECTION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #uim_pin_resp_t
 */
QMI_SDK_error_t telit_uim_set_pin_protection_unpack(uint8_t *rsp, uint16_t len, uim_pin_resp_t *out);

/**
 * @brief QMI UIM verify pin information type
 *
 */
typedef struct {
    /** PIN ID to be changed, see #QMI_UIM_pin_id_t */
    enum8_t pin_id;
    /** PIN value length */
    uint8_t pin_len;
    /** PIN */
    char pin[QMI_UIM_MAX_PIN_LEN];
} uim_verify_pin_pin_information_t;

/**
 * @brief input type for QMI_UIM_VERIFY_PIN
 *
 */
typedef struct {
    /** #uim_session_information_t instance */
    uim_session_information_t session_information;
    /** #uim_verify_pin_pin_information_t instance */
    uim_verify_pin_pin_information_t pin_info;
    /** set if encrypted_pin1 should be considered */
    bool encrypted_pin1_available;
    /** #uim_encrypted_pin1_t instance */
    uim_encrypted_pin1_t encrypted_pin1;
    /** set if pin_key_ref_id should be considered */
    bool key_reference_available;
    /** PIN key reference ID, valid values are from 1 to 8 as specified in ETSI TS 102 221, used only for PIN1 and PIN2 */
    uint8_t pin_key_ref_id;
    /** set if indication_token should be considered */
    bool response_as_indication_available;
    /** Token used when response is returned as indication */
    uint32_t indication_token;
} uim_verify_pin_info_t;

/**
 * @brief Pack QMI_UIM_VERIFY_PIN (38 - 0x0026) request
 *
 * This function packs a qmi request for verifying the PIN
 * before the card content is accessed
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_verify_pin_info_t
 *
 */
QMI_SDK_error_t telit_uim_verify_pin_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_verify_pin_info_t *input);

/**
 * @brief Unpack QMI_UIM_VERIFY_PIN (38 - 0x0026) request
 *
 * This function unpacks the result of a QMI_UIM_VERIFY_PIN request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #uim_pin_resp_t
 */
QMI_SDK_error_t telit_uim_verify_pin_unpack(uint8_t *rsp, uint16_t len, uim_pin_resp_t *out);

/**
 * @brief QMI UIM unblock pin information type
 *
 */
typedef struct {
    /** PIN ID to be changed, see #QMI_UIM_pin_id_t */
    enum8_t pin_id;
    /** PUK value length */
    uint8_t puk_len;
    /** PUK */
    char puk[QMI_UIM_MAX_PIN_LEN];
    /** New PIN value length */
    uint8_t new_pin_len;
    /** New PIN */
    char new_pin[QMI_UIM_MAX_PIN_LEN];
} uim_unblock_pin_pin_information_t;

/**
 * @brief input type for QMI_UIM_UNBLOCK_PIN
 *
 */
typedef struct {
    /** #uim_session_information_t instance */
    uim_session_information_t session_information;
    /** #uim_unblock_pin_pin_information_t instance */
    uim_unblock_pin_pin_information_t pin_info;
    /** set if pin_key_ref_id should be considered */
    bool key_reference_available;
    /** PIN key reference ID, valid values are from 1 to 8 as specified in ETSI TS 102 221, used only for PIN1 and PIN2 */
    uint8_t pin_key_ref_id;
    /** set if indication_token should be considered */
    bool response_as_indication_available;
    /** Token used when response is returned as indication */
    uint32_t indication_token;
} uim_unblock_pin_info_t;

/**
 * @brief Pack QMI_UIM_UNBLOCK_PIN (39 - 0x0027) request
 *
 * This function packs a qmi request for unblocking a blocked PIN
 * using the PUK code
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_unblock_pin_info_t
 *
 */
QMI_SDK_error_t telit_uim_unblock_pin_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_unblock_pin_info_t *input);

/**
 * @brief Unpack QMI_UIM_UNBLOCK_PIN (39 - 0x0027) request
 *
 * This function unpacks the result of a QMI_UIM_UNBLOCK_PIN request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #uim_pin_resp_t
 */
QMI_SDK_error_t telit_uim_unblock_pin_unpack(uint8_t *rsp, uint16_t len, uim_pin_resp_t *out);

/**
 * @brief QMI UIM change pin information type
 *
 */
typedef struct {
    /** PIN ID to be changed, see #QMI_UIM_pin_id_t */
    enum8_t pin_id;
    /** Old PIN value length */
    uint8_t old_pin_len;
    /** Old PIN */
    char old_pin[QMI_UIM_MAX_PIN_LEN];
    /** New PIN value length */
    uint8_t new_pin_len;
    /** New PIN */
    char new_pin[QMI_UIM_MAX_PIN_LEN];
} uim_change_pin_pin_information_t;

/**
 * @brief input type for QMI_UIM_CHANGE_PIN
 *
 */
typedef struct {
    /** #uim_session_information_t instance */
    uim_session_information_t session_information;
    /** #uim_change_pin_pin_information_t instance */
    uim_change_pin_pin_information_t pin_info;
    /** set if pin_key_ref_id should be considered */
    bool key_reference_available;
    /** PIN key reference ID, valid values are from 1 to 8 as specified in ETSI TS 102 221, used only for PIN1 and PIN2 */
    uint8_t pin_key_ref_id;
    /** set if indication_token should be considered */
    bool response_as_indication_available;
    /** Token used when response is returned as indication */
    uint32_t indication_token;
} uim_change_pin_info_t;

/**
 * @brief Pack QMI_UIM_CHANGE_PIN (40 - 0x0028) request
 *
 * This function packs a qmi request for changing the value
 * of the specified PIN
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_change_pin_info_t
 *
 */
QMI_SDK_error_t telit_uim_change_pin_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_change_pin_info_t *input);

/**
 * @brief Unpack QMI_UIM_CHANGE_PIN (40 - 0x0028) request
 *
 * This function unpacks the result of a QMI_UIM_CHANGE_PIN request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #uim_pin_resp_t
 */
QMI_SDK_error_t telit_uim_change_pin_unpack(uint8_t *rsp, uint16_t len, uim_pin_resp_t *out);

/**
 * @brief QMI UIM event registration elements
 *
 */
typedef enum {
    /** Card status */
    QMI_UIM_EVENT_REG_CARD_STATUS          = (1 << 0),
    /** Physical slot status */
    QMI_UIM_EVENT_REG_PHYSICAL_SLOT_STATUS = (1 << 4),
} QMI_UIM_event_t;

/**
 * @brief QMI UIM event registration mask type
 *
 */
typedef struct {
    /** set in response if bitmask is available, not considered in request **/
    bool bitmask_available;
    /** Bitmask of the events to be received by the client, see #QMI_UIM_event_t */
    uint32_t bitmask;
} uim_event_reg_info_t;

/**
 * @brief Pack QMI_UIM_EVENT_REG (46 - 0x002E) request
 *
 * This function packs a qmi request for registering for event
 * notifications from the card.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_event_reg_info_t
 */
QMI_SDK_error_t telit_uim_event_reg_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        uim_event_reg_info_t *input);

/**
 * @brief Unpack QMI_UIM_EVENT_REG (46 - 0x002E) request
 *
 * This function unpacks the result of a QMI_UIM_EVENT_REG request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_uim_event_reg_unpack(uint8_t *rsp, uint16_t len, uim_event_reg_info_t *out);

/**
 * @brief QMI UIM card errors
 *
 */
typedef enum {
    /** Unknown */
    QMI_UIM_CARD_ERROR_CODE_UNKNOWN                = 0x00,
    /** Power down */
    QMI_UIM_CARD_ERROR_CODE_POWER_DOWN             = 0x01,
    /** Poll error */
    QMI_UIM_CARD_ERROR_CODE_POLL_ERROR             = 0x02,
    /** No ATR received */
    QMI_UIM_CARD_ERROR_CODE_NO_ATR_RECEIVED        = 0x03,
    /** Volt mismatch */
    QMI_UIM_CARD_ERROR_CODE_VOLT_MISMATCH          = 0x04,
    /** Parity error */
    QMI_UIM_CARD_ERROR_CODE_PARITY_ERROR           = 0x05,
    /** Unknown, possibly removed */
    QMI_UIM_CARD_ERROR_CODE_POSSIBILY_REMOVED      = 0x06,
    /** Card returned technical problems */
    QMI_UIM_CARD_ERROR_CODE_SIM_TECHNICAL_PROBLEMS = 0x07,
    /** Card returned NULL bytes */
    QMI_UIM_CARD_ERROR_CODE_NULL_BYTES             = 0x08,
    /** Terminal in SAP mode */
    QMI_UIM_CARD_ERROR_CODE_SAP_CONNECTED          = 0x09,
    /** Command timeout error */
    QMI_UIM_CARD_ERROR_CODE_CMD_TIMEOUT            = 0x0A,
} QMI_UIM_card_error_t;

/**
 * @brief QMI UIM app types
 *
 */
typedef enum {
    /** Unkown */
    QMI_UIM_APP_TYPE_UNKNOWN = 0x00,
    /** SIM card */
    QMI_UIM_APP_TYPE_SIM     = 0x01,
    /** USIM application */
    QMI_UIM_APP_TYPE_USIM    = 0x02,
    /** RUIM card */
    QMI_UIM_APP_TYPE_RUIM    = 0x03,
    /** CSIM application */
    QMI_UIM_APP_TYPE_CSIM    = 0x04,
    /** ISIM application */
    QMI_UIM_APP_TYPE_ISIM    = 0x05,
} QMI_UIM_app_type_t;

/**
 * @brief QMI UIM app states
 *
 */
typedef enum {
    /** Unknown */
    QMI_UIM_APP_STATE_UNKNOWN           = 0x00,
    /** Detected */
    QMI_UIM_APP_STATE_DETECTED          = 0x01,
    /** PIN1 or UPIN is required */
    QMI_UIM_APP_STATE_PIN1_OR_UPIN_REQ  = 0x02,
    /** PUK1 or PUK for UPIN is required */
    QMI_UIM_APP_STATE_PUK1_OR_PUK_REQ   = 0x03,
    /** Personalization state should be checked */
    QMI_UIM_APP_STATE_PERSON_CHECK_REQ  = 0x04,
    /** PIN1 is blocked */
    QMI_UIM_APP_STATE_PIN1_PERM_BLOCKED = 0x05,
    /** Illegal */
    QMI_UIM_APP_STATE_ILLEGAL           = 0x06,
    /** Ready */
    QMI_UIM_APP_STATE_READY             = 0x07,
} QMI_UIM_app_state_t;

/**
 * @brief QMI UIM personalization states
 *
 */
typedef enum {
    /** Unknown */
    QMI_UIM_PERSO_STATE_UNKNOWN             = 0x00,
    /** Personalization operation is in progress */
    QMI_UIM_PERSO_STATE_IN_PROGRESS         = 0x01,
    /** Ready */
    QMI_UIM_PERSO_STATE_READY               = 0x02,
    /** Personalization code is required */
    QMI_UIM_PERSO_STATE_CODE_REQ            = 0x03,
    /** PUK for personalization code is required */
    QMI_UIM_PERSO_STATE_PUK_REQ             = 0x04,
    /** Permanently blocked */
    QMI_UIM_PERSO_STATE_PERMANENTLY_BLOCKED = 0x05,
} QMI_UIM_perso_state_t;

/**
 * @brief QMI UIM personalization features
 *
 */
typedef enum {
    /** GW network personalization */
    QMI_UIM_PERSO_FEATURE_GW_NETWORK                                = 0x00,
    /** GW network subset personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_GW_NETWORK_SUBSET                  = 0x01,
    /** GW service provider personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_GW_SERVICE_PROVIDER                = 0x02,
    /** GW corporate personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_GW_CORPORATE                       = 0x03,
    /** GW UIM personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_GW_UIM                             = 0x04,
    /** 1X network type 1 personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_1X_NETWORK_TYPE_1                  = 0x05,
    /** 1X network type 2 personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_1X_NETWORK_TYPE_2                  = 0x06,
    /** HRPD personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_1X_HRPD                            = 0x07,
    /** 1X service provider personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_1X_SERVICE_PROVIDER                = 0x08,
    /** 1X corporate personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_1X_CORPORATE                       = 0x09,
    /** 1X RUIM personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_1X_RUIM                            = 0x0A,
    /** Unknown */
    QMI_UIM_PERSO_FEATURE_STATUS_UNKNOWN                            = 0x0B,
    /** GW service provider personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_GW_SERVICE_PROVIDER_NAME           = 0x0C,
    /** GW service provider with equivalent HPLMN personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_GW_SP_EHPLMN                       = 0x0D,
    /** GW ICCID personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_GW_ICCID                           = 0x0E,
    /** GW IMPI personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_GW_IMPI                            = 0x0F,
    /** GW network subset and service provider personalization */
    QMI_UIM_PERSO_FEATURE_STATUS_GW_NETWORK_SUBSET_SERVICE_PROVIDER = 0x10,
} QMI_UIM_perso_feature_t;

/**
 * @brief QMI UIM UPIN enablement values
 *
 */
typedef enum {
    /** PIN1 is used */
    QMI_UIM_UNIV_PIN_PIN1_USED          = 0x00,
    /** UPIN replaces PIN1 */
    QMI_UIM_UNIV_PIN_UPIN_REPLACES_PIN1 = 0x01,
} QMI_UIM_UPIN_t;

/**
 * @brief QMI UIM hot-swap status
 *
 */
typedef enum {
    /** Hot swap not supported */
    QMI_UIM_HOT_SWAP_NOT_SUPPORTED        = 0x00,
    /** Hot swap is supported, but the status of the switch is not supported */
    QMI_UIM_HOT_SWAP_STATUS_NOT_SUPPORTED = 0x01,
    /** Switch indicates that the card is present */
    QMI_UIM_HOT_SWAP_STATUS_PRESENT       = 0x02,
    /** Switch indicates that the card is not present */
    QMI_UIM_HOT_SWAP_STATUS_NOT_PRESENT   = 0x03,
} QMI_UIM_hot_swap_status_t;

#define QMI_UIM_MAX_SIZE_HOT_SWAP 10
/**
 * @brief QMI UIM hot swap status type
 *
 */
typedef struct {
    /** Hot Swap status array length */
    uint8_t hot_swap_len;
    uint8_t hot_swap[QMI_UIM_MAX_SIZE_HOT_SWAP];  /** Indicates the status pf the hot-swap switch, see #QMI_UIM_hot_swap_status_t */
} uim_hot_swap_t;

/**
 * @brief QMI UIM PIN States
 *
 */
typedef enum {
    /** Unknown */
    QMI_UIM_PIN_STATE_UNKNOWN              = 0x00,
    /** Enabled and not verified */
    QMI_UIM_PIN_STATE_ENABLED_NOT_VERIFIED = 0x01,
    /** Enabled and verified */
    QMI_UIM_PIN_STATE_ENABLED_VERIFIED     = 0x02,
    /** Disabled */
    QMI_UIM_PIN_STATE_DISABLED             = 0x03,
    /** Blocked */
    QMI_UIM_PIN_STATE_BLOCKED              = 0x04,
    /** Permanently blocked */
    QMI_UIM_PIN_STATE_PERMANENTLY_BLOCKED  = 0x05,
} QMI_UIM_pin_state_t;

/**
 * @brief QMI UIM app info type
 *
 */
typedef struct {
    /** Indicates the app type, see #QMI_UIM_app_type_t */
    enum8_t type;
    /** Indicates the app state, see #QMI_UIM_app_state_t */
    enum8_t state;
    /** Indicates the personalization state, see #QMI_UIM_perso_state_t */
    enum8_t perso_state;
    /** Indicates the personalization feature, see #QMI_UIM_perso_feature_t */
    enum8_t perso_feature;
    /** Indicates the number of retries remaining to disable the personalization */
    uint8_t perso_retries;
    /** Indicates the number of retries remaining to disable the personalization */
    uint8_t perso_unblock_retries;
    /** Indicates the AID string length */
    uint8_t aid_len;
    uint8_t aid[QMI_UIM_MAX_AID_LEN];  /* Application identifier */
    /** Indicates wether UPIN replaces PIN1, see #QMI_UIM_UPIN_t */
    enum8_t univ_pin;
    /** Indicates PIN1 state, see #QMI_UIM_pin_state_t */
    enum8_t pin1_state;
    /** Indicates the number of retries remaining to verify PIN1 */
    uint8_t pin1_retries;
    /** Indicates the number of retries remaining to unblock PIN1 */
    uint8_t puk1_retries;
    /** Indicates PIN2 state, see #QMI_UIM_pin_state_t */
    enum8_t pin2_state;
    /** Indicates the number of retries remaining to verify PIN2 */
    uint8_t pin2_retries;
    /** Indicates the number of retries remaining to unblock PIN2 */
    uint8_t puk2_retries;
} uim_card_status_slot_app_t;

/**
 * @brief QMI UIM Card States
 *
 */
typedef enum {
    /** Absent */
    QMI_UIM_CARD_STATE_ABSENT  = 0x00,
    /** Present */
    QMI_UIM_CARD_STATE_PRESENT = 0x01,
    /** Error */
    QMI_UIM_CARD_STATE_ERROR   = 0x02,
    /** Unknown */
    QMI_UIM_CARD_STATE_UNKNOWN = 0x03,
} QMI_UIM_card_state_t;

#define QMI_UIM_MAX_SIZE_APP 6

/**
 * @brief QMI UIM slot info type
 *
 */
typedef struct {
    /** Indicates the state of the card, see #QMI_UIM_card_state_t */
    enum8_t card_state;
    /** Indicates the state of UPIN, see #QMI_UIM_pin_state_t */
    enum8_t upin_state;
    /** Indicates the number of retries remaining to verify the UPIN */
    uint8_t upin_retries;
    /** Indicates the number of retries remaining to unblock the UPIN */
    uint8_t upuk_retries;
    /** Indicates the reason of the card error, see #QMI_UIM_card_error_t */
    enum8_t error;
    /** App array length */
    uint8_t num_app;
    /** Array of #uim_card_status_slot_t uim_card_status_slot_app_t */
    uim_card_status_slot_app_t app[QMI_UIM_MAX_SIZE_APP];
} uim_card_status_slot_t;

#define QMI_UIM_MAX_SIZE_SLOT 4
/**
 * @brief QMI UIM card status type
 *
 */
typedef struct {
    /** Index of the primary GW provisioning application.
     * The most significant byte indicates the slot (starting from 0), while the least indicates the application for that slot (starting from 0).
     * The value 0xFFFF identifies that the session does not exist.
     */
    uint16_t index_gw_pri;
    /** Index of the primary 1x provisioning application.
     * The most significant byte indicates the slot (starting from 0), while the least indicates the application for that slot (starting from 0).
     * The value 0xFFFF identifies that the session does not exist.
     */
    uint16_t index_1x_pri;
    /** Index of the secondary GW provisioning application.
     * The most significant byte indicates the slot (starting from 0), while the least indicates the application for that slot (starting from 0).
     * The value 0xFFFF identifies that the session does not exist.
     */
    uint16_t index_gw_sec;
    /** Index of the secondary 1x provisioning application.
     * The most significant byte indicates the slot (starting from 0), while the least indicates the application for that slot (starting from 0).
     * The value 0xFFFF identifies that the session does not exist.
     */
    uint16_t index_1x_sec;
    /** Slot status array length */
    uint8_t num_slot;
    /** Array of #uim_card_status_slot_t instances */
    uim_card_status_slot_t slot[QMI_UIM_MAX_SIZE_SLOT];
} uim_card_status_t;

/**
 * @brief QMI UIM slot status info type
 *
 */
typedef struct {
    /** set if card_status is available */
    bool card_status_available;
    /** #uim_card_status_t instance */
    uim_card_status_t card_status;
    /** set if hot_swap_status is available */
    bool hot_swap_available;
    /** #uim_hot_swap_t instance */
    uim_hot_swap_t hot_swap_status;
    /** valid_card_status array size */
    uint8_t valid_card_status_len;
    /** Indicates whether the reported status of
       the card on the corresponding slot is valid, according to the following values:
       - false - status of the card is unknown (even if reported as absent)
       - true - status of the card is valid
     */
    bool valid_card_status[QMI_UIM_MAX_SIZE_SLOT];
} uim_card_status_info_t;

/**
 * @brief Pack QMI_UIM_GET_CARD_STATUS (47 - 0x002F) request
 *
 * This function packs a qmi request for retrieving the current
 * status of the card
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_uim_get_card_status_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_UIM_GET_CARD_STATUS (47 - 0x002F) request
 *
 * This function unpacks the result of a QMI_UIM_GET_CARD_STATUS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #uim_slot_status_info_t
 */
QMI_SDK_error_t telit_uim_get_card_status_unpack(uint8_t *rsp, uint16_t len, uim_card_status_info_t *out);

/**
 * @brief QMI UIM power down/up slot info type
 *
 */
typedef struct {
    /** Slot whose power setting should be changed, range 1 - 5 */
    uint8_t slot;
} uim_power_slot_info_t;

/**
 * @brief Pack QMI_UIM_POWER_DOWN (48 - 0x0030) request
 *
 * This function packs a qmi request for powering down the chosen slot
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_power_slot_info_t
 */
QMI_SDK_error_t telit_uim_power_down_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_power_slot_info_t *input);

/**
 * @brief Unpack QMI_UIM_POWER_DOWN (48 - 0x0030) request
 *
 * This function unpacks the result of a QMI_UIM_POWER_DOWN request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_uim_power_down_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_UIM_POWER_UP (49 - 0x0031) request
 *
 * This function packs a qmi request for powering up the chosen slot
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_power_slot_info_t
 */
QMI_SDK_error_t telit_uim_power_up_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_power_slot_info_t *input);

/**
 * @brief Unpack QMI_UIM_POWER_UP (49 - 0x0031) request
 *
 * This function unpacks the result of a QMI_UIM_POWER_UP request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_uim_power_up_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI UIM application information type
 *
 */
typedef struct {
    /** Slot to use, range 1 - 5 */
    uint8_t slot;
    /** Application identifier length, to be used when opening the channel */
    uint8_t aid_len;
    /** Application identifier to be opened*/
    uint8_t aid[QMI_UIM_MAX_AID_LEN];
} uim_application_info_t;

/**
 * @brief QMI UIM change provisioning session type
 *
 */
typedef struct {
    /** Session type, see #QMI_UIM_session_type_t */
    enum8_t session_type;
    /** Indicates whether to deactivate or activate the provisioning session */
    bool activate;
    /** set if application_info is available */
    bool application_info_available;
    /** Application information, see #uim_application_info_t */
    uim_application_info_t application_info;
} uim_change_provisioning_session_t;

/**
 * @brief Pack QMI_UIM_CHANGE_PROVISIONING_SESSION (56 - 0x0038) request
 *
 * This function packs a qmi request for changing the provisioning session
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_change_provisioning_session_t
 */
QMI_SDK_error_t telit_uim_change_provisioning_session_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_change_provisioning_session_t *input);

/**
 * @brief Unpack QMI_UIM_CHANGE_PROVISIONING_SESSION (56 - 0x0038) request
 *
 * This function unpacks the result of a QMI_UIM_CHANGE_PROVISIONING_SESSION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_uim_change_provisioning_session_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI UIM Procedure bytes setting
 *
 */
typedef enum {
    /** Expect intermediate procedure bytes */
    QMI_UIM_APDU_RETURN_PROCEDURE_BYTES = 0x00,
    /** Expect final result only */
    QMI_UIM_APDU_SKIP_PROCEDURE_BYTES   = 0x01,
} QMI_UIM_procedure_bytes_setting_t;

#define QMI_UIM_MAX_APDU_SZ 2048

/**
 * @brief QMI UIM send APDU info type
 *
 */
typedef struct {
    /** Slot to use, range 1 - 5 */
    uint8_t slot;
    /** apdu size */
    uint16_t apdu_sz;
    /** APDU */
    uint8_t apdu[QMI_UIM_MAX_APDU_SZ];
    /** set if channel_id is available */
    bool channel_id_available;
    /** channel id to use */
    uint8_t channel_id;
    /** set if procedure_bytes_setting is available */
    bool procedure_bytes_setting_available;
    /** procedure bytes setting, see #QMI_UIM_procedure_bytes_setting_t */
    enum8_t procedure_bytes_setting;
} uim_send_apdu_info_t;

/**
 * @brief QMI UIM long APDU response info type
 *
 */
typedef struct {
    /** Total length of the APDU data for long responses */
    uint16_t total_length;
    /** Token used in the subsequent indications with APDU response */
    uint32_t token;
} uim_long_apdu_response_info_t;

/**
 * @brief QMI UIM send APDU response type
 *
 */
typedef struct {
    /** apdu size */
    uint16_t apdu_sz;
    /** APDU */
    uint8_t apdu[QMI_UIM_MAX_APDU_SZ];
    /** set if long_apdu_response_info is available */
    bool long_apdu_response_info_available;
    /** Token and total length to reconstruct the APDU response from the card */
    uim_long_apdu_response_info_t long_apdu_response_info;
} uim_send_apdu_response_t;

/**
 * @brief Pack QMI_UIM_SEND_APDU (59 - 0x003B) request
 *
 * This function packs a qmi request for sending an APDU
 * to the card
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_switch_slot_info_t
 */
QMI_SDK_error_t telit_uim_send_apdu_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_send_apdu_info_t *input);

/**
 * @brief Unpack QMI_UIM_SEND_APDU (59 - 0x003B) request
 *
 * This function unpacks the result of a QMI_UIM_SEND_APDU request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #uim_logical_channel_resp_t
 */
QMI_SDK_error_t telit_uim_send_apdu_unpack(uint8_t *rsp, uint16_t len,
        uim_send_apdu_response_t *out);

/**
 * @brief QMI UIM logical channel info type
 *
 */
typedef struct {
    /** Slot to use, range 1 - 5 */
    uint8_t slot;
    /** Application identifier length, to be used when opening the channel */
    uint8_t aid_len;
    /** Application identifier to be opened*/
    uint8_t aid[QMI_UIM_MAX_AID_LEN];
    /** set if channel_id_to_close is available */
    bool channel_id_to_close_available;
    /** channel id of the channel to be closed */
    uint8_t channel_id_to_close;
} uim_logical_channel_info_t;

#define QMI_UIM_MAX_SELECT_RESPONSE_SZ 255
#define QMI_UIM_MAX_LONG_SELECT_RESPONSE_SZ 2048

/**
 * @brief QMI UIM logical channel info response
 *
 */
typedef struct {
    /** set if channel_id is available following an open */
    bool channel_id_available;
    /** opened channel id */
    uint8_t channel_id;
    /** set if card_result is available */
    bool card_result_available;
    /** #uim_card_result_t instance */
    uim_card_result_t card_result;
    /** select_response size */
    uint8_t select_response_sz;
    /** Raw value of the response sent by the card to the initial SELECT command */
    uint8_t select_response[QMI_UIM_MAX_SELECT_RESPONSE_SZ];
    /** long_select_response size */
    uint16_t long_select_response_sz;
    /** Raw value of the response sent by the card to the initial SELECT command if longer than 255 */
    uint8_t long_select_response[QMI_UIM_MAX_LONG_SELECT_RESPONSE_SZ];
} uim_logical_channel_resp_t;

/**
 * @brief Pack QMI_UIM_LOGICAL_CHANNEL (63 - 0x003F) request
 *
 * This function packs a qmi request for opening/closing a
 * logical channel to an application on a UICC card
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_switch_slot_info_t
 */
QMI_SDK_error_t telit_uim_logical_channel_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_logical_channel_info_t *input);

/**
 * @brief Unpack QMI_UIM_LOGICAL_CHANNEL (63 - 0x003F) request
 *
 * This function unpacks the result of a QMI_UIM_LOGICAL_CHANNEL request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #uim_logical_channel_resp_t
 */
QMI_SDK_error_t telit_uim_logical_channel_unpack(uint8_t *rsp, uint16_t len, uim_logical_channel_resp_t *out);

/**
 * @brief QMI UIM switch slot info type
 *
 */
typedef struct {
    /** Logical slot that is switched to physical slot, range 1 - 5 */
    uint8_t logical_slot;
    /** Physical slot that the logical slot is to be switched to, range 1 - 5 */
    uint32_t physical_slot;
} uim_switch_slot_info_t;

/**
 * @brief Pack QMI_UIM_SWITCH_SLOT (70 - 0x0046) request
 *
 * This function packs a qmi request for switching the binding
 * between a logical slot and a physical slot
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #uim_switch_slot_info_t
 */
QMI_SDK_error_t telit_uim_switch_slot_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_switch_slot_info_t *input);

/**
 * @brief Unpack QMI_UIM_SWITCH_SLOT (70 - 0x0046) request
 *
 * This function unpacks the result of a QMI_UIM_SWITCH_SLOT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_uim_switch_slot_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI UIM Physical Card Status
 *
 */
typedef enum {
    QMI_UIM_PHYSICAL_CARD_STATE_UNKNOWN = 0x00,
    QMI_UIM_PHYSICAL_CARD_STATE_ABSENT  = 0x01,
    QMI_UIM_PHYSICAL_CARD_STATE_PRESENT = 0x02,
} QMI_UIM_physical_card_status_t;

/**
 * @brief QMI UIM Physical Slot states
 *
 */
typedef enum {
    QMI_UIM_PHYSICAL_SLOT_STATE_INACTIVE = 0x00,
    QMI_UIM_PHYSICAL_SLOT_STATE_ACTIVE   = 0x01,
} QMI_UIM_physical_slot_state_t;

#define QMI_UIM_MAX_SLOTS 10
#define QMI_UIM_MAX_ICCID_SIZE 128

/**
 * @brief QMI UIM slot status info type
 *
 */
typedef struct {
    /** status of the card in the physical slot, see #QMI_UIM_physical_card_status_t */
    enum32_t physical_card_status;
    /** state of the physical slot, see #QMI_UIM_physical_slot_state_t */
    enum32_t physical_slot_state;
    /** logical slot associated with this physical slot. This is valid only if the physical slot is active, range 1 - 4 */
    enum8_t logical_slot;
    /** iccid length */
    uint8_t iccid_len;
    /** iccid of the card in the physical slot */
    uint8_t iccid[QMI_UIM_MAX_ICCID_SIZE];
} uim_slot_status_t;

typedef struct {
    /** number of slot instances in slot */
    uint8_t slot_size;
    /** array of uim_slot_status_t instances */
    uim_slot_status_t slot[QMI_UIM_MAX_SLOTS];
} uim_slot_status_info_t;

/**
 * @brief Pack QMI_UIM_GET_SLOTS_STATUS (71 - 0x0047) request
 *
 * This function packs a qmi request for retrieving the current
 * status of the physical and logical slots
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_uim_get_slots_status_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_UIM_GET_SLOTS_STATUS (71 - 0x0047) request
 *
 * This function unpacks the result of a QMI_UIM_GET_SLOTS_STATUS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #uim_slot_status_info_t
 */
QMI_SDK_error_t telit_uim_get_slots_status_unpack(uint8_t *rsp, uint16_t len, uim_slot_status_info_t *out);

/**
 * Unpack uim indications
 * @param[in] msg_id indication id
 * @param[in] rsp indication
 * @param[in] len indication length
 * @param[out] out type according to the following
 *      QMI_UIM_STATUS_CHANGE_IND      (50 - 0x0032)   -> #uim_card_status_info_t
 *      QMI_UIM_SLOT_STATUS_CHANGE_IND (72 - 0x0048)   -> #uim_slot_status_info_t
 * @param[in] out_len buffer size
 */
QMI_SDK_error_t telit_uim_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len,
        uint8_t *out, uint16_t out_len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_UIM_H_ */
