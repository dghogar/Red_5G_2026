#include <stdint.h>
#include <stddef.h>
#include <string.h>
#if defined __WINDOWS_QMI__
#include "portable_endian.h"
#else
#include <endian.h>
#endif

#include "telit_sdk.h"
#include "telit_sdk_private.h"
#include "telit_sdk_uim.h"
#include "telit_sdk_private_uim.h"

/*****************************************************************************
 * QMI_UIM_READ_TRANSPARENT (32 - 0x0020)
 */

QMI_SDK_error_t telit_uim_read_transparent_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_read_file_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_generic_session_t *tlv_0x01;
    QMI_UIM_generic_file_id_t *tlv_0x02;
    QMI_UIM_generic_read_info_t *tlv_0x03;
    uint32_t hdr_sz;
    uint16_t tlv_sz;
    uint8_t *aid_string;
    uint8_t i;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_READ_TRANSPARENT;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->session_information.aid_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_generic_session_t *)buf_position;
    COPY_HTOM(tlv_0x01->session_type, input->session_information.session_type);
    COPY_HTOM(tlv_0x01->aid_len, input->session_information.aid_len);
    aid_string = (uint8_t *)(&(tlv_0x01->aid_len));
    /* Moving to the start of the string */
    aid_string += sizeof(tlv_0x01->aid_len);
    memcpy(aid_string, input->session_information.aid, input->session_information.aid_len);

    buf_position += tlv_sz;

    /* Pack QMI_UIM_generic_file_id_t */
    tlv_sz = sizeof(*tlv_0x02) + input->file_path_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_UIM_generic_file_id_t *)buf_position;
    COPY_HTOM(tlv_0x02->file_id, input->file_id);
    COPY_HTOM(tlv_0x02->path_len, input->file_path_len);
    buf_position = (uint8_t *)(&(tlv_0x02->path_len));
    /* Moving to the start of the array */
    buf_position += sizeof(tlv_0x02->path_len);
    for (i = 0; i < input->file_path_len / 2; i++) {
        uint16_t parent_id = input->file_path[i];

        *buf_position = (uint8_t)(parent_id & 0xFF);
        buf_position++;

        *buf_position = (uint8_t)((parent_id >> 8) & 0xFF);
        buf_position++;
    }

    /* Pack QMI_UIM_generic_read_info_t */
    tlv_sz = sizeof(*tlv_0x03);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x03;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x03 = (QMI_UIM_generic_read_info_t *)buf_position;
    tlv_0x03->offset = 0;
    tlv_0x03->length = 0;

    buf_position += tlv_sz;

    if (input->response_as_indication_available) {
        QMI_UIM_generic_ind_token_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_UIM_generic_ind_token_t *)buf_position;
        COPY_HTOM(tlv_0x10->indication_token, input->indication_token);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

static QMI_SDK_error_t telit_uim_generic_read_transparent_unpack(uint8_t *rsp, uint16_t len,
        uim_read_file_info_t *out, bool indication)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    if (indication) {
        QMI_UIM_generic_ind_token_t *tlv_0x01;

        RLOGD("%s: parsing indication", __FUNCTION__);

        payload_len = len;
        msg = rsp;
        ret = QMI_SDK_ERR_NONE;

        FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_ind_token_t);

        hdr++;
        tlv_0x01 = (QMI_UIM_generic_ind_token_t *)hdr;
        out->response_as_indication_available = true;
        COPY_MTOH(out->indication_token, tlv_0x01->indication_token);
    } else {
        RLOGD("%s: parsing response", __FUNCTION__);

        ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
        /* Not exiting for protocol errors, since we want to collect the retries */
        if (ret != QMI_SDK_ERR_NONE && ret < QMI_SDK_ERR_ERR_QMI_OFFSET) {
            RLOGE("%s: validate unpack failed", __FUNCTION__);
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_UIM_generic_card_result_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_card_result_t);

        hdr++;
        tlv_0x10 = (QMI_UIM_generic_card_result_t *)hdr;
        out->card_result_available = true;
        COPY_MTOH(out->card_result.sw1, tlv_0x10->sw1);
        COPY_MTOH(out->card_result.sw2, tlv_0x10->sw2);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_UIM_generic_read_content_t *tlv_0x11;
        uint8_t *content;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_read_content_t);

        hdr++;
        tlv_0x11 = (QMI_UIM_generic_read_content_t *)hdr;
        out->read_result_available = true;
        COPY_MTOH(out->read_result.len, tlv_0x11->content_len);

        if (out->read_result.len > QMI_UIM_MAX_FILE_LENGTH) {
            RLOGE("%s: read_result.len = %u > QMI_UIM_MAX_FILE_LENGTH, increase the define",
                    __FUNCTION__, out->read_result.len);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        content = (uint8_t *)(&(tlv_0x11->content_len));
        /* Moving to the start of the string */
        content += sizeof(tlv_0x11->content_len);
        memcpy(out->read_result.content, content, out->read_result.len);
    }

    if (!indication) {
        hdr = search_TLV(msg, 0x12, payload_len);
        /* TLV 0x12 is optional */
        if (hdr) {
            QMI_UIM_generic_ind_token_t *tlv_0x12;

            CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_ind_token_t);

            hdr++;
            tlv_0x12 = (QMI_UIM_generic_ind_token_t *)hdr;
            out->response_as_indication_available = true;
            COPY_MTOH(out->indication_token, tlv_0x12->indication_token);
        }
    }

end:
    return ret;
}

QMI_SDK_error_t telit_uim_read_transparent_unpack(uint8_t *rsp, uint16_t len, uim_read_file_info_t *out)
{
    return telit_uim_generic_read_transparent_unpack(rsp, len, out, false);
}

/*****************************************************************************
 * QMI_UIM_READ_RECORD (33 - 0x0021)
 */

QMI_SDK_error_t telit_uim_read_record_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_read_record_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_generic_session_t *tlv_0x01;
    QMI_UIM_generic_file_id_t *tlv_0x02;
    QMI_UIM_generic_read_info_t *tlv_0x03;
    uint32_t hdr_sz;
    uint16_t tlv_sz;
    uint8_t *aid_string;
    uint8_t i;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_READ_RECORD;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->session_information.aid_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_generic_session_t *)buf_position;
    COPY_HTOM(tlv_0x01->session_type, input->session_information.session_type);
    COPY_HTOM(tlv_0x01->aid_len, input->session_information.aid_len);
    aid_string = (uint8_t *)(&(tlv_0x01->aid_len));
    /* Moving to the start of the string */
    aid_string += sizeof(tlv_0x01->aid_len);
    memcpy(aid_string, input->session_information.aid, input->session_information.aid_len);

    buf_position += tlv_sz;

    /* Pack QMI_UIM_generic_file_id_t */
    tlv_sz = sizeof(*tlv_0x02) + input->file_path_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_UIM_generic_file_id_t *)buf_position;
    COPY_HTOM(tlv_0x02->file_id, input->file_id);
    COPY_HTOM(tlv_0x02->path_len, input->file_path_len);
    buf_position = (uint8_t *)(&(tlv_0x02->path_len));
    /* Moving to the start of the array */
    buf_position += sizeof(tlv_0x02->path_len);
    for (i = 0; i < input->file_path_len / 2; i++) {
        uint16_t parent_id = input->file_path[i];

        *buf_position = (uint8_t)(parent_id & 0xFF);
        buf_position++;

        *buf_position = (uint8_t)((parent_id >> 8) & 0xFF);
        buf_position++;
    }

    /* Pack QMI_UIM_generic_read_info_t */
    tlv_sz = sizeof(*tlv_0x03);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x03;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x03 = (QMI_UIM_generic_read_info_t *)buf_position;
    COPY_HTOM(tlv_0x03->offset, input->record);
    COPY_HTOM(tlv_0x03->length, input->record_length);

    buf_position += tlv_sz;

    if (input->response_as_indication_available) {
        QMI_UIM_generic_ind_token_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_UIM_generic_ind_token_t *)buf_position;
        COPY_HTOM(tlv_0x11->indication_token, input->indication_token);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

static QMI_SDK_error_t telit_uim_generic_read_record_unpack(uint8_t *rsp, uint16_t len,
        uim_read_record_info_t *out, bool indication)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    if (indication) {
        QMI_UIM_generic_ind_token_t *tlv_0x01;

        RLOGD("%s: parsing indication", __FUNCTION__);

        payload_len = len;
        msg = rsp;
        ret = QMI_SDK_ERR_NONE;

        FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_ind_token_t);

        hdr++;
        tlv_0x01 = (QMI_UIM_generic_ind_token_t *)hdr;
        out->response_as_indication_available = true;
        COPY_MTOH(out->indication_token, tlv_0x01->indication_token);
    } else {
        RLOGD("%s: parsing response", __FUNCTION__);

        ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
        /* Not exiting for protocol errors, since we want to collect the retries */
        if (ret != QMI_SDK_ERR_NONE && ret < QMI_SDK_ERR_ERR_QMI_OFFSET) {
            RLOGE("%s: validate unpack failed", __FUNCTION__);
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_UIM_generic_card_result_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_card_result_t);

        hdr++;
        tlv_0x10 = (QMI_UIM_generic_card_result_t *)hdr;
        out->card_result_available = true;
        COPY_MTOH(out->card_result.sw1, tlv_0x10->sw1);
        COPY_MTOH(out->card_result.sw2, tlv_0x10->sw2);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_UIM_generic_read_content_t *tlv_0x11;
        uint8_t *content;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_read_content_t);

        hdr++;
        tlv_0x11 = (QMI_UIM_generic_read_content_t *)hdr;
        out->read_result_available = true;
        COPY_MTOH(out->read_result.len, tlv_0x11->content_len);

        if (out->read_result.len > QMI_UIM_MAX_FILE_LENGTH) {
            RLOGE("%s: read_result.len = %u > QMI_UIM_MAX_FILE_LENGTH, increase the define",
                    __FUNCTION__, out->read_result.len);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        content = (uint8_t *)(&(tlv_0x11->content_len));
        /* Moving to the start of the string */
        content += sizeof(tlv_0x11->content_len);
        memcpy(out->read_result.content, content, out->read_result.len);
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_UIM_generic_read_content_t *tlv_0x12;
        uint8_t *content;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_read_content_t);

        hdr++;
        tlv_0x12 = (QMI_UIM_generic_read_content_t *)hdr;
        out->additional_read_result_available = true;
        COPY_MTOH(out->additional_read_result.len, tlv_0x12->content_len);

        if (out->additional_read_result.len > QMI_UIM_MAX_FILE_LENGTH) {
            RLOGE("%s: additional_read_result.len = %u > QMI_UIM_MAX_FILE_LENGTH, increase the define",
                    __FUNCTION__, out->additional_read_result.len);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        content = (uint8_t *)(&(tlv_0x12->content_len));
        /* Moving to the start of the string */
        content += sizeof(tlv_0x12->content_len);
        memcpy(out->additional_read_result.content, content, out->additional_read_result.len);
    }

    if (!indication) {
        hdr = search_TLV(msg, 0x13, payload_len);
        /* TLV 0x13 is optional */
        if (hdr) {
            QMI_UIM_generic_ind_token_t *tlv_0x13;

            CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_ind_token_t);

            hdr++;
            tlv_0x13 = (QMI_UIM_generic_ind_token_t *)hdr;
            out->response_as_indication_available = true;
            COPY_MTOH(out->indication_token, tlv_0x13->indication_token);
        }
    }

end:
    return ret;
}

QMI_SDK_error_t telit_uim_read_record_unpack(uint8_t *rsp, uint16_t len, uim_read_record_info_t *out)
{
    return telit_uim_generic_read_record_unpack(rsp, len, out, false);
}

/*****************************************************************************
 * QMI_UIM_GET_FILE_ATTRIBUTES (36 - 0x0024)
 */

QMI_SDK_error_t telit_uim_get_file_attributes_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_read_file_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_generic_session_t *tlv_0x01;
    QMI_UIM_generic_file_id_t *tlv_0x02;
    uint32_t hdr_sz;
    uint16_t tlv_sz;
    uint8_t *aid_string;
    uint8_t i;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_GET_FILE_ATTRIBUTES;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->session_information.aid_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_generic_session_t *)buf_position;
    COPY_HTOM(tlv_0x01->session_type, input->session_information.session_type);
    COPY_HTOM(tlv_0x01->aid_len, input->session_information.aid_len);
    aid_string = (uint8_t *)(&(tlv_0x01->aid_len));
    /* Moving to the start of the string */
    aid_string += sizeof(tlv_0x01->aid_len);
    memcpy(aid_string, input->session_information.aid, input->session_information.aid_len);

    buf_position += tlv_sz;

    /* Pack QMI_UIM_generic_file_id_t */
    tlv_sz = sizeof(*tlv_0x02) + input->file_path_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_UIM_generic_file_id_t *)buf_position;
    COPY_HTOM(tlv_0x02->file_id, input->file_id);
    COPY_HTOM(tlv_0x02->path_len, input->file_path_len);
    buf_position = (uint8_t *)(&(tlv_0x02->path_len));
    /* Moving to the start of the array */
    buf_position += sizeof(tlv_0x02->path_len);
    for (i = 0; i < input->file_path_len / 2; i++) {
        uint16_t parent_id = input->file_path[i];

        *buf_position = (uint8_t)(parent_id & 0xFF);
        buf_position++;

        *buf_position = (uint8_t)((parent_id >> 8) & 0xFF);
        buf_position++;
    }

    if (input->response_as_indication_available) {
        QMI_UIM_generic_ind_token_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_UIM_generic_ind_token_t *)buf_position;
        COPY_HTOM(tlv_0x10->indication_token, input->indication_token);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

static QMI_SDK_error_t telit_uim_generic_get_file_attributes_unpack(uint8_t *rsp, uint16_t len,
        uim_read_file_info_t *out, bool indication)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    if (indication) {
        QMI_UIM_generic_ind_token_t *tlv_0x01;

        RLOGD("%s: parsing indication", __FUNCTION__);

        payload_len = len;
        msg = rsp;
        ret = QMI_SDK_ERR_NONE;

        FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_ind_token_t);

        hdr++;
        tlv_0x01 = (QMI_UIM_generic_ind_token_t *)hdr;
        out->response_as_indication_available = true;
        COPY_MTOH(out->indication_token, tlv_0x01->indication_token);
    } else {
        RLOGD("%s: parsing response", __FUNCTION__);

        ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
        /* Not exiting for protocol errors, since we want to collect the retries */
        if (ret != QMI_SDK_ERR_NONE && ret < QMI_SDK_ERR_ERR_QMI_OFFSET) {
            RLOGE("%s: validate unpack failed", __FUNCTION__);
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_UIM_generic_card_result_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_card_result_t);

        hdr++;
        tlv_0x10 = (QMI_UIM_generic_card_result_t *)hdr;
        out->card_result_available = true;
        COPY_MTOH(out->card_result.sw1, tlv_0x10->sw1);
        COPY_MTOH(out->card_result.sw2, tlv_0x10->sw2);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_UIM_GET_FILE_ATTRIBUTES_response_0x11_t *tlv_0x11;
        uint8_t *content;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_GET_FILE_ATTRIBUTES_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_UIM_GET_FILE_ATTRIBUTES_response_0x11_t *)hdr;
        out->read_result_available = true;
        COPY_MTOH(out->read_result.len, tlv_0x11->raw_len);

        if (out->read_result.len > QMI_UIM_MAX_FILE_LENGTH) {
            RLOGE("%s: read_result.len = %u > QMI_UIM_MAX_FILE_LENGTH, increase the define",
                    __FUNCTION__, out->read_result.len);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        content = (uint8_t *)(&(tlv_0x11->raw_len));
        /* Moving to the start of the string */
        content += sizeof(tlv_0x11->raw_len);
        memcpy(out->read_result.content, content, out->read_result.len);
    }

    if (!indication) {
        hdr = search_TLV(msg, 0x12, payload_len);
        /* TLV 0x12 is optional */
        if (hdr) {
            QMI_UIM_generic_ind_token_t *tlv_0x12;

            CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_ind_token_t);

            hdr++;
            tlv_0x12 = (QMI_UIM_generic_ind_token_t *)hdr;
            out->response_as_indication_available = true;
            COPY_MTOH(out->indication_token, tlv_0x12->indication_token);
        }
    }

end:
    return ret;
}

QMI_SDK_error_t telit_uim_get_file_attributes_unpack(uint8_t *rsp, uint16_t len, uim_read_file_info_t *out)
{
    return telit_uim_generic_get_file_attributes_unpack(rsp, len, out, false);
}

/*****************************************************************************
 * QMI_UIM_SET_PIN_PROTECTION (37 - 0x0025)
 */

QMI_SDK_error_t telit_uim_set_pin_protection_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_pin_protection_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_generic_session_t *tlv_0x01;
    QMI_UIM_SET_PIN_PROTECTION_request_0x02_t *tlv_0x02;
    uint32_t hdr_sz;
    uint16_t tlv_sz;
    uint8_t *pin_string;
    uint8_t *aid_string;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_SET_PIN_PROTECTION;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->session_information.aid_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_generic_session_t *)buf_position;
    COPY_HTOM(tlv_0x01->session_type, input->session_information.session_type);
    COPY_HTOM(tlv_0x01->aid_len, input->session_information.aid_len);
    aid_string = (uint8_t *)(&(tlv_0x01->aid_len));
    /* Moving to the start of the string */
    aid_string += sizeof(tlv_0x01->aid_len);
    memcpy(aid_string, input->session_information.aid, input->session_information.aid_len);

    buf_position += tlv_sz;

    tlv_sz = sizeof(*tlv_0x02) + input->pin_info.pin_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_UIM_SET_PIN_PROTECTION_request_0x02_t *)buf_position;
    COPY_HTOM(tlv_0x02->pin_id, input->pin_info.pin_id);
    COPY_HTOM(tlv_0x02->pin_operation, input->pin_info.pin_operation);
    COPY_HTOM(tlv_0x02->pin_len, input->pin_info.pin_len);
    pin_string = (uint8_t *)(&(tlv_0x02->pin_len));
    /* Moving to the start of the string */
    pin_string += sizeof(tlv_0x02->pin_len);
    memcpy(pin_string, input->pin_info.pin, input->pin_info.pin_len);

    buf_position += tlv_sz;

    if (input->key_reference_available) {
        QMI_UIM_PIN_generic_key_ref_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_UIM_PIN_generic_key_ref_t *)buf_position;
        COPY_HTOM(tlv_0x10->pin_key_ref_id, input->pin_key_ref_id);

        buf_position += tlv_sz;
    }

    if (input->response_as_indication_available) {
        QMI_UIM_generic_ind_token_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_UIM_generic_ind_token_t *)buf_position;
        COPY_HTOM(tlv_0x11->indication_token, input->indication_token);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

static QMI_SDK_error_t telit_uim_generic_pin_unpack(uint8_t *rsp, uint16_t len, uim_pin_resp_t *out,
        bool indication)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    if (indication) {
        QMI_UIM_generic_ind_token_t *tlv_0x01;

        RLOGD("%s: parsing indication", __FUNCTION__);
        payload_len = len;
        msg = rsp;
        ret = QMI_SDK_ERR_NONE;

        FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_ind_token_t);

        hdr++;
        tlv_0x01 = (QMI_UIM_generic_ind_token_t *)hdr;
        out->response_as_indication_available = true;
        COPY_MTOH(out->indication_token, tlv_0x01->indication_token);
    } else {
        RLOGD("%s: parsing response", __FUNCTION__);

        ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
        /* Not exiting for protocol errors, since we want to collect the retries */
        if (ret != QMI_SDK_ERR_NONE && ret < QMI_SDK_ERR_ERR_QMI_OFFSET) {
            RLOGE("%s: validate unpack failed", __FUNCTION__);
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_UIM_PIN_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_PIN_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_UIM_PIN_response_0x10_t *)hdr;
        out->pin_retries_available = true;
        COPY_MTOH(out->retries_remaining.verify_left, tlv_0x10->verify_left);
        COPY_MTOH(out->retries_remaining.unblock_left, tlv_0x10->unblock_left);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_UIM_PIN_response_0x11_t *tlv_0x11;
        uint8_t *enc_pin;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_PIN_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_UIM_PIN_response_0x11_t *)hdr;
        out->encrypted_pin1_available = true;
        COPY_MTOH(out->encrypted_pin1.enc_pin1_len, tlv_0x11->enc_pin1_len);
        enc_pin = (uint8_t *)(&(tlv_0x11->enc_pin1_len));
        /* Moving to the start of the string */
        enc_pin += sizeof(tlv_0x11->enc_pin1_len);
        memcpy(out->encrypted_pin1.enc_pin1, enc_pin, out->encrypted_pin1.enc_pin1_len);
    }

    /* TLV 0x12 in indication is sim codes response, while in response is
     * response as indication token
     */
    if (!indication) {
        hdr = search_TLV(msg, 0x12, payload_len);
        /* TLV 0x12 is optional */
        if (hdr) {
            QMI_UIM_generic_ind_token_t *tlv_0x12;

            CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_ind_token_t);

            hdr++;
            tlv_0x12 = (QMI_UIM_generic_ind_token_t *)hdr;
            out->response_as_indication_available = true;
            COPY_MTOH(out->indication_token, tlv_0x12->indication_token);
        }
    }

end:
    return ret;
}

QMI_SDK_error_t telit_uim_set_pin_protection_unpack(uint8_t *rsp, uint16_t len, uim_pin_resp_t *out)
{
    return telit_uim_generic_pin_unpack(rsp, len, out, false);
}

/*****************************************************************************
 * QMI_UIM_VERIFY_PIN (38 - 0x0026)
 */

QMI_SDK_error_t telit_uim_verify_pin_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_verify_pin_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_generic_session_t *tlv_0x01;
    QMI_UIM_VERIFY_PIN_request_0x02_t *tlv_0x02;
    uint32_t hdr_sz;
    uint16_t tlv_sz;
    uint8_t *pin_string;
    uint8_t *aid_string;
    uint8_t *enc_pin;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_VERIFY_PIN;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->session_information.aid_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_generic_session_t *)buf_position;
    COPY_HTOM(tlv_0x01->session_type, input->session_information.session_type);
    COPY_HTOM(tlv_0x01->aid_len, input->session_information.aid_len);
    aid_string = (uint8_t *)(&(tlv_0x01->aid_len));
    /* Moving to the start of the string */
    aid_string += sizeof(tlv_0x01->aid_len);
    memcpy(aid_string, input->session_information.aid, input->session_information.aid_len);

    buf_position += tlv_sz;

    tlv_sz = sizeof(*tlv_0x02) + input->pin_info.pin_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_UIM_VERIFY_PIN_request_0x02_t *)buf_position;
    COPY_HTOM(tlv_0x02->pin_id, input->pin_info.pin_id);
    COPY_HTOM(tlv_0x02->pin_len, input->pin_info.pin_len);
    pin_string = (uint8_t *)(&(tlv_0x02->pin_len));
    /* Moving to the start of the string */
    pin_string += sizeof(tlv_0x02->pin_len);
    memcpy(pin_string, input->pin_info.pin, input->pin_info.pin_len);

    buf_position += tlv_sz;

    if (input->encrypted_pin1_available) {
        QMI_UIM_VERIFY_PIN_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10) + input->encrypted_pin1.enc_pin1_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_UIM_VERIFY_PIN_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->enc_pin1_len, input->encrypted_pin1.enc_pin1_len);
        enc_pin = (uint8_t *)(&(tlv_0x10->enc_pin1_len));
        /* Moving to the start of the string */
        enc_pin += sizeof(tlv_0x10->enc_pin1_len);
        memcpy(enc_pin, input->encrypted_pin1.enc_pin1, input->encrypted_pin1.enc_pin1_len);

        buf_position += tlv_sz;
    }

    if (input->key_reference_available) {
        QMI_UIM_PIN_generic_key_ref_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_UIM_PIN_generic_key_ref_t *)buf_position;
        COPY_HTOM(tlv_0x11->pin_key_ref_id, input->pin_key_ref_id);

        buf_position += tlv_sz;
    }

    if (input->response_as_indication_available) {
        QMI_UIM_generic_ind_token_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_UIM_generic_ind_token_t *)buf_position;
        COPY_HTOM(tlv_0x12->indication_token, input->indication_token);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_uim_verify_pin_unpack(uint8_t *rsp, uint16_t len, uim_pin_resp_t *out)
{
    return telit_uim_generic_pin_unpack(rsp, len, out, false);
}

/*****************************************************************************
 * QMI_UIM_UNBLOCK_PIN (39 - 0x0027)
 */

QMI_SDK_error_t telit_uim_unblock_pin_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_unblock_pin_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_generic_session_t *tlv_0x01;
    QMI_UIM_UNBLOCK_PIN_request_0x02_t *tlv_0x02;
    uint32_t hdr_sz;
    uint16_t tlv_sz;
    uint8_t *pin_string;
    uint8_t *aid_string;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_UNBLOCK_PIN;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->session_information.aid_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_generic_session_t *)buf_position;
    COPY_HTOM(tlv_0x01->session_type, input->session_information.session_type);
    COPY_HTOM(tlv_0x01->aid_len, input->session_information.aid_len);
    aid_string = (uint8_t *)(&(tlv_0x01->aid_len));
    /* Moving to the start of the string */
    aid_string += sizeof(tlv_0x01->aid_len);
    memcpy(aid_string, input->session_information.aid, input->session_information.aid_len);

    buf_position += tlv_sz;

    tlv_sz = sizeof(*tlv_0x02) + input->pin_info.puk_len + 1 + input->pin_info.new_pin_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_UIM_UNBLOCK_PIN_request_0x02_t *)buf_position;
    COPY_HTOM(tlv_0x02->pin_id, input->pin_info.pin_id);
    COPY_HTOM(tlv_0x02->puk_len, input->pin_info.puk_len);
    pin_string = (uint8_t *)(&(tlv_0x02->puk_len));
    /* Moving to the start of the string */
    pin_string += sizeof(tlv_0x02->puk_len);
    memcpy(pin_string, input->pin_info.puk, input->pin_info.puk_len);
    pin_string += input->pin_info.puk_len;
    /* Setting new_pin_len */
    ec_htom(pin_string, sizeof(*pin_string),
            &(input->pin_info.new_pin_len), sizeof(input->pin_info.new_pin_len));
    /* Moving to the start of the string */
    pin_string += sizeof(input->pin_info.new_pin_len);
    memcpy(pin_string, input->pin_info.new_pin, input->pin_info.new_pin_len);

    buf_position += tlv_sz;

    if (input->key_reference_available) {
        QMI_UIM_PIN_generic_key_ref_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_UIM_PIN_generic_key_ref_t *)buf_position;
        COPY_HTOM(tlv_0x10->pin_key_ref_id, input->pin_key_ref_id);

        buf_position += tlv_sz;
    }

    if (input->response_as_indication_available) {
        QMI_UIM_generic_ind_token_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_UIM_generic_ind_token_t *)buf_position;
        COPY_HTOM(tlv_0x11->indication_token, input->indication_token);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_uim_unblock_pin_unpack(uint8_t *rsp, uint16_t len, uim_pin_resp_t *out)
{
    return telit_uim_generic_pin_unpack(rsp, len, out, false);
}

/*****************************************************************************
 * QMI_UIM_CHANGE_PIN (40 - 0x0028)
 */

QMI_SDK_error_t telit_uim_change_pin_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_change_pin_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_generic_session_t *tlv_0x01;
    QMI_UIM_CHANGE_PIN_request_0x02_t *tlv_0x02;
    uint32_t hdr_sz;
    uint16_t tlv_sz;
    uint8_t *pin_string;
    uint8_t *aid_string;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_CHANGE_PIN;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->session_information.aid_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_generic_session_t *)buf_position;
    COPY_HTOM(tlv_0x01->session_type, input->session_information.session_type);
    COPY_HTOM(tlv_0x01->aid_len, input->session_information.aid_len);
    aid_string = (uint8_t *)(&(tlv_0x01->aid_len));
    /* Moving to the start of the string */
    aid_string += sizeof(tlv_0x01->aid_len);
    memcpy(aid_string, input->session_information.aid, input->session_information.aid_len);

    buf_position += tlv_sz;

    tlv_sz = sizeof(*tlv_0x02) + input->pin_info.old_pin_len + 1 + input->pin_info.new_pin_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_UIM_CHANGE_PIN_request_0x02_t *)buf_position;
    COPY_HTOM(tlv_0x02->pin_id, input->pin_info.pin_id);
    COPY_HTOM(tlv_0x02->old_pin_len, input->pin_info.old_pin_len);
    pin_string = (uint8_t *)(&(tlv_0x02->old_pin_len));
    /* Moving to the start of the string */
    pin_string += sizeof(tlv_0x02->old_pin_len);
    memcpy(pin_string, input->pin_info.old_pin, input->pin_info.old_pin_len);
    pin_string += input->pin_info.old_pin_len;
    /* Setting new_pin_len */
    ec_htom(pin_string, sizeof(*pin_string),
            &(input->pin_info.new_pin_len), sizeof(input->pin_info.new_pin_len));
    /* Moving to the start of the string */
    pin_string += sizeof(input->pin_info.new_pin_len);
    memcpy(pin_string, input->pin_info.new_pin, input->pin_info.new_pin_len);

    buf_position += tlv_sz;

    if (input->key_reference_available) {
        QMI_UIM_PIN_generic_key_ref_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_UIM_PIN_generic_key_ref_t *)buf_position;
        COPY_HTOM(tlv_0x10->pin_key_ref_id, input->pin_key_ref_id);

        buf_position += tlv_sz;
    }

    if (input->response_as_indication_available) {
        QMI_UIM_generic_ind_token_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_UIM_generic_ind_token_t *)buf_position;
        COPY_HTOM(tlv_0x11->indication_token, input->indication_token);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_uim_change_pin_unpack(uint8_t *rsp, uint16_t len, uim_pin_resp_t *out)
{
    return telit_uim_generic_pin_unpack(rsp, len, out, false);
}

/*****************************************************************************
 * QMI_UIM_EVENT_REG (46 - 0x002E)
 */

QMI_SDK_error_t telit_uim_event_reg_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        uim_event_reg_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_EVENT_REG_generic_mask_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_EVENT_REG;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_EVENT_REG_generic_mask_t *)buf_position;
    COPY_HTOM(tlv_0x01->bitmask, input->bitmask);

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_uim_event_reg_unpack(uint8_t *rsp, uint16_t len, uim_event_reg_info_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_UIM_EVENT_REG_generic_mask_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_EVENT_REG_generic_mask_t);

        hdr++;
        tlv_0x10 = (QMI_UIM_EVENT_REG_generic_mask_t *)hdr;
        out->bitmask_available = true;
        COPY_MTOH(out->bitmask, tlv_0x10->bitmask);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_UIM_GET_CARD_STATUS (47 - 0x002F)
 */

QMI_SDK_error_t telit_uim_get_card_status_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_UIM_GET_CARD_STATUS,
            QMI_SVC_UIM,
            QMI_TIMEOUT_SMALL);
}

static QMI_SDK_error_t telit_uim_generic_get_card_status_unpack(uint8_t *rsp, uint16_t len,
        uim_card_status_info_t *out,
        bool indication)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    uint8_t *aid_tmp;
    uint8_t *buf_position;
    uint8_t i, j;

    memset(out, 0, sizeof(*out));

    if (indication) {
        RLOGD("%s: parsing indication", __FUNCTION__);

        payload_len = len;
        msg = rsp;
        ret = QMI_SDK_ERR_NONE;
    } else {
        RLOGD("%s: parsing response", __FUNCTION__);

        ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
        if (ret != QMI_SDK_ERR_NONE) {
            RLOGE("%s: validate unpack failed", __FUNCTION__);
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_UIM_GET_CARD_STATUS_request_0x10_t *tlv_0x10;
        QMI_UIM_GET_CARD_STATUS_request_slot_instance_t *slot_tmp;
        QMI_UIM_GET_CARD_STATUS_request_app_instance_t *app_tmp;
        QMI_UIM_GET_CARD_STATUS_request_app_instance_sub1_t *app_sub1_tmp;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_GET_CARD_STATUS_request_0x10_t);

        out->card_status_available = true;

        hdr++;
        tlv_0x10 = (QMI_UIM_GET_CARD_STATUS_request_0x10_t *)hdr;
        COPY_MTOH(out->card_status.index_gw_pri, tlv_0x10->index_gw_pri);
        COPY_MTOH(out->card_status.index_1x_pri, tlv_0x10->index_1x_pri);
        COPY_MTOH(out->card_status.index_gw_sec, tlv_0x10->index_gw_sec);
        COPY_MTOH(out->card_status.index_1x_sec, tlv_0x10->index_1x_sec);
        COPY_MTOH(out->card_status.num_slot, tlv_0x10->num_slot);

        if (out->card_status.num_slot > QMI_UIM_MAX_SIZE_SLOT) {
            RLOGE("%s: num_slot %u > QMI_UIM_MAX_SIZE_SLOT, increase the define",
                    __FUNCTION__, tlv_0x10->num_slot);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        buf_position = (uint8_t *)(&(tlv_0x10->num_slot));
        /* Moving to the start of the array */
        buf_position += sizeof(tlv_0x10->num_slot);
        if (out->card_status.num_slot > 0) {
            slot_tmp = (QMI_UIM_GET_CARD_STATUS_request_slot_instance_t *)buf_position;

            i = 0;
            while (slot_tmp != NULL) {
                COPY_MTOH(out->card_status.slot[i].card_state, slot_tmp->card_state);
                COPY_MTOH(out->card_status.slot[i].upin_state, slot_tmp->upin_state);
                COPY_MTOH(out->card_status.slot[i].upin_retries, slot_tmp->upin_retries);
                COPY_MTOH(out->card_status.slot[i].upuk_retries, slot_tmp->upuk_retries);
                COPY_MTOH(out->card_status.slot[i].error, slot_tmp->error);
                COPY_MTOH(out->card_status.slot[i].num_app, slot_tmp->num_app);

                if (out->card_status.slot[i].num_app > QMI_UIM_MAX_SIZE_APP) {
                    RLOGE("%s: num_app %u > QMI_UIM_MAX_SIZE_APP, increase the define",
                            __FUNCTION__, slot_tmp->num_app);
                    ret = QMI_SDK_ERR_INTERNAL;
                    goto end;
                }

                buf_position = (uint8_t *)(&(slot_tmp->num_app));
                /* Moving to the start of the array */
                buf_position += sizeof(slot_tmp->num_app);

                if (out->card_status.slot[i].num_app > 0) {
                    app_tmp = (QMI_UIM_GET_CARD_STATUS_request_app_instance_t *)buf_position;

                    j = 0;
                    while (app_tmp != NULL) {
                        COPY_MTOH(out->card_status.slot[i].app[j].type, app_tmp->type);
                        COPY_MTOH(out->card_status.slot[i].app[j].state, app_tmp->state);
                        COPY_MTOH(out->card_status.slot[i].app[j].perso_state, app_tmp->perso_state);
                        COPY_MTOH(out->card_status.slot[i].app[j].perso_feature, app_tmp->perso_feature);
                        COPY_MTOH(out->card_status.slot[i].app[j].perso_retries, app_tmp->perso_retries);
                        COPY_MTOH(out->card_status.slot[i].app[j].perso_unblock_retries,
                                app_tmp->perso_unblock_retries);
                        COPY_MTOH(out->card_status.slot[i].app[j].aid_len, app_tmp->aid_len);

                        if (out->card_status.slot[i].app[j].aid_len > QMI_UIM_MAX_AID_LEN) {
                            RLOGE("%s: aid_len exceeds expected maximum value (%d > %d)",
                                    __FUNCTION__, out->card_status.slot[i].app[j].aid_len,
                                    QMI_UIM_MAX_AID_LEN);
                            ret = QMI_SDK_ERR_INTERNAL;
                            goto end;
                        }

                        buf_position = (uint8_t *)(&(app_tmp->aid_len));
                        /* Moving to the start of the array */
                        buf_position += sizeof(app_tmp->aid_len);
                        aid_tmp = (uint8_t *)buf_position;
                        memcpy(out->card_status.slot[i].app[j].aid, aid_tmp,
                                out->card_status.slot[i].app[j].aid_len);
                        buf_position += out->card_status.slot[i].app[j].aid_len;
                        app_sub1_tmp = (QMI_UIM_GET_CARD_STATUS_request_app_instance_sub1_t *)buf_position;
                        COPY_MTOH(out->card_status.slot[i].app[j].univ_pin, app_sub1_tmp->univ_pin);
                        COPY_MTOH(out->card_status.slot[i].app[j].pin1_state, app_sub1_tmp->pin1_state);
                        COPY_MTOH(out->card_status.slot[i].app[j].pin1_retries, app_sub1_tmp->pin1_retries);
                        COPY_MTOH(out->card_status.slot[i].app[j].puk1_retries, app_sub1_tmp->puk1_retries);
                        COPY_MTOH(out->card_status.slot[i].app[j].pin2_state, app_sub1_tmp->pin2_state);
                        COPY_MTOH(out->card_status.slot[i].app[j].pin2_retries, app_sub1_tmp->pin2_retries);
                        COPY_MTOH(out->card_status.slot[i].app[j].puk2_retries, app_sub1_tmp->puk2_retries);

                        buf_position += sizeof(*app_sub1_tmp);
                        j++;
                        if ((j == out->card_status.slot[i].num_app) || (j == QMI_UIM_MAX_SIZE_APP)) {
                            app_tmp = NULL;
                        } else {
                            app_tmp = (QMI_UIM_GET_CARD_STATUS_request_app_instance_t *)buf_position;
                        }
                    }
                }
                i++;
                if ((i == out->card_status.num_slot) || (i == QMI_UIM_MAX_SIZE_SLOT)) {
                    slot_tmp = NULL;
                } else {
                    slot_tmp = (QMI_UIM_GET_CARD_STATUS_request_slot_instance_t *)buf_position;
                }
            }
        }
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        if (indication) {
            QMI_UIM_STATUS_CHANGE_IND_indication_0x11_t *tlv_0x11;
            uint8_t *valid_card_tmp;

            CHECK_UNPACK_TLV_SIZE(QMI_UIM_STATUS_CHANGE_IND_indication_0x11_t);

            hdr++;
            tlv_0x11 = (QMI_UIM_STATUS_CHANGE_IND_indication_0x11_t *)hdr;
            COPY_MTOH(out->valid_card_status_len, tlv_0x11->card_status_validity_len);
            valid_card_tmp = (uint8_t *)(&(tlv_0x11->card_status_validity_len));
            /* Moving to the start of the string */
            valid_card_tmp += sizeof(tlv_0x11->card_status_validity_len);
            memcpy(out->valid_card_status, valid_card_tmp, out->valid_card_status_len);
        } else {
            QMI_UIM_GET_CARD_STATUS_request_0x11_t *tlv_0x11;
            uint8_t *hot_swap_tmp;

            CHECK_UNPACK_TLV_SIZE(QMI_UIM_GET_CARD_STATUS_request_0x11_t);

            out->hot_swap_available = true;

            hdr++;
            tlv_0x11 = (QMI_UIM_GET_CARD_STATUS_request_0x11_t *)hdr;
            COPY_MTOH(out->hot_swap_status.hot_swap_len, tlv_0x11->hot_swap_len);
            if (out->hot_swap_status.hot_swap_len > QMI_UIM_MAX_SIZE_HOT_SWAP) {
                RLOGW("%s: hot_swap_len %u > QMI_UIM_MAX_SIZE_HOT_SWAP, increase the define",
                        __FUNCTION__, out->hot_swap_status.hot_swap_len);
                ret = QMI_SDK_ERR_INTERNAL;
                goto end;
            }
            hot_swap_tmp = (uint8_t *)(&(tlv_0x11->hot_swap_len));
            /* Moving to the start of the string */
            hot_swap_tmp += sizeof(tlv_0x11->hot_swap_len);
            memcpy(out->hot_swap_status.hot_swap, hot_swap_tmp, out->hot_swap_status.hot_swap_len);
        }
    }

end:
    return ret;
}

QMI_SDK_error_t telit_uim_get_card_status_unpack(uint8_t *rsp, uint16_t len, uim_card_status_info_t *out)
{
    return telit_uim_generic_get_card_status_unpack(rsp, len, out, false);
}

/*****************************************************************************
 * QMI_UIM_POWER_DOWN (48 - 0x0030)
 */

QMI_SDK_error_t telit_uim_power_down_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_power_slot_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_generic_slot_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_POWER_DOWN;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_MEDIUM;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_generic_slot_t *)buf_position;
    COPY_HTOM(tlv_0x01->slot, input->slot);

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_uim_power_down_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_UIM_POWER_UP (49 - 0x0031)
 */

QMI_SDK_error_t telit_uim_power_up_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_power_slot_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_generic_slot_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_POWER_UP;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_MEDIUM;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_generic_slot_t *)buf_position;
    COPY_HTOM(tlv_0x01->slot, input->slot);

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_uim_power_up_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_UIM_CHANGE_PROVISIONING_SESSION (56 - 0x0038)
 */

QMI_SDK_error_t telit_uim_change_provisioning_session_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_change_provisioning_session_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_CHANGE_PROVISIONING_SESSION_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_CHANGE_PROVISIONING_SESSION;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_MEDIUM;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_CHANGE_PROVISIONING_SESSION_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->session_type, input->session_type);
    COPY_HTOM(tlv_0x01->activate, input->activate);

    buf_position += tlv_sz;

    if (input->application_info_available) {
        QMI_UIM_CHANGE_PROVISIONING_SESSION_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10) + input->application_info.aid_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_UIM_CHANGE_PROVISIONING_SESSION_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->slot, input->application_info.slot);
        COPY_HTOM(tlv_0x10->aid_len, input->application_info.aid_len);
        buf_position = &tlv_0x10->aid_len;
        buf_position += sizeof(tlv_0x10->aid_len);
        memcpy(buf_position, input->application_info.aid, input->application_info.aid_len);
    }

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_uim_change_provisioning_session_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_UIM_SEND_APDU (59 - 0x003B)
 */

QMI_SDK_error_t telit_uim_send_apdu_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_send_apdu_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_generic_slot_t *tlv_0x01;
    QMI_UIM_generic_apdu_t *tlv_0x02;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_SEND_APDU;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_MEDIUM;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_generic_slot_t *)buf_position;
    COPY_HTOM(tlv_0x01->slot, input->slot);

    buf_position += tlv_sz;

    tlv_sz = sizeof(*tlv_0x02) + input->apdu_sz;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_UIM_generic_apdu_t *)buf_position;
    COPY_HTOM(tlv_0x02->len, input->apdu_sz);
    buf_position += sizeof(tlv_0x02->len);
    memcpy(buf_position, input->apdu, input->apdu_sz);

    buf_position += input->apdu_sz;

    if (input->channel_id_available) {
        QMI_UIM_generic_channel_id_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_UIM_generic_channel_id_t *)buf_position;
        COPY_HTOM(tlv_0x10->channel_id, input->channel_id);
        buf_position += tlv_sz;
    }

    if (input->procedure_bytes_setting_available) {
        QMI_UIM_SEND_APDU_request_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_UIM_SEND_APDU_request_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->procedure_bytes, input->procedure_bytes_setting);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_uim_send_apdu_unpack(uint8_t *rsp, uint16_t len,
        uim_send_apdu_response_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_UIM_generic_apdu_t *tlv_0x10;
        uint8_t *buf_position;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_apdu_t);

        hdr++;
        tlv_0x10 = (QMI_UIM_generic_apdu_t *)hdr;
        COPY_MTOH(out->apdu_sz, tlv_0x10->len);
        if (out->apdu_sz > QMI_UIM_MAX_APDU_SZ) {
            RLOGE(
                    "%s: apdu_sz = %u > QMI_UIM_MAX_APDU_SZ, increase the define",
                    __FUNCTION__, out->apdu_sz);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = (uint8_t *)&tlv_0x10->len;
        buf_position += sizeof(tlv_0x10->len);
        memcpy(out->apdu, buf_position, out->apdu_sz);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_UIM_SEND_APDU_response_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_SEND_APDU_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_UIM_SEND_APDU_response_0x11_t *)hdr;
        out->long_apdu_response_info_available = true;
        COPY_MTOH(out->long_apdu_response_info.total_length, tlv_0x11->total_length);
        COPY_MTOH(out->long_apdu_response_info.token, tlv_0x11->token);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_UIM_LOGICAL_CHANNEL (63 - 0x003F)
 */

QMI_SDK_error_t telit_uim_logical_channel_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_logical_channel_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_generic_slot_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_LOGICAL_CHANNEL;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_MEDIUM;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_generic_slot_t *)buf_position;
    COPY_HTOM(tlv_0x01->slot, input->slot);

    buf_position += tlv_sz;

    if (input->aid_len) {
        QMI_UIM_generic_aid_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10) + input->aid_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_UIM_generic_aid_t *)buf_position;
        COPY_HTOM(tlv_0x10->aid_len, input->aid_len);
        buf_position += sizeof(tlv_0x10->aid_len);
        memcpy(buf_position, input->aid, input->aid_len);

        buf_position += input->aid_len;
    }

    if (input->channel_id_to_close_available) {
        QMI_UIM_generic_channel_id_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_UIM_generic_channel_id_t *)buf_position;
        COPY_HTOM(tlv_0x11->channel_id, input->channel_id_to_close);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_uim_logical_channel_unpack(uint8_t *rsp, uint16_t len,
        uim_logical_channel_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_UIM_generic_channel_id_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_channel_id_t);

        hdr++;
        tlv_0x10 = (QMI_UIM_generic_channel_id_t *)hdr;
        out->channel_id_available = true;
        COPY_MTOH(out->channel_id, tlv_0x10->channel_id);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_UIM_generic_card_result_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_generic_card_result_t);

        hdr++;
        tlv_0x11 = (QMI_UIM_generic_card_result_t *)hdr;
        out->card_result_available = true;
        COPY_MTOH(out->card_result.sw1, tlv_0x11->sw1);
        COPY_MTOH(out->card_result.sw2, tlv_0x11->sw2);
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_UIM_LOGICAL_CHANNEL_response_0x12_t *tlv_0x12;
        uint8_t *select_response;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_LOGICAL_CHANNEL_response_0x12_t);

        hdr++;
        tlv_0x12 = (QMI_UIM_LOGICAL_CHANNEL_response_0x12_t *)hdr;
        COPY_MTOH(out->select_response_sz, tlv_0x12->len);
        select_response = (uint8_t *)(&(tlv_0x12->len));
        /* Moving to the start of the string */
        select_response += sizeof(tlv_0x12->len);
        memcpy(out->select_response, select_response, out->select_response_sz);
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_UIM_LOGICAL_CHANNEL_response_0x13_t *tlv_0x13;
        uint8_t *long_select_response;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_LOGICAL_CHANNEL_response_0x13_t);

        hdr++;
        tlv_0x13 = (QMI_UIM_LOGICAL_CHANNEL_response_0x13_t *)hdr;
        COPY_MTOH(out->long_select_response_sz, tlv_0x13->len);
        if (out->long_select_response_sz > QMI_UIM_MAX_LONG_SELECT_RESPONSE_SZ) {
            RLOGE(
                    "%s: long_select_response_sz = %u > QMI_UIM_MAX_LONG_SELECT_RESPONSE_SZ, increase the define",
                    __FUNCTION__, out->long_select_response_sz);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        long_select_response = (uint8_t *)(&(tlv_0x13->len));
        /* Moving to the start of the string */
        long_select_response += sizeof(tlv_0x13->len);
        memcpy(out->long_select_response, long_select_response, out->long_select_response_sz);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_UIM_SWITCH_SLOT (70 - 0x0046)
 */

QMI_SDK_error_t telit_uim_switch_slot_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        uim_switch_slot_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_UIM_SWITCH_SLOT_request_0x01_t *tlv_0x01;
    QMI_UIM_SWITCH_SLOT_request_0x02_t *tlv_0x02;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_UIM_SWITCH_SLOT;
    req_ctx->svc_id = QMI_SVC_UIM;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_UIM_SWITCH_SLOT_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->logical_slot, input->logical_slot);

    buf_position += tlv_sz;

    tlv_sz = sizeof(*tlv_0x02);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_UIM_SWITCH_SLOT_request_0x02_t *)buf_position;
    COPY_HTOM(tlv_0x02->physical_slot, input->physical_slot);

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_uim_switch_slot_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_UIM_GET_SLOTS_STATUS (71 - 0x0047)
 */

QMI_SDK_error_t telit_uim_get_slots_status_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_UIM_GET_SLOTS_STATUS,
            QMI_SVC_UIM,
            QMI_TIMEOUT_SMALL);
}

static QMI_SDK_error_t telit_uim_generic_get_slots_status_unpack(uint8_t *rsp, uint16_t len,
        uim_slot_status_info_t *out, bool indication)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    uint8_t *buf_position;
    uint8_t *iccid;
    uint8_t i;

    memset(out, 0, sizeof(*out));

    if (indication) {
        RLOGD("%s: parsing indication", __FUNCTION__);

        payload_len = len;
        msg = rsp;
        ret = QMI_SDK_ERR_NONE;
    } else {
        RLOGD("%s: parsing response", __FUNCTION__);

        ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
        if (ret != QMI_SDK_ERR_NONE) {
            RLOGE("%s: validate unpack failed", __FUNCTION__);
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_UIM_GET_SLOT_STATUS_response_0x10_t *tlv_0x10;
        QMI_UIM_GET_SLOT_STATUS_response_slot_instance_t *slot_tmp;

        CHECK_UNPACK_TLV_SIZE(QMI_UIM_GET_SLOT_STATUS_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_UIM_GET_SLOT_STATUS_response_0x10_t *)hdr;
        COPY_MTOH(out->slot_size, tlv_0x10->slot_size);

        if (out->slot_size > QMI_UIM_MAX_SLOTS) {
            RLOGE("%s: slot_size %u > QMI_UIM_MAX_SLOTS, increase the define",
                    __FUNCTION__, out->slot_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        buf_position = (uint8_t *)(&(tlv_0x10->slot_size));
        /* Moving to the start of the array */
        buf_position += sizeof(tlv_0x10->slot_size);

        if (out->slot_size > 0) {
            slot_tmp = (QMI_UIM_GET_SLOT_STATUS_response_slot_instance_t *)buf_position;

            i = 0;
            while (slot_tmp != NULL) {
                COPY_MTOH(out->slot[i].physical_card_status, slot_tmp->physical_card_status);
                COPY_MTOH(out->slot[i].physical_slot_state, slot_tmp->physical_slot_state);
                COPY_MTOH(out->slot[i].logical_slot, slot_tmp->logical_slot);
                COPY_MTOH(out->slot[i].iccid_len, slot_tmp->iccid_len);

                if (out->slot[i].iccid_len > QMI_UIM_MAX_ICCID_SIZE) {
                    RLOGE("iccid_len = %u > QMI_UIM_MAX_ICCID_SIZE)",
                            out->slot[i].iccid_len);
                    ret = QMI_SDK_ERR_INTERNAL;
                    goto end;
                }

                buf_position = (uint8_t *)(&(slot_tmp->iccid_len));
                /* Moving to the start of the array */
                buf_position += sizeof(slot_tmp->iccid_len);
                iccid = (uint8_t *)buf_position;
                memcpy(out->slot[i].iccid, iccid, out->slot[i].iccid_len);
                buf_position += out->slot[i].iccid_len;
                i++;
                if ((i == out->slot_size) || (i == QMI_UIM_MAX_SLOTS)) {
                    slot_tmp = NULL;
                } else {
                    slot_tmp = (QMI_UIM_GET_SLOT_STATUS_response_slot_instance_t *)buf_position;
                }
            }
        }

    }

end:
    return ret;
}

QMI_SDK_error_t telit_uim_get_slots_status_unpack(uint8_t *rsp, uint16_t len,
        uim_slot_status_info_t *out)
{
    return telit_uim_generic_get_slots_status_unpack(rsp, len, out, false);
}

/*****************************************************************************
 * QMI UIM indications
 */

QMI_SDK_error_t telit_uim_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len __unused,
        uint8_t *out, uint16_t out_len)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    uint16_t payload_len;
    uint8_t *msg;

    if (!rsp || !out) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    payload_len = get_payload_size(rsp);
    msg = rsp + sizeof(QMI_service_raw_transaction_header_t) + \
            sizeof(QMI_raw_message_header_t);

    switch (msg_id) {
        case QMI_UIM_STATUS_CHANGE_IND:
            if (out_len < sizeof(uim_card_status_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(uim_card_status_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_uim_generic_get_card_status_unpack(msg, payload_len, (uim_card_status_info_t *)out,
                    true);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_UIM_GET_SLOTS_STATUS_IND:
            if (out_len < sizeof(uim_slot_status_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(uim_read_file_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_uim_generic_get_slots_status_unpack(msg, payload_len, (uim_slot_status_info_t *)out,
                    true);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_UIM_READ_TRANSPARENT_IND:
            if (out_len < sizeof(uim_read_file_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(uim_read_file_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_uim_generic_read_transparent_unpack(msg, payload_len, (uim_read_file_info_t *)out,
                    true);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_UIM_READ_RECORD_IND:
            if (out_len < sizeof(uim_read_record_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(uim_read_record_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_uim_generic_read_record_unpack(msg, payload_len, (uim_read_record_info_t *)out, true);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_UIM_CHANGE_PIN_IND:
        case QMI_UIM_SET_PIN_PROTECTION_IND:
        case QMI_UIM_UNBLOCK_PIN_IND:
        case QMI_UIM_VERIFY_PIN_IND:
            if (out_len < sizeof(uim_pin_resp_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(uim_pin_resp_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_uim_generic_pin_unpack(msg, payload_len, (uim_pin_resp_t *)out, true);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        default:
            break;
    }

end:
    return ret;
}
