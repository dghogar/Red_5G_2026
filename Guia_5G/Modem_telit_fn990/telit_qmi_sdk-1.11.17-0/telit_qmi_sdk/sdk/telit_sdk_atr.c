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
#include "telit_sdk_private_atr.h"
#include "telit_sdk_atr.h"

/*****************************************************************************
 * QMI_ATR_CMD_SEND (0 - 0x0000)
 */

QMI_SDK_error_t telit_atr_cmd_send_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        atr_cmd_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_ATR_generic_cmd_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    if (input->cmd_len > QMI_ATR_MAX_CMD_LEN) {
        RLOGE("%s: maximum allowed command length is %d",
                __FUNCTION__, QMI_ATR_MAX_CMD_LEN);
        goto end;
    }

    req_ctx->msg_id = QMI_ATR_CMD_SEND;
    req_ctx->svc_id = QMI_SVC_ATR;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->cmd_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_ATR_generic_cmd_t *)buf_position;
    COPY_HTOM(tlv_0x01->cmd_len, input->cmd_len);
    buf_position = (uint8_t *)&(tlv_0x01->cmd_len);
    buf_position += sizeof(tlv_0x01->cmd_len);
    memcpy(buf_position, input->cmd, input->cmd_len);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_atr_cmd_send_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI ATR indications
 */

static QMI_SDK_error_t telit_atr_handle_cmd_ind(uint16_t *payload_len, uint8_t *msg,
        uint8_t *out, uint16_t out_len)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_ATR_generic_cmd_t *tlv_0x01;
    atr_cmd_t *ev_rep = (atr_cmd_t *) out;
    uint8_t *buf_position;

    if (out_len < sizeof(*ev_rep)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }
    memset(ev_rep, 0, sizeof(*ev_rep));

    FIND_MANDATORY_TLV(msg, 0x01, *payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_ATR_generic_cmd_t);

    hdr++;
    tlv_0x01 = (QMI_ATR_generic_cmd_t *)hdr;
    COPY_MTOH(ev_rep->cmd_len, tlv_0x01->cmd_len);
    if (ev_rep->cmd_len > QMI_ATR_MAX_CMD_LEN) {
        RLOGE("%s: cmd_len = %u > QMI_ATR_MAX_CMD_LEN, increase the define",
                __FUNCTION__, ev_rep->cmd_len);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }
    buf_position = (uint8_t *)&(tlv_0x01->cmd_len);
    buf_position += sizeof(tlv_0x01->cmd_len);
    memcpy(ev_rep->cmd, buf_position, ev_rep->cmd_len);

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

QMI_SDK_error_t telit_atr_indication_handler(uint16_t msg_id,
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
        case QMI_ATR_CMD_IND:
            ret = telit_atr_handle_cmd_ind(&payload_len, msg, out, out_len);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        default:
            RLOGW("%s: Indication not supported", __FUNCTION__);
            break;
    }

end:
    return ret;
}
