#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "telit_sdk.h"
#include "telit_sms_helper_private.h"

unicode_to_gsm_t unicode_gsm_extended_set[GSM_EXTENDED_SET_SIZE + 1] = {
    {0x0A, 1, {0x0C, 0x00, 0x00}},  /* FF, Page Break control */
    {0x14, 1, {0x5E, 0x00, 0x00}},  /* ^ */
    {0x28, 1, {0x7B, 0x00, 0x00}},  /* { */
    {0x29, 1, {0x7D, 0x00, 0x00}},  /* } */
    {0x2F, 1, {0x5C, 0x00, 0x00}},  /* backslash */
    {0x3C, 1, {0x5B, 0x00, 0x00}},  /* [ */
    {0x3D, 1, {0x7E, 0x00, 0x00}},  /* ~ */
    {0x3E, 1, {0x5D, 0x00, 0x00}},  /* ] */
    {0x40, 1, {0x7C, 0x00, 0x00}},  /* | */
    {0x65, 3, {0xE2, 0x82, 0xAC}},  /* € */
    GSM_SET_EMPTY_ITEM
};

unicode_to_gsm_t unicode_gsm_base_set[GSM_BASE_SET_SIZE + 1] = {
    {0x00, 1, {0x40, 0x00, 0x00}},  /* @ */
    {0x01, 2, {0xC2, 0xA3, 0x00}},  /* £ */
    {0x02, 1, {0x24, 0x00, 0x00}},  /* $ */
    {0x03, 2, {0xC2, 0xA5, 0x00}},  /* ¥ */
    {0x04, 2, {0xC3, 0xA8, 0x00}},  /* è */
    {0x05, 2, {0xC3, 0xA9, 0x00}},  /* é */
    {0x06, 2, {0xC3, 0xB9, 0x00}},  /* ù */
    {0x07, 2, {0xC3, 0xAC, 0x00}},  /* ì */
    {0x08, 2, {0xC3, 0xB2, 0x00}},  /* ò */
    {0x09, 2, {0xC3, 0x87, 0x00}},  /* Ç */
    {0x0A, 1, {0x0A, 0x00, 0x00}},  /* line feed */
    {0x0B, 2, {0xC3, 0x98, 0x00}},  /* Ø */
    {0x0C, 2, {0xC3, 0xB8, 0x00}},  /* ø */
    {0x0D, 1, {0x0D, 0x00, 0x00}},  /* carriage return */
    {0x0E, 2, {0xC3, 0x85, 0x00}},  /* Å */
    {0x0F, 2, {0xC3, 0xA5, 0x00}},  /* å */
    {0x10, 2, {0xCE, 0x94, 0x00}},  /* Δ */
    {0x11, 1, {0x5F, 0x00, 0x00}},  /* _ */
    {0x12, 2, {0xCE, 0xA6, 0x00}},  /* Φ */
    {0x13, 2, {0xCE, 0x93, 0x00}},  /* Γ */
    {0x14, 2, {0xCE, 0x9B, 0x00}},  /* Λ */
    {0x15, 2, {0xCE, 0xA9, 0x00}},  /* Ω */
    {0x16, 2, {0xCE, 0xA0, 0x00}},  /* Π */
    {0x17, 2, {0xCE, 0xA8, 0x00}},  /* Ψ */
    {0x18, 2, {0xCE, 0xA3, 0x00}},  /* Σ */
    {0x19, 2, {0xCE, 0x98, 0x00}},  /* Θ */
    {0x1A, 2, {0xCE, 0x9E, 0x00}},  /* Ξ */
    /* 0x1B is the escape character to access the extended alphabet */
    {0x1C, 2, {0xC3, 0x86, 0x00}},  /* Æ */
    {0x1D, 2, {0xC3, 0xA6, 0x00}},  /* æ */
    {0x1E, 2, {0xC3, 0x9F, 0x00}},  /* ß */
    {0x1F, 2, {0xC3, 0x89, 0x00}},  /* É */
    {0x20, 1, {0x20, 0x00, 0x00}},  /* SPACE */
    {0x21, 1, {0x21, 0x00, 0x00}},  /* ! */
    {0x22, 1, {0x22, 0x00, 0x00}},  /* " */
    {0x23, 1, {0x23, 0x00, 0x00}},  /* # */
    {0x24, 2, {0xC2, 0xA4, 0x00}},  /* ¤ */
    {0x25, 1, {0x25, 0x00, 0x00}},  /* % */
    {0x26, 1, {0x26, 0x00, 0x00}},  /* & */
    {0x27, 1, {0x27, 0x00, 0x00}},  /* ' */
    {0x28, 1, {0x28, 0x00, 0x00}},  /* ( */
    {0x29, 1, {0x29, 0x00, 0x00}},  /* ) */
    {0x2A, 1, {0x2A, 0x00, 0x00}},  /* * */
    {0x2B, 1, {0x2B, 0x00, 0x00}},  /* + */
    {0x2C, 1, {0x2C, 0x00, 0x00}},  /* , */
    {0x2D, 1, {0x2D, 0x00, 0x00}},  /* - */
    {0x2E, 1, {0x2E, 0x00, 0x00}},  /* . */
    {0x2F, 1, {0x2F, 0x00, 0x00}},  /* / */
    {0x30, 1, {0x30, 0x00, 0x00}},  /* 0 */
    {0x31, 1, {0x31, 0x00, 0x00}},  /* 1 */
    {0x32, 1, {0x32, 0x00, 0x00}},  /* 2 */
    {0x33, 1, {0x33, 0x00, 0x00}},  /* 3 */
    {0x34, 1, {0x34, 0x00, 0x00}},  /* 4 */
    {0x35, 1, {0x35, 0x00, 0x00}},  /* 5 */
    {0x36, 1, {0x36, 0x00, 0x00}},  /* 6 */
    {0x37, 1, {0x37, 0x00, 0x00}},  /* 7 */
    {0x38, 1, {0x38, 0x00, 0x00}},  /* 8 */
    {0x39, 1, {0x39, 0x00, 0x00}},  /* 9 */
    {0x3A, 1, {0x3A, 0x00, 0x00}},  /* : */
    {0x3B, 1, {0x3B, 0x00, 0x00}},  /* ; */
    {0x3C, 1, {0x3C, 0x00, 0x00}},  /* < */
    {0x3D, 1, {0x3D, 0x00, 0x00}},  /* = */
    {0x3E, 1, {0x3E, 0x00, 0x00}},  /* > */
    {0x3F, 1, {0x3F, 0x00, 0x00}},  /* ? */
    {0x40, 2, {0xC2, 0xA1, 0x00}},  /* ¡ */
    {0x41, 1, {0x41, 0x00, 0x00}},  /* A */
    {0x42, 1, {0x42, 0x00, 0x00}},  /* B */
    {0x43, 1, {0x43, 0x00, 0x00}},  /* C */
    {0x44, 1, {0x44, 0x00, 0x00}},  /* D */
    {0x45, 1, {0x45, 0x00, 0x00}},  /* E */
    {0x46, 1, {0x46, 0x00, 0x00}},  /* F */
    {0x47, 1, {0x47, 0x00, 0x00}},  /* G */
    {0x48, 1, {0x48, 0x00, 0x00}},  /* H */
    {0x49, 1, {0x49, 0x00, 0x00}},  /* I */
    {0x4A, 1, {0x4A, 0x00, 0x00}},  /* J */
    {0x4B, 1, {0x4B, 0x00, 0x00}},  /* K */
    {0x4C, 1, {0x4C, 0x00, 0x00}},  /* L */
    {0x4D, 1, {0x4D, 0x00, 0x00}},  /* M */
    {0x4E, 1, {0x4E, 0x00, 0x00}},  /* N */
    {0x4F, 1, {0x4F, 0x00, 0x00}},  /* O */
    {0x50, 1, {0x50, 0x00, 0x00}},  /* P */
    {0x51, 1, {0x51, 0x00, 0x00}},  /* Q */
    {0x52, 1, {0x52, 0x00, 0x00}},  /* R */
    {0x53, 1, {0x53, 0x00, 0x00}},  /* S */
    {0x54, 1, {0x54, 0x00, 0x00}},  /* T */
    {0x55, 1, {0x55, 0x00, 0x00}},  /* U */
    {0x56, 1, {0x56, 0x00, 0x00}},  /* V */
    {0x57, 1, {0x57, 0x00, 0x00}},  /* W */
    {0x58, 1, {0x58, 0x00, 0x00}},  /* X */
    {0x59, 1, {0x59, 0x00, 0x00}},  /* Y */
    {0x5A, 1, {0x5A, 0x00, 0x00}},  /* Z */
    {0x5B, 2, {0xC3, 0x84, 0x00}},  /* Ä */
    {0x5C, 2, {0xC3, 0x96, 0x00}},  /* Ö */
    {0x5D, 2, {0xC3, 0x91, 0x00}},  /* Ñ */
    {0x5E, 2, {0xC3, 0x9C, 0x00}},  /* Ü */
    {0x5F, 2, {0xC2, 0xA7, 0x00}},  /* § */
    {0x60, 2, {0xC2, 0xBF, 0x00}},  /* ¿ */
    {0x61, 1, {0x61, 0x00, 0x00}},  /* a */
    {0x62, 1, {0x62, 0x00, 0x00}},  /* b */
    {0x63, 1, {0x63, 0x00, 0x00}},  /* c */
    {0x64, 1, {0x64, 0x00, 0x00}},  /* d */
    {0x65, 1, {0x65, 0x00, 0x00}},  /* e */
    {0x66, 1, {0x66, 0x00, 0x00}},  /* f */
    {0x67, 1, {0x67, 0x00, 0x00}},  /* g */
    {0x68, 1, {0x68, 0x00, 0x00}},  /* h */
    {0x69, 1, {0x69, 0x00, 0x00}},  /* i */
    {0x6A, 1, {0x6A, 0x00, 0x00}},  /* j */
    {0x6B, 1, {0x6B, 0x00, 0x00}},  /* k */
    {0x6C, 1, {0x6C, 0x00, 0x00}},  /* l */
    {0x6D, 1, {0x6D, 0x00, 0x00}},  /* m */
    {0x6E, 1, {0x6E, 0x00, 0x00}},  /* n */
    {0x6F, 1, {0x6F, 0x00, 0x00}},  /* o */
    {0x70, 1, {0x70, 0x00, 0x00}},  /* p */
    {0x71, 1, {0x71, 0x00, 0x00}},  /* q */
    {0x72, 1, {0x72, 0x00, 0x00}},  /* r */
    {0x73, 1, {0x73, 0x00, 0x00}},  /* s */
    {0x74, 1, {0x74, 0x00, 0x00}},  /* t */
    {0x75, 1, {0x75, 0x00, 0x00}},  /* u */
    {0x76, 1, {0x76, 0x00, 0x00}},  /* v */
    {0x77, 1, {0x77, 0x00, 0x00}},  /* w */
    {0x78, 1, {0x78, 0x00, 0x00}},  /* x */
    {0x79, 1, {0x79, 0x00, 0x00}},  /* y */
    {0x7A, 1, {0x7A, 0x00, 0x00}},  /* z */
    {0x7B, 2, {0xC3, 0xA4, 0x00}},  /* ä */
    {0x7C, 2, {0xC3, 0xB6, 0x00}},  /* ö */
    {0x7D, 2, {0xC3, 0xB1, 0x00}},  /* ñ */
    {0x7E, 2, {0xC3, 0xBC, 0x00}},  /* ü */
    {0x7F, 2, {0xC3, 0xA0, 0x00}},  /* à */
    GSM_SET_EMPTY_ITEM
};


addr_type_t get_address_type(const char *address)
{
    addr_type_t type = ADDRESS_TYPE_NATIONAL;

    assert(address);
    if (address[0] == '+') {
        type = ADDRESS_TYPE_INTERNATIONAL;
    }

    return type;
}

uint8_t string_to_byte_array(const char *string, const uint32_t string_len, uint8_t *byte_array,
        const uint32_t max_byte_array_len)
{
    uint8_t i, j = 0;
    uint8_t byte_array_len;

    assert(string);
    assert(byte_array);

    byte_array_len = (string_len % 2) ? (string_len / 2) + 1 : string_len;
    if (max_byte_array_len < byte_array_len) {
        RLOGE("%s: could not convert string '%s' into byte array: out of space",
                __FUNCTION__, string);
    }

    for (i = 0; i < string_len; i += 2) {
        uint64_t value = 0;
        char sub[3];

        strlcpy(sub, string + i, sizeof(sub));
        value = strtoul(sub, NULL, 16);
        byte_array[j++] = (value > 0xff) ? 0xff : (uint8_t)value;
    }

    return j;
}

uint8_t swap_msb_lsb_in_byte_array(uint8_t *array, const uint32_t len)
{
    uint32_t i;

    assert(array);

    for (i = 0; i < len; i++) {
        uint8_t pivot;
        pivot = array[i] & 0x0F;
        array[i] = (array[i] & 0xF0) >> 4;
        array[i] |= pivot << 4;
    }

    return i;
}

int32_t string_address_to_dso(const char *string, const uint8_t string_len, uint8_t *encoded)
{
    uint8_t len;
    char copy[MAX_STRING_SIZE] = {0};

    assert(string);
    assert(encoded);

    memcpy(copy, string, string_len);

    len = string_len / 2;

    if (string_len % 2) {
        /* String is odd, padding it with 'F'*/
        copy[string_len] = 'F';
        len++;
    }

    string_to_byte_array(copy, string_len, encoded, 1024);
    return swap_msb_lsb_in_byte_array(encoded, len);
}

int32_t dso_number_to_string(const uint8_t *encoded, const uint32_t encoded_len, char *string,
        const uint32_t max_string_len)
{
    int32_t ret;
    uint32_t i, j = 0;
    uint8_t offset = 0x30;

    assert(encoded);
    assert(string);

    for (i = 0; i < encoded_len; i++) {
        int8_t converted = 0;

        converted = (encoded[i] & 0x0F) + offset;
        if (converted < 0x30 || converted > 0x39) {
            RLOGE("%s: could not convert byte 0x%02X into number: out of bound",
                    __FUNCTION__, encoded[i]);
            ret = -1;
            goto end;
        }
        string[j++] = converted;

        converted = (encoded[i] >> 4) & 0x0F;
        if (converted == 0x0F) {
            /* the number is odd, the conversion is done already */
            ret = (int32_t)j;
            goto end;
        }

        converted += offset;
        if ((converted < 0x30 || converted > 0x39)) {
            RLOGE("%s: could not convert byte 0x%02X into string: out of bound",
                    __FUNCTION__, encoded[i]);
            ret = -1;
            goto end;
        }

        if (j + 1 >= max_string_len) {
            RLOGE("%s: number conversion error: not enough space", __FUNCTION__);
            ret = -1;
            goto end;
        }
        string[j++] = converted;
    }

    ret = (int32_t)j;

end:
    return ret;
}

char *get_next_unicode_char(const char *msg, const uint32_t len)
{
    uint32_t left = len;

    assert(msg);

    for (++msg; --left && ((*msg & 0xC0) == 0x80); ++msg) {
    }

    return (char *)msg;
}

static int8_t is_unicode_char_in_map(const char *src, const uint8_t src_len, const unicode_to_gsm_t *map)
{
    int8_t i;

    assert(src);
    assert(map);

    for (i = 0; map[i].unicode_len != 0; i++) {
        if (src_len != map[i].unicode_len) {
            continue;
        }

        if (memcmp(src, map[i].unicode, src_len) != 0) {
            continue;
        }

        return i;
    }

    return -1;
}

uint32_t unicode_to_unpacked_gsm(const char *msg, uint32_t msg_len, uint8_t *gsm)
{
    char *curr;
    uint8_t curr_len = 0;
    uint32_t len = 0;
    uint32_t i;

    assert(msg);
    assert(gsm);

    curr = (char *)msg;

    for (i = 0; i < msg_len; i += curr_len) {
        int8_t match;
        char *next;

        next = get_next_unicode_char(curr, msg_len - i);
        curr_len = next - curr;

        match = is_unicode_char_in_map(curr, curr_len, unicode_gsm_extended_set);
        if (match >= 0) {
            gsm[len++] = GSM_ESCAPE_CHAR;
            gsm[len++] = unicode_gsm_extended_set[match].gsm;
        } else {
            match = is_unicode_char_in_map(curr, curr_len, unicode_gsm_base_set);
            if (match >= 0) {
                gsm[len++] = unicode_gsm_base_set[match].gsm;
            }
        }

        if (match < 0) {
            int k;
            RLOGE("%s: could not convert sequence at index %d of len %d: ", __FUNCTION__, i, curr_len);
            for (k = 0; k < curr_len; k++) {
                RLOGE("%s: 0x%02X ", __FUNCTION__, (uint8_t)curr[k]);
            }
            len = 0;
            goto end;
        }

        curr = next;
    }

end:
    return len;
}

uint32_t unpacked_gsm_to_unicode(const uint8_t *gsm,
        const uint32_t gsm_len,
        char *msg,
        const uint32_t max_msg_len)
{
    uint8_t i;
    uint32_t len = 0;

    assert(gsm);
    assert(msg);

    for (i = 0; i < gsm_len; i++) {
        unicode_to_gsm_t *map = NULL;
        bool converted = false;
        uint8_t j;

        if (gsm[i] == GSM_ESCAPE_CHAR) {
            map = unicode_gsm_extended_set;
            i++;
        } else {
            map = unicode_gsm_base_set;
        }

        for (j = 0; !converted && (map[j].unicode_len != 0x00); j++) {
            if (gsm[i] != map[j].gsm) {
                continue;
            }

            if (max_msg_len < len + map[j].unicode_len) {
                RLOGE("%s: conversion error: not enough space (%d < %d + %d)",
                        __FUNCTION__, max_msg_len, len, map[j].unicode_len);
                goto end;
            }

            memcpy(&msg[len], map[j].unicode, map[j].unicode_len);
            len += map[j].unicode_len;
            converted = true;
        }

        if (!converted) {
            RLOGE("%s: could not convert unpacked GSM 0x%02X to unicode",
                    __FUNCTION__, gsm[i]);
            len = 0;
            break;
        }
    }

end:
    return len;
}

uint32_t gsm_pack(const uint8_t *src, uint32_t src_len, uint8_t *packed, uint32_t packed_max_len)
{
    uint8_t shift;
    uint32_t i, j, len;
    uint32_t mask_7bits = 0x7F;

    assert(src);
    assert(packed);

    memset(packed, 0, packed_max_len);

    len = (src_len * 7);             /* len in bits first */
    len = (len % 8) ? len + 8 : len; /* check if one more byte is needed */
    len /= 8;                        /* len in bytes at last */

    if (packed_max_len < len) {
        RLOGE("%s: pack error: not enough space", __FUNCTION__);
        len = 0;
        goto end;
    }

    for (i = 0, j = 0, shift = 0; i < src_len; i++) {
        packed[j] |= (src[i] & mask_7bits) << shift;

        if (shift > 1) {
            packed[j + 1] = (src[i] & mask_7bits) >> (8 - shift);
        }

        j = shift ? j + 1 : j;
        shift = shift ? shift - 1 : 7;
    }

end:
    return len;
}

uint32_t gsm_unpack(const uint8_t *gsm, const uint32_t septets, uint8_t *unpacked,
        const uint32_t unpacked_max_len)
{
    uint32_t i = 0;

    assert(gsm);
    assert(unpacked);

    memset(unpacked, 0, unpacked_max_len);

    while (i < septets) {
        uint8_t cur_pos;
        uint8_t next_pos;
        uint8_t current;
        uint8_t offset;
        uint8_t tmp;
        uint32_t needle;

        needle = (i * 7);
        offset = needle % 8;
        cur_pos = offset ? (8 - offset) : 7;
        next_pos = 7 - cur_pos;

        current = gsm[needle / 8];
        tmp = (current >> offset) & (0xFF >> (8 - cur_pos));

        if (next_pos) {
            current = gsm[(needle / 8) + 1];
            tmp |= (current & (0xFF >> (8 - next_pos))) << cur_pos;
        }

        unpacked[i++] = tmp;
    }

    return i;
}
