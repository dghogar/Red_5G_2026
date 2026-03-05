#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined __WINDOWS_QMI__
#include "portable_endian.h"
#else
#include <endian.h>
#endif

#include "telit_sdk.h"
#include "telit_sdk_private.h"
#include "telit_sdk_private_cat.h"
#include "telit_sdk_cat.h"

/*****************************************************************************
 * QMI_CAT_SET_EVENT_REPORT (1 - 0x0001)
 */

QMI_SDK_error_t telit_cat_set_event_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        cat_set_event_report_masks_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_CAT_SET_EVENT_REPORT;
    req_ctx->svc_id = QMI_SVC_CAT;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    if (input->pc_evt_report_req_mask_available) {
        QMI_CAT_generic_pc_evt_mask_t *tlv_0x10;
        uint32_t hdr_sz = sizeof(*hdr);
        uint16_t tlv_sz = sizeof(*tlv_0x10);

        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_CAT_generic_pc_evt_mask_t *)buf_position;
        COPY_HTOM(tlv_0x10->pc_evt_report_req_mask, input->pc_evt_report_req_mask);

        buf_position += tlv_sz;
    }

    if (input->pc_full_func_evt_report_req_mask_available) {
        QMI_CAT_pc_full_func_evt_mask_t *tlv_0x13;
        uint32_t hdr_sz = sizeof(*hdr);
        uint16_t tlv_sz = sizeof(*tlv_0x13);

        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x13 = (QMI_CAT_pc_full_func_evt_mask_t *)buf_position;
        COPY_HTOM(tlv_0x13->pc_full_func_evt_report_req_mask, input->pc_full_func_evt_report_req_mask);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_cat_set_event_report_unpack(uint8_t *rsp,
        uint16_t len, cat_set_event_report_masks_t *out)
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
        QMI_CAT_generic_pc_evt_mask_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_CAT_generic_pc_evt_mask_t);

        hdr++;
        tlv_0x10 = (QMI_CAT_generic_pc_evt_mask_t *)hdr;
        out->pc_evt_report_req_mask_available = true;
        COPY_MTOH(out->pc_evt_report_req_mask, tlv_0x10->pc_evt_report_req_mask);
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_CAT_pc_full_func_evt_mask_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_CAT_pc_full_func_evt_mask_t);

        hdr++;
        tlv_0x12 = (QMI_CAT_pc_full_func_evt_mask_t *)hdr;
        out->pc_full_func_evt_report_req_mask_available = true;
        COPY_MTOH(out->pc_full_func_evt_report_req_mask, tlv_0x12->pc_full_func_evt_report_req_mask);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_CAT_SEND_TR (33 - 0x0021)
 */

QMI_SDK_error_t telit_cat_send_tr_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        cat_send_tr_input_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_CAT_generic_proactive_command_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_CAT_SEND_TR;
    req_ctx->svc_id = QMI_SVC_CAT;
    req_ctx->timeout = QMI_TIMEOUT_MEDIUM;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->terminal_response.cmd_sz;

    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_CAT_generic_proactive_command_t *)buf_position;
    COPY_HTOM(tlv_0x01->uim_ref_id, input->terminal_response.uim_ref_id);
    COPY_HTOM(tlv_0x01->cmd_sz, input->terminal_response.cmd_sz);
    buf_position = (uint8_t *)&tlv_0x01->cmd_sz;
    buf_position += sizeof(tlv_0x01->cmd_sz);
    memcpy(buf_position, input->terminal_response.cmd, input->terminal_response.cmd_sz);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_cat_send_tr_unpack(uint8_t *rsp, uint16_t len,
        cat_send_tr_info_t *out)
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
    if (hdr) {
        QMI_CAT_generic_terminal_response_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_CAT_generic_terminal_response_t *)hdr;
        COPY_MTOH(out->terminal_response.sw1, tlv->sw1);
        COPY_MTOH(out->terminal_response.sw2, tlv->sw2);
        COPY_MTOH(out->terminal_response.tr_response_sz, tlv->tr_response_sz);
        /* No need to check tr_response_sz, since it's uint8_t always < QMI_CAT_TERMINAL_RESPONSE_MAX_SZ */
        buf_position = (uint8_t *)&tlv->tr_response_sz;
        buf_position += sizeof(tlv->tr_response_sz);
        memcpy(out->terminal_response.tr_response, buf_position, out->terminal_response.tr_response_sz);
        out->terminal_response_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_CAT_SEND_ENVELOPE_CMD (34 - 0x0022)
 */

QMI_SDK_error_t telit_cat_send_envelope_cmd_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        cat_send_envelope_cmd_input_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_CAT_SEND_ENVELOPE_CMD_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_CAT_SEND_ENVELOPE_CMD;
    req_ctx->svc_id = QMI_SVC_CAT;
    req_ctx->timeout = QMI_TIMEOUT_MEDIUM;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->env_cmd_sz;

    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_CAT_SEND_ENVELOPE_CMD_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->env_cmd_type, input->env_cmd_type);
    COPY_HTOM(tlv_0x01->env_cmd_sz, input->env_cmd_sz);
    buf_position = (uint8_t *)&tlv_0x01->env_cmd_sz;
    buf_position += sizeof(tlv_0x01->env_cmd_sz);
    memcpy(buf_position, input->env_cmd, input->env_cmd_sz);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_cat_send_envelope_cmd_unpack(uint8_t *rsp, uint16_t len,
        cat_send_envelope_cmd_info_t *out)
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
    if (hdr) {
        QMI_CAT_generic_terminal_response_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_CAT_generic_terminal_response_t *)hdr;
        COPY_MTOH(out->envelope_response.sw1, tlv->sw1);
        COPY_MTOH(out->envelope_response.sw2, tlv->sw2);
        COPY_MTOH(out->envelope_response.tr_response_sz, tlv->tr_response_sz);
        /* No need to check tr_response_sz, since it's uint8_t always < QMI_CAT_TERMINAL_RESPONSE_MAX_SZ */
        buf_position = (uint8_t *)&tlv->tr_response_sz;
        buf_position += sizeof(tlv->tr_response_sz);
        memcpy(out->envelope_response.tr_response, buf_position, out->envelope_response.tr_response_sz);
        out->envelope_response_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_CAT_SET_CONFIGURATION (45 - 0x002D)
 */

QMI_SDK_error_t telit_cat_set_configuration_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        cat_configuration_mode_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_CAT_generic_configuration_mode_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_CAT_SET_CONFIGURATION;
    req_ctx->svc_id = QMI_SVC_CAT;
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

    tlv_0x01 = (QMI_CAT_generic_configuration_mode_t *)buf_position;
    COPY_HTOM(tlv_0x01->cat_config_mode, input->cat_config_mode);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_cat_set_configuration_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_CAT_GET_CONFIGURATION (46 - 0x002E)
 */

QMI_SDK_error_t telit_cat_get_configuration_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_CAT_GET_CONFIGURATION,
            QMI_SVC_CAT,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_cat_get_configuration_unpack(uint8_t *rsp, uint16_t len,
        cat_configuration_mode_t *out)
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
        QMI_CAT_generic_configuration_mode_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_CAT_generic_configuration_mode_t);

        hdr++;
        tlv_0x10 = (QMI_CAT_generic_configuration_mode_t *)hdr;
        out->cat_config_mode_available = true;
        COPY_MTOH(out->cat_config_mode, tlv_0x10->cat_config_mode);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_CAT_GET_CACHED_PROACTIVE_CMD (47 - 0x002F)
 */

QMI_SDK_error_t telit_cat_get_cached_proactive_cmd_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        cat_cached_proactive_cmd_input_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_CAT_GET_CACHED_PROACTIVE_CMD_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_CAT_GET_CACHED_PROACTIVE_CMD;
    req_ctx->svc_id = QMI_SVC_CAT;
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

    tlv_0x01 = (QMI_CAT_GET_CACHED_PROACTIVE_CMD_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->command_id, input->command_id);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_cat_get_cached_proactive_cmd_unpack(uint8_t *rsp, uint16_t len,
        cat_cached_proactive_cmd_info_t *out)
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
    if (hdr) {
        QMI_CAT_generic_proactive_command_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_CAT_generic_proactive_command_t *)hdr;
        COPY_MTOH(out->setup_menu_event.uim_ref_id, tlv->uim_ref_id);
        COPY_MTOH(out->setup_menu_event.cmd_sz, tlv->cmd_sz);
        if (out->setup_menu_event.cmd_sz > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ) {
            RLOGE("%s: cmd_sz = %u > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ, increase the define",
                    __FUNCTION__, out->setup_menu_event.cmd_sz);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = (uint8_t *)&tlv->cmd_sz;
        buf_position += sizeof(tlv->cmd_sz);
        memcpy(out->setup_menu_event.cmd, buf_position, out->setup_menu_event.cmd_sz);
        out->setup_menu_event_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        QMI_CAT_generic_proactive_command_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_CAT_generic_proactive_command_t *)hdr;
        COPY_MTOH(out->setup_event_list_raw_event.uim_ref_id, tlv->uim_ref_id);
        COPY_MTOH(out->setup_event_list_raw_event.cmd_sz, tlv->cmd_sz);
        if (out->setup_event_list_raw_event.cmd_sz > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ) {
            RLOGE("%s: cmd_sz = %u > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ, increase the define",
                    __FUNCTION__, out->setup_event_list_raw_event.cmd_sz);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = (uint8_t *)&tlv->cmd_sz;
        buf_position += sizeof(tlv->cmd_sz);
        memcpy(out->setup_event_list_raw_event.cmd, buf_position, out->setup_event_list_raw_event.cmd_sz);
        out->setup_event_list_raw_event_available = true;
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    if (hdr) {
        QMI_CAT_generic_proactive_command_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_CAT_generic_proactive_command_t *)hdr;
        COPY_MTOH(out->setup_idle_mode_text_event.uim_ref_id, tlv->uim_ref_id);
        COPY_MTOH(out->setup_idle_mode_text_event.cmd_sz, tlv->cmd_sz);
        if (out->setup_idle_mode_text_event.cmd_sz > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ) {
            RLOGE("%s: cmd_sz = %u > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ, increase the define",
                    __FUNCTION__, out->setup_idle_mode_text_event.cmd_sz);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = (uint8_t *)&tlv->cmd_sz;
        buf_position += sizeof(tlv->cmd_sz);
        memcpy(out->setup_idle_mode_text_event.cmd, buf_position, out->setup_idle_mode_text_event.cmd_sz);
        out->setup_idle_mode_text_event_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI CAT indications
 */

static QMI_SDK_error_t telit_cat_handle_event_report_ind(uint16_t *payload_len, uint8_t *msg,
        uint8_t *out, uint16_t out_len)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    cat_ind_event_report_t *ev_rep = (cat_ind_event_report_t *) out;
    uint8_t *buf_position;

    if (out_len < sizeof(*ev_rep)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }
    memset(ev_rep, 0, sizeof(*ev_rep));

    hdr = search_TLV(msg, 0x10, *payload_len);
    if (hdr) {
        QMI_CAT_generic_proactive_command_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_CAT_generic_proactive_command_t *)hdr;
        COPY_MTOH(ev_rep->display_text_event.uim_ref_id, tlv->uim_ref_id);
        COPY_MTOH(ev_rep->display_text_event.cmd_sz, tlv->cmd_sz);
        if (ev_rep->display_text_event.cmd_sz > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ) {
            RLOGE("%s: cmd_sz = %u > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ, increase the define",
                    __FUNCTION__, ev_rep->display_text_event.cmd_sz);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = (uint8_t *)&tlv->cmd_sz;
        buf_position += sizeof(tlv->cmd_sz);
        memcpy(ev_rep->display_text_event.cmd, buf_position, ev_rep->display_text_event.cmd_sz);
        ev_rep->display_text_event_available = true;
    }

    hdr = search_TLV(msg, 0x11, *payload_len);
    if (hdr) {
        QMI_CAT_generic_proactive_command_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_CAT_generic_proactive_command_t *)hdr;
        COPY_MTOH(ev_rep->get_inkey_event.uim_ref_id, tlv->uim_ref_id);
        COPY_MTOH(ev_rep->get_inkey_event.cmd_sz, tlv->cmd_sz);
        if (ev_rep->get_inkey_event.cmd_sz > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ) {
            RLOGE("%s: cmd_sz = %u > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ, increase the define",
                    __FUNCTION__, ev_rep->get_inkey_event.cmd_sz);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = (uint8_t *)&tlv->cmd_sz;
        buf_position += sizeof(tlv->cmd_sz);
        memcpy(ev_rep->get_inkey_event.cmd, buf_position, ev_rep->get_inkey_event.cmd_sz);
        ev_rep->get_inkey_event_available = true;
    }

    hdr = search_TLV(msg, 0x12, *payload_len);
    if (hdr) {
        QMI_CAT_generic_proactive_command_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_CAT_generic_proactive_command_t *)hdr;
        COPY_MTOH(ev_rep->get_input_event.uim_ref_id, tlv->uim_ref_id);
        COPY_MTOH(ev_rep->get_input_event.cmd_sz, tlv->cmd_sz);
        if (ev_rep->get_input_event.cmd_sz > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ) {
            RLOGE("%s: cmd_sz = %u > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ, increase the define",
                    __FUNCTION__, ev_rep->get_input_event.cmd_sz);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = (uint8_t *)&tlv->cmd_sz;
        buf_position += sizeof(tlv->cmd_sz);
        memcpy(ev_rep->get_input_event.cmd, buf_position, ev_rep->get_input_event.cmd_sz);
        ev_rep->get_input_event_available = true;
    }

    hdr = search_TLV(msg, 0x13, *payload_len);
    if (hdr) {
        QMI_CAT_generic_proactive_command_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_CAT_generic_proactive_command_t *)hdr;
        COPY_MTOH(ev_rep->setup_menu_event.uim_ref_id, tlv->uim_ref_id);
        COPY_MTOH(ev_rep->setup_menu_event.cmd_sz, tlv->cmd_sz);
        if (ev_rep->setup_menu_event.cmd_sz > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ) {
            RLOGE("%s: cmd_sz = %u > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ, increase the define",
                    __FUNCTION__, ev_rep->setup_menu_event.cmd_sz);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = (uint8_t *)&tlv->cmd_sz;
        buf_position += sizeof(tlv->cmd_sz);
        memcpy(ev_rep->setup_menu_event.cmd, buf_position, ev_rep->setup_menu_event.cmd_sz);
        ev_rep->setup_menu_event_available = true;
    }

    hdr = search_TLV(msg, 0x14, *payload_len);
    if (hdr) {
        QMI_CAT_generic_proactive_command_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_CAT_generic_proactive_command_t *)hdr;
        COPY_MTOH(ev_rep->select_item_event.uim_ref_id, tlv->uim_ref_id);
        COPY_MTOH(ev_rep->select_item_event.cmd_sz, tlv->cmd_sz);
        if (ev_rep->select_item_event.cmd_sz > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ) {
            RLOGE("%s: cmd_sz = %u > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ, increase the define",
                    __FUNCTION__, ev_rep->select_item_event.cmd_sz);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = (uint8_t *)&tlv->cmd_sz;
        buf_position += sizeof(tlv->cmd_sz);
        memcpy(ev_rep->select_item_event.cmd, buf_position, ev_rep->select_item_event.cmd_sz);
        ev_rep->select_item_event_available = true;
    }

    hdr = search_TLV(msg, 0x1A, *payload_len);
    if (hdr) {
        QMI_CAT_EVENT_REPORT_indication_0x1A_t *tlv_0x1A;

        CHECK_UNPACK_TLV_SIZE(*tlv_0x1A);

        hdr++;
        tlv_0x1A = (QMI_CAT_EVENT_REPORT_indication_0x1A_t *)hdr;
        COPY_MTOH(ev_rep->proactive_session_end_type,
                tlv_0x1A->proactive_session_end_type);
        ev_rep->proactive_session_end_type_available = true;
    }

    hdr = search_TLV(msg, 0x4B, *payload_len);
    if (hdr) {
        QMI_CAT_generic_proactive_command_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_CAT_generic_proactive_command_t *)hdr;
        COPY_MTOH(ev_rep->send_sms_event.uim_ref_id, tlv->uim_ref_id);
        COPY_MTOH(ev_rep->send_sms_event.cmd_sz, tlv->cmd_sz);
        if (ev_rep->send_sms_event.cmd_sz > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ) {
            RLOGE("%s: cmd_sz = %u > QMI_CAT_PROACTIVE_COMMAND_MAX_SZ, increase the define",
                    __FUNCTION__, ev_rep->send_sms_event.cmd_sz);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = (uint8_t *)&tlv->cmd_sz;
        buf_position += sizeof(tlv->cmd_sz);
        memcpy(ev_rep->send_sms_event.cmd, buf_position, ev_rep->send_sms_event.cmd_sz);
        ev_rep->send_sms_event_available = true;
    }

    hdr = search_TLV(msg, 0x50, *payload_len);
    if (hdr) {
        QMI_CAT_EVENT_REPORT_indication_0x50_t *tlv_0x50;

        CHECK_UNPACK_TLV_SIZE(*tlv_0x50);

        hdr++;
        tlv_0x50 = (QMI_CAT_EVENT_REPORT_indication_0x50_t *)hdr;
        COPY_MTOH(ev_rep->slot, tlv_0x50->slot);
        ev_rep->slot_available = true;
    }

    hdr = search_TLV(msg, 0x68, *payload_len);
    if (hdr) {
        QMI_CAT_EVENT_REPORT_indication_0x68_t *tlv_0x68;

        CHECK_UNPACK_TLV_SIZE(*tlv_0x68);

        hdr++;
        tlv_0x68 = (QMI_CAT_EVENT_REPORT_indication_0x68_t *)hdr;
        COPY_MTOH(ev_rep->rsp_type, tlv_0x68->rsp_type);
        ev_rep->rsp_type_available = true;
    }

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

QMI_SDK_error_t telit_cat_indication_handler(uint16_t msg_id,
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
        case QMI_CAT_EVENT_REPORT_IND:
            ret = telit_cat_handle_event_report_ind(&payload_len, msg, out, out_len);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        default:
            RLOGW("%s: indication %u not supported", __FUNCTION__, msg_id);
            break;
    }

end:
    return ret;
}
