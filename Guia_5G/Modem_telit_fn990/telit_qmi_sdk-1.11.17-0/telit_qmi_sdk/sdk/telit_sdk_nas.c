#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#if defined __WINDOWS_QMI__
#include "portable_endian.h"
#else
#include <endian.h>
#endif

#include "telit_sdk.h"
#include "telit_sdk_private.h"
#include "telit_sdk_private_nas.h"
#include "telit_sdk_nas.h"

/*****************************************************************************
 * QMI_NAS_SET_EVENT_REPORT (2 - 0x0002)
 */

QMI_SDK_error_t telit_nas_set_event_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_set_event_report_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_NAS_EVENT_REG_request_0x10_t *tlv_0x10;
    uint32_t hdr_sz;
    uint16_t tlv_sz;
    uint8_t *ptr = NULL;
    uint8_t i;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    if (input->threshold_size > QMI_NAS_SET_EVENT_REPORT_MAX_THRESHOLD) {
        RLOGE("%s: threshold_size = %u > QMI_NAS_SET_EVENT_REPORT_MAX_THRESHOLD, increase the define",
                __FUNCTION__, input->threshold_size);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    req_ctx->msg_id = QMI_NAS_SET_EVENT_REPORT;
    req_ctx->svc_id = QMI_SVC_NAS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x10) + input->threshold_size;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x10;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x10 = (QMI_NAS_EVENT_REG_request_0x10_t *)buf_position;

    tlv_0x10->report_signal_strength = 1;
    COPY_HTOM(tlv_0x10->number_of_thresholds, input->threshold_size);
    ptr = (uint8_t *)(&(tlv_0x10->number_of_thresholds));
    ptr += sizeof(tlv_0x10->number_of_thresholds);

    for (i = 0; (i < input->threshold_size); i++) {
        ec_htom(ptr, sizeof(*ptr),
                &(input->threshold[i]), sizeof(input->threshold[i]));
        ptr++;
    }

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_nas_set_event_report_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_NAS_INDICATION_REGISTER (3 - 0x0003)
 */

QMI_SDK_error_t telit_nas_indication_register_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_indication_register_t *input)
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

    req_ctx->msg_id = QMI_NAS_INDICATION_REGISTER;
    req_ctx->svc_id = QMI_SVC_NAS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->serving_system < QMI_IND_ACTION_NONE) {
        QMI_NAS_INDICATION_REGISTER_request_0x13_t *tlv_0x13;

        tlv_sz = sizeof(*tlv_0x13);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x13 = (QMI_NAS_INDICATION_REGISTER_request_0x13_t *)buf_position;
        tlv_0x13->req_serving_system = input->serving_system ? 1 : 0;
        buf_position += tlv_sz;
    }

    if (input->reg_network_time < QMI_IND_ACTION_NONE) {
        QMI_NAS_INDICATION_REGISTER_request_0x17_t *tlv_0x17;

        tlv_sz = sizeof(*tlv_0x17);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x17;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x17 = (QMI_NAS_INDICATION_REGISTER_request_0x17_t *)buf_position;
        tlv_0x17->reg_network_time = input->reg_network_time ? 1 : 0;
        buf_position += tlv_sz;
    }

    if (input->sys_info < QMI_IND_ACTION_NONE) {
        QMI_NAS_INDICATION_REGISTER_request_0x18_t *tlv_0x18;

        tlv_sz = sizeof(*tlv_0x18);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x18;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x18 = (QMI_NAS_INDICATION_REGISTER_request_0x18_t *)buf_position;
        tlv_0x18->req_sys_info = input->sys_info ? 1 : 0;
        buf_position += tlv_sz;
    }

    if (input->sig_info < QMI_IND_ACTION_NONE) {
        QMI_NAS_INDICATION_REGISTER_request_0x19_t *tlv_0x19;

        tlv_sz = sizeof(*tlv_0x19);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x19;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x19 = (QMI_NAS_INDICATION_REGISTER_request_0x19_t *)buf_position;
        tlv_0x19->req_sig_info = input->sig_info ? 1 : 0;
        buf_position += tlv_sz;
    }

    if (input->reg_operator_name_data < QMI_IND_ACTION_NONE) {
        QMI_NAS_INDICATION_REGISTER_request_0x22_t *tlv_0x22;

        tlv_sz = sizeof(*tlv_0x22);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x22;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x22 = (QMI_NAS_INDICATION_REGISTER_request_0x22_t *)buf_position;
        tlv_0x22->reg_operator_name_data = input->reg_operator_name_data ? 1 : 0;
        buf_position += tlv_sz;
    }

    if (input->reg_csp_plmn_mode_bit < QMI_IND_ACTION_NONE) {
        QMI_NAS_INDICATION_REGISTER_request_0x23_t *tlv_0x23;

        tlv_sz = sizeof(*tlv_0x23);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x23;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x23 = (QMI_NAS_INDICATION_REGISTER_request_0x23_t *)buf_position;
        tlv_0x23->reg_csp_plmn_mode_bit = input->reg_csp_plmn_mode_bit ? 1 : 0;
        buf_position += tlv_sz;
    }

    if (input->reg_nr5g_time_sync_pulse_report < QMI_IND_ACTION_NONE) {
        QMI_NAS_INDICATION_REGISTER_request_0x4F_t *tlv_0x4F;

        tlv_sz = sizeof(*tlv_0x4F);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *) buf_position;
        hdr->type_id = 0x4F;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x4F = (QMI_NAS_INDICATION_REGISTER_request_0x4F_t *) buf_position;
        tlv_0x4F->req_reg_nr5g_time_sync_pulse_report =
                input->reg_nr5g_time_sync_pulse_report ? 1 : 0;
        buf_position += tlv_sz;
    }

    if (input->reg_nr5g_lost_sync_frame < QMI_IND_ACTION_NONE) {
        QMI_NAS_INDICATION_REGISTER_request_0x50_t *tlv;

        tlv_sz = sizeof(*tlv);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *) buf_position;
        hdr->type_id = 0x50;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_NAS_INDICATION_REGISTER_request_0x50_t *) buf_position;
        tlv->reg_nr5g_lost_sync_frame = input->reg_nr5g_lost_sync_frame ? 1 : 0;
        buf_position += tlv_sz;
    }

    if (input->reg_nr5g_utc_time_update < QMI_IND_ACTION_NONE) {
        QMI_NAS_INDICATION_REGISTER_request_0x5E_t *tlv;

        tlv_sz = sizeof(*tlv);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *) buf_position;
        hdr->type_id = 0x5E;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_NAS_INDICATION_REGISTER_request_0x5E_t *) buf_position;
        tlv->reg_nr5g_utc_time_update = input->reg_nr5g_utc_time_update ? 1 : 0;
        buf_position += tlv_sz;
    }

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_nas_indication_register_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_NAS_GET_SIGNAL_STRENGTH (32 - 0x0020)
 */

QMI_SDK_error_t telit_nas_get_signal_strength_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        nas_signal_strength_request_t *input)
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
        return QMI_SDK_ERR_INVALID_ARG;
    }

    req_ctx->msg_id = QMI_NAS_GET_SIGNAL_STRENGTH;
    req_ctx->svc_id = QMI_SVC_NAS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input) {
        QMI_NAS_GET_SIGNAL_STRENGTH_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_NAS_GET_SIGNAL_STRENGTH_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->signal_strength_req_mask, input->signal_strength_req_mask);

        add_header(req_ctx, payload_len, original_buffer, len);
    } else {
        ret = pack_request(req_ctx, req, len);
    }

end:
    return ret;
}

QMI_SDK_error_t telit_nas_get_signal_strength_unpack(uint8_t *rsp, uint16_t len, nas_signal_strength_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_NAS_GET_SIGNAL_STRENGTH_response_0x01_t *tlv_0x01;
    uint8_t i;
    uint8_t *skip = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SIGNAL_STRENGTH_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_NAS_GET_SIGNAL_STRENGTH_response_0x01_t *)hdr;

    /* First element of the RSSI list always contains the current Signal
     * strength and Radio Interface. */
    COPY_MTOH(out->signal_strength_list[0].signal_strength, tlv_0x01->signal_strength_dBm);
    COPY_MTOH(out->signal_strength_list[0].radio_if, tlv_0x01->radio_interface);
    out->signal_strength_list_size = 1;

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_NAS_GET_SIGNAL_STRENGTH_response_0x10_t *tlv_0x10;
        signal_strength_instance_t *signal_strength_instance = NULL;
        uint16_t instances = 0;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SIGNAL_STRENGTH_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_NAS_GET_SIGNAL_STRENGTH_response_0x10_t *)hdr;
        COPY_MTOH(instances, tlv_0x10->number_of_instances);

        if (instances + 1 > QMI_NAS_SIGNAL_STRENGTH_MAX_LIST_ELEMENTS) {
            RLOGE(
                    "%s: number_of_instances = %u > QMI_NAS_SIGNAL_STRENGTH_MAX_LIST_ELEMENTS, increase the define",
                    __FUNCTION__, instances + 1);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        out->signal_strength_list_size += instances;

        skip = (uint8_t *)(&(tlv_0x10->number_of_instances));
        skip += sizeof(tlv_0x10->number_of_instances);
        signal_strength_instance = (signal_strength_instance_t *)skip;

        if (instances > 0) {
            for (i = 1; i < (instances + 1); i++) {  /* (i = 1) because first list element is used */
                COPY_MTOH(out->signal_strength_list[i].signal_strength,
                        signal_strength_instance->signal_strength_dBm);
                COPY_MTOH(out->signal_strength_list[i].radio_if, signal_strength_instance->radio_interface);
                signal_strength_instance++;
            }
        } else {
            RLOGW("%s: no instances available", __FUNCTION__);
        }
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_NAS_GET_SIGNAL_STRENGTH_response_0x12_t *tlv_0x12;
        ecio_instance_t *ecio_instance = NULL;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SIGNAL_STRENGTH_response_0x12_t);

        hdr++;
        tlv_0x12 = (QMI_NAS_GET_SIGNAL_STRENGTH_response_0x12_t *)hdr;
        COPY_MTOH(out->ecio_list_size, tlv_0x12->number_of_instances);

        if (out->ecio_list_size  > QMI_NAS_SIGNAL_STRENGTH_MAX_LIST_ELEMENTS) {
            RLOGE("%s: ecio_list_size = %u > QMI_NAS_SIGNAL_STRENGTH_MAX_LIST_ELEMENTS, increase the define",
                    __FUNCTION__, out->ecio_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x12->number_of_instances));
        skip += sizeof(tlv_0x12->number_of_instances);
        ecio_instance = (ecio_instance_t *)skip;

        if (out->ecio_list_size > 0) {
            for (i = 0; i < out->ecio_list_size; i++) {
                COPY_MTOH(out->ecio_list[i].ecio, ecio_instance->ECIO);
                COPY_MTOH(out->ecio_list[i].radio_if, ecio_instance->radio_interface);
                ecio_instance++;
            }
        } else {
            RLOGW("%s: no instances available", __FUNCTION__);
        }
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_NAS_GET_SIGNAL_STRENGTH_response_0x13_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SIGNAL_STRENGTH_response_0x13_t);

        hdr++;
        tlv_0x13 = (QMI_NAS_GET_SIGNAL_STRENGTH_response_0x13_t *)hdr;
        out->io_available = true;
        COPY_MTOH(out->io, tlv_0x13->IO);
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_NAS_GET_SIGNAL_STRENGTH_response_0x14_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SIGNAL_STRENGTH_response_0x14_t);

        hdr++;
        tlv_0x14 = (QMI_NAS_GET_SIGNAL_STRENGTH_response_0x14_t *)hdr;
        out->sinr_available = true;
        COPY_MTOH(out->sinr, tlv_0x14->SINR);
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    /* TLV 0x15 is optional */
    if (hdr) {
        QMI_NAS_GET_SIGNAL_STRENGTH_response_0x15_t *tlv_0x15;
        error_instance_t *error_instance = NULL;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SIGNAL_STRENGTH_response_0x15_t);

        hdr++;
        tlv_0x15 = (QMI_NAS_GET_SIGNAL_STRENGTH_response_0x15_t *)hdr;
        COPY_MTOH(out->error_rate_list_size, tlv_0x15->number_of_instances);

        if (out->error_rate_list_size > QMI_NAS_SIGNAL_STRENGTH_MAX_LIST_ELEMENTS) {
            RLOGE(
                    "%s: error_rate_list_size = %u > QMI_NAS_SIGNAL_STRENGTH_MAX_LIST_ELEMENTS, increase the define",
                    __FUNCTION__, out->error_rate_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x15->number_of_instances));
        skip += sizeof(tlv_0x15->number_of_instances);
        error_instance = (error_instance_t *)skip;

        if (out->error_rate_list_size > 0) {
            for (i = 0; i < out->error_rate_list_size; i++) {
                COPY_MTOH(out->error_rate_list[i].error_rate, error_instance->error_rate);
                COPY_MTOH(out->error_rate_list[i].radio_if, error_instance->radio_interface);
                error_instance++;
            }
        } else {
            RLOGW("%s: no instances available", __FUNCTION__);
        }
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    /* TLV 0x16 is optional */
    if (hdr) {
        QMI_NAS_GET_SIGNAL_STRENGTH_response_0x16_t *tlv_0x16;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SIGNAL_STRENGTH_response_0x16_t);

        hdr++;
        tlv_0x16 = (QMI_NAS_GET_SIGNAL_STRENGTH_response_0x16_t *)hdr;
        out->rsrq_info_available = true;
        COPY_MTOH(out->rsrq_info.rsrq, tlv_0x16->RSRQ);
        COPY_MTOH(out->rsrq_info.radio_if, tlv_0x16->radio_interface);
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    /* TLV 0x17 is optional */
    if (hdr) {
        QMI_NAS_GET_SIGNAL_STRENGTH_response_0x17_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SIGNAL_STRENGTH_response_0x17_t);

        hdr++;
        tlv_0x17 = (QMI_NAS_GET_SIGNAL_STRENGTH_response_0x17_t *)hdr;
        out->lte_snr_available = true;
        COPY_MTOH(out->lte_snr, tlv_0x17->LTESNR);
    }

    hdr = search_TLV(msg, 0x18, payload_len);
    /* TLV 0x18 is optional */
    if (hdr) {
        QMI_NAS_GET_SIGNAL_STRENGTH_response_0x18_t *tlv_0x18;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SIGNAL_STRENGTH_response_0x18_t);

        hdr++;
        tlv_0x18 = (QMI_NAS_GET_SIGNAL_STRENGTH_response_0x18_t *)hdr;
        out->lte_rsrp_available = true;
        COPY_MTOH(out->lte_rsrp, tlv_0x18->LTERSRP);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_NAS_PERFORM_NETWORK_SCAN (33 - 0x0021)
 */

QMI_SDK_error_t telit_nas_perform_network_scan_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_network_scan_mask_t *input)
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
        return QMI_SDK_ERR_INVALID_ARG;
    }

    req_ctx->msg_id = QMI_NAS_PERFORM_NETWORK_SCAN;
    req_ctx->svc_id = QMI_SVC_NAS;
    req_ctx->timeout = QMI_TIMEOUT_BIG;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input) {
        QMI_NAS_SCAN_NETS_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_NAS_SCAN_NETS_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->network_scan_req_mask, input->network_scan_req_mask);

        add_header(req_ctx, payload_len, original_buffer, len);
    } else {
        ret = pack_request(req_ctx, req, len);
    }

end:
    return ret;
}

QMI_SDK_error_t telit_nas_perform_network_scan_unpack(uint8_t *rsp, uint16_t len, nas_network_scan_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    uint8_t i;
    uint8_t *skip = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_NAS_SCAN_NETS_response_0x10_t *tlv_0x10;
        network_info_instance_t *network_info_inst = NULL;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_SCAN_NETS_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_NAS_SCAN_NETS_response_0x10_t *)hdr;
        COPY_MTOH(out->network_3gpp_list_size, tlv_0x10->number_of_instances);

        if (out->network_3gpp_list_size > QMI_NAS_SCAN_NETS_MAX_LIST_ELEMENTS) {
            RLOGE(
                    "%s: network_3gpp_list_size = %u > QMI_NAS_SCAN_NETS_MAX_LIST_ELEMENTS, increase the define",
                    __FUNCTION__, out->network_3gpp_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x10->number_of_instances));
        skip += sizeof(tlv_0x10->number_of_instances);
        network_info_inst = (network_info_instance_t *)skip;

        if (out->network_3gpp_list_size > 0) {
            for (i = 0; i < out->network_3gpp_list_size; i++) {
                uint8_t *descr_ptr;
                uint8_t network_status;

                COPY_MTOH(out->network_3gpp_info_list[i].mobile_country_code, network_info_inst->mcc);
                COPY_MTOH(out->network_3gpp_info_list[i].mobile_network_code, network_info_inst->mnc);
                COPY_MTOH(network_status, network_info_inst->network_status);
                out->network_3gpp_info_list[i].in_use_status = (network_status & 0b11);
                out->network_3gpp_info_list[i].roaming_status = ((network_status >> 2) & 0b11);
                out->network_3gpp_info_list[i].forbidden_status = ((network_status >> 4) & 0b11);
                out->network_3gpp_info_list[i].preferred_status = ((network_status >> 6) & 0b11);
                COPY_MTOH(out->network_3gpp_info_list[i].description_len, network_info_inst->description_len);

                if (out->network_3gpp_info_list[i].description_len >
                        QMI_NAS_MAX_NETWORK_DESCRIPTION_LENGTH - 1) {
                    RLOGE(
                            "%s: description_len = %u > QMI_NAS_MAX_NETWORK_DESCRIPTION_LENGTH, increase the define",
                            __FUNCTION__, out->network_3gpp_info_list[i].description_len);
                    ret = QMI_SDK_ERR_INTERNAL;
                    goto end;
                }

                descr_ptr = &(network_info_inst->description_len);
                descr_ptr += sizeof(network_info_inst->description_len);

                memcpy(out->network_3gpp_info_list[i].description, descr_ptr,
                        out->network_3gpp_info_list[i].description_len);

                descr_ptr += out->network_3gpp_info_list[i].description_len;
                network_info_inst = (network_info_instance_t *) descr_ptr;
            }
        } else {
            RLOGW("%s: no instances available", __FUNCTION__);
        }
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_NAS_SCAN_NETS_response_0x11_t *tlv_0x11;
        rat_info_instance_t *rat_inst = NULL;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_SCAN_NETS_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_NAS_SCAN_NETS_response_0x11_t *)hdr;
        COPY_MTOH(out->rat_list_size, tlv_0x11->rat_info_list_size);

        if (out->rat_list_size > QMI_NAS_SCAN_NETS_MAX_LIST_ELEMENTS) {
            RLOGE("%s: rat_list_size = %u > QMI_NAS_SCAN_NETS_MAX_LIST_ELEMENTS, increase the define",
                    __FUNCTION__, out->rat_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x11->rat_info_list_size));
        skip += sizeof(tlv_0x11->rat_info_list_size);
        rat_inst = (rat_info_instance_t *)skip;

        if (out->rat_list_size > 0) {
            for (i = 0; i < out->rat_list_size; i++) {
                COPY_MTOH(out->rat_list[i].MCC, rat_inst->mcc);
                COPY_MTOH(out->rat_list[i].MNC, rat_inst->mnc);
                COPY_MTOH(out->rat_list[i].RAT, rat_inst->rat);
                rat_inst++;
            }
        } else {
            RLOGW("%s: no instances available", __FUNCTION__);
        }
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_NAS_SCAN_NETS_response_0x12_t *tlv_0x12;
        pcs_digit_instance_t *pcs_digit_inst = NULL;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_SCAN_NETS_response_0x12_t);

        hdr++;
        tlv_0x12 = (QMI_NAS_SCAN_NETS_response_0x12_t *)hdr;
        COPY_MTOH(out->pcs_digit_list_size, tlv_0x12->pcs_digit_list_size);

        if (out->pcs_digit_list_size > QMI_NAS_SCAN_NETS_MAX_LIST_ELEMENTS) {
            RLOGE("%s: pcs_digit_list_size = %u > QMI_NAS_SCAN_NETS_MAX_LIST_ELEMENTS, increase the define",
                    __FUNCTION__, out->pcs_digit_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x12->pcs_digit_list_size));
        skip += sizeof(tlv_0x12->pcs_digit_list_size);
        pcs_digit_inst = (pcs_digit_instance_t *)skip;

        if (out->pcs_digit_list_size > 0) {
            for (i = 0; i < out->pcs_digit_list_size; i++) {
                COPY_MTOH(out->pcs_digit_list[i].MCC, pcs_digit_inst->mcc);
                COPY_MTOH(out->pcs_digit_list[i].MNC, pcs_digit_inst->mnc);
                COPY_MTOH(out->pcs_digit_list[i].includes_pcs_digit, pcs_digit_inst->mcn_includes_pcs);
                pcs_digit_inst++;
            }
        } else {
            RLOGW("%s: no instances available", __FUNCTION__);
        }
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_NAS_SCAN_NETS_response_0x13_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_SCAN_NETS_response_0x13_t);

        hdr++;
        tlv_0x13 = (QMI_NAS_SCAN_NETS_response_0x13_t *)hdr;
        COPY_MTOH(out->scan_result, tlv_0x13->scan_result);
        out->scan_result_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_NAS_GET_SERVING_SYSTEM (36 - 0x0024)
 */

QMI_SDK_error_t telit_nas_get_serving_system_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_NAS_GET_SERVING_SYSTEM,
            QMI_SVC_NAS,
            QMI_TIMEOUT_SMALL);
}

static QMI_SDK_error_t telit_nas_generic_serving_system_unpack(uint8_t *rsp, uint16_t len,
        nas_serving_system_info_t *out,
        bool indication)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_NAS_GET_SS_INFO_response_0x01_t *tlv_0x01;
    uint8_t i;
    uint8_t *str;
    uint8_t *skip = NULL;
    uint8_t TLV = 0;

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

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_NAS_GET_SS_INFO_response_0x01_t *)hdr;
    COPY_MTOH(out->serving_system.reg_state, tlv_0x01->registration_state);
    COPY_MTOH(out->serving_system.cs_attach_state, tlv_0x01->CS_attach_state);
    COPY_MTOH(out->serving_system.ps_attach_state, tlv_0x01->PS_attach_state);
    COPY_MTOH(out->serving_system.sel_network, tlv_0x01->registered_network);
    COPY_MTOH(out->serving_system.radio_interface_size, tlv_0x01->number_of_radio_interfaces_in_use);

    if (out->serving_system.radio_interface_size > QMI_NAS_SERVING_SYSTEM_MAX_RADIO_IF) {
        RLOGE(
                "%s: serving_system.radio_interface_size = %u > QMI_NAS_SERVING_SYSTEM_MAX_RADIO_IF, increase the define",
                __FUNCTION__, out->serving_system.radio_interface_size);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    str = (uint8_t *)(&(tlv_0x01->number_of_radio_interfaces_in_use));
    str += sizeof(tlv_0x01->number_of_radio_interfaces_in_use);

    for (i = 0; i < out->serving_system.radio_interface_size; i++) {
        /* do not use COPY_MTOH, since it expects both DST and SRC not to be pointers */
        ec_mtoh(&(out->serving_system.radio_interface[i]), sizeof(out->serving_system.radio_interface[i]),
                str, sizeof(*str));
        str++;
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_NAS_GET_SS_INFO_response_0x10_t *)hdr;
        COPY_MTOH(out->roam_indicator_value, tlv_0x10->roaming_indicator);
        out->roam_indicator_value_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_NAS_GET_SS_INFO_response_0x11_t *)hdr;
        COPY_MTOH(out->data_caps_size, tlv_0x11->number_of_data_capabilities);

        if (out->data_caps_size > QMI_NAS_SERVING_SYSTEM_MAX_DATA_CAPS) {
            RLOGE("%s: data_caps_size = %u > QMI_NAS_SERVING_SYSTEM_MAX_DATA_CAPS, increase the define",
                    __FUNCTION__, out->data_caps_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        str = (uint8_t *)(&(tlv_0x11->number_of_data_capabilities));
        str += sizeof(tlv_0x11->number_of_data_capabilities);

        for (i = 0; i < tlv_0x11->number_of_data_capabilities; i++) {
            /* do not use COPY_MTOH, since it expects both DST and SRC not to be pointers */
            ec_mtoh(&(out->data_caps[i]), sizeof(out->data_caps[i]), str, sizeof(*str));
            str++;
        }
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x12_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x12_t);

        hdr++;
        tlv_0x12 = (QMI_NAS_GET_SS_INFO_response_0x12_t *)hdr;
        out->current_PLMN_available = true;
        COPY_MTOH(out->current_PLMN.mcc, tlv_0x12->mobile_country_code);
        COPY_MTOH(out->current_PLMN.mnc, tlv_0x12->mobile_network_code);
        COPY_MTOH(out->current_PLMN.net_descr_len, tlv_0x12->description_len);

        if (out->current_PLMN.net_descr_len > MAX_STRING_SIZE - 1) {
            RLOGE("%s: out->current_PLMN.net_descr_len = %u > MAX_STRING_SIZE, increase the define",
                    __FUNCTION__, out->current_PLMN.net_descr_len);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        str = (uint8_t *)(&(tlv_0x12->description_len));
        str += sizeof(tlv_0x12->description_len);

        memcpy(out->current_PLMN.network_name, str, out->current_PLMN.net_descr_len);
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x13_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x13_t);

        hdr++;
        tlv_0x13 = (QMI_NAS_GET_SS_INFO_response_0x13_t *)hdr;
        out->cdma_system_available = true;
        COPY_MTOH(out->cdma_system.cdma_system_id, tlv_0x13->system_ID);
        COPY_MTOH(out->cdma_system.cdma_network_id, tlv_0x13->network_ID);
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x14_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x14_t);

        hdr++;
        tlv_0x14 = (QMI_NAS_GET_SS_INFO_response_0x14_t *)hdr;
        out->base_station_available = true;
        COPY_MTOH(out->base_station.cdma_base_station_id, tlv_0x14->base_station_ID);
        COPY_MTOH(out->base_station.cdma_base_station_latitude, tlv_0x14->latitude);
        COPY_MTOH(out->base_station.cdma_base_station_longitude, tlv_0x14->longitude);
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    /* TLV 0x15 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x15_t *tlv_0x15;
        roaming_indicator_instance_t *roaming_indicator_instance = NULL;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x15_t);

        hdr++;
        tlv_0x15 = (QMI_NAS_GET_SS_INFO_response_0x15_t *)hdr;
        COPY_MTOH(out->roaming_indicators_size, tlv_0x15->num_instances);

        if (out->roaming_indicators_size > QMI_NAS_SERVING_SYSTEM_MAX_ROAMING_IND) {
            RLOGE(
                    "%s: roaming_indicators_size = %u > QMI_NAS_SERVING_SYSTEM_MAX_ROAMING_IND, increase the define",
                    __FUNCTION__, out->roaming_indicators_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x15->num_instances));
        skip += sizeof(tlv_0x15->num_instances);
        roaming_indicator_instance = (roaming_indicator_instance_t *)skip;

        if (out->roaming_indicators_size > 0) {
            for (i = 0; i < out->roaming_indicators_size; i++) {
                COPY_MTOH(out->roaming_indicators[i].radio_interface, roaming_indicator_instance->radio_if);
                COPY_MTOH(out->roaming_indicators[i].roam_indicator,
                        roaming_indicator_instance->roaming_indicator);
                roaming_indicator_instance++;
            }
        } else {
            RLOGW("%s: no instances available", __FUNCTION__);
        }
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    /* TLV 0x16 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x16_t *tlv_0x16;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x16_t);

        hdr++;
        tlv_0x16 = (QMI_NAS_GET_SS_INFO_response_0x16_t *)hdr;
        out->default_roam_ind_available = true;
        COPY_MTOH(out->default_roam_ind, tlv_0x16->roaming_indicator);
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    /* TLV 0x17 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x17_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x17_t);

        hdr++;
        tlv_0x17 = (QMI_NAS_GET_SS_INFO_response_0x17_t *)hdr;
        out->time_zone_3gpp2_available = true;
        COPY_MTOH(out->time_zone_3gpp2.leap_seconds, tlv_0x17->leapseconds);
        COPY_MTOH(out->time_zone_3gpp2.local_time_offset, tlv_0x17->local_time_offset);
        COPY_MTOH(out->time_zone_3gpp2.daylight_saving, tlv_0x17->daylight_savings_in_effect);
    }

    hdr = search_TLV(msg, 0x18, payload_len);
    /* TLV 0x18 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x18_t *tlv_0x18;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x18_t);

        hdr++;
        tlv_0x18 = (QMI_NAS_GET_SS_INFO_response_0x18_t *)hdr;
        out->CDMA_p_rev_available = true;
        COPY_MTOH(out->CDMA_p_rev, tlv_0x18->protocol_revision);
    }

    hdr = search_TLV(msg, 0x1A, payload_len);
    /* TLV 0x1A is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x1A_t *tlv_0x1A;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x1A_t);

        hdr++;
        tlv_0x1A = (QMI_NAS_GET_SS_INFO_response_0x1A_t *)hdr;
        out->time_zone_3gpp_available = true;
        COPY_MTOH(out->time_zone_3gpp, tlv_0x1A->_3gpp_time_zone);
    }

    hdr = search_TLV(msg, 0x1B, payload_len);
    /* TLV 0x1B is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x1B_t *tlv_0x1B;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x1B_t);

        hdr++;
        tlv_0x1B = (QMI_NAS_GET_SS_INFO_response_0x1B_t *)hdr;
        out->network_DST_adjustment_3gpp_available = true;
        COPY_MTOH(out->network_DST_adjustment_3gpp, tlv_0x1B->daylight_savings_adjustment);
    }

    TLV = (indication ? 0x1D : 0x1C);
    hdr = search_TLV(msg, TLV, payload_len);
    /* TLV 0x1C is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x1C_t *tlv_0x1C;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x1C_t);

        hdr++;
        tlv_0x1C = (QMI_NAS_GET_SS_INFO_response_0x1C_t *)hdr;
        out->location_area_code_available = true;
        COPY_MTOH(out->location_area_code, tlv_0x1C->location_area_code);
    }

    TLV = (indication ? 0x1E : 0x1D);
    hdr = search_TLV(msg, TLV, payload_len);
    /* TLV 0x1D is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x1D_t *tlv_0x1D;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x1D_t);

        hdr++;
        tlv_0x1D = (QMI_NAS_GET_SS_INFO_response_0x1D_t *)hdr;
        out->cell_id_available = true;
        COPY_MTOH(out->cell_id, tlv_0x1D->cell_ID);
    }

    TLV = (indication ? 0x1F : 0x1E);
    hdr = search_TLV(msg, TLV, payload_len);
    /* TLV 0x1E is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x1E_t *tlv_0x1E;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x1E_t);

        hdr++;
        tlv_0x1E = (QMI_NAS_GET_SS_INFO_response_0x1E_t *)hdr;
        out->conc_svc_info_available = true;
        COPY_MTOH(out->conc_svc_info, tlv_0x1E->concurrent_service);
    }

    TLV = (indication ? 0x20 : 0x1F);
    hdr = search_TLV(msg, TLV, payload_len);
    /* TLV 0x1F is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x1F_t *tlv_0x1F;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x1F_t);

        hdr++;
        tlv_0x1F = (QMI_NAS_GET_SS_INFO_response_0x1F_t *)hdr;
        out->PRL_ind_available = true;
        COPY_MTOH(out->PRL_ind, tlv_0x1F->PRL_indicator);
    }

    TLV = (indication ? 0x21 : 0x20);
    hdr = search_TLV(msg, TLV, payload_len);
    /* TLV 0x20 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x20_t *tlv_0x20;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x20_t);

        hdr++;
        tlv_0x20 = (QMI_NAS_GET_SS_INFO_response_0x20_t *)hdr;
        out->DTM_ind_available = true;
        COPY_MTOH(out->DTM_ind, tlv_0x20->DTM);
    }

    TLV = (indication ? 0x22 : 0x21);
    hdr = search_TLV(msg, TLV, payload_len);
    /* TLV 0x21 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x21_t *tlv_0x21;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x21_t);

        hdr++;
        tlv_0x21 = (QMI_NAS_GET_SS_INFO_response_0x21_t *)hdr;
        out->detailed_svc_info_available = true;
        COPY_MTOH(out->detailed_svc_info.service_status, tlv_0x21->service_status);
        COPY_MTOH(out->detailed_svc_info.service_caps, tlv_0x21->service_caps);
        COPY_MTOH(out->detailed_svc_info.hdr_srv_status, tlv_0x21->hdr_srv_status);
        COPY_MTOH(out->detailed_svc_info.hdr_hybrid, tlv_0x21->hdr_hybrid);
        COPY_MTOH(out->detailed_svc_info.is_sys_forbidden, tlv_0x21->is_sys_forbidden);
    }

    TLV = (indication ? 0x23 : 0x22);
    hdr = search_TLV(msg, TLV, payload_len);
    /* TLV 0x22 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x22_t *tlv_0x22;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x22_t);

        hdr++;
        tlv_0x22 = (QMI_NAS_GET_SS_INFO_response_0x22_t *)hdr;
        out->CDMA_system_info_ext_available = true;
        COPY_MTOH(out->CDMA_system_info_ext.MCC, tlv_0x22->MCC);
        COPY_MTOH(out->CDMA_system_info_ext.imsi_11_12, tlv_0x22->imsi_11_12);
    }

    TLV = (indication ? 0x24 : 0x23);
    hdr = search_TLV(msg, TLV, payload_len);
    /* TLV 0x23 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x23_t *tlv_0x23;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x23_t);

        hdr++;
        tlv_0x23 = (QMI_NAS_GET_SS_INFO_response_0x23_t *)hdr;
        out->hdr_personality_available = true;
        COPY_MTOH(out->hdr_personality, tlv_0x23->hdr_personality);
    }

    TLV = (indication ? 0x25 : 0x24);
    hdr = search_TLV(msg, TLV, payload_len);
    /* TLV 0x24 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x24_t *tlv_0x24;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x24_t);

        hdr++;
        tlv_0x24 = (QMI_NAS_GET_SS_INFO_response_0x24_t *)hdr;
        out->track_area_code_available = true;
        COPY_MTOH(out->track_area_code, tlv_0x24->track_area_code);
    }

    TLV = (indication ? 0x26 : 0x25);
    hdr = search_TLV(msg, TLV, payload_len);
    /* TLV 0x25 is optional */
    if (hdr) {
        QMI_NAS_GET_SS_INFO_response_0x25_t *tlv_0x25;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SS_INFO_response_0x25_t);

        hdr++;
        tlv_0x25 = (QMI_NAS_GET_SS_INFO_response_0x25_t *)hdr;
        out->call_bar_status_available = true;
        COPY_MTOH(out->call_bar_status.cs_bar_status, tlv_0x25->CS_call_bar_status);
        COPY_MTOH(out->call_bar_status.ps_bar_status, tlv_0x25->PS_call_bar_status);
    }

end:
    return ret;
}

QMI_SDK_error_t telit_nas_get_serving_system_unpack(uint8_t *rsp, uint16_t len,
        nas_serving_system_info_t *out)
{
    return telit_nas_generic_serving_system_unpack(rsp, len, out, false);
}

/*****************************************************************************
 * QMI_NAS_GET_HOME_NETWORK (37 - 0x0025)
 */

QMI_SDK_error_t telit_nas_get_home_network_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_NAS_GET_HOME_NETWORK,
            QMI_SVC_NAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_nas_get_home_network_unpack(uint8_t *rsp, uint16_t len, nas_home_info_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_NAS_GET_HOME_INFO_response_0x01_t *tlv_0x01;
    uint8_t *description;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    out->mcc = 0xffff;
    out->mnc = 0xffff;
    out->home_system_id.nid = 0xffff;
    out->home_system_id.sid = 0xffff;

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_HOME_INFO_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_NAS_GET_HOME_INFO_response_0x01_t *)hdr;
    COPY_MTOH(out->mcc, tlv_0x01->MCC);
    COPY_MTOH(out->mnc, tlv_0x01->MNC);
    COPY_MTOH(out->name_len, tlv_0x01->description_len);

    if (out->name_len > MAX_STRING_SIZE - 1) {
        RLOGE("%s: name_len = %u > MAX_STRING_SIZE, increase the define",
                __FUNCTION__, out->name_len);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    /* Set pointer to the description string */
    description = (uint8_t *)(&(tlv_0x01->description_len));
    /* move pointer to actual string */
    description += sizeof(tlv_0x01->description_len);
    memcpy(out->name, description, out->name_len);

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_NAS_GET_HOME_INFO_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_HOME_INFO_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_NAS_GET_HOME_INFO_response_0x10_t *)hdr;
        COPY_MTOH(out->home_system_id.nid, tlv_0x10->nid);
        COPY_MTOH(out->home_system_id.sid, tlv_0x10->sid);
        out->sid_nid_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_NAS_GET_TECHNOLOGY_PREFERENCE (43 - 0x002B)
 */

QMI_SDK_error_t telit_nas_get_technology_preference_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_NAS_GET_TECHNOLOGY_PREFERENCE,
            QMI_SVC_NAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_nas_get_technology_preference_unpack(uint8_t *rsp, uint16_t len,
        nas_get_network_preference_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr;
    QMI_NAS_GET_NETWORK_PREFERENCE_response_0x01_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        goto end;
    }

    out->technology_preference = 0xffff;
    out->technology_preference_duration = 0xff;
    out->persistent = 0xffff;

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_NETWORK_PREFERENCE_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_NAS_GET_NETWORK_PREFERENCE_response_0x01_t *)hdr;
    COPY_MTOH(out->technology_preference, tlv_0x01->technology_preference);
    COPY_MTOH(out->technology_preference_duration, tlv_0x01->technology_preference_duration);

    hdr = search_TLV(msg, 0x10, payload_len);
    if (hdr) {
        QMI_NAS_GET_NETWORK_PREFERENCE_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_NETWORK_PREFERENCE_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_NAS_GET_NETWORK_PREFERENCE_response_0x10_t *)hdr;
        out->persistent_available = true;
        COPY_MTOH(out->persistent, tlv_0x10->persistent);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_NAS_GET_RF_BAND_INFO (49 - 0x0031)
 */

QMI_SDK_error_t telit_nas_get_rf_band_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_NAS_GET_RF_BAND_INFO,
            QMI_SVC_NAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_nas_get_rf_band_info_unpack(uint8_t *rsp, uint16_t len, nas_rf_info_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    uint8_t i = 0;
    QMI_NAS_GET_RF_INFO_response_t *tlv;
    QMI_NAS_GET_RF_INFO_instance_t *instance;
    uint8_t *skip = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_RF_INFO_response_t);

    hdr++;
    tlv = (QMI_NAS_GET_RF_INFO_response_t *)hdr;
    COPY_MTOH(out->rf_info_instances_size, tlv->num_instances);

    if (out->rf_info_instances_size > QMI_NAS_MAX_RF_INFO_INSTANCES) {
        RLOGE("%s: rf_info_instances_size = %u > QMI_NAS_MAX_RF_INFO_INSTANCES, increase the define",
                __FUNCTION__, out->rf_info_instances_size);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    skip = (uint8_t *)(&(tlv->num_instances));
    skip += sizeof(tlv->num_instances);

    instance = (QMI_NAS_GET_RF_INFO_instance_t *)skip;

    for (i = 0; i < out->rf_info_instances_size; i++) {
        COPY_MTOH(out->rf_info_instance[i].radio_if, instance->radio_interface);
        COPY_MTOH(out->rf_info_instance[i].active_band, instance->active_band_class);
        COPY_MTOH(out->rf_info_instance[i].active_channel, instance->active_channel);
        instance++;
    }

    /* Unpack Persistent 0x11 TLV (optional) */
    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        QMI_NAS_GET_RF_INFO_extended_instance_t *extended_instance;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_RF_INFO_response_t);

        hdr++;
        tlv = (QMI_NAS_GET_RF_INFO_response_t *)hdr;
        COPY_MTOH(out->rf_info_extended_size, tlv->num_instances);

        if (out->rf_info_extended_size > QMI_NAS_MAX_RF_INFO_INSTANCES) {
            RLOGE("%s: rf_info_extended_size = %u > QMI_NAS_MAX_RF_INFO_INSTANCES, increase the define",
                    __FUNCTION__, out->rf_info_extended_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv->num_instances));
        skip += sizeof(tlv->num_instances);

        extended_instance = (QMI_NAS_GET_RF_INFO_extended_instance_t *)skip;

        for (i = 0; i < out->rf_info_extended_size; i++) {
            COPY_MTOH(out->rf_info_extended[i].radio_if, extended_instance->radio_interface);
            COPY_MTOH(out->rf_info_extended[i].active_band, extended_instance->active_band_class);
            COPY_MTOH(out->rf_info_extended[i].active_channel, extended_instance->active_channel);
            extended_instance++;
        }
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE (51 - 0x0033)
 */

QMI_SDK_error_t telit_nas_set_system_selection_preference_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_set_system_selection_preference_t *input)
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

    req_ctx->msg_id = QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE;
    req_ctx->svc_id = QMI_SVC_NAS;
    /* Custom timeout */
    req_ctx->timeout = 60;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    /* Add TLV 0x10 */
    if (input->emergency_mode_available) {
        QMI_NAS_generic_emergency_mode_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_NAS_generic_emergency_mode_t *)buf_position;
        COPY_HTOM(tlv_0x10->emergency_mode_on, input->emergency_mode_on);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x11 */
    if (input->mode_pref_available) {
        QMI_NAS_generic_mode_preference_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_NAS_generic_mode_preference_t *)buf_position;
        COPY_HTOM(tlv_0x11->mode_pref, input->mode_pref);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x12 */
    if (input->band_pref_available) {
        QMI_NAS_generic_band_preference_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_NAS_generic_band_preference_t *)buf_position;
        COPY_HTOM(tlv_0x12->band_pref, input->band_pref);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x14 */
    if (input->roam_pref_available) {
        QMI_NAS_generic_roaming_preference_t *tlv_0x14;

        tlv_sz = sizeof(*tlv_0x14);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x14;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x14 = (QMI_NAS_generic_roaming_preference_t *)buf_position;
        COPY_HTOM(tlv_0x14->roam_pref, input->roam_pref);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x15 */
    if (input->lte_band_pref_available) {
        QMI_NAS_generic_LTE_band_preference_t *tlv_0x15;

        tlv_sz = sizeof(*tlv_0x15);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x15;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x15 = (QMI_NAS_generic_LTE_band_preference_t *)buf_position;
        COPY_HTOM(tlv_0x15->lte_band_pref, input->lte_band_pref);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x16 */
    if (input->net_sel_pref_available) {
        QMI_NAS_SET_SYS_SELECT_PREF_request_0x16_t *tlv_0x16;
        /* TLV 0x1A is related to 0x16*/
        QMI_NAS_SET_SYS_SELECT_PREF_request_0x1A_t *tlv_0x1A;

        tlv_sz = sizeof(*tlv_0x16);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x16;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x16 = (QMI_NAS_SET_SYS_SELECT_PREF_request_0x16_t *)buf_position;
        COPY_HTOM(tlv_0x16->net_sel_pref, input->network_selection_preference.net_sel_pref);
        COPY_HTOM(tlv_0x16->mcc, input->network_selection_preference.mobile_country_code);
        COPY_HTOM(tlv_0x16->mnc, input->network_selection_preference.mobile_network_code);

        buf_position += tlv_sz;

        tlv_sz = sizeof(*tlv_0x1A);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1A;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1A = (QMI_NAS_SET_SYS_SELECT_PREF_request_0x1A_t *)buf_position;
        COPY_HTOM(tlv_0x1A->mnc_includes_pcs_digit, input->mnc_inc_pcs_dig_stat);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x17 */
    if (input->chg_duration_available) {
        QMI_NAS_SET_SYS_SELECT_PREF_request_0x17_t *tlv_0x17;

        tlv_sz = sizeof(*tlv_0x17);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x17;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x17 = (QMI_NAS_SET_SYS_SELECT_PREF_request_0x17_t *)buf_position;
        COPY_HTOM(tlv_0x17->change_duration, input->chg_duration);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x18 */
    if (input->srv_domain_pref_available) {
        QMI_NAS_generic_service_domain_preference_t *tlv_0x18;

        tlv_sz = sizeof(*tlv_0x18);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x18;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x18 = (QMI_NAS_generic_service_domain_preference_t *)buf_position;
        COPY_HTOM(tlv_0x18->srv_domain_pref, input->srv_domain_pref);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x19 */
    if (input->gw_acq_order_pref_available) {
        QMI_NAS_generic_gw_acq_order_pref_t *tlv_0x19;

        tlv_sz = sizeof(*tlv_0x19);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x19;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x19 = (QMI_NAS_generic_gw_acq_order_pref_t *)buf_position;
        COPY_HTOM(tlv_0x19->gw_acq_order_pref, input->gw_acq_order_pref);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x1E */
    if (input->acq_order_pref_available) {
        QMI_NAS_SET_SYS_SELECT_PREF_request_0x1E_t *tlv_0x1E;
        uint8_t *acq_order = NULL;

        tlv_sz = sizeof(*tlv_0x1E) +
                input->acq_order_pref.acq_order_size;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1E;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1E = (QMI_NAS_SET_SYS_SELECT_PREF_request_0x1E_t *)buf_position;
        COPY_HTOM(tlv_0x1E->acq_order_size, input->acq_order_pref.acq_order_size);

        /* No need to check acq_order_size, since it's uint8_t always <= QMI_NAS_ACQ_ORDER_SIZE */
        acq_order = (uint8_t *)(&(tlv_0x1E->acq_order_size));
        acq_order += sizeof(tlv_0x1E->acq_order_size);
        memcpy(acq_order, input->acq_order_pref.acq_order, input->acq_order_pref.acq_order_size);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x1F */
    if (input->srv_reg_restriction_available) {
        QMI_NAS_SET_SYS_SELECT_PREF_request_0x1F_t *tlv_0x1F;

        tlv_sz = sizeof(*tlv_0x1F);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1F;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1F = (QMI_NAS_SET_SYS_SELECT_PREF_request_0x1F_t *)buf_position;
        COPY_HTOM(tlv_0x1F->srv_reg_restriction, input->srv_reg_restriction);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x20 */
    if (input->csg_id_available) {
        QMI_NAS_SET_SYS_SELECT_PREF_request_0x20_t *tlv_0x20;

        tlv_sz = sizeof(*tlv_0x20);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x20;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x20 = (QMI_NAS_SET_SYS_SELECT_PREF_request_0x20_t *)buf_position;
        COPY_HTOM(tlv_0x20->mcc, input->csg_id.mcc);
        COPY_HTOM(tlv_0x20->mnc, input->csg_id.mnc);
        COPY_HTOM(tlv_0x20->mnc_includes_pcs_digit, input->csg_id.mnc_includes_pcs_digit);
        COPY_HTOM(tlv_0x20->id, input->csg_id.id);
        COPY_HTOM(tlv_0x20->rat, input->csg_id.rat);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x22 */
    if (input->rat_available) {
        QMI_NAS_SET_SYS_SELECT_PREF_request_0x22_t *tlv_0x22;

        tlv_sz = sizeof(*tlv_0x22);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x22;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x22 = (QMI_NAS_SET_SYS_SELECT_PREF_request_0x22_t *)buf_position;
        COPY_HTOM(tlv_0x22->rat, input->rat);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x24 */
    if (input->lte_band_preference_extended_available) {
        QMI_NAS_generic_lte_bands_extended_t *tlv_0x24;

        tlv_sz = sizeof(*tlv_0x24);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x24;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x24 = (QMI_NAS_generic_lte_bands_extended_t *)buf_position;
        COPY_HTOM(tlv_0x24->bits_1_64, input->lte_band_preference_extended.bits_1_64);
        COPY_HTOM(tlv_0x24->bits_65_128, input->lte_band_preference_extended.bits_65_128);
        COPY_HTOM(tlv_0x24->bits_129_192, input->lte_band_preference_extended.bits_129_192);
        COPY_HTOM(tlv_0x24->bits_193_256, input->lte_band_preference_extended.bits_193_256);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x2B */
    if (input->nr5g_band_preference_available) {
        QMI_NAS_generic_nr5g_bands_t *tlv_0x2B;

        tlv_sz = sizeof(*tlv_0x2B);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x2B;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x2B = (QMI_NAS_generic_nr5g_bands_t *)buf_position;
        COPY_HTOM(tlv_0x2B->bits_1_64, input->nr5g_band_preference.bits_1_64);
        COPY_HTOM(tlv_0x2B->bits_65_128, input->nr5g_band_preference.bits_65_128);
        COPY_HTOM(tlv_0x2B->bits_129_192, input->nr5g_band_preference.bits_129_192);
        COPY_HTOM(tlv_0x2B->bits_193_256, input->nr5g_band_preference.bits_193_256);
        COPY_HTOM(tlv_0x2B->bits_257_320, input->nr5g_band_preference.bits_257_320);
        COPY_HTOM(tlv_0x2B->bits_321_384, input->nr5g_band_preference.bits_321_384);
        COPY_HTOM(tlv_0x2B->bits_385_448, input->nr5g_band_preference.bits_385_448);
        COPY_HTOM(tlv_0x2B->bits_449_512, input->nr5g_band_preference.bits_449_512);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x2E */
    if (input->nr5g_sa_nsa_mode_disable_available) {
        QMI_NAS_SET_SYS_SELECT_PREF_generic_sa_nsa_mode_disable_t *tlv_0x2E;

        tlv_sz = sizeof(*tlv_0x2E);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x2E;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x2E = (QMI_NAS_SET_SYS_SELECT_PREF_generic_sa_nsa_mode_disable_t *)buf_position;
        COPY_HTOM(tlv_0x2E->nr5g_sa_nsa_mode_disable, input->nr5g_sa_nsa_mode_disable);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x2F */
    if (input->nr5g_sa_band_preference_available) {
        QMI_NAS_generic_nr5g_bands_t *tlv_0x2F;

        tlv_sz = sizeof(*tlv_0x2F);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x2F;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x2F = (QMI_NAS_generic_nr5g_bands_t *)buf_position;
        COPY_HTOM(tlv_0x2F->bits_1_64, input->nr5g_sa_band_preference.bits_1_64);
        COPY_HTOM(tlv_0x2F->bits_65_128, input->nr5g_sa_band_preference.bits_65_128);
        COPY_HTOM(tlv_0x2F->bits_129_192, input->nr5g_sa_band_preference.bits_129_192);
        COPY_HTOM(tlv_0x2F->bits_193_256, input->nr5g_sa_band_preference.bits_193_256);
        COPY_HTOM(tlv_0x2F->bits_257_320, input->nr5g_sa_band_preference.bits_257_320);
        COPY_HTOM(tlv_0x2F->bits_321_384, input->nr5g_sa_band_preference.bits_321_384);
        COPY_HTOM(tlv_0x2F->bits_385_448, input->nr5g_sa_band_preference.bits_385_448);
        COPY_HTOM(tlv_0x2F->bits_449_512, input->nr5g_sa_band_preference.bits_449_512);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x30 */
    if (input->nr5g_nsa_band_preference_available) {
        QMI_NAS_generic_nr5g_bands_t *tlv_0x30;

        tlv_sz = sizeof(*tlv_0x30);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x30;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x30 = (QMI_NAS_generic_nr5g_bands_t *)buf_position;
        COPY_HTOM(tlv_0x30->bits_1_64, input->nr5g_nsa_band_preference.bits_1_64);
        COPY_HTOM(tlv_0x30->bits_65_128, input->nr5g_nsa_band_preference.bits_65_128);
        COPY_HTOM(tlv_0x30->bits_129_192, input->nr5g_nsa_band_preference.bits_129_192);
        COPY_HTOM(tlv_0x30->bits_193_256, input->nr5g_nsa_band_preference.bits_193_256);
        COPY_HTOM(tlv_0x30->bits_257_320, input->nr5g_nsa_band_preference.bits_257_320);
        COPY_HTOM(tlv_0x30->bits_321_384, input->nr5g_nsa_band_preference.bits_321_384);
        COPY_HTOM(tlv_0x30->bits_385_448, input->nr5g_nsa_band_preference.bits_385_448);
        COPY_HTOM(tlv_0x30->bits_449_512, input->nr5g_nsa_band_preference.bits_449_512);
    }

    ret = QMI_SDK_ERR_NONE;

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_nas_set_system_selection_preference_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE (52 - 0x0034)
 */

QMI_SDK_error_t telit_nas_get_system_selection_preference_pack(pack_qmi_t *req_ctx, uint8_t *req,
        uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE,
            QMI_SVC_NAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_nas_get_system_selection_preference_unpack(uint8_t *rsp, uint16_t len,
        nas_system_selection_pref_t *out)
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
        QMI_NAS_generic_emergency_mode_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_emergency_mode_t);

        hdr++;
        tlv_0x10 = (QMI_NAS_generic_emergency_mode_t *)hdr;

        if (tlv_0x10->emergency_mode_on) {
            out->emergency_mode_on = true;
        }
        out->emergency_mode_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_NAS_generic_mode_preference_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_mode_preference_t);

        hdr++;
        tlv_0x11 = (QMI_NAS_generic_mode_preference_t *)hdr;
        COPY_MTOH(out->mode_pref, tlv_0x11->mode_pref);
        out->mode_pref_available = true;
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_NAS_generic_band_preference_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_band_preference_t);

        hdr++;
        tlv_0x12 = (QMI_NAS_generic_band_preference_t *)hdr;
        COPY_MTOH(out->band_pref, tlv_0x12->band_pref);
        out->band_pref_available = true;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_NAS_generic_roaming_preference_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_roaming_preference_t);

        hdr++;
        tlv_0x14 = (QMI_NAS_generic_roaming_preference_t *)hdr;
        COPY_MTOH(out->roam_pref, tlv_0x14->roam_pref);
        out->roam_pref_available = true;
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    /* TLV 0x15 is optional */
    if (hdr) {
        QMI_NAS_generic_LTE_band_preference_t *tlv_0x15;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_LTE_band_preference_t);

        hdr++;
        tlv_0x15 = (QMI_NAS_generic_LTE_band_preference_t *)hdr;
        COPY_MTOH(out->lte_band_pref, tlv_0x15->lte_band_pref);
        out->lte_band_pref_available = true;
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    /* TLV 0x16 is optional */
    if (hdr) {
        QMI_NAS_GET_SYS_SELECT_PREF_response_0x16_t *tlv_0x16;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYS_SELECT_PREF_response_0x16_t);

        hdr++;
        tlv_0x16 = (QMI_NAS_GET_SYS_SELECT_PREF_response_0x16_t *)hdr;
        COPY_MTOH(out->net_sel_pref, tlv_0x16->net_sel_pref);
        out->net_sel_pref_available = true;
    }

    hdr = search_TLV(msg, 0x18, payload_len);
    /* TLV 0x18 is optional */
    if (hdr) {
        QMI_NAS_generic_service_domain_preference_t *tlv_0x18;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_service_domain_preference_t);

        hdr++;
        tlv_0x18 = (QMI_NAS_generic_service_domain_preference_t *)hdr;
        COPY_MTOH(out->srv_domain_pref, tlv_0x18->srv_domain_pref);
        out->srv_domain_pref_available = true;
    }

    hdr = search_TLV(msg, 0x19, payload_len);
    /* TLV 0x19 is optional */
    if (hdr) {
        QMI_NAS_generic_gw_acq_order_pref_t *tlv_0x19;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_gw_acq_order_pref_t);

        hdr++;
        tlv_0x19 = (QMI_NAS_generic_gw_acq_order_pref_t *)hdr;
        COPY_MTOH(out->gw_acq_order_pref, tlv_0x19->gw_acq_order_pref);
        out->gw_acq_order_pref_available = true;
    }

    hdr = search_TLV(msg, 0x23, payload_len);
    /* TLV 0x23 is optional */
    if (hdr) {
        QMI_NAS_generic_lte_bands_extended_t *tlv_0x23;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_lte_bands_extended_t);

        hdr++;
        tlv_0x23 = (QMI_NAS_generic_lte_bands_extended_t *)hdr;
        COPY_MTOH(out->lte_band_preference_extended.bits_1_64, tlv_0x23->bits_1_64);
        COPY_MTOH(out->lte_band_preference_extended.bits_65_128, tlv_0x23->bits_65_128);
        COPY_MTOH(out->lte_band_preference_extended.bits_129_192, tlv_0x23->bits_129_192);
        COPY_MTOH(out->lte_band_preference_extended.bits_193_256, tlv_0x23->bits_193_256);
        out->lte_band_preference_extended_available = true;
    }

    hdr = search_TLV(msg, 0x28, payload_len);
    /* TLV 0x28 is optional */
    if (hdr) {
        QMI_NAS_generic_nr5g_bands_t *tlv_0x28;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_nr5g_bands_t);

        hdr++;
        tlv_0x28 = (QMI_NAS_generic_nr5g_bands_t *)hdr;
        COPY_MTOH(out->nr5g_band_preference.bits_1_64, tlv_0x28->bits_1_64);
        COPY_MTOH(out->nr5g_band_preference.bits_65_128, tlv_0x28->bits_65_128);
        COPY_MTOH(out->nr5g_band_preference.bits_129_192, tlv_0x28->bits_129_192);
        COPY_MTOH(out->nr5g_band_preference.bits_193_256, tlv_0x28->bits_193_256);
        COPY_MTOH(out->nr5g_band_preference.bits_257_320, tlv_0x28->bits_257_320);
        COPY_MTOH(out->nr5g_band_preference.bits_321_384, tlv_0x28->bits_321_384);
        COPY_MTOH(out->nr5g_band_preference.bits_385_448, tlv_0x28->bits_385_448);
        COPY_MTOH(out->nr5g_band_preference.bits_449_512, tlv_0x28->bits_449_512);
        out->nr5g_band_preference_available = true;
    }

    hdr = search_TLV(msg, 0x2B, payload_len);
    /* TLV 0x2B is optional */
    if (hdr) {
        QMI_NAS_SET_SYS_SELECT_PREF_generic_sa_nsa_mode_disable_t *tlv_0x2B;
        CHECK_UNPACK_TLV_SIZE(QMI_NAS_SET_SYS_SELECT_PREF_generic_sa_nsa_mode_disable_t);

        hdr++;
        tlv_0x2B = (QMI_NAS_SET_SYS_SELECT_PREF_generic_sa_nsa_mode_disable_t *)hdr;
        COPY_MTOH(out->nr5g_sa_nsa_mode_disable, tlv_0x2B->nr5g_sa_nsa_mode_disable);
        out->nr5g_sa_nsa_mode_disable_available = true;
    }

    hdr = search_TLV(msg, 0x2C, payload_len);
    /* TLV 0x2C is optional */
    if (hdr) {
        QMI_NAS_generic_nr5g_bands_t *tlv_0x2C;
        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_nr5g_bands_t);

        hdr++;
        tlv_0x2C = (QMI_NAS_generic_nr5g_bands_t *)hdr;
        COPY_MTOH(out->nr5g_sa_band_preference.bits_1_64, tlv_0x2C->bits_1_64);
        COPY_MTOH(out->nr5g_sa_band_preference.bits_65_128, tlv_0x2C->bits_65_128);
        COPY_MTOH(out->nr5g_sa_band_preference.bits_129_192, tlv_0x2C->bits_129_192);
        COPY_MTOH(out->nr5g_sa_band_preference.bits_193_256, tlv_0x2C->bits_193_256);
        COPY_MTOH(out->nr5g_sa_band_preference.bits_257_320, tlv_0x2C->bits_257_320);
        COPY_MTOH(out->nr5g_sa_band_preference.bits_321_384, tlv_0x2C->bits_321_384);
        COPY_MTOH(out->nr5g_sa_band_preference.bits_385_448, tlv_0x2C->bits_385_448);
        COPY_MTOH(out->nr5g_sa_band_preference.bits_449_512, tlv_0x2C->bits_449_512);
        out->nr5g_sa_band_preference_available = true;
    }

    hdr = search_TLV(msg, 0x2D, payload_len);
    /* TLV 0x2D is optional */
    if (hdr) {
        QMI_NAS_generic_nr5g_bands_t *tlv_0x2D;
        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_nr5g_bands_t);

        hdr++;
        tlv_0x2D = (QMI_NAS_generic_nr5g_bands_t *)hdr;
        COPY_MTOH(out->nr5g_nsa_band_preference.bits_1_64, tlv_0x2D->bits_1_64);
        COPY_MTOH(out->nr5g_nsa_band_preference.bits_65_128, tlv_0x2D->bits_65_128);
        COPY_MTOH(out->nr5g_nsa_band_preference.bits_129_192, tlv_0x2D->bits_129_192);
        COPY_MTOH(out->nr5g_nsa_band_preference.bits_193_256, tlv_0x2D->bits_193_256);
        COPY_MTOH(out->nr5g_nsa_band_preference.bits_257_320, tlv_0x2D->bits_257_320);
        COPY_MTOH(out->nr5g_nsa_band_preference.bits_321_384, tlv_0x2D->bits_321_384);
        COPY_MTOH(out->nr5g_nsa_band_preference.bits_385_448, tlv_0x2D->bits_385_448);
        COPY_MTOH(out->nr5g_nsa_band_preference.bits_449_512, tlv_0x2D->bits_449_512);
        out->nr5g_nsa_band_preference_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_NAS_GET_CELL_LOCATION_INFO (67 - 0x0043)
 */

QMI_SDK_error_t telit_nas_get_cell_location_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_NAS_GET_CELL_LOCATION_INFO,
            QMI_SVC_NAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_nas_get_cell_location_info_unpack(uint8_t *rsp, uint16_t len,
        nas_cell_location_info_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    uint8_t *skip = NULL;
    uint8_t i = 0;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_NAS_GET_CELL_LOC_INFO_response_0x10_t *tlv_0x10;
        nmr_cell_info_instance_t *nmr_cell_info;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_CELL_LOC_INFO_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_NAS_GET_CELL_LOC_INFO_response_0x10_t *)hdr;
        out->geran_info_available = true;
        COPY_MTOH(out->geran_info.cell_id, tlv_0x10->cell_id);
        memcpy(out->geran_info.plmn, tlv_0x10->plmn, 3);
        COPY_MTOH(out->geran_info.lac, tlv_0x10->lac);
        COPY_MTOH(out->geran_info.arfcn, tlv_0x10->arfcn);
        COPY_MTOH(out->geran_info.bsic, tlv_0x10->bsic);
        COPY_MTOH(out->geran_info.timing_advance, tlv_0x10->timing_advance);
        COPY_MTOH(out->geran_info.rx_lev, tlv_0x10->rx_lev);
        COPY_MTOH(out->geran_info.nmr_cell_info_size, tlv_0x10->nmr_cell_info_size);

        if (out->geran_info.nmr_cell_info_size > QMI_NAS_MAX_CELLS) {
            RLOGE("%s: geran_info.nmr_cell_info_size = %u > QMI_NAS_MAX_CELLS, increase the define",
                    __FUNCTION__, out->geran_info.nmr_cell_info_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x10->nmr_cell_info_size));
        skip += sizeof(tlv_0x10->nmr_cell_info_size);
        nmr_cell_info = (nmr_cell_info_instance_t *)skip;

        for (i = 0; i < out->geran_info.nmr_cell_info_size; i++) {
            COPY_MTOH(out->geran_info.nmr_cell_info[i].nmr_cell_id, nmr_cell_info->nmr_cell_id);
            memcpy(out->geran_info.nmr_cell_info[i].nmr_plmn, nmr_cell_info->nmr_plmn, 3);
            COPY_MTOH(out->geran_info.nmr_cell_info[i].nmr_lac, nmr_cell_info->nmr_lac);
            COPY_MTOH(out->geran_info.nmr_cell_info[i].nmr_arfcn, nmr_cell_info->nmr_arfcn);
            COPY_MTOH(out->geran_info.nmr_cell_info[i].nmr_bsic, nmr_cell_info->nmr_bsic);
            COPY_MTOH(out->geran_info.nmr_cell_info[i].nmr_rx_lev, nmr_cell_info->nmr_rx_lev);

            nmr_cell_info++;
        }
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_NAS_GET_CELL_LOC_INFO_response_0x11_t *tlv_0x11;
        geran_instance_t *geran_instance;
        umts_instance_t *umts_instance;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_CELL_LOC_INFO_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_NAS_GET_CELL_LOC_INFO_response_0x11_t *)hdr;
        out->umts_info_available = true;
        COPY_MTOH(out->umts_info.cell_id, tlv_0x11->cell_id);
        memcpy(out->umts_info.plmn, tlv_0x11->plmn, 3);
        COPY_MTOH(out->umts_info.lac, tlv_0x11->lac);
        COPY_MTOH(out->umts_info.uarfcn, tlv_0x11->uarfcn);
        COPY_MTOH(out->umts_info.psc, tlv_0x11->psc);
        COPY_MTOH(out->umts_info.rscp, tlv_0x11->rscp);
        COPY_MTOH(out->umts_info.ecio, tlv_0x11->ecio);
        COPY_MTOH(out->umts_info.umts_insts_size, tlv_0x11->umts_insts_size);

        if (out->umts_info.umts_insts_size > QMI_NAS_MAX_CELLS) {
            RLOGE("%s: umts_info.umts_insts_size = %u > QMI_NAS_MAX_CELLS, increase the define",
                    __FUNCTION__, out->umts_info.umts_insts_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x11->umts_insts_size));
        skip += sizeof(tlv_0x11->umts_insts_size);
        umts_instance = (umts_instance_t *)skip;

        for (i = 0; i < out->umts_info.umts_insts_size; i++) {
            COPY_MTOH(out->umts_info.umts_insts[i].umts_uarfcn, umts_instance->umts_uarfcn);
            COPY_MTOH(out->umts_info.umts_insts[i].umts_psc, umts_instance->umts_psc);
            COPY_MTOH(out->umts_info.umts_insts[i].umts_rscp, umts_instance->umts_rscp);
            COPY_MTOH(out->umts_info.umts_insts[i].umts_ecio, umts_instance->umts_ecio);

            umts_instance++;
        }

        /* set a pointer to the end of the umts instances */
        skip = (uint8_t *)(umts_instance);
        /* skip now points to the number of geran instances */
        /* NOTE: do not use COPY_MTOH, since it expects both DST and SRC not to be pointers */
        ec_mtoh(&out->umts_info.geran_insts_size, sizeof(out->umts_info.geran_insts_size), skip,
                sizeof(*skip));

        if (out->umts_info.geran_insts_size > QMI_NAS_MAX_CELLS) {
            RLOGE("%s: umts_info.geran_insts_size = %u > QMI_NAS_MAX_CELLS, increase the define",
                    __FUNCTION__, out->umts_info.geran_insts_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        /* Skipping 1-byte length */
        skip++;
        geran_instance = (geran_instance_t *)skip;

        for (i = 0; i < out->umts_info.geran_insts_size; i++) {
            COPY_MTOH(out->umts_info.geran_insts[i].geran_arfcn, geran_instance->geran_arfcn);
            COPY_MTOH(out->umts_info.geran_insts[i].geran_bsic_ncc, geran_instance->geran_bsic_ncc);
            COPY_MTOH(out->umts_info.geran_insts[i].geran_bsic_bcc, geran_instance->geran_bsic_bcc);
            COPY_MTOH(out->umts_info.geran_insts[i].geran_rssi, geran_instance->geran_rssi);

            geran_instance++;
        }
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_NAS_GET_CELL_LOC_INFO_response_0x13_t *tlv_0x13;
        cell_params_instance_t *cell_params_instance;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_CELL_LOC_INFO_response_0x13_t);

        hdr++;
        tlv_0x13 = (QMI_NAS_GET_CELL_LOC_INFO_response_0x13_t *)hdr;
        out->lte_info_intrafreq_available = true;
        COPY_MTOH(out->lte_info_intrafreq.ue_in_idle, tlv_0x13->ue_in_idle);
        memcpy(out->lte_info_intrafreq.plmn, tlv_0x13->plmn, 3);
        COPY_MTOH(out->lte_info_intrafreq.tac, tlv_0x13->tac);
        COPY_MTOH(out->lte_info_intrafreq.global_cell_id, tlv_0x13->global_cell_id);
        COPY_MTOH(out->lte_info_intrafreq.earfcn, tlv_0x13->earfcn);
        COPY_MTOH(out->lte_info_intrafreq.serving_cell_id, tlv_0x13->serving_cell_id);
        COPY_MTOH(out->lte_info_intrafreq.cell_resel_priority, tlv_0x13->cell_resel_priority);
        COPY_MTOH(out->lte_info_intrafreq.s_non_intra_search, tlv_0x13->s_non_intra_search);
        COPY_MTOH(out->lte_info_intrafreq.thresh_serving_low, tlv_0x13->thresh_serving_low);
        COPY_MTOH(out->lte_info_intrafreq.s_intra_search, tlv_0x13->s_intra_search);
        COPY_MTOH(out->lte_info_intrafreq.cell_intra_freq_params_size, tlv_0x13->cell_intra_freq_params_size);

        if (out->lte_info_intrafreq.cell_intra_freq_params_size > QMI_NAS_MAX_CELLS) {
            RLOGE(
                    "%s: lte_info_intrafreq.cell_intra_freq_params_size = %u > QMI_NAS_MAX_CELLS, increase the define",
                    __FUNCTION__, out->lte_info_intrafreq.cell_intra_freq_params_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x13->cell_intra_freq_params_size));
        skip += sizeof(tlv_0x13->cell_intra_freq_params_size);
        cell_params_instance = (cell_params_instance_t *)skip;

        for (i = 0; i < out->lte_info_intrafreq.cell_intra_freq_params_size; i++) {
            COPY_MTOH(out->lte_info_intrafreq.cell_intra_freq_params[i].pci, cell_params_instance->pci);
            COPY_MTOH(out->lte_info_intrafreq.cell_intra_freq_params[i].rsrq, cell_params_instance->rsrq);
            COPY_MTOH(out->lte_info_intrafreq.cell_intra_freq_params[i].rsrp, cell_params_instance->rsrp);
            COPY_MTOH(out->lte_info_intrafreq.cell_intra_freq_params[i].rssi, cell_params_instance->rssi);
            COPY_MTOH(out->lte_info_intrafreq.cell_intra_freq_params[i].srxlev, cell_params_instance->srxlev);

            cell_params_instance++;
        }
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_NAS_GET_CELL_LOC_INFO_response_0x14_t *tlv_0x14;
        inter_freqs_instance_t *inter_freqs;
        cell_params_instance_t *cell_params;
        uint8_t j = 0;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_CELL_LOC_INFO_response_0x14_t);

        hdr++;
        tlv_0x14 = (QMI_NAS_GET_CELL_LOC_INFO_response_0x14_t *)hdr;
        out->lte_info_interfreq_available = true;

        if (tlv_0x14->ue_in_idle) {
            out->lte_info_interfreq.ue_in_idle = true;
        }

        COPY_MTOH(out->lte_info_interfreq.inter_freqs_size, tlv_0x14->inter_freqs_size);

        if (out->lte_info_interfreq.inter_freqs_size > QMI_NAS_MAX_CELLS) {
            RLOGE("%s: lte_info_interfreq.inter_freqs_size = %u > QMI_NAS_MAX_CELLS, increase the define",
                    __FUNCTION__, out->lte_info_interfreq.inter_freqs_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x14->inter_freqs_size));
        skip += sizeof(tlv_0x14->inter_freqs_size);
        inter_freqs = (inter_freqs_instance_t *)skip;

        for (i = 0; i < out->lte_info_interfreq.inter_freqs_size; i++) {
            COPY_MTOH(out->lte_info_interfreq.inter_freqs[i].earfcn, inter_freqs->earfcn);
            COPY_MTOH(out->lte_info_interfreq.inter_freqs[i].threshX_low, inter_freqs->threshX_low);
            COPY_MTOH(out->lte_info_interfreq.inter_freqs[i].threshX_high, inter_freqs->threshX_high);
            COPY_MTOH(out->lte_info_interfreq.inter_freqs[i].cell_resel_priority,
                    inter_freqs->cell_resel_priority);

            COPY_MTOH(out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params_size,
                    inter_freqs->cell_inter_freq_params_size);

            if (out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params_size > QMI_NAS_MAX_CELLS) {
                RLOGE(
                        "%s: lte_info_interfreq.inter_freqs[i].cell_inter_freq_params_size = %u > QMI_NAS_MAX_CELLS, increase the define",
                        __FUNCTION__, out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params_size);
                ret = QMI_SDK_ERR_INTERNAL;
                goto end;
            }

            skip = (uint8_t *)(&(inter_freqs->cell_inter_freq_params_size));
            skip += sizeof(inter_freqs->cell_inter_freq_params_size);
            cell_params = (cell_params_instance_t *)skip;

            for (j = 0; j < out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params_size; j++) {
                COPY_MTOH(out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j].pci,
                        cell_params->pci);
                COPY_MTOH(out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j].rsrq,
                        cell_params->rsrq);
                COPY_MTOH(out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j].rsrp,
                        cell_params->rsrp);
                COPY_MTOH(out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j].rssi,
                        cell_params->rssi);
                COPY_MTOH(out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j].srxlev,
                        cell_params->srxlev);

                cell_params++;
            }

            inter_freqs = (inter_freqs_instance_t *)(cell_params);
        }
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    /* TLV 0x15 is optional */
    if (hdr) {
        QMI_NAS_GET_CELL_LOC_INFO_response_0x15_t *tlv_0x15;
        lte_gsm_cell_instance_t *lte_gsm_cell;
        gsm_cell_instance_t *gsm_cell;
        uint8_t j = 0;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_CELL_LOC_INFO_response_0x15_t);

        hdr++;
        tlv_0x15 = (QMI_NAS_GET_CELL_LOC_INFO_response_0x15_t *)hdr;
        out->lte_info_neighboring_gsm_available = true;

        if (tlv_0x15->ue_in_idle) {
            out->lte_info_neighboring_gsm.ue_in_idle = true;
        }

        COPY_MTOH(out->lte_info_neighboring_gsm.lte_gsm_cells_size, tlv_0x15->lte_gsm_cells_size);

        if (out->lte_info_neighboring_gsm.lte_gsm_cells_size > QMI_NAS_MAX_CELLS) {
            RLOGE(
                    "%s: lte_info_neighboring_gsm.lte_gsm_cells_size = %u > QMI_NAS_MAX_CELLS, increase the define",
                    __FUNCTION__, out->lte_info_neighboring_gsm.lte_gsm_cells_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x15->lte_gsm_cells_size));
        skip += sizeof(tlv_0x15->lte_gsm_cells_size);
        lte_gsm_cell = (lte_gsm_cell_instance_t *)skip;

        for (i = 0; i < out->lte_info_neighboring_gsm.lte_gsm_cells_size; i++) {
            COPY_MTOH(out->lte_info_neighboring_gsm.lte_gsm_cells[i].cell_resel_priority,
                    lte_gsm_cell->cell_resel_priority);
            COPY_MTOH(out->lte_info_neighboring_gsm.lte_gsm_cells[i].thresh_gsm_high,
                    lte_gsm_cell->thresh_gsm_high);
            COPY_MTOH(out->lte_info_neighboring_gsm.lte_gsm_cells[i].thresh_gsm_low,
                    lte_gsm_cell->thresh_gsm_low);
            COPY_MTOH(out->lte_info_neighboring_gsm.lte_gsm_cells[i].ncc_permitted,
                    lte_gsm_cell->ncc_permitted);

            COPY_MTOH(out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells_size,
                    lte_gsm_cell->gsm_cells_size);

            if (out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells_size > QMI_NAS_MAX_CELLS) {
                RLOGE(
                        "%s: lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells_size = %u > QMI_NAS_MAX_CELLS, increase the define",
                        __FUNCTION__, out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells_size);
                ret = QMI_SDK_ERR_INTERNAL;
                goto end;
            }

            skip = (uint8_t *)(&(lte_gsm_cell->gsm_cells_size));
            skip += sizeof(lte_gsm_cell->gsm_cells_size);
            gsm_cell = (gsm_cell_instance_t *)skip;

            for (j = 0; j < out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells_size; j++) {
                COPY_MTOH(out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].arfcn, gsm_cell->arfcn);
                COPY_MTOH(out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].band_1900,
                        gsm_cell->band_1900);
                COPY_MTOH(out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].cell_id_valid,
                        gsm_cell->cell_id_valid);
                COPY_MTOH(out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].bsic_id,
                        gsm_cell->bsic_id);
                COPY_MTOH(out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].rssi, gsm_cell->rssi);
                COPY_MTOH(out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].srxlev,
                        gsm_cell->srxlev);

                gsm_cell++;
            }

            lte_gsm_cell = (lte_gsm_cell_instance_t *)(gsm_cell);
        }
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    /* TLV 0x16 is optional */
    if (hdr) {
        QMI_NAS_GET_CELL_LOC_INFO_response_0x16_t *tlv_0x16;
        lte_wcdma_cell_instance_t *lte_wcdma_cell;
        wcdma_cell_instance_t *wcdma_cell;
        uint8_t j = 0;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_CELL_LOC_INFO_response_0x16_t);

        hdr++;
        tlv_0x16 = (QMI_NAS_GET_CELL_LOC_INFO_response_0x16_t *)hdr;
        out->lte_info_neighboring_wcdma_available = true;

        if (tlv_0x16->ue_in_idle) {
            out->lte_info_neighboring_wcdma.ue_in_idle = true;
        }

        COPY_MTOH(out->lte_info_neighboring_wcdma.lte_wcdma_cells_size, tlv_0x16->lte_wcdma_cells_size);

        if (out->lte_info_neighboring_wcdma.lte_wcdma_cells_size > QMI_NAS_MAX_CELLS) {
            RLOGE(
                    "%s: lte_info_neighboring_wcdma.lte_wcdma_cells_size = %u > QMI_NAS_MAX_CELLS, increase the define",
                    __FUNCTION__, out->lte_info_neighboring_wcdma.lte_wcdma_cells_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x16->lte_wcdma_cells_size));
        skip += sizeof(tlv_0x16->lte_wcdma_cells_size);
        lte_wcdma_cell = (lte_wcdma_cell_instance_t *)skip;

        for (i = 0; i < out->lte_info_neighboring_wcdma.lte_wcdma_cells_size; i++) {
            COPY_MTOH(out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].uarfcn, lte_wcdma_cell->uarfcn);
            COPY_MTOH(out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].cell_resel_priority,
                    lte_wcdma_cell->cell_resel_priority);
            COPY_MTOH(out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].thresh_Xhigh,
                    lte_wcdma_cell->thresh_Xhigh);
            COPY_MTOH(out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].threshXlow,
                    lte_wcdma_cell->threshXlow);

            COPY_MTOH(out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells_size,
                    lte_wcdma_cell->wcdma_cells_size);

            if (out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells_size > QMI_NAS_MAX_CELLS) {
                RLOGE(
                        "%s: lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells_len = %u > QMI_NAS_MAX_CELLS, increase the define",
                        __FUNCTION__, out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells_size);
                ret = QMI_SDK_ERR_INTERNAL;
                goto end;
            }

            skip = (uint8_t *)(&(lte_wcdma_cell->wcdma_cells_size));
            skip += sizeof(lte_wcdma_cell->wcdma_cells_size);
            wcdma_cell = (wcdma_cell_instance_t *)skip;

            for (j = 0; j < out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells_size; j++) {
                COPY_MTOH(out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells[j].psc,
                        wcdma_cell->psc);
                COPY_MTOH(out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells[j].cpich_rscp,
                        wcdma_cell->cpich_rscp);
                COPY_MTOH(out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells[j].cpich_ecno,
                        wcdma_cell->cpich_ecno);
                COPY_MTOH(out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells[j].srxlev,
                        wcdma_cell->srxlev);

                wcdma_cell++;
            }

            lte_wcdma_cell = (lte_wcdma_cell_instance_t *)(wcdma_cell);
        }
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    /* TLV 0x17 is optional */
    if (hdr) {
        QMI_NAS_GET_CELL_LOC_INFO_response_0x17_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_CELL_LOC_INFO_response_0x17_t);

        hdr++;
        tlv_0x17 = (QMI_NAS_GET_CELL_LOC_INFO_response_0x17_t *)hdr;
        out->umts_cell_id_available = true;
        COPY_MTOH(out->umts_cell_id, tlv_0x17->umts_cell_id);
    }

    hdr = search_TLV(msg, 0x18, payload_len);
    /* TLV 0x18 is optional */
    if (hdr) {
        QMI_NAS_GET_CELL_LOC_INFO_response_0x18_t *tlv_0x18;
        umts_lte_nbr_cell_instance_t *umts_lte_nbr_cell;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_CELL_LOC_INFO_response_0x18_t);

        hdr++;
        tlv_0x18 = (QMI_NAS_GET_CELL_LOC_INFO_response_0x18_t *)hdr;
        out->wcdma_info_lte_neighbor_cell_available = true;
        COPY_MTOH(out->wcdma_info_lte_neighbor_cell.wcdma_rrc_state, tlv_0x18->wcdma_rrc_state);
        COPY_MTOH(out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cell_size, tlv_0x18->umts_lte_nbr_cell_size);

        if (out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cell_size > QMI_NAS_MAX_CELLS) {
            RLOGE(
                    "%s: wcdma_info_lte_neighbor_cell.umts_lte_nbr_cell_size = %u > QMI_NAS_MAX_CELLS, increase the define",
                    __FUNCTION__, out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cell_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x18->umts_lte_nbr_cell_size));
        skip += sizeof(tlv_0x18->umts_lte_nbr_cell_size);
        umts_lte_nbr_cell = (umts_lte_nbr_cell_instance_t *)skip;

        for (i = 0; i < out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cell_size; i++) {
            COPY_MTOH(out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].earfcn,
                    umts_lte_nbr_cell->earfcn);
            COPY_MTOH(out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].pci, umts_lte_nbr_cell->pci);
            COPY_MTOH(out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].rsrp, umts_lte_nbr_cell->rsrp);
            COPY_MTOH(out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].rsrq, umts_lte_nbr_cell->rsrq);
            COPY_MTOH(out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].srxlev,
                    umts_lte_nbr_cell->srxlev);
            if (umts_lte_nbr_cell->cell_is_tdd) {
                out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].cell_is_tdd = true;
            }

            umts_lte_nbr_cell++;
        }
    }

    hdr = search_TLV(msg, 0x2E, payload_len);
    /* TLV 0x2E is optional */
    if (hdr) {
        QMI_NAS_GET_CELL_LOC_INFO_response_0x2E_t *tlv_0x2E;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_CELL_LOC_INFO_response_0x2E_t);

        hdr++;
        tlv_0x2E = (QMI_NAS_GET_CELL_LOC_INFO_response_0x2E_t *)hdr;
        out->nr5g_arfcn_available = true;
        COPY_MTOH(out->nr5g_arfcn, tlv_0x2E->nr5g_arfcn);
    }

    hdr = search_TLV(msg, 0x2F, payload_len);
    /* TLV 0x2F is optional */
    if (hdr) {
        QMI_NAS_GET_CELL_LOC_INFO_response_0x2F_t *tlv_0x2F;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_CELL_LOC_INFO_response_0x2F_t);

        hdr++;
        tlv_0x2F = (QMI_NAS_GET_CELL_LOC_INFO_response_0x2F_t *)hdr;
        out->nr5g_serving_cell_available = true;
        memcpy(out->nr5g_serving_cell.plmn, tlv_0x2F->plmn, QMI_NAS_PLMN_LEN);
        memcpy(out->nr5g_serving_cell.tac, tlv_0x2F->tac, 3);
        COPY_MTOH(out->nr5g_serving_cell.global_cell_id, tlv_0x2F->global_cell_id);
        COPY_MTOH(out->nr5g_serving_cell.pci, tlv_0x2F->pci);
        COPY_MTOH(out->nr5g_serving_cell.rsrq, tlv_0x2F->rsrq);
        COPY_MTOH(out->nr5g_serving_cell.rsrp, tlv_0x2F->rsrp);
        COPY_MTOH(out->nr5g_serving_cell.snr, tlv_0x2F->snr);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_NAS_GET_PLMN_NAME (68 - 0x0044)
 */

QMI_SDK_error_t telit_nas_get_plmn_name_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_plmn_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_NAS_GET_PLMN_NAME_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        return QMI_SDK_ERR_INVALID_ARG;
    }

    req_ctx->msg_id = QMI_NAS_GET_PLMN_NAME;
    req_ctx->svc_id = QMI_SVC_NAS;
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

    tlv_0x01 = (QMI_NAS_GET_PLMN_NAME_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->mcc, input->mcc);
    COPY_HTOM(tlv_0x01->mnc, input->mnc);

    buf_position += tlv_sz;

    if (input->mnc_includes_pcs_digit_available) {
        QMI_NAS_GET_PLMN_NAME_request_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = tlv_sz;

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_NAS_GET_PLMN_NAME_request_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->mnc_includes_pcs_digit, input->mnc_includes_pcs_digit);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_nas_get_plmn_name_unpack(uint8_t *rsp, uint16_t len, nas_plmn_names_t *out)
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
        QMI_NAS_GET_PLMN_NAME_response_0x10_t *tlv_0x10;
        uint8_t *skip = NULL;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_PLMN_NAME_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_NAS_GET_PLMN_NAME_response_0x10_t *)hdr;
        out->eons_plmn_name_available = true;
        COPY_MTOH(out->eons_plmn_name.name.spn_encoding, tlv_0x10->spn_enc);
        COPY_MTOH(out->eons_plmn_name.name.spn_name_len, tlv_0x10->spn_len);

        if (out->eons_plmn_name.name.spn_name_len > MAX_STRING_SIZE - 1) {
            RLOGE("%s: eons_plmn_name.name.spn_name_len = %u > MAX_STRING_SIZE, increase the define",
                    __FUNCTION__, out->eons_plmn_name.name.spn_name_len);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x10->spn_len));
        skip += sizeof(tlv_0x10->spn_len);
        memcpy(out->eons_plmn_name.name.spn_name, skip,
                out->eons_plmn_name.name.spn_name_len);
        skip += out->eons_plmn_name.name.spn_name_len;

        /* do not use COPY_MTOH, since it expects both DST and SRC not to be pointers */
        ec_mtoh(&(out->eons_plmn_name.name_short.short_name_encoding),
                sizeof(out->eons_plmn_name.name_short.short_name_encoding),
                skip, sizeof(*skip));
        skip++;
        ec_mtoh(&(out->eons_plmn_name.name_short.short_name_country_initials),
                sizeof(out->eons_plmn_name.name_short.short_name_country_initials),
                skip, sizeof(*skip));
        skip++;
        ec_mtoh(&(out->eons_plmn_name.name_short.short_name_spare_bits),
                sizeof(out->eons_plmn_name.name_short.short_name_spare_bits),
                skip, sizeof(*skip));
        skip++;
        ec_mtoh(&(out->eons_plmn_name.name_short.short_name_len),
                sizeof(out->eons_plmn_name.name_short.short_name_len),
                skip, sizeof(*skip));
        skip++;

        if (out->eons_plmn_name.name_short.short_name_len > MAX_STRING_SIZE - 1) {
            RLOGE("%s: eons_plmn_name.name_short.short_name_len = %u > MAX_STRING_SIZE, increase the define",
                    __FUNCTION__, out->eons_plmn_name.name_short.short_name_len);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        memcpy(out->eons_plmn_name.name_short.short_name, skip,
                out->eons_plmn_name.name_short.short_name_len);

        skip += out->eons_plmn_name.name_short.short_name_len;
        ec_mtoh(&(out->eons_plmn_name.name_long.long_name_encoding),
                sizeof(out->eons_plmn_name.name_long.long_name_encoding),
                skip, sizeof(*skip));
        skip++;
        ec_mtoh(&(out->eons_plmn_name.name_long.long_name_country_initials),
                sizeof(out->eons_plmn_name.name_long.long_name_country_initials),
                skip, sizeof(*skip));
        skip++;
        ec_mtoh(&(out->eons_plmn_name.name_long.long_name_long_bits),
                sizeof(out->eons_plmn_name.name_long.long_name_long_bits),
                skip, sizeof(*skip));
        skip++;
        ec_mtoh(&(out->eons_plmn_name.name_long.long_name_len),
                sizeof(out->eons_plmn_name.name_long.long_name_len),
                skip, sizeof(*skip));
        skip++;

        if (out->eons_plmn_name.name_long.long_name_len > MAX_STRING_SIZE - 1) {
            RLOGE("%s: eons_plmn_name.name_long.long_name_len = %u > MAX_STRING_SIZE, increase the define",
                    __FUNCTION__, out->eons_plmn_name.name_long.long_name_len);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        memcpy(out->eons_plmn_name.name_long.long_name, skip,
                out->eons_plmn_name.name_long.long_name_len);
    }

end:
    return ret;
}

/*****************************************************************************
 *  QMI_NAS_NETWORK_TIME_IND (76 - 0x004C)
 */
static QMI_SDK_error_t telit_nas_network_time_ind_unpack(uint16_t *payload_len, uint8_t *msg,
        nas_network_time_t *out, uint16_t out_len)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr;
    QMI_NAS_NETWORK_TIME_IND_0x01_t *tlv_0x01;

    if (out_len < sizeof(*out)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }
    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(msg, 0x01, *payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_NAS_NETWORK_TIME_IND_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_NAS_NETWORK_TIME_IND_0x01_t *)hdr;
    COPY_MTOH(out->year, tlv_0x01->year);
    COPY_MTOH(out->month, tlv_0x01->month);
    COPY_MTOH(out->day, tlv_0x01->day);
    COPY_MTOH(out->hour, tlv_0x01->hour);
    COPY_MTOH(out->minute, tlv_0x01->minute);
    COPY_MTOH(out->second, tlv_0x01->second);
    COPY_MTOH(out->day_of_week, tlv_0x01->day_of_week);

    hdr = search_TLV(msg, 0x10, *payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_NAS_NETWORK_TIME_IND_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(*tlv_0x10);

        hdr++;
        tlv_0x10 = (QMI_NAS_NETWORK_TIME_IND_0x10_t *) hdr;
        COPY_MTOH(out->time_zone, tlv_0x10->time_zone);
        out->time_zone_available = true;
    }

    hdr = search_TLV(msg, 0x11, *payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_NAS_NETWORK_TIME_IND_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(*tlv_0x11);

        hdr++;
        tlv_0x11 = (QMI_NAS_NETWORK_TIME_IND_0x11_t *) hdr;
        COPY_MTOH(out->daylt_sav_adj, tlv_0x11->daylt_sav_adj);
        out->daylt_sav_adj_available = true;
    }

    hdr = search_TLV(msg, 0x12, *payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_NAS_NETWORK_TIME_IND_0x12_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(*tlv_0x12);

        hdr++;
        tlv_0x12 = (QMI_NAS_NETWORK_TIME_IND_0x12_t *) hdr;
        COPY_MTOH(out->radio_if, tlv_0x12->radio_if);
        out->radio_if_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_NAS_GET_SYS_INFO (77 - 0x004D)
 */

QMI_SDK_error_t telit_nas_get_sys_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_NAS_GET_SYS_INFO,
            QMI_SVC_NAS,
            QMI_TIMEOUT_SMALL);
}

static QMI_SDK_error_t telit_nas_generic_sys_info_unpack(uint8_t *rsp, uint16_t len, nas_system_info_t *out,
        bool indication)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

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

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_NAS_GET_SYSTEM_INFO_response_0x11_t *)hdr;
        COPY_MTOH(out->hdr_service_status_info.preferred_data_path, tlv_0x11->preferred_data_path);
        COPY_MTOH(out->hdr_service_status_info.service_status, tlv_0x11->service_status);
        out->hdr_service_status_info_available = true;
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_NAS_generic_service_status_info_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_service_status_info_t);

        hdr++;
        tlv_0x12 = (QMI_NAS_generic_service_status_info_t *)hdr;
        COPY_MTOH(out->gsm_service_status_info.preferred_data_path, tlv_0x12->preferred_data_path);
        COPY_MTOH(out->gsm_service_status_info.service_status, tlv_0x12->service_status);
        COPY_MTOH(out->gsm_service_status_info.true_service_status, tlv_0x12->true_service_status);
        out->gsm_service_status_info_available = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_NAS_generic_service_status_info_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_service_status_info_t);

        hdr++;
        tlv_0x13 = (QMI_NAS_generic_service_status_info_t *)hdr;
        COPY_MTOH(out->wcdma_service_status_info.preferred_data_path, tlv_0x13->preferred_data_path);
        COPY_MTOH(out->wcdma_service_status_info.service_status, tlv_0x13->service_status);
        COPY_MTOH(out->wcdma_service_status_info.true_service_status, tlv_0x13->true_service_status);
        out->wcdma_service_status_info_available = true;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_NAS_generic_service_status_info_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_service_status_info_t);

        hdr++;
        tlv_0x14 = (QMI_NAS_generic_service_status_info_t *)hdr;
        COPY_MTOH(out->lte_service_status_info.preferred_data_path, tlv_0x14->preferred_data_path);
        COPY_MTOH(out->lte_service_status_info.service_status, tlv_0x14->service_status);
        COPY_MTOH(out->lte_service_status_info.true_service_status, tlv_0x14->true_service_status);
        out->lte_service_status_info_available = true;
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    /* TLV 0x16 is optional */
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x16_t *tlv_0x16;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x16_t);

        hdr++;
        tlv_0x16 = (QMI_NAS_GET_SYSTEM_INFO_response_0x16_t *)hdr;
        COPY_MTOH(out->HDR_sys_info.srv_domain_available, tlv_0x16->srv_domain_valid);
        COPY_MTOH(out->HDR_sys_info.srv_domain, tlv_0x16->srv_domain);
        COPY_MTOH(out->HDR_sys_info.srv_capability_available, tlv_0x16->srv_capability_valid);
        COPY_MTOH(out->HDR_sys_info.srv_capability, tlv_0x16->srv_capability);
        COPY_MTOH(out->HDR_sys_info.roam_status_available, tlv_0x16->roam_status_valid);
        COPY_MTOH(out->HDR_sys_info.roam_status, tlv_0x16->roam_status);
        COPY_MTOH(out->HDR_sys_info.is_sys_forbidden_available, tlv_0x16->is_sys_forbidden_valid);
        COPY_MTOH(out->HDR_sys_info.is_sys_forbidden, tlv_0x16->is_sys_forbidden);
        COPY_MTOH(out->HDR_sys_info.is_sys_prl_match_available, tlv_0x16->is_sys_prl_match_valid);
        COPY_MTOH(out->HDR_sys_info.is_sys_prl_match, tlv_0x16->is_sys_prl_match);
        COPY_MTOH(out->HDR_sys_info.hdr_personality_available, tlv_0x16->hdr_personality_valid);
        COPY_MTOH(out->HDR_sys_info.hdr_personality, tlv_0x16->hdr_personality);
        COPY_MTOH(out->HDR_sys_info.hdr_active_prot_available, tlv_0x16->hdr_active_prot_valid);
        COPY_MTOH(out->HDR_sys_info.hdr_active_prot, tlv_0x16->hdr_active_prot);
        COPY_MTOH(out->HDR_sys_info.is856_sys_id_available, tlv_0x16->is856_sys_id_valid);
        memcpy(out->HDR_sys_info.is856_sys_id, tlv_0x16->is856_sys_id, SYS_ID_SIZE);
        out->HDR_sys_info_available = true;
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    /* TLV 0x17 is optional */
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x17_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x17_t);

        hdr++;
        tlv_0x17 = (QMI_NAS_GET_SYSTEM_INFO_response_0x17_t *)hdr;
        COPY_MTOH(out->gsm_sys_info.srv_domain_available, tlv_0x17->srv_domain_valid);
        COPY_MTOH(out->gsm_sys_info.srv_domain, tlv_0x17->srv_domain);
        COPY_MTOH(out->gsm_sys_info.srv_capability_available, tlv_0x17->srv_capability_valid);
        COPY_MTOH(out->gsm_sys_info.srv_capability, tlv_0x17->srv_capability);
        COPY_MTOH(out->gsm_sys_info.roam_status_available, tlv_0x17->roam_status_valid);
        COPY_MTOH(out->gsm_sys_info.roam_status, tlv_0x17->roam_status);
        COPY_MTOH(out->gsm_sys_info.is_sys_forbidden_available, tlv_0x17->is_sys_forbidden_valid);
        COPY_MTOH(out->gsm_sys_info.is_sys_forbidden, tlv_0x17->is_sys_forbidden);
        COPY_MTOH(out->gsm_sys_info.lac_available, tlv_0x17->lac_valid);
        COPY_MTOH(out->gsm_sys_info.lac, tlv_0x17->lac);
        COPY_MTOH(out->gsm_sys_info.cell_id_available, tlv_0x17->cell_id_valid);
        COPY_MTOH(out->gsm_sys_info.cell_id, tlv_0x17->cell_id);
        COPY_MTOH(out->gsm_sys_info.reg_reject_info_available, tlv_0x17->reg_reject_info_valid);
        COPY_MTOH(out->gsm_sys_info.reject_srv_domain, tlv_0x17->reject_srv_domain);
        COPY_MTOH(out->gsm_sys_info.rej_cause, tlv_0x17->rej_cause);
        COPY_MTOH(out->gsm_sys_info.network_id_available, tlv_0x17->network_id_valid);
        memcpy(out->gsm_sys_info.mcc, tlv_0x17->mcc, 3);
        memcpy(out->gsm_sys_info.mnc, tlv_0x17->mnc, 3);
        COPY_MTOH(out->gsm_sys_info.egprs_supp_available, tlv_0x17->egprs_supp_valid);
        COPY_MTOH(out->gsm_sys_info.egprs_supp, tlv_0x17->egprs_supp);
        COPY_MTOH(out->gsm_sys_info.dtm_supp_available, tlv_0x17->dtm_supp_valid);
        COPY_MTOH(out->gsm_sys_info.dtm_supp, tlv_0x17->dtm_supp);
        out->gsm_sys_info_available = true;
    }

    hdr = search_TLV(msg, 0x18, payload_len);
    /* TLV 0x18 is optional */
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x18_t *tlv_0x18;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x18_t);

        hdr++;
        tlv_0x18 = (QMI_NAS_GET_SYSTEM_INFO_response_0x18_t *)hdr;
        COPY_MTOH(out->wcdma_sys_info.srv_domain_available, tlv_0x18->srv_domain_valid);
        COPY_MTOH(out->wcdma_sys_info.srv_domain, tlv_0x18->srv_domain);
        COPY_MTOH(out->wcdma_sys_info.srv_capability_available, tlv_0x18->srv_capability_valid);
        COPY_MTOH(out->wcdma_sys_info.srv_capability, tlv_0x18->srv_capability);
        COPY_MTOH(out->wcdma_sys_info.roam_status_available, tlv_0x18->roam_status_valid);
        COPY_MTOH(out->wcdma_sys_info.roam_status, tlv_0x18->roam_status);
        COPY_MTOH(out->wcdma_sys_info.is_sys_forbidden_available, tlv_0x18->is_sys_forbidden_valid);
        COPY_MTOH(out->wcdma_sys_info.is_sys_forbidden, tlv_0x18->is_sys_forbidden);
        COPY_MTOH(out->wcdma_sys_info.lac_available, tlv_0x18->lac_valid);
        COPY_MTOH(out->wcdma_sys_info.lac, tlv_0x18->lac);
        COPY_MTOH(out->wcdma_sys_info.cell_id_available, tlv_0x18->cell_id_valid);
        COPY_MTOH(out->wcdma_sys_info.cell_id, tlv_0x18->cell_id);
        COPY_MTOH(out->wcdma_sys_info.reg_reject_info_available, tlv_0x18->reg_reject_info_valid);
        COPY_MTOH(out->wcdma_sys_info.reject_srv_domain, tlv_0x18->reject_srv_domain);
        COPY_MTOH(out->wcdma_sys_info.rej_cause, tlv_0x18->rej_cause);
        COPY_MTOH(out->wcdma_sys_info.network_id_available, tlv_0x18->network_id_valid);
        memcpy(out->wcdma_sys_info.mcc, tlv_0x18->mcc, 3);
        memcpy(out->wcdma_sys_info.mnc, tlv_0x18->mnc, 3);
        COPY_MTOH(out->wcdma_sys_info.hs_call_status_available, tlv_0x18->hs_call_status_valid);
        COPY_MTOH(out->wcdma_sys_info.hs_call_status, tlv_0x18->hs_call_status);
        COPY_MTOH(out->wcdma_sys_info.hs_ind_available, tlv_0x18->hs_ind_valid);
        COPY_MTOH(out->wcdma_sys_info.hs_ind, tlv_0x18->hs_ind);
        COPY_MTOH(out->wcdma_sys_info.psc_available, tlv_0x18->psc_valid);
        COPY_MTOH(out->wcdma_sys_info.psc, tlv_0x18->psc);
        out->wcdma_sys_info_available = true;
    }

    hdr = search_TLV(msg, 0x19, payload_len);
    /* TLV 0x19 is optional */
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x19_t *tlv_0x19;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x19_t);

        hdr++;
        tlv_0x19 = (QMI_NAS_GET_SYSTEM_INFO_response_0x19_t *)hdr;
        COPY_MTOH(out->lte_sys_info.srv_domain_available, tlv_0x19->srv_domain_valid);
        COPY_MTOH(out->lte_sys_info.srv_domain, tlv_0x19->srv_domain);
        COPY_MTOH(out->lte_sys_info.srv_capability_available, tlv_0x19->srv_capability_valid);
        COPY_MTOH(out->lte_sys_info.srv_capability, tlv_0x19->srv_capability);
        COPY_MTOH(out->lte_sys_info.roam_status_available, tlv_0x19->roam_status_valid);
        COPY_MTOH(out->lte_sys_info.roam_status, tlv_0x19->roam_status);
        COPY_MTOH(out->lte_sys_info.is_sys_forbidden_available, tlv_0x19->is_sys_forbidden_valid);
        COPY_MTOH(out->lte_sys_info.is_sys_forbidden, tlv_0x19->is_sys_forbidden);
        COPY_MTOH(out->lte_sys_info.lac_available, tlv_0x19->lac_valid);
        COPY_MTOH(out->lte_sys_info.lac, tlv_0x19->lac);
        COPY_MTOH(out->lte_sys_info.cell_id_available, tlv_0x19->cell_id_valid);
        COPY_MTOH(out->lte_sys_info.cell_id, tlv_0x19->cell_id);
        COPY_MTOH(out->lte_sys_info.reg_reject_info_available, tlv_0x19->reg_reject_info_valid);
        COPY_MTOH(out->lte_sys_info.reject_srv_domain, tlv_0x19->reject_srv_domain);
        COPY_MTOH(out->lte_sys_info.rej_cause, tlv_0x19->rej_cause);
        COPY_MTOH(out->lte_sys_info.network_id_available, tlv_0x19->network_id_valid);
        memcpy(out->lte_sys_info.mcc, tlv_0x19->mcc, 3);
        memcpy(out->lte_sys_info.mnc, tlv_0x19->mnc, 3);
        COPY_MTOH(out->lte_sys_info.tac_available, tlv_0x19->tac_valid);
        COPY_MTOH(out->lte_sys_info.tac, tlv_0x19->tac);
        out->lte_sys_info_available = true;
    }

    hdr = search_TLV(msg, 0x1B, payload_len);
    /* TLV 0x1B is optional */
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x1B_t *tlv_0x1B;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x1B_t);

        hdr++;
        tlv_0x1B = (QMI_NAS_GET_SYSTEM_INFO_response_0x1B_t *)hdr;
        COPY_MTOH(out->add_hdr_sys_info, tlv_0x1B->reg_prd);
        out->add_hdr_sys_info_available = true;
    }

    hdr = search_TLV(msg, 0x1C, payload_len);
    /* TLV 0x1C is optional */
    if (hdr) {
        QMI_NAS_generic_additional_system_info_t *tlv_0x1C;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_additional_system_info_t);

        hdr++;
        tlv_0x1C = (QMI_NAS_generic_additional_system_info_t *)hdr;
        COPY_MTOH(out->add_gsm_sys_info.cell_broadcast_cap, tlv_0x1C->cell_broadcast_cap);
        COPY_MTOH(out->add_gsm_sys_info.geo_sys_idx, tlv_0x1C->geo_sys_idx);
        out->add_gsm_sys_info_available = true;
    }

    hdr = search_TLV(msg, 0x1D, payload_len);
    /* TLV 0x1D is optional */
    if (hdr) {
        QMI_NAS_generic_additional_system_info_t *tlv_0x1D;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_additional_system_info_t);

        hdr++;
        tlv_0x1D = (QMI_NAS_generic_additional_system_info_t *)hdr;
        COPY_MTOH(out->add_wcdma_sys_info.cell_broadcast_cap, tlv_0x1D->cell_broadcast_cap);
        COPY_MTOH(out->add_wcdma_sys_info.geo_sys_idx, tlv_0x1D->geo_sys_idx);
        out->add_wcdma_sys_info_available = true;
    }

    hdr = search_TLV(msg, 0x1E, payload_len);
    /* TLV 0x1E is optional */
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x1E_t *tlv_0x1E;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x1E_t);

        hdr++;
        tlv_0x1E = (QMI_NAS_GET_SYSTEM_INFO_response_0x1E_t *)hdr;
        COPY_MTOH(out->lte_geo_sys_idx, tlv_0x1E->geo_sys_idx);
        out->lte_geo_sys_idx_available = true;
    }

    hdr = search_TLV(msg, 0x1F, payload_len);
    /* TLV 0x1F is optional */
    if (hdr) {
        QMI_NAS_generic_call_barring_info_t *tlv_0x1F;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_call_barring_info_t);

        hdr++;
        tlv_0x1F = (QMI_NAS_generic_call_barring_info_t *)hdr;
        COPY_MTOH(out->gsm_call_barring_sys_info.cs_bar_status, tlv_0x1F->cs_bar_status);
        COPY_MTOH(out->gsm_call_barring_sys_info.ps_bar_status, tlv_0x1F->ps_bar_status);
        out->gsm_call_barring_sys_info_available = true;
    }

    hdr = search_TLV(msg, 0x20, payload_len);
    /* TLV 0x20 is optional */
    if (hdr) {
        QMI_NAS_generic_call_barring_info_t *tlv_0x20;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_call_barring_info_t);

        hdr++;
        tlv_0x20 = (QMI_NAS_generic_call_barring_info_t *)hdr;
        COPY_MTOH(out->wcdma_call_barring_sys_info.cs_bar_status, tlv_0x20->cs_bar_status);
        COPY_MTOH(out->wcdma_call_barring_sys_info.ps_bar_status, tlv_0x20->ps_bar_status);
        out->wcdma_call_barring_sys_info_available = true;
    }

    hdr = search_TLV(msg, 0x21, payload_len);
    /* TLV 0x21 is optional */
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x21_t *tlv_0x21;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x21_t);

        hdr++;
        tlv_0x21 = (QMI_NAS_GET_SYSTEM_INFO_response_0x21_t *)hdr;
        COPY_MTOH(out->lte_voice_support_sys_info, tlv_0x21->voice_support_on_lte);
    }

    hdr = search_TLV(msg, 0x22, payload_len);
    /* TLV 0x22 is optional */
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x22_t *tlv_0x22;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x22_t);

        hdr++;
        tlv_0x22 = (QMI_NAS_GET_SYSTEM_INFO_response_0x22_t *)hdr;
        COPY_MTOH(out->gsm_cipher_domain_sys_info, tlv_0x22->gsm_cipher_domain);
        out->gsm_cipher_domain_sys_info_available = true;
    }

    hdr = search_TLV(msg, 0x23, payload_len);
    /* TLV 0x23 is optional */
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x23_t *tlv_0x23;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x23_t);

        hdr++;
        tlv_0x23 = (QMI_NAS_GET_SYSTEM_INFO_response_0x23_t *)hdr;
        COPY_MTOH(out->wcdma_cipher_domain_sys_info, tlv_0x23->wcdma_cipher_domain);
        out->wcdma_cipher_domain_sys_info_available = true;
    }

    if (indication) {
        hdr = search_TLV(msg, 0x4C, payload_len);
    } else {
        hdr = search_TLV(msg, 0x4A, payload_len);
    }
    if (hdr) {
        QMI_NAS_generic_service_status_info_t *tlv_0x4a;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_service_status_info_t);

        hdr++;
        tlv_0x4a = (QMI_NAS_generic_service_status_info_t *)hdr;
        COPY_MTOH(out->nr5g_service_status_info.preferred_data_path, tlv_0x4a->preferred_data_path);
        COPY_MTOH(out->nr5g_service_status_info.service_status, tlv_0x4a->service_status);
        COPY_MTOH(out->nr5g_service_status_info.true_service_status, tlv_0x4a->true_service_status);
        out->nr5g_service_status_info_available = true;
    }

    if (indication) {
        hdr = search_TLV(msg, 0x4D, payload_len);
    } else {
        hdr = search_TLV(msg, 0x4B, payload_len);
    }
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x4B_t *tlv_0x4B;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x4B_t);

        hdr++;
        tlv_0x4B = (QMI_NAS_GET_SYSTEM_INFO_response_0x4B_t *)hdr;
        COPY_MTOH(out->nr5g_sys_info.srv_domain_available, tlv_0x4B->srv_domain_valid);
        COPY_MTOH(out->nr5g_sys_info.srv_domain, tlv_0x4B->srv_domain);
        COPY_MTOH(out->nr5g_sys_info.srv_capability_available, tlv_0x4B->srv_capability_valid);
        COPY_MTOH(out->nr5g_sys_info.srv_capability, tlv_0x4B->srv_capability);
        COPY_MTOH(out->nr5g_sys_info.roam_status_available, tlv_0x4B->roam_status_valid);
        COPY_MTOH(out->nr5g_sys_info.roam_status, tlv_0x4B->roam_status);
        COPY_MTOH(out->nr5g_sys_info.is_sys_forbidden_available, tlv_0x4B->is_sys_forbidden_valid);
        COPY_MTOH(out->nr5g_sys_info.is_sys_forbidden, tlv_0x4B->is_sys_forbidden);
        COPY_MTOH(out->nr5g_sys_info.lac_available, tlv_0x4B->lac_valid);
        COPY_MTOH(out->nr5g_sys_info.lac, tlv_0x4B->lac);
        COPY_MTOH(out->nr5g_sys_info.cell_id_available, tlv_0x4B->cell_id_valid);
        COPY_MTOH(out->nr5g_sys_info.cell_id, tlv_0x4B->cell_id);
        COPY_MTOH(out->nr5g_sys_info.reg_reject_info_available, tlv_0x4B->reg_reject_info_valid);
        COPY_MTOH(out->nr5g_sys_info.reject_srv_domain, tlv_0x4B->reject_srv_domain);
        COPY_MTOH(out->nr5g_sys_info.rej_cause, tlv_0x4B->rej_cause);
        COPY_MTOH(out->nr5g_sys_info.network_id_available, tlv_0x4B->network_id_valid);
        memcpy(out->nr5g_sys_info.mcc, tlv_0x4B->mcc, 3);
        memcpy(out->nr5g_sys_info.mnc, tlv_0x4B->mnc, 3);
        COPY_MTOH(out->nr5g_sys_info.tac_available, tlv_0x4B->tac_valid);
        COPY_MTOH(out->nr5g_sys_info.tac, tlv_0x4B->tac);
        out->nr5g_sys_info_available = true;
    }

    if (indication) {
        hdr = search_TLV(msg, 0x4E, payload_len);
    } else {
        hdr = search_TLV(msg, 0x4C, payload_len);
    }
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x4C_t *tlv_0x4C;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x4C_t);

        hdr++;
        tlv_0x4C = (QMI_NAS_GET_SYSTEM_INFO_response_0x4C_t *)hdr;
        COPY_MTOH(out->nr5g_cell_status, tlv_0x4C->nr5g_cell_status);
        out->nr5g_cell_status_available = true;
    }

    if (indication) {
        hdr = search_TLV(msg, 0x50, payload_len);
    } else {
        hdr = search_TLV(msg, 0x4E, payload_len);
    }
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x4E_t *tlv_0x4E;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x4E_t);

        hdr++;
        tlv_0x4E = (QMI_NAS_GET_SYSTEM_INFO_response_0x4E_t *)hdr;
        COPY_MTOH(out->endc_availability, tlv_0x4E->endc_availability);
        out->endc_availability_available = true;
    }

    if (indication) {
        hdr = search_TLV(msg, 0x51, payload_len);
    } else {
        hdr = search_TLV(msg, 0x4F, payload_len);
    }
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x4F_t *tlv_0x4F;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x4F_t);

        hdr++;
        tlv_0x4F = (QMI_NAS_GET_SYSTEM_INFO_response_0x4F_t *)hdr;
        COPY_MTOH(out->restrict_dcnr, tlv_0x4F->restrict_dcnr);
        out->restrict_dcnr_available = true;
    }

    if (indication) {
        hdr = search_TLV(msg, 0x52, payload_len);
    } else {
        hdr = search_TLV(msg, 0x50, payload_len);
    }
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x50_t *tlv_0x50;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x50_t);

        hdr++;
        tlv_0x50 = (QMI_NAS_GET_SYSTEM_INFO_response_0x50_t *)hdr;
        memcpy(out->nr5g_tac, tlv_0x50->tac, 3);
        out->nr5g_tac_available = true;
    }

    if (indication) {
        hdr = search_TLV(msg, 0x56, payload_len);
    } else {
        hdr = search_TLV(msg, 0x54, payload_len);
    }
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x54_t *tlv_0x54;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x54_t);

        hdr++;
        tlv_0x54 = (QMI_NAS_GET_SYSTEM_INFO_response_0x54_t *)hdr;
        COPY_MTOH(out->nr5g_pci, tlv_0x54->nr5g_pci);
        out->nr5g_pci_available = true;
    }

    if (indication) {
        hdr = search_TLV(msg, 0x57, payload_len);
    } else {
        hdr = search_TLV(msg, 0x55, payload_len);
    }
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x55_t *tlv_0x55;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x55_t);

        hdr++;
        tlv_0x55 = (QMI_NAS_GET_SYSTEM_INFO_response_0x55_t *)hdr;
        COPY_MTOH(out->plmn_infolist_r15_availability, tlv_0x55->plmn_infolist_r15_availability);
        out->plmn_infolist_r15_availability_available = true;
    }

    if (indication) {
        hdr = search_TLV(msg, 0x60, payload_len);
    } else {
        hdr = search_TLV(msg, 0x5E, payload_len);
    }
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x5E_t *tlv_0x5E;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x5E_t);

        hdr++;
        tlv_0x5E = (QMI_NAS_GET_SYSTEM_INFO_response_0x5E_t *)hdr;
        COPY_MTOH(out->nr5g_cell_id, tlv_0x5E->nr5g_cell_id);
        out->nr5g_cell_id_available = true;
    }

    if (indication) {
        hdr = search_TLV(msg, 0x61, payload_len);
    } else {
        hdr = search_TLV(msg, 0x5F, payload_len);
    }
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x5F_t *tlv_0x5F;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x5F_t);

        hdr++;
        tlv_0x5F = (QMI_NAS_GET_SYSTEM_INFO_response_0x5F_t *)hdr;
        COPY_MTOH(out->sys_info_plmn.radio_if, tlv_0x5F->radio_if);
        memcpy(out->sys_info_plmn.mcc, tlv_0x5F->mcc, 3);
        memcpy(out->sys_info_plmn.mnc, tlv_0x5F->mnc, 3);
        out->sys_info_plmn_available = true;
    }

    if (indication) {
        hdr = search_TLV(msg, 0x64, payload_len);
    } else {
        hdr = search_TLV(msg, 0x60, payload_len);
    }
    if (hdr) {
        QMI_NAS_GET_SYSTEM_INFO_response_0x60_t *tlv_0x60;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SYSTEM_INFO_response_0x60_t);

        hdr++;
        tlv_0x60 = (QMI_NAS_GET_SYSTEM_INFO_response_0x60_t *)hdr;
        COPY_MTOH(out->nr5g_arfcn, tlv_0x60->nr5g_arfcn);
        out->nr5g_arfcn_available = true;
    }

end:
    return ret;
}

QMI_SDK_error_t telit_nas_get_sys_info_unpack(uint8_t *rsp, uint16_t len, nas_system_info_t *out)
{
    return telit_nas_generic_sys_info_unpack(rsp, len, out, false);
}

/*****************************************************************************
 * QMI_NAS_GET_SIG_INFO (79 - 0x004F)
 */

static QMI_SDK_error_t telit_nas_generic_signal_info_unpack(uint8_t *rsp, uint16_t len,
        nas_signal_info_t *out,
        bool indication)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

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

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_NAS_GET_SIGNAL_INFO_response_0x12_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SIGNAL_INFO_response_0x12_t);

        hdr++;
        tlv_0x12 = (QMI_NAS_GET_SIGNAL_INFO_response_0x12_t *)hdr;
        COPY_MTOH(out->signal_info_gsm.rssi, tlv_0x12->RSSI);
        out->signal_info_gsm_set = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_NAS_GET_SIGNAL_INFO_response_0x13_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SIGNAL_INFO_response_0x13_t);

        hdr++;
        tlv_0x13 = (QMI_NAS_GET_SIGNAL_INFO_response_0x13_t *)hdr;
        COPY_MTOH(out->signal_info_wcdma.rssi, tlv_0x13->RSSI);
        COPY_MTOH(out->signal_info_wcdma.ecio, tlv_0x13->ECIO);
        out->signal_info_wcdma_set = true;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_NAS_GET_SIGNAL_INFO_response_0x14_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SIGNAL_INFO_response_0x14_t);

        hdr++;
        tlv_0x14 = (QMI_NAS_GET_SIGNAL_INFO_response_0x14_t *)hdr;
        COPY_MTOH(out->signal_info_lte.rssi, tlv_0x14->RSSI);
        COPY_MTOH(out->signal_info_lte.rsrq, tlv_0x14->RSRQ);
        COPY_MTOH(out->signal_info_lte.rsrp, tlv_0x14->RSRP);
        COPY_MTOH(out->signal_info_lte.snr, tlv_0x14->SNR);
        out->signal_info_lte_set = true;
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    /* TLV 0x17 is optional */
    if (hdr) {
        QMI_NAS_GET_SIGNAL_INFO_response_0x17_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_SIGNAL_INFO_response_0x17_t);

        hdr++;
        tlv_0x17 = (QMI_NAS_GET_SIGNAL_INFO_response_0x17_t *)hdr;
        COPY_MTOH(out->signal_info_nr5g.rsrp, tlv_0x17->RSRP);
        COPY_MTOH(out->signal_info_nr5g.snr, tlv_0x17->SNR);
        out->signal_info_nr5g_set = true;
    }

end:
    return ret;
}

QMI_SDK_error_t telit_nas_get_sig_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_NAS_GET_SIG_INFO,
            QMI_SVC_NAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_nas_get_sig_info_unpack(uint8_t *rsp, uint16_t len, nas_signal_info_t *out)
{
    return telit_nas_generic_signal_info_unpack(rsp, len, out, false);
}

/*****************************************************************************
 * QMI_NAS_GET_TX_RX_INFO (90 - 0x005A)
 */

QMI_SDK_error_t telit_nas_get_tx_rx_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_radio_if_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_NAS_GET_TX_RX_INFO_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_NAS_GET_TX_RX_INFO;
    req_ctx->svc_id = QMI_SVC_NAS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = tlv_sz;

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_NAS_GET_TX_RX_INFO_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->radio_if, input->radio_if);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_nas_get_tx_rx_info_unpack(uint8_t *rsp, uint16_t len, nas_tx_rx_info_t *out)
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
        QMI_NAS_GET_TX_RX_INFO_response_0x10_0x11_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_TX_RX_INFO_response_0x10_0x11_t);

        hdr++;
        tlv_0x10 = (QMI_NAS_GET_TX_RX_INFO_response_0x10_0x11_t *)hdr;
        out->rx_chain_0_info_available = true;
        COPY_MTOH(out->rx_chain_0_info.is_radio_tuned, tlv_0x10->is_radio_tuned);
        COPY_MTOH(out->rx_chain_0_info.rx_pwr, tlv_0x10->rx_pwr);
        COPY_MTOH(out->rx_chain_0_info.ecio, tlv_0x10->ecio);
        COPY_MTOH(out->rx_chain_0_info.rscp, tlv_0x10->rscp);
        COPY_MTOH(out->rx_chain_0_info.rsrp, tlv_0x10->rsrp);
        COPY_MTOH(out->rx_chain_0_info.phase, tlv_0x10->phase);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_NAS_GET_TX_RX_INFO_response_0x10_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_TX_RX_INFO_response_0x10_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_NAS_GET_TX_RX_INFO_response_0x10_0x11_t *)hdr;
        out->rx_chain_1_info_available = true;
        COPY_MTOH(out->rx_chain_1_info.is_radio_tuned, tlv_0x11->is_radio_tuned);
        COPY_MTOH(out->rx_chain_1_info.rx_pwr, tlv_0x11->rx_pwr);
        COPY_MTOH(out->rx_chain_1_info.ecio, tlv_0x11->ecio);
        COPY_MTOH(out->rx_chain_1_info.rscp, tlv_0x11->rscp);
        COPY_MTOH(out->rx_chain_1_info.rsrp, tlv_0x11->rsrp);
        COPY_MTOH(out->rx_chain_1_info.phase, tlv_0x11->phase);
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_NAS_GET_TX_RX_INFO_response_0x12_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_TX_RX_INFO_response_0x12_t);

        hdr++;
        tlv_0x12 = (QMI_NAS_GET_TX_RX_INFO_response_0x12_t *)hdr;
        out->tx_info_available = true;
        COPY_MTOH(out->tx_info.is_in_traffic, tlv_0x12->is_in_traffic);
        COPY_MTOH(out->tx_info.tx_pwr, tlv_0x12->tx_pwr);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_NAS_CONFIG_SIG_INFO2 (108 - 0x006C)
 */

QMI_SDK_error_t telit_nas_config_sig_info2_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_config_sig2_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    uint32_t hdr_sz;
    uint16_t tlv_sz;
    int16_t *threshold;
    int i;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_NAS_CONFIG_SIG_INFO2;
    req_ctx->svc_id = QMI_SVC_NAS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    /* Add TLV 0x1C */
    if (input->gsm_rssi_thr_list_size) {
        QMI_NAS_generic_threshold_list_t *tlv_0x1C;

        if (input->gsm_rssi_thr_list_size > QMI_NAS_THRESHOLD_LIST_MAX_SIZE) {
            RLOGE("%s: gsm_rssi_thr_list_size = %u > QMI_NAS_THRESHOLD_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->gsm_rssi_thr_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x1C);
        tlv_sz += (input->gsm_rssi_thr_list_size * sizeof(int16_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1C;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1C = (QMI_NAS_generic_threshold_list_t *)buf_position;
        COPY_HTOM(tlv_0x1C->threshold_list_size, input->gsm_rssi_thr_list_size);
        buf_position += sizeof(tlv_0x1C->threshold_list_size);
        threshold = (int16_t *)buf_position;
        for (i = 0; i < input->gsm_rssi_thr_list_size; i++)
            COPY_HTOM(threshold[i], input->gsm_rssi_thr_list[i]);

        buf_position += input->gsm_rssi_thr_list_size * sizeof(int16_t);
    }

    /* Add TLV 0x1D */
    if (input->gsm_rssi_delta_available) {
        QMI_NAS_generic_delta_t *tlv_0x1D;

        tlv_sz = sizeof(*tlv_0x1D);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1D;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1D = (QMI_NAS_generic_delta_t *)buf_position;
        COPY_HTOM(tlv_0x1D->delta, input->gsm_rssi_delta);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x1E */
    if (input->wcdma_rssi_thr_list_size) {
        QMI_NAS_generic_threshold_list_t *tlv_0x1E;

        if (input->wcdma_rssi_thr_list_size > QMI_NAS_THRESHOLD_LIST_MAX_SIZE) {
            RLOGE("%s: wcdma_rssi_thr_list_size = %u > QMI_NAS_THRESHOLD_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->wcdma_rssi_thr_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x1E);
        tlv_sz += (input->wcdma_rssi_thr_list_size * sizeof(int16_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1E;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1E = (QMI_NAS_generic_threshold_list_t *)buf_position;
        COPY_HTOM(tlv_0x1E->threshold_list_size, input->wcdma_rssi_thr_list_size);
        buf_position += sizeof(tlv_0x1E->threshold_list_size);
        threshold = (int16_t *)buf_position;
        for (i = 0; i < input->wcdma_rssi_thr_list_size; i++)
            COPY_HTOM(threshold[i], input->wcdma_rssi_thr_list[i]);

        buf_position += input->wcdma_rssi_thr_list_size * sizeof(int16_t);
    }

    /* Add TLV 0x1F */
    if (input->wcdma_rssi_delta_available) {
        QMI_NAS_generic_delta_t *tlv_0x1F;

        tlv_sz = sizeof(*tlv_0x1F);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1F;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1F = (QMI_NAS_generic_delta_t *)buf_position;
        COPY_HTOM(tlv_0x1F->delta, input->wcdma_rssi_delta);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x20 */
    if (input->wcdma_ecio_thr_list_size) {
        QMI_NAS_generic_threshold_list_t *tlv_0x20;

        if (input->wcdma_ecio_thr_list_size > QMI_NAS_THRESHOLD_LIST_MAX_SIZE) {
            RLOGE("%s: wcdma_ecio_thr_list_size = %u > QMI_NAS_THRESHOLD_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->wcdma_ecio_thr_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x20);
        tlv_sz += (input->wcdma_ecio_thr_list_size * sizeof(int16_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x20;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x20 = (QMI_NAS_generic_threshold_list_t *)buf_position;
        COPY_HTOM(tlv_0x20->threshold_list_size, input->wcdma_ecio_thr_list_size);
        buf_position += sizeof(tlv_0x20->threshold_list_size);
        threshold = (int16_t *)buf_position;
        for (i = 0; i < input->wcdma_ecio_thr_list_size; i++)
            COPY_HTOM(threshold[i], input->wcdma_ecio_thr_list[i]);

        buf_position += input->wcdma_ecio_thr_list_size * sizeof(int16_t);
    }

    /* Add TLV 0x21 */
    if (input->wcdma_ecio_delta_available) {
        QMI_NAS_generic_delta_t *tlv_0x21;

        tlv_sz = sizeof(*tlv_0x21);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x21;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x21 = (QMI_NAS_generic_delta_t *)buf_position;
        COPY_HTOM(tlv_0x21->delta, input->wcdma_ecio_delta);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x22 */
    if (input->lte_rssi_thr_list_size) {
        QMI_NAS_generic_threshold_list_t *tlv_0x22;

        if (input->lte_rssi_thr_list_size > QMI_NAS_THRESHOLD_LIST_MAX_SIZE) {
            RLOGE("%s: lte_rssi_thr_list_size = %u > QMI_NAS_THRESHOLD_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->lte_rssi_thr_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x22);
        tlv_sz += (input->lte_rssi_thr_list_size * sizeof(int16_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x22;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x22 = (QMI_NAS_generic_threshold_list_t *)buf_position;
        COPY_HTOM(tlv_0x22->threshold_list_size, input->lte_rssi_thr_list_size);
        buf_position += sizeof(tlv_0x22->threshold_list_size);
        threshold = (int16_t *)buf_position;
        for (i = 0; i < input->lte_rssi_thr_list_size; i++)
            COPY_HTOM(threshold[i], input->lte_rssi_thr_list[i]);

        buf_position += input->lte_rssi_thr_list_size * sizeof(int16_t);
    }

    /* Add TLV 0x23 */
    if (input->lte_rssi_delta_available) {
        QMI_NAS_generic_delta_t *tlv_0x23;

        tlv_sz = sizeof(*tlv_0x23);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x23;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x23 = (QMI_NAS_generic_delta_t *)buf_position;
        COPY_HTOM(tlv_0x23->delta, input->lte_rssi_delta);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x24 */
    if (input->lte_snr_thr_list_size) {
        QMI_NAS_generic_threshold_list_t *tlv_0x24;

        if (input->lte_snr_thr_list_size > QMI_NAS_THRESHOLD_LIST_MAX_SIZE) {
            RLOGE("%s: lte_snr_thr_list_size = %u > QMI_NAS_THRESHOLD_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->lte_snr_thr_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x24);
        tlv_sz += (input->lte_snr_thr_list_size * sizeof(int16_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x24;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x24 = (QMI_NAS_generic_threshold_list_t *)buf_position;
        COPY_HTOM(tlv_0x24->threshold_list_size, input->lte_snr_thr_list_size);
        buf_position += sizeof(tlv_0x24->threshold_list_size);
        threshold = (int16_t *)buf_position;
        for (i = 0; i < input->lte_snr_thr_list_size; i++)
            COPY_HTOM(threshold[i], input->lte_snr_thr_list[i]);

        buf_position += input->lte_snr_thr_list_size * sizeof(int16_t);
    }

    /* Add TLV 0x25 */
    if (input->lte_snr_delta_available) {
        QMI_NAS_generic_delta_t *tlv_0x25;

        tlv_sz = sizeof(*tlv_0x25);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x25;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x25 = (QMI_NAS_generic_delta_t *)buf_position;
        COPY_HTOM(tlv_0x25->delta, input->lte_snr_delta);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x26 */
    if (input->lte_rsrq_thr_list_size) {
        QMI_NAS_generic_threshold_list_t *tlv_0x26;

        if (input->lte_rsrq_thr_list_size > QMI_NAS_THRESHOLD_LIST_MAX_SIZE) {
            RLOGE("%s: lte_rsrq_thr_list_size = %u > QMI_NAS_THRESHOLD_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->lte_rsrq_thr_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x26);
        tlv_sz += (input->lte_rsrq_thr_list_size * sizeof(int16_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x26;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x26 = (QMI_NAS_generic_threshold_list_t *)buf_position;
        COPY_HTOM(tlv_0x26->threshold_list_size, input->lte_rsrq_thr_list_size);
        buf_position += sizeof(tlv_0x26->threshold_list_size);
        threshold = (int16_t *)buf_position;
        for (i = 0; i < input->lte_rsrq_thr_list_size; i++)
            COPY_HTOM(threshold[i], input->lte_rsrq_thr_list[i]);

        buf_position += input->lte_rsrq_thr_list_size * sizeof(int16_t);
    }

    /* Add TLV 0x27 */
    if (input->lte_rsrq_delta_available) {
        QMI_NAS_generic_delta_t *tlv_0x27;

        tlv_sz = sizeof(*tlv_0x27);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x27;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x27 = (QMI_NAS_generic_delta_t *)buf_position;
        COPY_HTOM(tlv_0x27->delta, input->lte_rsrq_delta);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x28 */
    if (input->lte_rsrp_thr_list_size) {
        QMI_NAS_generic_threshold_list_t *tlv_0x28;

        if (input->lte_rsrp_thr_list_size > QMI_NAS_THRESHOLD_LIST_MAX_SIZE) {
            RLOGE("%s: lte_rsrp_thr_list_size = %u > QMI_NAS_THRESHOLD_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->lte_rsrp_thr_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x28);
        tlv_sz += (input->lte_rsrp_thr_list_size * sizeof(int16_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x28;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x28 = (QMI_NAS_generic_threshold_list_t *)buf_position;
        COPY_HTOM(tlv_0x28->threshold_list_size, input->lte_rsrp_thr_list_size);
        buf_position += sizeof(tlv_0x28->threshold_list_size);
        threshold = (int16_t *)buf_position;
        for (i = 0; i < input->lte_rsrp_thr_list_size; i++)
            COPY_HTOM(threshold[i], input->lte_rsrp_thr_list[i]);

        buf_position += input->lte_rsrp_thr_list_size * sizeof(int16_t);
    }

    /* Add TLV 0x29 */
    if (input->lte_rsrp_delta_available) {
        QMI_NAS_generic_delta_t *tlv_0x29;

        tlv_sz = sizeof(*tlv_0x29);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x29;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x29 = (QMI_NAS_generic_delta_t *)buf_position;
        COPY_HTOM(tlv_0x29->delta, input->lte_rsrp_delta);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x2A */
    if (input->lte_signal_report_config_available) {
        QMI_NAS_generic_signal_rate_t *tlv_0x2A;

        tlv_sz = sizeof(*tlv_0x2A);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x2A;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x2A = (QMI_NAS_generic_signal_rate_t *)buf_position;
        COPY_HTOM(tlv_0x2A->rpt_rate, input->lte_signal_report_config.rpt_rate);
        COPY_HTOM(tlv_0x2A->avg_period, input->lte_signal_report_config.avg_period);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x33 */
    if (input->nr5g_snr_thr_list_size) {
        QMI_NAS_generic_threshold_list_t *tlv_0x33;

        if (input->nr5g_snr_thr_list_size > QMI_NAS_THRESHOLD_LIST_MAX_SIZE) {
            RLOGE("%s: nr5g_snr_thr_list_size = %u > QMI_NAS_THRESHOLD_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->nr5g_snr_thr_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x33);
        tlv_sz += (input->nr5g_snr_thr_list_size * sizeof(int16_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x33;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x33 = (QMI_NAS_generic_threshold_list_t *)buf_position;
        COPY_HTOM(tlv_0x33->threshold_list_size, input->nr5g_snr_thr_list_size);
        buf_position += sizeof(tlv_0x33->threshold_list_size);
        threshold = (int16_t *)buf_position;
        for (i = 0; i < input->nr5g_snr_thr_list_size; i++)
            COPY_HTOM(threshold[i], input->nr5g_snr_thr_list[i]);

        buf_position += input->nr5g_snr_thr_list_size * sizeof(int16_t);
    }

    /* Add TLV 0x34 */
    if (input->nr5g_snr_delta_available) {
        QMI_NAS_generic_delta_t *tlv_0x34;

        tlv_sz = sizeof(*tlv_0x34);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x34;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x34 = (QMI_NAS_generic_delta_t *)buf_position;
        COPY_HTOM(tlv_0x34->delta, input->nr5g_snr_delta);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x35 */
    if (input->nr5g_rsrp_thr_list_size) {
        QMI_NAS_generic_threshold_list_t *tlv_0x35;

        if (input->nr5g_rsrp_thr_list_size > QMI_NAS_THRESHOLD_LIST_MAX_SIZE) {
            RLOGE("%s: nr5g_rsrp_thr_list_size = %u > QMI_NAS_THRESHOLD_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->nr5g_rsrp_thr_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x35);
        tlv_sz += (input->nr5g_rsrp_thr_list_size * sizeof(int16_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x35;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x35 = (QMI_NAS_generic_threshold_list_t *)buf_position;
        COPY_HTOM(tlv_0x35->threshold_list_size, input->nr5g_rsrp_thr_list_size);
        buf_position += sizeof(tlv_0x35->threshold_list_size);
        threshold = (int16_t *)buf_position;
        for (i = 0; i < input->nr5g_rsrp_thr_list_size; i++)
            COPY_HTOM(threshold[i], input->nr5g_rsrp_thr_list[i]);

        buf_position += input->nr5g_rsrp_thr_list_size * sizeof(int16_t);
    }

    /* Add TLV 0x36 */
    if (input->nr5g_rsrp_delta_available) {
        QMI_NAS_generic_delta_t *tlv_0x36;

        tlv_sz = sizeof(*tlv_0x36);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x36;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x36 = (QMI_NAS_generic_delta_t *)buf_position;
        COPY_HTOM(tlv_0x36->delta, input->nr5g_rsrp_delta);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x37 */
    if (input->nr5g_signal_report_config_available) {
        QMI_NAS_generic_signal_rate_t *tlv_0x37;

        tlv_sz = sizeof(*tlv_0x37);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x37;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x37 = (QMI_NAS_generic_signal_rate_t *)buf_position;
        COPY_HTOM(tlv_0x37->rpt_rate, input->nr5g_signal_report_config.rpt_rate);
        COPY_HTOM(tlv_0x37->avg_period, input->nr5g_signal_report_config.avg_period);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x38 */
    if (input->nr5g_rsrq_thr_list_size) {
        QMI_NAS_generic_threshold_list_t *tlv_0x38;

        if (input->nr5g_rsrq_thr_list_size > QMI_NAS_THRESHOLD_LIST_MAX_SIZE) {
            RLOGE("%s: nr5g_rsrq_thr_list_size = %u > QMI_NAS_THRESHOLD_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->nr5g_rsrq_thr_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x38);
        tlv_sz += (input->nr5g_rsrq_thr_list_size * sizeof(int16_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x38;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x38 = (QMI_NAS_generic_threshold_list_t *)buf_position;
        COPY_HTOM(tlv_0x38->threshold_list_size, input->nr5g_rsrq_thr_list_size);
        buf_position += sizeof(tlv_0x38->threshold_list_size);
        threshold = (int16_t *)buf_position;
        for (i = 0; i < input->nr5g_rsrq_thr_list_size; i++)
            COPY_HTOM(threshold[i], input->nr5g_rsrq_thr_list[i]);

        buf_position += input->nr5g_rsrq_thr_list_size * sizeof(int16_t);
    }

    /* Add TLV 0x39 */
    if (input->nr5g_rsrq_delta_available) {
        QMI_NAS_generic_delta_t *tlv_0x39;

        tlv_sz = sizeof(*tlv_0x39);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x39;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x39 = (QMI_NAS_generic_delta_t *)buf_position;
        COPY_HTOM(tlv_0x39->delta, input->nr5g_rsrq_delta);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x3A */
    if (input->wcdma_rscp_thr_list_size) {
        QMI_NAS_generic_threshold_list_t *tlv_0x3A;

        if (input->wcdma_rscp_thr_list_size > QMI_NAS_THRESHOLD_LIST_MAX_SIZE) {
            RLOGE("%s: wcdma_ecio_thr_list_size = %u > QMI_NAS_THRESHOLD_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->wcdma_rscp_thr_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x3A);
        tlv_sz += (input->wcdma_rscp_thr_list_size * sizeof(int16_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x3A;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x3A = (QMI_NAS_generic_threshold_list_t *)buf_position;
        COPY_HTOM(tlv_0x3A->threshold_list_size, input->wcdma_rscp_thr_list_size);
        buf_position += sizeof(tlv_0x3A->threshold_list_size);
        threshold = (int16_t *)buf_position;
        for (i = 0; i < input->wcdma_rscp_thr_list_size; i++)
            COPY_HTOM(threshold[i], input->wcdma_rscp_thr_list[i]);

        buf_position += input->wcdma_rscp_thr_list_size * sizeof(int16_t);
    }

    /* Add TLV 0x3B */
    if (input->wcdma_rscp_delta_available) {
        QMI_NAS_generic_delta_t *tlv_0x3B;

        tlv_sz = sizeof(*tlv_0x3B);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x3B;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x3B = (QMI_NAS_generic_delta_t *)buf_position;
        COPY_HTOM(tlv_0x3B->delta, input->wcdma_rscp_delta);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x3C */
    if (input->wcdma_hysteresis_list_size) {
        QMI_NAS_generic_hysteresis_list_t *tlv_0x3C;
        QMI_NAS_generic_hysteresis_item_t *hysteresis_item;

        if (input->wcdma_hysteresis_list_size > QMI_NAS_HYSTERESIS_LIST_MAX_SIZE) {
            RLOGE("%s: wcdma_ecio_thr_list_size = %u > QMI_NAS_HYSTERESIS_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->wcdma_hysteresis_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x3C);
        tlv_sz += (input->wcdma_hysteresis_list_size * sizeof(QMI_NAS_generic_hysteresis_item_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x3C;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x3C = (QMI_NAS_generic_hysteresis_list_t *)buf_position;
        COPY_HTOM(tlv_0x3C->hysteresis_list_size, input->wcdma_hysteresis_list_size);
        buf_position += sizeof(tlv_0x3C->hysteresis_list_size);
        hysteresis_item = (QMI_NAS_generic_hysteresis_item_t *)buf_position;
        for (i = 0; i < input->wcdma_hysteresis_list_size; i++) {
            COPY_HTOM(hysteresis_item[i].signal, input->wcdma_hysteresis_list[i].signal);
            COPY_HTOM(hysteresis_item[i].delta, input->wcdma_hysteresis_list[i].delta);
        }

        buf_position += input->wcdma_hysteresis_list_size * sizeof(QMI_NAS_generic_hysteresis_item_t);
    }

    /* Add TLV 0x3D */
    if (input->gsm_hysteresis_list_size) {
        QMI_NAS_generic_hysteresis_list_t *tlv_0x3D;
        QMI_NAS_generic_hysteresis_item_t *hysteresis_item;

        if (input->gsm_hysteresis_list_size > QMI_NAS_HYSTERESIS_LIST_MAX_SIZE) {
            RLOGE("%s: wcdma_ecio_thr_list_size = %u > QMI_NAS_HYSTERESIS_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->gsm_hysteresis_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x3D);
        tlv_sz += (input->gsm_hysteresis_list_size * sizeof(QMI_NAS_generic_hysteresis_item_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x3D;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x3D = (QMI_NAS_generic_hysteresis_list_t *)buf_position;
        COPY_HTOM(tlv_0x3D->hysteresis_list_size, input->gsm_hysteresis_list_size);
        buf_position += sizeof(tlv_0x3D->hysteresis_list_size);
        hysteresis_item = (QMI_NAS_generic_hysteresis_item_t *)buf_position;
        for (i = 0; i < input->gsm_hysteresis_list_size; i++) {
            COPY_HTOM(hysteresis_item[i].signal, input->gsm_hysteresis_list[i].signal);
            COPY_HTOM(hysteresis_item[i].delta, input->gsm_hysteresis_list[i].delta);
        }

        buf_position += input->gsm_hysteresis_list_size * sizeof(QMI_NAS_generic_hysteresis_item_t);
    }

    /* Add TLV 0x3E */
    if (input->lte_hysteresis_list_size) {
        QMI_NAS_generic_hysteresis_list_t *tlv_0x3E;
        QMI_NAS_generic_hysteresis_item_t *hysteresis_item;

        if (input->lte_hysteresis_list_size > QMI_NAS_HYSTERESIS_LIST_MAX_SIZE) {
            RLOGE("%s: wcdma_ecio_thr_list_size = %u > QMI_NAS_HYSTERESIS_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->lte_hysteresis_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x3E);
        tlv_sz += (input->lte_hysteresis_list_size * sizeof(QMI_NAS_generic_hysteresis_item_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x3E;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x3E = (QMI_NAS_generic_hysteresis_list_t *)buf_position;
        COPY_HTOM(tlv_0x3E->hysteresis_list_size, input->lte_hysteresis_list_size);
        buf_position += sizeof(tlv_0x3E->hysteresis_list_size);
        hysteresis_item = (QMI_NAS_generic_hysteresis_item_t *)buf_position;
        for (i = 0; i < input->lte_hysteresis_list_size; i++) {
            COPY_HTOM(hysteresis_item[i].signal, input->lte_hysteresis_list[i].signal);
            COPY_HTOM(hysteresis_item[i].delta, input->lte_hysteresis_list[i].delta);
        }

        buf_position += input->lte_hysteresis_list_size * sizeof(QMI_NAS_generic_hysteresis_item_t);
    }

    /* Add TLV 0x42 */
    if (input->hysteresis_timer_available) {
        QMI_NAS_generic_hysteresis_timer_t *tlv_0x42;

        tlv_sz = sizeof(*tlv_0x42);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x42;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x42 = (QMI_NAS_generic_hysteresis_timer_t *)buf_position;
        COPY_HTOM(tlv_0x42->hysteresis_timer, input->hysteresis_timer);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x43 */
    if (input->nr5g_hysteresis_list_size) {
        QMI_NAS_generic_hysteresis_list_t *tlv_0x43;
        QMI_NAS_generic_hysteresis_item_t *hysteresis_item;

        if (input->nr5g_hysteresis_list_size > QMI_NAS_HYSTERESIS_LIST_MAX_SIZE) {
            RLOGE("%s: wcdma_ecio_thr_list_size = %u > QMI_NAS_HYSTERESIS_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->nr5g_hysteresis_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tlv_sz = sizeof(*tlv_0x43);
        tlv_sz += (input->nr5g_hysteresis_list_size * sizeof(QMI_NAS_generic_hysteresis_item_t));
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x43;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x43 = (QMI_NAS_generic_hysteresis_list_t *)buf_position;
        COPY_HTOM(tlv_0x43->hysteresis_list_size, input->nr5g_hysteresis_list_size);
        buf_position += sizeof(tlv_0x43->hysteresis_list_size);
        hysteresis_item = (QMI_NAS_generic_hysteresis_item_t *)buf_position;
        for (i = 0; i < input->nr5g_hysteresis_list_size; i++) {
            COPY_HTOM(hysteresis_item[i].signal, input->nr5g_hysteresis_list[i].signal);
            COPY_HTOM(hysteresis_item[i].delta, input->nr5g_hysteresis_list[i].delta);
        }
    }

    ret = QMI_SDK_ERR_NONE;

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_nas_config_sig_info2_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING (112 - 0x0070)
 */

QMI_SDK_error_t telit_nas_limit_sys_info_ind_reporting_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        nas_limit_sys_info_ind_reporting_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    const uint32_t hdr_sz = sizeof(*hdr);
    void *original_buffer = req;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint16_t tlv_sz;
    QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING_request_0x01_t *tlv_0x01;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING;
    req_ctx->svc_id = QMI_SVC_NAS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    buf_position = skip_msg_header(req);

    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *) buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING_request_0x01_t *) buf_position;
    COPY_HTOM(tlv_0x01->limit_sys_info_chg_rpt, input->limit_sys_info_chg_rpt);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_nas_limit_sys_info_ind_reporting_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_NAS_GET_NETWORK_TIME (125 - 0x007D)
 */

QMI_SDK_error_t telit_nas_get_network_time_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_NAS_GET_NETWORK_TIME,
            QMI_SVC_NAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_nas_get_network_time_unpack(uint8_t *rsp, uint16_t len, nas_network_time_info_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, (void *)out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_NAS_generic_time_info_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_time_info_t);

        hdr++;
        tlv_0x10 = (QMI_NAS_generic_time_info_t *)hdr;
        COPY_MTOH(out->time_information_3gpp2.year, tlv_0x10->year);
        COPY_MTOH(out->time_information_3gpp2.month, tlv_0x10->month);
        COPY_MTOH(out->time_information_3gpp2.day, tlv_0x10->day);
        COPY_MTOH(out->time_information_3gpp2.hour, tlv_0x10->hour);
        COPY_MTOH(out->time_information_3gpp2.minute, tlv_0x10->minute);
        COPY_MTOH(out->time_information_3gpp2.second, tlv_0x10->second);
        COPY_MTOH(out->time_information_3gpp2.time_zone, tlv_0x10->time_zone);
        COPY_MTOH(out->time_information_3gpp2.day_of_week, tlv_0x10->day_of_week);
        COPY_MTOH(out->time_information_3gpp2.daylt_sav_adj, tlv_0x10->daylt_sav_adj);
        COPY_MTOH(out->time_information_3gpp2.radio_if, tlv_0x10->radio_if);
        out->time_information_3gpp2.time_zone_available = true;
        out->time_information_3gpp2.daylt_sav_adj_available = true;
        out->time_information_3gpp2.radio_if_available = true;
        out->time_information_3gpp2_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_NAS_generic_time_info_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_generic_time_info_t);

        hdr++;
        tlv_0x11 = (QMI_NAS_generic_time_info_t *)hdr;
        COPY_MTOH(out->time_information_3gpp.year, tlv_0x11->year);
        COPY_MTOH(out->time_information_3gpp.month, tlv_0x11->month);
        COPY_MTOH(out->time_information_3gpp.day, tlv_0x11->day);
        COPY_MTOH(out->time_information_3gpp.hour, tlv_0x11->hour);
        COPY_MTOH(out->time_information_3gpp.minute, tlv_0x11->minute);
        COPY_MTOH(out->time_information_3gpp.second, tlv_0x11->second);
        COPY_MTOH(out->time_information_3gpp.time_zone, tlv_0x11->time_zone);
        COPY_MTOH(out->time_information_3gpp.day_of_week, tlv_0x11->day_of_week);
        COPY_MTOH(out->time_information_3gpp.daylt_sav_adj, tlv_0x11->daylt_sav_adj);
        COPY_MTOH(out->time_information_3gpp.radio_if, tlv_0x11->radio_if);
        out->time_information_3gpp.time_zone_available = true;
        out->time_information_3gpp.daylt_sav_adj_available = true;
        out->time_information_3gpp.radio_if_available = true;
        out->time_information_3gpp_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_NAS_GET_LTE_CPHY_CA_INFO (172 - 0x00AC)
 */

QMI_SDK_error_t telit_nas_get_lte_cphy_ca_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_NAS_GET_LTE_CPHY_CA_INFO,
            QMI_SVC_NAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_nas_get_lte_cphy_ca_info_unpack(uint8_t *rsp,
        uint16_t len,
        nas_get_lte_cphy_ca_info_t *out)
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
        QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x10_t *)hdr;
        COPY_MTOH(out->scell_ind_type.freq, tlv_0x10->freq);
        COPY_MTOH(out->scell_ind_type.pci, tlv_0x10->pci);
        COPY_MTOH(out->scell_ind_type.scell_state, tlv_0x10->scell_state);
        out->scell_ind_type_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x11_t *)hdr;
        COPY_MTOH(out->dl_bw_value, tlv_0x11->cphy_ca_dl_bandwidth);
        out->dl_bw_value_available = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x13_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x13_t);

        hdr++;
        tlv_0x13 = (QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x13_t *)hdr;
        COPY_MTOH(out->pcell_info.pci, tlv_0x13->pci);
        COPY_MTOH(out->pcell_info.freq, tlv_0x13->freq);
        COPY_MTOH(out->pcell_info.dl_bw_value, tlv_0x13->cphy_ca_dl_bandwidth);
        COPY_MTOH(out->pcell_info.band, tlv_0x13->band);
        out->pcell_info_available = true;
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    /* TLV 0x15 is optional */
    if (hdr) {
        QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x15_t *tlv_0x15;
        cphy_scell_info_instance_t *cphy_scell_info_instance;
        uint8_t *skip = NULL;
        uint8_t i = 0;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x15_t);

        hdr++;
        tlv_0x15 = (QMI_NAS_GET_LTE_CPHY_CA_INFO_response_0x15_t *)hdr;
        COPY_MTOH(out->scell_info_list_size, tlv_0x15->cphy_scell_info_list_size);
        skip = (uint8_t *)(&(tlv_0x15->cphy_scell_info_list_size));
        /* No need to check scell_info_list_size, since it's uint8_t always <= QMI_NAS_MAX_SCELL_INFO_INSTANCES */
        skip += sizeof(out->scell_info_list_size);
        cphy_scell_info_instance = (cphy_scell_info_instance_t *)skip;

        if (out->scell_info_list_size > 0) {
            for (i = 0; i < out->scell_info_list_size; i++) {
                COPY_MTOH(out->scell_info_list[i].pci, cphy_scell_info_instance->pci);
                COPY_MTOH(out->scell_info_list[i].freq, cphy_scell_info_instance->freq);
                COPY_MTOH(out->scell_info_list[i].dl_bw_value,
                        cphy_scell_info_instance->cphy_ca_dl_bandwidth);
                COPY_MTOH(out->scell_info_list[i].band, cphy_scell_info_instance->band);
                COPY_MTOH(out->scell_info_list[i].scell_state, cphy_scell_info_instance->scell_state);
                COPY_MTOH(out->scell_info_list[i].scell_idx, cphy_scell_info_instance->scell_idx);
                cphy_scell_info_instance++;
            }
        } else {
            RLOGW("%s: no instances available", __FUNCTION__);
        }
    }

end:
    return ret;
}


/*****************************************************************************
 *  QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND (288 - 0x0120)
 */
static QMI_SDK_error_t telit_nas_nr5g_time_sync_pulse_report_ind_unpack(uint16_t *payload_len, uint8_t *msg,
        nas_nr5g_time_sync_pulse_report_t *out, uint16_t out_len)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr;

    if (out_len < sizeof(*out)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }
    memset(out, 0, sizeof(*out));

    hdr = search_TLV(msg, 0x10, *payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x10_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x10_t *) hdr;
        COPY_MTOH(out->gps_time, tlv->gps_time);
        out->gps_time_available = true;
    }

    hdr = search_TLV(msg, 0x11, *payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x11_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x11_t *) hdr;
        COPY_MTOH(out->sfn, tlv->sfn);
        out->sfn_available = true;
    }

    hdr = search_TLV(msg, 0x12, *payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x12_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x12_t *) hdr;
        COPY_MTOH(out->nta, tlv->nta);
        out->nta_available = true;
    }

    hdr = search_TLV(msg, 0x13, *payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x13_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x13_t *) hdr;
        COPY_MTOH(out->nta_offset, tlv->nta_offset);
        out->nta_offset_available = true;
    }

    hdr = search_TLV(msg, 0x14, *payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x14_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x14_t *) hdr;
        COPY_MTOH(out->leapseconds, tlv->leapseconds);
        out->leapseconds_available = true;
    }

    hdr = search_TLV(msg, 0x15, *payload_len);
    /* TLV 0x15 is optional */
    if (hdr) {
        QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x15_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND_0x15_t *) hdr;
        COPY_MTOH(out->utc_time, tlv->utc_time);
        out->utc_time_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 *  QMI_NAS_NR5G_LOST_FRAME_SYNC_IND (289 - 0x0121)
 */
static QMI_SDK_error_t telit_nas_nr5g_lost_frame_sync_ind_unpack(uint16_t *payload_len, uint8_t *msg,
        nas_nr5g_lost_frame_sync_t *out, uint16_t out_len)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr;

    if (out_len < sizeof(*out)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }
    memset(out, 0, sizeof(*out));

    hdr = search_TLV(msg, 0x10, *payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_NAS_NR5G_LOST_FRAME_SYNC_IND_0x10_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_NAS_NR5G_LOST_FRAME_SYNC_IND_0x10_t *) hdr;
        COPY_MTOH(out->nr5g_sync_lost_reason, tlv->nr5g_sync_lost_reason);
        out->nr5g_sync_lost_reason_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_NAS_SET_NR5G_SYNC_PULSE_GEN (290 - 0x0122)
 */

QMI_SDK_error_t telit_nas_set_nr5g_sync_pulse_gen_pack(pack_qmi_t *req_ctx, void *req, uint16_t *len,
        nas_set_nr5g_sync_pulse_gen_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    const uint32_t hdr_sz = sizeof(*hdr);
    void *original_buffer = req;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_NAS_SET_NR5G_SYNC_PULSE_GEN;
    req_ctx->svc_id = QMI_SVC_NAS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    buf_position = skip_msg_header(req);

    /* Add TLV 0x01 */
    if (input->pulse_period > 128) {
        RLOGE("%s: pulse_period = %u > 128", __FUNCTION__, input->pulse_period);
        goto end;
    } else {
        QMI_NAS_SET_NR5G_SYNC_PULSE_GEN_0x01_t *tlv;

        tlv_sz = sizeof(*tlv);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *) buf_position;
        hdr->type_id = 0x01;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_NAS_SET_NR5G_SYNC_PULSE_GEN_0x01_t *) buf_position;
        COPY_HTOM(tlv->pulse_period, input->pulse_period);

        buf_position += tlv_sz;
    }

    if (input->start_sfn_available) {
        QMI_NAS_SET_NR5G_SYNC_PULSE_GEN_0x10_t *tlv;

        if (input->start_sfn > 1024) {
            RLOGE("%s: start_sfn = %u > 1024", __FUNCTION__, input->start_sfn);
            goto end;
        }

        tlv_sz = sizeof(*tlv);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *) buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_NAS_SET_NR5G_SYNC_PULSE_GEN_0x10_t *) buf_position;
        COPY_HTOM(tlv->start_sfn, input->start_sfn);

        buf_position += tlv_sz;
    }

    if (input->report_period_available) {
        QMI_NAS_SET_NR5G_SYNC_PULSE_GEN_0x11_t *tlv;

        if (input->report_period > 128) {
            RLOGE("%s: report_period = %u > 128", __FUNCTION__, input->report_period);
            goto end;
        }

        tlv_sz = sizeof(*tlv);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *) buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_NAS_SET_NR5G_SYNC_PULSE_GEN_0x11_t *) buf_position;
        COPY_HTOM(tlv->report_period, input->report_period);

        buf_position += tlv_sz;
    }

    add_header(req_ctx, payload_len, original_buffer, len);

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

QMI_SDK_error_t telit_nas_set_nr5g_sync_pulse_gen_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_NAS_NR5G_RRC_UTC_TIME_UPDATE_IND (335 - 0x0149)
 * QMI_NAS_GET_NR5G_RRC_UTC_TIME (336 - 0x0150)
 */

QMI_SDK_error_t telit_nas_get_nr5g_rrc_utc_time_pack(pack_qmi_t *req_ctx, void *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_NAS_GET_NR5G_RRC_UTC_TIME,
            QMI_SVC_NAS,
            QMI_TIMEOUT_SMALL);
}

static QMI_SDK_error_t telit_nas_generic_nr5g_rrc_utc_time_unpack(uint8_t *rsp, uint16_t len,
        nas_nr5g_rrc_utc_time_t *out, size_t out_len, bool indication)
{
    QMI_raw_content_header_t *hdr;
    uint16_t payload_len;
    QMI_SDK_error_t ret;
    uint8_t *msg;

    if (out_len < sizeof(*out)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }
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
        QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x10_t *tlv;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x10_t);

        hdr++;
        tlv = (QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x10_t *) hdr;
        out->utc_sib9_acquired_available = true;
        COPY_MTOH(out->utc_sib9_acquired, tlv->utc_sib9_acquired);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x11_t *tlv;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x11_t);

        hdr++;
        tlv = (QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x11_t *) hdr;
        out->r16_timing_info_available = true;
        COPY_MTOH(out->r16_timing_info, tlv->r16_timing_info);
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x12_t *tlv;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x12_t);

        hdr++;
        tlv = (QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x12_t *) hdr;
        out->day_light_saving_time_available = true;
        COPY_MTOH(out->day_light_saving_time, tlv->day_light_saving_time);
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x13_t *tlv;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x13_t);

        hdr++;
        tlv = (QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x13_t *) hdr;
        out->leap_seconds_available = true;
        COPY_MTOH(out->leap_seconds, tlv->leap_seconds);
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x14_t *tlv;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x14_t);

        hdr++;
        tlv = (QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x14_t *) hdr;
        out->local_time_offset_available = true;
        COPY_MTOH(out->local_time_offset, tlv->local_time_offset);
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    /* TLV 0x15 is optional */
    if (hdr) {
        QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x15_t *tlv;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x15_t);

        hdr++;
        tlv = (QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x15_t *) hdr;
        out->reference_time_info_available = true;
        COPY_MTOH(out->reference_time_info.ref_days, tlv->ref_days);
        COPY_MTOH(out->reference_time_info.ref_seconds, tlv->ref_seconds);
        COPY_MTOH(out->reference_time_info.ref_milliseconds, tlv->ref_milliseconds);
        COPY_MTOH(out->reference_time_info.ref_ten_nano_seconds, tlv->ref_ten_nano_seconds);
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    /* TLV 0x16 is optional */
    if (hdr) {
        QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x16_t *tlv;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x16_t);

        hdr++;
        tlv = (QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x16_t *) hdr;
        out->uncertainty_available = true;
        COPY_MTOH(out->uncertainty, tlv->uncertainty);
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    /* TLV 0x17 is optional */
    if (hdr) {
        QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x17_t *tlv;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x17_t);

        hdr++;
        tlv = (QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x17_t *) hdr;
        out->time_info_type_available = true;
        COPY_MTOH(out->time_info_type, tlv->time_info_type);
    }

    hdr = search_TLV(msg, 0x18, payload_len);
    /* TLV 0x18 is optional */
    if (hdr) {
        QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x18_t *tlv;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x18_t);

        hdr++;
        tlv = (QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x18_t *) hdr;
        out->reference_sfn_available = true;
        COPY_MTOH(out->reference_sfn, tlv->reference_sfn);
    }

    hdr = search_TLV(msg, 0x19, payload_len);
    /* TLV 0x19 is optional */
    if (hdr) {
        QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x19_t *tlv;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x19_t);

        hdr++;
        tlv = (QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x19_t *) hdr;
        out->abs_time_available = true;
        COPY_MTOH(out->abs_time, tlv->abs_time);
    }

    hdr = search_TLV(msg, 0x1A, payload_len);
    /* TLV 0x1A is optional */
    if (hdr) {
        QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x1A_t *tlv;

        CHECK_UNPACK_TLV_SIZE(QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x1A_t);

        hdr++;
        tlv = (QMI_NAS_GET_NR5G_RRC_UTC_TIME_response_0x1A_t *) hdr;
        out->universal_time_available = true;
        COPY_MTOH(out->universal_time.year, tlv->year);
        COPY_MTOH(out->universal_time.month, tlv->month);
        COPY_MTOH(out->universal_time.day, tlv->day);
        COPY_MTOH(out->universal_time.hour, tlv->hour);
        COPY_MTOH(out->universal_time.minute, tlv->minute);
        COPY_MTOH(out->universal_time.second, tlv->second);
        COPY_MTOH(out->universal_time.millisecond, tlv->millisecond);
        COPY_MTOH(out->universal_time.day_of_week, tlv->day_of_week);
    }

end:
    return ret;
}

QMI_SDK_error_t telit_nas_get_nr5g_rrc_utc_time_unpack(uint8_t *rsp, uint16_t len,
        nas_nr5g_rrc_utc_time_t *out)
{
    return telit_nas_generic_nr5g_rrc_utc_time_unpack(rsp, len, out, sizeof(nas_nr5g_rrc_utc_time_t), false);
}

static QMI_SDK_error_t telit_nas_nr5g_rrc_utc_time_update_ind_unpack(uint8_t *rsp, uint16_t len,
        nas_nr5g_rrc_utc_time_t *out, size_t out_len)
{
    return telit_nas_generic_nr5g_rrc_utc_time_unpack(rsp, len, out, out_len, true);
}

/*****************************************************************************
 * QMI_NAS_SWITCH_ANTENNA_LTE (21849 - 0x5559)
 */

static QMI_SDK_error_t telit_nas_switch_antenna_generic_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        nas_antenna_t *input, uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_NAS_SWITCH_ANTENNA_generic_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        return QMI_SDK_ERR_INVALID_ARG;
    }

    req_ctx->msg_id = msg_id;
    req_ctx->svc_id = QMI_SVC_NAS;
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

    tlv_0x01 = (QMI_NAS_SWITCH_ANTENNA_generic_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->antenna, input->antenna);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_nas_switch_antenna_lte_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        nas_antenna_t *input)
{
    return telit_nas_switch_antenna_generic_pack(req_ctx, req, len, input, QMI_NAS_SWITCH_ANTENNA_LTE);
}

QMI_SDK_error_t telit_nas_switch_antenna_lte_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_NAS_SWITCH_ANTENNA_WCDMA (21850 - 0x555a)
 */

QMI_SDK_error_t telit_nas_switch_antenna_wcdma_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        nas_antenna_t *input)
{
    return telit_nas_switch_antenna_generic_pack(req_ctx, req, len, input, QMI_NAS_SWITCH_ANTENNA_WCDMA);
}

QMI_SDK_error_t telit_nas_switch_antenna_wcdma_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI NAS indications
 */

static QMI_SDK_error_t telit_nas_handle_event_ind(uint16_t *payload_len, uint8_t *msg,
        uint8_t *out, uint16_t out_len)
{
    nas_ind_event_report_t *ev_rep = (nas_ind_event_report_t *) out;
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;

    if (out_len < sizeof(*ev_rep)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }
    memset(ev_rep, 0, sizeof(*ev_rep));

    hdr = search_TLV(msg, 0x10, *payload_len);
    if (hdr) {
        QMI_NAS_EVENT_IND_response_0x10_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_NAS_EVENT_IND_response_0x10_t *)hdr;
        ev_rep->signal_strength_set = true;
        COPY_MTOH(ev_rep->signal_strength.strength, tlv->signal_strength_dBm);
        COPY_MTOH(ev_rep->signal_strength.radio_interface, tlv->radio_interface);
    }

    hdr = search_TLV(msg, 0x12, *payload_len);
    if (hdr) {
        QMI_NAS_EVENT_IND_response_0x12_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_NAS_EVENT_IND_response_0x12_t *)hdr;
        ev_rep->registration_reject_set = true;
        COPY_MTOH(ev_rep->registration_reject.domain, tlv->service_domain);
        COPY_MTOH(ev_rep->registration_reject.reject_cause, tlv->reject_cause);
    }

end:
    return ret;
}

QMI_SDK_error_t telit_nas_indication_handler(uint16_t msg_id,
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
        case QMI_NAS_EVENT_REPORT_IND:
            ret = telit_nas_handle_event_ind(&payload_len, msg, out, out_len);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_NAS_NETWORK_TIME_IND:
            if (out_len < sizeof(nas_network_time_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(nas_network_time_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_nas_network_time_ind_unpack(&payload_len, msg,
                    (nas_network_time_t *) out, out_len);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_NAS_SYS_INFO_IND:
            if (out_len < sizeof(nas_system_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(nas_system_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_nas_generic_sys_info_unpack(msg, payload_len, (nas_system_info_t *)out, true);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_NAS_SERVING_SYSTEM_IND:
            if (out_len < sizeof(nas_serving_system_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(nas_serving_system_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_nas_generic_serving_system_unpack(msg, payload_len, (nas_serving_system_info_t *)out,
                    true);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_NAS_SIG_INFO_IND:
            if (out_len < sizeof(nas_signal_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(nas_signal_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_nas_generic_signal_info_unpack(msg, payload_len, (nas_signal_info_t *)out, true);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND:
            ret = telit_nas_nr5g_time_sync_pulse_report_ind_unpack(&payload_len, msg,
                    (nas_nr5g_time_sync_pulse_report_t *) out, out_len);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_NAS_NR5G_LOST_FRAME_SYNC_IND:
            ret = telit_nas_nr5g_lost_frame_sync_ind_unpack(&payload_len, msg,
                    (nas_nr5g_lost_frame_sync_t *) out, out_len);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_NAS_NR5G_RRC_UTC_TIME_UPDATE_IND:
            ret = telit_nas_nr5g_rrc_utc_time_update_ind_unpack(msg, payload_len,
                    (nas_nr5g_rrc_utc_time_t *) out, out_len);
            if (ret != QMI_SDK_ERR_NONE)
                goto end;
            break;
        default:
            RLOGW("%s: Indication not managed (%d)", __FUNCTION__, msg_id);
            break;
    }

end:
    return ret;
}
