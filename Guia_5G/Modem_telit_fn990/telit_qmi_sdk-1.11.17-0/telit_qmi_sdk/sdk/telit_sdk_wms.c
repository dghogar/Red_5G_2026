#include <stdint.h>
#include <stdio.h>
#include <string.h>
#if defined __WINDOWS_QMI__
#include "portable_endian.h"
#else
#include <endian.h>
#endif

#include "telit_sdk.h"
#include "telit_sdk_private.h"
#include "telit_sdk_wms.h"
#include "telit_sdk_private_wms.h"

/*****************************************************************************
 * QMI_WMS_SET_EVENT_REPORT (1 - 0x0001)
 */

QMI_SDK_error_t telit_wms_set_event_report_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_set_event_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_WMS_SET_EVENT_REPORT;
    req_ctx->svc_id = QMI_SVC_WMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->report_mt_message_available) {
        QMI_WMS_SET_EVENT_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_WMS_SET_EVENT_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->report_mt_message, input->report_mt_message);

        buf_position += tlv_sz;
    }

    if (input->report_call_control_info_available) {
        QMI_WMS_SET_EVENT_request_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_WMS_SET_EVENT_request_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->report_call_control_info, input->report_call_control_info);

        buf_position += tlv_sz;
    }

    if (input->report_mwi_message_available) {
        QMI_WMS_SET_EVENT_request_0x12_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_WMS_SET_EVENT_request_0x12_t *)buf_position;
        COPY_HTOM(tlv_0x12->report_mwi_message, input->report_mwi_message);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wms_set_event_report_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_WMS_RAW_SEND (32 - 0x0020)
 */

QMI_SDK_error_t telit_wms_raw_send_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_raw_send_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WMS_RAW_SEND_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_WMS_RAW_SEND;
    req_ctx->svc_id = QMI_SVC_WMS;
    req_ctx->timeout = QMI_TIMEOUT_BIG;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->raw_data_size;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    /* pack Raw Message Data 0x01 TLV */
    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_WMS_RAW_SEND_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->format, input->format);
    COPY_HTOM(tlv_0x01->raw_data_size, input->raw_data_size);

    if (input->raw_data_size > QMI_WMS_MAX_RAW_DATA_LEN) {
        RLOGE("%s: raw_data_size = %u > QMI_WMS_MAX_RAW_DATA_LEN, increase the define",
                __FUNCTION__, input->raw_data_size);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    PACK_TLV_ARRAY(tlv_0x01->raw_data_size,
            input->raw_data,
            input->raw_data_size,
            QMI_WMS_MAX_RAW_DATA_LEN);

    buf_position += tlv_sz;

    /* pack CDMA Force On DC 0x10 TLV */
    if (input->force_on_dc_available) {
        QMI_WMS_RAW_SEND_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_WMS_RAW_SEND_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->force, input->force_on_dc.force);
        COPY_HTOM(tlv_0x10->service_option, input->force_on_dc.service_option);

        buf_position += tlv_sz;
    }

    /* pack CDMA Follow On DC 0x11 TLV */
    if (input->follow_available) {
        QMI_WMS_RAW_SEND_request_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_WMS_RAW_SEND_request_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->follow, input->follow);

        buf_position += tlv_sz;
    }

    /* pack GSM WCDMA Link Timer 0x12 TLV */
    if (input->gsm_wcdma_link_timer_available) {
        QMI_WMS_RAW_SEND_request_0x12_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_WMS_RAW_SEND_request_0x12_t *)buf_position;
        COPY_HTOM(tlv_0x12->gsm_wcdma_link_timer, input->gsm_wcdma_link_timer);

        buf_position += tlv_sz;
    }

    /* pack SMS on IMS 0x13 TLV */
    if (input->sms_on_ims_available) {
        QMI_WMS_RAW_SEND_request_0x13_t *tlv_0x13;

        tlv_sz = sizeof(*tlv_0x13);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x13 = (QMI_WMS_RAW_SEND_request_0x13_t *)buf_position;
        COPY_HTOM(tlv_0x13->sms_on_ims, input->sms_on_ims);

        buf_position += tlv_sz;
    }

    /* pack Retry Message 0x14 TLV */
    if (input->retry_message_available) {
        QMI_WMS_RAW_SEND_request_0x14_t *tlv_0x14;

        tlv_sz = sizeof(*tlv_0x14);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x14;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x14 = (QMI_WMS_RAW_SEND_request_0x14_t *)buf_position;
        COPY_HTOM(tlv_0x14->retry_message, input->retry_message);

        buf_position += tlv_sz;
    }

    /* pack Retry Message ID 0x15 TLV */
    if (input->retry_message_id_available) {
        QMI_WMS_RAW_SEND_request_0x15_t *tlv_0x15;

        tlv_sz = sizeof(*tlv_0x15);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x15;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x15 = (QMI_WMS_RAW_SEND_request_0x15_t *)buf_position;
        COPY_HTOM(tlv_0x15->retry_message_id, input->retry_message_id);

        buf_position += tlv_sz;
    }

    /* pack Link enable Mode 0x16 TLV */
    if (input->link_enable_mode_available) {
        QMI_WMS_RAW_SEND_request_0x16_t *tlv_0x16;

        tlv_sz = sizeof(*tlv_0x16);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x16;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x16 = (QMI_WMS_RAW_SEND_request_0x16_t *)buf_position;
        COPY_HTOM(tlv_0x16->link_enable_mode, input->link_enable_mode);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wms_raw_send_unpack(uint8_t *rsp, uint16_t len, wms_raw_send_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_WMS_RAW_SEND_response_0x01_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    /* Not exiting for protocol errors, since we want to collect the extended error */
    if (ret != QMI_SDK_ERR_NONE && ret < QMI_SDK_ERR_ERR_QMI_OFFSET) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_WMS_RAW_SEND_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_WMS_RAW_SEND_response_0x01_t *)hdr;
    COPY_MTOH(out->message_id, tlv_0x01->message_id);

    hdr = search_TLV(msg, 0x10, payload_len);
    if (hdr) {
        /* TLV 0x10 is optional */
        QMI_WMS_RAW_SEND_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_WMS_RAW_SEND_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_WMS_RAW_SEND_response_0x10_t *)hdr;
        COPY_MTOH(out->cause_code, tlv_0x10->cause_code);
        out->cause_code_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        /* TLV 0x11 is optional */
        QMI_WMS_RAW_SEND_response_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_WMS_RAW_SEND_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_WMS_RAW_SEND_response_0x11_t *)hdr;
        COPY_MTOH(out->error_class, tlv_0x11->error_class);
        out->error_class_available = true;
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    if (hdr) {
        /* TLV 0x12 is optional */
        QMI_WMS_RAW_SEND_response_0x12_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_WMS_RAW_SEND_response_0x12_t);

        hdr++;
        tlv_0x12 = (QMI_WMS_RAW_SEND_response_0x12_t *)hdr;

        COPY_MTOH(out->gw_cause_info.rp_cause, tlv_0x12->rp_cause);
        COPY_MTOH(out->gw_cause_info.tp_cause, tlv_0x12->tp_cause);
        out->gw_cause_info_available = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    if (hdr) {
        /* TLV 0x13 is optional */
        QMI_WMS_RAW_SEND_response_0x13_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_WMS_RAW_SEND_response_0x13_t);

        hdr++;
        tlv_0x13 = (QMI_WMS_RAW_SEND_response_0x13_t *)hdr;
        COPY_MTOH(out->message_delivery_failure_type, tlv_0x13->message_delivery_failure_type);
        out->message_delivery_failure_type_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_WMS_RAW_WRITE (33 - 0x0021)
 */

QMI_SDK_error_t telit_wms_raw_write_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_raw_write_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WMS_RAW_WRITE_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_WMS_RAW_WRITE;
    req_ctx->svc_id = QMI_SVC_WMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->raw_data_size;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    /* pack Raw Message Data 0x01 TLV */
    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_WMS_RAW_WRITE_request_0x01_t *)buf_position;

    COPY_HTOM(tlv_0x01->storage_type, input->storage_type);
    COPY_HTOM(tlv_0x01->format, input->format);
    COPY_HTOM(tlv_0x01->raw_data_size, input->raw_data_size);

    if (input->raw_data_size > QMI_WMS_MAX_RAW_DATA_LEN) {
        RLOGE("%s: raw_data_size = %u > QMI_WMS_MAX_RAW_DATA_LEN, increase the define",
                __FUNCTION__, input->raw_data_size);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    PACK_TLV_ARRAY(tlv_0x01->raw_data_size,
            input->raw_data,
            input->raw_data_size,
            QMI_WMS_MAX_RAW_DATA_LEN);

    buf_position += tlv_sz;

    /* pack Message Type 0x10 TLV */
    if (input->tag_type_available) {
        QMI_WMS_RAW_WRITE_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_WMS_RAW_WRITE_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->tag_type, input->tag_type);
    }
    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wms_raw_write_unpack(uint8_t *rsp, uint16_t len, wms_raw_write_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_WMS_RAW_WRITE_response_0x01_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_WMS_RAW_WRITE_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_WMS_RAW_WRITE_response_0x01_t *)hdr;
    COPY_MTOH(out->memory_index, tlv_0x01->memory_index);

end:
    return ret;
}

/*****************************************************************************
 * QMI_WMS_RAW_READ (34 - 0x0022)
 */

QMI_SDK_error_t telit_wms_raw_read_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_raw_read_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WMS_RAW_READ_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_WMS_RAW_READ;
    req_ctx->svc_id = QMI_SVC_WMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    /* pack Message Memory Storage ID 0x01 TLV */
    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_WMS_RAW_READ_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->storage_type, input->storage_type);
    COPY_HTOM(tlv_0x01->memory_index, input->memory_index);

    buf_position += tlv_sz;

    /* pack Message Mode 0x10 TLV */
    if (input->message_mode_available) {
        QMI_WMS_RAW_READ_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_WMS_RAW_READ_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->message_mode, input->message_mode);

        buf_position += tlv_sz;
    }

    /* pack SMS on IMS 0x11 TLV */
    if (input->sms_on_ims_available) {
        QMI_WMS_RAW_READ_request_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_WMS_RAW_READ_request_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->sms_on_ims, input->sms_on_ims);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wms_raw_read_unpack(uint8_t *rsp, uint16_t len, wms_raw_read_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_WMS_RAW_READ_response_0x01_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_WMS_RAW_READ_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_WMS_RAW_READ_response_0x01_t *)hdr;

    COPY_MTOH(out->message_tag, tlv_0x01->message_tag);
    COPY_MTOH(out->format, tlv_0x01->format);
    COPY_MTOH(out->raw_data_size, tlv_0x01->raw_data_size);

    if (out->raw_data_size > QMI_WMS_MAX_RAW_DATA_LEN) {
        RLOGE("%s: raw_data_size = %u > QMI_WMS_MAX_RAW_DATA_LEN, increase the define",
                __FUNCTION__, out->raw_data_size);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    UNPACK_ARRAY(tlv_0x01->raw_data_size,
            out->raw_data_size,
            out->raw_data,
            QMI_WMS_MAX_RAW_DATA_LEN);

end:
    return ret;
}

/*****************************************************************************
 * QMI_WMS_MODIFY_TAG (35 - 0x0023)
 */

QMI_SDK_error_t telit_wms_modify_tag_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_modify_tag_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WMS_MODIFY_TAG_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_WMS_MODIFY_TAG;
    req_ctx->svc_id = QMI_SVC_WMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_WMS_MODIFY_TAG_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->storage_type, input->message_tag.storage_type);
    COPY_HTOM(tlv_0x01->storage_index, input->message_tag.storage_index);
    COPY_HTOM(tlv_0x01->tag_type, input->message_tag.tag_type);

    buf_position += tlv_sz;

    if (input->message_mode_available) {
        QMI_WMS_MODIFY_TAG_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_WMS_MODIFY_TAG_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->message_mode, input->message_mode);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wms_modify_tag_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_WMS_DELETE (36 - 0x0024)
 */

QMI_SDK_error_t telit_wms_delete_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len, wms_delete_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WMS_DELETE_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_WMS_DELETE;
    req_ctx->svc_id = QMI_SVC_WMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_WMS_DELETE_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->storage_type, input->storage_type);

    buf_position += tlv_sz;

    if (input->memory_index_available) {
        QMI_WMS_DELETE_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_WMS_DELETE_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->memory_index, input->memory_index);

        buf_position += tlv_sz;
    }

    if (input->tag_type_available) {
        QMI_WMS_DELETE_request_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_WMS_DELETE_request_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->tag_type, input->tag_type);

        buf_position += tlv_sz;
    }

    if (input->message_mode_available) {
        QMI_WMS_DELETE_request_0x12_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_WMS_DELETE_request_0x12_t *)buf_position;
        COPY_HTOM(tlv_0x12->message_mode, input->message_mode);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wms_delete_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_WMS_LIST_MESSAGES (49 - 0x0031)
 */

QMI_SDK_error_t telit_wms_list_messages_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_list_messages_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WMS_LIST_MESSAGES_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_WMS_LIST_MESSAGES;
    req_ctx->svc_id = QMI_SVC_WMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_WMS_LIST_MESSAGES_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->storage_type, input->storage_type);

    buf_position += tlv_sz;

    if (input->requested_tag_available) {
        QMI_WMS_LIST_MESSAGES_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_WMS_LIST_MESSAGES_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->requested_tag, input->requested_tag);

        buf_position += tlv_sz;
    }

    if (input->message_mode_available) {
        QMI_WMS_LIST_MESSAGES_request_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_WMS_LIST_MESSAGES_request_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->message_mode, input->message_mode);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wms_list_messages_unpack(uint8_t *rsp, uint16_t len, wms_list_messages_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_WMS_LIST_MESSAGES_response_0x01_t *tlv_0x01;
    uint8_t *raw_data_ptr;
    uint32_t i;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_WMS_LIST_MESSAGES_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_WMS_LIST_MESSAGES_response_0x01_t *)hdr;
    COPY_MTOH(out->message_list_size, tlv_0x01->message_list_size);

    if (out->message_list_size > QMI_WMS_MAX_NUMBER_OF_MESSAGES) {
        RLOGE("%s: message_list_size = %u > QMI_WMS_MAX_NUMBER_OF_MESSAGES, increase the define",
                __FUNCTION__, out->message_list_size);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    /* jump to the head of the message list */
    raw_data_ptr = (uint8_t *)(&(tlv_0x01->message_list_size));
    raw_data_ptr += sizeof(tlv_0x01->message_list_size);

    for (i = 0; i < out->message_list_size; i++) {
        uint32_t index;
        uint8_t tag_type;

        memcpy(&index, raw_data_ptr, sizeof(index));
        raw_data_ptr += sizeof(index);
        tag_type = *raw_data_ptr;
        raw_data_ptr++;

        COPY_MTOH(out->message_list[i].message_index, index);
        COPY_MTOH(out->message_list[i].tag_type, tag_type);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_WMS_SET_ROUTES (50 - 0x0032)
 */

QMI_SDK_error_t telit_wms_set_routes_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_set_routes_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WMS_SET_ROUTES_request_0x01_t *tlv_0x01;
    QMI_WMS_generic_route_t *route;
    uint32_t hdr_sz;
    uint16_t tlv_sz;
    uint8_t i;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_WMS_SET_ROUTES;
    req_ctx->svc_id = QMI_SVC_WMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->routes_size * sizeof(*route);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = tlv_sz;

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_WMS_SET_ROUTES_request_0x01_t *)buf_position;

    if (input->routes_size > QMI_WMS_MAX_ROUTES_LEN) {
        RLOGE("%s: routes_size = %u > QMI_WMS_MAX_ROUTES_LEN, increase the define",
                __FUNCTION__, input->routes_size);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    COPY_HTOM(tlv_0x01->routes_size, input->routes_size);
    /* Skipping routes_size */
    buf_position = (uint8_t *)&tlv_0x01->routes_size;
    buf_position += sizeof(tlv_0x01->routes_size);
    route = (QMI_WMS_generic_route_t *)buf_position;
    for (i = 0; i < input->routes_size; i++) {
        COPY_HTOM(route[i].message_type, input->routes[i].message_type);
        COPY_HTOM(route[i].message_class, input->routes[i].message_class);
        COPY_HTOM(route[i].route_storage, input->routes[i].route_storage);
        COPY_HTOM(route[i].receipt_action, input->routes[i].receipt_action);
    }

    buf_position += (input->routes_size * sizeof(*route));

    if (input->transfer_ind_available) {
        QMI_WMS_SET_ROUTES_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_WMS_SET_ROUTES_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->transfer_ind, input->transfer_ind);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wms_set_routes_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_WMS_GET_SMSC_ADDR (52 - 0x0034)
 */

QMI_SDK_error_t telit_wms_get_smsc_address_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_WMS_GET_SMSC_ADDR,
            QMI_SVC_WMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_wms_get_smsc_address_unpack(uint8_t *rsp, uint16_t len, wms_smsc_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_WMS_GET_SMSC_ADDRESS_response_0x01_t *tlv_0x01;
    uint8_t *raw_data_ptr;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_WMS_GET_SMSC_ADDRESS_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_WMS_GET_SMSC_ADDRESS_response_0x01_t *)hdr;

    memcpy(out->smsc_address_type, tlv_0x01->smsc_address_type, 3);
    out->smsc_address_type_len = 3;
    COPY_MTOH(out->smsc_address_len, tlv_0x01->smsc_address_length);

    if (out->smsc_address_len > QMI_WMS_MAX_SMSC_ADDRESS_LEN) {
        RLOGE("%s:smsc_address_len = %u > QMI_WMS_MAX_SMSC_ADDRESS_LEN, increase the define",
                __FUNCTION__, out->smsc_address_len);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    /* jump to the head of smsc address */
    raw_data_ptr = (uint8_t *)(&(tlv_0x01->smsc_address_length));
    raw_data_ptr += sizeof(tlv_0x01->smsc_address_length);
    memcpy(out->smsc_address, raw_data_ptr, out->smsc_address_len);

end:
    return ret;
}

/*****************************************************************************
 * QMI_WMS_SET_SMSC_ADDR (53 - 0x0035)
 */

QMI_SDK_error_t telit_wms_set_smsc_address_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_smsc_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    if (input->smsc_address_len == 0) {
        goto end;
    }

    req_ctx->msg_id = QMI_WMS_SET_SMSC_ADDR;
    req_ctx->svc_id = QMI_SVC_WMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    /* Modem wants a NULL-terminated string */
    tlv_sz = input->smsc_address_len + 1;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    memcpy(buf_position, input->smsc_address, input->smsc_address_len);
    buf_position[input->smsc_address_len] = '\0';

    buf_position += tlv_sz;

    if (input->smsc_address_type_len) {
        tlv_sz = input->smsc_address_type_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        memcpy(buf_position, input->smsc_address_type, input->smsc_address_type_len);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wms_set_smsc_address_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_WMS_SEND_ACK (55 - 0x0037)
 */

QMI_SDK_error_t telit_wms_send_ack_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        wms_send_ack_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WMS_SEND_ACK_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_WMS_SEND_ACK;
    req_ctx->svc_id = QMI_SVC_WMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_WMS_SEND_ACK_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->transaction_id, input->transaction_id);
    COPY_HTOM(tlv_0x01->message_protocol, input->message_protocol);
    COPY_HTOM(tlv_0x01->success, input->success);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wms_send_ack_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI WMS indications
 */

static QMI_SDK_error_t telit_wms_handle_event_ind(uint16_t *payload_len, uint8_t *msg,
        uint8_t *out, uint16_t out_len)
{
    wms_ind_event_report_t *ev_rep = (wms_ind_event_report_t *) out;
    QMI_SDK_error_t ret = QMI_SDK_ERR_BUFFER_SZ;
    QMI_raw_content_header_t *hdr = NULL;

    if (out_len < sizeof(*ev_rep)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        goto end;
    }
    memset(ev_rep, 0, sizeof(*ev_rep));

    hdr = search_TLV(msg, 0x10, *payload_len);
    if (hdr) {
        if (sizeof(QMI_WMS_EVENT_IND_response_0x10_t) <= le16toh(hdr->length)) {
            QMI_WMS_EVENT_IND_response_0x10_t *tlv_0x10;
            hdr++;

            tlv_0x10 = (QMI_WMS_EVENT_IND_response_0x10_t *)hdr;
            ev_rep->mt_message_set = true;
            COPY_MTOH(ev_rep->mt_message.storage_type, tlv_0x10->storage_type);
            COPY_MTOH(ev_rep->mt_message.storage_index, tlv_0x10->storage_index);
        } else {
            RLOGE("%s: malformed TLV, returned %u, expected at least %zu",
                    __FUNCTION__, le16toh(hdr->length),
                    sizeof(QMI_WMS_EVENT_IND_response_0x10_t));
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x11, *payload_len);
    if (hdr) {
        if (sizeof(QMI_WMS_EVENT_IND_response_0x11_t) <= le16toh(hdr->length)) {
            QMI_WMS_EVENT_IND_response_0x11_t *tlv_0x11;
            hdr++;

            tlv_0x11 = (QMI_WMS_EVENT_IND_response_0x11_t *)hdr;
            ev_rep->transfer_route_mt_message_set = true;
            COPY_MTOH(ev_rep->transfer_route_mt_message.ack_indicator, tlv_0x11->ack_indicator);
            COPY_MTOH(ev_rep->transfer_route_mt_message.transaction_id, tlv_0x11->transaction_id);
            COPY_MTOH(ev_rep->transfer_route_mt_message.format, tlv_0x11->format);
            COPY_MTOH(ev_rep->transfer_route_mt_message.size, tlv_0x11->size);

            if (ev_rep->transfer_route_mt_message.size > QMI_WMS_MAX_RAW_DATA_LEN) {
                RLOGE(
                        "%s: transfer_route_mt_message.size = %u > QMI_WMS_MAX_RAW_DATA_LEN, increase the define",
                        __FUNCTION__, ev_rep->transfer_route_mt_message.size);
                ret = QMI_SDK_ERR_INTERNAL;
                goto end;
            }

            UNPACK_ARRAY(tlv_0x11->size,
                    ev_rep->transfer_route_mt_message.size,
                    ev_rep->transfer_route_mt_message.data,
                    QMI_WMS_MAX_RAW_DATA_LEN);
        } else {
            RLOGE("%s: malformed TLV, returned %u, expected at least %zu",
                    __FUNCTION__, le16toh(hdr->length),
                    sizeof(QMI_WMS_EVENT_IND_response_0x11_t));
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x12, *payload_len);
    if (hdr) {
        if (sizeof(QMI_WMS_EVENT_IND_response_0x12_t) <= le16toh(hdr->length)) {
            QMI_WMS_EVENT_IND_response_0x12_t *tlv_0x12;
            hdr++;

            tlv_0x12 = (QMI_WMS_EVENT_IND_response_0x12_t *)hdr;
            ev_rep->message_mode_set = true;
            COPY_MTOH(ev_rep->message_mode, tlv_0x12->message_mode);
        } else {
            RLOGE("%s: malformed TLV, returned %u, expected at least %zu",
                    __FUNCTION__, le16toh(hdr->length),
                    sizeof(QMI_WMS_EVENT_IND_response_0x12_t));
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x13, *payload_len);
    if (hdr) {
        if (sizeof(QMI_WMS_EVENT_IND_response_0x13_t) <= le16toh(hdr->length)) {
            QMI_WMS_EVENT_IND_response_0x13_t *tlv_0x13;
            hdr++;

            tlv_0x13 = (QMI_WMS_EVENT_IND_response_0x13_t *)hdr;
            ev_rep->notification_type_set = true;
            COPY_MTOH(ev_rep->notification_type.type, tlv_0x13->notification_type);
            COPY_MTOH(ev_rep->notification_type.size, tlv_0x13->size);

            if (ev_rep->notification_type.size > QMI_WMS_MAX_RAW_DATA_LEN) {
                RLOGE("%s: notification_type.size = %u > QMI_WMS_MAX_RAW_DATA_LEN, increase the define",
                        __FUNCTION__, ev_rep->notification_type.size);
                ret = QMI_SDK_ERR_INTERNAL;
                goto end;
            }

            UNPACK_ARRAY(tlv_0x13->size,
                    ev_rep->notification_type.size,
                    ev_rep->notification_type.data,
                    QMI_WMS_MAX_RAW_DATA_LEN);
        } else {
            RLOGE("%s: malformed TLV, returned %u, expected at least %zu",
                    __FUNCTION__, le16toh(hdr->length),
                    sizeof(QMI_WMS_EVENT_IND_response_0x13_t));
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x14, *payload_len);
    if (hdr) {
        if (sizeof(QMI_WMS_EVENT_IND_response_0x14_t) <= le16toh(hdr->length)) {
            QMI_WMS_EVENT_IND_response_0x14_t *tlv_0x14;
            hdr++;

            tlv_0x14 = (QMI_WMS_EVENT_IND_response_0x14_t *)hdr;
            ev_rep->etws_plmn_info_set = true;
            COPY_MTOH(ev_rep->etws_plmn_info.mobile_country_code, tlv_0x14->mobile_country_code);
            COPY_MTOH(ev_rep->etws_plmn_info.mobile_network_code, tlv_0x14->mobile_network_code);
        } else {
            RLOGE("%s: malformed TLV, returned %u, expected at least %zu",
                    __FUNCTION__, le16toh(hdr->length),
                    sizeof(QMI_WMS_EVENT_IND_response_0x14_t));
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x15, *payload_len);
    if (hdr) {
        if (sizeof(QMI_WMS_EVENT_IND_response_0x15_t) <= le16toh(hdr->length)) {
            QMI_WMS_EVENT_IND_response_0x15_t *tlv_0x15;
            hdr++;

            tlv_0x15 = (QMI_WMS_EVENT_IND_response_0x15_t *)hdr;
            ev_rep->smsc_address_set = true;
            COPY_MTOH(ev_rep->smsc_address.len, tlv_0x15->len);

            if (ev_rep->smsc_address.len > SMSC_ADDRESS_DATA_MAX_LEN) {
                RLOGE("%s: smsc_address.len = %u > SMSC_ADDRESS_DATA_MAX_LEN, increase the define",
                        __FUNCTION__, ev_rep->smsc_address.len);
                ret = QMI_SDK_ERR_INTERNAL;
                goto end;
            }

            UNPACK_ARRAY(tlv_0x15->len,
                    ev_rep->smsc_address.len,
                    ev_rep->smsc_address.data,
                    SMSC_ADDRESS_DATA_MAX_LEN);
        } else {
            RLOGE("%s: malformed TLV, returned %u, expected at least %zu",
                    __FUNCTION__, le16toh(hdr->length),
                    sizeof(QMI_WMS_EVENT_IND_response_0x15_t));
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x16, *payload_len);
    if (hdr) {
        if (sizeof(QMI_WMS_EVENT_IND_response_0x16_t) <= le16toh(hdr->length)) {
            QMI_WMS_EVENT_IND_response_0x16_t *tlv_0x16;
            hdr++;

            tlv_0x16 = (QMI_WMS_EVENT_IND_response_0x16_t *)hdr;
            ev_rep->sms_on_ims_set = true;
            COPY_MTOH(ev_rep->sms_on_ims, tlv_0x16->sms_on_ims);
        } else {
            RLOGE("%s: malformed TLV, returned %u, expected at least %zu",
                    __FUNCTION__, le16toh(hdr->length),
                    sizeof(QMI_WMS_EVENT_IND_response_0x16_t));
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x18, *payload_len);
    if (hdr) {
        if (sizeof(QMI_WMS_EVENT_IND_response_0x18_t) <= le16toh(hdr->length)) {
            QMI_WMS_EVENT_IND_response_0x18_t *tlv_0x18;
            hdr++;
            tlv_0x18 = (QMI_WMS_EVENT_IND_response_0x18_t *)hdr;
            COPY_MTOH(ev_rep->bc_sms_rat_info, tlv_0x18->bc_sms_rat_info);
            ev_rep->bc_sms_rat_info_set = true;
        } else {
            RLOGE("%s: malformed TLV, returned %u, expected at least %zu",
                    __FUNCTION__, le16toh(hdr->length),
                    sizeof(QMI_WMS_EVENT_IND_response_0x18_t));
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }
    }

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

static QMI_SDK_error_t telit_wms_handle_memory_full_ind(uint16_t *payload_len, uint8_t *msg,
        uint8_t *out, uint16_t out_len)
{
    wms_memory_full_info_t *ev_rep = (wms_memory_full_info_t *) out;
    QMI_SDK_error_t ret = QMI_SDK_ERR_BUFFER_SZ;
    QMI_raw_content_header_t *hdr = NULL;

    if (out_len < sizeof(*ev_rep)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        goto end;
    }
    memset(ev_rep, 0, sizeof(*ev_rep));

    FIND_MANDATORY_TLV(msg, 0x01, *payload_len, hdr);
    if (sizeof(QMI_WMS_MEMORY_FULL_indication_0x01_t) <= le16toh(hdr->length)) {
        QMI_WMS_MEMORY_FULL_indication_0x01_t *tlv_0x01;
        hdr++;
        tlv_0x01 = (QMI_WMS_MEMORY_FULL_indication_0x01_t *)hdr;
        COPY_MTOH(ev_rep->storage_type, tlv_0x01->storage_type);
        COPY_MTOH(ev_rep->message_mode, tlv_0x01->message_mode);
    } else {
        RLOGE("%s: malformed TLV, returned %u, expected at least %zu",
                __FUNCTION__, le16toh(hdr->length),
                sizeof(QMI_WMS_MEMORY_FULL_indication_0x01_t));
        ret = QMI_SDK_ERR_MALFORMED_RSP;
        goto end;
    }

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

QMI_SDK_error_t telit_wms_indication_handler(uint16_t msg_id,
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
        case QMI_WMS_EVENT_REPORT_IND:
            ret = telit_wms_handle_event_ind(&payload_len, msg, out, out_len);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_WMS_MEMORY_FULL_IND:
            ret = telit_wms_handle_memory_full_ind(&payload_len, msg, out, out_len);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        default:
            RLOGW("%s: Indication not managed (%d)", __FUNCTION__, msg_id);
            break;
    }

end:
    return ret;
}
