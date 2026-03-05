#include <stdio.h>
#include <string.h>

#include "telit_sdk.h"
#include "telit_sms_helper.h"
#include "telit_sms_helper_private.h"

#define TPDU_TYPE_DELIVERY 0x00
#define TPDU_TYPE_SUBMIT   0x01

int32_t telit_helper_sms_encode(telit_helper_sms_encode_t *req)
{
    int32_t ret = 0;
    uint8_t gsm_msg[MAX_ENCODED_MSG_LEN] = {0};
    uint8_t packed[MAX_ENCODED_MSG_LEN] = {0};
    uint8_t enc_dest_addr[MAX_ADDRESS_LEN] = {0};
    uint8_t *pdu = req->pdu_message;
    char *dest_addr;
    uint8_t dest_addr_len = 0;
    uint8_t index = 0;
    uint32_t message_len;
    uint32_t len = 0;
    addr_type_t addr_type = ADDRESS_TYPE_UNKNOWN;
    gsm_alphabet_t alphabet = req->alphabet;

    if (alphabet != GSM_ALPHABET_7BIT_ASCII) {
        RLOGE("%s: GSM Alphabet %d is unsupported", __FUNCTION__, alphabet);
        goto end;
    }

    pdu[index++] = 0x00;  /* SMSC length 0, means that SMSC stored in the phone is used */
    if (req->status_report_requested) {
        pdu[index++] = GSM_SMS_CODE_SUBMIT_STATUS_REPORT;
    } else {
        pdu[index++] = GSM_SMS_CODE_SUBMIT;
    }
    pdu[index++] = 0x00;  /* tp-message-reference, 0x00 lets the phone set the message reference number itself */

    addr_type = get_address_type(req->dest_addr);
    if (addr_type == ADDRESS_TYPE_INTERNATIONAL) {
        dest_addr = req->dest_addr + 1;  /* skip '+' sign */
    } else {
        dest_addr = req->dest_addr;
    }
    dest_addr_len = strnlen((const char *)dest_addr, MAX_ADDRESS_LEN);

    len = string_address_to_dso(dest_addr, dest_addr_len, enc_dest_addr);
    if (!len) {
        RLOGE("%s: could not encode address number", __FUNCTION__);
        goto end;
    }

    pdu[index++] = dest_addr_len;  /* original phone number length */
    pdu[index++] = addr_type;
    memcpy(pdu + index, enc_dest_addr, len);
    index += len;

    pdu[index++] = 0x00;          /* tp-pid */
    pdu[index++] = req->alphabet; /* tp-dcs, data coding scheme
                                     (0x00 is the default 7bit alphabet,
                                     0x02 for 8bit alphabet) */
    pdu[index++] = 0xAA;          /* tp-validity period (4 days) */

    /* TODO: when support for 8 bits alphabet will be available, make this */
    /* limit dinamic (max length is 140 characters in that case) */
    if (req->message_size > MAX_TEXT_MSG_LEN) {
        RLOGI("%s: Input message is too long (%dB, limit is %dB)",
                __FUNCTION__, req->message_size, MAX_TEXT_MSG_LEN);
    }
    len = (req->message_size <= MAX_TEXT_MSG_LEN) ? req->message_size : MAX_TEXT_MSG_LEN;

    message_len = unicode_to_unpacked_gsm(req->text_msg, len, gsm_msg);
    if (!message_len) {
        RLOGE("%s: could not encode unicode message", __FUNCTION__);
        goto end;
    }

    len = gsm_pack(gsm_msg, message_len, packed, MAX_ENCODED_MSG_LEN);
    if (!len) {
        RLOGE("%s: could not pack gsm message", __FUNCTION__);
        goto end;
    }

    pdu[index++] = message_len;
    memcpy(pdu + index, packed, len);
    ret = index + len;

end:
    return ret;
}

static int complete_sms_delivery_decode(uint8_t index, telit_helper_sms_decode_t *req);
static int complete_sms_submit_decode(uint8_t index, telit_helper_sms_decode_t *req);
static int decode_pdu_message_part(uint8_t index, telit_helper_sms_decode_t *req);
int32_t telit_helper_sms_decode(telit_helper_sms_decode_t *req)
{
    uint32_t len = 0;
    uint8_t *pdu = req->message;
    uint8_t index = 0;
    uint8_t smsc_info_octets;

    smsc_info_octets = pdu[index++];  /* Address type + Actual address length */

    if (smsc_info_octets) {
        char sc_addr[MAX_ADDRESS_LEN] = {0};
        addr_type_t addr_type = ADDRESS_TYPE_UNKNOWN;

        addr_type = pdu[index++];

        req->sc_addr_len = dso_number_to_string(&pdu[index],
                smsc_info_octets - 1,
                sc_addr, 16);
        if (addr_type == ADDRESS_TYPE_INTERNATIONAL) {
            req->sc_addr_len++;
            snprintf(req->sc_addr, req->sc_addr_len + 1, "+%s", sc_addr);
        } else {
            strlcpy(req->sc_addr, sc_addr, sizeof(req->sc_addr));
        }
        index += (smsc_info_octets - 1);
    }

    if ((pdu[index] & 0x03) == TPDU_TYPE_DELIVERY) {
        RLOGD("%s: decoding sms-delivery (0x%02X)", __FUNCTION__, pdu[index]);
        index++;
        len = complete_sms_delivery_decode(index, req);
    } else if ((pdu[index] & 0x03) == TPDU_TYPE_SUBMIT) {
        RLOGD("%s: decoding sms-submit (0x%02X)", __FUNCTION__, pdu[index]);
        index++;
        len = complete_sms_submit_decode(index, req);
    } else {
        RLOGE("%s: Unknown SMS type (0x%02X)", __FUNCTION__, pdu[index]);
        len = 0;
    }

    return len;
}

static int complete_sms_submit_decode(uint8_t index, telit_helper_sms_decode_t *req)
{
    uint8_t *pdu = req->message;
    uint8_t addr_len;

    index++;  /* skip tp-message-reference */
    addr_len = pdu[index++] / 2;
    if (addr_len % 2) {
        addr_len += 1;
    }
    index++;           /* skip type-of-address */
    index += addr_len; /* skip dest address since output does not report it */
    index++;           /* skip tp-pid */
    index++;           /* skip tp-dcs */
    index++;           /* skip tp-validity period */

    return decode_pdu_message_part(index, req);
}

static int complete_sms_delivery_decode(uint8_t index, telit_helper_sms_decode_t *req)
{
    uint8_t *pdu = req->message;
    uint32_t len;
    uint8_t array[8] = {0};
    char sender_addr[MAX_ADDRESS_LEN] = {0};
    addr_type_t addr_type = ADDRESS_TYPE_UNKNOWN;
    uint8_t sender_addr_len;
    uint8_t sender_addr_octets;

    sender_addr_len = pdu[index++];
    sender_addr_octets = sender_addr_len / 2;
    if (sender_addr_len % 2) {
        sender_addr_octets++;
    }

    addr_type = pdu[index++];
    req->sender_addr_len = dso_number_to_string(&pdu[index],
            sender_addr_octets,
            sender_addr, 24);
    if (addr_type == ADDRESS_TYPE_INTERNATIONAL) {
        req->sender_addr_len++;
        snprintf(req->sender_addr, req->sender_addr_len + 1, "+%s", sender_addr);
    } else {
        strlcpy(req->sender_addr, sender_addr, sizeof(req->sender_addr));
    }
    index += sender_addr_octets;

    index++;  /* skip tp pid */
    /* TODO take the alphabet from below */
    index++;  /* skip tp dcs */

    memcpy(array, &pdu[index], 7);
    len = swap_msb_lsb_in_byte_array(array, 7);
    if (len != 7) {
        RLOGE("%s: could not parse timestamp\n", __FUNCTION__);
    } else {
        snprintf(req->date, DATE_TIME_SIZE, "%02x/%02x/%02x", array[0], array[1], array[2]);
        snprintf(req->time, DATE_TIME_SIZE, "%02x:%02x.%02x", array[3], array[4], array[5]);
    }
    index += 7;

    return decode_pdu_message_part(index, req);
}

static int
decode_pdu_message_part(uint8_t index, telit_helper_sms_decode_t *req)
{
    uint8_t *pdu = req->message;
    uint8_t pdu_msg_len;
    uint32_t len;
    uint8_t array[MAX_PDU_LEN] = {0};

    req->text_msg_len = pdu[index++];
    pdu_msg_len = req->message_len - index;

    len = gsm_unpack(&pdu[index], (pdu_msg_len * 8) / 7, array, req->text_msg_len);
    if (!len) {
        RLOGE("%s: could not unpack message", __FUNCTION__);
        goto end;
    }

    if ((req->text_msg_len % 8 == 7) && (array[len - 1] == 0x00)) {
        RLOGD("%s: dropping padding character", __FUNCTION__);
        len--;
    }

    len = unpacked_gsm_to_unicode(array, len, req->text_msg, MAX_TEXT_MSG_LEN);
    if (!len) {
        RLOGE("%s: could not convert unpacked message to unicode", __FUNCTION__);
    }

end:
    return len;
}
