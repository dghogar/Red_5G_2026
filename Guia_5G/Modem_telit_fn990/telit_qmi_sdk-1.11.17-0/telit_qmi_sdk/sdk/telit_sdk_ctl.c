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
#include "telit_sdk_private_ctl.h"
#include "telit_sdk_ctl.h"

/*****************************************************************************
 * CTL helper functions
 */

static uint16_t get_ctl_payload_size(uint8_t *buf)
{
    QMI_raw_message_header_t *hdr_message;

    hdr_message = (QMI_raw_message_header_t *)(buf + sizeof(QMI_CTL_raw_transaction_header_t));
    RLOGD("%s: message length: %u", __FUNCTION__, le16toh(hdr_message->len));

    return le16toh(hdr_message->len);
}

static QMI_SDK_error_t validate_ctl_unpack(uint8_t *rsp,
        uint16_t len __unused,
        void *out, bool have_response,
        uint16_t *payload_len,
        uint8_t **msg)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    uint16_t qmi_error = 0;
    uint8_t *msg_tmp;
    uint16_t payload_len_tmp;

    if (have_response && !out) {
        RLOGE("%s: invalid arg: output buffer expected", __FUNCTION__);
        goto end;
    }

    if (!rsp) {
        RLOGE("%s: invalid arg: response expected", __FUNCTION__);
        goto end;
    }

    payload_len_tmp = get_ctl_payload_size(rsp);
    msg_tmp = rsp + sizeof(QMI_CTL_raw_transaction_header_t) + \
            sizeof(QMI_raw_message_header_t);

    ret = validate_message(msg_tmp, payload_len_tmp, &qmi_error);
    if (ret == QMI_SDK_ERR_MALFORMED_RSP) {
        RLOGE("%s: malformed response", __FUNCTION__);
        goto end;
    }

    if (ret == QMI_SDK_ERR_QMI_PROTOCOL_ERROR) {
        RLOGE("%s: qmi protocol error", __FUNCTION__);
        ret = qmi_error + QMI_SDK_ERR_ERR_QMI_OFFSET;
    }

    if (payload_len) {
        *payload_len = payload_len_tmp;
    }

    if (msg) {
        *msg = msg_tmp;
    }

end:
    return ret;
}

static void add_ctl_header(const pack_qmi_t *context,
        uint16_t payload_len,
        uint8_t *buffer,
        uint16_t *buffer_len)
{
    QMI_raw_message_header_t *msg = NULL;
    QMI_CTL_raw_transaction_header_t *hdr = NULL;
    const unsigned long total_hdr_sz =
            (unsigned long)sizeof(QMI_CTL_raw_transaction_header_t) + \
            (unsigned long)sizeof(QMI_raw_message_header_t);

    /* Formatting header */
    hdr = (QMI_CTL_raw_transaction_header_t *) &buffer[0];
    hdr->control_flags = 0;
    hdr->transaction_id = context->xid;

    hdr++;

    msg = (QMI_raw_message_header_t *)hdr;
    msg->msg_id = htole16(context->msg_id);
    msg->len = (unsigned short)htole16(payload_len);

    *buffer_len = (payload_len + total_hdr_sz);

    RLOGD("%s: transaction id: %u", __FUNCTION__, context->xid);
    RLOGD("%s: msg id: %u", __FUNCTION__, msg->msg_id);
    RLOGD("%s: msg len: %u", __FUNCTION__, msg->len);
    RLOGD("%s: buffer len: %u", __FUNCTION__, *buffer_len);

    telit_print_buffer(buffer, *buffer_len, LOG_DEBUG);
}

static void *skip_ctl_msg_header(void *buf)
{
    uint8_t *tmp = (uint8_t *)buf;

    tmp += (sizeof(QMI_CTL_raw_transaction_header_t) +
            sizeof(QMI_raw_message_header_t));

    return tmp;
}

QMI_SDK_error_t add_qmux_header(const pack_qmi_t *context,
        uint8_t client_id,
        const uint8_t *payload,
        uint16_t payload_len,
        uint8_t *buffer,
        uint16_t *buffer_len)
{
    QMI_qmux_header_t *hdr = NULL;
    /* Message length does not include if_type */
    const unsigned long msg_sz =
            (unsigned long)sizeof(QMI_qmux_header_t) - 1
            + payload_len;

    if (*buffer_len < (msg_sz + 1)) {
        RLOGD("%s: required buffer size %lu, provided %u", __FUNCTION__, msg_sz + 1, *buffer_len);

        return QMI_SDK_ERR_BUFFER_SZ;
    }

    /* Formatting header */
    hdr = (QMI_qmux_header_t *) &buffer[0];
    hdr->if_type = 0x01;
    hdr->length = htole16(msg_sz);
    hdr->control_flags = 0;
    hdr->service_type = context->svc_id;
    hdr->client_id = client_id;

    hdr++;

    memcpy(hdr, payload, payload_len);

    *buffer_len = (msg_sz + 1);

    RLOGD("%s: buffer len: %u", __FUNCTION__, *buffer_len);

    return QMI_SDK_ERR_NONE;
}

QMI_SDK_error_t skip_qmux_header(const uint8_t *rsp,
        uint16_t rsp_len,
        uint8_t **payload,
        uint16_t *payload_len)
{
    int sz = (rsp_len - sizeof(QMI_qmux_header_t));

    if (sz <= 0) {
        return QMI_SDK_ERR_MALFORMED_RSP;
    }

    *payload_len = (uint16_t)sz;
    *payload = ((uint8_t *)rsp + sizeof(QMI_qmux_header_t));
    RLOGD("%s: payload = %p", __FUNCTION__, *payload);

    return QMI_SDK_ERR_NONE;
}

/*****************************************************************************
 * QMI_CTL_GET_VERSION_INFO (33 - 0x0021)
 */

QMI_SDK_error_t telit_ctl_get_version_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    if (!req_ctx || !req || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_CTL_GET_VERSION_INFO;
    req_ctx->svc_id = QMI_SVC_CONTROL;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    if ((sizeof(QMI_CTL_raw_transaction_header_t) +
            sizeof(QMI_raw_message_header_t)) > (*len)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }

    add_ctl_header(req_ctx, 0, req, len);

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

QMI_SDK_error_t telit_ctl_get_version_info_unpack(uint8_t *rsp,
        uint16_t len,
        ctl_get_version_info_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_CTL_GET_VERSION_INFO_response_0x01_t *tlv_0x01;
    QMI_CTL_generic_service_version_t *item;
    uint8_t *skip = NULL;
    uint8_t i;

    memset(out, 0, sizeof(*out));

    ret = validate_ctl_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_CTL_GET_VERSION_INFO_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_CTL_GET_VERSION_INFO_response_0x01_t *)hdr;
    /* Size is not checked, since there can't be a number of services > QMI_CTL_MAX_SERVICE_LIST_LEN */
    COPY_MTOH(out->service_version_list_size, tlv_0x01->service_version_list_size);
    skip = &tlv_0x01->service_version_list_size;
    skip += sizeof(tlv_0x01->service_version_list_size);
    item = (QMI_CTL_generic_service_version_t *)skip;
    for (i = 0; i < out->service_version_list_size; i++) {
        COPY_MTOH(out->service_version_list[i].qmi_svc_type, item[i].qmi_svc_type);
        COPY_MTOH(out->service_version_list[i].major_ver, item[i].major_ver);
        COPY_MTOH(out->service_version_list[i].minor_ver, item[i].minor_ver);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_CTL_GET_CLIENT_ID (34 - 0x0022)
 */

QMI_SDK_error_t telit_ctl_get_client_id_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        ctl_get_client_id_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_CTL_generic_svc_type_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_CTL_GET_CLIENT_ID;
    req_ctx->svc_id = QMI_SVC_CONTROL;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_ctl_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_CTL_generic_svc_type_t *)buf_position;
    COPY_HTOM(tlv_0x01->qmi_svc_type, input->qmi_svc_type);

    add_ctl_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

static QMI_SDK_error_t telit_ctl_client_id_unpack(uint8_t *rsp, uint16_t len, ctl_get_client_id_info_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_CTL_generic_client_id_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    ret = validate_ctl_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_CTL_generic_client_id_t);

    hdr++;
    tlv_0x01 = (QMI_CTL_generic_client_id_t *)hdr;
    COPY_MTOH(out->qmi_svc_type, tlv_0x01->qmi_svc_type);
    COPY_MTOH(out->client_id, tlv_0x01->client_id);

end:
    return ret;
}

QMI_SDK_error_t telit_ctl_get_client_id_unpack(uint8_t *rsp, uint16_t len, ctl_get_client_id_info_t *out)
{
    return telit_ctl_client_id_unpack(rsp, len, out);
}

/*****************************************************************************
 * QMI_CTL_RELEASE_CLIENT_ID (35 - 0x0023)
 */

QMI_SDK_error_t telit_ctl_release_client_id_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        ctl_get_client_id_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_CTL_generic_client_id_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_CTL_RELEASE_CLIENT_ID;
    req_ctx->svc_id = QMI_SVC_CONTROL;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_ctl_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_CTL_generic_client_id_t *)buf_position;
    COPY_HTOM(tlv_0x01->qmi_svc_type, input->qmi_svc_type);
    COPY_HTOM(tlv_0x01->client_id, input->client_id);

    add_ctl_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_ctl_release_client_id_unpack(uint8_t *rsp, uint16_t len, ctl_get_client_id_info_t *out)
{
    return telit_ctl_client_id_unpack(rsp, len, out);
}
