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
#include "telit_sdk_private_wda.h"
#include "telit_sdk_wda.h"

/*****************************************************************************
 * QMI_WDA_SET_DATA_FORMAT (32 - 0x0020)
 */

QMI_SDK_error_t telit_wda_set_data_format_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        wda_data_format_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_WDA_SET_DATA_FORMAT;
    req_ctx->svc_id = QMI_SVC_WDA;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->qos_format_available) {
        QMI_WDA_generic_qos_format_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_WDA_generic_qos_format_t *)buf_position;
        COPY_HTOM(tlv_0x10->qos_format, input->qos_format);

        buf_position += tlv_sz;
    }

    if (input->link_prot_available) {
        QMI_WDA_generic_link_protocol_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_WDA_generic_link_protocol_t *)buf_position;
        COPY_HTOM(tlv_0x11->link_prot, input->link_prot);

        buf_position += tlv_sz;
    }

    if (input->ul_data_aggregation_protocol_available) {
        QMI_WDA_generic_data_aggregation_protocol_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_WDA_generic_data_aggregation_protocol_t *)buf_position;
        COPY_HTOM(tlv_0x12->data_aggregation_protocol, input->ul_data_aggregation_protocol);

        buf_position += tlv_sz;
    }

    if (input->dl_data_aggregation_protocol_available) {
        QMI_WDA_generic_data_aggregation_protocol_t *tlv_0x13;

        tlv_sz = sizeof(*tlv_0x13);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x13 = (QMI_WDA_generic_data_aggregation_protocol_t *)buf_position;
        COPY_HTOM(tlv_0x13->data_aggregation_protocol, input->dl_data_aggregation_protocol);

        buf_position += tlv_sz;
    }

    if (input->dl_data_aggregation_max_datagrams_available) {
        QMI_WDA_generic_data_aggregation_max_datagrams_t *tlv_0x15;

        tlv_sz = sizeof(*tlv_0x15);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x15;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x15 = (QMI_WDA_generic_data_aggregation_max_datagrams_t *)buf_position;
        COPY_HTOM(tlv_0x15->data_aggregation_max_datagrams, input->dl_data_aggregation_max_datagrams);

        buf_position += tlv_sz;
    }

    if (input->dl_data_aggregation_max_size_available) {
        QMI_WDA_generic_data_aggregation_max_datagram_size_t *tlv_0x16;

        tlv_sz = sizeof(*tlv_0x16);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x16;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x16 = (QMI_WDA_generic_data_aggregation_max_datagram_size_t *)buf_position;
        COPY_HTOM(tlv_0x16->data_aggregation_max_datagram_size, input->dl_data_aggregation_max_size);

        buf_position += tlv_sz;
    }

    if (input->peripheral_endpoint_available) {
        QMI_WDA_generic_peripheral_endpoint_t *tlv_0x17;

        tlv_sz = sizeof(*tlv_0x17);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x17;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x17 = (QMI_WDA_generic_peripheral_endpoint_t *)buf_position;
        COPY_HTOM(tlv_0x17->ep_type, input->peripheral_endpoint.ep_type);
        COPY_HTOM(tlv_0x17->iface_id, input->peripheral_endpoint.iface_id);

        buf_position += tlv_sz;
    }

    if (input->dl_minimum_padding_available) {
        QMI_WDA_generic_dl_minimum_padding_t *tlv_0x19;

        tlv_sz = sizeof(*tlv_0x19);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x19;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x19 = (QMI_WDA_generic_dl_minimum_padding_t *)buf_position;
        COPY_HTOM(tlv_0x19->dl_minimum_padding, input->dl_minimum_padding);

        buf_position += tlv_sz;
    }

    if (input->ul_data_aggregation_max_datagrams_available) {
        QMI_WDA_generic_data_aggregation_max_datagrams_t *tlv_0x1B;

        tlv_sz = sizeof(*tlv_0x1B);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1B;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1B = (QMI_WDA_generic_data_aggregation_max_datagrams_t *)buf_position;
        COPY_HTOM(tlv_0x1B->data_aggregation_max_datagrams, input->ul_data_aggregation_max_datagrams);

        buf_position += tlv_sz;
    }

    if (input->ul_data_aggregation_max_size_available) {
        QMI_WDA_generic_data_aggregation_max_datagram_size_t *tlv_0x1C;

        tlv_sz = sizeof(*tlv_0x1C);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1C;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1C = (QMI_WDA_generic_data_aggregation_max_datagram_size_t *)buf_position;
        COPY_HTOM(tlv_0x1C->data_aggregation_max_datagram_size, input->ul_data_aggregation_max_size);
    }

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

static QMI_SDK_error_t telit_wda_generic_data_format_unpack(uint8_t *rsp, uint16_t len,
        wda_data_format_t *out)
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
    if (hdr) {
        QMI_WDA_generic_qos_format_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_WDA_generic_qos_format_t);

        hdr++;
        tlv_0x10 = (QMI_WDA_generic_qos_format_t *)hdr;
        COPY_MTOH(out->qos_format, tlv_0x10->qos_format);
        out->qos_format_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        QMI_WDA_generic_link_protocol_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_WDA_generic_link_protocol_t);

        hdr++;
        tlv_0x11 = (QMI_WDA_generic_link_protocol_t *)hdr;
        COPY_MTOH(out->link_prot, tlv_0x11->link_prot);
        out->link_prot_available = true;
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    if (hdr) {
        QMI_WDA_generic_data_aggregation_protocol_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_WDA_generic_data_aggregation_protocol_t);

        hdr++;
        tlv_0x12 = (QMI_WDA_generic_data_aggregation_protocol_t *)hdr;
        COPY_MTOH(out->ul_data_aggregation_protocol, tlv_0x12->data_aggregation_protocol);
        out->ul_data_aggregation_protocol_available = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    if (hdr) {
        QMI_WDA_generic_data_aggregation_protocol_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_WDA_generic_data_aggregation_protocol_t);

        hdr++;
        tlv_0x13 = (QMI_WDA_generic_data_aggregation_protocol_t *)hdr;
        COPY_MTOH(out->dl_data_aggregation_protocol, tlv_0x13->data_aggregation_protocol);
        out->dl_data_aggregation_protocol_available = true;
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    if (hdr) {
        QMI_WDA_generic_data_aggregation_max_datagrams_t *tlv_0x15;

        CHECK_UNPACK_TLV_SIZE(QMI_WDA_generic_data_aggregation_max_datagrams_t);

        hdr++;
        tlv_0x15 = (QMI_WDA_generic_data_aggregation_max_datagrams_t *)hdr;
        COPY_MTOH(out->dl_data_aggregation_max_datagrams, tlv_0x15->data_aggregation_max_datagrams);
        out->dl_data_aggregation_max_datagrams_available = true;
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    if (hdr) {
        QMI_WDA_generic_data_aggregation_max_datagram_size_t *tlv_0x16;

        CHECK_UNPACK_TLV_SIZE(QMI_WDA_generic_data_aggregation_max_datagram_size_t);

        hdr++;
        tlv_0x16 = (QMI_WDA_generic_data_aggregation_max_datagram_size_t *)hdr;
        COPY_MTOH(out->dl_data_aggregation_max_size, tlv_0x16->data_aggregation_max_datagram_size);
        out->dl_data_aggregation_max_size_available = true;
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    if (hdr) {
        QMI_WDA_generic_data_aggregation_max_datagrams_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_WDA_generic_data_aggregation_max_datagrams_t);

        hdr++;
        tlv_0x17 = (QMI_WDA_generic_data_aggregation_max_datagrams_t *)hdr;
        COPY_MTOH(out->ul_data_aggregation_max_datagrams, tlv_0x17->data_aggregation_max_datagrams);
        out->ul_data_aggregation_max_datagrams_available = true;
    }

    hdr = search_TLV(msg, 0x18, payload_len);
    if (hdr) {
        QMI_WDA_generic_data_aggregation_max_datagram_size_t *tlv_0x18;

        CHECK_UNPACK_TLV_SIZE(QMI_WDA_generic_data_aggregation_max_datagram_size_t);

        hdr++;
        tlv_0x18 = (QMI_WDA_generic_data_aggregation_max_datagram_size_t *)hdr;
        COPY_MTOH(out->ul_data_aggregation_max_size, tlv_0x18->data_aggregation_max_datagram_size);
        out->ul_data_aggregation_max_size_available = true;
    }

    hdr = search_TLV(msg, 0x1A, payload_len);
    if (hdr) {
        QMI_WDA_generic_dl_minimum_padding_t *tlv_0x1A;

        CHECK_UNPACK_TLV_SIZE(QMI_WDA_generic_dl_minimum_padding_t);

        hdr++;
        tlv_0x1A = (QMI_WDA_generic_dl_minimum_padding_t *)hdr;
        COPY_MTOH(out->dl_minimum_padding, tlv_0x1A->dl_minimum_padding);
        out->dl_minimum_padding_available = true;
    }

end:
    return ret;
}

QMI_SDK_error_t telit_wda_set_data_format_unpack(uint8_t *rsp, uint16_t len, wda_data_format_t *out)
{
    return telit_wda_generic_data_format_unpack(rsp, len, out);
}

/*****************************************************************************
 * QMI_WDA_GET_DATA_FORMAT (33 - 0x0021)
 */

QMI_SDK_error_t telit_wda_get_data_format_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        wda_get_data_format_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_WDA_GET_DATA_FORMAT;
    req_ctx->svc_id = QMI_SVC_WDA;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->peripheral_endpoint_available) {
        QMI_WDA_generic_peripheral_endpoint_t *tlv_0x17;

        tlv_sz = sizeof(*tlv_0x17);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x17 = (QMI_WDA_generic_peripheral_endpoint_t *)buf_position;
        COPY_HTOM(tlv_0x17->ep_type, input->peripheral_endpoint.ep_type);
        COPY_HTOM(tlv_0x17->iface_id, input->peripheral_endpoint.iface_id);
    }

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

    /* When the optional TLV has not been set */
    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

QMI_SDK_error_t telit_wda_get_data_format_unpack(uint8_t *rsp, uint16_t len, wda_data_format_t *out)
{
    return telit_wda_generic_data_format_unpack(rsp, len, out);
}
