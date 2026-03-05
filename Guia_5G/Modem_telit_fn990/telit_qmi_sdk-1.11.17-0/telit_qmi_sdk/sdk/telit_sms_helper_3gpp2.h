/**
 * @file telit_sms_helper_3gpp2.h
 * @brief sms helper functions for 3GPP2
 */
#ifndef _TELIT_SMS_HELPER_3GPP2_H_
#define _TELIT_SMS_HELPER_3GPP2_H_

#include <stdint.h>
#include "telit_sms_helper.h"

#define BCD_DECODER(byteC) (int)(((byteC >> 4) & 0x0F) * 10 + (byteC & 0x0F))

#define SMS_3GPP2_SMS_MAX_ADDR_FIELDS       130
#define SMS_3GPP2_SMS_DIGIT_MODE_8BIT_ASCII 1
#define SMS_3GPP2_SMS_NUM_MODE_DIGIT        0
#define SMS_3GPP2_SMS_DIGIT_MODE_4BIT_DTMF  0
#define SMS_3GPP2_SMS_ENCODING_7BIT_ASCII   2
#define SMS_3GPP2_SMS_TELEPHONY_NUM_PLAN    1
#define SMS_3GPP2_SMS_MSG_TYPE_SUBMIT       2

#define PARAMETER_TELESERVICE_ID            0  /* Teleservice Identify, C.S0015-B v2.0, 3.4.3.1 */
#define PARAMETER_SERVICE_CATEGORY          1  /* Service Category, C.S0015-B v2.0, 3.4.3.2 */
#define PARAMETER_ORIGINATE_ADDRESS         2  /* Originate Address, C.S0015-B v2.0, 3.4.3.3 */
#define PARAMETER_ORIGINATE_SUBADDRESS      3  /* Originate Subaddress, C.S0015-B v2.0, 3.4.3.4 */
#define PARAMETER_DESTINATION_ADDRESS       4  /* Destination Address,  C.S0015-B v2.0, 3.4.3.3 */
#define PARAMETER_DESTINATION_SUBADDRESS    5  /* Destination Subaddress, C.S0015-B v2.0, 3.4.3.4 */
#define PARAMETER_BEARER_REPLY_OPTION       6  /* Bearer Reply Option, C.S0015-B v2.0, 3.4.3.5 */
#define PARAMETER_CAUSE_CODE                7  /* Cause Code, C.S0015-B v2.0, 3.4.3.6 */
#define PARAMETER_BEARER_DATA               8  /* Bearer Data, C.S0015-B v2.0, 3.4.3.7 */

#define SUBPARAMETER_MESSAGE_ID             0  /* Bearer Data - Message ID, C.S0015-B v2.0, 4.5.1 */
#define SUBPARAMETER_USER_DATA              1  /* Bearer Data - User data, C.S0015-B v2.0, 4.5.2 */
#define SUBPARAMETER_VALIDITY_PERIOD_REL    5  /* Bearer Data - Validity period relative, C.S0015-B v2.0, 4.5.6 */
#define SUBPARAMETER_PRIORITY               8  /* Bearer Data - Priority, C.S0015-B v2.0, 4.5.9 */
#define SUBPARAMETER_CALLBACK_NUMBER        14 /* Bearer Data - Call-Back Number, C.S0015-B v2.0, 4.5.15 */

#define PDU_3GPP2_SIZE 500
#define SMS_3GPP2_MAX_ADDR_FIELDS 130
#define TIME_DATE_STR_SIZE 16
#define MAX_MESSAGE_PAYLOAD 1000
#define MAX_ADDRESS_LENGTH 120

#define PARSE_SEMI_OCTET(a) ((a & 0xF) * 10) + (a >> 4)

/**
 * @brief Telit SMS helper date type
 *
 */
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
    int relative;
} time_date_t;

/**
 * @brief Telit SMS helper 3GPP2 address type
 *
 */
typedef struct {
    uint8_t digit_mode;  /* 0: 4bit DTMF, 1: 8-bit ASCII */
    uint8_t number_mode; /* 0: the address format is specified by ATIS-1000607.2000 (R2009), 1: the address format is data network address */
    uint8_t number_type; /* (unused) if digitMode=1 and numberMode=0 see table 2.7.1.3.2.4-2 of 3GPP2 C.S0005-E v3.0; if digitMode=1 and numberMode=1 then 0: unknown, 1: Internet Protocol, 2: Internet Email Address */
    uint8_t number_plan; /* (unused) if digitMode=1 and numberMode=0 see table 2.7.1.3.2.4-3 of 3GPP2 C.S0005-E v3.0; otherwise it is omitted */
    uint8_t num_fields;  /* number of address digits or characters */
    char address[SMS_3GPP2_SMS_MAX_ADDR_FIELDS];
} address_3gpp2_t;

/**
 * @brief Telit SMS helper 3GPP2 beared data type
 *
 */
typedef struct {
    int64_t bitmask;
    char msg_type;                     /* 1:Deliver,2:Submit,3:Cancellation,4:DeliveryAck,5:UserAck,6:ReadAck,7:DeliveryReport,8:SubmitReport */
    uint16_t msg_id;                   /*16bit message ID */
    char user_header;                  /*0: UDH not present, 1:UDH is present */
    uint8_t msg_encoding;              /*0: 8bit, 2: 7bit ASCII, 4: UCS2, 9: GSM 7bit */
    uint8_t user_msg_type;             /*Valid only if 'msg_encoding' is 1 or 10 (so we do not use) */
    uint32_t num_fields;               /*number of fields in 'message' */
    char message[MAX_MESSAGE_PAYLOAD]; /*payload, including the UDH (if present) */
    uint8_t priority;                  /*0: Normal, 1: Interactive, 2: Urgent, 3: Emergency -- ONLY FOR Verizon: 0: Normal, 1: High */
    uint8_t privacy;                   /*0: not restricted, 1: restricted, 2: confidential, 3: secret */
    uint8_t rel_validity;
    address_3gpp2_t callback_address;
} bearer_data_fields_t;

/**
 *  @brief  Telit SMS helper Digit mode - 3GPP2 C.S0015-A v1.0 Section 3.4.3.3
 */
typedef enum {
    SMS_DIGIT_MODE_4BIT_DTMF  = 0,
    SMS_DIGIT_MODE_8BIT_ASCII = 1
} sms_3gpp2_digit_mode_t;

/**
 *  @brief  Telit SMS helper Number mode indicator - 3GPP2 C.S0015-A v1.0 Section 3.4.3.3
 */
typedef enum {
    SMS_NUM_MODE_DIGIT   = 0,
    SMS_NUM_MODE_DATA_NW = 1
} sms_3gpp2_num_mode_t;

/**
 * @brief Telit SMS helper 3GPP2 PDU type
 *
 */
typedef struct {
    uint8_t msg_type; /* 3gpp2 message type */
    int teleservice_id;
    uint8_t tpmti;    /* 3gpp message type */
    address_3gpp2_t origAddr;
    address_3gpp2_t destAddr;
    uint8_t bearerReplyOpt;
    uint8_t CCReplySeq;
    uint8_t CCErrClass;
    uint8_t CCErrCause;
    bearer_data_fields_t bdf;
} pdu_3gpp2_fields;

/**
 * @brief Telit SMS helper 3GPP2 address types
 *
 */
typedef enum {
    ADDR_TYPE_UNKNOWN,
    ADDR_TYPE_INTERNATIONAL,
    ADDR_TYPE_NATIONAL,
    ADDR_TYPE_INTERNATIONAL_00,
} addr_type_3gpp2_t;

/**
 * @brief Telit SMS helper 3GPP2 sms encode type
 *
 */
typedef struct {
    /** in: pMessage buffer max size, out:len of constructed message in pMessage */
    uint8_t message_size;
    /** the constructed raw message */
    uint8_t pdu_message[MAX_PDU_LEN];
    /** msg ref number, incremented at each msg sent by host */
    uint8_t message_id;
    /** Gives NULL-terminated ASCII String containing destination address */
    char dest_addr[MAX_ADDRESS_LEN];
    /** null term ascii string callback addr (with international support) */
    char cbk_address[MAX_ADDRESS_LEN];
    /** Number of UCS2 chars in text message (NULL excluded) */
    uint32_t text_msg_length;
    /** text message to be encoded */
    char text_msg[MAX_TEXT_MSG_LEN + 1];
    /** prio 0, 1, 2, ... */
    uint8_t priority;
    /** enc alphabet:
     * 0: octets
     * 2: 7 bit ASCII
     * 8: Latin */
    uint8_t alphabet;
    /** Gives the relative validity period of the outgoing message
     * - 0 to 143: (value + 1) × 5 minutes
     * - 144 to 167: 12 hours + ((value – 143) × 30 minutes)
     * - 168 to 196: (value - 166) days
     * - 197 to 244: (value - 192) weeks
     * - 245: Indefinite
     * - 246: Immediate (the message should be discarded if not immediately deliverable)
     * - 247: Valid until mobile becomes inactive
     * - 248: Valid until registration area changes, discard if not registered
     * - 249 to 255: Reserved */
    uint8_t rel_validity;
} telit_helper_sms_encode_3gpp2_t;

/**
 * @brief This function can be used to encode a CDMA SMS message
 *
 * @param req See @ref telit_helper_sms_encode_3gpp2_t for more information
 *
 * @return the total PDU length in bytes or 0 in case of error
 */
int32_t telit_helper_sms_encode_3gpp2(telit_helper_sms_encode_3gpp2_t *req);

/**
 * @brief Telit SMS helper 3GPP2 sms decode type
 *
 */
typedef struct {
    /** msg to be decoded len in bytes */
    uint32_t message_len;
    /** msg read of the device */
    uint8_t message[MAX_PDU_LEN];
    /** msg reference */
    uint32_t message_id;
    /** in: senderaddr maxlen, out senderaddr len */
    uint8_t sender_addr_len;
    char sender_addr[MAX_ADDRESS_LEN];
    /** in: UCS2 characters max in textMsg, out: actual len */
    uint32_t text_msg_len;
    char text_msg[MAX_TEXT_MSG_LEN + 1];
    uint8_t priority;
    /** enum */
    uint8_t privacy;
    /** enum */
    uint8_t language;
    /** Returns the message center timestamp which takes the form: YYMMDDHHMMSSTZ, all values are decimal */
    uint8_t mc_time_stamp[8];
    uint8_t absolute_validity[8];
    uint8_t relative_validity;
    /** enum */
    uint8_t display_mode;
    /** Returns the user (manual) acknowledgment request parameter */
    bool user_acknowledgement_req;
    /** Returns the read acknowledgement request parameter*/
    bool read_acknowledgement_req;
    /** Returns the alerting parameter setting (enum) */
    uint8_t alert_priority;
    uint8_t callbk_addr_len;
    char callbk_addr[32];
    uint8_t ref_num;
    uint8_t total_num;
    uint8_t seq_num;
    uint8_t udh_ind;
} telit_helper_sms_decode_3gpp2_t;

/**
 * @brief This function can be used to decode a received CDMA SMS message
 *
 * @param req See @ref telit_helper_sms_decode_3gpp2_t for more information
 *
 * @return an integer greater than 0 in case of success, 0 otherwise
 */
int32_t telit_helper_sms_decode_3gpp2(telit_helper_sms_decode_3gpp2_t *req);

#endif  /* _TELIT_SMS_HELPER_3GPP2_H_ */
