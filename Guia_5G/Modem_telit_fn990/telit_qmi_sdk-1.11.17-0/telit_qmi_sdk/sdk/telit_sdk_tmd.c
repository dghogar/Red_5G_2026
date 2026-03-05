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
#include "telit_sdk_private_tmd.h"
#include "telit_sdk_tmd.h"

/*****************************************************************************
 * QMI_TMD_GET_MITIGATION_DEVICE_LIST (32 - 0x0020)
 */

QMI_SDK_error_t telit_tmd_get_mitigation_device_list_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_TMD_GET_MITIGATION_DEVICE_LIST,
            QMI_SVC_TMD,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_tmd_get_mitigation_device_list_unpack(uint8_t *rsp, uint16_t len,
        tmd_mitigation_device_list_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len = 0;
    uint8_t *msg = NULL;
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
        QMI_TMD_GET_MITIGATION_DEVICE_LIST_response_0x10_t *tlv_0x10;
        tmd_mitigation_device_instance_t *device_instance = NULL;
        uint8_t *skip = NULL;
        uint8_t i = 0;

        CHECK_UNPACK_TLV_SIZE(QMI_TMD_GET_MITIGATION_DEVICE_LIST_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_TMD_GET_MITIGATION_DEVICE_LIST_response_0x10_t *)hdr;
        COPY_MTOH(out->mitigation_device_list_size, tlv_0x10->mitigation_device_list_size);

        if (out->mitigation_device_list_size > QMI_TMD_MAX_MITIGATION_DEVICES) {
            RLOGE("%s: mitigation_device_list_size = %u > QMI_TMD_MAX_MITIGATION_DEVICES)",
                    __FUNCTION__, out->mitigation_device_list_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x10->mitigation_device_list_size));
        skip += sizeof(tlv_0x10->mitigation_device_list_size);
        device_instance = (tmd_mitigation_device_instance_t *)skip;

        for (i = 0; i < out->mitigation_device_list_size; i++) {
            COPY_MTOH(out->mitigation_devices[i].mitigation_dev_id_len,
                    device_instance->mitigation_dev_id_len);

            if (out->mitigation_devices[i].mitigation_dev_id_len > QMI_TMD_MAX_MITIGATION_STRING_SIZE) {
                RLOGE("%s: mitigation_dev_id_len = %u > QMI_TMD_MAX_MITIGATION_STRING_SIZE",
                        __FUNCTION__, out->mitigation_devices[i].mitigation_dev_id_len);
                ret = QMI_SDK_ERR_INTERNAL;
                goto end;
            }

            skip = (uint8_t *)(&(device_instance->mitigation_dev_id_len));
            skip += sizeof(device_instance->mitigation_dev_id_len);
            memcpy(out->mitigation_devices[i].mitigation_dev_id, skip,
                    out->mitigation_devices[i].mitigation_dev_id_len);

            skip += out->mitigation_devices[i].mitigation_dev_id_len;

            out->mitigation_devices[i].max_mitigation_level = *skip;
            out->mitigation_devices[i].max_mitigation_level_available = true;
            skip += sizeof(device_instance->max_mitigation_level);

            device_instance = (tmd_mitigation_device_instance_t *)skip;
        }
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_TMD_GET_MITIGATION_LEVEL (34 - 0x0022)
 */

QMI_SDK_error_t telit_tmd_get_mitigation_level_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        tmd_mitigation_device_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_TMD_GET_MITIGATION_LEVEL_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;
    uint8_t *skip = NULL;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    if (input->mitigation_dev_id_len !=
            strnlen(input->mitigation_dev_id, QMI_TMD_MAX_MITIGATION_STRING_SIZE)) {
        RLOGW("%s: Check length %d, actual mitigation_dev_id length: %zu",
                __FUNCTION__, input->mitigation_dev_id_len,
                strnlen(input->mitigation_dev_id, QMI_TMD_MAX_MITIGATION_STRING_SIZE));
    }

    req_ctx->msg_id = QMI_TMD_GET_MITIGATION_LEVEL;
    req_ctx->svc_id = QMI_SVC_TMD;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = input->mitigation_dev_id_len + sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_TMD_GET_MITIGATION_LEVEL_request_0x01_t *)(buf_position);
    COPY_HTOM(tlv_0x01->mitigation_dev_id_len, input->mitigation_dev_id_len);

    skip = (uint8_t *)(&(tlv_0x01->mitigation_dev_id_len));
    skip += sizeof(tlv_0x01->mitigation_dev_id_len);
    memcpy(skip, input->mitigation_dev_id, input->mitigation_dev_id_len);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_tmd_get_mitigation_level_unpack(uint8_t *rsp, uint16_t len,
        tmd_mitigation_level_t *out)
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
        QMI_TMD_GET_MITIGATION_LEVEL_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_TMD_GET_MITIGATION_LEVEL_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_TMD_GET_MITIGATION_LEVEL_response_0x10_t *)hdr;
        COPY_MTOH(out->current_mitigation_level, tlv_0x10->current_mitigation_level);
        out->current_mitigation_level_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_TMD_GET_MITIGATION_LEVEL_response_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_TMD_GET_MITIGATION_LEVEL_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_TMD_GET_MITIGATION_LEVEL_response_0x11_t *)hdr;
        COPY_MTOH(out->requested_mitigation_level, tlv_0x11->requested_mitigation_level);
        out->requested_mitigation_level_available = true;
    }

end:
    return ret;
}
