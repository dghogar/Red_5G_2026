#ifndef _SMS_HELPER_H_
#define _SMS_HELPER_H_

#include <stdint.h>

/**
 * @file telit_sms_helper.h
 * @brief sms helper functions
 */

#define MAX_TEXT_MSG_LEN    (320)
#define MAX_ENCODED_MSG_LEN (400)
#define MAX_PDU_LEN         (1024)
#define MAX_ADDRESS_LEN     (24)

/**
 * @brief Telit SMS helper gsm alphabets
 *
 */
typedef enum {
    GSM_ALPHABET_7BIT_ASCII,   /* default alphabet */
    GSM_ALPHABET_8BIT_ASCII,   /* not supported at this time */
    GSM_ALPHABET_IS91EP,       /* not supported at this time */
    GSM_ALPHABET_IA5,          /* not supported at this time */
    GSM_ALPHABET_UNICODE,      /* not supported at this time */
    GSM_ALPHABET_SHIFT_JIS,    /* not supported at this time */
    GSM_ALPHABET_KOREAN,       /* not supported at this time */
    GSM_ALPHABET_LATIN_HEBREW, /* not supported at this time */
    GSM_ALPHABET_LATIN,        /* not supported at this time */
} gsm_alphabet_t;

/**
 * @brief Telit SMS helper 3GPP sms encode type
 *
 */
typedef struct {
    /** The length of the message contents in bytes */
    uint32_t message_size;
    /** Gives NULL-terminated ASCII String containing destination address */
    char dest_addr[MAX_ADDRESS_LEN];
    /** Text message to be encoded */
    char text_msg[MAX_TEXT_MSG_LEN + 1];
    /** Encoded PDU message */
    uint8_t pdu_message[MAX_PDU_LEN];
    /** Alphabet */
    uint8_t alphabet;
    /** True if status report is requested */
    bool status_report_requested;
} telit_helper_sms_encode_t;

/**
 * @brief This function can be used to encode a WCDMA SMS message
 *
 * @param req See @ref telit_helper_sms_encode for more information
 *
 * @return the total PDU length in bytes or 0 in case of error
 */
int32_t telit_helper_sms_encode(telit_helper_sms_encode_t *req);

#define DATE_TIME_SIZE 9

/**
 * @brief Telit SMS helper 3GPP sms decode type
 *
 */
typedef struct {
    /** Length of the PDU in bytes */
    uint32_t message_len;
    /** Message read off the device via SLQSGetSMS */
    uint8_t message[MAX_PDU_LEN];
    /** Upon successful output, returns the length
       of destination address string. */
    uint8_t sender_addr_len;
    /** NULL-terminated ASCII String containing
       destination address */
    char sender_addr[MAX_ADDRESS_LEN];
    /** Upon successful output, returns the number of
       characters in the given text message buffer.*/
    uint32_t text_msg_len;
    /** Encoded PDU message */
    char text_msg[MAX_TEXT_MSG_LEN + 1];
    /** service center address length */
    uint8_t sc_addr_len;
    /** NULL-terminated ASCII String containing service center
       address */
    char sc_addr[MAX_ADDRESS_LEN];
    /** Time fetched from message */
    char time[DATE_TIME_SIZE];
    /** Date fetched from message */
    char date[DATE_TIME_SIZE];
} telit_helper_sms_decode_t;

/**
 * @brief This function can be used to decode a received WCDMA SMS message
 *
 * @param req See @ref telit_helper_sms_decode_t for more information
 *
 * @return an integer greater than 0 in case of success, 0 otherwise
 */
int32_t telit_helper_sms_decode(telit_helper_sms_decode_t *req);

#endif  /* _SMS_HELPER_H_ */
