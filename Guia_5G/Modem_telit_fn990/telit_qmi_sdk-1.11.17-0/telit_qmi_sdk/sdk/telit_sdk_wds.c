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
#include "telit_sdk_wds.h"
#include "telit_sdk_gms.h"
#include "telit_sdk_private_wds.h"

/*****************************************************************************
 * QMI_WDS_SET_EVENT_REPORT (1 - 0x0001)
 */

QMI_SDK_error_t telit_wds_set_event_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        wds_set_event_report_info_t *input)
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

    req_ctx->msg_id = QMI_WDS_SET_EVENT_REPORT;
    req_ctx->svc_id = QMI_SVC_WDS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->transfer_stats_available) {
        QMI_WDS_SET_EVENT_REPORT_request_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_WDS_SET_EVENT_REPORT_request_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->stats_period, input->transfer_stats.stats_period);
        COPY_HTOM(tlv_0x11->stats_mask, input->transfer_stats.stats_mask);

        buf_position += tlv_sz;
    }

    if (input->report_data_bearer_tech != QMI_IND_ACTION_NONE) {
        QMI_WDS_SET_EVENT_REPORT_request_0x12_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_WDS_SET_EVENT_REPORT_request_0x12_t *)buf_position;
        tlv_0x12->report_data_bearer_tech = input->report_data_bearer_tech;

        buf_position += tlv_sz;
    }

    if (input->report_dormancy_status != QMI_IND_ACTION_NONE) {
        QMI_WDS_SET_EVENT_REPORT_request_0x13_t *tlv_0x13;

        tlv_sz = sizeof(*tlv_0x13);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x13 = (QMI_WDS_SET_EVENT_REPORT_request_0x13_t *)buf_position;
        tlv_0x13->report_dormancy_status = input->report_dormancy_status;

        buf_position += tlv_sz;
    }

    if (input->report_mip_status != QMI_IND_ACTION_NONE) {
        QMI_WDS_SET_EVENT_REPORT_request_0x14_t *tlv_0x14;

        tlv_sz = sizeof(*tlv_0x14);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x14;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x14 = (QMI_WDS_SET_EVENT_REPORT_request_0x14_t *)buf_position;
        tlv_0x14->report_mip_status = input->report_mip_status;

        buf_position += tlv_sz;
    }

    if (input->report_current_data_bearer_tech != QMI_IND_ACTION_NONE) {
        QMI_WDS_SET_EVENT_REPORT_request_0x15_t *tlv_0x15;

        tlv_sz = sizeof(*tlv_0x15);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x15;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x15 = (QMI_WDS_SET_EVENT_REPORT_request_0x15_t *)buf_position;
        tlv_0x15->report_current_data_bearer_tech = input->report_current_data_bearer_tech;

        buf_position += tlv_sz;
    }

    if (input->report_data_call_status_change != QMI_IND_ACTION_NONE) {
        QMI_WDS_SET_EVENT_REPORT_request_0x17_t *tlv_0x17;

        tlv_sz = sizeof(*tlv_0x17);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x17;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x17 = (QMI_WDS_SET_EVENT_REPORT_request_0x17_t *)buf_position;
        tlv_0x17->report_data_call_status_change = input->report_data_call_status_change;

        buf_position += tlv_sz;
    }

    if (input->report_data_system_status != QMI_IND_ACTION_NONE) {
        QMI_WDS_SET_EVENT_REPORT_request_0x1A_t *tlv_0x1A;

        tlv_sz = sizeof(*tlv_0x1A);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1A;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1A = (QMI_WDS_SET_EVENT_REPORT_request_0x1A_t *)buf_position;
        tlv_0x1A->report_data_system_status = input->report_data_system_status;

        buf_position += tlv_sz;
    }

    if (input->report_data_bearer_tech_ext != QMI_IND_ACTION_NONE) {
        QMI_WDS_SET_EVENT_REPORT_request_0x1E_t *tlv_0x1E;

        tlv_sz = sizeof(*tlv_0x1E);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1E;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1E = (QMI_WDS_SET_EVENT_REPORT_request_0x1E_t *)buf_position;
        tlv_0x1E->report_data_bearer_tech_ext = input->report_data_bearer_tech_ext;
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wds_set_event_report_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_WDS_INDICATION_REGISTER (3 - 0x0003)
 */

QMI_SDK_error_t telit_wds_indication_register_pack(pack_qmi_t *req_ctx,
        void *req,
        uint16_t *len,
        wds_indication_register_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    uint32_t hdr_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_WDS_INDICATION_REGISTER;
    req_ctx->svc_id = QMI_SVC_WDS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->report_apn_param_change_info != QMI_IND_ACTION_NONE) {
        QMI_WDS_INDICATION_REGISTER_request_0x1E_t *tlv_0x1E;
        uint16_t tlv_sz = sizeof(*tlv_0x1E);

        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1E;
        hdr->length = htole16(tlv_sz);
        buf_position += hdr_sz;

        tlv_0x1E = (QMI_WDS_INDICATION_REGISTER_request_0x1E_t *)buf_position;
        tlv_0x1E->report_apn_change_info = input->report_apn_param_change_info;
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wds_indication_register_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_WDS_START_NETWORK_INTERFACE (32 - 0x0020)
 */

QMI_SDK_error_t telit_wds_start_network_interface_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_start_network_info_t *input)
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

    req_ctx->msg_id = QMI_WDS_START_NETWORK_INTERFACE;
    req_ctx->svc_id = QMI_SVC_WDS;
    req_ctx->timeout = QMI_TIMEOUT_BIG;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->apn_len) {
        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x14;
        hdr->length = htole16(input->apn_len);

        /* TBD: check if buffer size allows adding the TLV */
        buf_position += hdr_sz;

        memcpy(buf_position, input->apn, input->apn_len);
        payload_len += hdr_sz + input->apn_len;

        buf_position += input->apn_len;
    }

    if (input->authentication_pref_available) {
        QMI_WDS_generic_authentication_pref_t *tlv_0x16;

        tlv_sz = sizeof(*tlv_0x16);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x16;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x16 = (QMI_WDS_generic_authentication_pref_t *)buf_position;
        COPY_HTOM(tlv_0x16->authentication_pref, input->authentication_pref);

        buf_position += tlv_sz;
    }

    if (input->username_len) {
        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x17;
        hdr->length = htole16(input->username_len);

        /* TBD: check if buffer size allows adding the TLV */
        buf_position += hdr_sz;

        memcpy(buf_position, input->username, input->username_len);
        payload_len += hdr_sz + input->username_len;

        buf_position += input->username_len;
    }

    if (input->password_len) {
        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x18;
        hdr->length = htole16(input->password_len);

        /* TBD: check if buffer size allows adding the TLV */
        buf_position += hdr_sz;

        memcpy(buf_position, input->password, input->password_len);
        payload_len += hdr_sz + input->password_len;

        buf_position += input->password_len;
    }

    if (input->ip_family_preference_available) {
        QMI_WDS_generic_ip_family_t *tlv_0x19;

        tlv_sz = sizeof(*tlv_0x19);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x19;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x19 = (QMI_WDS_generic_ip_family_t *)buf_position;
        COPY_HTOM(tlv_0x19->ip_family, input->ip_family_preference);

        buf_position += tlv_sz;
    }

    if (input->technology_preference_available) {
        QMI_WDS_START_NETWORK_INTERFACE_request_0x30_t *tlv_0x30;

        tlv_sz = sizeof(*tlv_0x30);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x30;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x30 = (QMI_WDS_START_NETWORK_INTERFACE_request_0x30_t *)buf_position;
        COPY_HTOM(tlv_0x30->technology_preference, input->technology_preference);

        buf_position += tlv_sz;
    }

    if (input->profile_index_3gpp_available) {
        QMI_WDS_generic_profile_index_t *tlv_0x31;

        tlv_sz = sizeof(*tlv_0x31);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x31;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x31 = (QMI_WDS_generic_profile_index_t *)buf_position;
        COPY_HTOM(tlv_0x31->profile_index, input->profile_index_3gpp);

        buf_position += tlv_sz;
    }

    if (input->disallow_in_roaming_available) {
        QMI_WDS_START_NETWORK_INTERFACE_request_0x39_t *tlv_0x39;

        tlv_sz = sizeof(*tlv_0x39);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x39;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x39 = (QMI_WDS_START_NETWORK_INTERFACE_request_0x39_t *)buf_position;
        COPY_HTOM(tlv_0x39->disallow_in_roaming, input->disallow_in_roaming);
    }

    if (input->bring_up_by_apn_name_available) {
        QMI_WDS_START_NETWORK_INTERFACE_request_0x3C_t *tlv_0x3C;

        tlv_sz = sizeof(*tlv_0x3C);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x3C;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x3C = (QMI_WDS_START_NETWORK_INTERFACE_request_0x3C_t *)buf_position;
        COPY_HTOM(tlv_0x3C->bring_up_by_apn_name, input->bring_up_by_apn_name);
    }

    ret = QMI_SDK_ERR_NONE;

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wds_start_network_interface_unpack(uint8_t *rsp, uint16_t len,
        wds_start_network_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    /* Not exiting for protocol errors, since we want to collect the protocol errors */
    if (ret != QMI_SDK_ERR_NONE && ret < QMI_SDK_ERR_ERR_QMI_OFFSET) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    if (ret == QMI_SDK_ERR_NONE) {
        QMI_WDS_generic_pkt_data_handle_t *tlv_0x01;

        FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_pkt_data_handle_t);

        hdr++;
        tlv_0x01 = (QMI_WDS_generic_pkt_data_handle_t *)hdr;
        out->pkt_data_handle_available = true;
        COPY_MTOH(out->pkt_data_handle, tlv_0x01->pkt_data_handle);
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_WDS_generic_call_end_reason_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_call_end_reason_t);

        hdr++;
        tlv_0x10 = (QMI_WDS_generic_call_end_reason_t *)hdr;
        out->call_end_reason_available = true;
        COPY_MTOH(out->call_end_reason, tlv_0x10->call_end_reason);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_WDS_generic_verbose_call_end_reason_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_verbose_call_end_reason_t);

        hdr++;
        tlv_0x11 = (QMI_WDS_generic_verbose_call_end_reason_t *)hdr;
        out->verbose_call_end_reason_available = true;
        COPY_MTOH(out->verbose_call_end_reason.call_end_reason_type, tlv_0x11->call_end_reason_type);
        COPY_MTOH(out->verbose_call_end_reason.call_end_reason, tlv_0x11->call_end_reason);
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_WDS_generic_mux_id_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_mux_id_t);

        hdr++;
        tlv_0x13 = (QMI_WDS_generic_mux_id_t *)hdr;
        out->mux_id_available = true;
        COPY_MTOH(out->mux_id, tlv_0x13->mux_id);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_WDS_STOP_NETWORK_INTERFACE (33 - 0x0021)
 */

QMI_SDK_error_t telit_wds_stop_network_interface_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_stop_network_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WDS_generic_pkt_data_handle_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_WDS_STOP_NETWORK_INTERFACE;
    req_ctx->svc_id = QMI_SVC_WDS;
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

    tlv_0x01 = (QMI_WDS_generic_pkt_data_handle_t *)buf_position;
    COPY_HTOM(tlv_0x01->pkt_data_handle, input->pkt_data_handle);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wds_stop_network_interface_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_WDS_GET_PKT_SRVC_STATUS (34 - 0x0022)
 */

QMI_SDK_error_t telit_wds_get_pkt_srvc_status_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_WDS_GET_PKT_SRVC_STATUS,
            QMI_SVC_WDS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_wds_get_pkt_srvc_status_unpack(uint8_t *rsp, uint16_t len,
        wds_get_packet_service_status_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_WDS_GET_PKT_SRVC_STATUS_response_0x01_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_WDS_GET_PKT_SRVC_STATUS_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_WDS_GET_PKT_SRVC_STATUS_response_0x01_t *)hdr;
    COPY_MTOH(out->conn_status, tlv_0x01->connection_status);

end:
    return ret;
}

/*****************************************************************************
 * QMI_WDS_GET_PKT_STATISTICS (36 - 0x0024)
 */

QMI_SDK_error_t telit_wds_get_pkt_statistics_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_get_packet_statistics_mask_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WDS_GET_PKT_STATISTICS_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_WDS_GET_PKT_STATISTICS;
    req_ctx->svc_id = QMI_SVC_WDS;
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

    tlv_0x01 = (QMI_WDS_GET_PKT_STATISTICS_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->stats_mask, input->stats_mask);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wds_get_pkt_statistics_unpack(uint8_t *rsp, uint16_t len,
        wds_get_packet_statistics_t *out)
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
        QMI_WDS_generic_packet_stats_value_32_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_32_t);

        hdr++;
        tlv_0x10 = (QMI_WDS_generic_packet_stats_value_32_t *)hdr;
        COPY_MTOH(out->tx_out_count, tlv_0x10->value_32);
        out->tx_out_count_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_32_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_32_t);

        hdr++;
        tlv_0x11 = (QMI_WDS_generic_packet_stats_value_32_t *)hdr;
        COPY_MTOH(out->rx_out_count, tlv_0x11->value_32);
        out->rx_out_count_available = true;
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_32_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_32_t);

        hdr++;
        tlv_0x12 = (QMI_WDS_generic_packet_stats_value_32_t *)hdr;
        COPY_MTOH(out->tx_err_count, tlv_0x12->value_32);
        out->tx_err_count_available = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_32_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_32_t);

        hdr++;
        tlv_0x13 = (QMI_WDS_generic_packet_stats_value_32_t *)hdr;
        COPY_MTOH(out->rx_err_count, tlv_0x13->value_32);
        out->rx_err_count_available = true;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_32_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_32_t);

        hdr++;
        tlv_0x14 = (QMI_WDS_generic_packet_stats_value_32_t *)hdr;
        COPY_MTOH(out->tx_ofl_count, tlv_0x14->value_32);
        out->tx_ofl_count_available = true;
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    /* TLV 0x15 is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_32_t *tlv_0x15;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_32_t);

        hdr++;
        tlv_0x15 = (QMI_WDS_generic_packet_stats_value_32_t *)hdr;
        COPY_MTOH(out->rx_ofl_count, tlv_0x15->value_32);
        out->rx_ofl_count_available = true;
    }

    hdr = search_TLV(msg, 0x19, payload_len);
    /* TLV 0x19 is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_64_t *tlv_0x19;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_64_t);

        hdr++;
        tlv_0x19 = (QMI_WDS_generic_packet_stats_value_64_t *)hdr;
        COPY_MTOH(out->tx_ok_bytes_count, tlv_0x19->value_64);
        out->tx_ok_bytes_count_available = true;
    }

    hdr = search_TLV(msg, 0x1A, payload_len);
    /* TLV 0x1A is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_64_t *tlv_0x1A;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_64_t);

        hdr++;
        tlv_0x1A = (QMI_WDS_generic_packet_stats_value_64_t *)hdr;
        COPY_MTOH(out->rx_ok_bytes_count, tlv_0x1A->value_64);
        out->rx_ok_bytes_count_available = true;
    }

    hdr = search_TLV(msg, 0x1B, payload_len);
    /* TLV 0x1B is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_64_t *tlv_0x1B;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_64_t);

        hdr++;
        tlv_0x1B = (QMI_WDS_generic_packet_stats_value_64_t *)hdr;
        COPY_MTOH(out->last_call_tx_ok_bytes_count, tlv_0x1B->value_64);
        out->last_call_tx_ok_bytes_count_available = true;
    }

    hdr = search_TLV(msg, 0x1C, payload_len);
    /* TLV 0x1C is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_64_t *tlv_0x1C;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_64_t);

        hdr++;
        tlv_0x1C = (QMI_WDS_generic_packet_stats_value_64_t *)hdr;
        COPY_MTOH(out->last_call_rx_ok_bytes_count, tlv_0x1C->value_64);
        out->last_call_rx_ok_bytes_count_available = true;
    }

    hdr = search_TLV(msg, 0x1D, payload_len);
    /* TLV 0x1D is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_32_t *tlv_0x1D;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_32_t);

        hdr++;
        tlv_0x1D = (QMI_WDS_generic_packet_stats_value_32_t *)hdr;
        COPY_MTOH(out->tx_dropped_count, tlv_0x1D->value_32);
        out->tx_dropped_count_available = true;
    }

    hdr = search_TLV(msg, 0x1E, payload_len);
    /* TLV 0x1E is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_32_t *tlv_0x1E;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_32_t);

        hdr++;
        tlv_0x1E = (QMI_WDS_generic_packet_stats_value_32_t *)hdr;
        COPY_MTOH(out->rx_dropped_count, tlv_0x1E->value_32);
        out->rx_dropped_count_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_WDS_CREATE_PROFILE (39 - 0x0027)
 */

QMI_SDK_error_t telit_wds_generic_modify_profile_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_profile_t *input,
        uint32_t msg_type)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    uint32_t hdr_sz;
    uint16_t tlv_sz;
    uint8_t type_id_offset;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    switch (msg_type) {
        case QMI_WDS_CREATE_PROFILE:
        case QMI_WDS_MODIFY_PROFILE_SETTINGS:
            type_id_offset = 0;
            req_ctx->svc_id = QMI_SVC_WDS;
            break;
        case QMI_GMS_WDS_CREATE_PROFILE:
            type_id_offset = 1;
            req_ctx->svc_id = QMI_SVC_GMS;
            break;
        default:
            RLOGE("%s: invalid arg, msg_type %u", __FUNCTION__, msg_type);
            goto end;
    }

    req_ctx->msg_id = msg_type;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;

    if (msg_type == QMI_WDS_CREATE_PROFILE || msg_type == QMI_GMS_WDS_CREATE_PROFILE) {
        QMI_WDS_generic_profile_type_0x01_t *tlv_0x01;

        tlv_sz = sizeof(*tlv_0x01);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x01 = (QMI_WDS_generic_profile_type_0x01_t *)buf_position;
        COPY_HTOM(tlv_0x01->profile_type, input->profile_id.type);

        buf_position += tlv_sz;
    } else {
        QMI_WDS_generic_profile_details_0x01_t *tlv_0x01;

        tlv_sz = sizeof(*tlv_0x01);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x01 = (QMI_WDS_generic_profile_details_0x01_t *)buf_position;
        COPY_HTOM(tlv_0x01->profile_type, input->profile_id.type);
        COPY_HTOM(tlv_0x01->profile_index, input->profile_id.index);

        buf_position += tlv_sz;
    }

    if (msg_type == QMI_GMS_WDS_CREATE_PROFILE) {
        if (input->profile_id.index) {
            QMI_WDS_generic_profile_index_t *tlv_0x10;

            tlv_sz = sizeof(*tlv_0x10);
            CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

            hdr = (QMI_raw_content_header_t *)buf_position;
            hdr->type_id = 0x10;
            hdr->length = htole16(tlv_sz);

            buf_position += hdr_sz;

            tlv_0x10 = (QMI_WDS_generic_profile_index_t *)buf_position;
            COPY_HTOM(tlv_0x10->profile_index, input->profile_id.index);

            buf_position += tlv_sz;
        }
    }

    if (input->profile_settings.profile_name_len) {
        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10 + type_id_offset;
        hdr->length = htole16(input->profile_settings.profile_name_len);

        buf_position += hdr_sz;

        memcpy(buf_position, input->profile_settings.profile_name, input->profile_settings.profile_name_len);
        payload_len += hdr_sz + input->profile_settings.profile_name_len;

        buf_position += input->profile_settings.profile_name_len;
    }

    if (input->profile_settings.pdp_type_available) {
        QMI_WDS_generic_profile_details_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_WDS_generic_profile_details_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->pdp_type, input->profile_settings.pdp_type);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.pdp_hdr_comp_type_available) {
        QMI_WDS_generic_profile_details_0x12_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_WDS_generic_profile_details_0x12_t *)buf_position;
        COPY_HTOM(tlv_0x12->pdp_hdr_comp_type, input->profile_settings.pdp_hdr_comp_type);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.pdp_data_comp_type_available) {
        QMI_WDS_generic_profile_details_0x13_t *tlv_0x13;

        tlv_sz = sizeof(*tlv_0x13);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x13 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x13 = (QMI_WDS_generic_profile_details_0x13_t *)buf_position;
        COPY_HTOM(tlv_0x13->pdp_data_comp_type, input->profile_settings.pdp_data_comp_type);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.apn_len) {
        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x14 + type_id_offset;
        hdr->length = htole16(input->profile_settings.apn_len);

        buf_position += hdr_sz;

        memcpy(buf_position, input->profile_settings.apn, input->profile_settings.apn_len);
        payload_len += hdr_sz + input->profile_settings.apn_len;

        buf_position += input->profile_settings.apn_len;
    }

    if (input->profile_settings.pri_dns_ipv4_addr_pref_available) {
        QMI_WDS_generic_profile_details_0x15_t *tlv_0x15;

        tlv_sz = sizeof(*tlv_0x15);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x15 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x15 = (QMI_WDS_generic_profile_details_0x15_t *)buf_position;
        tlv_0x15->pri_dns_ipv4_addr_pref = input->profile_settings.pri_dns_ipv4_addr_pref;  /* NOTE: should be in network order */

        buf_position += tlv_sz;
    }

    if (input->profile_settings.sec_dns_ipv4_addr_pref_available) {
        QMI_WDS_generic_profile_details_0x16_t *tlv_0x16;

        tlv_sz = sizeof(*tlv_0x16);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x16 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x16 = (QMI_WDS_generic_profile_details_0x16_t *)buf_position;
        tlv_0x16->sec_dns_ipv4_addr_pref = input->profile_settings.sec_dns_ipv4_addr_pref;  /* NOTE: should be in network order */

        buf_position += tlv_sz;
    }

    if (input->profile_settings.umts_req_qos_available) {
        QMI_WDS_generic_umts_qos_t *tlv_0x17;

        tlv_sz = sizeof(*tlv_0x17);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x17 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x17 = (QMI_WDS_generic_umts_qos_t *)buf_position;
        COPY_HTOM(tlv_0x17->traffic_class, input->profile_settings.umts_req_qos.traffic_class);
        COPY_HTOM(tlv_0x17->max_uplink_bitrate, input->profile_settings.umts_req_qos.max_uplink_bitrate);
        COPY_HTOM(tlv_0x17->max_downlink_bitrate, input->profile_settings.umts_req_qos.max_downlink_bitrate);
        COPY_HTOM(tlv_0x17->guaranteed_uplink_bitrate,
                input->profile_settings.umts_req_qos.guaranteed_uplink_bitrate);
        COPY_HTOM(tlv_0x17->guaranteed_downlink_bitrate,
                input->profile_settings.umts_req_qos.guaranteed_downlink_bitrate);
        COPY_HTOM(tlv_0x17->qos_delivery_order, input->profile_settings.umts_req_qos.qos_delivery_order);
        COPY_HTOM(tlv_0x17->max_sdu_size, input->profile_settings.umts_req_qos.max_sdu_size);
        COPY_HTOM(tlv_0x17->sdu_error_ratio, input->profile_settings.umts_req_qos.sdu_error_ratio);
        COPY_HTOM(tlv_0x17->residual_bit_error_ratio,
                input->profile_settings.umts_req_qos.residual_bit_error_ratio);
        COPY_HTOM(tlv_0x17->delivery_erroneous_SDUs,
                input->profile_settings.umts_req_qos.delivery_erroneous_SDUs);
        COPY_HTOM(tlv_0x17->transfer_delay, input->profile_settings.umts_req_qos.transfer_delay);
        COPY_HTOM(tlv_0x17->traffic_handling_priority,
                input->profile_settings.umts_req_qos.traffic_handling_priority);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.umts_min_qos_available) {
        QMI_WDS_generic_umts_qos_t *tlv_0x18;

        tlv_sz = sizeof(*tlv_0x18);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x18 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x18 = (QMI_WDS_generic_umts_qos_t *)buf_position;
        COPY_HTOM(tlv_0x18->traffic_class, input->profile_settings.umts_min_qos.traffic_class);
        COPY_HTOM(tlv_0x18->max_uplink_bitrate, input->profile_settings.umts_min_qos.max_uplink_bitrate);
        COPY_HTOM(tlv_0x18->max_downlink_bitrate, input->profile_settings.umts_min_qos.max_downlink_bitrate);
        COPY_HTOM(tlv_0x18->guaranteed_uplink_bitrate,
                input->profile_settings.umts_min_qos.guaranteed_uplink_bitrate);
        COPY_HTOM(tlv_0x18->guaranteed_downlink_bitrate,
                input->profile_settings.umts_min_qos.guaranteed_downlink_bitrate);
        COPY_HTOM(tlv_0x18->qos_delivery_order, input->profile_settings.umts_min_qos.qos_delivery_order);
        COPY_HTOM(tlv_0x18->max_sdu_size, input->profile_settings.umts_min_qos.max_sdu_size);
        COPY_HTOM(tlv_0x18->sdu_error_ratio, input->profile_settings.umts_min_qos.sdu_error_ratio);
        COPY_HTOM(tlv_0x18->residual_bit_error_ratio,
                input->profile_settings.umts_min_qos.residual_bit_error_ratio);
        COPY_HTOM(tlv_0x18->delivery_erroneous_SDUs,
                input->profile_settings.umts_min_qos.delivery_erroneous_SDUs);
        COPY_HTOM(tlv_0x18->transfer_delay, input->profile_settings.umts_min_qos.transfer_delay);
        COPY_HTOM(tlv_0x18->traffic_handling_priority,
                input->profile_settings.umts_min_qos.traffic_handling_priority);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.gprs_req_qos_available) {
        QMI_WDS_generic_gprs_qos_t *tlv_0x19;

        tlv_sz = sizeof(*tlv_0x19);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x19 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x19 = (QMI_WDS_generic_gprs_qos_t *)buf_position;
        COPY_HTOM(tlv_0x19->precedence_class, input->profile_settings.gprs_req_qos.precedence_class);
        COPY_HTOM(tlv_0x19->delay_class, input->profile_settings.gprs_req_qos.delay_class);
        COPY_HTOM(tlv_0x19->reliability_class, input->profile_settings.gprs_req_qos.reliability_class);
        COPY_HTOM(tlv_0x19->peak_throughput_class,
                input->profile_settings.gprs_req_qos.peak_throughput_class);
        COPY_HTOM(tlv_0x19->mean_throughput_class,
                input->profile_settings.gprs_req_qos.mean_throughput_class);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.gprs_min_qos_available) {
        QMI_WDS_generic_gprs_qos_t *tlv_0x1A;

        tlv_sz = sizeof(*tlv_0x1A);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1A + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1A = (QMI_WDS_generic_gprs_qos_t *)buf_position;
        COPY_HTOM(tlv_0x1A->precedence_class, input->profile_settings.gprs_min_qos.precedence_class);
        COPY_HTOM(tlv_0x1A->delay_class, input->profile_settings.gprs_min_qos.delay_class);
        COPY_HTOM(tlv_0x1A->reliability_class, input->profile_settings.gprs_min_qos.reliability_class);
        COPY_HTOM(tlv_0x1A->peak_throughput_class,
                input->profile_settings.gprs_min_qos.peak_throughput_class);
        COPY_HTOM(tlv_0x1A->mean_throughput_class,
                input->profile_settings.gprs_min_qos.mean_throughput_class);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.username_len) {
        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1B + type_id_offset;
        hdr->length = htole16(input->profile_settings.username_len);

        buf_position += hdr_sz;

        memcpy(buf_position, input->profile_settings.username, input->profile_settings.username_len);
        payload_len += hdr_sz + input->profile_settings.username_len;

        buf_position += input->profile_settings.username_len;
    }

    if (input->profile_settings.password_len) {
        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1C + type_id_offset;
        hdr->length = htole16(input->profile_settings.password_len);

        buf_position += hdr_sz;

        memcpy(buf_position, input->profile_settings.password, input->profile_settings.password_len);
        payload_len += hdr_sz + input->profile_settings.password_len;

        buf_position += input->profile_settings.password_len;
    }

    if (input->profile_settings.authentication_pref_available) {
        QMI_WDS_generic_authentication_pref_t *tlv_0x1D;

        tlv_sz = sizeof(*tlv_0x1D);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1D + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1D = (QMI_WDS_generic_authentication_pref_t *)buf_position;
        COPY_HTOM(tlv_0x1D->authentication_pref, input->profile_settings.authentication_pref);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.ipv4_addr_pref_available) {
        QMI_WDS_generic_ipv4_addr_t *tlv_0x1E;

        tlv_sz = sizeof(*tlv_0x1E);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1E + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1E = (QMI_WDS_generic_ipv4_addr_t *)buf_position;
        tlv_0x1E->ipv4_addr = input->profile_settings.ipv4_addr_pref;  /* NOTE: should be in network order */

        buf_position += tlv_sz;
    }

    if (input->profile_settings.pcscf_addr_using_pco) {
        QMI_WDS_generic_pcscf_addr_with_pco_t *tlv_0x1F;

        tlv_sz = sizeof(*tlv_0x1F);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1F + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1F = (QMI_WDS_generic_pcscf_addr_with_pco_t *)buf_position;
        COPY_HTOM(tlv_0x1F->pcscf_addr_using_pco, input->profile_settings.pcscf_addr_using_pco);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.pdp_access_control_flag_available) {
        QMI_WDS_generic_profile_details_0x20_t *tlv_0x20;

        tlv_sz = sizeof(*tlv_0x20);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x20 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x20 = (QMI_WDS_generic_profile_details_0x20_t *)buf_position;
        COPY_HTOM(tlv_0x20->pdp_access_control_flag, input->profile_settings.pdp_access_control_flag);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.pcscf_addr_using_dhcp) {
        QMI_WDS_generic_profile_details_0x21_t *tlv_0x21;

        tlv_sz = sizeof(*tlv_0x21);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x21 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x21 = (QMI_WDS_generic_profile_details_0x21_t *)buf_position;
        COPY_HTOM(tlv_0x21->pcscf_addr_using_dhcp, input->profile_settings.pcscf_addr_using_dhcp);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.im_cn_flag) {
        QMI_WDS_generic_im_cn_flag_t *tlv_0x22;

        tlv_sz = sizeof(*tlv_0x22);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x22 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x22 = (QMI_WDS_generic_im_cn_flag_t *)buf_position;
        COPY_HTOM(tlv_0x22->im_cn_flag, input->profile_settings.im_cn_flag);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.tft_id1_available) {
        QMI_WDS_generic_tft_parameters_t *tlv_0x23;

        tlv_sz = sizeof(*tlv_0x23);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x23 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x23 = (QMI_WDS_generic_tft_parameters_t *)buf_position;
        COPY_HTOM(tlv_0x23->filter_id, input->profile_settings.tft_id1.filter_id);
        COPY_HTOM(tlv_0x23->eval_id, input->profile_settings.tft_id1.eval_id);
        COPY_HTOM(tlv_0x23->ip_version, input->profile_settings.tft_id1.ip_version);
        memcpy(tlv_0x23->source_ip, input->profile_settings.tft_id1.source_ip, 16);  /* NOTE: should be in network order */
        COPY_HTOM(tlv_0x23->source_ip_mask, input->profile_settings.tft_id1.source_ip_mask);
        COPY_HTOM(tlv_0x23->next_header, input->profile_settings.tft_id1.next_header);
        COPY_HTOM(tlv_0x23->dest_port_range_start, input->profile_settings.tft_id1.dest_port_range_start);
        COPY_HTOM(tlv_0x23->dest_port_range_end, input->profile_settings.tft_id1.dest_port_range_end);
        COPY_HTOM(tlv_0x23->src_port_range_start, input->profile_settings.tft_id1.src_port_range_start);
        COPY_HTOM(tlv_0x23->src_port_range_end, input->profile_settings.tft_id1.src_port_range_end);
        COPY_HTOM(tlv_0x23->ipsec_spi, input->profile_settings.tft_id1.ipsec_spi);
        COPY_HTOM(tlv_0x23->tos_mask, input->profile_settings.tft_id1.tos_mask);
        COPY_HTOM(tlv_0x23->flow_label, input->profile_settings.tft_id1.flow_label);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.tft_id2_available) {
        QMI_WDS_generic_tft_parameters_t *tlv_0x24;

        tlv_sz = sizeof(*tlv_0x24);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x24 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x24 = (QMI_WDS_generic_tft_parameters_t *)buf_position;
        COPY_HTOM(tlv_0x24->filter_id, input->profile_settings.tft_id2.filter_id);
        COPY_HTOM(tlv_0x24->eval_id, input->profile_settings.tft_id2.eval_id);
        COPY_HTOM(tlv_0x24->ip_version, input->profile_settings.tft_id2.ip_version);
        memcpy(tlv_0x24->source_ip, input->profile_settings.tft_id2.source_ip, 16);  /* NOTE: should be in network order */
        COPY_HTOM(tlv_0x24->source_ip_mask, input->profile_settings.tft_id2.source_ip_mask);
        COPY_HTOM(tlv_0x24->next_header, input->profile_settings.tft_id2.next_header);
        COPY_HTOM(tlv_0x24->dest_port_range_start, input->profile_settings.tft_id2.dest_port_range_start);
        COPY_HTOM(tlv_0x24->dest_port_range_end, input->profile_settings.tft_id2.dest_port_range_end);
        COPY_HTOM(tlv_0x24->src_port_range_start, input->profile_settings.tft_id2.src_port_range_start);
        COPY_HTOM(tlv_0x24->src_port_range_end, input->profile_settings.tft_id2.src_port_range_end);
        COPY_HTOM(tlv_0x24->ipsec_spi, input->profile_settings.tft_id2.ipsec_spi);
        COPY_HTOM(tlv_0x24->tos_mask, input->profile_settings.tft_id2.tos_mask);
        COPY_HTOM(tlv_0x24->flow_label, input->profile_settings.tft_id2.flow_label);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.pdp_context_available) {
        QMI_WDS_generic_profile_details_0x25_t *tlv_0x25;

        tlv_sz = sizeof(*tlv_0x25);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x25 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x25 = (QMI_WDS_generic_profile_details_0x25_t *)buf_position;
        COPY_HTOM(tlv_0x25->pdp_context, input->profile_settings.pdp_context);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.secondary_flag) {
        QMI_WDS_generic_profile_details_0x26_t *tlv_0x26;

        tlv_sz = sizeof(*tlv_0x26);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x26 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x26 = (QMI_WDS_generic_profile_details_0x26_t *)buf_position;
        COPY_HTOM(tlv_0x26->secondary_flag, input->profile_settings.secondary_flag);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.primary_id_available) {
        QMI_WDS_generic_profile_details_0x27_t *tlv_0x27;

        tlv_sz = sizeof(*tlv_0x27);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x27 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x27 = (QMI_WDS_generic_profile_details_0x27_t *)buf_position;
        COPY_HTOM(tlv_0x27->primary_id, input->profile_settings.primary_id);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.ipv6_addr_pref_available) {
        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x28 + type_id_offset;
        hdr->length = htole16(IPV6_ADDR_MAX_SIZE);

        buf_position += hdr_sz;

        payload_len += hdr_sz + IPV6_ADDR_MAX_SIZE;
        memcpy(buf_position, input->profile_settings.ipv6_addr_pref, IPV6_ADDR_MAX_SIZE);

        buf_position += IPV6_ADDR_MAX_SIZE;
    }

    if (input->profile_settings.umts_req_qos_signaling_available) {
        QMI_WDS_generic_umts_qos_with_ind_t *tlv_0x29;

        tlv_sz = sizeof(*tlv_0x29);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x29 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x29 = (QMI_WDS_generic_umts_qos_with_ind_t *)buf_position;
        COPY_HTOM(tlv_0x29->traffic_class, input->profile_settings.umts_req_qos_signaling.traffic_class);
        COPY_HTOM(tlv_0x29->max_uplink_bitrate,
                input->profile_settings.umts_req_qos_signaling.max_uplink_bitrate);
        COPY_HTOM(tlv_0x29->max_downlink_bitrate,
                input->profile_settings.umts_req_qos_signaling.max_downlink_bitrate);
        COPY_HTOM(tlv_0x29->guaranteed_uplink_bitrate,
                input->profile_settings.umts_req_qos_signaling.guaranteed_uplink_bitrate);
        COPY_HTOM(tlv_0x29->guaranteed_downlink_bitrate,
                input->profile_settings.umts_req_qos_signaling.guaranteed_downlink_bitrate);
        COPY_HTOM(tlv_0x29->qos_delivery_order,
                input->profile_settings.umts_req_qos_signaling.qos_delivery_order);
        COPY_HTOM(tlv_0x29->max_sdu_size, input->profile_settings.umts_req_qos_signaling.max_sdu_size);
        COPY_HTOM(tlv_0x29->sdu_error_ratio, input->profile_settings.umts_req_qos_signaling.sdu_error_ratio);
        COPY_HTOM(tlv_0x29->residual_bit_error_ratio,
                input->profile_settings.umts_req_qos_signaling.residual_bit_error_ratio);
        COPY_HTOM(tlv_0x29->delivery_erroneous_SDUs,
                input->profile_settings.umts_req_qos_signaling.delivery_erroneous_SDUs);
        COPY_HTOM(tlv_0x29->transfer_delay, input->profile_settings.umts_req_qos_signaling.transfer_delay);
        COPY_HTOM(tlv_0x29->traffic_handling_priority,
                input->profile_settings.umts_req_qos_signaling.traffic_handling_priority);
        COPY_HTOM(tlv_0x29->sig_ind, input->profile_settings.umts_req_qos_signaling.sig_ind);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.umts_min_qos_signaling_available) {
        QMI_WDS_generic_umts_qos_with_ind_t *tlv_0x2A;

        tlv_sz = sizeof(*tlv_0x2A);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x2A + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x2A = (QMI_WDS_generic_umts_qos_with_ind_t *)buf_position;
        COPY_HTOM(tlv_0x2A->traffic_class, input->profile_settings.umts_min_qos_signaling.traffic_class);
        COPY_HTOM(tlv_0x2A->max_uplink_bitrate,
                input->profile_settings.umts_min_qos_signaling.max_uplink_bitrate);
        COPY_HTOM(tlv_0x2A->max_downlink_bitrate,
                input->profile_settings.umts_min_qos_signaling.max_downlink_bitrate);
        COPY_HTOM(tlv_0x2A->guaranteed_uplink_bitrate,
                input->profile_settings.umts_min_qos_signaling.guaranteed_uplink_bitrate);
        COPY_HTOM(tlv_0x2A->guaranteed_downlink_bitrate,
                input->profile_settings.umts_min_qos_signaling.guaranteed_downlink_bitrate);
        COPY_HTOM(tlv_0x2A->qos_delivery_order,
                input->profile_settings.umts_min_qos_signaling.qos_delivery_order);
        COPY_HTOM(tlv_0x2A->max_sdu_size, input->profile_settings.umts_min_qos_signaling.max_sdu_size);
        COPY_HTOM(tlv_0x2A->sdu_error_ratio, input->profile_settings.umts_min_qos_signaling.sdu_error_ratio);
        COPY_HTOM(tlv_0x2A->residual_bit_error_ratio,
                input->profile_settings.umts_min_qos_signaling.residual_bit_error_ratio);
        COPY_HTOM(tlv_0x2A->delivery_erroneous_SDUs,
                input->profile_settings.umts_min_qos_signaling.delivery_erroneous_SDUs);
        COPY_HTOM(tlv_0x2A->transfer_delay, input->profile_settings.umts_min_qos_signaling.transfer_delay);
        COPY_HTOM(tlv_0x2A->traffic_handling_priority,
                input->profile_settings.umts_min_qos_signaling.traffic_handling_priority);
        COPY_HTOM(tlv_0x2A->sig_ind, input->profile_settings.umts_min_qos_signaling.sig_ind);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.pri_dns_ipv6_addr_pref_available) {
        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x2B + type_id_offset;
        hdr->length = htole16(IPV6_ADDR_MAX_SIZE);

        buf_position += hdr_sz;

        payload_len += hdr_sz + IPV6_ADDR_MAX_SIZE;
        memcpy(buf_position, input->profile_settings.pri_dns_ipv6_addr_pref, IPV6_ADDR_MAX_SIZE);

        buf_position += IPV6_ADDR_MAX_SIZE;
    }

    if (input->profile_settings.sec_dns_ipv6_addr_pref_available) {
        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x2C + type_id_offset;
        hdr->length = htole16(IPV6_ADDR_MAX_SIZE);

        buf_position += hdr_sz;

        payload_len += hdr_sz + IPV6_ADDR_MAX_SIZE;
        memcpy(buf_position, input->profile_settings.sec_dns_ipv6_addr_pref, IPV6_ADDR_MAX_SIZE);

        buf_position += IPV6_ADDR_MAX_SIZE;
    }

    if (input->profile_settings.addr_allocation_pref_available) {
        QMI_WDS_generic_profile_details_0x2D_t *tlv_0x2D;

        tlv_sz = sizeof(*tlv_0x2D);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x2D + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x2D = (QMI_WDS_generic_profile_details_0x2D_t *)buf_position;
        COPY_HTOM(tlv_0x2D->addr_allocation_pref, input->profile_settings.addr_allocation_pref);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.lte_qos_params_available) {
        QMI_WDS_generic_profile_details_0x2E_t *tlv_0x2E;

        tlv_sz = sizeof(*tlv_0x2E);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x2E + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x2E = (QMI_WDS_generic_profile_details_0x2E_t *)buf_position;
        COPY_HTOM(tlv_0x2E->qci, input->profile_settings.lte_qos_params.qci);
        COPY_HTOM(tlv_0x2E->g_dl_bit_rate, input->profile_settings.lte_qos_params.g_dl_bit_rate);
        COPY_HTOM(tlv_0x2E->max_dl_bit_rate, input->profile_settings.lte_qos_params.max_dl_bit_rate);
        COPY_HTOM(tlv_0x2E->g_ul_bit_rate, input->profile_settings.lte_qos_params.g_ul_bit_rate);
        COPY_HTOM(tlv_0x2E->max_ul_bit_rate, input->profile_settings.lte_qos_params.max_ul_bit_rate);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.apn_disabled_flag) {
        QMI_WDS_generic_profile_details_0x2F_t *tlv_0x2F;

        tlv_sz = sizeof(*tlv_0x2F);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x2F + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x2F = (QMI_WDS_generic_profile_details_0x2F_t *)buf_position;
        COPY_HTOM(tlv_0x2F->apn_disabled_flag, input->profile_settings.apn_disabled_flag);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.pdn_inactivity_timeout_available) {
        QMI_WDS_generic_profile_details_0x30_t *tlv_0x30;

        tlv_sz = sizeof(*tlv_0x30);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x30 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x30 = (QMI_WDS_generic_profile_details_0x30_t *)buf_position;
        COPY_HTOM(tlv_0x30->pdn_inactivity_timeout, input->profile_settings.pdn_inactivity_timeout);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.apn_class_available) {
        QMI_WDS_generic_profile_details_0x31_t *tlv_0x31;

        tlv_sz = sizeof(*tlv_0x31);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x31 + type_id_offset;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x31 = (QMI_WDS_generic_profile_details_0x31_t *)buf_position;
        COPY_HTOM(tlv_0x31->apn_class, input->profile_settings.apn_class);

        buf_position += tlv_sz;
    }

    if (input->profile_settings.support_emergency_calls) {
        QMI_WDS_generic_profile_details_emergency_t *tlv_emergency;

        tlv_sz = sizeof(*tlv_emergency);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        /* Support emergency calls tlv does not have the same id */
        if (msg_type == QMI_GMS_WDS_CREATE_PROFILE) {
            hdr->type_id = 0x33;
        } else {
            hdr->type_id = 0x36;
        }
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_emergency = (QMI_WDS_generic_profile_details_emergency_t *)buf_position;
        COPY_HTOM(tlv_emergency->support_emergency_calls, input->profile_settings.support_emergency_calls);
    }

    ret = QMI_SDK_ERR_NONE;

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wds_create_profile_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_profile_t *input)
{
    return telit_wds_generic_modify_profile_pack(req_ctx, req,
            len, input,
            QMI_WDS_CREATE_PROFILE);
}

QMI_SDK_error_t telit_wds_create_profile_unpack(uint8_t *rsp, uint16_t len, wds_profile_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    /* Not exiting for protocol errors, since we want to collect the extended error */
    if (ret != QMI_SDK_ERR_NONE && ret < QMI_SDK_ERR_ERR_QMI_OFFSET) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    if (ret == QMI_SDK_ERR_NONE) {
        QMI_WDS_generic_profile_id_t *tlv_0x01;

        FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_id_t);

        hdr++;
        tlv_0x01 = (QMI_WDS_generic_profile_id_t *)hdr;
        COPY_MTOH(out->profile_id.type, tlv_0x01->profile_type);
        COPY_MTOH(out->profile_id.index, tlv_0x01->profile_index);
        out->profile_id_available = true;
    }

    hdr = search_TLV(msg, 0xE0, payload_len);
    /* TLV 0xE0 is optional */
    if (hdr) {
        QMI_WDS_generic_resp_0xE0_t *tlv_0xE0;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_resp_0xE0_t);

        hdr++;
        tlv_0xE0 = (QMI_WDS_generic_resp_0xE0_t *)hdr;
        COPY_MTOH(out->extended_error_code, tlv_0xE0->extended_error_code);
        out->extended_error_code_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_WDS_MODIFY_PROFILE_SETTINGS (40 - 0x0028)
 */

static QMI_SDK_error_t telit_wds_generic_extended_error_unpack(uint8_t *rsp, uint16_t len,
        wds_profile_ext_err_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    /* Not exiting for protocol errors, since we want to collect the extended error */
    if (ret != QMI_SDK_ERR_NONE && ret < QMI_SDK_ERR_ERR_QMI_OFFSET) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0xE0, payload_len);
    /* TLV 0xE0 is optional */
    if (hdr) {
        QMI_WDS_generic_resp_0xE0_t *tlv_0xE0;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_resp_0xE0_t);

        hdr++;
        tlv_0xE0 = (QMI_WDS_generic_resp_0xE0_t *)hdr;
        COPY_MTOH(out->extended_error_code, tlv_0xE0->extended_error_code);
    }

end:
    return ret;
}

QMI_SDK_error_t telit_wds_modify_profile_settings_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_profile_t *input)
{
    return telit_wds_generic_modify_profile_pack(req_ctx, req,
            len, input,
            QMI_WDS_MODIFY_PROFILE_SETTINGS);
}

QMI_SDK_error_t telit_wds_modify_profile_settings_unpack(uint8_t *rsp, uint16_t len,
        wds_profile_ext_err_t *out)
{
    return telit_wds_generic_extended_error_unpack(rsp, len, out);
}

/*****************************************************************************
 * QMI_WDS_DELETE_PROFILE (41 - 0x0029)
 */

static QMI_SDK_error_t telit_wds_generic_manage_profile_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_settings_profile_id_t *input,
        QMI_WDS_message_t msg_type)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WDS_generic_profile_details_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = msg_type;
    req_ctx->svc_id = QMI_SVC_WDS;
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

    tlv_0x01 = (QMI_WDS_generic_profile_details_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->profile_type, input->type);
    COPY_HTOM(tlv_0x01->profile_index, input->index);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wds_delete_profile_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_settings_profile_id_t *input)
{
    return telit_wds_generic_manage_profile_pack(req_ctx, req, len,
            input, QMI_WDS_DELETE_PROFILE);
}

QMI_SDK_error_t telit_wds_delete_profile_unpack(uint8_t *rsp, uint16_t len, wds_profile_ext_err_t *out)
{
    return telit_wds_generic_extended_error_unpack(rsp, len, out);
}

/*****************************************************************************
 * QMI_WDS_GET_PROFILE_LIST (42 - 0x002A)
 */

QMI_SDK_error_t telit_wds_get_profile_list_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_profile_list_input_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_WDS_GET_PROFILE_LIST;
    req_ctx->svc_id = QMI_SVC_WDS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->profile_type_available) {
        QMI_WDS_generic_profile_type_0x01_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_WDS_generic_profile_type_0x01_t *)buf_position;
        COPY_HTOM(tlv_0x10->profile_type, input->profile_type);

        buf_position += tlv_sz;
    }

    if (input->search_type_available) {
        QMI_WDS_GET_PROFILE_LIST_request_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_WDS_GET_PROFILE_LIST_request_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->search_type, input->search_type);

        buf_position += tlv_sz;
    }

    if (input->apn_name_len) {
        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(input->apn_name_len);

        buf_position += hdr_sz;

        memcpy(buf_position, input->apn_name, input->apn_name_len);
        payload_len += hdr_sz + input->apn_name_len;
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wds_get_profile_list_unpack(uint8_t *rsp,
        uint16_t len,
        wds_profile_list_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_WDS_GET_PROFILE_LIST_response_0x01_t *tlv_0x01;
    QMI_WDS_profile_list_instance_t *profile_instance;
    uint8_t *skip = NULL;
    uint8_t i;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_WDS_GET_PROFILE_LIST_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_WDS_GET_PROFILE_LIST_response_0x01_t *)hdr;

    COPY_MTOH(out->profile_list_size, tlv_0x01->profile_list_size);

    skip = (uint8_t *)(&(tlv_0x01->profile_list_size));
    skip += sizeof(tlv_0x01->profile_list_size);

    profile_instance = (QMI_WDS_profile_list_instance_t *)skip;

    if (out->profile_list_size > QMI_WDS_PROFILE_LIST_MAX_ELEMENTS) {
        RLOGE("%s: profile_list_size = %u > QMI_WDS_PROFILE_LIST_MAX_ELEMENTS, increase the define",
                __FUNCTION__, out->profile_list_size);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    for (i = 0; i < out->profile_list_size; i++) {
        COPY_MTOH(out->profile_list[i].profile_type, profile_instance->profile_type);
        COPY_MTOH(out->profile_list[i].profile_index, profile_instance->profile_index);
        COPY_MTOH(out->profile_list[i].profile_name_len, profile_instance->profile_name_len);

        skip = (uint8_t *)(&(profile_instance->profile_name_len));
        skip += sizeof(profile_instance->profile_name_len);

        memcpy(out->profile_list[i].profile_name, skip,
                out->profile_list[i].profile_name_len);

        skip += out->profile_list[i].profile_name_len;
        profile_instance = (QMI_WDS_profile_list_instance_t *)skip;
    }

    hdr = search_TLV(msg, 0xE0, payload_len);
    /* TLV 0xE0 is optional */
    if (hdr) {
        QMI_WDS_generic_resp_0xE0_t *tlv_0xE0;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_resp_0xE0_t);

        hdr++;
        tlv_0xE0 = (QMI_WDS_generic_resp_0xE0_t *)hdr;
        COPY_MTOH(out->extended_error_code, tlv_0xE0->extended_error_code);
        out->extended_error_code_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_WDS_GET_PROFILE_SETTINGS (43 - 0x002B)
 */

QMI_SDK_error_t telit_wds_get_profile_settings_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_settings_profile_id_t *input)
{
    return telit_wds_generic_manage_profile_pack(req_ctx, req, len,
            input, QMI_WDS_GET_PROFILE_SETTINGS);
}

static QMI_SDK_error_t telit_wds_common_profile_settings_unpack(uint8_t *msg, uint16_t payload_len,
        wds_profile_t *out)
{
    QMI_SDK_error_t ret;
    QMI_raw_content_header_t *hdr = NULL;

    ret = get_string(msg, payload_len, 0x10,
            QMI_WDS_MAX_STRING_SIZE_PROFILE, out->profile_settings.profile_name);
    if (ret == QMI_SDK_ERR_NONE) {
        out->profile_settings.profile_name_len = strlen(out->profile_settings.profile_name);
    } else if (ret != QMI_SDK_ERR_MISSING_TLV) {
        RLOGE("%s: unable to collect profile_settings.profile_name", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x11_t);

        out->profile_settings.pdp_type_available = true;

        hdr++;
        tlv_0x11 = (QMI_WDS_generic_profile_details_0x11_t *)hdr;
        COPY_MTOH(out->profile_settings.pdp_type, tlv_0x11->pdp_type);
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x12_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x12_t);

        out->profile_settings.pdp_hdr_comp_type_available = true;

        hdr++;
        tlv_0x12 = (QMI_WDS_generic_profile_details_0x12_t *)hdr;
        COPY_MTOH(out->profile_settings.pdp_hdr_comp_type, tlv_0x12->pdp_hdr_comp_type);
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x13_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x13_t);

        out->profile_settings.pdp_data_comp_type_available = true;

        hdr++;
        tlv_0x13 = (QMI_WDS_generic_profile_details_0x13_t *)hdr;
        COPY_MTOH(out->profile_settings.pdp_data_comp_type, tlv_0x13->pdp_data_comp_type);
    }

    ret = get_string(msg, payload_len, 0x14,
            QMI_WDS_MAX_STRING_SIZE_PROFILE, out->profile_settings.apn);
    if (ret == QMI_SDK_ERR_NONE) {
        out->profile_settings.apn_len = strlen(out->profile_settings.apn);
    } else if (ret != QMI_SDK_ERR_MISSING_TLV) {
        RLOGE("%s: unable to collect profile_settings.apn", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    /* TLV 0x15 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x15_t *tlv_0x15;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x15_t);

        out->profile_settings.pri_dns_ipv4_addr_pref_available = true;

        hdr++;
        tlv_0x15 = (QMI_WDS_generic_profile_details_0x15_t *)hdr;
        /* No conversion for this, since we want the address in network byte order */
        out->profile_settings.pri_dns_ipv4_addr_pref = tlv_0x15->pri_dns_ipv4_addr_pref;
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    /* TLV 0x16 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x16_t *tlv_0x16;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x16_t);

        out->profile_settings.sec_dns_ipv4_addr_pref_available = true;

        hdr++;
        tlv_0x16 = (QMI_WDS_generic_profile_details_0x16_t *)hdr;
        /* No conversion for this, since we want the address in network byte order */
        out->profile_settings.sec_dns_ipv4_addr_pref = tlv_0x16->sec_dns_ipv4_addr_pref;
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    /* TLV 0x17 is optional */
    if (hdr) {
        QMI_WDS_generic_umts_qos_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_umts_qos_t);

        out->profile_settings.umts_req_qos_available = true;

        hdr++;
        tlv_0x17 = (QMI_WDS_generic_umts_qos_t *)hdr;
        COPY_MTOH(out->profile_settings.umts_req_qos.traffic_class, tlv_0x17->traffic_class);
        COPY_MTOH(out->profile_settings.umts_req_qos.max_uplink_bitrate, tlv_0x17->max_uplink_bitrate);
        COPY_MTOH(out->profile_settings.umts_req_qos.max_downlink_bitrate, tlv_0x17->max_downlink_bitrate);
        COPY_MTOH(out->profile_settings.umts_req_qos.guaranteed_uplink_bitrate,
                tlv_0x17->guaranteed_uplink_bitrate);
        COPY_MTOH(out->profile_settings.umts_req_qos.guaranteed_downlink_bitrate,
                tlv_0x17->guaranteed_downlink_bitrate);
        COPY_MTOH(out->profile_settings.umts_req_qos.qos_delivery_order, tlv_0x17->qos_delivery_order);
        COPY_MTOH(out->profile_settings.umts_req_qos.max_sdu_size, tlv_0x17->max_sdu_size);
        COPY_MTOH(out->profile_settings.umts_req_qos.sdu_error_ratio, tlv_0x17->sdu_error_ratio);
        COPY_MTOH(out->profile_settings.umts_req_qos.residual_bit_error_ratio,
                tlv_0x17->residual_bit_error_ratio);
        COPY_MTOH(out->profile_settings.umts_req_qos.delivery_erroneous_SDUs,
                tlv_0x17->delivery_erroneous_SDUs);
        COPY_MTOH(out->profile_settings.umts_req_qos.transfer_delay, tlv_0x17->transfer_delay);
        COPY_MTOH(out->profile_settings.umts_req_qos.traffic_handling_priority,
                tlv_0x17->traffic_handling_priority);
    }

    hdr = search_TLV(msg, 0x18, payload_len);
    /* TLV 0x18 is optional */
    if (hdr) {
        QMI_WDS_generic_umts_qos_t *tlv_0x18;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_umts_qos_t);

        out->profile_settings.umts_min_qos_available = true;

        hdr++;
        tlv_0x18 = (QMI_WDS_generic_umts_qos_t *)hdr;
        COPY_MTOH(out->profile_settings.umts_min_qos.traffic_class, tlv_0x18->traffic_class);
        COPY_MTOH(out->profile_settings.umts_min_qos.max_uplink_bitrate, tlv_0x18->max_uplink_bitrate);
        COPY_MTOH(out->profile_settings.umts_min_qos.max_downlink_bitrate, tlv_0x18->max_downlink_bitrate);
        COPY_MTOH(out->profile_settings.umts_min_qos.guaranteed_uplink_bitrate,
                tlv_0x18->guaranteed_uplink_bitrate);
        COPY_MTOH(out->profile_settings.umts_min_qos.guaranteed_downlink_bitrate,
                tlv_0x18->guaranteed_downlink_bitrate);
        COPY_MTOH(out->profile_settings.umts_min_qos.qos_delivery_order, tlv_0x18->qos_delivery_order);
        COPY_MTOH(out->profile_settings.umts_min_qos.max_sdu_size, tlv_0x18->max_sdu_size);
        COPY_MTOH(out->profile_settings.umts_min_qos.sdu_error_ratio, tlv_0x18->sdu_error_ratio);
        COPY_MTOH(out->profile_settings.umts_min_qos.residual_bit_error_ratio,
                tlv_0x18->residual_bit_error_ratio);
        COPY_MTOH(out->profile_settings.umts_min_qos.delivery_erroneous_SDUs,
                tlv_0x18->delivery_erroneous_SDUs);
        COPY_MTOH(out->profile_settings.umts_min_qos.transfer_delay, tlv_0x18->transfer_delay);
        COPY_MTOH(out->profile_settings.umts_min_qos.traffic_handling_priority,
                tlv_0x18->traffic_handling_priority);
    }

    hdr = search_TLV(msg, 0x19, payload_len);
    /* TLV 0x19 is optional */
    if (hdr) {
        QMI_WDS_generic_gprs_qos_t *tlv_0x19;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_gprs_qos_t);

        out->profile_settings.gprs_req_qos_available = true;

        hdr++;
        tlv_0x19 = (QMI_WDS_generic_gprs_qos_t *)hdr;
        COPY_MTOH(out->profile_settings.gprs_req_qos.precedence_class, tlv_0x19->precedence_class);
        COPY_MTOH(out->profile_settings.gprs_req_qos.delay_class, tlv_0x19->delay_class);
        COPY_MTOH(out->profile_settings.gprs_req_qos.reliability_class, tlv_0x19->reliability_class);
        COPY_MTOH(out->profile_settings.gprs_req_qos.peak_throughput_class, tlv_0x19->peak_throughput_class);
        COPY_MTOH(out->profile_settings.gprs_req_qos.mean_throughput_class, tlv_0x19->mean_throughput_class);
    }

    hdr = search_TLV(msg, 0x1A, payload_len);
    /* TLV 0x1A is optional */
    if (hdr) {
        QMI_WDS_generic_gprs_qos_t *tlv_0x1A;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_gprs_qos_t);

        out->profile_settings.gprs_min_qos_available = true;

        hdr++;
        tlv_0x1A = (QMI_WDS_generic_gprs_qos_t *)hdr;
        COPY_MTOH(out->profile_settings.gprs_min_qos.precedence_class, tlv_0x1A->precedence_class);
        COPY_MTOH(out->profile_settings.gprs_min_qos.delay_class, tlv_0x1A->delay_class);
        COPY_MTOH(out->profile_settings.gprs_min_qos.reliability_class, tlv_0x1A->reliability_class);
        COPY_MTOH(out->profile_settings.gprs_min_qos.peak_throughput_class, tlv_0x1A->peak_throughput_class);
        COPY_MTOH(out->profile_settings.gprs_min_qos.mean_throughput_class, tlv_0x1A->mean_throughput_class);
    }

    ret = get_string(msg, payload_len, 0x1B,
            QMI_WDS_MAX_STRING_SIZE_PROFILE, out->profile_settings.username);
    if (ret == QMI_SDK_ERR_NONE) {
        out->profile_settings.username_len = strlen(out->profile_settings.username);
    } else if (ret != QMI_SDK_ERR_MISSING_TLV) {
        RLOGE("%s: unable to collect profile_settings.username", __FUNCTION__);
        goto end;
    }

    ret = get_string(msg, payload_len, 0x1C,
            QMI_WDS_MAX_STRING_SIZE_PROFILE, out->profile_settings.password);
    if (ret == QMI_SDK_ERR_NONE) {
        out->profile_settings.password_len = strlen(out->profile_settings.password);
    } else if (ret != QMI_SDK_ERR_MISSING_TLV) {
        RLOGE("%s: unable to collect profile_settings.password", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x1D, payload_len);
    /* TLV 0x1D is optional */
    if (hdr) {
        QMI_WDS_generic_authentication_pref_t *tlv_0x1D;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_authentication_pref_t);

        out->profile_settings.authentication_pref_available = true;

        hdr++;
        tlv_0x1D = (QMI_WDS_generic_authentication_pref_t *)hdr;
        COPY_MTOH(out->profile_settings.authentication_pref, tlv_0x1D->authentication_pref);
    }

    hdr = search_TLV(msg, 0x1E, payload_len);
    /* TLV 0x1E is optional */
    if (hdr) {
        QMI_WDS_generic_ipv4_addr_t *tlv_0x1E;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_ipv4_addr_t);

        out->profile_settings.ipv4_addr_pref_available = true;

        hdr++;
        tlv_0x1E = (QMI_WDS_generic_ipv4_addr_t *)hdr;
        /* No conversion for this, since we want the address in network byte order */
        out->profile_settings.ipv4_addr_pref = tlv_0x1E->ipv4_addr;
    }

    out->profile_settings_available = true;
    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

QMI_SDK_error_t telit_wds_get_profile_settings_unpack(uint8_t *rsp, uint16_t len, wds_profile_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    /* Not exiting for protocol errors, since we want to collect the extended error  */
    if (ret != QMI_SDK_ERR_NONE && ret < QMI_SDK_ERR_ERR_QMI_OFFSET) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    } else if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        telit_wds_generic_extended_error_unpack(rsp, len, &out->extended_error_code);
        out->extended_error_code_available = true;
        goto end;
    }

    ret = telit_wds_common_profile_settings_unpack(msg, payload_len, out);
    if (ret != QMI_SDK_ERR_NONE) {
        goto end;
    }

    hdr = search_TLV(msg, 0x1F, payload_len);
    /* TLV 0x1F is optional */
    if (hdr) {
        QMI_WDS_generic_pcscf_addr_with_pco_t *tlv_0x1F;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_pcscf_addr_with_pco_t);

        hdr++;
        tlv_0x1F = (QMI_WDS_generic_pcscf_addr_with_pco_t *)hdr;
        COPY_MTOH(out->profile_settings.pcscf_addr_using_pco, tlv_0x1F->pcscf_addr_using_pco);
    } else {
        out->profile_settings.pcscf_addr_using_pco = false;
    }

    hdr = search_TLV(msg, 0x20, payload_len);
    /* TLV 0x20 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x20_t *tlv_0x20;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x20_t);

        out->profile_settings.pdp_access_control_flag_available = true;

        hdr++;
        tlv_0x20 = (QMI_WDS_generic_profile_details_0x20_t *)hdr;
        COPY_MTOH(out->profile_settings.pdp_access_control_flag, tlv_0x20->pdp_access_control_flag);
    }

    hdr = search_TLV(msg, 0x21, payload_len);
    /* TLV 0x21 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x21_t *tlv_0x21;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x21_t);

        hdr++;
        tlv_0x21 = (QMI_WDS_generic_profile_details_0x21_t *)hdr;
        COPY_MTOH(out->profile_settings.pcscf_addr_using_dhcp, tlv_0x21->pcscf_addr_using_dhcp);
    } else {
        out->profile_settings.pcscf_addr_using_dhcp = false;
    }

    hdr = search_TLV(msg, 0x22, payload_len);
    /* TLV 0x22 is optional */
    if (hdr) {
        QMI_WDS_generic_im_cn_flag_t *tlv_0x22;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_im_cn_flag_t);

        hdr++;
        tlv_0x22 = (QMI_WDS_generic_im_cn_flag_t *)hdr;
        COPY_MTOH(out->profile_settings.im_cn_flag, tlv_0x22->im_cn_flag);
    } else {
        out->profile_settings.im_cn_flag = false;
    }

    hdr = search_TLV(msg, 0x23, payload_len);
    /* TLV 0x23 is optional */
    if (hdr) {
        QMI_WDS_generic_tft_parameters_t *tlv_0x23;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_tft_parameters_t);

        out->profile_settings.tft_id1_available = true;

        hdr++;
        tlv_0x23 = (QMI_WDS_generic_tft_parameters_t *)hdr;
        COPY_MTOH(out->profile_settings.tft_id1.filter_id, tlv_0x23->filter_id);
        COPY_MTOH(out->profile_settings.tft_id1.eval_id, tlv_0x23->eval_id);
        COPY_MTOH(out->profile_settings.tft_id1.ip_version, tlv_0x23->ip_version);
        memcpy(out->profile_settings.tft_id1.source_ip, tlv_0x23->source_ip, 16);
        COPY_MTOH(out->profile_settings.tft_id1.source_ip_mask, tlv_0x23->source_ip_mask);
        COPY_MTOH(out->profile_settings.tft_id1.next_header, tlv_0x23->next_header);
        COPY_MTOH(out->profile_settings.tft_id1.dest_port_range_start, tlv_0x23->dest_port_range_start);
        COPY_MTOH(out->profile_settings.tft_id1.dest_port_range_end, tlv_0x23->dest_port_range_end);
        COPY_MTOH(out->profile_settings.tft_id1.src_port_range_start, tlv_0x23->src_port_range_start);
        COPY_MTOH(out->profile_settings.tft_id1.src_port_range_end, tlv_0x23->src_port_range_end);
        COPY_MTOH(out->profile_settings.tft_id1.ipsec_spi, tlv_0x23->ipsec_spi);
        COPY_MTOH(out->profile_settings.tft_id1.tos_mask, tlv_0x23->tos_mask);
        COPY_MTOH(out->profile_settings.tft_id1.flow_label, tlv_0x23->flow_label);
    }

    hdr = search_TLV(msg, 0x24, payload_len);
    /* TLV 0x24 is optional */
    if (hdr) {
        QMI_WDS_generic_tft_parameters_t *tlv_0x24;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_tft_parameters_t);

        out->profile_settings.tft_id2_available = true;

        hdr++;
        tlv_0x24 = (QMI_WDS_generic_tft_parameters_t *)hdr;
        COPY_MTOH(out->profile_settings.tft_id2.filter_id, tlv_0x24->filter_id);
        COPY_MTOH(out->profile_settings.tft_id2.eval_id, tlv_0x24->eval_id);
        COPY_MTOH(out->profile_settings.tft_id2.ip_version, tlv_0x24->ip_version);
        memcpy(out->profile_settings.tft_id2.source_ip, tlv_0x24->source_ip, 16);
        COPY_MTOH(out->profile_settings.tft_id2.source_ip_mask, tlv_0x24->source_ip_mask);
        COPY_MTOH(out->profile_settings.tft_id2.next_header, tlv_0x24->next_header);
        COPY_MTOH(out->profile_settings.tft_id2.dest_port_range_start, tlv_0x24->dest_port_range_start);
        COPY_MTOH(out->profile_settings.tft_id2.dest_port_range_end, tlv_0x24->dest_port_range_end);
        COPY_MTOH(out->profile_settings.tft_id2.src_port_range_start, tlv_0x24->src_port_range_start);
        COPY_MTOH(out->profile_settings.tft_id2.src_port_range_end, tlv_0x24->src_port_range_end);
        COPY_MTOH(out->profile_settings.tft_id2.ipsec_spi, tlv_0x24->ipsec_spi);
        COPY_MTOH(out->profile_settings.tft_id2.tos_mask, tlv_0x24->tos_mask);
        COPY_MTOH(out->profile_settings.tft_id2.flow_label, tlv_0x24->flow_label);
    }

    hdr = search_TLV(msg, 0x25, payload_len);
    /* TLV 0x25 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x25_t *tlv_0x25;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x25_t);

        out->profile_settings.pdp_context_available = true;

        hdr++;
        tlv_0x25 = (QMI_WDS_generic_profile_details_0x25_t *)hdr;
        COPY_MTOH(out->profile_settings.pdp_context, tlv_0x25->pdp_context);
    }

    hdr = search_TLV(msg, 0x26, payload_len);
    /* TLV 0x26 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x26_t *tlv_0x26;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x26_t);

        hdr++;
        tlv_0x26 = (QMI_WDS_generic_profile_details_0x26_t *)hdr;
        COPY_MTOH(out->profile_settings.secondary_flag, tlv_0x26->secondary_flag);
    } else {
        out->profile_settings.secondary_flag = false;
    }

    hdr = search_TLV(msg, 0x27, payload_len);
    /* TLV 0x27 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x27_t *tlv_0x27;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x27_t);

        out->profile_settings.primary_id_available = true;

        hdr++;
        tlv_0x27 = (QMI_WDS_generic_profile_details_0x27_t *)hdr;
        COPY_MTOH(out->profile_settings.primary_id, tlv_0x27->primary_id);
    }

    hdr = search_TLV(msg, 0x28, payload_len);
    /* TLV 0x28 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x28_t *tlv_0x28;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x28_t);

        out->profile_settings.ipv6_addr_pref_available = true;

        hdr++;
        tlv_0x28 = (QMI_WDS_generic_profile_details_0x28_t *)hdr;
        memcpy(out->profile_settings.ipv6_addr_pref, tlv_0x28->ipv6_addr_pref, 16);
    }

    hdr = search_TLV(msg, 0x29, payload_len);
    /* TLV 0x29 is optional */
    if (hdr) {
        QMI_WDS_generic_umts_qos_with_ind_t *tlv_0x29;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_umts_qos_with_ind_t);

        out->profile_settings.umts_req_qos_signaling_available = true;

        hdr++;
        tlv_0x29 = (QMI_WDS_generic_umts_qos_with_ind_t *)hdr;
        COPY_MTOH(out->profile_settings.umts_req_qos_signaling.traffic_class, tlv_0x29->traffic_class);
        COPY_MTOH(out->profile_settings.umts_req_qos_signaling.max_uplink_bitrate,
                tlv_0x29->max_uplink_bitrate);
        COPY_MTOH(out->profile_settings.umts_req_qos_signaling.max_downlink_bitrate,
                tlv_0x29->max_downlink_bitrate);
        COPY_MTOH(out->profile_settings.umts_req_qos_signaling.guaranteed_uplink_bitrate,
                tlv_0x29->guaranteed_uplink_bitrate);
        COPY_MTOH(out->profile_settings.umts_req_qos_signaling.guaranteed_downlink_bitrate,
                tlv_0x29->guaranteed_downlink_bitrate);
        COPY_MTOH(out->profile_settings.umts_req_qos_signaling.qos_delivery_order,
                tlv_0x29->qos_delivery_order);
        COPY_MTOH(out->profile_settings.umts_req_qos_signaling.max_sdu_size, tlv_0x29->max_sdu_size);
        COPY_MTOH(out->profile_settings.umts_req_qos_signaling.sdu_error_ratio, tlv_0x29->sdu_error_ratio);
        COPY_MTOH(out->profile_settings.umts_req_qos_signaling.residual_bit_error_ratio,
                tlv_0x29->residual_bit_error_ratio);
        COPY_MTOH(out->profile_settings.umts_req_qos_signaling.delivery_erroneous_SDUs,
                tlv_0x29->delivery_erroneous_SDUs);
        COPY_MTOH(out->profile_settings.umts_req_qos_signaling.transfer_delay, tlv_0x29->transfer_delay);
        COPY_MTOH(out->profile_settings.umts_req_qos_signaling.traffic_handling_priority,
                tlv_0x29->traffic_handling_priority);
        COPY_MTOH(out->profile_settings.umts_req_qos_signaling.sig_ind, tlv_0x29->sig_ind);
    }

    hdr = search_TLV(msg, 0x2A, payload_len);
    /* TLV 0x2A is optional */
    if (hdr) {
        QMI_WDS_generic_umts_qos_with_ind_t *tlv_0x2A;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_umts_qos_with_ind_t);

        out->profile_settings.umts_min_qos_signaling_available = true;

        hdr++;
        tlv_0x2A = (QMI_WDS_generic_umts_qos_with_ind_t *)hdr;
        COPY_MTOH(out->profile_settings.umts_min_qos_signaling.traffic_class, tlv_0x2A->traffic_class);
        COPY_MTOH(out->profile_settings.umts_min_qos_signaling.max_uplink_bitrate,
                tlv_0x2A->max_uplink_bitrate);
        COPY_MTOH(out->profile_settings.umts_min_qos_signaling.max_downlink_bitrate,
                tlv_0x2A->max_downlink_bitrate);
        COPY_MTOH(out->profile_settings.umts_min_qos_signaling.guaranteed_uplink_bitrate,
                tlv_0x2A->guaranteed_uplink_bitrate);
        COPY_MTOH(out->profile_settings.umts_min_qos_signaling.guaranteed_downlink_bitrate,
                tlv_0x2A->guaranteed_downlink_bitrate);
        COPY_MTOH(out->profile_settings.umts_min_qos_signaling.qos_delivery_order,
                tlv_0x2A->qos_delivery_order);
        COPY_MTOH(out->profile_settings.umts_min_qos_signaling.max_sdu_size, tlv_0x2A->max_sdu_size);
        COPY_MTOH(out->profile_settings.umts_min_qos_signaling.sdu_error_ratio, tlv_0x2A->sdu_error_ratio);
        COPY_MTOH(out->profile_settings.umts_min_qos_signaling.residual_bit_error_ratio,
                tlv_0x2A->residual_bit_error_ratio);
        COPY_MTOH(out->profile_settings.umts_min_qos_signaling.delivery_erroneous_SDUs,
                tlv_0x2A->delivery_erroneous_SDUs);
        COPY_MTOH(out->profile_settings.umts_min_qos_signaling.transfer_delay, tlv_0x2A->transfer_delay);
        COPY_MTOH(out->profile_settings.umts_min_qos_signaling.traffic_handling_priority,
                tlv_0x2A->traffic_handling_priority);
        COPY_MTOH(out->profile_settings.umts_min_qos_signaling.sig_ind, tlv_0x2A->sig_ind);
    }

    hdr = search_TLV(msg, 0x2B, payload_len);
    /* TLV 0x2B is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x2B_t *tlv_0x2B;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x2B_t);

        out->profile_settings.pri_dns_ipv6_addr_pref_available = true;

        hdr++;
        tlv_0x2B = (QMI_WDS_generic_profile_details_0x2B_t *)hdr;
        memcpy(out->profile_settings.pri_dns_ipv6_addr_pref, tlv_0x2B->pri_dns_ipv6_addr_pref, 16);
    }

    hdr = search_TLV(msg, 0x2C, payload_len);
    /* TLV 0x2C is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x2C_t *tlv_0x2C;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x2C_t);

        out->profile_settings.sec_dns_ipv6_addr_pref_available = true;

        hdr++;
        tlv_0x2C = (QMI_WDS_generic_profile_details_0x2C_t *)hdr;
        memcpy(out->profile_settings.sec_dns_ipv6_addr_pref, tlv_0x2C->sec_dns_ipv6_addr_pref, 16);
    }

    hdr = search_TLV(msg, 0x2D, payload_len);
    /* TLV 0x2D is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x2D_t *tlv_0x2D;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x2D_t);

        out->profile_settings.addr_allocation_pref_available = true;

        hdr++;
        tlv_0x2D = (QMI_WDS_generic_profile_details_0x2D_t *)hdr;
        COPY_MTOH(out->profile_settings.addr_allocation_pref, tlv_0x2D->addr_allocation_pref);
    }

    hdr = search_TLV(msg, 0x2E, payload_len);
    /* TLV 0x2E is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x2E_t *tlv_0x2E;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x2E_t);

        out->profile_settings.lte_qos_params_available = true;

        hdr++;
        tlv_0x2E = (QMI_WDS_generic_profile_details_0x2E_t *)hdr;
        COPY_MTOH(out->profile_settings.lte_qos_params.qci, tlv_0x2E->qci);
        COPY_MTOH(out->profile_settings.lte_qos_params.g_dl_bit_rate, tlv_0x2E->g_dl_bit_rate);
        COPY_MTOH(out->profile_settings.lte_qos_params.max_dl_bit_rate, tlv_0x2E->max_dl_bit_rate);
        COPY_MTOH(out->profile_settings.lte_qos_params.g_ul_bit_rate, tlv_0x2E->g_ul_bit_rate);
        COPY_MTOH(out->profile_settings.lte_qos_params.max_ul_bit_rate, tlv_0x2E->max_ul_bit_rate);
    }

    hdr = search_TLV(msg, 0x2F, payload_len);
    /* TLV 0x2F is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x2F_t *tlv_0x2F;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x2F_t);

        hdr++;
        tlv_0x2F = (QMI_WDS_generic_profile_details_0x2F_t *)hdr;
        COPY_MTOH(out->profile_settings.apn_disabled_flag, tlv_0x2F->apn_disabled_flag);
    } else {
        out->profile_settings.apn_disabled_flag = false;
    }

    hdr = search_TLV(msg, 0x30, payload_len);
    /* TLV 0x30 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x30_t *tlv_0x30;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x30_t);

        out->profile_settings.pdn_inactivity_timeout_available = true;

        hdr++;
        tlv_0x30 = (QMI_WDS_generic_profile_details_0x30_t *)hdr;
        COPY_MTOH(out->profile_settings.pdn_inactivity_timeout, tlv_0x30->pdn_inactivity_timeout);
    }

    hdr = search_TLV(msg, 0x31, payload_len);
    /* TLV 0x31 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_0x31_t *tlv_0x31;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_0x31_t);

        out->profile_settings.apn_class_available = true;

        hdr++;
        tlv_0x31 = (QMI_WDS_generic_profile_details_0x31_t *)hdr;
        COPY_MTOH(out->profile_settings.apn_class, tlv_0x31->apn_class);
    }

    hdr = search_TLV(msg, 0x36, payload_len);
    /* TLV 0x36 is optional */
    if (hdr) {
        QMI_WDS_generic_profile_details_emergency_t *tlv_0x36;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_details_emergency_t);

        hdr++;
        tlv_0x36 = (QMI_WDS_generic_profile_details_emergency_t *)hdr;
        COPY_MTOH(out->profile_settings.support_emergency_calls, tlv_0x36->support_emergency_calls);
    } else {
        out->profile_settings.support_emergency_calls = false;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_WDS_GET_RUNTIME_SETTINGS (45 - 0x002D)
 */

QMI_SDK_error_t telit_wds_get_runtime_settings_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_runtime_settings_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WDS_GET_RUNTIME_SETTINGS_request_0x10_t *tlv_0x10;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_WDS_GET_RUNTIME_SETTINGS;
    req_ctx->svc_id = QMI_SVC_WDS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x10);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x10;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x10 = (QMI_WDS_GET_RUNTIME_SETTINGS_request_0x10_t *)buf_position;
    COPY_HTOM(tlv_0x10->requested_settings, input->requested_settings);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wds_get_runtime_settings_unpack(uint8_t *rsp, uint16_t len, wds_profile_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    uint8_t *buf_position;
    uint8_t i;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    /* Exiting for malformed responses and invalid args, since we want to collect protocol errors */
    if (ret != QMI_SDK_ERR_NONE && ret < QMI_SDK_ERR_ERR_QMI_OFFSET) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    } else if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        telit_wds_generic_extended_error_unpack(rsp, len, &out->extended_error_code);
        out->extended_error_code_available = true;
        goto end;
    }

    ret = telit_wds_common_profile_settings_unpack(msg, payload_len, out);
    if (ret != QMI_SDK_ERR_NONE) {
        goto end;
    }

    hdr = search_TLV(msg, 0x1F, payload_len);
    /* TLV 0x1F is optional */
    if (hdr) {
        QMI_WDS_generic_profile_id_t *tlv_0x1F;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_profile_id_t);

        out->profile_id_available = true;

        hdr++;
        tlv_0x1F = (QMI_WDS_generic_profile_id_t *)hdr;
        COPY_MTOH(out->profile_id.type, tlv_0x1F->profile_type);
        COPY_MTOH(out->profile_id.index, tlv_0x1F->profile_index);
    }

    hdr = search_TLV(msg, 0x20, payload_len);
    /* TLV 0x20 is optional */
    if (hdr) {
        QMI_WDS_generic_ipv4_addr_t *tlv_0x20;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_ipv4_addr_t);

        out->profile_settings.ipv4_gateway_addr_available = true;

        hdr++;
        tlv_0x20 = (QMI_WDS_generic_ipv4_addr_t *)hdr;
        /* No conversion for this, since we want the address in network byte order */
        out->profile_settings.ipv4_gateway_addr = tlv_0x20->ipv4_addr;
    }

    hdr = search_TLV(msg, 0x21, payload_len);
    /* TLV 0x21 is optional */
    if (hdr) {
        QMI_WDS_generic_ipv4_addr_t *tlv_0x21;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_ipv4_addr_t);

        out->profile_settings.ipv4_subnet_mask_available = true;

        hdr++;
        tlv_0x21 = (QMI_WDS_generic_ipv4_addr_t *)hdr;
        /* No conversion for this, since we want the address in network byte order */
        out->profile_settings.ipv4_subnet_mask_addr = tlv_0x21->ipv4_addr;
    }

    hdr = search_TLV(msg, 0x22, payload_len);
    /* TLV 0x22 is optional */
    if (hdr) {
        QMI_WDS_generic_pcscf_addr_with_pco_t *tlv_0x22;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_pcscf_addr_with_pco_t);

        hdr++;
        tlv_0x22 = (QMI_WDS_generic_pcscf_addr_with_pco_t *)hdr;
        COPY_MTOH(out->profile_settings.pcscf_addr_using_pco, tlv_0x22->pcscf_addr_using_pco);
    } else {
        out->profile_settings.pcscf_addr_using_pco = false;
    }

    hdr = search_TLV(msg, 0x23, payload_len);
    /* TLV 0x23 is optional */
    if (hdr) {
        QMI_WDS_GET_RUNTIME_SETTINGS_response_0x23_t *tlv_0x23;
        uint32_t *addr;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_GET_RUNTIME_SETTINGS_response_0x23_t);

        hdr++;
        tlv_0x23 = (QMI_WDS_GET_RUNTIME_SETTINGS_response_0x23_t *)hdr;
        COPY_MTOH(out->profile_settings.pcscf_ipv4_address_size, tlv_0x23->pcscf_ipv4_address_size);

        if (out->profile_settings.pcscf_ipv4_address_size > QMI_WDS_IPV4_PCSCF_ADDR_LIST_MAX_SIZE) {
            RLOGE(
                    "%s: pcscf_ipv4_address_size %u > QMI_WDS_IPV4_PCSCF_ADDR_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__,
                    out->profile_settings.pcscf_ipv4_address_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        buf_position = &(tlv_0x23->pcscf_ipv4_address_size);
        /* Moving to the start of the list */
        buf_position += sizeof(tlv_0x23->pcscf_ipv4_address_size);
        addr = (uint32_t *)buf_position;
        for (i = 0; i < out->profile_settings.pcscf_ipv4_address_size; i++) {
            /* No conversion for this, since we want the address in network byte order */
            out->profile_settings.pcscf_ipv4_address[i] = addr[i];
        }
    }

    hdr = search_TLV(msg, 0x24, payload_len);
    /* TLV 0x24 is optional */
    if (hdr) {
        QMI_WDS_GET_RUNTIME_SETTINGS_response_0x24_t *tlv_0x24;
        QMI_WDS_generic_fqdn_t *fqdn;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_GET_RUNTIME_SETTINGS_response_0x24_t);

        hdr++;
        tlv_0x24 = (QMI_WDS_GET_RUNTIME_SETTINGS_response_0x24_t *)hdr;
        COPY_MTOH(out->profile_settings.fqdn_size, tlv_0x24->fqdn_size);

        if (out->profile_settings.fqdn_size > QMI_WDS_FQDN_LIST_MAX_SIZE) {
            RLOGE("%s: fqdn_size %u > QMI_WDS_FQDN_LIST_MAX_SIZE, increase the define", __FUNCTION__,
                    out->profile_settings.fqdn_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        buf_position = &(tlv_0x24->fqdn_size);
        /* Moving to the start of the list */
        buf_position += sizeof(tlv_0x24->fqdn_size);
        for (i = 0; i < out->profile_settings.fqdn_size; i++) {
            fqdn = (QMI_WDS_generic_fqdn_t *)buf_position;
            COPY_MTOH(out->profile_settings.fqdn[i].fqdn_len, fqdn->fqdn_len);

            if (out->profile_settings.fqdn[i].fqdn_len > QMI_WDS_MAX_STRING_SIZE_PROFILE) {
                RLOGE("%s: fqdn_len %u > QMI_WDS_MAX_STRING_SIZE_PROFILE, increase the define",
                        __FUNCTION__, out->profile_settings.fqdn[i].fqdn_len);
                ret = QMI_SDK_ERR_INTERNAL;
                goto end;
            }

            buf_position = (uint8_t *)(&(fqdn->fqdn_len));

            /* Moving to the start of the list */
            buf_position += sizeof(fqdn->fqdn_len);
            memcpy(out->profile_settings.fqdn[i].fqdn, buf_position,
                    out->profile_settings.fqdn[i].fqdn_len);
            buf_position += out->profile_settings.fqdn[i].fqdn_len;
        }
    }

    hdr = search_TLV(msg, 0x25, payload_len);
    /* TLV 0x25 is optional */
    if (hdr) {
        QMI_WDS_generic_ipv6_addr_with_prefix_t *tlv_0x25;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_ipv6_addr_with_prefix_t);

        hdr++;
        tlv_0x25 = (QMI_WDS_generic_ipv6_addr_with_prefix_t *)hdr;
        COPY_MTOH(out->profile_settings.ipv6_addr.prefix_len, tlv_0x25->ipv6_prefix_length);
        for (i = 0; i < IPV6_ADDR_SIZE; i++) {
            /* No conversion, since arriving in big endian and needs to be kept in big endian */
            out->profile_settings.ipv6_addr.addr[i] = tlv_0x25->ipv6_addr[i];
        }
        out->profile_settings.ipv6_addr_available = true;
    }

    hdr = search_TLV(msg, 0x26, payload_len);
    /* TLV 0x26 is optional */
    if (hdr) {
        QMI_WDS_generic_ipv6_addr_with_prefix_t *tlv_0x26;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_ipv6_addr_with_prefix_t);

        hdr++;
        tlv_0x26 = (QMI_WDS_generic_ipv6_addr_with_prefix_t *)hdr;
        COPY_MTOH(out->profile_settings.ipv6_gw_addr.prefix_len, tlv_0x26->ipv6_prefix_length);
        for (i = 0; i < IPV6_ADDR_SIZE; i++) {
            /* No conversion, since arriving in big endian and needs to be kept in big endian */
            out->profile_settings.ipv6_gw_addr.addr[i] = tlv_0x26->ipv6_addr[i];
        }
        out->profile_settings.ipv6_gw_addr_available = true;
    }

    hdr = search_TLV(msg, 0x27, payload_len);
    /* TLV 0x27 is optional */
    if (hdr) {
        QMI_WDS_generic_ipv6_addr_t *tlv_0x27;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_ipv6_addr_t);

        hdr++;
        tlv_0x27 = (QMI_WDS_generic_ipv6_addr_t *)hdr;
        for (i = 0; i < IPV6_ADDR_SIZE; i++) {
            /* No conversion, since arriving in big endian and needs to be kept in big endian */
            out->profile_settings.pri_dns_ipv6_addr[i] = tlv_0x27->ipv6_addr[i];
        }
        out->profile_settings.pri_dns_ipv6_addr_available = true;
    }

    hdr = search_TLV(msg, 0x28, payload_len);
    /* TLV 0x28 is optional */
    if (hdr) {
        QMI_WDS_generic_ipv6_addr_t *tlv_0x28;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_ipv6_addr_t);

        hdr++;
        tlv_0x28 = (QMI_WDS_generic_ipv6_addr_t *)hdr;
        for (i = 0; i < IPV6_ADDR_SIZE; i++) {
            /* No conversion, since arriving in big endian and needs to be kept in big endian */
            out->profile_settings.sec_dns_ipv6_addr[i] = tlv_0x28->ipv6_addr[i];
        }
        out->profile_settings.sec_dns_ipv6_addr_available = true;
    }

    hdr = search_TLV(msg, 0x29, payload_len);
    /* TLV 0x29 is optional */
    if (hdr) {
        QMI_WDS_GET_RUNTIME_SETTINGS_response_0x29_t *tlv_0x29;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_GET_RUNTIME_SETTINGS_response_0x29_t);

        out->profile_settings.mtu_available = true;

        hdr++;
        tlv_0x29 = (QMI_WDS_GET_RUNTIME_SETTINGS_response_0x29_t *)hdr;
        COPY_MTOH(out->profile_settings.mtu, tlv_0x29->mtu);
    }

    hdr = search_TLV(msg, 0x2A, payload_len);
    /* TLV 0x2A is optional */
    if (hdr) {
        QMI_WDS_GET_RUNTIME_SETTINGS_response_0x2A_t *tlv_0x2A;
        QMI_WDS_generic_domain_name_t *domain_name;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_GET_RUNTIME_SETTINGS_response_0x2A_t);

        hdr++;
        tlv_0x2A = (QMI_WDS_GET_RUNTIME_SETTINGS_response_0x2A_t *)hdr;
        COPY_MTOH(out->profile_settings.domain_name_list_size, tlv_0x2A->domain_name_list_size);

        if (out->profile_settings.domain_name_list_size > QMI_DOMAIN_NAME_LIST_MAX_SIZE) {
            RLOGE("%s: domain_name_list_size %u > QMI_DOMAIN_NAME_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__,
                    out->profile_settings.domain_name_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        buf_position = &(tlv_0x2A->domain_name_list_size);
        /* Moving to the start of the list */
        buf_position += sizeof(tlv_0x2A->domain_name_list_size);
        for (i = 0; i < out->profile_settings.domain_name_list_size; i++) {

            domain_name = (QMI_WDS_generic_domain_name_t *)buf_position;
            COPY_MTOH(out->profile_settings.domain_name_list[i].domain_name_len,
                    domain_name->domain_name_len);

            if (out->profile_settings.domain_name_list[i].domain_name_len > QMI_DOMAIN_NAME_MAX_SIZE) {
                RLOGE("%s: domain_name_len %u > QMI_DOMAIN_NAME_MAX_SIZE, increase the define",
                        __FUNCTION__, out->profile_settings.domain_name_list[i].domain_name_len);
                ret = QMI_SDK_ERR_INTERNAL;
                goto end;
            }

            buf_position = (uint8_t *)(&(domain_name->domain_name_len));
            /* Moving to the start of the list */
            buf_position += sizeof(domain_name->domain_name_len);
            memcpy(out->profile_settings.domain_name_list[i].domain_name, buf_position,
                    out->profile_settings.domain_name_list[i].domain_name_len);
            buf_position += out->profile_settings.domain_name_list[i].domain_name_len;
        }
    }

    hdr = search_TLV(msg, 0x2B, payload_len);
    /* TLV 0x2B is optional */
    if (hdr) {
        QMI_WDS_generic_ip_family_t *tlv_0x2B;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_ip_family_t);

        out->profile_settings.ip_family_available = true;

        hdr++;
        tlv_0x2B = (QMI_WDS_generic_ip_family_t *)hdr;
        COPY_MTOH(out->profile_settings.ip_family, tlv_0x2B->ip_family);
    }

    hdr = search_TLV(msg, 0x2C, payload_len);
    /* TLV 0x2C is optional */
    if (hdr) {
        QMI_WDS_generic_im_cn_flag_t *tlv_0x2C;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_im_cn_flag_t);

        hdr++;
        tlv_0x2C = (QMI_WDS_generic_im_cn_flag_t *)hdr;
        COPY_MTOH(out->profile_settings.im_cn_flag, tlv_0x2C->im_cn_flag);
    } else {
        out->profile_settings.im_cn_flag = false;
    }

    hdr = search_TLV(msg, 0x2D, payload_len);
    /* TLV 0x2D is optional */
    if (hdr) {
        QMI_WDS_generic_tech_name_t *tlv_0x2D;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_tech_name_t);

        out->profile_settings.technology_name_available = true;

        hdr++;
        tlv_0x2D = (QMI_WDS_generic_tech_name_t *)hdr;
        COPY_MTOH(out->profile_settings.technology_name, tlv_0x2D->technology_name);
    }

    hdr = search_TLV(msg, 0x2E, payload_len);
    /* TLV 0x2E is optional */
    if (hdr) {
        QMI_WDS_GET_RUNTIME_SETTINGS_response_0x2E_t *tlv_0x2E;
        uint8_t *buf_position;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_GET_RUNTIME_SETTINGS_response_0x2E_t);

        hdr++;
        tlv_0x2E = (QMI_WDS_GET_RUNTIME_SETTINGS_response_0x2E_t *)hdr;
        COPY_MTOH(out->profile_settings.pcscf_ipv6_addr_list_sz, tlv_0x2E->pcscf_ipv6_addr_list_sz);
        if (out->profile_settings.pcscf_ipv6_addr_list_sz > QMI_WDS_PCSCF_IPV6_LIST_MAX_SIZE) {
            RLOGE("%s: pcscf_ipv6_addr_list_sz %u > QMI_WDS_PCSCF_IPV6_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, out->profile_settings.pcscf_ipv6_addr_list_sz);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = &tlv_0x2E->pcscf_ipv6_addr_list_sz;
        buf_position += sizeof(tlv_0x2E->pcscf_ipv6_addr_list_sz);
        for (int j = 0; j < out->profile_settings.pcscf_ipv6_addr_list_sz; j++) {
            uint16_t *ipv6_addr = (uint16_t *)buf_position;
            for (i = 0; i < IPV6_ADDR_SIZE; i++) {
                /* No conversion, since arriving in big endian and needs to be kept in big endian */
                out->profile_settings.pcscf_ipv6_addr_list[j][i] = ipv6_addr[i];
            }
            buf_position += sizeof(QMI_WDS_generic_ipv6_addr_t);
        }
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_WDS_GET_CALL_DURATION (53 - 0x0035)
 */

QMI_SDK_error_t telit_wds_get_call_duration_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_WDS_GET_CALL_DURATION,
            QMI_SVC_WDS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_wds_get_call_duration_unpack(uint8_t *rsp, uint16_t len, wds_duration_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_WDS_GET_CALL_DURATION_response_0x01_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_WDS_GET_CALL_DURATION_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_WDS_GET_CALL_DURATION_response_0x01_t *)hdr;
    COPY_MTOH(out->call_duration, tlv_0x01->call_duration);

end:
    return ret;
}

/*****************************************************************************
 * QMI_WDS_GET_DEFAULT_PROFILE_NUM (73 - 0x0049)
 */

QMI_SDK_error_t telit_wds_get_default_profile_num_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_default_profile_input_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WDS_GET_DEFAULT_PROFILE_NUM_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_WDS_GET_DEFAULT_PROFILE_NUM;
    req_ctx->svc_id = QMI_SVC_WDS;
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

    tlv_0x01 = (QMI_WDS_GET_DEFAULT_PROFILE_NUM_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->profile_type, input->profile_type);
    COPY_HTOM(tlv_0x01->profile_family, input->profile_family);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wds_get_default_profile_num_unpack(uint8_t *rsp,
        uint16_t len, wds_default_profile_out_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_WDS_GET_DEFAULT_PROFILE_NUM_response_0x01_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_WDS_GET_DEFAULT_PROFILE_NUM_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_WDS_GET_DEFAULT_PROFILE_NUM_response_0x01_t *)hdr;
    COPY_MTOH(out->profile_index, tlv_0x01->profile_index);

end:
    return ret;
}

/*****************************************************************************
 * QMI_WDS_SET_CLIENT_IP_FAMILY_PREF (77 - 0x004D)
 */

QMI_SDK_error_t telit_wds_set_client_ip_family_pref_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_ip_family_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WDS_SET_CLIENT_IP_FAMILY_PREF_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_WDS_SET_CLIENT_IP_FAMILY_PREF;
    req_ctx->svc_id = QMI_SVC_WDS;
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

    tlv_0x01 = (QMI_WDS_SET_CLIENT_IP_FAMILY_PREF_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->ip_preference, input->ip_preference);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wds_set_client_ip_family_pref_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_WDS_SET_LTE_ATTACH_PDN_LIST (147 - 0x0093)
 */

QMI_SDK_error_t telit_wds_set_lte_attach_pdn_list_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_attach_pdn_list_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_WDS_generic_attach_pdn_list_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;
    uint16_t *profile_id;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_WDS_SET_LTE_ATTACH_PDN_LIST;
    req_ctx->svc_id = QMI_SVC_WDS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->attach_pdn_list_size * sizeof(input->attach_pdn_list[0]);

    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_WDS_generic_attach_pdn_list_t *)buf_position;
    COPY_HTOM(tlv_0x01->attach_pdn_list_size, input->attach_pdn_list_size);
    buf_position = &(tlv_0x01->attach_pdn_list_size);
    /* Moving to the start of the list */
    buf_position += sizeof(tlv_0x01->attach_pdn_list_size);
    profile_id = (uint16_t *)buf_position;
    for (int i = 0; i < input->attach_pdn_list_size; i++) {
        COPY_HTOM(profile_id[i], input->attach_pdn_list[i]);
    }
    buf_position += (input->attach_pdn_list_size * sizeof(input->attach_pdn_list[0]));

    if (input->action_available) {
        QMI_WDS_SET_LTE_ATTACH_PDN_LIST_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_WDS_SET_LTE_ATTACH_PDN_LIST_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->action, input->action);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wds_set_lte_attach_pdn_list_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_WDS_GET_LTE_ATTACH_PDN_LIST (148 - 0x0094)
 */

QMI_SDK_error_t telit_wds_get_lte_attach_pdn_list_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_WDS_GET_LTE_ATTACH_PDN_LIST,
            QMI_SVC_WDS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_wds_get_lte_attach_pdn_list_unpack(uint8_t *rsp, uint16_t len,
        wds_get_attach_pdn_list_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    uint8_t *buf_position;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_WDS_generic_attach_pdn_list_t *tlv_0x10;
        uint16_t *profile_id;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_attach_pdn_list_t);

        hdr++;
        tlv_0x10 = (QMI_WDS_generic_attach_pdn_list_t *)hdr;
        COPY_MTOH(out->attach_pdn_list_size, tlv_0x10->attach_pdn_list_size);
        if (out->attach_pdn_list_size > QMI_ATTACH_PDN_LIST_MAX_SIZE) {
            RLOGE("%s: attach_pdn_list_size %u > QMI_ATTACH_PDN_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, out->attach_pdn_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = &tlv_0x10->attach_pdn_list_size;
        buf_position += sizeof(tlv_0x10->attach_pdn_list_size);
        profile_id = (uint16_t *)buf_position;
        for (int i = 0; i < out->attach_pdn_list_size; i++)
            COPY_MTOH(out->attach_pdn_list[i], profile_id[i]);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_WDS_generic_attach_pdn_list_t *tlv_0x11;
        uint16_t *profile_id;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_attach_pdn_list_t);

        hdr++;
        tlv_0x11 = (QMI_WDS_generic_attach_pdn_list_t *)hdr;
        COPY_MTOH(out->pending_attach_pdn_list_size, tlv_0x11->attach_pdn_list_size);
        if (out->pending_attach_pdn_list_size > QMI_ATTACH_PDN_LIST_MAX_SIZE) {
            RLOGE("%s: pending_attach_pdn_list_size %u > QMI_ATTACH_PDN_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, out->pending_attach_pdn_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = &tlv_0x11->attach_pdn_list_size;
        buf_position += sizeof(tlv_0x11->attach_pdn_list_size);
        profile_id = (uint16_t *)buf_position;
        for (int i = 0; i < out->pending_attach_pdn_list_size; i++)
            COPY_MTOH(out->pending_attach_pdn_list[i], profile_id[i]);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_WDS_BIND_MUX_DATA_PORT (162 - 0x00A2)
 */

QMI_SDK_error_t telit_wds_bind_mux_data_port_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_bind_mux_data_port_t *input)
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

    req_ctx->msg_id = QMI_WDS_BIND_MUX_DATA_PORT;
    req_ctx->svc_id = QMI_SVC_WDS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->peripheral_endpoint_available) {
        QMI_WDS_BIND_MUX_DATA_PORT_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_WDS_BIND_MUX_DATA_PORT_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->ep_type, input->peripheral_endpoint.ep_type);
        COPY_HTOM(tlv_0x10->iface_id, input->peripheral_endpoint.iface_id);

        buf_position += tlv_sz;
    }

    if (input->mux_id_available) {
        QMI_WDS_generic_mux_id_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_WDS_generic_mux_id_t *)buf_position;
        COPY_HTOM(tlv_0x11->mux_id, input->mux_id);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_wds_bind_mux_data_port_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI WDS indications
 */

static QMI_SDK_error_t telit_wds_handle_event_ind(uint16_t *payload_len, uint8_t *msg,
        uint8_t *out, uint16_t out_len)
{
    wds_ind_event_report_t *ev_rep = (wds_ind_event_report_t *) out;
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;

    if (out_len < sizeof(*ev_rep)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }
    memset(ev_rep, 0, sizeof(*ev_rep));

    hdr = search_TLV(msg, 0x10, *payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_32_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_32_t);

        hdr++;
        tlv_0x10 = (QMI_WDS_generic_packet_stats_value_32_t *)hdr;
        ev_rep->tx_ok_count_available = true;
        COPY_MTOH(ev_rep->tx_ok_count, tlv_0x10->value_32);
    }

    hdr = search_TLV(msg, 0x11, *payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_32_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_32_t);

        hdr++;
        tlv_0x11 = (QMI_WDS_generic_packet_stats_value_32_t *)hdr;
        ev_rep->rx_ok_count_available = true;
        COPY_MTOH(ev_rep->rx_ok_count, tlv_0x11->value_32);
    }

    hdr = search_TLV(msg, 0x17, *payload_len);
    /* TLV 0x17 is optional */
    if (hdr) {
        QMI_WDS_EVENT_REPORT_indication_0x17_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_EVENT_REPORT_indication_0x17_t);

        hdr++;
        tlv_0x17 = (QMI_WDS_EVENT_REPORT_indication_0x17_t *)hdr;
        ev_rep->data_bearer_tech_available = true;
        COPY_MTOH(ev_rep->data_bearer_tech, tlv_0x17->data_bearer_tech);
    }

    hdr = search_TLV(msg, 0x18, *payload_len);
    /* TLV 0x18 is optional */
    if (hdr) {
        QMI_WDS_EVENT_REPORT_indication_0x18_t *tlv_0x18;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_EVENT_REPORT_indication_0x18_t);

        hdr++;
        tlv_0x18 = (QMI_WDS_EVENT_REPORT_indication_0x18_t *)hdr;
        ev_rep->dormancy_status_available = true;
        COPY_MTOH(ev_rep->dormancy_status, tlv_0x18->dormancy_status);
    }

    hdr = search_TLV(msg, 0x19, *payload_len);
    /* TLV 0x19 is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_64_t *tlv_0x19;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_64_t);

        hdr++;
        tlv_0x19 = (QMI_WDS_generic_packet_stats_value_64_t *)hdr;
        ev_rep->tx_ok_bytes_count_available = true;
        COPY_MTOH(ev_rep->tx_ok_bytes_count, tlv_0x19->value_64);
    }

    hdr = search_TLV(msg, 0x1A, *payload_len);
    /* TLV 0x1A is optional */
    if (hdr) {
        QMI_WDS_generic_packet_stats_value_64_t *tlv_0x1A;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_packet_stats_value_64_t);

        hdr++;
        tlv_0x1A = (QMI_WDS_generic_packet_stats_value_64_t *)hdr;
        ev_rep->rx_ok_bytes_count_available = true;
        COPY_MTOH(ev_rep->rx_ok_bytes_count, tlv_0x1A->value_64);
    }

    hdr = search_TLV(msg, 0x1B, *payload_len);
    /* TLV 0x1B is optional */
    if (hdr) {
        QMI_WDS_EVENT_REPORT_indication_0x1B_t *tlv_0x1B;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_EVENT_REPORT_indication_0x1B_t);

        hdr++;
        tlv_0x1B = (QMI_WDS_EVENT_REPORT_indication_0x1B_t *)hdr;
        ev_rep->mip_status_available = true;
        COPY_MTOH(ev_rep->mip_status, tlv_0x1B->mip_status);
    }

    hdr = search_TLV(msg, 0x1D, *payload_len);
    /* TLV 0x1D is optional */
    if (hdr) {
        QMI_WDS_EVENT_REPORT_indication_0x1D_t *tlv_0x1D;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_EVENT_REPORT_indication_0x1D_t);

        hdr++;
        tlv_0x1D = (QMI_WDS_EVENT_REPORT_indication_0x1D_t *)hdr;
        ev_rep->current_data_bearer_tech_available = true;
        COPY_MTOH(ev_rep->current_data_bearer_tech.current_nw, tlv_0x1D->current_nw);
        COPY_MTOH(ev_rep->current_data_bearer_tech.rat_mask, tlv_0x1D->rat_mask);
        COPY_MTOH(ev_rep->current_data_bearer_tech.so_mask, tlv_0x1D->so_mask);
    }

    hdr = search_TLV(msg, 0x1F, *payload_len);
    /* TLV 0x1F is optional */
    if (hdr) {
        QMI_WDS_EVENT_REPORT_indication_0x1F_t *tlv_0x1F;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_EVENT_REPORT_indication_0x1F_t);

        hdr++;
        tlv_0x1F = (QMI_WDS_EVENT_REPORT_indication_0x1F_t *)hdr;
        ev_rep->data_call_status_available = true;
        COPY_MTOH(ev_rep->data_call_status, tlv_0x1F->data_call_status);
    }

    hdr = search_TLV(msg, 0x24, *payload_len);
    /* TLV 0x24 is optional */
    if (hdr) {
        QMI_WDS_EVENT_REPORT_indication_0x24_t *tlv_0x24;
        QMI_WDS_EVENT_REPORT_network_info_t *network_info_tmp;
        uint8_t *content;
        uint8_t i;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_EVENT_REPORT_indication_0x24_t);

        hdr++;
        tlv_0x24 = (QMI_WDS_EVENT_REPORT_indication_0x24_t *)hdr;
        ev_rep->data_system_status_available = true;
        COPY_MTOH(ev_rep->data_system_status.preferred_network, tlv_0x24->preferred_network);
        COPY_MTOH(ev_rep->data_system_status.network_info_size, tlv_0x24->network_info_size);

        if (ev_rep->data_system_status.network_info_size > QMI_WDS_MAX_NETWORK_INFO_SIZE) {
            RLOGE("%s: network_info_size %u > QMI_WDS_MAX_NETWORK_INFO_SIZE, increase the define",
                    __FUNCTION__, ev_rep->data_system_status.network_info_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        content = (uint8_t *)(&(tlv_0x24->network_info_size));
        /* Moving to the start of the array */
        content += sizeof(tlv_0x24->network_info_size);
        for (i = 0; i < ev_rep->data_system_status.network_info_size; i++) {
            network_info_tmp = (QMI_WDS_EVENT_REPORT_network_info_t *)content;

            COPY_MTOH(ev_rep->data_system_status.network_info[i].network, network_info_tmp->network);
            COPY_MTOH(ev_rep->data_system_status.network_info[i].rat_mask, network_info_tmp->rat_mask);
            COPY_MTOH(ev_rep->data_system_status.network_info[i].so_mask, network_info_tmp->so_mask);

            content += sizeof(*network_info_tmp);
        }
    }

    hdr = search_TLV(msg, 0x2A, *payload_len);
    /* TLV 0x2A is optional */
    if (hdr) {
        QMI_WDS_EVENT_REPORT_indication_0x2A_t *tlv_0x2A;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_EVENT_REPORT_indication_0x2A_t);

        hdr++;
        tlv_0x2A = (QMI_WDS_EVENT_REPORT_indication_0x2A_t *)hdr;
        ev_rep->data_bearer_tech_ext_available = true;
        COPY_MTOH(ev_rep->data_bearer_tech_ext.technology, tlv_0x2A->technology);
        COPY_MTOH(ev_rep->data_bearer_tech_ext.rat_value, tlv_0x2A->rat_value);
        COPY_MTOH(ev_rep->data_bearer_tech_ext.so_mask, tlv_0x2A->so_mask);
    }

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

static QMI_SDK_error_t telit_wds_handle_pkt_srvc_status_ind(uint16_t *payload_len, uint8_t *msg,
        uint8_t *out, uint16_t out_len)
{
    wds_ind_session_state_t *ev_rep = (wds_ind_session_state_t *) out;
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_WDS_PKT_SRVC_STATUS_indication_0x01_t *tlv_0x01;

    if (out_len < sizeof(*ev_rep)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }
    memset(ev_rep, 0, sizeof(*ev_rep));

    FIND_MANDATORY_TLV(msg, 0x01, *payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_WDS_PKT_SRVC_STATUS_indication_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_WDS_PKT_SRVC_STATUS_indication_0x01_t *)hdr;
    COPY_MTOH(ev_rep->connection_status, tlv_0x01->connection_status);
    COPY_MTOH(ev_rep->reconfiguration_required, tlv_0x01->reconfiguration_required);

    hdr = search_TLV(msg, 0x10, *payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_WDS_generic_call_end_reason_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_call_end_reason_t);

        hdr++;
        tlv_0x10 = (QMI_WDS_generic_call_end_reason_t *)hdr;
        ev_rep->call_end_reason_available = true;
        COPY_MTOH(ev_rep->call_end_reason, tlv_0x10->call_end_reason);
    }

    hdr = search_TLV(msg, 0x11, *payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_WDS_generic_verbose_call_end_reason_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_verbose_call_end_reason_t);

        hdr++;
        tlv_0x11 = (QMI_WDS_generic_verbose_call_end_reason_t *)hdr;
        ev_rep->call_end_reason_verbose_available = true;
        COPY_MTOH(ev_rep->call_end_reason_verbose.call_end_reason_type, tlv_0x11->call_end_reason_type);
        COPY_MTOH(ev_rep->call_end_reason_verbose.call_end_reason, tlv_0x11->call_end_reason);
    }

    hdr = search_TLV(msg, 0x12, *payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_WDS_generic_ip_family_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_ip_family_t);

        hdr++;
        tlv_0x12 = (QMI_WDS_generic_ip_family_t *)hdr;
        ev_rep->ip_family_available = true;
        COPY_MTOH(ev_rep->ip_family, tlv_0x12->ip_family);
    }

    hdr = search_TLV(msg, 0x13, *payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_WDS_generic_tech_name_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_generic_tech_name_t);

        hdr++;
        tlv_0x13 = (QMI_WDS_generic_tech_name_t *)hdr;
        ev_rep->tech_name_available = true;
        COPY_MTOH(ev_rep->tech_name, tlv_0x13->technology_name);
    }

    hdr = search_TLV(msg, 0x14, *payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_WDS_PKT_SRVC_STATUS_indication_0x14_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_PKT_SRVC_STATUS_indication_0x14_t);

        hdr++;
        tlv_0x14 = (QMI_WDS_PKT_SRVC_STATUS_indication_0x14_t *)hdr;
        ev_rep->bearer_id_available = true;
        COPY_MTOH(ev_rep->bearer_id, tlv_0x14->bearer_id);
    }

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

static QMI_SDK_error_t telit_wds_handle_apn_info_change_ind(uint16_t *payload_len, uint8_t *msg,
        uint8_t *out, uint16_t out_len)
{
    wds_ind_apn_change_t *ev_rep = (wds_ind_apn_change_t *) out;
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint8_t *skip = NULL;

    if (out_len < sizeof(*ev_rep)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }
    memset(ev_rep, 0, sizeof(*ev_rep));

    hdr = search_TLV(msg, 0x10, *payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_WDS_APN_PARAM_INFO_CHANGE_indication_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_APN_PARAM_INFO_CHANGE_indication_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_WDS_APN_PARAM_INFO_CHANGE_indication_0x10_t *)hdr;
        ev_rep->apn_string_info_available = true;
        COPY_MTOH(ev_rep->apn_string_info.old_apn_len, tlv_0x10->old_apn_string_len);
        skip = (uint8_t *)(&(tlv_0x10->old_apn_string_len));
        skip++;
        memcpy(ev_rep->apn_string_info.old_apn, skip,
                ev_rep->apn_string_info.old_apn_len);
        skip += ev_rep->apn_string_info.old_apn_len;
        ev_rep->apn_string_info.new_apn_len = *skip;
        skip++;
        memcpy(ev_rep->apn_string_info.new_apn, skip,
                ev_rep->apn_string_info.new_apn_len);
    }

    hdr = search_TLV(msg, 0x11, *payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_WDS_APN_PARAM_INFO_CHANGE_indication_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_WDS_APN_PARAM_INFO_CHANGE_indication_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_WDS_APN_PARAM_INFO_CHANGE_indication_0x11_t *)hdr;
        ev_rep->pdp_type_info_available = true;
        COPY_MTOH(ev_rep->pdp_type_info.old_pdp_type, tlv_0x11->old_pdp_type);
        COPY_MTOH(ev_rep->pdp_type_info.new_pdp_type, tlv_0x11->new_pdp_type);
    }

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

QMI_SDK_error_t telit_wds_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len __unused,
        uint8_t *out, uint16_t out_len)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    uint16_t payload_len = 0;
    uint8_t *msg;

    if (!rsp || !out) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    payload_len = get_payload_size(rsp);
    msg = rsp + sizeof(QMI_service_raw_transaction_header_t) + \
            sizeof(QMI_raw_message_header_t);

    switch (msg_id) {
        case QMI_WDS_EVENT_REPORT_IND:
            ret = telit_wds_handle_event_ind(&payload_len, msg, out, out_len);
            if (ret != QMI_SDK_ERR_NONE) {
                RLOGE("%s: QMI_WDS_EVENT_IND error %u", __FUNCTION__, ret);
            }
            break;
        case QMI_WDS_PKT_SRVC_STATUS_IND:
            ret = telit_wds_handle_pkt_srvc_status_ind(&payload_len, msg, out, out_len);
            if (ret != QMI_SDK_ERR_NONE) {
                RLOGE("%s: QMI_WDS_PKT_SRVC_STATUS_IND error %u", __FUNCTION__, ret);
            }
            break;
        case QMI_WDS_APN_PARAM_INFO_CHANGE_IND:
            ret = telit_wds_handle_apn_info_change_ind(&payload_len, msg, out, out_len);
            if (ret != QMI_SDK_ERR_NONE) {
                RLOGE("%s: QMI_WDS_APN_PARAM_INFO_CHANGE_IND error %u", __FUNCTION__, ret);
            }
            break;
        default:
            RLOGW("%s: Indication not managed (%d)", __FUNCTION__, msg_id);
            break;
    }

end:
    return ret;
}
