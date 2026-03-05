#include <assert.h>
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
#include "telit_sdk_private_gas.h"
#include "telit_sdk_gas.h"

/*****************************************************************************
 * QMI_GAS_DMS_USB_CFG_SET (515 - 0x0203)
 */

QMI_SDK_error_t telit_gas_dms_usb_cfg_set_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gas_dms_usb_cfg_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GAS_DMS_generic_usb_cfg_pid_t *tlv_0x01;
    QMI_GAS_DMS_generic_usb_cfg_hsic_t *tlv_0x02;
    QMI_GAS_DMS_generic_usb_cfg_persistence_t *tlv_0x03;
    QMI_GAS_DMS_generic_usb_cfg_immediate_t *tlv_0x04;
    QMI_GAS_DMS_generic_usb_cfg_reboot_t *tlv_0x05;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_GAS_DMS_USB_CFG_SET;
    req_ctx->svc_id = QMI_SVC_GAS;
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

    tlv_0x01 = (QMI_GAS_DMS_generic_usb_cfg_pid_t *)buf_position;
    COPY_HTOM(tlv_0x01->pid, input->pid);

    buf_position += tlv_sz;

    tlv_sz = sizeof(*tlv_0x02);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_GAS_DMS_generic_usb_cfg_hsic_t *)buf_position;
    COPY_HTOM(tlv_0x02->hsic, input->hsic);

    buf_position += tlv_sz;

    tlv_sz = sizeof(*tlv_0x03);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x03;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x03 = (QMI_GAS_DMS_generic_usb_cfg_persistence_t *)buf_position;
    tlv_0x03->persistence = input->persistence;

    buf_position += tlv_sz;

    tlv_sz = sizeof(*tlv_0x04);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x04;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x04 = (QMI_GAS_DMS_generic_usb_cfg_immediate_t *)buf_position;
    tlv_0x04->immediate = input->immediate;

    buf_position += tlv_sz;

    tlv_sz = sizeof(*tlv_0x05);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x05;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x05 = (QMI_GAS_DMS_generic_usb_cfg_reboot_t *)buf_position;
    tlv_0x05->reboot = input->reboot;

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gas_dms_usb_cfg_set_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GAS_DMS_USB_CFG_GET (516 - 0x0204)
 */

QMI_SDK_error_t telit_gas_dms_usb_cfg_get_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GAS_DMS_USB_CFG_GET,
            QMI_SVC_GAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gas_dms_usb_cfg_get_unpack(uint8_t *rsp, uint16_t len, gas_dms_usb_cfg_info_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GAS_DMS_generic_usb_cfg_pid_t *tlv_0x03;
    QMI_GAS_DMS_generic_usb_cfg_hsic_t *tlv_0x04;
    QMI_GAS_DMS_generic_usb_cfg_persistence_t *tlv_0x05;
    QMI_GAS_DMS_generic_usb_cfg_immediate_t *tlv_0x06;
    QMI_GAS_DMS_generic_usb_cfg_reboot_t *tlv_0x07;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_generic_usb_cfg_pid_t);

    hdr++;
    tlv_0x03 = (QMI_GAS_DMS_generic_usb_cfg_pid_t *)hdr;
    COPY_MTOH(out->pid, tlv_0x03->pid);

    FIND_MANDATORY_TLV(msg, 0x04, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_generic_usb_cfg_hsic_t);

    hdr++;
    tlv_0x04 = (QMI_GAS_DMS_generic_usb_cfg_hsic_t *)hdr;
    COPY_MTOH(out->hsic, tlv_0x04->hsic);

    FIND_MANDATORY_TLV(msg, 0x05, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_generic_usb_cfg_persistence_t);

    hdr++;
    tlv_0x05 = (QMI_GAS_DMS_generic_usb_cfg_persistence_t *)hdr;
    out->persistence = tlv_0x05->persistence;

    FIND_MANDATORY_TLV(msg, 0x06, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_generic_usb_cfg_immediate_t);

    hdr++;
    tlv_0x06 = (QMI_GAS_DMS_generic_usb_cfg_immediate_t *)hdr;
    out->immediate = tlv_0x06->immediate;

    FIND_MANDATORY_TLV(msg, 0x07, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_generic_usb_cfg_reboot_t);

    hdr++;
    tlv_0x07 = (QMI_GAS_DMS_generic_usb_cfg_reboot_t *)hdr;
    out->reboot = tlv_0x07->reboot;

end:
    return ret;
}

/*****************************************************************************
 * QMI_GAS_DMS_ACTIVE_FW (518 - 0x0206)
 */

QMI_SDK_error_t telit_gas_dms_active_fw_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gas_dms_active_fw_t *input)
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

    req_ctx->msg_id = QMI_GAS_DMS_ACTIVE_FW;
    req_ctx->svc_id = QMI_SVC_GAS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    /* Add TLV 0x10 */
    if (input->carrier_name_available) {
        QMI_GAS_DMS_generic_fw_string_t *tlv_0x10;
        uint8_t *carrier_name = NULL;

        tlv_sz = sizeof(*tlv_0x10) + input->carrier_name.name_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_GAS_DMS_generic_fw_string_t *)buf_position;
        COPY_HTOM(tlv_0x10->len, input->carrier_name.name_len);
        carrier_name = (uint8_t *)(&(tlv_0x10->len));
        carrier_name += sizeof(tlv_0x10->len);
        memcpy(carrier_name, input->carrier_name.name, input->carrier_name.name_len);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x11 */
    if (input->slot_index_available) {
        QMI_GAS_DMS_generic_fw_slot_index_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_GAS_DMS_generic_fw_slot_index_t *)buf_position;
        COPY_HTOM(tlv_0x11->slot_index, input->slot_index);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x12 */
    if (input->version_available) {
        QMI_GAS_DMS_generic_fw_string_t *tlv_0x12;
        uint8_t *version = NULL;

        tlv_sz = sizeof(*tlv_0x12) + input->version.versions_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_GAS_DMS_generic_fw_string_t *)buf_position;
        COPY_HTOM(tlv_0x12->len, input->version.versions_len);
        version = (uint8_t *)(&(tlv_0x12->len));
        version += sizeof(tlv_0x12->len);
        memcpy(version, input->version.versions, input->version.versions_len);

        buf_position += tlv_sz;
    }

    /* pack optional TLV 0x13 */
    if (input->auto_sim_enable_available) {
        QMI_GAS_DMS_ACTIVE_FW_request_0x13_t *tlv_0x13;

        tlv_sz = sizeof(*tlv_0x13);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x13 = (QMI_GAS_DMS_ACTIVE_FW_request_0x13_t *)buf_position;
        COPY_HTOM(tlv_0x13->auto_sim_enable, input->auto_sim_enable);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gas_dms_active_fw_unpack(uint8_t *rsp, uint16_t len __unused)
{
    /* this is a special unpack because this command may return
     * a qmi error code even if the result code is SUCCESS */

    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    uint16_t payload_len;
    uint8_t *msg_tmp;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t *tlv_0x02;
    uint16_t error = 0;
    uint16_t qmi_error = 0;
    uint32_t tlv_value;

    if (!rsp) {
        RLOGE("%s: invalid arg: response expected", __FUNCTION__);
        goto end;
    }

    payload_len = get_payload_size(rsp);

    msg_tmp = rsp + sizeof(QMI_service_raw_transaction_header_t) +
            sizeof(QMI_raw_message_header_t);

    hdr = search_TLV(msg_tmp, 0x02, payload_len);
    if (!hdr) {
        RLOGE("%s: mandatory TLV missing\n", __FUNCTION__);
        ret = QMI_SDK_ERR_MALFORMED_RSP;
        goto end;
    }
    hdr++;
    tlv_0x02 = (uint32_t *)hdr;
    tlv_value = le32toh(*tlv_0x02);

    error = ((tlv_value) & 0x0000FFFF);
    qmi_error = ((((tlv_value) & 0xFFFF0000)) >> 16);

    if (error || qmi_error) {
        RLOGE("%s: QMI_RESULT_FAILURE", __FUNCTION__);
        RLOGE("%s: qmi protocol error 0x%04X (%u): %s",
                __FUNCTION__, qmi_error, qmi_error, qmi_strerror(qmi_error));
        ret = qmi_error + QMI_SDK_ERR_ERR_QMI_OFFSET;
    } else {
        ret = QMI_SDK_ERR_NONE;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GAS_DMS_GET_FW (520 - 0x0208)
 */

static QMI_SDK_error_t gas_dms_select_fw_pack(const uint16_t msg_id,
        pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gas_dms_get_fw_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GAS_DMS_generic_fw_operate_mode_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = msg_id;
    req_ctx->svc_id = QMI_SVC_GAS;
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

    tlv_0x01 = (QMI_GAS_DMS_generic_fw_operate_mode_t *)buf_position;
    COPY_HTOM(tlv_0x01->operate_mode, input->operate_mode);

    buf_position += tlv_sz;

    /* Add TLV 0x10 */
    if (input->slot_index_available) {
        QMI_GAS_DMS_generic_fw_slot_index_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_GAS_DMS_generic_fw_slot_index_t *)buf_position;
        COPY_HTOM(tlv_0x10->slot_index, input->slot_index);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x11 */
    if (input->carrier_name_available) {
        QMI_GAS_DMS_generic_fw_string_t *tlv_0x11;
        uint8_t *carrier_name = NULL;

        tlv_sz = sizeof(*tlv_0x11) + input->carrier_name.name_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_GAS_DMS_generic_fw_string_t *)buf_position;
        COPY_HTOM(tlv_0x11->len, input->carrier_name.name_len);
        carrier_name = (uint8_t *)(&(tlv_0x11->len));
        carrier_name += sizeof(tlv_0x11->len);
        memcpy(carrier_name, input->carrier_name.name, input->carrier_name.name_len);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x12 */
    if (input->version_available) {
        QMI_GAS_DMS_generic_fw_string_t *tlv_0x12;
        uint8_t *version = NULL;

        tlv_sz = sizeof(*tlv_0x12) + input->version.versions_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_GAS_DMS_generic_fw_string_t *)buf_position;
        COPY_HTOM(tlv_0x12->len, input->version.versions_len);
        version = (uint8_t *)(&(tlv_0x12->len));
        version += sizeof(tlv_0x12->len);
        memcpy(version, input->version.versions, input->version.versions_len);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gas_dms_get_fw_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gas_dms_get_fw_t *input)
{
    return gas_dms_select_fw_pack(QMI_GAS_DMS_GET_FW,
            req_ctx,
            req,
            len,
            input);
}

static uint8_t get_chipset_codes(const char *version, char *model)
{
    uint8_t i;
    static struct {
        char *version_pattern;
        uint8_t sku;
        char *model;
    } chipset_map [] = {
        {"24.", 24, "LM940"},
        {"32.", 32, "LM960"},
        {NULL, 0, NULL},
    };

    for (i = 0; chipset_map[i].version_pattern != NULL; i++) {
        if (strncmp(version, chipset_map[i].version_pattern, 3) == 0) {
            strlcpy(model, chipset_map[i].model, QMI_GAS_MODEL_STR_LEN);
            return chipset_map[i].sku;
        }
    }

    return 0;
}

static uint32_t get_carrier_code(const char *carrier_name)
{
    uint8_t i;
    static struct {
        char *carrier_name;
        QMI_GAS_carrier_t id;
    } carrier_map [] = {
        {"Generic", QMI_GAS_CARRIER_GENERIC},
        {"Verizon", QMI_GAS_CARRIER_VERIZON},
        {"Sprint", QMI_GAS_CARRIER_SPRINT},
        {"ATT", QMI_GAS_CARRIER_ATT},
        {NULL, QMI_GAS_CARRIER_NONE},
    };

    for (i = 0; carrier_map[i].carrier_name != NULL; i++) {
        if (strncmp(carrier_map[i].carrier_name, carrier_name, QMI_GAS_FW_STRING_MAX_LEN) == 0) {
            return carrier_map[i].id;
        }
    }

    return QMI_GAS_CARRIER_NONE;
}

QMI_SDK_error_t telit_gas_dms_get_fw_unpack(uint8_t *rsp,
        uint16_t len,
        gas_dms_fw_info_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GAS_DMS_generic_fw_operate_mode_t *tlv_0x01;
    uint8_t *skip = NULL;
    uint8_t i;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_generic_fw_operate_mode_t);

    hdr++;
    tlv_0x01 = (QMI_GAS_DMS_generic_fw_operate_mode_t *)hdr;
    COPY_MTOH(out->operate_mode, tlv_0x01->operate_mode);

    for (i = 0; i < QMI_GAS_FW_MAX_NUMBER; i++) {
        hdr = search_TLV(msg, 0x10 + i, payload_len);
        if (hdr) {
            uint8_t sku;
            int32_t carrier_code;
            QMI_GAS_DMS_generic_fw_info_t *info_tlv;

            CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_generic_fw_info_t);

            hdr++;
            info_tlv = (QMI_GAS_DMS_generic_fw_info_t *)hdr;
            COPY_MTOH(out->fw_info[out->available_firmware].index, info_tlv->index);
            COPY_MTOH(out->fw_info[out->available_firmware].carrier_name.name_len, info_tlv->name_len);

            skip = (uint8_t *)(&(info_tlv->name_len));
            skip += sizeof(info_tlv->name_len);
            memcpy(out->fw_info[out->available_firmware].carrier_name.name, skip,
                    out->fw_info[out->available_firmware].carrier_name.name_len);
            skip += out->fw_info[out->available_firmware].carrier_name.name_len;

            carrier_code = get_carrier_code(out->fw_info[out->available_firmware].carrier_name.name);
            snprintf(out->fw_info[out->available_firmware].carrier_str, QMI_GAS_MAX_CARRIER_STR_LEN + 1,
                    "%u", carrier_code);
            out->fw_info[out->available_firmware].carrier_str_len =
                    strlen(out->fw_info[out->available_firmware].carrier_str);

            ec_mtoh(&(out->fw_info[out->available_firmware].version.versions_len),
                    sizeof(out->fw_info[out->available_firmware].version.versions_len),
                    skip, sizeof(*skip));

            skip++;
            memcpy(out->fw_info[out->available_firmware].version.versions, skip,
                    out->fw_info[out->available_firmware].version.versions_len);
            skip += out->fw_info[out->available_firmware].version.versions_len;

            sku = get_chipset_codes(out->fw_info[out->available_firmware].version.versions,
                    out->fw_info[out->available_firmware].model);
            assert(sku < 100);
            if (sku) {
                snprintf(out->fw_info[out->available_firmware].sku, QMI_GAS_SKU_STR_LEN + 1, "%d", sku);
                out->fw_info[out->available_firmware].sku_len =
                        strlen(out->fw_info[out->available_firmware].sku);
                out->fw_info[out->available_firmware].model_len =
                        strlen(out->fw_info[out->available_firmware].model);
            } else {
                RLOGE("%s: could not get sku and model from version", __FUNCTION__);
            }

            ec_mtoh(&(out->fw_info[out->available_firmware].pri_rev.pri_rev_len),
                    sizeof(out->fw_info[out->available_firmware].pri_rev.pri_rev_len),
                    skip, sizeof(*skip));

            skip++;
            memcpy(out->fw_info[out->available_firmware].pri_rev.pri_rev, skip,
                    out->fw_info[out->available_firmware].pri_rev.pri_rev_len);

            out->available_firmware++;
        }
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    if (hdr) {
        QMI_GAS_DMS_GET_FW_response_0x14_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_GET_FW_response_0x14_t);

        hdr++;
        tlv_0x14 = (QMI_GAS_DMS_GET_FW_response_0x14_t *)hdr;
        COPY_MTOH(out->auto_sim_status, tlv_0x14->auto_sim_status);
        out->auto_sim_status_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GAS_DMS_CLEAR_FW (521 - 0x0209)
 */

QMI_SDK_error_t telit_gas_dms_clear_fw_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gas_dms_get_fw_t *input)
{
    return gas_dms_select_fw_pack(QMI_GAS_DMS_CLEAR_FW,
            req_ctx,
            req,
            len,
            input);
}

QMI_SDK_error_t telit_gas_dms_clear_fw_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GAS_DMS_RESET_INFO_IND_REG (522 - 0x020A)
 */

QMI_SDK_error_t telit_gas_dms_reset_info_ind_reg_pack(pack_qmi_t *req_ctx, uint8_t *req,
        uint16_t *len,
        gas_dms_reset_info_indication_register_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GAS_DMS_SET_RESET_INFO_IND_REGISTER_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_GAS_DMS_RESET_INFO_IND_REG;
    req_ctx->svc_id = QMI_SVC_GAS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->reset_info_enable != QMI_IND_ACTION_NONE) {
        tlv_sz = sizeof(*tlv_0x01);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x01;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x01 = (QMI_GAS_DMS_SET_RESET_INFO_IND_REGISTER_request_0x01_t *)buf_position;
        tlv_0x01->reset_info_enable = input->reset_info_enable;

        add_header(req_ctx, payload_len, original_buffer, len);
    } else {
        RLOGE("%s: invalid arg", __FUNCTION__);
    }

end:
    return ret;
}

QMI_SDK_error_t telit_gas_dms_reset_info_ind_reg_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GAS_DMS_GET_RESET_INFO (523 - 0x020B)
 */

QMI_SDK_error_t telit_gas_dms_get_reset_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GAS_DMS_GET_RESET_INFO,
            QMI_SVC_GAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gas_dms_get_reset_info_unpack(uint8_t *rsp, uint16_t len, gas_dms_reset_info_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GAS_DMS_generic_reset_info_t *tlv_0x03;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_generic_reset_info_t);

    hdr++;
    tlv_0x03 = (QMI_GAS_DMS_generic_reset_info_t *)hdr;
    COPY_MTOH(out->type, tlv_0x03->type);
    COPY_MTOH(out->source, tlv_0x03->source);

end:
    return ret;
}

/*****************************************************************************
 * QMI_GAS_DMS_SET_CRASH_ACTION (524 - 0x020C)
 */

QMI_SDK_error_t telit_gas_dms_set_crash_action_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gas_dms_crash_action_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GAS_DMS_generic_crash_action_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_GAS_DMS_SET_CRASH_ACTION;
    req_ctx->svc_id = QMI_SVC_GAS;
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

    tlv_0x01 = (QMI_GAS_DMS_generic_crash_action_t *)buf_position;
    COPY_HTOM(tlv_0x01->crash_action, input->crash_action);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gas_dms_set_crash_action_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GAS_DMS_GET_CRASH_INFO (525 - 0x020D)
 */

QMI_SDK_error_t telit_gas_dms_get_crash_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        gas_dms_get_crash_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GAS_DMS_GET_CRASH_INFO_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_GAS_DMS_GET_CRASH_INFO;
    req_ctx->svc_id = QMI_SVC_GAS;
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

    tlv_0x01 = (QMI_GAS_DMS_GET_CRASH_INFO_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->clear, input->clear);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gas_dms_get_crash_info_unpack(uint8_t *rsp, uint16_t len,
        gas_dms_get_crash_info_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GAS_DMS_GET_CRASH_INFO_response_0x03_t *tlv_0x03;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_GET_CRASH_INFO_response_0x03_t);

    hdr++;
    tlv_0x03 = (QMI_GAS_DMS_GET_CRASH_INFO_response_0x03_t *)hdr;
    COPY_MTOH(out->crash_status, tlv_0x03->crash_status);

    hdr = search_TLV(msg, 0x10, payload_len);
    if (hdr) {
        QMI_GAS_DMS_GET_CRASH_INFO_response_0x10_t *tlv_0x10;
        uint8_t *buf_position;

        CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_GET_CRASH_INFO_response_0x10_t);

        out->crash_information_available = true;

        hdr++;
        buf_position = (uint8_t *)hdr;

        ec_mtoh(&(out->crash_info.excep_info_sw_ver_len), sizeof(out->crash_info.excep_info_sw_ver_len),
                buf_position, sizeof(out->crash_info.excep_info_sw_ver_len));
        if (out->crash_info.excep_info_sw_ver_len > QMI_GAS_SHORT_STRING_SIZE) {
            RLOGE("%s: crash_info.excep_info_sw_ver is too long (%u)",
                    __FUNCTION__, out->crash_info.excep_info_sw_ver_len);
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }
        buf_position++;
        memcpy(out->crash_info.excep_info_sw_ver, buf_position, out->crash_info.excep_info_sw_ver_len);
        buf_position += out->crash_info.excep_info_sw_ver_len;

        ec_mtoh(&(out->crash_info.excep_info_date_len), sizeof(out->crash_info.excep_info_date_len),
                buf_position, sizeof(out->crash_info.excep_info_date_len));
        if (out->crash_info.excep_info_date_len > QMI_GAS_SHORT_STRING_SIZE) {
            RLOGE("%s: crash_info.excep_info_date is too long (%u)",
                    __FUNCTION__, out->crash_info.excep_info_date_len);
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }
        buf_position++;
        memcpy(out->crash_info.excep_info_date, buf_position, out->crash_info.excep_info_date_len);
        buf_position += out->crash_info.excep_info_date_len;

        ec_mtoh(&(out->crash_info.excep_info_time_len), sizeof(out->crash_info.excep_info_time_len),
                buf_position, sizeof(out->crash_info.excep_info_time_len));
        if (out->crash_info.excep_info_time_len > QMI_GAS_SHORT_STRING_SIZE) {
            RLOGE("%s: crash_info.excep_info_time is too long (%u)",
                    __FUNCTION__, out->crash_info.excep_info_time_len);
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }
        buf_position++;
        memcpy(out->crash_info.excep_info_time, buf_position, out->crash_info.excep_info_time_len);
        buf_position += out->crash_info.excep_info_time_len;

        ec_mtoh(&(out->crash_info.excep_info_line), sizeof(out->crash_info.excep_info_line),
                buf_position, sizeof(out->crash_info.excep_info_line));
        buf_position += sizeof(tlv_0x10->excep_info_line);

        ec_mtoh(&(out->crash_info.excep_info_filename_len), sizeof(out->crash_info.excep_info_filename_len),
                buf_position, sizeof(out->crash_info.excep_info_filename_len));
        if (out->crash_info.excep_info_filename_len > QMI_GAS_SHORT_STRING_SIZE * 2) {
            RLOGE("%s: crash_info.excep_info_filename is too long (%u)",
                    __FUNCTION__, out->crash_info.excep_info_filename_len);
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }
        buf_position++;
        memcpy(out->crash_info.excep_info_filename, buf_position, out->crash_info.excep_info_filename_len);
        buf_position += out->crash_info.excep_info_filename_len;

        ec_mtoh(&(out->crash_info.excep_info_msg_len), sizeof(out->crash_info.excep_info_msg_len),
                buf_position, sizeof(out->crash_info.excep_info_msg_len));
        /* No need to check excep_info_msg_len, since it's uint8_t always < MAX_STRING_SIZE */
        buf_position += sizeof(tlv_0x10->excep_info_msg_len);
        memcpy(out->crash_info.excep_info_msg, buf_position, out->crash_info.excep_info_msg_len);
        buf_position += out->crash_info.excep_info_msg_len;

        ec_mtoh(&(out->crash_info.excep_info_crash_total_cnt),
                sizeof(out->crash_info.excep_info_crash_total_cnt),
                buf_position, sizeof(tlv_0x10->excep_info_crash_total_cnt));
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GAS_DMS_ERRGEN (527 - 0x020F)
 */

QMI_SDK_error_t telit_gas_dms_errgen_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GAS_DMS_ERRGEN,
            QMI_SVC_GAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gas_dms_errgen_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GAS_DMS_GET_HOST_FWVER (528 - 0x0210)
 */

QMI_SDK_error_t telit_gas_dms_get_host_fwver_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GAS_DMS_GET_HOST_FWVER,
            QMI_SVC_GAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gas_dms_get_host_fwver_unpack(uint8_t *rsp,
        uint16_t len,
        gas_dms_versions_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    ret = get_string(msg, payload_len, 0x03, 32, out->versions);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->versions_len = strlen(out->versions);

end:
    return ret;
}

/*****************************************************************************
 * QMI_GAS_DMS_GET_OEM_FWVER (529 - 0x0211)
 */

QMI_SDK_error_t telit_gas_dms_get_oem_fwver_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GAS_DMS_GET_OEM_FWVER,
            QMI_SVC_GAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gas_dms_get_oem_fwver_unpack(uint8_t *rsp, uint16_t len,
        gas_dms_get_oem_fwver_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    ret = get_string(rsp, payload_len, 0x03, QMI_GAS_MAX_OEM_FIRMWARE_NAME, out->oem_fwver);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect oem_fwver", __FUNCTION__);
        goto end;
    }
    out->oem_fwver_len = strlen(out->oem_fwver);

    ret = get_string(rsp, payload_len, 0x10, MAX_STRING_SIZE, out->reserved_ver_1);
    if (ret == QMI_SDK_ERR_NONE) {
        out->reserved_ver_1_len = strlen(out->reserved_ver_1);
    }

    ret = get_string(rsp, payload_len, 0x11, MAX_STRING_SIZE, out->reserved_ver_2);
    if (ret == QMI_SDK_ERR_NONE) {
        out->reserved_ver_2_len = strlen(out->reserved_ver_2);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GAS_DMS_GET_CRASH_ACTION (530 - 0x0212)
 */

QMI_SDK_error_t telit_gas_dms_get_crash_action_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GAS_DMS_GET_CRASH_ACTION,
            QMI_SVC_GAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gas_dms_get_crash_action_unpack(uint8_t *rsp, uint16_t len, gas_dms_crash_action_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GAS_DMS_generic_crash_action_t *tlv_0x03;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_generic_crash_action_t);

    hdr++;
    tlv_0x03 = (QMI_GAS_DMS_generic_crash_action_t *)hdr;
    COPY_MTOH(out->crash_action, tlv_0x03->crash_action);

end:
    return ret;
}

/*****************************************************************************
 * QMI_GAS_DMS_SET_USB_SWITCH (531 - 0x0213)
 */

QMI_SDK_error_t telit_gas_dms_set_usb_switch_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        gas_dms_usb_switch_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GAS_DMS_generic_usb_switch_config_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_GAS_DMS_SET_USB_SWITCH;
    req_ctx->svc_id = QMI_SVC_GAS;
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

    tlv_0x01 = (QMI_GAS_DMS_generic_usb_switch_config_t *)buf_position;
    COPY_HTOM(tlv_0x01->config, input->config);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gas_dms_set_usb_switch_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GAS_DMS_GET_USB_SWITCH (532 - 0x0214)
 */

QMI_SDK_error_t telit_gas_dms_get_usb_switch_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GAS_DMS_GET_USB_SWITCH,
            QMI_SVC_GAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gas_dms_get_usb_switch_unpack(uint8_t *rsp, uint16_t len, gas_dms_usb_switch_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GAS_DMS_generic_usb_switch_config_t *tlv_0x03;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_generic_usb_switch_config_t);

    hdr++;
    tlv_0x03 = (QMI_GAS_DMS_generic_usb_switch_config_t *)hdr;

    COPY_MTOH(out->config, tlv_0x03->config);

end:
    return ret;
}

/*****************************************************************************
 * QMI_GAS_DMS_SET_USB_EXT (553 - 0x0229)
 */

QMI_SDK_error_t telit_gas_dms_set_usb_ext_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        gas_dms_usb_ext_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GAS_DMS_generic_usb_ext_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_GAS_DMS_SET_USB_EXT;
    req_ctx->svc_id = QMI_SVC_GAS;
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

    tlv_0x01 = (QMI_GAS_DMS_generic_usb_ext_t *)buf_position;
    COPY_HTOM(tlv_0x01->mode, input->mode);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gas_dms_set_usb_ext_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GAS_DMS_GET_USB_EXT (554 - 0x022A)
 */

QMI_SDK_error_t telit_gas_dms_get_usb_ext_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GAS_DMS_GET_USB_EXT,
            QMI_SVC_GAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gas_dms_get_usb_ext_unpack(uint8_t *rsp, uint16_t len, gas_dms_usb_ext_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GAS_DMS_generic_usb_ext_t *tlv_0x03;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_generic_usb_ext_t);

    hdr++;
    tlv_0x03 = (QMI_GAS_DMS_generic_usb_ext_t *)hdr;

    COPY_MTOH(out->mode, tlv_0x03->mode);

end:
    return ret;
}

/*****************************************************************************
 * QMI_GAS_PSM_GET_WDISA_CFG (58372 - 0xE404)
 */

QMI_SDK_error_t telit_gas_psm_get_wdisa_cfg_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GAS_PSM_GET_WDISA_CFG,
            QMI_SVC_GAS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gas_psm_get_wdisa_cfg_unpack(uint8_t *rsp, uint16_t len, gas_psm_wdisa_cfg_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GAS_PSM_generic_WDISA_config_t *tlv_0x03;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GAS_PSM_generic_WDISA_config_t);

    hdr++;
    tlv_0x03 = (QMI_GAS_PSM_generic_WDISA_config_t *)hdr;

    COPY_MTOH(out->w_disable_cfg, tlv_0x03->w_disable_cfg);

end:
    return ret;
}

/*****************************************************************************
 * QMI_GAS_PSM_SET_WDISA_CFG (58373 - 0xE405)
 */

QMI_SDK_error_t telit_gas_psm_set_wdisa_cfg_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        gas_psm_wdisa_cfg_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GAS_PSM_generic_WDISA_config_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_GAS_PSM_SET_WDISA_CFG;
    req_ctx->svc_id = QMI_SVC_GAS;
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

    tlv_0x01 = (QMI_GAS_PSM_generic_WDISA_config_t *)buf_position;
    COPY_HTOM(tlv_0x01->w_disable_cfg, input->w_disable_cfg);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gas_psm_set_wdisa_cfg_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GAS_LOG_SET_REDIRECT (58624 - 0xE500)
 */

QMI_SDK_error_t telit_gas_log_set_redirect_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gas_log_redirect_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GAS_LOG_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_GAS_LOG_SET_REDIRECT;
    req_ctx->svc_id = QMI_SVC_GAS;
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

    tlv_0x01 = (QMI_GAS_LOG_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->enable_redirect, input->enable_redirect);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gas_log_set_redirect_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI GAS indications
 */

static QMI_SDK_error_t telit_gas_dms_handle_reset_info_ind(uint16_t *payload_len, uint8_t *msg,
        uint8_t *out, uint16_t out_len)
{
    gas_dms_reset_info_t *ev_rep = (gas_dms_reset_info_t *) out;
    QMI_SDK_error_t ret = QMI_SDK_ERR_BUFFER_SZ;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GAS_DMS_generic_reset_info_t *tlv_0x01;

    if (out_len < sizeof(*ev_rep)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        goto end;
    }
    memset(ev_rep, 0, sizeof(*ev_rep));

    FIND_MANDATORY_TLV(msg, 0x01, *payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GAS_DMS_generic_reset_info_t);

    hdr++;
    tlv_0x01 = (QMI_GAS_DMS_generic_reset_info_t *)hdr;
    COPY_MTOH(ev_rep->type, tlv_0x01->type);
    COPY_MTOH(ev_rep->source, tlv_0x01->source);

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

QMI_SDK_error_t telit_gas_indication_handler(uint16_t msg_id,
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
        case QMI_GAS_DMS_RESET_INFO_IND:
            ret = telit_gas_dms_handle_reset_info_ind(&payload_len, msg, out, out_len);
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
