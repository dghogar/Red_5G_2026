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
#include "telit_sdk_private_voice.h"
#include "telit_sdk_voice.h"

/*****************************************************************************
 * QMI_VOICE_INDICATION_REGISTER (3 - 0x0003)
 */

QMI_SDK_error_t telit_voice_indication_register_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_indications_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_VOICE_INDICATION_REGISTER_event_t *tlv;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_VOICE_INDICATION_REGISTER;
    req_ctx->svc_id = QMI_SVC_VOICE;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv);

    if (input->reg_dtmf_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->reg_dtmf_events;

        buf_position += tlv_sz;
    }

    if (input->reg_voice_privacy_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->reg_voice_privacy_events;

        buf_position += tlv_sz;
    }

    if (input->supps_notification_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->supps_notification_events;

        buf_position += tlv_sz;
    }

    if (input->call_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->call_events;

        buf_position += tlv_sz;
    }

    if (input->handover_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x14;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->handover_events;

        buf_position += tlv_sz;
    }

    if (input->speech_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x15;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->speech_events;

        buf_position += tlv_sz;
    }

    if (input->ussd_notification_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x16;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->ussd_notification_events;

        buf_position += tlv_sz;
    }

    if (input->sups_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x17;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->sups_events;

        buf_position += tlv_sz;
    }

    if (input->modification_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x18;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->modification_events;

        buf_position += tlv_sz;
    }

    if (input->uus_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x19;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->uus_events;

        buf_position += tlv_sz;
    }

    if (input->aoc_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1A;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->aoc_events;

        buf_position += tlv_sz;
    }

    if (input->conference_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1B;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->conference_events;

        buf_position += tlv_sz;
    }

    if (input->ext_brst_intl_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1C;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->ext_brst_intl_events;

        buf_position += tlv_sz;
    }

    if (input->page_miss_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1D;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->page_miss_events;

        buf_position += tlv_sz;
    }

    if (input->cc_result_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1E;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->cc_result_events;

        buf_position += tlv_sz;
    }

    if (input->conf_participants_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1F;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->conf_participants_events;

        buf_position += tlv_sz;
    }

    if (input->tty_info_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x20;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->tty_info_events;

        buf_position += tlv_sz;
    }

    if (input->orig_fail_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x21;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->orig_fail_events;

        buf_position += tlv_sz;
    }

    if (input->vs_status_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x22;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->vs_status_events;

        buf_position += tlv_sz;
    }

    if (input->audio_rat_change_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x23;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->audio_rat_change_events;

        buf_position += tlv_sz;
    }

    if (input->additional_call_info_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x24;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->additional_call_info_events;

        buf_position += tlv_sz;
    }

    if (input->ecall_status_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x25;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->ecall_status_events;

        buf_position += tlv_sz;
    }

    if (input->call_reestab_status_events != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x26;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->call_reestab_status_events;

        buf_position += tlv_sz;
    }

    if (input->vice_dialog_event != QMI_IND_ACTION_NONE) {
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x27;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv = (QMI_VOICE_INDICATION_REGISTER_event_t *)buf_position;
        tlv->event = input->vice_dialog_event;
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_voice_indication_register_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_VOICE_DIAL_CALL (32 - 0x0020)
 */

QMI_SDK_error_t telit_voice_dial_call_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_call_number_t *input)
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

    req_ctx->msg_id = QMI_VOICE_DIAL_CALL;
    req_ctx->svc_id = QMI_SVC_VOICE;
    req_ctx->timeout = QMI_TIMEOUT_BIG;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = input->calling_number_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    memcpy(buf_position, input->calling_number, input->calling_number_len);

    buf_position += input->calling_number_len;

    if (input->clir_type_available) {
        QMI_VOICE_DIAL_CALL_request_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_VOICE_DIAL_CALL_request_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->clir_type, input->clir_type);
    }

    /* following optional TLVs are missing:
     * Call Type
     * UUS**
     * CUG**
     * Emergency Category
     * Called Party Subaddress
     * Service Type
     * SIP URI Overflow
     * Audio Attribute for VT or VOIP Call
     * Video Attribute for VT or VOIP Call
     * Presentation Indicator for VT or VOIP Call
     * Call Attributes for Videoshare Call
     * eCall Variant
     * Conference URI List
     * Display Text
     * eCall MSD
     * Call Pulled from Secondary Device to Primary
     * Codec Profile
     * Is Secure Call */

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_voice_dial_call_unpack(uint8_t *rsp, uint16_t len,
        voice_call_t *out)
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
        QMI_VOICE_generic_call_id_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_call_id_t);

        hdr++;
        tlv_0x10 = (QMI_VOICE_generic_call_id_t *)hdr;
        COPY_MTOH(out->call_id, tlv_0x10->call_id);
        out->call_id_available = true;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    if (hdr) {
        QMI_VOICE_DIAL_CALL_response_0x14_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_DIAL_CALL_response_0x14_t);

        hdr++;
        tlv_0x14 = (QMI_VOICE_DIAL_CALL_response_0x14_t *)hdr;
        COPY_MTOH(out->end_reason, tlv_0x14->end_reason);
        out->end_reason_available = true;
    }

    /* following optional TLVs are missing:
     * Alpha Identifier
     * Call Control Result Type
     * Call Control Supplementary Service Type
     * Media ID */

end:
    return ret;
}

/*****************************************************************************
 * QMI_VOICE_END_CALL (33 - 0x0021)
 */

QMI_SDK_error_t telit_voice_end_call_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_call_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_VOICE_generic_call_id_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_VOICE_END_CALL;
    req_ctx->svc_id = QMI_SVC_VOICE;
    req_ctx->timeout = QMI_TIMEOUT_BIG;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_VOICE_generic_call_id_t *)buf_position;
    COPY_HTOM(tlv_0x01->call_id, input->call_id);

    buf_position += tlv_sz;

    if (input->end_reason_available) {
        QMI_VOICE_END_CALL_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_VOICE_END_CALL_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->end_reason, input->end_reason);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_voice_end_call_unpack(uint8_t *rsp, uint16_t len,
        voice_call_t *out)
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
        QMI_VOICE_generic_call_id_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_call_id_t);

        hdr++;
        tlv_0x10 = (QMI_VOICE_generic_call_id_t *)hdr;
        COPY_MTOH(out->call_id, tlv_0x10->call_id);
        out->call_id_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_VOICE_ANSWER_CALL (34 - 0x0022)
 */

QMI_SDK_error_t telit_voice_answer_call_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_answer_call_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_VOICE_generic_call_id_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_VOICE_ANSWER_CALL;
    req_ctx->svc_id = QMI_SVC_VOICE;
    req_ctx->timeout = QMI_TIMEOUT_BIG;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_VOICE_generic_call_id_t *)buf_position;
    COPY_HTOM(tlv_0x01->call_id, input->call_id);

    buf_position += tlv_sz;

    if (input->reject_call) {
        QMI_VOICE_ANSWER_CALL_request_0x15_t *tlv_0x15;

        tlv_sz = sizeof(*tlv_0x15);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x15;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x15 = (QMI_VOICE_ANSWER_CALL_request_0x15_t *)buf_position;
        tlv_0x15->reject_call = 0x01;

        buf_position += tlv_sz;
    }

    if (input->reject_cause_available) {
        QMI_VOICE_generic_reject_cause_t *tlv_0x16;

        tlv_sz = sizeof(*tlv_0x16);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x16;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x16 = (QMI_VOICE_generic_reject_cause_t *)buf_position;
        COPY_HTOM(tlv_0x16->reject_cause, input->reject_cause);
    }

    /* following optional TLVs are missing:
     * Call Type
     * Audio Attribute for VT or VOIP Call
     * Video Attribute for VT or VOIP Call
     * Presentation Indicator for VT or VOIP Call
     * File Attributes for Videoshare Call
     * SIP Reject Cause
     * Codec Profile */

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_voice_answer_call_unpack(uint8_t *rsp, uint16_t len,
        voice_call_t *out)
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
        QMI_VOICE_generic_call_id_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_call_id_t);

        hdr++;
        tlv_0x10 = (QMI_VOICE_generic_call_id_t *)hdr;
        COPY_MTOH(out->call_id, tlv_0x10->call_id);
        out->call_id_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_VOICE_BURST_DTMF (40 - 0x0028)
 */

QMI_SDK_error_t telit_voice_burst_dtmf_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_burst_dtmf_input_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_VOICE_BURST_DTMF_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_VOICE_BURST_DTMF;
    req_ctx->svc_id = QMI_SVC_VOICE;
    req_ctx->timeout = QMI_TIMEOUT_MEDIUM;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    if (input->digit_len > QMI_VOICE_MAX_DTMF_BUFFER_SIZE) {
        RLOGE("%s: digit_len %u > QMI_VOICE_MAX_DTMF_BUFFER_SIZE, increase the define",
                __FUNCTION__, input->digit_len);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->digit_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_VOICE_BURST_DTMF_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->call_id, input->call_id);
    COPY_HTOM(tlv_0x01->digit_cnt, input->digit_len);

    buf_position = (uint8_t *)(&(tlv_0x01->digit_cnt));
    /* Moving to the start of the string */
    buf_position += sizeof(tlv_0x01->digit_cnt);
    memcpy(buf_position, input->digit, input->digit_len);

    buf_position += input->digit_len;

    if (input->dtmf_lengths_available) {
        QMI_VOICE_BURST_DTMF_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_VOICE_BURST_DTMF_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->dtmf_onlength, input->dtmf_lengths.dtmf_onlength);
        COPY_HTOM(tlv_0x10->dtmf_offlength, input->dtmf_lengths.dtmf_offlength);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

static QMI_SDK_error_t telit_voice_generic_dtmf_unpack(uint8_t *rsp, uint16_t len,
        voice_dtmf_response_t *out)
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
        QMI_VOICE_generic_call_id_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_call_id_t);

        hdr++;
        tlv_0x10 = (QMI_VOICE_generic_call_id_t *)hdr;
        COPY_MTOH(out->call_id, tlv_0x10->call_id);
        out->call_id_available = true;
    }

end:
    return ret;
}

QMI_SDK_error_t telit_voice_burst_dtmf_unpack(uint8_t *rsp, uint16_t len,
        voice_dtmf_response_t *out)
{
    return telit_voice_generic_dtmf_unpack(rsp, len, out);
}

/*****************************************************************************
 * QMI_VOICE_START_CONT_DTMF (41 - 0x0029)
 */

QMI_SDK_error_t telit_voice_start_cont_dtmf_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_cont_dtmf_input_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_VOICE_START_CONT_DTMF_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_VOICE_START_CONT_DTMF;
    req_ctx->svc_id = QMI_SVC_VOICE;
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

    tlv_0x01 = (QMI_VOICE_START_CONT_DTMF_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->call_id, input->call_id);
    COPY_HTOM(tlv_0x01->digit, input->digit);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_voice_start_cont_dtmf_unpack(uint8_t *rsp, uint16_t len,
        voice_dtmf_response_t *out)
{
    return telit_voice_generic_dtmf_unpack(rsp, len, out);
}

/*****************************************************************************
 * QMI_VOICE_STOP_CONT_DTMF (42 - 0x002A)
 */

QMI_SDK_error_t telit_voice_stop_cont_dtmf_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_cont_dtmf_input_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_VOICE_STOP_CONT_DTMF_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_VOICE_STOP_CONT_DTMF;
    req_ctx->svc_id = QMI_SVC_VOICE;
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

    tlv_0x01 = (QMI_VOICE_STOP_CONT_DTMF_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->call_id, input->call_id);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_voice_stop_cont_dtmf_unpack(uint8_t *rsp, uint16_t len,
        voice_dtmf_response_t *out)
{
    return telit_voice_generic_dtmf_unpack(rsp, len, out);
}

/*****************************************************************************
 * QMI_VOICE_GET_ALL_CALL_INFO (47 - 0x002F)
 */

QMI_SDK_error_t telit_voice_get_all_call_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_VOICE_GET_ALL_CALL_INFO,
            QMI_SVC_VOICE,
            QMI_TIMEOUT_SMALL);
}

static uint8_t voice_parse_call_info(QMI_VOICE_GET_ALL_CALL_INFO_generic_info_t *tlv,
        voice_call_info_t *out)
{
    QMI_VOICE_GET_ALL_CALL_INFO_generic_info_instance_t *instance;
    uint8_t call_info_size;
    uint8_t *skip = NULL;
    uint8_t i;

    COPY_MTOH(call_info_size, tlv->num_of_instances);

    skip = (uint8_t *)(&(tlv->num_of_instances));
    /* Moving to the start of the array */
    skip += sizeof(tlv->num_of_instances);

    instance = (QMI_VOICE_GET_ALL_CALL_INFO_generic_info_instance_t *)skip;

    if (call_info_size > QMI_VOICE_MAX_CALL_INFO) {
        RLOGE("%s: call_info_size = %u > QMI_VOICE_MAX_CALL_INFO, increase the define",
                __FUNCTION__, call_info_size);
        call_info_size = 0;
        goto end;
    }

    for (i = 0; i < call_info_size; i++) {
        COPY_MTOH(out[i].call_id, instance->call_id);
        COPY_MTOH(out[i].call_state, instance->call_state);
        COPY_MTOH(out[i].call_type, instance->call_type);
        COPY_MTOH(out[i].direction, instance->direction);
        COPY_MTOH(out[i].mode, instance->mode);
        COPY_MTOH(out[i].is_mpty, instance->is_mpty);
        COPY_MTOH(out[i].als, instance->als);
        instance++;
    }

end:
    return call_info_size;
}

QMI_SDK_error_t telit_voice_get_all_call_info_unpack(uint8_t *rsp, uint16_t len,
        voice_get_all_call_info_t *out)
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

    /* Unpack optional TLV 0x10 */
    hdr = search_TLV(msg, 0x10, payload_len);
    if (hdr) {
        QMI_VOICE_GET_ALL_CALL_INFO_generic_info_t *tlv;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_GET_ALL_CALL_INFO_generic_info_t);

        hdr++;
        tlv = (QMI_VOICE_GET_ALL_CALL_INFO_generic_info_t *)hdr;
        if (tlv->num_of_instances == 0) {
            goto end;
        }
        out->call_info_size = voice_parse_call_info(tlv, out->call_info);
    }

    /* Unpack optional TLV 0x11 */
    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        QMI_VOICE_GET_ALL_CALL_INFO_response_0x11_t *tlv_0x11;
        QMI_VOICE_array_remote_party_number_t *instance;
        uint8_t i;
        uint8_t *skip = NULL;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_GET_ALL_CALL_INFO_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_VOICE_GET_ALL_CALL_INFO_response_0x11_t *)hdr;
        COPY_MTOH(out->remote_party_number_size, tlv_0x11->num_of_instances);

        if (out->remote_party_number_size > QMI_VOICE_MAX_CALL_INFO) {
            RLOGE("%s: remote_party_number_size = %u > QMI_VOICE_MAX_CALL_INFO, increase the define",
                    __FUNCTION__, out->remote_party_number_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x11->num_of_instances));
        /* Moving to the start of the array */
        skip += sizeof(tlv_0x11->num_of_instances);

        instance = (QMI_VOICE_array_remote_party_number_t *)skip;

        for (i = 0; i < out->remote_party_number_size; i++) {
            uint8_t *ptr = NULL;

            COPY_MTOH(out->remote_party_number[i].call_id, instance->call_id);
            COPY_MTOH(out->remote_party_number[i].number_pi, instance->number_pi);
            COPY_MTOH(out->remote_party_number[i].number_len, instance->number_len);
            ptr = (uint8_t *)(&(instance->number_len));
            ptr += sizeof(instance->number_len);
            memcpy(out->remote_party_number[i].number, ptr, instance->number_len);
            instance++;
        }
    }

    /* Unpack optional TLV 0x12 */
    hdr = search_TLV(msg, 0x12, payload_len);
    if (hdr) {
        QMI_VOICE_GET_ALL_CALL_INFO_response_0x12_t *tlv_0x12;
        QMI_VOICE_array_remote_party_name_t *instance;
        uint8_t i;
        uint8_t *skip = NULL;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_GET_ALL_CALL_INFO_response_0x12_t);

        hdr++;
        tlv_0x12 = (QMI_VOICE_GET_ALL_CALL_INFO_response_0x12_t *)hdr;
        COPY_MTOH(out->remote_party_name_size, tlv_0x12->num_of_instances);

        if (out->remote_party_name_size > QMI_VOICE_MAX_CALL_INFO) {
            RLOGE("%s: remote_party_name_size = %u > QMI_VOICE_MAX_CALL_INFO, increase the define",
                    __FUNCTION__, out->remote_party_name_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x12->num_of_instances));
        skip += sizeof(tlv_0x12->num_of_instances);

        instance = (QMI_VOICE_array_remote_party_name_t *)skip;

        for (i = 0; i < out->remote_party_name_size; i++) {
            uint8_t *ptr = NULL;

            COPY_MTOH(out->remote_party_name[i].call_id, instance->call_id);
            COPY_MTOH(out->remote_party_name[i].name_pi, instance->name_pi);
            COPY_MTOH(out->remote_party_name[i].coding, instance->coding);
            COPY_MTOH(out->remote_party_name[i].name_len, instance->name_len);
            ptr = (uint8_t *)(&(instance->name_len));
            ptr += sizeof(instance->name_len);
            memcpy(out->remote_party_name[i].name, ptr, instance->name_len);
            instance++;
        }
    }

    /* Unpack optional TLV 0x14 */
    hdr = search_TLV(msg, 0x14, payload_len);
    if (hdr) {
        QMI_VOICE_GET_ALL_CALL_INFO_response_0x14_t *tlv_0x14;
        QMI_VOICE_array_uus_info_t *instance;
        uint8_t i;
        uint8_t *skip = NULL;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_GET_ALL_CALL_INFO_response_0x14_t);

        hdr++;
        tlv_0x14 = (QMI_VOICE_GET_ALL_CALL_INFO_response_0x14_t *)hdr;
        COPY_MTOH(out->array_uus_size, tlv_0x14->num_of_instances);

        if (out->array_uus_size > QMI_VOICE_MAX_CALL_INFO) {
            RLOGE("%s: array_uus_size = %u > QMI_VOICE_MAX_CALL_INFO, increase the define",
                    __FUNCTION__, out->array_uus_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x14->num_of_instances));
        skip += sizeof(tlv_0x14->num_of_instances);

        instance = (QMI_VOICE_array_uus_info_t *)skip;

        for (i = 0; i < out->array_uus_size; i++) {
            uint8_t *ptr = NULL;

            COPY_MTOH(out->array_uus[i].call_id, instance->call_id);
            COPY_MTOH(out->array_uus[i].uus_type, instance->uus_type);
            COPY_MTOH(out->array_uus[i].uus_dcs, instance->uus_dcs);
            COPY_MTOH(out->array_uus[i].uus_data_len, instance->uus_data_len);
            ptr = (uint8_t *)(&(instance->uus_data_len));
            ptr += sizeof(instance->uus_data_len);
            memcpy(out->array_uus[i].uus_data, ptr, instance->uus_data_len);
            instance++;
        }
    }

    /* Unpack optional TLV 0x1A */
    hdr = search_TLV(msg, 0x1A, payload_len);
    if (hdr) {
        QMI_VOICE_GET_ALL_CALL_INFO_response_0x1A_t *tlv_0x1A;
        QMI_VOICE_array_conn_party_number_t *instance;
        uint8_t i;
        uint8_t *skip = NULL;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_GET_ALL_CALL_INFO_response_0x1A_t);

        hdr++;
        tlv_0x1A = (QMI_VOICE_GET_ALL_CALL_INFO_response_0x1A_t *)hdr;
        COPY_MTOH(out->conn_party_number_size, tlv_0x1A->num_of_instances);

        if (out->conn_party_number_size > QMI_VOICE_MAX_CALL_INFO) {
            RLOGE("%s: conn_party_number_size = %u > QMI_VOICE_MAX_CALL_INFO, increase the define",
                    __FUNCTION__, out->conn_party_number_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x1A->num_of_instances));
        skip += sizeof(tlv_0x1A->num_of_instances);

        instance = (QMI_VOICE_array_conn_party_number_t *)skip;

        for (i = 0; i < out->conn_party_number_size; i++) {
            uint8_t *ptr = NULL;

            COPY_MTOH(out->conn_party_number[i].call_id, instance->call_id);
            COPY_MTOH(out->conn_party_number[i].conn_num_pi, instance->conn_num_pi);
            COPY_MTOH(out->conn_party_number[i].conn_num_si, instance->conn_num_si);
            COPY_MTOH(out->conn_party_number[i].conn_num_type, instance->conn_num_type);
            COPY_MTOH(out->conn_party_number[i].conn_num_plan, instance->conn_num_plan);
            COPY_MTOH(out->conn_party_number[i].conn_num_len, instance->conn_num_len);
            ptr = (uint8_t *)(&(instance->conn_num_len));
            ptr += sizeof(instance->conn_num_len);
            memcpy(out->conn_party_number[i].conn_num, ptr, instance->conn_num_len);
            instance++;
        }
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_VOICE_MANAGE_CALLS (49 - 0x0031)
 */

QMI_SDK_error_t telit_voice_manage_calls_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_manage_call_request_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_VOICE_MANAGE_CALLS_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_VOICE_MANAGE_CALLS;
    req_ctx->svc_id = QMI_SVC_VOICE;
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

    tlv_0x01 = (QMI_VOICE_MANAGE_CALLS_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->sups_type, input->sups_type);

    buf_position += tlv_sz;

    if (input->call_id_available) {
        QMI_VOICE_generic_call_id_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_VOICE_generic_call_id_t *)buf_position;
        COPY_HTOM(tlv_0x10->call_id, input->call_id);

        buf_position += tlv_sz;
    }

    if (input->reject_cause_available) {
        QMI_VOICE_generic_reject_cause_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_VOICE_generic_reject_cause_t *)buf_position;
        COPY_HTOM(tlv_0x11->reject_cause, input->reject_cause);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_voice_manage_calls_unpack(uint8_t *rsp, uint16_t len,
        voice_manage_call_response_t *out)
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
        QMI_VOICE_generic_failure_cause_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_failure_cause_t);

        hdr++;
        tlv_0x10 = (QMI_VOICE_generic_failure_cause_t *)hdr;
        COPY_MTOH(out->failure_cause, tlv_0x10->failure_cause);
        out->failure_cause_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_VOICE_SET_SUPS_SERVICE (51 - 0x0033)
 */

QMI_SDK_error_t telit_voice_set_sups_service_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_set_sup_service_input_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_VOICE_SET_SUPS_SERVICE_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_VOICE_SET_SUPS_SERVICE;
    req_ctx->svc_id = QMI_SVC_VOICE;
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

    tlv_0x01 = (QMI_VOICE_SET_SUPS_SERVICE_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->voice_service, input->voice_service);
    COPY_HTOM(tlv_0x01->reason, input->reason);

    buf_position += tlv_sz;

    if (input->service_class_available) {
        QMI_VOICE_generic_service_class_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_VOICE_generic_service_class_t *)buf_position;
        COPY_HTOM(tlv_0x10->service_class, input->service_class);

        buf_position += tlv_sz;
    }

    if (input->password_len) {
        tlv_sz = input->password_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        memcpy(buf_position, input->password, tlv_sz);

        buf_position += tlv_sz;
    }

    if (input->number_len) {
        tlv_sz = input->number_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        memcpy(buf_position, input->number, tlv_sz);

        buf_position += tlv_sz;
    }

    if (input->timer_value_available) {
        QMI_VOICE_SET_SUPS_SERVICE_request_0x13_t *tlv_0x13;

        tlv_sz = sizeof(*tlv_0x13);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x13 = (QMI_VOICE_SET_SUPS_SERVICE_request_0x13_t *)buf_position;
        COPY_HTOM(tlv_0x13->timer_value, input->timer_value);

        buf_position += tlv_sz;
    }

    if (input->num_type_and_plan_available) {
        QMI_VOICE_SET_SUPS_SERVICE_request_0x14_t *tlv_0x14;

        tlv_sz = sizeof(*tlv_0x14);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x14;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x14 = (QMI_VOICE_SET_SUPS_SERVICE_request_0x14_t *)buf_position;
        COPY_HTOM(tlv_0x14->num_type, input->num_type_and_plan.num_type);
        COPY_HTOM(tlv_0x14->num_plan, input->num_type_and_plan.num_plan);

        buf_position += tlv_sz;
    }

    if (input->service_class_ext_available) {
        QMI_VOICE_generic_service_class_ext_t *tlv_0x15;

        tlv_sz = sizeof(*tlv_0x15);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x15;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x15 = (QMI_VOICE_generic_service_class_ext_t *)buf_position;
        COPY_HTOM(tlv_0x15->service_class_ext, input->service_class_ext);

        buf_position += tlv_sz;
    }

    if (input->call_barring_numbers_list_size > 0) {
        QMI_VOICE_SET_SUPS_SERVICE_request_0x16_t *tlv_0x16;
        uint8_t i = 0;

        tlv_sz = sizeof(*tlv_0x16);

        if (input->call_barring_numbers_list_size > QMI_VOICE_BARRING_NUMBERS_LIST_MAX_SIZE) {
            RLOGE(
                    "%s: call_barring_numbers_list_size = %u > QMI_VOICE_BARRING_NUMBERS_LIST_MAX_SIZE, increase the define",
                    __FUNCTION__, input->call_barring_numbers_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        for (i = 0; i < input->call_barring_numbers_list_size; i++) {
            tlv_sz += sizeof(QMI_VOICE_call_barring_generic_number_t);

            if (input->call_barring_numbers_list[i].number_len !=
                    strlen(input->call_barring_numbers_list[i].number)) {
                RLOGW("%s: string \"%s\", actual length: %zu is different from len variable %d",
                        __FUNCTION__, input->call_barring_numbers_list[i].number,
                        strlen(input->call_barring_numbers_list[i].number),
                        input->call_barring_numbers_list[i].number_len);
            }

            tlv_sz += input->call_barring_numbers_list[i].number_len;
        }

        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x16;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x16 = (QMI_VOICE_SET_SUPS_SERVICE_request_0x16_t *)buf_position;
        COPY_HTOM(tlv_0x16->num_of_instances, input->call_barring_numbers_list_size);

        buf_position += sizeof(tlv_0x16->num_of_instances);

        for (i = 0; i < input->call_barring_numbers_list_size; i++) {
            *buf_position = input->call_barring_numbers_list[i].number_len;
            buf_position += sizeof(input->call_barring_numbers_list[i].number_len);
            memcpy(buf_position, input->call_barring_numbers_list[i].number,
                    input->call_barring_numbers_list[i].number_len);

            buf_position += input->call_barring_numbers_list[i].number_len;
        }
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_voice_set_sups_service_unpack(uint8_t *rsp, uint16_t len,
        voice_set_sup_service_response_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len = 0;
    uint8_t *msg = NULL;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        /* We do not want to bail-out in order to get the failure cause TLV */
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    if (hdr) {
        QMI_VOICE_generic_failure_cause_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_failure_cause_t);

        hdr++;
        tlv_0x10 = (QMI_VOICE_generic_failure_cause_t *)hdr;
        COPY_MTOH(out->failure_cause, tlv_0x10->failure_cause);
        out->failure_cause_available = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    if (hdr) {
        QMI_VOICE_generic_call_id_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_call_id_t);

        hdr++;
        tlv_0x13 = (QMI_VOICE_generic_call_id_t *)hdr;
        COPY_MTOH(out->call_id, tlv_0x13->call_id);
        out->call_id_available = true;
    }

    /* following optional TLVs are missing:
     * Alpha Identifier
     * Call Control Result Type
     * Call Control Supplementary Service Type
     * Service Status
     * Failure Cause Description
     * Retry Duration */

end:
    return ret;
}

/*****************************************************************************
 * QMI_VOICE_GET_CALL_WAITING (52 - 0x0034)
 */

static QMI_SDK_error_t telit_voice_generic_get_service_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_get_call_service_input_t *input,
        uint16_t msg_id)
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

    req_ctx->msg_id = msg_id;
    req_ctx->svc_id = QMI_SVC_VOICE;
    req_ctx->timeout = 60;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (msg_id != QMI_VOICE_GET_CALL_WAITING) {
        QMI_VOICE_generic_service_reason_t *tlv_0x01;

        tlv_sz = sizeof(*tlv_0x01);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x01;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x01 = (QMI_VOICE_generic_service_reason_t *)buf_position;
        COPY_HTOM(tlv_0x01->reason, input->reason);

        buf_position += tlv_sz;
    }

    if (input->service_class_available) {
        QMI_VOICE_generic_service_class_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_VOICE_generic_service_class_t *)buf_position;
        COPY_HTOM(tlv_0x10->service_class, input->service_class);

        buf_position += tlv_sz;
    }

    if (input->service_class_ext_available) {
        QMI_VOICE_generic_service_class_ext_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_VOICE_generic_service_class_ext_t *)buf_position;
        COPY_HTOM(tlv_0x11->service_class_ext, input->service_class_ext);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

    /* This is for when QMI_VOICE_GET_CALL_WAITING has no optional TLV */
    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

QMI_SDK_error_t telit_voice_get_call_waiting_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_get_call_service_input_t *input)
{
    return telit_voice_generic_get_service_pack(req_ctx, req, len, input, QMI_VOICE_GET_CALL_WAITING);
}

QMI_SDK_error_t telit_voice_get_call_waiting_unpack(uint8_t *rsp, uint16_t len,
        voice_get_call_waiting_response_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len = 0;
    uint8_t *msg = NULL;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        /* We do not want to bail-out in order to get the failure cause TLV */
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    if (hdr) {
        QMI_VOICE_generic_service_class_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_service_class_t);

        hdr++;
        tlv_0x10 = (QMI_VOICE_generic_service_class_t *)hdr;
        COPY_MTOH(out->service_class, tlv_0x10->service_class);
        out->service_class_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        QMI_VOICE_generic_failure_cause_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_failure_cause_t);

        hdr++;
        tlv_0x11 = (QMI_VOICE_generic_failure_cause_t *)hdr;
        COPY_MTOH(out->failure_cause, tlv_0x11->failure_cause);
        out->failure_cause_available = true;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    if (hdr) {
        QMI_VOICE_generic_call_id_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_call_id_t);

        hdr++;
        tlv_0x14 = (QMI_VOICE_generic_call_id_t *)hdr;
        COPY_MTOH(out->call_id, tlv_0x14->call_id);
        out->call_id_available = true;
    }

    /* following optional TLVs are missing:
     * Alpha Identifier
     * Call Control Result Type
     * Call Control Supplementary Service Type
     * Retry Duration */

end:
    return ret;
}

/*****************************************************************************
 * QMI_VOICE_GET_CALL_BARRING (53 - 0x0035)
 */

QMI_SDK_error_t telit_voice_get_call_barring_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_get_call_service_input_t *input)
{
    return telit_voice_generic_get_service_pack(req_ctx, req, len, input, QMI_VOICE_GET_CALL_BARRING);
}

QMI_SDK_error_t telit_voice_get_call_barring_unpack(uint8_t *rsp, uint16_t len,
        voice_call_barring_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len = 0;
    uint8_t *msg = NULL;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        /* We do not want to bail-out in order to get the failure cause TLV */
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    if (hdr) {
        QMI_VOICE_generic_service_class_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_service_class_t);

        hdr++;
        tlv_0x10 = (QMI_VOICE_generic_service_class_t *)hdr;
        COPY_MTOH(out->service_class, tlv_0x10->service_class);
        out->service_class_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        QMI_VOICE_generic_failure_cause_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_failure_cause_t);

        hdr++;
        tlv_0x11 = (QMI_VOICE_generic_failure_cause_t *)hdr;
        COPY_MTOH(out->failure_cause, tlv_0x11->failure_cause);
        out->failure_cause_available = true;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    if (hdr) {
        QMI_VOICE_generic_call_id_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_call_id_t);

        hdr++;
        tlv_0x14 = (QMI_VOICE_generic_call_id_t *)hdr;
        COPY_MTOH(out->call_id, tlv_0x14->call_id);
        out->call_id_available = true;
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    if (hdr) {
        QMI_VOICE_GET_CALL_BARRING_response_0x17_t *tlv_0x17;
        QMI_VOICE_call_barring_service_instance_t *service_instance;
        uint8_t *skip = NULL;
        uint8_t i;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_GET_CALL_BARRING_response_0x17_t);

        hdr++;
        tlv_0x17 = (QMI_VOICE_GET_CALL_BARRING_response_0x17_t *)hdr;
        COPY_MTOH(out->call_barring_service_size, tlv_0x17->call_barring_service_size);

        if (out->call_barring_service_size > QMI_VOICE_MAX_CALL_BARRING_SERVICE) {
            RLOGE(
                    "%s: call_barring_service_size = %u > QMI_VOICE_MAX_CALL_BARRING_SERVICE, increase the define",
                    __FUNCTION__, out->call_barring_service_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x17->call_barring_service_size));
        /* Moving to the start of the array */
        skip += sizeof(tlv_0x17->call_barring_service_size);

        for (i = 0; i < out->call_barring_service_size; i++) {
            uint8_t j;
            QMI_VOICE_call_barring_number_instance_t *number_instance;

            service_instance = (QMI_VOICE_call_barring_service_instance_t *)skip;

            COPY_MTOH(out->call_barring_service[i].service_class_ext, service_instance->service_class_ext);
            COPY_MTOH(out->call_barring_service[i].call_barring_number_list_size,
                    service_instance->service_class_ext);

            skip += sizeof(*service_instance);

            for (j = 0; j < out->call_barring_service[i].call_barring_number_list_size; j++) {
                number_instance = (QMI_VOICE_call_barring_number_instance_t *)skip;

                COPY_MTOH(out->call_barring_service[i].call_barring_number_list[j].active_status,
                        number_instance->active_status);
                COPY_MTOH(out->call_barring_service[i].call_barring_number_list[j].number_len,
                        number_instance->number_len);
                skip = (uint8_t *)(&(number_instance->number_len));
                skip += sizeof(number_instance->number_len);
                memcpy(out->call_barring_service[i].call_barring_number_list[j].number, skip,
                        out->call_barring_service[i].call_barring_number_list[j].number_len);
                skip += out->call_barring_service[i].call_barring_number_list[j].number_len;
            }
        }
    }

    /* following optional TLVs are missing:
     * Alpha Identifier
     * Call Control Result Type
     * Call Control Supplementary Service Type
     * Retry Duration
     * SIP error code */

end:
    return ret;
}

/*****************************************************************************
 * QMI_VOICE_GET_CLIP (54 - 0x0036)
 */

QMI_SDK_error_t telit_voice_get_clip_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_VOICE_GET_CLIP,
            QMI_SVC_VOICE,
            QMI_TIMEOUT_BIG);
}

static QMI_SDK_error_t telit_voice_generic_get_cl_unpack(uint8_t *rsp, uint16_t len,
        voice_get_calling_line_response_t *out)
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
        QMI_VOICE_generic_provision_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_provision_t);

        hdr++;
        tlv_0x10 = (QMI_VOICE_generic_provision_t *)hdr;
        COPY_MTOH(out->cl_response.active_status, tlv_0x10->active_status);
        COPY_MTOH(out->cl_response.provision_status, tlv_0x10->provision_status);
        out->cl_response_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        QMI_VOICE_generic_failure_cause_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_failure_cause_t);

        hdr++;
        tlv_0x11 = (QMI_VOICE_generic_failure_cause_t *)hdr;
        COPY_MTOH(out->failure_cause, tlv_0x11->failure_cause);
        out->failure_cause_available = true;
    }

end:
    return ret;
}

QMI_SDK_error_t telit_voice_get_clip_unpack(uint8_t *rsp, uint16_t len,
        voice_get_calling_line_response_t *out)
{
    return telit_voice_generic_get_cl_unpack(rsp, len, out);
}

/*****************************************************************************
 * QMI_VOICE_GET_CLIR (55 - 0x0037)
 */

QMI_SDK_error_t telit_voice_get_clir_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_VOICE_GET_CLIR,
            QMI_SVC_VOICE,
            QMI_TIMEOUT_BIG);
}

QMI_SDK_error_t telit_voice_get_clir_unpack(uint8_t *rsp, uint16_t len,
        voice_get_calling_line_response_t *out)
{
    return telit_voice_generic_get_cl_unpack(rsp, len, out);
}

/*****************************************************************************
 * QMI_VOICE_GET_CALL_FORWARDING (56 - 0x0038)
 */

QMI_SDK_error_t telit_voice_get_call_forwarding_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_get_call_service_input_t *input)
{
    return telit_voice_generic_get_service_pack(req_ctx, req, len, input, QMI_VOICE_GET_CALL_FORWARDING);
}

QMI_SDK_error_t telit_voice_get_call_forwarding_unpack(uint8_t *rsp, uint16_t len,
        voice_call_forwarding_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_VOICE_call_forwarding_info_instance_t *instance;
    uint8_t *skip = NULL;
    uint8_t i = 0;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    if (hdr) {
        QMI_VOICE_GET_CALL_FORWARDING_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_GET_CALL_FORWARDING_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_VOICE_GET_CALL_FORWARDING_response_0x10_t *)hdr;
        COPY_MTOH(out->call_forwarding_info_size, tlv_0x10->num_of_instances);

        if (out->call_forwarding_info_size > QMI_VOICE_MAX_CALL_FW_INFO) {
            RLOGE("%s: call_forwarding_info_size = %u > QMI_VOICE_MAX_CALL_FW_INFO, increase the define",
                    __FUNCTION__, out->call_forwarding_info_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x10->num_of_instances));
        /* Moving to the start of the array */
        skip += sizeof(tlv_0x10->num_of_instances);

        instance = (QMI_VOICE_call_forwarding_info_instance_t *)skip;

        for (i = 0; i < out->call_forwarding_info_size; i++) {
            COPY_MTOH(out->call_forwarding_info[i].service_status, instance->service_status);
            COPY_MTOH(out->call_forwarding_info[i].service_class, instance->service_class);
            COPY_MTOH(out->call_forwarding_info[i].number_len, instance->number_len);

            skip = (uint8_t *)(&(instance->number_len));
            skip += sizeof(instance->number_len);

            memcpy(out->call_forwarding_info[i].number, skip, instance->number_len);

            skip += instance->number_len;
            ec_mtoh(&(out->call_forwarding_info[i].no_reply_timer),
                    sizeof(out->call_forwarding_info[i].no_reply_timer),
                    skip, sizeof(*skip));

            skip += sizeof(out->call_forwarding_info[i].no_reply_timer);
            instance = (QMI_VOICE_call_forwarding_info_instance_t *)skip;
        }
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        QMI_VOICE_generic_failure_cause_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_failure_cause_t);

        hdr++;
        tlv_0x11 = (QMI_VOICE_generic_failure_cause_t *)hdr;
        COPY_MTOH(out->failure_cause, tlv_0x11->failure_cause);
        out->failure_cause_available = true;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    if (hdr) {
        QMI_VOICE_generic_call_id_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_call_id_t);

        hdr++;
        tlv_0x14 = (QMI_VOICE_generic_call_id_t *)hdr;
        COPY_MTOH(out->call_id, tlv_0x14->call_id);
        out->call_id_available = true;
    }

    /* following optional TLVs are missing:
     * Alpha Identifier
     * Call Control Result Type
     * Call Control Supplementary Service Type
     * Get Call Forwarding Extended Info
     * Get Call Forwarding Extended Info 2
     * Retry Duration
     * Provision Status
     * Call Forwarding Start Time
     * Call Forwarding End Time */

end:
    return ret;
}

/*****************************************************************************
 * QMI_VOICE_SET_CALL_BARRING_PASSWORD (57 - 0x0039)
 */

QMI_SDK_error_t telit_voice_set_call_barring_password_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        voice_call_barring_input_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_VOICE_SET_CALL_BARRING_PASSWORD_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_VOICE_SET_CALL_BARRING_PASSWORD;
    req_ctx->svc_id = QMI_SVC_VOICE;
    req_ctx->timeout = QMI_TIMEOUT_BIG;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_VOICE_SET_CALL_BARRING_PASSWORD_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->reason, input->reason);
    memcpy(tlv_0x01->old_password, input->old_password, CALL_BARRING_PASSWORD_LEN);
    memcpy(tlv_0x01->new_password, input->new_password, CALL_BARRING_PASSWORD_LEN);
    memcpy(tlv_0x01->new_password_again, input->new_password, CALL_BARRING_PASSWORD_LEN);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_voice_set_call_barring_password_unpack(uint8_t *rsp, uint16_t len,
        voice_call_barring_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len = 0;
    uint8_t *msg = NULL;
    QMI_raw_content_header_t *hdr = NULL;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        /* We do not want to bail-out in order to get the failure cause TLV */
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    if (hdr) {
        QMI_VOICE_generic_failure_cause_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_failure_cause_t);

        hdr++;
        tlv_0x10 = (QMI_VOICE_generic_failure_cause_t *)hdr;
        COPY_MTOH(out->failure_cause, tlv_0x10->failure_cause);
        out->failure_cause_available = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    if (hdr) {
        QMI_VOICE_generic_call_id_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_call_id_t);

        hdr++;
        tlv_0x13 = (QMI_VOICE_generic_call_id_t *)hdr;
        COPY_MTOH(out->call_id, tlv_0x13->call_id);
        out->call_id_available = true;
    }

    /* following optional TLVs are missing:
     * Alpha Identifier
     * Call Control Supplementary Service Type
     * Retry Duration
     * SIP error code */

end:
    return ret;
}

/*****************************************************************************
 * QMI VOICE indications
 */

static QMI_SDK_error_t telit_voice_all_call_status_ind_unpack(uint8_t *rsp, uint16_t len,
        voice_all_call_status_ind_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_VOICE_GET_ALL_CALL_INFO_generic_info_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(rsp, 0x01, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_VOICE_GET_ALL_CALL_INFO_generic_info_t);

    hdr++;
    tlv_0x01 = (QMI_VOICE_GET_ALL_CALL_INFO_generic_info_t *)hdr;

    out->call_info_size = voice_parse_call_info(tlv_0x01, out->call_info);
    if (out->call_info_size == 0) {
        ret = QMI_SDK_ERR_INTERNAL;
    }

end:
    return ret;
}

static QMI_SDK_error_t telit_voice_conference_info_ind_unpack(uint8_t *rsp, uint16_t len,
        voice_conference_info_ind_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_VOICE_CONFERENCE_INFO_IND_0x01_t *tlv_0x01;
    QMI_VOICE_CONFERENCE_INFO_IND_0x02_t *tlv_0x02;
    uint8_t *skip = NULL;

    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(rsp, 0x01, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_VOICE_CONFERENCE_INFO_IND_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_VOICE_CONFERENCE_INFO_IND_0x01_t *)hdr;
    COPY_MTOH(out->conference_xml_len, tlv_0x01->conference_xml_len);

    skip = (uint8_t *)(&(tlv_0x01->conference_xml_len));
    skip += sizeof(tlv_0x01->conference_xml_len);

    memcpy(out->conference_xml, skip, out->conference_xml_len);

    FIND_MANDATORY_TLV(rsp, 0x02, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_VOICE_CONFERENCE_INFO_IND_0x02_t);

    hdr++;
    tlv_0x02 = (QMI_VOICE_CONFERENCE_INFO_IND_0x02_t *)hdr;
    COPY_MTOH(out->sequence, tlv_0x02->sequence);

    hdr = search_TLV(rsp, 0x10, len);
    if (hdr) {
        QMI_VOICE_CONFERENCE_INFO_IND_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_CONFERENCE_INFO_IND_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_VOICE_CONFERENCE_INFO_IND_0x10_t *)hdr;
        COPY_MTOH(out->total_size, tlv_0x10->total_size);
        out->total_size_available = true;
    }

    hdr = search_TLV(rsp, 0x11, len);
    if (hdr) {
        QMI_VOICE_generic_call_id_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_call_id_t);

        hdr++;
        tlv_0x11 = (QMI_VOICE_generic_call_id_t *)hdr;
        COPY_MTOH(out->call_id, tlv_0x11->call_id);
        out->call_id_available = true;
    }

end:
    return ret;
}

static QMI_SDK_error_t telit_voice_conference_join_ind_unpack(uint8_t *rsp, uint16_t len,
        voice_conference_join_info_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_VOICE_generic_call_id_t *tlv_0x01;
    QMI_VOICE_generic_partecipant_info_t *tlv_0x02;
    uint8_t *skip = NULL;

    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(rsp, 0x01, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_call_id_t);

    hdr++;
    tlv_0x01 = (QMI_VOICE_generic_call_id_t *)hdr;
    COPY_MTOH(out->call_id, tlv_0x01->call_id);

    FIND_MANDATORY_TLV(rsp, 0x02, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_partecipant_info_t);

    hdr++;
    tlv_0x02 = (QMI_VOICE_generic_partecipant_info_t *)hdr;
    COPY_MTOH(out->partecipant_info.uri_name_len, tlv_0x02->uri_name_len);

    skip = (uint8_t *)(&(tlv_0x02->uri_name_len));
    skip += sizeof(tlv_0x02->uri_name_len);
    /* URI are made of UTF-16 chars */
    memcpy(out->partecipant_info.uri_name, skip,
            out->partecipant_info.uri_name_len * sizeof(uint16_t));
    skip += out->partecipant_info.uri_name_len * sizeof(uint16_t);

    ec_mtoh(&(out->partecipant_info.uri_description_len),
            sizeof(out->partecipant_info.uri_description_len),
            skip, sizeof(*skip));
    skip += sizeof(out->partecipant_info.uri_description_len);
    memcpy(out->partecipant_info.uri_description, skip,
            out->partecipant_info.uri_description_len * sizeof(uint16_t));

end:
    return ret;
}

static QMI_SDK_error_t telit_voice_conference_partecipant_update_ind_unpack(uint8_t *rsp, uint16_t len,
        voice_partecipant_info_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_VOICE_generic_partecipant_info_t *tlv_0x01;
    uint8_t *skip = NULL;

    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(rsp, 0x02, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_partecipant_info_t);

    hdr++;
    tlv_0x01 = (QMI_VOICE_generic_partecipant_info_t *)hdr;
    COPY_MTOH(out->uri_name_len, tlv_0x01->uri_name_len);

    skip = (uint8_t *)(&(tlv_0x01->uri_name_len));
    skip += sizeof(tlv_0x01->uri_name_len);
    /* URI are made of UTF-16 chars */
    memcpy(out->uri_name, skip,
            out->uri_name_len * sizeof(uint16_t));
    skip += out->uri_name_len * sizeof(uint16_t);

    ec_mtoh(&(out->uri_description_len),
            sizeof(out->uri_description_len),
            skip, sizeof(*skip));
    skip += sizeof(out->uri_description_len);
    memcpy(out->uri_description, skip,
            out->uri_description_len * sizeof(uint16_t));

end:
    return ret;
}

static QMI_SDK_error_t telit_voice_sups_ind_unpack(uint8_t *rsp, uint16_t len,
        voice_supplementary_service_info_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_VOICE_SUPS_IND_0x01_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(rsp, 0x01, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_VOICE_SUPS_IND_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_VOICE_SUPS_IND_0x01_t *)hdr;
    COPY_MTOH(out->service_type, tlv_0x01->service_type);
    COPY_MTOH(out->is_modified_by_call_control, tlv_0x01->is_modified_by_call_control);

    hdr = search_TLV(rsp, 0x15, len);
    if (hdr) {
        QMI_VOICE_generic_call_id_t *tlv_0x15;

        CHECK_UNPACK_TLV_SIZE(QMI_VOICE_generic_call_id_t);

        hdr++;
        tlv_0x15 = (QMI_VOICE_generic_call_id_t *)hdr;
        COPY_MTOH(out->call_id, tlv_0x15->call_id);
        out->call_id_available = true;
    }

end:
    return ret;
}

static QMI_SDK_error_t telit_voice_sups_notification_ind_unpack(uint8_t *rsp, uint16_t len,
        voice_sups_notification_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_VOICE_SUPS_NOTIFICATION_IND_0x01_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(rsp, 0x01, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_VOICE_SUPS_NOTIFICATION_IND_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_VOICE_SUPS_NOTIFICATION_IND_0x01_t *)hdr;
    COPY_MTOH(out->call_id, tlv_0x01->call_id);
    COPY_MTOH(out->notification_type, tlv_0x01->notification_type);

end:
    return ret;
}

QMI_SDK_error_t telit_voice_indication_handler(uint16_t msg_id,
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
        case QMI_VOICE_ALL_CALL_STATUS_IND:
            if (out_len < sizeof(voice_all_call_status_ind_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(voice_all_call_status_ind_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_voice_all_call_status_ind_unpack(msg, payload_len,
                    (voice_all_call_status_ind_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_VOICE_CONFERENCE_INFO_IND:
            if (out_len < sizeof(voice_conference_info_ind_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(voice_conference_info_ind_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_voice_conference_info_ind_unpack(msg, payload_len,
                    (voice_conference_info_ind_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_VOICE_CONFERENCE_JOIN_IND:
            if (out_len < sizeof(voice_conference_join_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(voice_conference_join_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_voice_conference_join_ind_unpack(msg, payload_len,
                    (voice_conference_join_info_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_VOICE_CONFERENCE_PARTICIPANT_UPDATE_IND:
            if (out_len < sizeof(voice_partecipant_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(voice_partecipant_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_voice_conference_partecipant_update_ind_unpack(msg, payload_len,
                    (voice_partecipant_info_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_VOICE_SUPS_IND:
            if (out_len < sizeof(voice_supplementary_service_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(voice_supplementary_service_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_voice_sups_ind_unpack(msg, payload_len,
                    (voice_supplementary_service_info_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_VOICE_SUPS_NOTIFICATION_IND:
            if (out_len < sizeof(voice_sups_notification_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(voice_sups_notification_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_voice_sups_notification_ind_unpack(msg, payload_len,
                    (voice_sups_notification_t *)out);
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
