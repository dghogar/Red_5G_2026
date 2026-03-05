#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#if defined __WINDOWS_QMI__
#include "portable_endian.h"
#else
#include <endian.h>
#endif

#include "telit_sdk.h"
#include "telit_sdk_private.h"
#include "version.h"

#define TELIT_SDK_VERSION MAJOR "."MINOR "."PATCH "-"CUST

const char qmi_msg_unknown[] = {"unknown"};
const char qmi_msg_response[] = {"response"};
const char qmi_msg_indication[] = {"indication"};

static char *qmi_error_name[] = {
    [QMI_ERR_NONE] = "QMI_ERR_NONE",
    [QMI_ERR_MALFORMED_MSG] = "QMI_ERR_MALFORMED_MSG",
    [QMI_ERR_NO_MEMORY] = "QMI_ERR_NO_MEMORY",
    [QMI_ERR_INTERNAL] = "QMI_ERR_INTERNAL",
    [QMI_ERR_ABORTED] = "QMI_ERR_ABORTED",
    [QMI_ERR_CLIENT_IDS_EXHAUSTED] = "QMI_ERR_CLIENT_IDS_EXHAUSTED",
    [QMI_ERR_UNABORTABLE_TRANSACTION] = "QMI_ERR_UNABORTABLE_TRANSACTION",
    [QMI_ERR_INVALID_CLIENT_ID] = "QMI_ERR_INVALID_CLIENT_ID",
    [QMI_ERR_NO_THRESHOLDS] = "QMI_ERR_NO_THRESHOLDS",
    [QMI_ERR_INVALID_HANDLE] = "QMI_ERR_INVALID_HANDLE",
    [QMI_ERR_INVALID_PROFILE] = "QMI_ERR_INVALID_PROFILE",
    [QMI_ERR_INVALID_PINID] = "QMI_ERR_INVALID_PINID",
    [QMI_ERR_INCORRECT_PIN] = "QMI_ERR_INCORRECT_PIN",
    [QMI_ERR_NO_NETWORK_FOUND] = "QMI_ERR_NO_NETWORK_FOUND",
    [QMI_ERR_CALL_FAILED] = "QMI_ERR_CALL_FAILED",
    [QMI_ERR_OUT_OF_CALL] = "QMI_ERR_OUT_OF_CALL",
    [QMI_ERR_NOT_PROVISIONED] = "QMI_ERR_NOT_PROVISIONED",
    [QMI_ERR_MISSING_ARG] = "QMI_ERR_MISSING_ARG",
    [QMI_ERR_ARG_TOO_LONG] = "QMI_ERR_ARG_TOO_LONG",
    [QMI_ERR_INVALID_TX_ID] = "QMI_ERR_INVALID_TX_ID",
    [QMI_ERR_DEVICE_IN_USE] = "QMI_ERR_DEVICE_IN_USE",
    [QMI_ERR_OP_NETWORK_UNSUPPORTED] = "QMI_ERR_OP_NETWORK_UNSUPPORTED",
    [QMI_ERR_OP_DEVICE_UNSUPPORTED] = "QMI_ERR_OP_DEVICE_UNSUPPORTED",
    [QMI_ERR_NO_EFFECT] = "QMI_ERR_NO_EFFECT",
    [QMI_ERR_NO_FREE_PROFILE] = "QMI_ERR_NO_FREE_PROFILE",
    [QMI_ERR_INVALID_PDP_TYPE] = "QMI_ERR_INVALID_PDP_TYPE",
    [QMI_ERR_INVALID_TECH_PREF] = "QMI_ERR_INVALID_TECH_PREF",
    [QMI_ERR_INVALID_PROFILE_TYPE] = "QMI_ERR_INVALID_PROFILE_TYPE",
    [QMI_ERR_INVALID_SERVICE_TYPE] = "QMI_ERR_INVALID_SERVICE_TYPE",
    [QMI_ERR_INVALID_REGISTER_ACTION] = "QMI_ERR_INVALID_REGISTER_ACTION",
    [QMI_ERR_INVALID_PS_ATTACH_ACTION] = "QMI_ERR_INVALID_PS_ATTACH_ACTION",
    [QMI_ERR_AUTHENTICATION_FAILED] = "QMI_ERR_AUTHENTICATION_FAILED",
    [QMI_ERR_PIN_BLOCKED] = "QMI_ERR_PIN_BLOCKED",
    [QMI_ERR_PIN_PERM_BLOCKED] = "QMI_ERR_PIN_PERM_BLOCKED",
    [QMI_ERR_SIM_NOT_INITIALIZED] = "QMI_ERR_SIM_NOT_INITIALIZED",
    [QMI_ERR_MAX_QOS_REQUESTS_IN_USE] = "QMI_ERR_MAX_QOS_REQUESTS_IN_USE",
    [QMI_ERR_INCORRECT_FLOW_FILTER] = "QMI_ERR_INCORRECT_FLOW_FILTER",
    [QMI_ERR_NETWORK_QOS_UNAWARE] = "QMI_ERR_NETWORK_QOS_UNAWARE",
    [QMI_ERR_INVALID] = "QMI_ERR_INVALID",
    [QMI_ERR_REQUESTED_NUM_UNSUPPORTED] = "QMI_ERR_REQUESTED_NUM_UNSUPPORTED",
    [QMI_ERR_INTERFACE_NOT_FOUND] = "QMI_ERR_INTERFACE_NOT_FOUND",
    [QMI_ERR_FLOW_SUSPENDED] = "QMI_ERR_FLOW_SUSPENDED",
    [QMI_ERR_INVALID_DATA_FORMAT] = "QMI_ERR_INVALID_DATA_FORMAT",
    [QMI_ERR_GENERAL] = "QMI_ERR_GENERAL",
    [QMI_ERR_UNKNOWN] = "QMI_ERR_UNKNOWN",
    [QMI_ERR_INVALID_ARG] = "QMI_ERR_INVALID_ARG",
    [QMI_ERR_INVALID_INDEX] = "QMI_ERR_INVALID_INDEX",
    [QMI_ERR_NO_ENTRY] = "QMI_ERR_NO_ENTRY",
    [QMI_ERR_DEVICE_STORAGE_FULL] = "QMI_ERR_DEVICE_STORAGE_FULL",
    [QMI_ERR_DEVICE_NOT_READY] = "QMI_ERR_DEVICE_NOT_READY",
    [QMI_ERR_NETWORK_NOT_READY] = "QMI_ERR_NETWORK_NOT_READY",
    [QMI_ERR_CAUSE_CODE] = "QMI_ERR_CAUSE_CODE",
    [QMI_ERR_MESSAGE_NOT_SENT] = "QMI_ERR_MESSAGE_NOT_SENT",
    [QMI_ERR_MESSAGE_DELIVERY_FAILURE] = "QMI_ERR_MESSAGE_DELIVERY_FAILURE",
    [QMI_ERR_INVALID_MESSAGE_ID] = "QMI_ERR_INVALID_MESSAGE_ID",
    [QMI_ERR_ENCODING] = "QMI_ERR_ENCODING",
    [QMI_ERR_AUTHENTICATION_LOCK] = "QMI_ERR_AUTHENTICATION_LOCK",
    [QMI_ERR_INVALID_TRANSITION] = "QMI_ERR_INVALID_TRANSITION",
    [QMI_ERR_NOT_A_MCAST_IFACE] = "QMI_ERR_NOT_A_MCAST_IFACE",
    [QMI_ERR_MAX_MCAST_REQUESTS_IN_USE] = "QMI_ERR_MAX_MCAST_REQUESTS_IN_USE",
    [QMI_ERR_INVALID_MCAST_HANDLE] = "QMI_ERR_INVALID_MCAST_HANDLE",
    [QMI_ERR_INVALID_IP_FAMILY_PREF] = "QMI_ERR_INVALID_IP_FAMILY_PREF",
    [QMI_ERR_SESSION_INACTIVE] = "QMI_ERR_SESSION_INACTIVE",
    [QMI_ERR_SESSION_INVALID] = "QMI_ERR_SESSION_INVALID",
    [QMI_ERR_SESSION_OWNERSHIP] = "QMI_ERR_SESSION_OWNERSHIP",
    [QMI_ERR_INSUFFICIENT_RESOURCES] = "QMI_ERR_INSUFFICIENT_RESOURCES",
    [QMI_ERR_DISABLED] = "QMI_ERR_DISABLED",
    [QMI_ERR_INVALID_OPERATION] = "QMI_ERR_INVALID_OPERATION",
    [QMI_ERR_INVALID_QMI_CMD] = "QMI_ERR_INVALID_QMI_CMD",
    [QMI_ERR_TPDU_TYPE] = "QMI_ERR_TPDU_TYPE",
    [QMI_ERR_SMSC_ADDR] = "QMI_ERR_SMSC_ADDR",
    [QMI_ERR_INFO_UNAVAILABLE] = "QMI_ERR_INFO_UNAVAILABLE",
    [QMI_ERR_SEGMENT_TOO_LONG] = "QMI_ERR_SEGMENT_TOO_LONG",
    [QMI_ERR_SEGMENT_ORDER] = "QMI_ERR_SEGMENT_ORDER",
    [QMI_ERR_BUNDLING_NOT_SUPPORTED] = "QMI_ERR_BUNDLING_NOT_SUPPORTED",
    [QMI_ERR_OP_PARTIAL_FAILURE] = "QMI_ERR_OP_PARTIAL_FAILURE",
    [QMI_ERR_POLICY_MISMATCH] = "QMI_ERR_POLICY_MISMATCH",
    [QMI_ERR_SIM_FILE_NOT_FOUND] = "QMI_ERR_SIM_FILE_NOT_FOUND",
    [QMI_ERR_EXTENDED_INTERNAL] = "QMI_ERR_EXTENDED_INTERNAL",
    [QMI_ERR_ACCESS_DENIED] = "QMI_ERR_ACCESS_DENIED",
    [QMI_ERR_HARDWARE_RESTRICTED] = "QMI_ERR_HARDWARE_RESTRICTED",
    [QMI_ERR_ACK_NOT_SENT] = "QMI_ERR_ACK_NOT_SENT",
    [QMI_ERR_INJECT_TIMEOUT] = "QMI_ERR_INJECT_TIMEOUT",
    [QMI_ERR_INCOMPATIBLE_STATE] = "QMI_ERR_INCOMPATIBLE_STATE",
    [QMI_ERR_FDN_RESTRICT] = "QMI_ERR_FDN_RESTRICT",
    [QMI_ERR_SUPS_FAILURE_CAUSE] = "QMI_ERR_SUPS_FAILURE_CAUSE",
    [QMI_ERR_NO_RADIO] = "QMI_ERR_NO_RADIO",
    [QMI_ERR_NOT_SUPPORTED] = "QMI_ERR_NOT_SUPPORTED",
    [QMI_ERR_NO_SUBSCRIPTION] = "QMI_ERR_NO_SUBSCRIPTION",
    [QMI_ERR_CARD_CALL_CONTROL_FAILED] = "QMI_ERR_CARD_CALL_CONTROL_FAILED",
    [QMI_ERR_NETWORK_ABORTED] = "QMI_ERR_NETWORK_ABORTED",
    [QMI_ERR_MSG_BLOCKED] = "QMI_ERR_MSG_BLOCKED",
    [QMI_ERR_INVALID_SESSION_TYPE] = "QMI_ERR_INVALID_SESSION_TYPE",
    [QMI_ERR_INVALID_PB_TYPE] = "QMI_ERR_INVALID_PB_TYPE",
    [QMI_ERR_NO_SIM] = "QMI_ERR_NO_SIM",
    [QMI_ERR_PB_NOT_READY] = "QMI_ERR_PB_NOT_READY",
    [QMI_ERR_PIN_RESTRICTION] = "QMI_ERR_PIN_RESTRICTION",
    [QMI_ERR_PIN2_RESTRICTION] = "QMI_ERR_PIN2_RESTRICTION",
    [QMI_ERR_PUK_RESTRICTION] = "QMI_ERR_PUK_RESTRICTION",
    [QMI_ERR_PUK2_RESTRICTION] = "QMI_ERR_PUK2_RESTRICTION",
    [QMI_ERR_PB_ACCESS_RESTRICTED] = "QMI_ERR_PB_ACCESS_RESTRICTED",
    [QMI_ERR_PB_DELETE_IN_PROG] = "QMI_ERR_PB_DELETE_IN_PROG",
    [QMI_ERR_PB_TEXT_TOO_LONG] = "QMI_ERR_PB_TEXT_TOO_LONG",
    [QMI_ERR_PB_NUMBER_TOO_LONG] = "QMI_ERR_PB_NUMBER_TOO_LONG",
    [QMI_ERR_PB_HIDDEN_KEY_RESTRICTION] = "QMI_ERR_PB_HIDDEN_KEY_RESTRICTION",
    [QMI_ERR_PB_NOT_AVAILABLE] = "QMI_ERR_PB_NOT_AVAILABLE",
    [QMI_ERR_DEVICE_MEMORY_ERROR] = "QMI_ERR_DEVICE_MEMORY_ERROR",
    [QMI_ERR_NO_PERMISSION] = "QMI_ERR_NO_PERMISSION",
    [QMI_ERR_TOO_SOON] = "QMI_ERR_TOO_SOON",
    [QMI_ERR_TIME_NOT_ACQUIRED] = "QMI_ERR_TIME_NOT_ACQUIRED",
    [QMI_ERR_OP_IN_PROGRESS] = "QMI_ERR_OP_IN_PROGRESS",
    [QMI_ERR_FW_WRITE_FAILED] = "QMI_ERR_FW_WRITE_FAILED",
    [QMI_ERR_FW_INFO_READ_FAILED] = "QMI_ERR_FW_INFO_READ_FAILED",
    [QMI_ERR_FW_FILE_NOT_FOUND] = "QMI_ERR_FW_FILE_NOT_FOUND",
    [QMI_ERR_FW_DIR_NOT_FOUND] = "QMI_ERR_FW_DIR_NOT_FOUND",
    [QMI_ERR_FW_ALREADY_ACTIVATED] = "QMI_ERR_FW_ALREADY_ACTIVATED",
    [QMI_ERR_FW_CANNOT_GENERIC_IMAGE] = "QMI_ERR_FW_CANNOT_GENERIC_IMAGE",
    [QMI_ERR_FW_FILE_OPEN_FAILED] = "QMI_ERR_FW_FILE_OPEN_FAILED",
    [QMI_ERR_FW_UPDATE_DISCONTINOUS_FRAME] = "QMI_ERR_FW_UPDATE_DISCONTINOUS_FRAME",
    [QMI_ERR_FW_UPDATE_FAILED] = "QMI_ERR_FW_UPDATE_FAILED",
    [QMI_ERR_AUTO_SIM_ALREADY_ENABLED] = "QMI_ERR_AUTO_SIM_ALREADY_ENABLED",
    [QMI_ERR_UNLISTED] = "QMI_ERR_UNLISTED",
};

char *qmi_strerror(const QMI_protocol_error_t error)
{
    if (error >= QMI_ERR_UNLISTED ||
            /* QMI Error enum is a sparse list of integers */
            qmi_error_name[error] == NULL) {
        return qmi_error_name[QMI_ERR_UNLISTED];
    }

    return qmi_error_name[error];
}

/* Private */
QMI_SDK_error_t generic_pack_request(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        uint16_t msg_id, uint8_t svc, int timeout)
{
    if (!req_ctx || !req || !len) {
        RLOGE("%s: invalid arg, msg_id %u, svc %u", __FUNCTION__, msg_id, svc);
        return QMI_SDK_ERR_INVALID_ARG;
    }

    req_ctx->msg_id = msg_id;
    req_ctx->svc_id = svc;
    req_ctx->timeout = timeout;

    return pack_request(req_ctx, req, len);
}

QMI_SDK_error_t generic_unpack_request(uint8_t *rsp, uint16_t len)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    ret = validate_unpack(rsp, len, NULL, false, NULL, NULL);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
    }

    return ret;
}

QMI_raw_content_header_t *search_TLV(const uint8_t *buf, uint8_t tlv, uint32_t rsp_sz)
{
    uint16_t tlv_len;
    uint32_t data_processed = 0;
    uint8_t *buf_iter;
    QMI_raw_content_header_t *hdr;

    buf_iter = (uint8_t *)buf;

    while (data_processed < rsp_sz) {
        hdr = (QMI_raw_content_header_t *)buf_iter;
        if (tlv == hdr->type_id) {
#if !defined(ANDROID)
            RLOGD("%s: TLV 0x%x found", __FUNCTION__, tlv);
#endif
            return hdr;
        }

        tlv_len = le16toh(hdr->length);
        buf_iter += (sizeof(*hdr) + tlv_len);
        data_processed += (sizeof(*hdr) + tlv_len);
    }

#if !defined(ANDROID)
    RLOGD("%s: TLV 0x%x not found", __FUNCTION__, tlv);
#endif

    return NULL;
}

QMI_SDK_error_t get_string(uint8_t *buf_in,
        uint16_t buf_len,
        uint8_t tlv_id,
        uint16_t str_sz,
        char *str)
{
    QMI_SDK_error_t err = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr;
    uint16_t str_len;

    if (str_sz == 0 || !str || !buf_in) {
        RLOGE("%s: invalid argument", __FUNCTION__);
        err = QMI_SDK_ERR_INVALID_ARG;
        goto out;
    }
    memset(str, '\0', str_sz);

    hdr = search_TLV(buf_in, tlv_id, buf_len);
    if (!hdr) {
        RLOGD("%s: TLV 0x%x missing", __FUNCTION__, tlv_id);
        err = QMI_SDK_ERR_MISSING_TLV;
        goto out;
    }

    str_len = (uint16_t)le16toh(hdr->length);
    RLOGD("%s: str_len %u", __FUNCTION__, str_len);
    hdr++;

    if (str_len > (str_sz - 1)) {
        RLOGE("%s: output string buffer too small", __FUNCTION__);
        err = QMI_SDK_ERR_BUFFER_SZ;
        goto out;
    }

    if (str_len != 0 && str_sz > 0) {
        memcpy( str, hdr, str_len );
    }

out:
    return err;
}

uint16_t get_payload_size(uint8_t *buf)
{
    QMI_raw_message_header_t *hdr_message;

    hdr_message = (QMI_raw_message_header_t *)(buf + sizeof(QMI_service_raw_transaction_header_t));
    RLOGD("%s: message length: %u", __FUNCTION__, le16toh(hdr_message->len));

    return le16toh(hdr_message->len);
}

void add_header(const pack_qmi_t *context,
        uint16_t payload_len,
        uint8_t *buffer,
        uint16_t *buffer_len)
{
    QMI_raw_message_header_t *msg = NULL;
    QMI_service_raw_transaction_header_t *hdr = NULL;
    const unsigned long total_hdr_sz =
            (unsigned long)sizeof(QMI_service_raw_transaction_header_t) + \
            (unsigned long)sizeof(QMI_raw_message_header_t);

    /* Formatting header */
    hdr = (QMI_service_raw_transaction_header_t *) &buffer[0];
    hdr->compound = 0;
    hdr->response = 0;
    hdr->indication = 0;
    hdr->reserved = 0;
    hdr->transaction_id = htole16(context->xid);

    hdr++;

    msg = (QMI_raw_message_header_t *)hdr;
    msg->msg_id = htole16(context->msg_id);
    msg->len = (unsigned short)htole16(payload_len);

    *buffer_len = (payload_len + total_hdr_sz);

    RLOGD("%s: transaction id: 0x%x", __FUNCTION__, context->xid);
    RLOGD("%s: msg id: 0x%x", __FUNCTION__, msg->msg_id);
    RLOGD("%s: msg len: %u", __FUNCTION__, msg->len);
    RLOGD("%s: buffer len: %u", __FUNCTION__, *buffer_len);

    telit_print_buffer(buffer, *buffer_len, LOG_DEBUG);
}

/* MODEM to HOST conversion function */
static void _ec_mtoh(void *dest, size_t dest_size, uint64_t src)
{
    if (dest_size == 8) {
        uint64_t src_to_be_converted = src;
        uint64_t *dest_value = dest;

        *dest_value = le64toh(src_to_be_converted);
    } else if (dest_size == 4) {
        uint32_t src_to_be_converted = src;
        uint32_t *dest_value = dest;

        *dest_value = le32toh(src_to_be_converted);
    } else if (dest_size == 2) {
        uint16_t src_to_be_converted = src;
        uint16_t *dest_value = dest;

        *dest_value = le16toh(src_to_be_converted);
    } else if (dest_size == 1) {
        uint8_t *dest_value = dest;

        *dest_value = src;
    } else {
        RLOGW("%s: destination size %zu not supported", __FUNCTION__, dest_size);
    }
}

void ec_mtoh(void *dest, size_t dest_size, void *src, size_t src_size)
{
    if (dest_size != src_size) {
        RLOGE("%s: dest_size %zu != src_size %zu", __FUNCTION__, dest_size, src_size);
    }

    switch (src_size) {
        case 1: {
            uint8_t *src_value = src;

            _ec_mtoh(dest, dest_size, *src_value);
            break;
        }
        case 2: {
            uint16_t *src_value = src;

            _ec_mtoh(dest, dest_size, *src_value);
            break;
        }
        case 4: {
            uint32_t *src_value = src;

            _ec_mtoh(dest, dest_size, *src_value);
            break;
        }
        case 8: {
            uint64_t *src_value = src;

            _ec_mtoh(dest, dest_size, *src_value);
            break;
        }
        default:
            RLOGW("%s: source size %zu not supported", __FUNCTION__, src_size);
    }
}

static void _ec_htom(void *dest, size_t dest_size, uint64_t src)
{
    if (dest_size == 8) {
        uint64_t src_to_be_converted = src;
        uint64_t *dest_value = dest;

        *dest_value = htole64(src_to_be_converted);
    } else if (dest_size == 4) {
        uint32_t src_to_be_converted = src;
        uint32_t *dest_value = dest;

        *dest_value = htole32(src_to_be_converted);
    } else if (dest_size == 2) {
        uint16_t src_to_be_converted = src;
        uint16_t *dest_value = dest;

        *dest_value = htole16(src_to_be_converted);
    } else if (dest_size == 1) {
        uint8_t *dest_value = dest;

        *dest_value = src;
    } else {
        RLOGW("%s: destination size %zu not supported", __FUNCTION__, dest_size);
    }
}

/* HOST to MODEM conversion function */
void ec_htom(void *dest, size_t dest_size, void *src, size_t src_size)
{
    if (dest_size != src_size) {
        RLOGE("%s: dest_size %zu != src_size %zu", __FUNCTION__, dest_size, src_size);
    }

    switch (src_size) {
        case 1: {
            uint8_t *src_value = src;

            _ec_htom(dest, dest_size, *src_value);
            break;
        }
        case 2: {
            uint16_t *src_value = src;

            _ec_htom(dest, dest_size, *src_value);
            break;
        }
        case 4: {
            uint32_t *src_value = src;

            _ec_htom(dest, dest_size, *src_value);
            break;
        }
        case 8: {
            uint64_t *src_value = src;

            _ec_htom(dest, dest_size, *src_value);
            break;
        }
        default:
            RLOGW("%s: source size %zu not supported", __FUNCTION__, src_size);
    }
}

/* End private */

/* Generic pack request */
QMI_SDK_error_t pack_request(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    QMI_SDK_error_t ret;

    ret = validate_pack(0, *len);
    if (ret) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        goto end;
    }

    add_header(req_ctx, 0, req, len);

end:
    return ret;
}

/* 32 bit is the length of qmi request mandatory result TLV */
QMI_SDK_error_t validate_message(uint8_t *rsp, uint16_t len, uint16_t *error)
{
    QMI_raw_content_header_t *hdr;
    uint32_t *tmp;
    uint16_t qmi_ret = 0;
    uint32_t tlv_value;

    hdr = search_TLV(rsp, 0x02, len);
    if (!hdr) {
        RLOGE("%s: mandatory TLV missing", __FUNCTION__);
        qmi_ret = QMI_SDK_ERR_MALFORMED_RSP;
        goto end;
    }
    hdr++;

    tmp = (uint32_t *)hdr;
    tlv_value = le32toh(*tmp);

    *error = ((tlv_value) & 0x0000FFFF);
    if (*error) {
        RLOGE("%s: QMI_RESULT_FAILURE", __FUNCTION__);
        *error = ((((tlv_value) & 0xFFFF0000)) >> 16);
        RLOGE("%s: qmi protocol error 0x%04X (%u): %s",
                __FUNCTION__, *error, *error, qmi_strerror(*error));
        qmi_ret = QMI_SDK_ERR_QMI_PROTOCOL_ERROR;
    }

end:
    return (QMI_SDK_error_t)qmi_ret;
}

QMI_SDK_error_t validate_unpack(uint8_t *rsp,
        uint16_t len,
        void *out, bool have_response,
        uint16_t *payload_len,
        uint8_t **msg)
{
    uint16_t qmi_error = 0;
    uint8_t *msg_tmp;
    uint16_t payload_len_tmp;
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    if (have_response && !out) {
        RLOGE("%s: invalid arg: output buffer expected", __FUNCTION__);
        goto end;
    }

    if (!rsp) {
        RLOGE("%s: invalid arg: response expected", __FUNCTION__);
        goto end;
    }

    payload_len_tmp = get_payload_size(rsp);
    msg_tmp = rsp + sizeof(QMI_service_raw_transaction_header_t) + \
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
    if (ret != QMI_SDK_ERR_INVALID_ARG) {
        telit_print_buffer(rsp, len, (ret != QMI_SDK_ERR_NONE) ? LOG_ERR : LOG_DEBUG);
    }

    return ret;
}

void *skip_msg_header(void *buf)
{
    uint8_t *tmp = (uint8_t *)buf;

    tmp += (sizeof(QMI_service_raw_transaction_header_t) +
            sizeof(QMI_raw_message_header_t));

    return tmp;
}

QMI_SDK_error_t validate_pack(uint16_t payload_len, uint16_t buffer_len)
{
    if ((payload_len +
            sizeof(QMI_service_raw_transaction_header_t) +
            sizeof(QMI_raw_message_header_t)) <= buffer_len) {
        return QMI_SDK_ERR_NONE;
    }

    RLOGE("%s: buffer too small", __FUNCTION__);

    return QMI_SDK_ERR_BUFFER_SZ;
}

const char *telit_helper_get_resp_ctx(
        uint8_t svc,
        uint8_t *rsp,
        uint32_t len,
        unpack_qmi_t *ctx)
{
    const char *ret = qmi_msg_unknown;
    QMI_raw_message_header_t *hdr_message;
    uint16_t remainder = 0;
    uint16_t hdr_sz = 0;

    memset(ctx, 0, sizeof(*ctx));

    if (svc == QMI_SVC_CONTROL) {
        hdr_sz = sizeof(QMI_CTL_raw_transaction_header_t);
    } else {
        hdr_sz = sizeof(QMI_service_raw_transaction_header_t);
    }

    if (len < hdr_sz + \
            sizeof(*hdr_message)) {
        goto end;
    }

    hdr_message = (QMI_raw_message_header_t *)(rsp + hdr_sz);

    if (svc == QMI_SVC_CONTROL) {
        QMI_CTL_raw_transaction_header_t *hdr = (QMI_CTL_raw_transaction_header_t *)rsp;

        RLOGD("%s: ctrl flags %u, trans id 0x%x, msg id 0x%x, len %u", __FUNCTION__,
                hdr->control_flags, le16toh(hdr->transaction_id),
                le16toh(hdr_message->msg_id), le16toh(hdr_message->len));

        ctx->type = (qmi_msg_type_t)(rsp[0] << 1);
        ctx->xid = hdr->transaction_id;
        ctx->msg_id = le16toh(hdr_message->msg_id);
    } else {
        QMI_service_raw_transaction_header_t *hdr = (QMI_service_raw_transaction_header_t *)rsp;

        RLOGD("%s: svc 0x%x, rsp %u, ind %u, trans id 0x%x, msg id 0x%x, len %u", __FUNCTION__,
                svc, hdr->response, hdr->indication, le16toh(hdr->transaction_id),
                le16toh(hdr_message->msg_id), le16toh(hdr_message->len));

        ctx->type = (qmi_msg_type_t)rsp[0];
        ctx->xid = le16toh(hdr->transaction_id);
        ctx->msg_id = le16toh(hdr_message->msg_id);
    }

    /* Do we have the complete qmi response? */
    remainder = (len - hdr_sz - \
            sizeof(QMI_raw_message_header_t) - le16toh(hdr_message->len));
    if (remainder == 0) {
        ret = (ctx->type == QMI_MSG_RESPONSE ? qmi_msg_response : qmi_msg_indication);
    } else {
        RLOGE("%s: malformed response, remainder %u", __FUNCTION__, remainder);
    }

end:
    return ret;
}

QMI_SDK_error_t telit_helper_qmux_get_svc_cli_id(uint8_t *rsp, uint32_t len,
        uint8_t *svc, uint8_t *cid)
{
    QMI_qmux_header_t *hdr;
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;

    if (cid == NULL || svc == NULL) {
        RLOGE("%s: invalid arg: NULL pointer", __FUNCTION__);
        ret = QMI_SDK_ERR_INVALID_ARG;
        goto end;
    }

    if (len < sizeof(QMI_qmux_header_t)) {
        ret = QMI_SDK_ERR_INVALID_ARG;
        goto end;
    }

    hdr = (QMI_qmux_header_t *)rsp;
    *svc = hdr->service_type;
    *cid = hdr->client_id;

    RLOGD("%s: service_type = %u, client_id = %u", __FUNCTION__,
            hdr->service_type, hdr->client_id);

end:
    return ret;
}

const char *telit_get_version()
{
    return TELIT_SDK_VERSION;
}
