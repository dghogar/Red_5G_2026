#ifndef _SMS_HELPER_PRIVATE_H_
#define _SMS_HELPER_PRIVATE_H_

#include <stdbool.h>
#include <stdint.h>

#define GSM_BASE_SET_SIZE     127
#define GSM_EXTENDED_SET_SIZE 12
#define GSM_ESCAPE_CHAR       0x1B
#define GSM_SET_EMPTY_ITEM    {0x00, 0, {0x00, 0x00, 0x00}}
#define UNICODE_SIZE          4

typedef enum {
    GSM_SMS_CODE_DELIVERY             = 0x04,
    GSM_SMS_CODE_SUBMIT               = 0x11,
    GSM_SMS_CODE_SUBMIT_STATUS_REPORT = 0x31,
} gsm_sms_codes_t;

typedef struct {
    uint8_t gsm;
    uint8_t unicode_len;
    uint8_t unicode[UNICODE_SIZE];
} unicode_to_gsm_t;

typedef enum {
    ADDRESS_TYPE_UNKNOWN,
    ADDRESS_TYPE_INTERNATIONAL = 0x91,
    ADDRESS_TYPE_NATIONAL      = 0xA1,
} addr_type_t;

/* Returns a pointer to the first byte of the next unicode character */
char *get_next_unicode_char(const char *msg, const uint32_t len);
/* Packs an unicode message encoded in GSM default alphabet into gsm semi-octets form */
uint32_t gsm_pack(const uint8_t *src, uint32_t src_len, uint8_t *packed, uint32_t packed_max_len);
/* Unpacks a gsm semi-octets data into an unicode message encoded in GSM default alphabet  */
uint32_t gsm_unpack(const uint8_t *gsm, const uint32_t septets, uint8_t *unpacked,
        const uint32_t unpacked_max_len);
/* uint32_t gsm_unpack                 (const uint8_t *packed, uint32_t num_septets, uint8_t *unpacked, uint32_t unpacked_max_len); */
/* Translates a unicode message using GSM default alphabet */
uint32_t unicode_to_unpacked_gsm(const char *msg, const uint32_t msg_len, uint8_t *gsm);
/* Translates a GSM default alphabet message into unicode */
uint32_t unpacked_gsm_to_unicode(const uint8_t *gsm, const uint32_t gsm_len, char *msg,
        const uint32_t max_msg_len);
/* Converts a number string into a decimal semi-octet (e.g. 0123... -> [0x01, 0x23, ...] */
int32_t string_address_to_dso(const char *number, const uint8_t number_len, uint8_t *encoded);
/* Converts a decimal semi-octet into a number string (e.g. [0x01, 0x23, ...] -> 0123...  */
int32_t dso_number_to_string(const uint8_t *encoded, const uint32_t enc_len, char *string,
        const uint32_t max_string_len);
/* Converts a string message, with characters in [0-9A-F], into a byte array */
uint8_t string_to_byte_array(const char *string, const uint32_t string_len, uint8_t *byte_array,
        const uint32_t max_byte_array_len);
/* Swap MSB with LSB of each byte in a byte array (e.g. [0x01, 0x23, ...] -> [0x10, 0x32, ...]*/
uint8_t swap_msb_lsb_in_byte_array(uint8_t *array, const uint32_t len);

/* Return the type of address of the given address string */
addr_type_t get_address_type(const char *address);

#endif  /* _SMS_HELPER_PRIVATE_H_ */
