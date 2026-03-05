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
#include "telit_sdk_qos.h"
#include "telit_sdk_private_qos.h"

/*****************************************************************************
 * QMI_QOS_SET_EVENT_REPORT (1 - 0x0001)
 */

QMI_SDK_error_t telit_qos_set_event_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        qos_set_event_report_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_QOS_SET_EVENT_REPORT;
    req_ctx->svc_id = QMI_SVC_QOS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->global_flow_reporting != QMI_IND_ACTION_NONE) {
        QMI_QOS_SET_EVENT_REPORT_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_QOS_SET_EVENT_REPORT_request_0x10_t *)buf_position;
        tlv_0x10->global_flow_reporting = input->global_flow_reporting;
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_qos_set_event_report_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_QOS_BIND_DATA_PORT (43 - 0x002B)
 */

QMI_SDK_error_t telit_qos_bind_data_port_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        qos_bind_data_port_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_QOS_BIND_DATA_PORT;
    req_ctx->svc_id = QMI_SVC_QOS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->endpoint_id_available) {
        QMI_QOS_BIND_DATA_PORT_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_QOS_BIND_DATA_PORT_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->ep_type, input->endpoint_id.ep_type);
        COPY_HTOM(tlv_0x10->iface_id, input->endpoint_id.iface_id);

        buf_position += tlv_sz;
    }

    if (input->mux_id_available) {
        QMI_QOS_BIND_DATA_PORT_request_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_QOS_BIND_DATA_PORT_request_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->mux_id, input->mux_id);

        buf_position += tlv_sz;
    }

    if (input->data_port_available) {
        QMI_QOS_BIND_DATA_PORT_request_0x12_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_QOS_BIND_DATA_PORT_request_0x12_t *)buf_position;
        COPY_HTOM(tlv_0x12->data_port, input->data_port);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_qos_bind_data_port_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_QOS_INDICATION_REGISTER (47 - 0x002F)
 */

QMI_SDK_error_t telit_qos_indication_register_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        qos_indication_register_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_QOS_INDICATION_REGISTER;
    req_ctx->svc_id = QMI_SVC_QOS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->global_qos_flows != QMI_IND_ACTION_NONE) {
        QMI_INDICATION_REGISTER_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_INDICATION_REGISTER_request_0x10_t *)buf_position;
        tlv_0x10->global_qos_flows = input->global_qos_flows;
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_qos_indication_register_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_QOS_GET_QOS_INFO (51 - 0x0033)
 */

QMI_SDK_error_t telit_qos_get_qos_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        qos_get_qos_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_QOS_GET_QOS_INFO_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_QOS_GET_QOS_INFO;
    req_ctx->svc_id = QMI_SVC_QOS;
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

    tlv_0x01 = (QMI_QOS_GET_QOS_INFO_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->qos_id, input->qos_id);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_qos_get_qos_info_unpack(uint8_t *rsp, uint16_t len, qos_get_qos_info_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg = NULL;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    if (hdr) {
        QMI_QOS_GET_QOS_INFO_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_GET_QOS_INFO_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_QOS_GET_QOS_INFO_response_0x10_t *)hdr;
        out->flow_status_available = true;
        COPY_MTOH(out->flow_status, tlv_0x10->flow_status);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        QMI_QOS_generic_granted_flow_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_granted_flow_t);

        hdr++;
        tlv_0x11 = (QMI_QOS_generic_granted_flow_t *)hdr;
        out->tx_qos_granted_flow_available = true;
        COPY_MTOH(out->tx_qos_granted_flow.flow_valid_params, tlv_0x11->flow_valid_params);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_trf_cls, tlv_0x11->ip_flow_trf_cls);
        COPY_MTOH(out->tx_qos_granted_flow.data_rate_max, tlv_0x11->data_rate_max);
        COPY_MTOH(out->tx_qos_granted_flow.guaranteed_rate, tlv_0x11->guaranteed_rate);
        COPY_MTOH(out->tx_qos_granted_flow.peak_rate, tlv_0x11->peak_rate);
        COPY_MTOH(out->tx_qos_granted_flow.token_rate, tlv_0x11->token_rate);
        COPY_MTOH(out->tx_qos_granted_flow.bucket_size, tlv_0x11->bucket_size);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_latency, tlv_0x11->ip_flow_latency);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_jitter, tlv_0x11->ip_flow_jitter);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_pkt_error_rate_multiplier,
                tlv_0x11->ip_flow_pkt_error_rate_multiplier);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_pkt_error_rate_exponent,
                tlv_0x11->ip_flow_pkt_error_rate_exponent);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_min_policed_packet_size,
                tlv_0x11->ip_flow_min_policed_packet_size);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_max_allowed_packet_size,
                tlv_0x11->ip_flow_max_allowed_packet_size);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_3gpp_residual_bit_error_rate,
                tlv_0x11->ip_flow_3gpp_residual_bit_error_rate);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_3gpp_traffic_handling_priority,
                tlv_0x11->ip_flow_3gpp_traffic_handling_priority);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_3gpp2_profile_id, tlv_0x11->ip_flow_3gpp2_profile_id);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_3gpp2_flow_priority,
                tlv_0x11->ip_flow_3gpp2_flow_priority);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_3gpp_im_cn_flag, tlv_0x11->ip_flow_3gpp_im_cn_flag);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_3gpp_sig_ind, tlv_0x11->ip_flow_3gpp_sig_ind);
        COPY_MTOH(out->tx_qos_granted_flow.ip_flow_lte_qci, tlv_0x11->ip_flow_lte_qci);
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    if (hdr) {
        QMI_QOS_generic_granted_flow_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_granted_flow_t);

        hdr++;
        tlv_0x12 = (QMI_QOS_generic_granted_flow_t *)hdr;
        out->rx_qos_granted_flow_available = true;
        COPY_MTOH(out->rx_qos_granted_flow.flow_valid_params, tlv_0x12->flow_valid_params);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_trf_cls, tlv_0x12->ip_flow_trf_cls);
        COPY_MTOH(out->rx_qos_granted_flow.data_rate_max, tlv_0x12->data_rate_max);
        COPY_MTOH(out->rx_qos_granted_flow.guaranteed_rate, tlv_0x12->guaranteed_rate);
        COPY_MTOH(out->rx_qos_granted_flow.peak_rate, tlv_0x12->peak_rate);
        COPY_MTOH(out->rx_qos_granted_flow.token_rate, tlv_0x12->token_rate);
        COPY_MTOH(out->rx_qos_granted_flow.bucket_size, tlv_0x12->bucket_size);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_latency, tlv_0x12->ip_flow_latency);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_jitter, tlv_0x12->ip_flow_jitter);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_pkt_error_rate_multiplier,
                tlv_0x12->ip_flow_pkt_error_rate_multiplier);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_pkt_error_rate_exponent,
                tlv_0x12->ip_flow_pkt_error_rate_exponent);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_min_policed_packet_size,
                tlv_0x12->ip_flow_min_policed_packet_size);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_max_allowed_packet_size,
                tlv_0x12->ip_flow_max_allowed_packet_size);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_3gpp_residual_bit_error_rate,
                tlv_0x12->ip_flow_3gpp_residual_bit_error_rate);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_3gpp_traffic_handling_priority,
                tlv_0x12->ip_flow_3gpp_traffic_handling_priority);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_3gpp2_profile_id, tlv_0x12->ip_flow_3gpp2_profile_id);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_3gpp2_flow_priority,
                tlv_0x12->ip_flow_3gpp2_flow_priority);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_3gpp_im_cn_flag, tlv_0x12->ip_flow_3gpp_im_cn_flag);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_3gpp_sig_ind, tlv_0x12->ip_flow_3gpp_sig_ind);
        COPY_MTOH(out->rx_qos_granted_flow.ip_flow_lte_qci, tlv_0x12->ip_flow_lte_qci);
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    if (hdr) {
        QMI_QOS_generic_filter_specs_t *tlv_0x13;
        QMI_QOS_generic_filter_spec_t *filter_spec;
        uint8_t i, j;
        uint8_t *buf_position;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_filter_specs_t);

        hdr++;
        tlv_0x13 = (QMI_QOS_generic_filter_specs_t *)hdr;
        COPY_MTOH(out->tx_qos_filter_specs_size, tlv_0x13->qos_filter_size);

        if (out->tx_qos_filter_specs_size > QMI_QOS_FILTER_MAX_SIZE) {
            RLOGE("%s: tx_qos_filter_specs_size = %u > QMI_QOS_FILTER_MAX_SIZE, increase the define",
                    __FUNCTION__, out->tx_qos_filter_specs_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        buf_position = (uint8_t *)&tlv_0x13->qos_filter_size;
        /* Skipping length */
        buf_position += sizeof(tlv_0x13->qos_filter_size);
        filter_spec = (QMI_QOS_generic_filter_spec_t *)buf_position;
        for (i = 0; i < out->tx_qos_filter_specs_size; i++) {
            COPY_MTOH(out->tx_qos_filter_specs[i].ip_version, filter_spec[i].ip_version);
            COPY_MTOH(out->tx_qos_filter_specs[i].valid_params_ipv4, filter_spec[i].valid_params_ipv4);
            /* No conversion, since arriving in big endian and needs to be kept in big endian */
            out->tx_qos_filter_specs[i].ipv4_addr_src.addr = filter_spec[i].ipv4_addr_src;
            out->tx_qos_filter_specs[i].ipv4_addr_src.mask = filter_spec[i].ipv4_mask_src;
            out->tx_qos_filter_specs[i].ipv4_addr_dest.addr = filter_spec[i].ipv4_addr_dest;
            out->tx_qos_filter_specs[i].ipv4_addr_dest.mask = filter_spec[i].ipv4_mask_dest;
            COPY_MTOH(out->tx_qos_filter_specs[i].ipv4_tos.val, filter_spec[i].ipv4_tos_val);
            COPY_MTOH(out->tx_qos_filter_specs[i].ipv4_tos.mask, filter_spec[i].ipv4_tos_mask);
            COPY_MTOH(out->tx_qos_filter_specs[i].valid_params_ipv6, filter_spec[i].valid_params_ipv6);
            COPY_MTOH(out->tx_qos_filter_specs[i].ipv6_addr_src.prefix_len,
                    filter_spec[i].ipv6_prefix_len_src);
            for (j = 0; j < IPV6_ADDR_SIZE; j++) {
                /* No conversion, since arriving in big endian and needs to be kept in big endian */
                out->tx_qos_filter_specs[i].ipv6_addr_src.addr[j] = filter_spec[i].ipv6_addr_src[j];
            }
            COPY_MTOH(out->tx_qos_filter_specs[i].ipv6_addr_dest.prefix_len,
                    filter_spec[i].ipv6_prefix_len_dest);
            for (j = 0; j < IPV6_ADDR_SIZE; j++) {
                /* No conversion, since arriving in big endian and needs to be kept in big endian */
                out->tx_qos_filter_specs[i].ipv6_addr_dest.addr[j] = filter_spec[i].ipv6_addr_dest[j];
            }
            COPY_MTOH(out->tx_qos_filter_specs[i].ipv6_traffic_class.val,
                    filter_spec[i].ipv6_traffic_class_val);
            COPY_MTOH(out->tx_qos_filter_specs[i].ipv6_traffic_class.mask,
                    filter_spec[i].ipv6_traffic_class_mask);
            COPY_MTOH(out->tx_qos_filter_specs[i].ipv6_flow_label, filter_spec[i].ipv6_flow_label);
            COPY_MTOH(out->tx_qos_filter_specs[i].xport_protocol, filter_spec[i].xport_protocol);
            COPY_MTOH(out->tx_qos_filter_specs[i].valid_params_tcp, filter_spec[i].valid_params_tcp);
            COPY_MTOH(out->tx_qos_filter_specs[i].tcp_port_src.port, filter_spec[i].port_src_tcp);
            COPY_MTOH(out->tx_qos_filter_specs[i].tcp_port_src.range, filter_spec[i].range_src_tcp);
            COPY_MTOH(out->tx_qos_filter_specs[i].tcp_port_dest.port, filter_spec[i].port_dest_tcp);
            COPY_MTOH(out->tx_qos_filter_specs[i].tcp_port_dest.range, filter_spec[i].range_dest_tcp);
            COPY_MTOH(out->tx_qos_filter_specs[i].valid_params_udp, filter_spec[i].valid_params_udp);
            COPY_MTOH(out->tx_qos_filter_specs[i].udp_port_src.port, filter_spec[i].port_src_udp);
            COPY_MTOH(out->tx_qos_filter_specs[i].udp_port_src.range, filter_spec[i].range_src_udp);
            COPY_MTOH(out->tx_qos_filter_specs[i].udp_port_dest.port, filter_spec[i].port_dest_udp);
            COPY_MTOH(out->tx_qos_filter_specs[i].udp_port_dest.range, filter_spec[i].range_dest_udp);
            COPY_MTOH(out->tx_qos_filter_specs[i].valid_params_icmp, filter_spec[i].valid_params_icmp);
            COPY_MTOH(out->tx_qos_filter_specs[i].icmp_type, filter_spec[i].icmp_type);
            COPY_MTOH(out->tx_qos_filter_specs[i].icmp_code, filter_spec[i].icmp_code);
            COPY_MTOH(out->tx_qos_filter_specs[i].valid_params_esp, filter_spec[i].valid_params_esp);
            COPY_MTOH(out->tx_qos_filter_specs[i].esp_spi, filter_spec[i].esp_spi);
            COPY_MTOH(out->tx_qos_filter_specs[i].valid_params_ah, filter_spec[i].valid_params_ah);
            COPY_MTOH(out->tx_qos_filter_specs[i].ah_spi, filter_spec[i].ah_spi);
            COPY_MTOH(out->tx_qos_filter_specs[i].filter_id, filter_spec[i].filter_id);
            COPY_MTOH(out->tx_qos_filter_specs[i].filter_precedence, filter_spec[i].filter_precedence);
        }
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    if (hdr) {
        QMI_QOS_generic_filter_specs_t *tlv_0x14;
        QMI_QOS_generic_filter_spec_t *filter_spec;
        uint8_t i, j;
        uint8_t *buf_position;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_filter_specs_t);

        hdr++;
        tlv_0x14 = (QMI_QOS_generic_filter_specs_t *)hdr;
        COPY_MTOH(out->rx_qos_filter_specs_size, tlv_0x14->qos_filter_size);

        if (out->rx_qos_filter_specs_size > QMI_QOS_FILTER_MAX_SIZE) {
            RLOGE("%s: rx_qos_filter_specs_size = %u > QMI_QOS_FILTER_MAX_SIZE, increase the define",
                    __FUNCTION__, out->rx_qos_filter_specs_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        buf_position = (uint8_t *)&tlv_0x14->qos_filter_size;
        /* Skipping length */
        buf_position += sizeof(tlv_0x14->qos_filter_size);
        filter_spec = (QMI_QOS_generic_filter_spec_t *)buf_position;
        for (i = 0; i < out->rx_qos_filter_specs_size; i++) {
            COPY_MTOH(out->rx_qos_filter_specs[i].ip_version, filter_spec[i].ip_version);
            COPY_MTOH(out->rx_qos_filter_specs[i].valid_params_ipv4, filter_spec[i].valid_params_ipv4);
            /* No conversion, since arriving in big endian and needs to be kept in big endian */
            out->rx_qos_filter_specs[i].ipv4_addr_src.addr = filter_spec[i].ipv4_addr_src;
            out->rx_qos_filter_specs[i].ipv4_addr_src.mask = filter_spec[i].ipv4_mask_src;
            out->rx_qos_filter_specs[i].ipv4_addr_dest.addr = filter_spec[i].ipv4_addr_dest;
            out->rx_qos_filter_specs[i].ipv4_addr_dest.mask = filter_spec[i].ipv4_mask_dest;
            COPY_MTOH(out->rx_qos_filter_specs[i].ipv4_tos.val, filter_spec[i].ipv4_tos_val);
            COPY_MTOH(out->rx_qos_filter_specs[i].ipv4_tos.mask, filter_spec[i].ipv4_tos_mask);
            COPY_MTOH(out->rx_qos_filter_specs[i].valid_params_ipv6, filter_spec[i].valid_params_ipv6);
            COPY_MTOH(out->rx_qos_filter_specs[i].ipv6_addr_src.prefix_len,
                    filter_spec[i].ipv6_prefix_len_src);
            for (j = 0; j < IPV6_ADDR_SIZE; j++) {
                /* No conversion, since arriving in big endian and needs to be kept in big endian */
                out->rx_qos_filter_specs[i].ipv6_addr_src.addr[j] = filter_spec[i].ipv6_addr_src[j];
            }
            COPY_MTOH(out->rx_qos_filter_specs[i].ipv6_addr_dest.prefix_len,
                    filter_spec[i].ipv6_prefix_len_dest);
            for (j = 0; j < IPV6_ADDR_SIZE; j++) {
                /* No conversion, since arriving in big endian and needs to be kept in big endian */
                out->rx_qos_filter_specs[i].ipv6_addr_dest.addr[j] = filter_spec[i].ipv6_addr_dest[j];
            }
            COPY_MTOH(out->rx_qos_filter_specs[i].ipv6_traffic_class.val,
                    filter_spec[i].ipv6_traffic_class_val);
            COPY_MTOH(out->rx_qos_filter_specs[i].ipv6_traffic_class.mask,
                    filter_spec[i].ipv6_traffic_class_mask);
            COPY_MTOH(out->rx_qos_filter_specs[i].ipv6_flow_label, filter_spec[i].ipv6_flow_label);
            COPY_MTOH(out->rx_qos_filter_specs[i].xport_protocol, filter_spec[i].xport_protocol);
            COPY_MTOH(out->rx_qos_filter_specs[i].valid_params_tcp, filter_spec[i].valid_params_tcp);
            COPY_MTOH(out->rx_qos_filter_specs[i].tcp_port_src.port, filter_spec[i].port_src_tcp);
            COPY_MTOH(out->rx_qos_filter_specs[i].tcp_port_src.range, filter_spec[i].range_src_tcp);
            COPY_MTOH(out->rx_qos_filter_specs[i].tcp_port_dest.port, filter_spec[i].port_dest_tcp);
            COPY_MTOH(out->rx_qos_filter_specs[i].tcp_port_dest.range, filter_spec[i].range_dest_tcp);
            COPY_MTOH(out->rx_qos_filter_specs[i].valid_params_udp, filter_spec[i].valid_params_udp);
            COPY_MTOH(out->rx_qos_filter_specs[i].udp_port_src.port, filter_spec[i].port_src_udp);
            COPY_MTOH(out->rx_qos_filter_specs[i].udp_port_src.range, filter_spec[i].range_src_udp);
            COPY_MTOH(out->rx_qos_filter_specs[i].udp_port_dest.port, filter_spec[i].port_dest_udp);
            COPY_MTOH(out->rx_qos_filter_specs[i].udp_port_dest.range, filter_spec[i].range_dest_udp);
            COPY_MTOH(out->rx_qos_filter_specs[i].valid_params_icmp, filter_spec[i].valid_params_icmp);
            COPY_MTOH(out->rx_qos_filter_specs[i].icmp_type, filter_spec[i].icmp_type);
            COPY_MTOH(out->rx_qos_filter_specs[i].icmp_code, filter_spec[i].icmp_code);
            COPY_MTOH(out->rx_qos_filter_specs[i].valid_params_esp, filter_spec[i].valid_params_esp);
            COPY_MTOH(out->rx_qos_filter_specs[i].esp_spi, filter_spec[i].esp_spi);
            COPY_MTOH(out->rx_qos_filter_specs[i].valid_params_ah, filter_spec[i].valid_params_ah);
            COPY_MTOH(out->rx_qos_filter_specs[i].ah_spi, filter_spec[i].ah_spi);
            COPY_MTOH(out->rx_qos_filter_specs[i].filter_id, filter_spec[i].filter_id);
            COPY_MTOH(out->rx_qos_filter_specs[i].filter_precedence, filter_spec[i].filter_precedence);
        }
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI QOS indications
 */

static QMI_SDK_error_t telit_qos_event_report_flow_spec_unpack(uint8_t *rsp, uint16_t len,
        qos_flow_spec_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_QOS_generic_ip_flow_index_t *tlv_0x10;

    payload_len = len;
    msg = rsp;

    FIND_MANDATORY_TLV(msg, 0x10, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_index_t);

    hdr++;
    tlv_0x10 = (QMI_QOS_generic_ip_flow_index_t *)hdr;
    COPY_MTOH(out->ip_flow_index, tlv_0x10->ip_flow_index);

    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        QMI_QOS_generic_ip_flow_traffic_class_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_traffic_class_t);

        hdr++;
        tlv_0x11 = (QMI_QOS_generic_ip_flow_traffic_class_t *)hdr;
        out->ip_flow_traffic_class_available = true;
        COPY_MTOH(out->ip_flow_traffic_class, tlv_0x11->ip_flow_traffic_class);
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    if (hdr) {
        QMI_QOS_generic_ip_flow_data_rate_min_max_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_data_rate_min_max_t);

        hdr++;
        tlv_0x12 = (QMI_QOS_generic_ip_flow_data_rate_min_max_t *)hdr;
        out->ip_flow_data_rate_min_max_available = true;
        COPY_MTOH(out->ip_flow_data_rate_min_max.data_rate_max, tlv_0x12->data_rate_max);
        COPY_MTOH(out->ip_flow_data_rate_min_max.guaranteed_data_rate, tlv_0x12->guaranteed_data_rate);
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    if (hdr) {
        QMI_QOS_generic_ip_flow_data_rate_token_bucket_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_data_rate_token_bucket_t);

        hdr++;
        tlv_0x13 = (QMI_QOS_generic_ip_flow_data_rate_token_bucket_t *)hdr;
        out->ip_flow_data_rate_token_bucket_available = true;
        COPY_MTOH(out->ip_flow_data_rate_token_bucket.peak_rate, tlv_0x13->peak_rate);
        COPY_MTOH(out->ip_flow_data_rate_token_bucket.token_rate, tlv_0x13->token_rate);
        COPY_MTOH(out->ip_flow_data_rate_token_bucket.bucket_size, tlv_0x13->bucket_size);
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    if (hdr) {
        QMI_QOS_generic_ip_flow_latency_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_latency_t);

        hdr++;
        tlv_0x14 = (QMI_QOS_generic_ip_flow_latency_t *)hdr;
        out->ip_flow_latency_available = true;
        COPY_MTOH(out->ip_flow_latency, tlv_0x14->ip_flow_latency);
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    if (hdr) {
        QMI_QOS_generic_ip_flow_jitter_t *tlv_0x15;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_jitter_t);

        hdr++;
        tlv_0x15 = (QMI_QOS_generic_ip_flow_jitter_t *)hdr;
        out->ip_flow_jitter_available = true;
        COPY_MTOH(out->ip_flow_jitter, tlv_0x15->ip_flow_jitter);
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    if (hdr) {
        QMI_QOS_generic_ip_flow_packet_error_rate_t *tlv_0x16;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_packet_error_rate_t);

        hdr++;
        tlv_0x16 = (QMI_QOS_generic_ip_flow_packet_error_rate_t *)hdr;
        out->ip_flow_packet_error_rate_available = true;
        COPY_MTOH(out->ip_flow_packet_error_rate.error_rate_multiplier, tlv_0x16->error_rate_multiplier);
        COPY_MTOH(out->ip_flow_packet_error_rate.error_rate_exponent, tlv_0x16->error_rate_exponent);
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    if (hdr) {
        QMI_QOS_generic_ip_flow_min_policed_pkt_sz_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_min_policed_pkt_sz_t);

        hdr++;
        tlv_0x17 = (QMI_QOS_generic_ip_flow_min_policed_pkt_sz_t *)hdr;
        out->ip_flow_minimum_policed_packet_size_available = true;
        COPY_MTOH(out->ip_flow_minimum_policed_packet_size, tlv_0x17->ip_flow_minimum_policed_packet_size);
    }

    hdr = search_TLV(msg, 0x18, payload_len);
    if (hdr) {
        QMI_QOS_generic_ip_flow_max_allowed_pkt_sz_t *tlv_0x18;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_max_allowed_pkt_sz_t);

        hdr++;
        tlv_0x18 = (QMI_QOS_generic_ip_flow_max_allowed_pkt_sz_t *)hdr;
        out->ip_flow_maximum_allowed_packet_size_available = true;
        COPY_MTOH(out->ip_flow_maximum_allowed_packet_size, tlv_0x18->ip_flow_maximum_allowed_packet_size);
    }

    hdr = search_TLV(msg, 0x19, payload_len);
    if (hdr) {
        QMI_QOS_generic_ip_flow_3gpp_residual_ber_t *tlv_0x19;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_3gpp_residual_ber_t);

        hdr++;
        tlv_0x19 = (QMI_QOS_generic_ip_flow_3gpp_residual_ber_t *)hdr;
        out->ip_flow_3gpp_residual_bit_error_rate_available = true;
        COPY_MTOH(out->ip_flow_3gpp_residual_bit_error_rate, tlv_0x19->ip_flow_3gpp_residual_bit_error_rate);
    }

    hdr = search_TLV(msg, 0x1A, payload_len);
    if (hdr) {
        QMI_QOS_generic_ip_flow_3gpp_traffic_handling_pri_t *tlv_0x1A;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_3gpp_traffic_handling_pri_t);

        hdr++;
        tlv_0x1A = (QMI_QOS_generic_ip_flow_3gpp_traffic_handling_pri_t *)hdr;
        out->ip_flow_3gpp_traffic_handling_priority_available = true;
        COPY_MTOH(out->ip_flow_3gpp_traffic_handling_priority,
                tlv_0x1A->ip_flow_3gpp_traffic_handling_priority);
    }

    hdr = search_TLV(msg, 0x1D, payload_len);
    if (hdr) {
        QMI_QOS_generic_ip_flow_3gpp_im_cn_flag_t *tlv_0x1D;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_3gpp_im_cn_flag_t);

        hdr++;
        tlv_0x1D = (QMI_QOS_generic_ip_flow_3gpp_im_cn_flag_t *)hdr;
        out->ip_flow_3gpp_im_cn_flag_available = true;
        COPY_MTOH(out->ip_flow_3gpp_im_cn_flag, tlv_0x1D->ip_flow_3gpp_im_cn_flag);
    }

    hdr = search_TLV(msg, 0x1E, payload_len);
    if (hdr) {
        QMI_QOS_generic_ip_flow_3gpp_sig_ind_t *tlv_0x1E;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_3gpp_sig_ind_t);

        hdr++;
        tlv_0x1E = (QMI_QOS_generic_ip_flow_3gpp_sig_ind_t *)hdr;
        out->ip_flow_3gpp_sig_ind_available = true;
        COPY_MTOH(out->ip_flow_3gpp_sig_ind, tlv_0x1E->ip_flow_3gpp_sig_ind);
    }

    hdr = search_TLV(msg, 0x1F, payload_len);
    if (hdr) {
        QMI_QOS_generic_lte_qci_t *tlv_0x1F;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_lte_qci_t);

        hdr++;
        tlv_0x1F = (QMI_QOS_generic_lte_qci_t *)hdr;
        out->lte_qos_class_identifier_available = true;
        COPY_MTOH(out->lte_qos_class_identifier, tlv_0x1F->lte_qos_class_identifier);
    }

end:
    return ret;
}

static QMI_SDK_error_t telit_qos_event_report_filter_spec_unpack(uint8_t *rsp, uint16_t len,
        qos_filter_spec_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_QOS_generic_ip_filter_index_t *tlv_0x10;
    QMI_QOS_generic_ip_filter_version_t *tlv_0x11;
    uint8_t i;

    payload_len = len;
    msg = rsp;

    FIND_MANDATORY_TLV(msg, 0x10, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_filter_index_t);

    hdr++;
    tlv_0x10 = (QMI_QOS_generic_ip_filter_index_t *)hdr;
    COPY_MTOH(out->ip_filter_index, tlv_0x10->ip_filter_index);

    FIND_MANDATORY_TLV(msg, 0x11, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_filter_version_t);

    hdr++;
    tlv_0x11 = (QMI_QOS_generic_ip_filter_version_t *)hdr;
    COPY_MTOH(out->ip_filter_version, tlv_0x11->ip_filter_version);

    hdr = search_TLV(msg, 0x12, payload_len);
    if (hdr) {
        QMI_QOS_generic_ipv4_address_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ipv4_address_t);

        hdr++;
        tlv_0x12 = (QMI_QOS_generic_ipv4_address_t *)hdr;
        out->ipv4_filter_src_addr_available = true;
        /* No conversion for this, since we want the address in network byte order */
        out->ipv4_filter_src_addr.addr = tlv_0x12->addr;
        out->ipv4_filter_src_addr.mask = tlv_0x12->mask;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    if (hdr) {
        QMI_QOS_generic_ipv4_address_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ipv4_address_t);

        hdr++;
        tlv_0x13 = (QMI_QOS_generic_ipv4_address_t *)hdr;
        out->ipv4_filter_dest_addr_available = true;
        /* No conversion for this, since we want the address in network byte order */
        out->ipv4_filter_dest_addr.addr = tlv_0x13->addr;
        out->ipv4_filter_dest_addr.mask = tlv_0x13->mask;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    if (hdr) {
        QMI_QOS_generic_ipv4_next_hdr_proto_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ipv4_next_hdr_proto_t);

        hdr++;
        tlv_0x14 = (QMI_QOS_generic_ipv4_next_hdr_proto_t *)hdr;
        out->ipv4_filter_next_header_protocol_available = true;
        COPY_MTOH(out->ipv4_filter_next_header_protocol, tlv_0x14->ipv4_filter_next_header_protocol);
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    if (hdr) {
        QMI_QOS_generic_ipv4_tos_t *tlv_0x15;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ipv4_tos_t);

        hdr++;
        tlv_0x15 = (QMI_QOS_generic_ipv4_tos_t *)hdr;
        out->ipv4_filter_tos_available = true;
        COPY_MTOH(out->ipv4_filter_tos.val, tlv_0x15->val);
        COPY_MTOH(out->ipv4_filter_tos.mask, tlv_0x15->mask);
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    if (hdr) {
        QMI_QOS_generic_ipv6_address_t *tlv_0x16;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ipv6_address_t);

        hdr++;
        tlv_0x16 = (QMI_QOS_generic_ipv6_address_t *)hdr;
        out->ipv6_filter_src_addr_available = true;
        COPY_MTOH(out->ipv6_filter_src_addr.prefix_len, tlv_0x16->prefix_len);
        for (i = 0; i < IPV6_ADDR_SIZE; i++) {
            /* No conversion, since arriving in big endian and needs to be kept in big endian */
            out->ipv6_filter_src_addr.addr[i] = tlv_0x16->addr[i];
        }
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    if (hdr) {
        QMI_QOS_generic_ipv6_address_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ipv6_address_t);

        hdr++;
        tlv_0x17 = (QMI_QOS_generic_ipv6_address_t *)hdr;
        out->ipv6_filter_dest_addr_available = true;
        COPY_MTOH(out->ipv6_filter_dest_addr.prefix_len, tlv_0x17->prefix_len);
        for (i = 0; i < 8; i++) {
            /* No conversion, since arriving in big endian and needs to be kept in big endian */
            out->ipv6_filter_dest_addr.addr[i] = tlv_0x17->addr[i];
        }
    }

    hdr = search_TLV(msg, 0x19, payload_len);
    if (hdr) {
        QMI_QOS_generic_ipv6_filter_traffic_class_t *tlv_0x19;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ipv6_filter_traffic_class_t);

        hdr++;
        tlv_0x19 = (QMI_QOS_generic_ipv6_filter_traffic_class_t *)hdr;
        out->ipv6_filter_traffic_class_available = true;
        COPY_MTOH(out->ipv6_filter_traffic_class.val, tlv_0x19->val);
        COPY_MTOH(out->ipv6_filter_traffic_class.mask, tlv_0x19->mask);
    }

    hdr = search_TLV(msg, 0x1A, payload_len);
    if (hdr) {
        QMI_QOS_generic_ipv6_filter_flow_label_t *tlv_0x1A;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ipv6_filter_flow_label_t);

        hdr++;
        tlv_0x1A = (QMI_QOS_generic_ipv6_filter_flow_label_t *)hdr;
        out->ipv6_filter_flow_label_available = true;
        COPY_MTOH(out->ipv6_filter_flow_label, tlv_0x1A->ipv6_filter_flow_label);
    }

    hdr = search_TLV(msg, 0x1B, payload_len);
    if (hdr) {
        QMI_QOS_generic_filter_port_t *tlv_0x1B;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_filter_port_t);

        hdr++;
        tlv_0x1B = (QMI_QOS_generic_filter_port_t *)hdr;
        out->tcp_filter_src_port_available = true;
        COPY_MTOH(out->tcp_filter_src_port.port, tlv_0x1B->port);
        COPY_MTOH(out->tcp_filter_src_port.range, tlv_0x1B->range);
    }

    hdr = search_TLV(msg, 0x1C, payload_len);
    if (hdr) {
        QMI_QOS_generic_filter_port_t *tlv_0x1C;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_filter_port_t);

        hdr++;
        tlv_0x1C = (QMI_QOS_generic_filter_port_t *)hdr;
        out->tcp_filter_dest_port_available = true;
        COPY_MTOH(out->tcp_filter_dest_port.port, tlv_0x1C->port);
        COPY_MTOH(out->tcp_filter_dest_port.range, tlv_0x1C->range);
    }

    hdr = search_TLV(msg, 0x1D, payload_len);
    if (hdr) {
        QMI_QOS_generic_filter_port_t *tlv_0x1D;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_filter_port_t);

        hdr++;
        tlv_0x1D = (QMI_QOS_generic_filter_port_t *)hdr;
        out->udp_filter_src_port_available = true;
        COPY_MTOH(out->udp_filter_src_port.port, tlv_0x1D->port);
        COPY_MTOH(out->udp_filter_src_port.range, tlv_0x1D->range);
    }

    hdr = search_TLV(msg, 0x1E, payload_len);
    if (hdr) {
        QMI_QOS_generic_filter_port_t *tlv_0x1E;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_filter_port_t);

        hdr++;
        tlv_0x1E = (QMI_QOS_generic_filter_port_t *)hdr;
        out->udp_filter_dest_port_available = true;
        COPY_MTOH(out->udp_filter_dest_port.port, tlv_0x1E->port);
        COPY_MTOH(out->udp_filter_dest_port.range, tlv_0x1E->range);
    }

    hdr = search_TLV(msg, 0x21, payload_len);
    if (hdr) {
        QMI_QOS_generic_filter_esp_t *tlv_0x21;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_filter_esp_t);

        hdr++;
        tlv_0x21 = (QMI_QOS_generic_filter_esp_t *)hdr;
        out->esp_filter_security_policy_index_available = true;
        COPY_MTOH(out->esp_filter_security_policy_index, tlv_0x21->esp_filter_security_policy_index);
    }

    hdr = search_TLV(msg, 0x22, payload_len);
    if (hdr) {
        QMI_QOS_generic_filter_precedence_t *tlv_0x22;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_filter_precedence_t);

        hdr++;
        tlv_0x22 = (QMI_QOS_generic_filter_precedence_t *)hdr;
        out->filter_precedence_available = true;
        COPY_MTOH(out->filter_precedence, tlv_0x22->filter_precedence);
    }

    hdr = search_TLV(msg, 0x23, payload_len);
    if (hdr) {
        QMI_QOS_generic_filter_id_t *tlv_0x23;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_filter_id_t);

        hdr++;
        tlv_0x23 = (QMI_QOS_generic_filter_id_t *)hdr;
        out->filter_id_available = true;
        COPY_MTOH(out->filter_id, tlv_0x23->filter_id);
    }

    hdr = search_TLV(msg, 0x24, payload_len);
    if (hdr) {
        QMI_QOS_generic_filter_port_t *tlv_0x24;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_filter_port_t);

        hdr++;
        tlv_0x24 = (QMI_QOS_generic_filter_port_t *)hdr;
        out->transport_prot_filter_src_port_available = true;
        COPY_MTOH(out->transport_prot_filter_src_port.port, tlv_0x24->port);
        COPY_MTOH(out->transport_prot_filter_src_port.range, tlv_0x24->range);
    }

    hdr = search_TLV(msg, 0x25, payload_len);
    if (hdr) {
        QMI_QOS_generic_filter_port_t *tlv_0x25;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_filter_port_t);

        hdr++;
        tlv_0x25 = (QMI_QOS_generic_filter_port_t *)hdr;
        out->transport_prot_filter_dest_port_available = true;
        COPY_MTOH(out->transport_prot_filter_dest_port.port, tlv_0x25->port);
        COPY_MTOH(out->transport_prot_filter_dest_port.range, tlv_0x25->range);
    }

end:
    return ret;
}

static QMI_SDK_error_t telit_qos_event_report_flow_info_unpack(uint8_t *rsp, uint16_t len,
        qos_flow_info_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_QOS_generic_flow_state_t *tlv_0x10;

    payload_len = len;
    msg = rsp;

    FIND_MANDATORY_TLV(msg, 0x10, payload_len, hdr);
    CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_flow_state_t);

    hdr++;
    tlv_0x10 = (QMI_QOS_generic_flow_state_t *)hdr;
    COPY_MTOH(out->flow_state.identifier, tlv_0x10->identifier);
    COPY_MTOH(out->flow_state.new_flow, tlv_0x10->new_flow);
    COPY_MTOH(out->flow_state.state_change, tlv_0x10->state_change);

    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_index_t);

        hdr++;
        out->tx_qos_flow_granted_available = true;

        ret = telit_qos_event_report_flow_spec_unpack((uint8_t *)hdr,
                le16toh(hdr->length),
                &(out->tx_qos_flow_granted));
        if (ret != QMI_SDK_ERR_NONE) {
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    if (hdr) {
        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_ip_flow_index_t);

        hdr++;
        out->rx_qos_flow_granted_available = true;

        ret = telit_qos_event_report_flow_spec_unpack((uint8_t *)hdr,
                le16toh(hdr->length),
                &(out->rx_qos_flow_granted));
        if (ret != QMI_SDK_ERR_NONE) {
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    if (hdr) {
        uint16_t tlv_len = le16toh(hdr->length);
        uint16_t data_structure_len = (sizeof(QMI_QOS_generic_ip_filter_index_t) +
                sizeof(QMI_QOS_generic_ip_filter_version_t));
        if (tlv_len < data_structure_len) {
            RLOGE("%s: malformed TLV %u, returned %u, minimum %u",
                    __FUNCTION__, hdr->type_id, tlv_len, data_structure_len);
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }

        hdr++;
        out->tx_qos_filter_available = true;

        ret = telit_qos_event_report_filter_spec_unpack((uint8_t *)hdr,
                tlv_len,
                &(out->tx_qos_filter));
        if (ret != QMI_SDK_ERR_NONE) {
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    if (hdr) {
        uint16_t tlv_len = le16toh(hdr->length);
        uint16_t data_structure_len = (sizeof(QMI_QOS_generic_ip_filter_index_t) +
                sizeof(QMI_QOS_generic_ip_filter_version_t));
        if (tlv_len < data_structure_len) {
            RLOGE("%s: malformed TLV %u, returned %u, minimum %u",
                    __FUNCTION__, hdr->type_id, tlv_len, data_structure_len);
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }

        hdr++;
        out->rx_qos_filter_available = true;

        ret = telit_qos_event_report_filter_spec_unpack((uint8_t *)hdr,
                tlv_len,
                &(out->rx_qos_filter));
        if (ret != QMI_SDK_ERR_NONE) {
            goto end;
        }
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    if (hdr) {
        QMI_QOS_generic_flow_type_t *tlv_0x15;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_flow_type_t);

        hdr++;
        tlv_0x15 = (QMI_QOS_generic_flow_type_t *)hdr;
        out->qos_flow_type_available = true;
        COPY_MTOH(out->qos_flow_type, tlv_0x15->qos_flow_type);
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    if (hdr) {
        QMI_QOS_generic_flow_bearer_id_t *tlv_0x16;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_flow_bearer_id_t);

        hdr++;
        tlv_0x16 = (QMI_QOS_generic_flow_bearer_id_t *)hdr;
        out->bearer_id_available = true;
        COPY_MTOH(out->bearer_id, tlv_0x16->bearer_id);
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    if (hdr) {
        QMI_QOS_generic_flow_seq_num_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_flow_seq_num_t);

        hdr++;
        tlv_0x17 = (QMI_QOS_generic_flow_seq_num_t *)hdr;
        out->flow_control_sequence_number_available = true;
        COPY_MTOH(out->flow_control_sequence_number, tlv_0x17->flow_control_sequence_number);
    }

end:
    return ret;
}

static QMI_SDK_error_t telit_qos_event_report_ind_unpack(uint8_t *rsp, uint16_t len,
        qos_event_report_info_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t hdr_iter = 0;
    uint8_t i = 0;

    payload_len = len;
    msg = rsp;

    memset(out, 0, sizeof(*out));

    /* One or more instances of the same TLV may be present (!) */
    while ((hdr = search_TLV(msg + hdr_iter, 0x10, payload_len - hdr_iter))) {
        QMI_QOS_generic_flow_state_t *tlv_0x10;
        uint16_t tlv_len = le16toh(hdr->length);
        /* Check only on mandatory TLV, since TLVs are nested (!) */
        uint16_t data_structure_len = sizeof(*tlv_0x10);

        if (tlv_len < data_structure_len) {
            RLOGE("%s: malformed upper TLV %u, returned %u, minimum %u",
                    __FUNCTION__, hdr->type_id, tlv_len, data_structure_len);
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }

        hdr++;
        ret = telit_qos_event_report_flow_info_unpack((uint8_t *)hdr,
                tlv_len,
                &(out->flow_info[i]));
        if (ret != QMI_SDK_ERR_NONE) {
            goto end;
        }

        i++;
        if (i == QMI_QOS_FLOW_INFO_MAX_SIZE) {
            RLOGI("%s: QMI_QOS_FLOW_INFO_MAX_SIZE reached, increase the define", __FUNCTION__);
            break;
        }
        hdr_iter += (sizeof(*hdr) + tlv_len);
    }
    out->flow_info_size = i;

end:
    return ret;
}

static void unpack_qos_global_flow_tlv(qos_global_flow_spec_t *out, QMI_QOS_generic_global_flow_spec_t *input,
        bool *nr5g_available)
{
    uint64_t flow_valid_params;

    COPY_MTOH(flow_valid_params, input->flow_valid_params);

    /* QMI_QOS_IP_FLOW_MASK_NONE */
    if (flow_valid_params == 0) {
        return;
    }

    /* QMI_QOS_IP_FLOW_MASK_TRF_CLASS */
    if (flow_valid_params & 0x1) {
        out->ip_flow_traffic_class_available = true;
        COPY_MTOH(out->ip_flow_traffic_class, input->ip_flow_traffic_class);
    }

    /* QMI_QOS_IP_FLOW_MASK_DATA_RATE_MIN_MAX */
    if (flow_valid_params & 0x2) {
        out->ip_flow_data_rate_min_max_available = true;
        COPY_MTOH(out->ip_flow_data_rate_min_max.data_rate_max, input->data_rate_max);
        COPY_MTOH(out->ip_flow_data_rate_min_max.guaranteed_data_rate, input->guaranteed_data_rate);
    }

    /* QMI_QOS_IP_FLOW_MASK_DATA_RATE_TOK_BUCKET */
    if (flow_valid_params & 0x4) {
        out->ip_flow_data_rate_token_bucket_available = true;
        COPY_MTOH(out->ip_flow_data_rate_token_bucket.peak_rate, input->peak_rate);
        COPY_MTOH(out->ip_flow_data_rate_token_bucket.token_rate, input->token_rate);
        COPY_MTOH(out->ip_flow_data_rate_token_bucket.bucket_size, input->bucket_size);
    }

    /* QMI_QOS_IP_FLOW_MASK_LATENCY */
    if (flow_valid_params & 0x8) {
        out->ip_flow_latency_available = true;
        COPY_MTOH(out->ip_flow_latency, input->ip_flow_latency);
    }

    /* QMI_QOS_IP_FLOW_MASK_JITTER */
    if (flow_valid_params & 0x10) {
        out->ip_flow_jitter_available = true;
        COPY_MTOH(out->ip_flow_jitter, input->ip_flow_jitter);
    }

    /* QMI_QOS_IP_FLOW_MASK_PKT_ERR_RATE */
    if (flow_valid_params & 0x20) {
        out->ip_flow_packet_error_rate_available = true;
        COPY_MTOH(out->ip_flow_packet_error_rate.error_rate_multiplier, input->error_rate_multiplier);
        COPY_MTOH(out->ip_flow_packet_error_rate.error_rate_exponent, input->error_rate_exponent);
    }

    /* QMI_QOS_IP_FLOW_MASK_MIN_POLICED_PKT_SIZE */
    if (flow_valid_params & 0x40) {
        out->ip_flow_minimum_policed_packet_size_available = true;
        COPY_MTOH(out->ip_flow_minimum_policed_packet_size, input->ip_flow_minimum_policed_packet_size);
    }

    /* QMI_QOS_IP_FLOW_MASK_MAX_ALLOWED_PKT_SIZE */
    if (flow_valid_params & 0x80) {
        out->ip_flow_maximum_allowed_packet_size_available = true;
        COPY_MTOH(out->ip_flow_maximum_allowed_packet_size, input->ip_flow_maximum_allowed_packet_size);
    }

    /* QMI_QOS_IP_FLOW_MASK_3GPP_RES_BER */
    if (flow_valid_params & 0x100) {
        out->ip_flow_3gpp_residual_bit_error_rate_available = true;
        COPY_MTOH(out->ip_flow_3gpp_residual_bit_error_rate, input->ip_flow_3gpp_residual_bit_error_rate);
    }

    /* QMI_QOS_IP_FLOW_MASK_3GPP_TRF_PRI */
    if (flow_valid_params & 0x200) {
        out->ip_flow_3gpp_traffic_handling_priority_available = true;
        COPY_MTOH(out->ip_flow_3gpp_traffic_handling_priority, input->ip_flow_3gpp_traffic_handling_priority);
    }

    /* QMI_QOS_IP_FLOW_MASK_3GPP_IM_CN_FLAG */
    if (flow_valid_params & 0x1000) {
        out->ip_flow_3gpp_im_cn_flag_available = true;
        COPY_MTOH(out->ip_flow_3gpp_im_cn_flag, input->ip_flow_3gpp_im_cn_flag);
    }

    /* QMI_QOS_IP_FLOW_MASK_3GPP_SIG_IND */
    if (flow_valid_params & 0x2000) {
        out->ip_flow_3gpp_sig_ind_available = true;
        COPY_MTOH(out->ip_flow_3gpp_sig_ind, input->ip_flow_3gpp_sig_ind);
    }

    /* QMI_QOS_IP_FLOW_MASK_LTE_QCI */
    if (flow_valid_params & 0x4000) {
        out->lte_qos_class_identifier_available = true;
        COPY_MTOH(out->lte_qos_class_identifier, input->lte_qos_class_identifier);
    }

    /* QMI_QOS_IP_FLOW_MASK_5G_QCI: QC new way of extending TLVs........... */
    if (flow_valid_params & 0x8000) {
        *nr5g_available = true;
    }
}

static void unpack_qos_global_filter_tlv(qos_global_filter_spec_t *out,
        QMI_QOS_generic_single_filter_spec_t *input)
{
    uint8_t i;
    uint64_t valid_params;

    COPY_MTOH(out->ip_filter_version, input->ip_filter_version);

    COPY_MTOH(valid_params, input->ipv4_valid_params);
    /* QMI_QOS_IPV4_FILTER_MASK_NONE */
    if (valid_params == 0) {
        goto skip_ipv4;
    }

    /* QMI_QOS_IPV4_FILTER_MASK_SRC_ADDR */
    if (valid_params & 0x1) {
        out->ipv4_filter_src_addr_available = true;
        COPY_MTOH(out->ipv4_filter_src_addr.addr, input->ipv4_filter_src_addr);
        COPY_MTOH(out->ipv4_filter_src_addr.mask, input->ipv4_filter_src_mask);
    }

    /* QMI_QOS_IPV4_FILTER_MASK_DEST_ADDR */
    if (valid_params & 0x2) {
        out->ipv4_filter_dest_addr_available = true;
        COPY_MTOH(out->ipv4_filter_dest_addr.addr, input->ipv4_filter_dest_addr);
        COPY_MTOH(out->ipv4_filter_dest_addr.mask, input->ipv4_filter_dest_mask);
    }

    /* QMI_QOS_IPV4_FILTER_MASK_TOS */
    if (valid_params & 0x4) {
        out->ipv4_filter_tos_available = true;
        COPY_MTOH(out->ipv4_filter_tos.val, input->ipv4_tos_val);
        COPY_MTOH(out->ipv4_filter_tos.mask, input->ipv4_tos_mask);
    }

skip_ipv4:
    COPY_MTOH(valid_params, input->ipv6_valid_params);
    /* QMI_QOS_IPV6_FILTER_MASK_NONE */
    if (valid_params == 0) {
        goto skip_ipv6;
    }

    /* QMI_QOS_IPV6_FILTER_MASK_SRC_ADDR */
    if (valid_params & 0x1) {
        out->ipv6_filter_src_addr_available = true;
        COPY_MTOH(out->ipv6_filter_src_addr.prefix_len, input->ipv6_filter_src_prefix_len);
        for (i = 0; i < IPV6_ADDR_SIZE; i++) {
            /* No conversion, since arriving in big endian and needs to be kept in big endian */
            out->ipv6_filter_src_addr.addr[i] = input->ipv6_filter_src_addr[i];
        }
    }

    /* QMI_QOS_IPV6_FILTER_MASK_DEST_ADDR */
    if (valid_params & 0x2) {
        out->ipv6_filter_dest_addr_available = true;
        COPY_MTOH(out->ipv6_filter_dest_addr.prefix_len, input->ipv6_filter_dest_prefix_len);
        for (i = 0; i < IPV6_ADDR_SIZE; i++) {
            /* No conversion, since arriving in big endian and needs to be kept in big endian */
            out->ipv6_filter_dest_addr.addr[i] = input->ipv6_filter_dest_addr[i];
        }
    }

    /* QMI_QOS_IPV6_FILTER_MASK_TRAFFIC_CLASS */
    if (valid_params & 0x4) {
        out->ipv6_filter_traffic_class_available = true;
        out->filter_xport_protocol_available = true;
        COPY_MTOH(out->ipv6_filter_traffic_class.val, input->ipv6_tos_val);
        COPY_MTOH(out->ipv6_filter_traffic_class.mask, input->ipv6_tos_mask);
    }

    /* QMI_QOS_IPV6_FILTER_MASK_FLOW_LABEL */
    if (valid_params & 0x8) {
        out->ipv6_filter_flow_label_available = true;
        out->filter_xport_protocol_available = true;
        COPY_MTOH(out->ipv6_filter_flow_label, input->ipv6_filter_flow_label);
        COPY_MTOH(out->filter_xport_protocol, input->ipv6_filter_xport_protocol);
    }

skip_ipv6:
    COPY_MTOH(valid_params, input->tcp_valid_params);
    /* QMI_QOS_PORT_INFO_FILTER_MASK_NONE */
    if (valid_params == 0) {
        goto skip_tcp_port_info;
    }

    /* QMI_QOS_PORT_INFO_FILTER_MASK_SRC_PORT */
    if (valid_params & 0x1) {
        out->tcp_filter_src_port_available = true;
        COPY_MTOH(out->tcp_filter_src_port.port, input->tcp_src_port);
        COPY_MTOH(out->tcp_filter_src_port.range, input->tcp_src_range);
    }

    /* QMI_QOS_PORT_INFO_FILTER_MASK_DEST_PORT */
    if (valid_params & 0x2) {
        out->tcp_filter_dest_port_available = true;
        COPY_MTOH(out->tcp_filter_dest_port.port, input->tcp_dest_port);
        COPY_MTOH(out->tcp_filter_dest_port.range, input->tcp_dest_range);
    }

skip_tcp_port_info:
    COPY_MTOH(valid_params, input->udp_valid_params);
    /* QMI_QOS_PORT_INFO_FILTER_MASK_NONE */
    if (valid_params == 0) {
        goto skip_udp_port_info;
    }

    /* QMI_QOS_PORT_INFO_FILTER_MASK_SRC_PORT */
    if (valid_params & 0x1) {
        out->udp_filter_src_port_available = true;
        COPY_MTOH(out->udp_filter_src_port.port, input->udp_src_port);
        COPY_MTOH(out->udp_filter_src_port.range, input->udp_src_range);
    }

    /* QMI_QOS_PORT_INFO_FILTER_MASK_DEST_PORT */
    if (valid_params & 0x2) {
        out->udp_filter_dest_port_available = true;
        COPY_MTOH(out->udp_filter_dest_port.port, input->udp_dest_port);
        COPY_MTOH(out->udp_filter_dest_port.range, input->udp_dest_range);
    }

skip_udp_port_info:
    COPY_MTOH(valid_params, input->icmp_valid_params);
    /* QMI_QOS_ICMP_FILTER_MASK_NONE */
    if (valid_params == 0) {
        goto skip_icmp_info;
    }

    /* QMI_QOS_ICMP_FILTER_MASK_MSG_TYPE */
    if (valid_params & 0x1) {
        out->icmp_filter_type_available = true;
        COPY_MTOH(out->icmp_filter_type, input->icmp_type);
    }

    /* QMI_QOS_ICMP_FILTER_MASK_MSG_CODE */
    if (valid_params & 0x2) {
        out->icmp_filter_code_available = true;
        COPY_MTOH(out->icmp_filter_code, input->icmp_code);
    }

skip_icmp_info:
    COPY_MTOH(valid_params, input->esp_valid_params);
    /* QMI_QOS_IPSEC_FILTER_MASK_SPI ESP */
    if (valid_params & 0x1) {
        out->esp_filter_spi_index_available = true;
        COPY_MTOH(out->esp_filter_spi_index, input->esp_spi);
    }

    /* See case 03837523 for reference */
    COPY_MTOH(valid_params, input->ah_valid_params);
    /* QMI_QOS_IPSEC_FILTER_MASK_SPI AH */
    if (valid_params & 0x1) {
        out->ah_filter_spi_index_available = true;
        out->ah_filter_id_available = true;
        out->ah_filter_precedence_available = true;
        COPY_MTOH(out->ah_filter_spi_index, input->ah_spi);
        COPY_MTOH(out->ah_filter_id, input->ah_filter_id);
        COPY_MTOH(out->ah_filter_precedence, input->ah_filter_precedence);
    }
}

static QMI_SDK_error_t telit_qos_global_flow_ind_unpack(uint8_t *rsp, uint16_t len,
        qos_global_flow_info_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_QOS_generic_global_flow_state_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    payload_len = len;
    msg = rsp;

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_global_flow_state_t);

    hdr++;
    tlv_0x01 = (QMI_QOS_generic_global_flow_state_t *)hdr;
    COPY_MTOH(out->flow_state.identifier, tlv_0x01->identifier);
    COPY_MTOH(out->flow_state.new_flow, tlv_0x01->new_flow);
    COPY_MTOH(out->flow_state.state_change, tlv_0x01->state_change);

    hdr = search_TLV(msg, 0x10, payload_len);
    if (hdr) {
        bool nr5g_available = 0;
        QMI_QOS_generic_global_flow_spec_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_global_flow_spec_t);

        hdr++;
        tlv_0x10 = (QMI_QOS_generic_global_flow_spec_t *)hdr;
        out->tx_qos_flow_granted_available = true;
        unpack_qos_global_flow_tlv(&(out->tx_qos_flow_granted), tlv_0x10, &nr5g_available);
        if (nr5g_available) {
            out->nr5g_tx_qos_class_identifier_available = true;
        }
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        bool nr5g_available = 0;
        QMI_QOS_generic_global_flow_spec_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_global_flow_spec_t);

        hdr++;
        tlv_0x11 = (QMI_QOS_generic_global_flow_spec_t *)hdr;
        out->rx_qos_flow_granted_available = true;
        unpack_qos_global_flow_tlv(&(out->rx_qos_flow_granted), tlv_0x11, &nr5g_available);
        if (nr5g_available) {
            out->nr5g_rx_qos_class_identifier_available = true;
        }
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    if (hdr) {
        QMI_QOS_generic_global_filter_spec_t *tlv_0x12;
        uint8_t i;
        uint8_t *buf_position;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_global_filter_spec_t);

        hdr++;
        tlv_0x12 = (QMI_QOS_generic_global_filter_spec_t *)hdr;
        COPY_MTOH(out->tx_qos_filter_size, tlv_0x12->qos_filters_size);
        buf_position = (uint8_t *)hdr;

        if (out->tx_qos_filter_size > QMI_QOS_FILTER_MAX_SIZE) {
            RLOGE("%s: tx_qos_filter_size = %u > QMI_QOS_FILTER_MAX_SIZE, increase the define",
                    __FUNCTION__, out->tx_qos_filter_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        /* Skipping length */
        buf_position += sizeof(tlv_0x12->qos_filters_size);
        for (i = 0; i < out->tx_qos_filter_size; i++) {
            unpack_qos_global_filter_tlv(&(out->tx_qos_filter[i]),
                    (QMI_QOS_generic_single_filter_spec_t *)buf_position);
            buf_position += sizeof(QMI_QOS_generic_single_filter_spec_t);
        }
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    if (hdr) {
        QMI_QOS_generic_global_filter_spec_t *tlv_0x13;
        uint8_t i;
        uint8_t *buf_position;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_global_filter_spec_t);

        hdr++;
        tlv_0x13 = (QMI_QOS_generic_global_filter_spec_t *)hdr;
        COPY_MTOH(out->rx_qos_filter_size, tlv_0x13->qos_filters_size);
        buf_position = (uint8_t *)hdr;

        if (out->rx_qos_filter_size > QMI_QOS_FILTER_MAX_SIZE) {
            RLOGE("%s: rx_qos_filter_size = %u > QMI_QOS_FILTER_MAX_SIZE, increase the define",
                    __FUNCTION__, out->rx_qos_filter_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        /* Skipping length */
        buf_position += sizeof(tlv_0x13->qos_filters_size);
        for (i = 0; i < out->rx_qos_filter_size; i++) {
            unpack_qos_global_filter_tlv(&(out->rx_qos_filter[i]),
                    (QMI_QOS_generic_single_filter_spec_t *)buf_position);
            buf_position += sizeof(QMI_QOS_generic_single_filter_spec_t);
        }
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    if (hdr) {
        QMI_QOS_global_flow_type_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_global_flow_type_t);

        hdr++;
        tlv_0x14 = (QMI_QOS_global_flow_type_t *)hdr;
        out->qos_flow_type_available = true;
        COPY_MTOH(out->qos_flow_type, tlv_0x14->qos_flow_type);
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    if (hdr) {
        QMI_QOS_generic_flow_bearer_id_t *tlv_0x15;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_flow_bearer_id_t);

        hdr++;
        tlv_0x15 = (QMI_QOS_generic_flow_bearer_id_t *)hdr;
        out->bearer_id_available = true;
        COPY_MTOH(out->bearer_id, tlv_0x15->bearer_id);
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    if (hdr) {
        QMI_QOS_generic_flow_seq_num_t *tlv_0x16;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_flow_seq_num_t);

        hdr++;
        tlv_0x16 = (QMI_QOS_generic_flow_seq_num_t *)hdr;
        out->flow_control_sequence_number_available = true;
        COPY_MTOH(out->flow_control_sequence_number, tlv_0x16->flow_control_sequence_number);
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    if (hdr && out->nr5g_tx_qos_class_identifier_available) {
        QMI_QOS_generic_nr5g_qci_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_nr5g_qci_t);

        hdr++;
        tlv_0x17 = (QMI_QOS_generic_nr5g_qci_t *)hdr;
        COPY_MTOH(out->nr5g_tx_qos_class_identifier, tlv_0x17->qci);
    }

    hdr = search_TLV(msg, 0x18, payload_len);
    if (hdr && out->nr5g_rx_qos_class_identifier_available) {
        QMI_QOS_generic_nr5g_qci_t *tlv_0x18;

        CHECK_UNPACK_TLV_SIZE(QMI_QOS_generic_nr5g_qci_t);

        hdr++;
        tlv_0x18 = (QMI_QOS_generic_nr5g_qci_t *)hdr;
        COPY_MTOH(out->nr5g_rx_qos_class_identifier, tlv_0x18->qci);
    }

end:
    return ret;
}

static QMI_SDK_error_t telit_qos_primary_event_ind_unpack(uint8_t *rsp, uint16_t len,
        qos_primary_event_info_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_QOS_PRIMARY_QOS_EVENT_indication_0x01_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    payload_len = len;
    msg = rsp;

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_QOS_PRIMARY_QOS_EVENT_indication_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_QOS_PRIMARY_QOS_EVENT_indication_0x01_t *)hdr;
    COPY_MTOH(out->event, tlv_0x01->event);

end:
    return ret;
}

QMI_SDK_error_t telit_qos_indication_handler(uint16_t msg_id,
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
        case QMI_QOS_EVENT_REPORT_IND:
            if (out_len < sizeof(qos_event_report_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(qos_event_report_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_qos_event_report_ind_unpack(msg, payload_len, (qos_event_report_info_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_QOS_PRIMARY_QOS_EVENT_IND:
            if (out_len < sizeof(qos_primary_event_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(qos_primary_event_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_qos_primary_event_ind_unpack(msg, payload_len, (qos_primary_event_info_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_QOS_GLOBAL_QOS_FLOW_IND:
            if (out_len < sizeof(qos_global_flow_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(qos_global_flow_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_qos_global_flow_ind_unpack(msg, payload_len, (qos_global_flow_info_t *)out);
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
