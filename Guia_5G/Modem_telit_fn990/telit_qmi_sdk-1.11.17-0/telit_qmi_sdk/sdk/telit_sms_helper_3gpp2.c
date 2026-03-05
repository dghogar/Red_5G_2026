#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "telit_sdk.h"
#include "telit_sms_helper_3gpp2.h"
#include "telit_sms_helper_private.h"

enum {
    ENCODING_OCTET                     = 0,
    ENCODING_EXTENDED_PROTOCOL_MESSAGE = 1,  /* unsupported */
    ENCODING_ASCII_7BIT                = 2,
    ENCODING_IA5                       = 3,  /* unsupported */
    ENCODING_UNICODE                   = 4,  /* unsupported */
    ENCODING_SHIFT_JIS                 = 5,  /* unsupported */
    ENCODING_KOREAN                    = 6,  /* unsupported */
    ENCODING_LATIN_HEBREW              = 7,  /* unsupported */
    ENCODING_LATIN                     = 8,
    ENCODING_GSM_7BIT                  = 9,  /* unsupported */
    ENCODING_GSM_DCS                   = 10, /* unsupported */
};

static uint8_t read_bits(uint8_t bits, uint8_t *shiftO, uint8_t *src, uint32_t *index);
static void sms_subparameter_decode(uint8_t *bitBuffer, uint32_t dataBufSize,
        telit_helper_sms_decode_3gpp2_t *req);
static int32_t decode_address_field(uint8_t *data, address_3gpp2_t *addr);
static uint8_t read_text_bits(const uint8_t *bytes, uint8_t b_offset, uint8_t n_bits);
static int32_t decode_message(const uint8_t encoding, const uint8_t num_fields, uint8_t *data,
        const uint32_t len, char *text_msg, uint32_t *text_len);

const char *privacy_map[] = {
    "Not restricted",
    "Restricted",
    "Confidential",
    "Secret"
};

const char *priority_map[] = {
    "normal",
    "interactive",
    "urgent",
    "emergency"
};

const char *msg_type_map[] = {
    "Reserved",
    "received", /*"Deliver", */
    "sent",     /*"Submit", */
    "Cancellation",
    "Deliever Acknowledge",
    "User Acknowledge",
    "Read Acknowledge",
    "Deliver Report",
    "Submit Report"
};

const char *sub_parameter_id_map[] = {
    "Message Identifier",
    "User Data",
    "User Response Code",
    "Message Center Time Stamp",
    "Validity Period - Absolute",
    "Validity Period - Relative",
    "Deferred Delivery Time - Absolute",
    "Deferred Delivery Time - Relative",
    "Priority Indicator",
    "Privacy Indicator",
    "Reply Option",
    "Number of Messages",
    "Alert on Message Delivery",
    "Language Indicator",
    "Call-Back Number",
    "Message Display Mode",
    "Multiple Encoding User Data",
    "Message Deposit Index",
    "Service Category Program Data",
    "Service Category Program Results",
    "Message Status",
    "TP-Failure Cause",
    "Enhanced VMN",
    "Enhanced VMN Ack",
    "Unknown subparameter"
};

static const char *get_subparameter_name(const uint8_t idx)
{
    if (idx >= 24) {
        return sub_parameter_id_map[24];
    }

    return sub_parameter_id_map[idx];
}

static void format_timestamp(time_date_t *td, uint8_t *timestamp)
{
    RLOGD("%s", __FUNCTION__);
    timestamp[0] = td->year;
    timestamp[1] = td->month;
    timestamp[2] = td->day;
    timestamp[3] = td->hour;
    timestamp[4] = td->min;
    timestamp[5] = td->sec;
    telit_print_buffer(timestamp, 6, LOG_DEBUG);
}

static void telit_helper_sms_init_decode_3gpp2(telit_helper_sms_decode_3gpp2_t *req)
{
    req->priority = 0xFF;
    req->privacy = 0xFF;
    req->language = 0xFF;
    req->relative_validity = 0xFF;
    req->display_mode = 0xFF;
    req->alert_priority = 0xFF;
}

int32_t telit_helper_sms_decode_3gpp2(telit_helper_sms_decode_3gpp2_t *req)
{
    bool err = false;
    uint32_t i = 1;
    uint8_t *data = req->message;
    uint8_t tlv_id = 0;

    telit_helper_sms_init_decode_3gpp2(req);

    RLOGD("%s: message type %d (src len %d)", __FUNCTION__, data[0], req->message_len);

    while (i < req->message_len) {
        uint16_t par_id = data[i++];
        uint16_t par_len = data[i++];

        RLOGD("%s: TLV %d: id:%x, len:%d", __FUNCTION__, tlv_id++, par_id, par_len);

        switch (par_id) {
            case PARAMETER_ORIGINATE_ADDRESS:
            case PARAMETER_DESTINATION_ADDRESS: {
                int32_t err;
                address_3gpp2_t addr = {0};
                char *message_type = (par_id == PARAMETER_ORIGINATE_ADDRESS) ? "deliver" : "submit";

                RLOGD("%s: message_type: %s", __FUNCTION__, message_type);

                err = decode_address_field(&data[i], &addr);
                if (err) {
                    RLOGE("%s: could not decode address field %d) (err %d)",
                            __FUNCTION__, par_id, err);
                    goto end;
                }

                req->sender_addr_len = strlen((const char *)addr.address);
                strlcpy(req->sender_addr, addr.address, sizeof(req->sender_addr));
                break;
            }
            case PARAMETER_BEARER_DATA:
                sms_subparameter_decode(&data[i], par_len, req);
                break;
            case PARAMETER_CAUSE_CODE:
            case PARAMETER_TELESERVICE_ID:
            case PARAMETER_SERVICE_CATEGORY:
            case PARAMETER_BEARER_REPLY_OPTION:
            default:
                RLOGD("%s: unsupported parameter id %d", __FUNCTION__, par_id);
                break;
        }
        i += par_len;
    }

end:
    return err ? 0 : req->text_msg_len;
}

static int32_t decode_address_field(uint8_t *data, address_3gpp2_t *addr)
{
    const int8_t Dtmf2Ascii[] =
    {'D', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#', 'A', 'B', 'C'};
    int32_t err = 0;
    uint8_t i = 0, j;
    uint8_t addr_digit;
    uint32_t bufferIndex = 0;

    addr->digit_mode = read_bits(1, &i, data, &bufferIndex);
    i++;
    /* TODO valid only if force number mode is False, but in the reference */
    /* implementation number mode is always False. */
    addr->number_mode = 0;
    i++;

    /* TODO review this (what to do if conditions are FALSE?) */
    if (addr->digit_mode == SMS_3GPP2_SMS_DIGIT_MODE_8BIT_ASCII) {
        i += 3;
    }

    if (addr->digit_mode == SMS_3GPP2_SMS_DIGIT_MODE_8BIT_ASCII &&
            addr->number_mode == SMS_3GPP2_SMS_NUM_MODE_DIGIT) {
        i += 4;  /* skip number_plan */
    }

    addr->num_fields = read_bits(8, &i, data, &bufferIndex);
    i += 8;

    RLOGD("%s: digit mode: %02x, number mode: %02x, num fieds: %d", __FUNCTION__,
            addr->digit_mode,
            addr->number_mode,
            addr->num_fields);

    for (j = 0; j < addr->num_fields; j++) {
        uint8_t bits = (addr->digit_mode == SMS_3GPP2_SMS_DIGIT_MODE_4BIT_DTMF) ? 4 : 8;

        addr_digit = read_bits(bits, &i, data, &bufferIndex);
        i += bits;

        if (addr->digit_mode == SMS_3GPP2_SMS_DIGIT_MODE_4BIT_DTMF) {
            addr->address[j] = Dtmf2Ascii[addr_digit];
            continue;
        }

        switch (addr->number_mode) {
            case 0:  /* ASCII represntation with MSB set to 0. Just treat as normal ASCII */
            case 2:  /* 8 bit ASCII */
            case 1:  /* Binary value of an octet of the address. FIXME: Not clear to me what it means, by default I decode in ASCII */
                addr->address[j] = addr_digit;
                break;
            default:
                RLOGE("%s: unsupported number mode %d", __FUNCTION__, addr->number_mode);
                err = -1;
        }
    }
    addr->address[j] = 0;
    RLOGD("%s: address: %s", __FUNCTION__, addr->address);

    return err;
}

uint8_t read_bits(uint8_t n_bits, uint8_t *b_offset, uint8_t *src, uint32_t *index)
{
    uint8_t retVal = 0;
    const int8_t mask[] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};

    if (n_bits <= 8) {
        int i = *index;
        int shift = *b_offset;

        if (*b_offset > 7) {
            i = *index = *index + 1;
            shift = *b_offset = *b_offset - 8;
        }

        if (shift + n_bits <= 8) {
            return (src[i] >> (8 - shift - n_bits)) & mask[n_bits];
        }

        retVal = (src[i + 1] >> (16 - shift - n_bits)) & mask[(shift + n_bits) - 8];
        retVal |= src[i] << (shift + n_bits - 8);
        retVal &= mask[n_bits];
    }

    return retVal;
}

static void timestamp_decoder(time_date_t *td, uint8_t *str)
{
    td->year = BCD_DECODER(str[0]);
    td->month = BCD_DECODER(str[1]);
    td->day = BCD_DECODER(str[2]);
    td->hour = BCD_DECODER(str[3]);
    td->min = BCD_DECODER(str[4]);
    td->sec = BCD_DECODER(str[5]);
}

static void sms_subparameter_decode(uint8_t *bitBuffer, uint32_t dataBufSize,
        telit_helper_sms_decode_3gpp2_t *req)
{
    int i = 0, j, remainBufSize = dataBufSize, msgLen = 0;
    uint8_t shiftIndex;
    uint32_t tmpI;

    while (remainBufSize > 0) {
        /* Fixed header */
        int id = (int)(bitBuffer[i++]);
        int length = (int)(bitBuffer[i++]);
        tmpI = i;
        shiftIndex = 0;

        remainBufSize -= (2 + length);

        RLOGD("%s parameter %d (%s), length: %d",
                __FUNCTION__, id, get_subparameter_name(id), length);

        switch (id) {
            case 0: {  /* Message Identifier, C.S0015-B v2.0, 4.5.1 */
                uint8_t msg_type;
                uint8_t msg_id;
                uint8_t user_header;

                msg_type = read_bits(4, &shiftIndex, bitBuffer, &tmpI);
                shiftIndex += 4;

                msg_id = read_bits(8, &shiftIndex, bitBuffer, &tmpI);
                shiftIndex += 8;

                msg_id = (msg_id << 8) | read_bits(8, &shiftIndex, bitBuffer, &tmpI);
                shiftIndex += 8;

                user_header = read_bits(1, &shiftIndex, bitBuffer, &tmpI);

                RLOGD("%s: msgType %d (%s), msg_id: %u, userheader: %d",
                        __FUNCTION__, msg_type, msg_type_map[(uint16_t)(msg_type)], msg_id, user_header);

                req->message_id = msg_id;
                break;
            }
            case 1: {  /* User Data, C.S0015-B v2.0, 4.5.2 */
                uint8_t msg_encoding;
                uint8_t num_fields;
                uint8_t msg_len_in_octets;
                uint8_t user_msg_type = 0;

                msg_encoding = read_bits(5, &shiftIndex, bitBuffer, &tmpI);
                shiftIndex += 5;

                if (msg_encoding == 1 || msg_encoding == 10) {
                    user_msg_type = read_bits(8, &shiftIndex, bitBuffer, &tmpI);
                    RLOGD("%s: user_msg_type: 0x%X", __FUNCTION__, user_msg_type);
                    msgLen = length - 3;
                    shiftIndex += 8;
                } else {
                    msgLen = length - 2;
                }

                num_fields = read_bits(8, &shiftIndex, bitBuffer, &tmpI);
                shiftIndex += 8;
                for (j = 0; j < msgLen; j++) {
                    req->message[j] = read_bits(8, &shiftIndex, bitBuffer, &tmpI);
                    shiftIndex += 8;
                }
                msg_len_in_octets = msgLen;

                switch (msg_encoding) {
                    case 2:  /*7bit ASCII */
                    case 3:  /*IA5 */
                    case 9:  /*7 bit GSM */
                        if ((msgLen * 8) < (num_fields * 7)) {
                            int last3bits;

                            last3bits = read_bits(3, &shiftIndex, bitBuffer, &tmpI);
                            req->message[j] = last3bits << 5;
                            msg_len_in_octets = msgLen + 1;
                        }
                }
                RLOGD("%s: msg_encoding %d, msgType: %d, num_fields: %d, msg_len_in_octets: %d", __FUNCTION__,
                        msg_encoding,
                        user_msg_type,
                        num_fields,
                        msg_len_in_octets);

                decode_message(msg_encoding,
                        num_fields,
                        req->message,
                        length,
                        req->text_msg,
                        &req->text_msg_len);

                break;
            }
            case 3: {  /* Message Center Time Stamp, C.S0015-B v2.0, 4.5.4 */
                time_date_t timestamp = {0};
                timestamp_decoder(&timestamp, &(bitBuffer[i]));
                RLOGD("%s: timestamp: %d/%d/%d, %d:%d:%d", __FUNCTION__,
                        timestamp.day, timestamp.month,
                        timestamp.year, timestamp.hour,
                        timestamp.min, timestamp.sec);

                format_timestamp(&timestamp, req->mc_time_stamp);
                break;
            }
            case 4: {  /* Validity Period - Absolute, C.S0015-B v2.0, 4.5.5 */
                time_date_t validity_period_abs = {0};

                timestamp_decoder(&validity_period_abs, &(bitBuffer[i]));
                RLOGD("%s: validityPeriodAbsolute: %d/%d/%d, %d:%d:%d", __FUNCTION__,
                        validity_period_abs.day, validity_period_abs.month,
                        validity_period_abs.year, validity_period_abs.hour,
                        validity_period_abs.min, validity_period_abs.sec);
                format_timestamp(&validity_period_abs, req->absolute_validity);
                break;
            }
            case 5:  /* Validity Period - Relative, C.S0015-B v2.0, 4.5.6 */
                req->relative_validity = bitBuffer[i];
                RLOGD("%s: validityPeriodRelative: %d", __FUNCTION__, req->relative_validity);
                break;
            case 8:  /* Priority Indicator, C.S0015-B v2.0, 4.5.9 */
                req->priority = read_bits(2, &shiftIndex, bitBuffer, &tmpI);
                RLOGD("%s: priority %d (%s)", __FUNCTION__, req->priority, priority_map[req->priority]);
                break;
            case 9:  /* Privacy Indicator, C.S0015-B v2.0, 4.5.10 */
                req->privacy = read_bits(2, &shiftIndex, bitBuffer, &tmpI);
                RLOGD("%s: privacy %d (%s)", __FUNCTION__, req->privacy, privacy_map[req->privacy]);
                break;
            case 10: {  /* Reply Option, C.S0015-B v2.0, 4.5.11 */
                uint8_t deliver_ack, deliver_report;

                req->user_acknowledgement_req = read_bits(1, &shiftIndex, bitBuffer, &tmpI);
                shiftIndex++;

                deliver_ack = read_bits(1, &shiftIndex, bitBuffer, &tmpI);
                shiftIndex++;

                req->read_acknowledgement_req = read_bits(1, &shiftIndex, bitBuffer, &tmpI);
                shiftIndex++;

                deliver_report = read_bits(1, &shiftIndex, bitBuffer, &tmpI);

                RLOGD("%s: user_ack: %d, deliver_ack: %d, read_ack: %d, deliver_report: %d", __FUNCTION__,
                        req->user_acknowledgement_req,
                        deliver_ack,
                        req->read_acknowledgement_req,
                        deliver_report);
                break;
            }
            case 12:  /* Alert on Message Delivery, C.S0015-B v2.0, 4.5.13 */
                req->alert_priority = read_bits(2, &shiftIndex, bitBuffer, &tmpI);
                RLOGD("%s: alert_priority: %d", __FUNCTION__, req->alert_priority);
                break;
            case 13:  /* Language Indicator, C.S0015-B v2.0, 4.5.14 and 3GPP2 C.R1001-H */
                req->language = bitBuffer[i];
                RLOGD("%s: language index: %d", __FUNCTION__, req->language);
                break;
            case 14: {  /* Callback Number, C.S0015-B v2.0, 4.5.15 */
                int32_t err;
                address_3gpp2_t addr = {0};

                err = decode_address_field(&bitBuffer[i], &addr);
                if (err) {
                    RLOGE("%s: could not decode callback address", __FUNCTION__);
                    break;
                }

                req->callbk_addr_len = strlen((const char *)addr.address);
                strlcpy(req->callbk_addr, addr.address, sizeof(req->callbk_addr));
                RLOGD("%s: callback addr: (%d) %s", __FUNCTION__, req->callbk_addr_len, req->callbk_addr);
                break;
            }
            case 15:  /* Message Display Mode, C.S0015-B v2.0, 4.5.16 */
                req->display_mode = read_bits(2, &shiftIndex, bitBuffer, &tmpI);
                RLOGD("%s: msgDisplayMode: %d", __FUNCTION__, req->display_mode);
                break;
            case 2:  /* User Response Code, C.S0015-B v2.0, 4.5.3 */
            case 6:  /* Deferred Delivery Time - Absolute, C.S0015-B v2.0, 4.5.7 */
            case 7:  /* Deferred Delivery Time - Relative, C.S0015-B v2.0, 4.5.8 */
            case 11: /* Number of Messages, C.S0015-B v2.0, 4.5.12 */
            case 16: /* Multiple Encoding User Data, C.S0015-B v2.0, 4.5.17 */
            case 17: /* Message Deposit Index, C.S0015-B v2.0, 4.5.18 */
            case 20: /* Message Status, C.S0015-B v2.0, 4.5.21 */
            case 21: /* TP-Failure Cause, C.S0015-B v2.0, 4.5.22 */
            default:
                RLOGD("%s: unsupported sub-parameter id %d", __FUNCTION__, id);
                break;
        }
        ;
        i += length;
    }
}

static void shift_left(uint8_t *ar, uint32_t size, uint32_t shift)
{
    int carry = 0, i;                                  /* Clear the initial carry bit */

    while (shift--) {                                  /* For each bit to shift */
        for (i = size - 1; i >= 0; i--) {              /* For each element of the array from low to high */
            uint32_t next = (ar[i] & 0x80) ? 0x01 : 0; /* if the high bit is set, set the carry bit */
            ar[i] = carry | (ar[i] << 1);              /* Shift the element one bit left and add the old carry */
            carry = next;                              /* Remember the old carry for next time. */
        }
    }
}

static void write_bits(uint32_t value, uint32_t bits, uint32_t *shift_O, uint8_t *src, uint32_t *index)
{
    const int8_t mask[] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};

    if (bits <= 16) {
        int32_t i = *index;
        int32_t shift = *shift_O;

        while (*shift_O > 7) {
            i = *index = *index + 1;
            shift = *shift_O = *shift_O - 8;
        }

        if (shift + bits <= 8) {  /* write in one byte only */
            uint8_t c_buf;
            c_buf = (uint8_t)(value & mask[bits]);
            c_buf = c_buf << (8 - (shift + bits));
            src[i] |= c_buf;
        } else {  /* write in 2 or 3 bytes */
            uint8_t cs_buf[3] = {0};
            if (bits > 8) {
                cs_buf[2] = (uint8_t)(value & 0xFF);
                cs_buf[1] = (uint8_t)((value >> 8) & 0xFF);
                cs_buf[0] = 0;
                shift_left(cs_buf, 3, 24 - bits - shift);
            } else {
                cs_buf[1] = (uint8_t)(value & 0xFF);
                cs_buf[0] = 0;
                shift_left(cs_buf, 2, 16 - bits - shift);
            }

            src[i] |= cs_buf[0];
            src[i + 1] = cs_buf[1];

            if (shift + bits > 16) {
                src[i + 2] = cs_buf[2];
            }
        }
    }
}

static uint32_t address_encoder(address_3gpp2_t *address_info, uint8_t *src)
{
    const char dtmf_chars[] =
    {'D', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#', 'A', 'B', 'C'};
    uint32_t shift = 0;
    uint32_t index = 1;
    uint32_t i;
    uint32_t addr_len = address_info->num_fields;
    char *pch;

    write_bits(address_info->digit_mode, 1, &shift, src, &index);
    shift++;
    write_bits(address_info->number_mode, 1, &shift, src, &index);
    shift++;

    if (address_info->digit_mode == SMS_DIGIT_MODE_8BIT_ASCII) {
        write_bits(address_info->number_type, 3, &shift, src, &index);
        shift += 3;

        if (address_info->number_mode == SMS_NUM_MODE_DIGIT) {
            write_bits(address_info->number_plan, 4, &shift, src, &index);
            shift += 4;
        }
    }

    /* Fill address size */
    write_bits(addr_len, 8, &shift, src, &index); shift += 8;

    /* Fill address */
    for (i = 0; i < addr_len; i++) {
        if (address_info->digit_mode == SMS_DIGIT_MODE_8BIT_ASCII) {
            write_bits(address_info->address[i] & 0x7F, 8, &shift, src, &index);
            shift += 8;
        } else {
            pch = strchr((const char *)dtmf_chars, address_info->address[i]);

            if (pch != NULL) {
                write_bits(pch - dtmf_chars, 4, &shift, src, &index);
                shift += 4;
            }
        }
    }

    if (shift > 7) {
        index++;
    }

    /* set data size */
    src[0] = index;

    return index + 1;
}

static int32_t sms_sub_parameter_encoder(bearer_data_fields_t *data, uint8_t *src)
{
    uint32_t shift_p;
    uint32_t index = 0;
    uint32_t i;
    uint32_t length_position = 0;

    for (i = 0; i < 24; i++) {  /*There are 24 possible subparameters, from 0 to 23. See C.S0015-B v2.0, table 4.5-1 */
        shift_p = 0;

        if (data->bitmask & (0x1L << i)) {
            switch (i) {
                case SUBPARAMETER_MESSAGE_ID:
                    index++;  /* move ahead index (index 0 is reserved for final size) */
                    src[index++] = SUBPARAMETER_MESSAGE_ID;
                    src[index++] = 3;
                    write_bits(data->msg_type, 4, &shift_p, src, &index);
                    shift_p += 4;
                    write_bits(data->msg_id, 16, &shift_p, src, &index);
                    shift_p += 16;
                    write_bits(data->user_header, 1, &shift_p, src, &index);
                    break;
                case SUBPARAMETER_USER_DATA: {
                    uint32_t j;
                    uint32_t bytes = 0;
                    uint32_t bits = 0;
                    uint8_t char_size = 0;

                    index++;                   /* move ahead index */
                    src[index++] = SUBPARAMETER_USER_DATA;
                    length_position = index++; /* save position of user data length value */

                    write_bits(data->msg_encoding, 5, &shift_p, src, &index);
                    shift_p += 5;
                    bits += 5;

                    write_bits(data->num_fields, 8, &shift_p, src, &index);
                    shift_p += 8;
                    bits += 8;

                    switch (data->msg_encoding) {
                        default:
                        case ENCODING_ASCII_7BIT:
                            char_size = 7;
                            break;
                        case ENCODING_OCTET:
                        case ENCODING_LATIN:
                            char_size = 8;
                            break;
                    }

                    for (j = 0; j < data->num_fields; j++) {
                        write_bits(data->message[j], char_size, &shift_p, src, &index);
                        shift_p += char_size;
                        bits += char_size;
                    }

                    bytes = (bits / 8);
                    if (bits % 8) {
                        bytes++;
                    }

                    src[length_position] = bytes;
                    RLOGD("%s: Write length %d", __FUNCTION__, bytes);

                    index = length_position + bytes;
                    break;
                }
                case SUBPARAMETER_VALIDITY_PERIOD_REL:
                    index++;  /* move ahead index */
                    src[index++] = SUBPARAMETER_VALIDITY_PERIOD_REL;
                    src[index++] = 1;
                    write_bits(data->rel_validity, 8, &shift_p, src, &index);
                    break;
                case SUBPARAMETER_PRIORITY:
                    index++;  /* move ahead index */
                    src[index++] = SUBPARAMETER_PRIORITY;
                    src[index++] = 1;
                    write_bits(data->priority, 2, &shift_p, src, &index);
                    break;
                case SUBPARAMETER_CALLBACK_NUMBER:
                    index++;  /* move ahead index */
                    src[index++] = SUBPARAMETER_CALLBACK_NUMBER;
                    index += address_encoder(&(data->callback_address), &src[index]) - 1;
                    break;
                default:
                    RLOGW("%s: Unsupported Parameter", __FUNCTION__);
                    break;
            }
        }
    }

    /* Calculate data size */
    src[0] = index;

    return index + 1;
}

static uint32_t encode_parameter(uint8_t id, uint32_t value, void *data, uint8_t *pdu)
{
    uint32_t shift_p = 0;
    uint32_t index = 2;

    pdu[0] = id;

    switch (id) {
        case PARAMETER_TELESERVICE_ID:   /* Teleservice Identify, C.S0015-B v2.0, 3.4.3.1 */
        case PARAMETER_SERVICE_CATEGORY: /* Service Category, C.S0015-B v2.0, 3.4.3.2 */
            pdu[1] = 2;                  /* always 2 */
            pdu[2] = (uint8_t)((value >> 8) & 0xFF);
            pdu[3] = (uint8_t)(value & 0xFF);
            return 4;
            break;
        case PARAMETER_ORIGINATE_ADDRESS:   /* Originate Address, C.S0015-B v2.0, 3.4.3.3 */
        case PARAMETER_DESTINATION_ADDRESS: /* Destination Address,  C.S0015-B v2.0, 3.4.3.3 */
        {
            address_3gpp2_t *as = (address_3gpp2_t *)data;
            return (1 + address_encoder(as, &(pdu[1])));
            break;
        }
        case PARAMETER_ORIGINATE_SUBADDRESS:   /* Originate Subaddress, C.S0015-B v2.0, 3.4.3.4 */
        case PARAMETER_DESTINATION_SUBADDRESS: /* Destination Subaddress, C.S0015-B v2.0, 3.4.3.4 */
            RLOGW("%s: Unsupported Parameter", __FUNCTION__);
            break;
        case PARAMETER_BEARER_REPLY_OPTION:    /* Bearer Reply Option, C.S0015-B v2.0, 3.4.3.5 */
            pdu[1] = 1;                        /* length is always 1 */
            write_bits(value, 6, &shift_p, pdu, &index);
            return 3;
            break;
        case PARAMETER_CAUSE_CODE:  /* Cause Code, C.S0015-B v2.0, 3.4.3.6 */
        {
            pdu_3gpp2_fields *cc = (pdu_3gpp2_fields *)data;

            if (cc->CCErrClass == 0) {
                pdu[1] = 1;
            } else {
                pdu[1] = 2;
            }

            write_bits(cc->CCReplySeq, 6, &shift_p, pdu, &index);
            shift_p += 6;
            write_bits(cc->CCErrClass, 2, &shift_p, pdu, &index);

            if (cc->CCErrClass != 0) {
                pdu[3] = cc->CCErrCause;
            } else {
                return 3;
            }

            return 4;
            break;
        }
        case PARAMETER_BEARER_DATA:  /* Bearer Data, C.S0015-B v2.0, 3.4.3.7 */
        {
            bearer_data_fields_t *bdf = (bearer_data_fields_t *)data;
            return (1 + sms_sub_parameter_encoder(bdf, &(pdu[1])));
            break;
        }
        default:
            break;
    }

    return 0;
}

static addr_type_3gpp2_t get_address_type_3gpp2(const char *address)
{
    addr_type_3gpp2_t type = ADDR_TYPE_NATIONAL;

    assert(address);
    if (address[0] == '+') {
        type = ADDR_TYPE_INTERNATIONAL;
    } else if (address[0] == '0' && address[1] == '0') {
        type = ADDR_TYPE_INTERNATIONAL_00;
    }

    return type;
}

int32_t telit_helper_sms_encode_3gpp2(telit_helper_sms_encode_3gpp2_t *req)
{
    int32_t teleservice_id = 4098;
    uint8_t *pdu = req->pdu_message;
    uint32_t index = 0;
    address_3gpp2_t destination_address = {0};
    bearer_data_fields_t bdf = {0};
    addr_type_3gpp2_t addr_type = ADDR_TYPE_UNKNOWN;
    uint32_t j;

    assert (req != NULL && req->text_msg_length > 0);

    if (req->alphabet != ENCODING_ASCII_7BIT &&
            req->alphabet != ENCODING_OCTET &&
            req->alphabet != ENCODING_LATIN) {
        RLOGE("%s: Alphabet %d not supported", __FUNCTION__, req->alphabet);
        return 0;
    }

    if (strlen(req->dest_addr) == 0) {
        RLOGE("%s: Destination address must be set", __FUNCTION__);
        return 0;
    }

    pdu[index++] = 0;                                                                         /* Point-to-Point */

    index += encode_parameter(PARAMETER_TELESERVICE_ID, teleservice_id, NULL, &(pdu[index])); /* Teleservice ID */

    addr_type = get_address_type_3gpp2(req->dest_addr);

    if (addr_type == ADDR_TYPE_INTERNATIONAL) {
        destination_address.number_type = ADDR_TYPE_INTERNATIONAL;                               /* international number */
        destination_address.num_fields = strlen(req->dest_addr) - 1;                             /* skip '+' sign */
        memcpy(destination_address.address, req->dest_addr + 1, destination_address.num_fields); /* skip '+' sign */
    } else if (addr_type == ADDR_TYPE_INTERNATIONAL_00) {
        destination_address.number_type = ADDR_TYPE_INTERNATIONAL;                               /* international number */
        destination_address.num_fields = strlen(req->dest_addr) - 2;                             /* skip '00' */
        memcpy(destination_address.address, req->dest_addr + 2, destination_address.num_fields); /* skip '00' */
    } else {
        destination_address.number_type = ADDR_TYPE_NATIONAL;                                    /* national number */
        destination_address.num_fields = strlen(req->dest_addr);
        memcpy(destination_address.address, req->dest_addr, destination_address.num_fields);
    }

    destination_address.digit_mode = SMS_DIGIT_MODE_8BIT_ASCII;
    destination_address.number_mode = SMS_NUM_MODE_DIGIT;
    destination_address.number_plan = SMS_3GPP2_SMS_TELEPHONY_NUM_PLAN;

    index +=
            encode_parameter(PARAMETER_DESTINATION_ADDRESS, 0, &destination_address, &(pdu[index]));  /* Destination Address */

    bdf.bitmask = (0x1L << SUBPARAMETER_MESSAGE_ID);
    bdf.msg_type = SMS_3GPP2_SMS_MSG_TYPE_SUBMIT;
    bdf.msg_id = req->message_id;

    bdf.bitmask |= (0x1L << SUBPARAMETER_USER_DATA);
    bdf.msg_encoding = req->alphabet;
    bdf.num_fields = req->text_msg_length;

    for (j = 0; j < bdf.num_fields; j++) {
        bdf.message[j] = req->text_msg[j];
    }

    if (req->rel_validity != 0xFF) {
        bdf.bitmask |= (0x1L << SUBPARAMETER_VALIDITY_PERIOD_REL);
        bdf.rel_validity = req->rel_validity;
    }

    if (req->priority != 0xFF) {
        bdf.bitmask |= (0x1L << SUBPARAMETER_PRIORITY);
        bdf.priority = req->priority;
    }

    if (strlen(req->cbk_address) > 0) {
        bdf.bitmask |= (0x1L << SUBPARAMETER_CALLBACK_NUMBER);
        addr_type = get_address_type_3gpp2(req->cbk_address);

        if (addr_type == ADDR_TYPE_INTERNATIONAL) {
            bdf.callback_address.number_type = ADDR_TYPE_INTERNATIONAL;                                  /* international number */
            bdf.callback_address.num_fields = strlen(req->cbk_address) - 1;                              /* skip '+' sign */
            memcpy(bdf.callback_address.address, req->cbk_address + 1, bdf.callback_address.num_fields); /* skip '+' sign */
        } else if (addr_type == ADDR_TYPE_INTERNATIONAL_00) {
            bdf.callback_address.number_type = ADDR_TYPE_INTERNATIONAL;                                  /* international number */
            bdf.callback_address.num_fields = strlen(req->cbk_address) - 2;                              /* skip '00' */
            memcpy(bdf.callback_address.address, req->cbk_address + 2, bdf.callback_address.num_fields); /* skip '00' */
        } else {
            bdf.callback_address.number_type = ADDR_TYPE_NATIONAL;                                       /* national number */
            bdf.callback_address.num_fields = strlen(req->cbk_address);
            memcpy(bdf.callback_address.address, req->cbk_address, bdf.callback_address.num_fields);
        }

        bdf.callback_address.digit_mode = SMS_DIGIT_MODE_8BIT_ASCII;
        bdf.callback_address.number_mode = SMS_NUM_MODE_DIGIT;
        bdf.callback_address.number_plan = SMS_3GPP2_SMS_TELEPHONY_NUM_PLAN;
    }

    RLOGD("%s: Encoding msg (id %d) \"%s\", num_fields: %d, priority: %d, val %d, addr \"%s\", cbk \"%s\"",
            __FUNCTION__, bdf.msg_id, bdf.message, bdf.num_fields, bdf.priority, req->rel_validity,
            req->dest_addr, req->cbk_address);

    index += encode_parameter(PARAMETER_BEARER_DATA, 0, &bdf, &(pdu[index]));

    return req->message_size = index;
}

static uint8_t
read_text_bits(const uint8_t *bytes, uint8_t b_offset, uint8_t n_bits)
{
    uint8_t ret = 0;

    assert(b_offset < 8);

    if (n_bits > 8) {
        return ret;
    }

    if (b_offset + n_bits <= 8) {
        ret = (bytes[0] >> (8 - b_offset - n_bits)) & ((1 << n_bits) - 1);
    } else {
        uint8_t b_tail;
        uint8_t b_head;

        b_tail = 8 - b_offset;
        b_head = n_bits - b_tail;

        ret = (read_text_bits(&bytes[0], b_offset, b_tail) << b_head) |
                read_text_bits(&bytes[1], 0, b_head);

    }

    return ret;
}

static int32_t decode_message(const uint8_t encoding, const uint8_t num_fields,
        uint8_t *data, const uint32_t len,
        char *text_msg, uint32_t *text_len)
{
    char text[MAX_TEXT_MSG_LEN + 1] = {0};
    uint32_t inner_text_len = 0;
    uint8_t byte_offset = 0;
    uint8_t bit_offset = 0;
    uint8_t exp_size;
    uint8_t i;

    switch (encoding) {
        case ENCODING_OCTET:
            exp_size = 1 + ((num_fields * 8) / 8);
            if (len < exp_size) {
                RLOGE("%s: could not read user data: got %u, needed %u", __FUNCTION__, len, exp_size);
                break;
            }

            for (i = 0; i < num_fields; i++) {
                text[i] = read_text_bits(&data[byte_offset], bit_offset, 8);
                bit_offset += 8;
                if (bit_offset >= 8) {
                    bit_offset -= 8;
                    byte_offset++;
                }
            }
            break;
        case ENCODING_ASCII_7BIT:
            exp_size = 1 + ((num_fields * 7) / 8);
            if (len < exp_size) {
                RLOGE("%s: could not read user data: got %u, needed %u", __FUNCTION__, len, exp_size);
                break;
            }

            for (i = 0; i < num_fields; i++) {
                text[i] = read_text_bits(&data[byte_offset], bit_offset, 7);
                bit_offset += 7;
                if (bit_offset >= 8) {
                    bit_offset -= 8;
                    byte_offset++;
                }
            }
            break;
        case ENCODING_UNICODE:
            /* UTF-8 encoding */
            exp_size = 1 + ((num_fields * 8) / 8);
            if (len < exp_size) {
                RLOGE("%s: could not read user data: got %u, needed %u", __FUNCTION__, len, exp_size);
                break;
            }

            for (i = 0; i < num_fields; i++) {
                text[i] = read_text_bits(&data[byte_offset], bit_offset, 8);
                bit_offset += 8;
                if (bit_offset >= 8) {
                    bit_offset -= 8;
                    byte_offset++;
                }
            }
            break;
        case ENCODING_LATIN:
            exp_size = 1 + ((num_fields * 8) / 8);
            if (len < exp_size) {
                RLOGE("%s: could not read user data: got %u, needed %u", __FUNCTION__, len, exp_size);
                break;
            }

            for (i = 0; i < num_fields; i++) {
                text[i] = read_text_bits(&data[byte_offset], bit_offset, 8);
                bit_offset += 8;
                if (bit_offset >= 8) {
                    bit_offset -= 8;
                    byte_offset++;
                }
            }
            /* TODO: conversion from ISO−8859−1 to UTF-8 might be needed */
            break;
        default:
            RLOGE("%s: unsupported encoding %d", __FUNCTION__, encoding);
            break;
    }

    inner_text_len = strnlen(text, MAX_TEXT_MSG_LEN);
    if (!inner_text_len) {
        RLOGE("%s: could not decode text message", __FUNCTION__);
        telit_print_buffer((uint8_t *)text, MAX_TEXT_MSG_LEN, LOG_ERR);
        return 0;
    }

    RLOGD ("%s: decoded text: (%d) '%s'", __FUNCTION__, inner_text_len, text);
    *text_len = inner_text_len;
    strlcpy(text_msg, text, MAX_TEXT_MSG_LEN + 1);

    return inner_text_len;
}
