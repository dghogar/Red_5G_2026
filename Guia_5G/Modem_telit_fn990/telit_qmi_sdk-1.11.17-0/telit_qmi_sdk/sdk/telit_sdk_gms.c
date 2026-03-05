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
#include "telit_sdk_private_gms.h"
#include "telit_sdk_gms.h"

/*****************************************************************************
 * QMI_GMS_WDS_3GPP_CONFIG_ITEM_SET (256 - 0x0100)
 */

QMI_SDK_error_t telit_gms_wds_3gpp_config_item_set_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_3gpp_config_item_t *input)
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

    if (input->attach_LTE_profile_list_size > QMI_GMS_LTE_ATTACH_PROF_LIST_MAX_SIZE) {
        RLOGE("%s: attach_LTE_profile_list_size = %u > QMI_GMS_LTE_ATTACH_PROF_LIST_MAX_SIZE, "
                "increase the define", __FUNCTION__, input->attach_LTE_profile_list_size);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    req_ctx->msg_id = QMI_GMS_WDS_3GPP_CONFIG_ITEM_SET;
    req_ctx->svc_id = QMI_SVC_GMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->lte_attach_profile_available) {
        QMI_GMS_WDS_3GPP_CONFIG_generic_lte_profile_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_GMS_WDS_3GPP_CONFIG_generic_lte_profile_t *)buf_position;
        COPY_HTOM(tlv_0x10->lte_attach_profile, input->lte_attach_profile);

        buf_position += tlv_sz;
    }

    if (input->profile_list_size > 0) {
        QMI_GMS_WDS_3GPP_CONFIG_generic_profile_list_t *tlv_0x11;
        uint8_t i;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_GMS_WDS_3GPP_CONFIG_generic_profile_list_t *)buf_position;
        /* NOTE:
         * FW version 32.00.0X2.1-B001 checks that all and each profile_list entries are in
         * the valid range 1~24, even those entries unmodified by the caller (e.g. when
         * profile_list_size is < QMI_GMS_PROF_LIST_MAX_SIZE).
         * To avoid a modem error on a valid request, we initialize all the profile_list
         * entries to a dummy value "1", which will be overwritten if len is
         * QMI_GMS_PROF_LIST_MAX_SIZE or ignored by the modem otherwise.
         */
        for (i = 1; i < QMI_GMS_PROF_LIST_MAX_SIZE; i++) {
            uint16_t dummy = 0x0001;
            COPY_HTOM(tlv_0x11->profile_list[i], dummy);
        }
        COPY_HTOM(tlv_0x11->profile_list[0], input->profile_list_size);
        for (i = 1; i <= input->profile_list_size; i++) {
            COPY_HTOM(tlv_0x11->profile_list[i], input->profile_list[i - 1]);
        }

        buf_position += tlv_sz;
    }

    if (input->default_PDN_enabled_available) {
        QMI_GMS_WDS_3GPP_CONFIG_generic_default_pdn_enabled_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_GMS_WDS_3GPP_CONFIG_generic_default_pdn_enabled_t *)buf_position;
        COPY_HTOM(tlv_0x12->default_PDN_enabled, input->default_PDN_enabled);

        buf_position += tlv_sz;
    }

    if (input->release_3gpp_available) {
        QMI_GMS_WDS_3GPP_CONFIG_generic_3gpp_release_t *tlv_0x13;

        tlv_sz = sizeof(*tlv_0x13);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x13 = (QMI_GMS_WDS_3GPP_CONFIG_generic_3gpp_release_t *)buf_position;
        COPY_HTOM(tlv_0x13->release_3gpp, input->release_3gpp);

        buf_position += tlv_sz;
    }

    if (input->attach_LTE_profile_list_size) {
        QMI_GMS_WDS_3GPP_CONFIG_generic_lte_attach_profile_list_t *tlv_0x14;
        uint16_t *attach_LTE_profile_list_item;
        uint8_t i;

        tlv_sz = sizeof(*tlv_0x14) + (input->attach_LTE_profile_list_size * 2);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x14;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x14 = (QMI_GMS_WDS_3GPP_CONFIG_generic_lte_attach_profile_list_t *)buf_position;
        COPY_HTOM(tlv_0x14->attach_LTE_profile_list_size, input->attach_LTE_profile_list_size);
        buf_position = &(tlv_0x14->attach_LTE_profile_list_size);
        buf_position += sizeof(tlv_0x14->attach_LTE_profile_list_size);
        attach_LTE_profile_list_item = (uint16_t *)buf_position;
        for (i = 0; i < input->attach_LTE_profile_list_size; i++) {
            COPY_HTOM(attach_LTE_profile_list_item[i], input->attach_LTE_profile_list[i]);
        }
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gms_wds_3gpp_config_item_set_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GMS_WDS_3GPP_CONFIG_ITEM_GET (257 - 0x0101)
 */

QMI_SDK_error_t telit_gms_wds_3gpp_config_item_get_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_WDS_3GPP_CONFIG_ITEM_GET,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_wds_3gpp_config_item_get_unpack(uint8_t *rsp,
        uint16_t len,
        wds_3gpp_config_item_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GMS_WDS_3GPP_CONFIG_generic_lte_profile_t *tlv_0x03;
    QMI_GMS_WDS_3GPP_CONFIG_generic_profile_list_t *tlv_0x04;
    QMI_GMS_WDS_3GPP_CONFIG_generic_default_pdn_enabled_t *tlv_0x05;
    QMI_GMS_WDS_3GPP_CONFIG_generic_3gpp_release_t *tlv_0x06;
    QMI_GMS_WDS_3GPP_CONFIG_generic_lte_attach_profile_list_t *tlv_0x07;
    uint8_t *buf_position;
    uint8_t i;
    uint16_t *profile_tmp;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_WDS_3GPP_CONFIG_generic_lte_profile_t);

    hdr++;
    tlv_0x03 = (QMI_GMS_WDS_3GPP_CONFIG_generic_lte_profile_t *)hdr;
    COPY_MTOH(out->lte_attach_profile, tlv_0x03->lte_attach_profile);

    FIND_MANDATORY_TLV(msg, 0x04, payload_len, hdr);

    hdr++;
    tlv_0x04 = (QMI_GMS_WDS_3GPP_CONFIG_generic_profile_list_t *)hdr;
    COPY_MTOH(out->profile_list_size, tlv_0x04->profile_list[0]);
    if (out->profile_list_size != 0xFFFF) {
        if (out->profile_list_size > QMI_GMS_PROF_LIST_MAX_SIZE) {
            /* This is a malformed response, since dictated by the protocol (see doc) */
            RLOGE("%s: malformed TLV, profile list len %u, maximum %u",
                    __FUNCTION__, out->profile_list_size, QMI_GMS_PROF_LIST_MAX_SIZE);
            ret = QMI_SDK_ERR_MALFORMED_RSP;
            goto end;
        }

        /* Starting from 1, since 0 is profile list length */
        for (i = 0; i < out->profile_list_size; i++) {
            COPY_MTOH(out->profile_list[i], tlv_0x04->profile_list[i + 1]);
        }
    }

    FIND_MANDATORY_TLV(msg, 0x05, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_WDS_3GPP_CONFIG_generic_default_pdn_enabled_t);

    hdr++;
    tlv_0x05 = (QMI_GMS_WDS_3GPP_CONFIG_generic_default_pdn_enabled_t *)hdr;
    COPY_MTOH(out->default_PDN_enabled, tlv_0x05->default_PDN_enabled);

    FIND_MANDATORY_TLV(msg, 0x06, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_WDS_3GPP_CONFIG_generic_3gpp_release_t);

    hdr++;
    tlv_0x06 = (QMI_GMS_WDS_3GPP_CONFIG_generic_3gpp_release_t *)hdr;
    COPY_MTOH(out->release_3gpp, tlv_0x06->release_3gpp);

    FIND_MANDATORY_TLV(msg, 0x07, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_WDS_3GPP_CONFIG_generic_lte_attach_profile_list_t);

    hdr++;
    tlv_0x07 = (QMI_GMS_WDS_3GPP_CONFIG_generic_lte_attach_profile_list_t *)hdr;
    COPY_MTOH(out->attach_LTE_profile_list_size, tlv_0x07->attach_LTE_profile_list_size);
    if (out->attach_LTE_profile_list_size > QMI_GMS_LTE_ATTACH_PROF_LIST_MAX_SIZE) {
        RLOGE("%s: attach_LTE_profile_list_size = %u > QMI_GMS_LTE_ATTACH_PROF_LIST_MAX_SIZE, "
                "increase the define", __FUNCTION__, out->attach_LTE_profile_list_size);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    buf_position = (uint8_t *)&(tlv_0x07->attach_LTE_profile_list_size);

    buf_position += sizeof(tlv_0x07->attach_LTE_profile_list_size);

    for (i = 0; i < out->attach_LTE_profile_list_size; i++) {
        profile_tmp = (uint16_t *)buf_position;
        ec_mtoh(&(out->attach_LTE_profile_list[i]), sizeof(out->attach_LTE_profile_list[i]),
                profile_tmp, sizeof(*profile_tmp));
        buf_position += 2;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_WDS_CREATE_PROFILE (259 - 0x0103)
 */

QMI_SDK_error_t telit_wds_generic_modify_profile_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_profile_t *input,
        uint32_t msg_type);

QMI_SDK_error_t telit_gms_wds_create_profile_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_profile_t *input)
{
    return telit_wds_generic_modify_profile_pack(req_ctx, req,
            len, input,
            QMI_GMS_WDS_CREATE_PROFILE);
}

QMI_SDK_error_t telit_gms_wds_create_profile_unpack(uint8_t *rsp, uint16_t len,
        wds_profile_resp_t *out)
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
        QMI_GMS_WDS_CREATE_PROFILE_response_0x03_t *tlv_0x03;
        QMI_GMS_WDS_CREATE_PROFILE_response_0x04_t *tlv_0x04;

        FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_WDS_CREATE_PROFILE_response_0x03_t);

        hdr++;
        tlv_0x03 = (QMI_GMS_WDS_CREATE_PROFILE_response_0x03_t *)hdr;
        COPY_MTOH(out->profile_id.type, tlv_0x03->profile_type);

        FIND_MANDATORY_TLV(msg, 0x04, payload_len, hdr);

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_WDS_CREATE_PROFILE_response_0x04_t);

        hdr++;
        tlv_0x04 = (QMI_GMS_WDS_CREATE_PROFILE_response_0x04_t *)hdr;
        COPY_MTOH(out->profile_id.index, tlv_0x04->profile_index);

        out->profile_id_available = true;
    }

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_GMS_WDS_CREATE_PROFILE_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_WDS_CREATE_PROFILE_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_GMS_WDS_CREATE_PROFILE_response_0x10_t *)hdr;
        COPY_MTOH(out->extended_error_code, tlv_0x10->extended_error_code);
        out->extended_error_code_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_WDS_PRESERVED_APN_BACKUP (260 - 0x0104)
 */

QMI_SDK_error_t telit_gms_wds_preserved_apn_backup_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_WDS_PRESERVED_APN_BACKUP,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_wds_preserved_apn_backup_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GMS_DMS_GET_DYING_GASP_CFG (512 - 0x0200)
 */

QMI_SDK_error_t telit_gms_dms_get_dying_gasp_cfg_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_DMS_GET_DYING_GASP_CFG,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_dms_get_dying_gasp_cfg_unpack(uint8_t *rsp, uint16_t len, dms_dying_gasp_cfg_t *out)
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
        QMI_GMS_DMS_SET_DYING_GASP_CFG_generic_cfg_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_DMS_SET_DYING_GASP_CFG_generic_cfg_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_GMS_DMS_SET_DYING_GASP_CFG_generic_cfg_0x10_t *)hdr;
        COPY_MTOH(out->sms_destination_number_len, tlv_0x10->sms_destination_number_len);
        buf_position = &tlv_0x10->sms_destination_number_len;
        /* Moving to the start of the string */
        buf_position += sizeof(tlv_0x10->sms_destination_number_len);

        memcpy(out->sms_destination_number, buf_position, out->sms_destination_number_len);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_GMS_DMS_SET_DYING_GASP_CFG_generic_cfg_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_DMS_SET_DYING_GASP_CFG_generic_cfg_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_GMS_DMS_SET_DYING_GASP_CFG_generic_cfg_0x11_t *)hdr;
        COPY_MTOH(out->sms_content_len, tlv_0x11->sms_content_len);
        buf_position = &tlv_0x11->sms_content_len;
        /* Moving to the start of the string */
        buf_position += sizeof(tlv_0x11->sms_content_len);

        memcpy(out->sms_content, buf_position, out->sms_content_len);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_DMS_SET_DYING_GASP_CFG (513 - 0x0201)
 */

QMI_SDK_error_t telit_gms_dms_set_dying_gasp_cfg_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        dms_dying_gasp_cfg_t *input)
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

    req_ctx->msg_id = QMI_GMS_DMS_SET_DYING_GASP_CFG;
    req_ctx->svc_id = QMI_SVC_GMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->sms_destination_number_len) {
        QMI_GMS_DMS_SET_DYING_GASP_CFG_generic_cfg_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10) + sizeof(uint8_t) * input->sms_destination_number_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_GMS_DMS_SET_DYING_GASP_CFG_generic_cfg_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->sms_destination_number_len, input->sms_destination_number_len);
        buf_position = &tlv_0x10->sms_destination_number_len;
        /* Moving to the start of the string */
        buf_position += sizeof(tlv_0x10->sms_destination_number_len);

        memcpy(buf_position, input->sms_destination_number, input->sms_destination_number_len);

        buf_position += sizeof(uint8_t) * input->sms_destination_number_len;
    }

    if (input->sms_content_len) {
        QMI_GMS_DMS_SET_DYING_GASP_CFG_generic_cfg_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11) + sizeof(uint8_t) * input->sms_content_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_GMS_DMS_SET_DYING_GASP_CFG_generic_cfg_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->sms_content_len, input->sms_content_len);
        buf_position = &tlv_0x11->sms_content_len;
        /* Moving to the start of the string */
        buf_position += sizeof(tlv_0x11->sms_content_len);

        memcpy(buf_position, input->sms_content, input->sms_content_len);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gms_dms_set_dying_gasp_cfg_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GMS_DMS_GET_DYING_GASP_STAT (514 - 0x0202)
 */

QMI_SDK_error_t telit_gms_dms_get_dying_gasp_stat_unpack(uint8_t *rsp, uint16_t len,
        dms_dying_gasp_stat_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GMS_DMS_GET_DYING_GASP_STAT_response_0x03_t *tlv_0x03;
    QMI_GMS_DMS_GET_DYING_GASP_STAT_response_0x04_t *tlv_0x04;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_DMS_GET_DYING_GASP_STAT_response_0x03_t);

    hdr++;
    tlv_0x03 = (QMI_GMS_DMS_GET_DYING_GASP_STAT_response_0x03_t *)hdr;
    COPY_MTOH(out->timestamp, tlv_0x03->timestamp);

    FIND_MANDATORY_TLV(msg, 0x04, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_DMS_GET_DYING_GASP_STAT_response_0x04_t);

    hdr++;
    tlv_0x04 = (QMI_GMS_DMS_GET_DYING_GASP_STAT_response_0x04_t *)hdr;
    COPY_MTOH(out->sms_attempted, tlv_0x04->sms_attempted);

end:
    return ret;
}

QMI_SDK_error_t telit_gms_dms_get_dying_gasp_stat_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_DMS_GET_DYING_GASP_STAT,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

/*****************************************************************************
 * QMI_GMS_DMS_CLR_DYING_GASP_STAT (515 - 0x0203)
 */

QMI_SDK_error_t telit_gms_dms_clr_dying_gasp_stat_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_DMS_CLR_DYING_GASP_STAT,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_dms_clr_dying_gasp_stat_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GMS_DMS_GET_CUST_FEATURES (516 - 0x0204)
 */

QMI_SDK_error_t telit_gms_dms_get_cust_features_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        gms_dms_get_cust_features_v2_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_GMS_DMS_GET_CUST_FEATURES;
    req_ctx->svc_id = QMI_SVC_GMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = input->cust_id_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    memcpy(buf_position, input->cust_id, MAX_STRING_SIZE);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gms_dms_get_cust_features_unpack(uint8_t *rsp, uint16_t len,
        gms_dms_get_cust_features_v2_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GMS_DMS_GET_CUST_FEATURES_V2_response_0x03_t *tlv_0x03;
    QMI_GMS_DMS_GET_CUST_FEATURES_V2_response_0x04_t *tlv_0x04;
    char cust_id_check[MAX_STRING_SIZE] = {0};
    uint8_t cust_id_check_len = 0;
    uint8_t *buf_position;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_DMS_GET_CUST_FEATURES_V2_response_0x03_t);

    hdr++;
    tlv_0x03 = (QMI_GMS_DMS_GET_CUST_FEATURES_V2_response_0x03_t *)hdr;

    COPY_MTOH(cust_id_check_len, tlv_0x03->cust_id_len);
    if (cust_id_check_len >= MAX_STRING_SIZE) {
        RLOGE("%s:cust id in tlv 0x03 is too long (%u, max is %u)",
                __FUNCTION__, cust_id_check_len, MAX_STRING_SIZE - 1);
        ret = QMI_SDK_ERR_MALFORMED_RSP;
        goto end;
    }
    buf_position = &(tlv_0x03->cust_id_len);
    buf_position += sizeof(tlv_0x03->cust_id_len);
    memcpy(cust_id_check, buf_position, cust_id_check_len);

    FIND_MANDATORY_TLV(msg, 0x04, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_DMS_GET_CUST_FEATURES_V2_response_0x04_t);

    hdr++;
    tlv_0x04 = (QMI_GMS_DMS_GET_CUST_FEATURES_V2_response_0x04_t *)hdr;

    COPY_MTOH(out->cust_id_len, tlv_0x04->cust_id_len);
    if (out->cust_id_len >= MAX_STRING_SIZE) {
        RLOGE("%s: cust id in tlv 0x04 is too long (%u, max is %u)",
                __FUNCTION__, out->cust_id_len, MAX_STRING_SIZE - 1);
        ret = QMI_SDK_ERR_MALFORMED_RSP;
        goto end;
    }
    buf_position = (uint8_t *)&(tlv_0x04->cust_id_len);
    buf_position += sizeof(tlv_0x04->cust_id_len);

    memcpy(out->cust_id, buf_position, out->cust_id_len);

    buf_position += out->cust_id_len;

    ec_mtoh(&(out->cust_value_len), sizeof(out->cust_value_len), buf_position, sizeof(out->cust_value_len));
    if (out->cust_value_len > QMI_GMS_MAX_CUST_VALUE_SIZE) {
        /* This is a malformed response, since dictated by the protocol (see doc) */
        RLOGE("%s: cust value in tlv 0x04 is too long (%u, max is %u)",
                __FUNCTION__, out->cust_value_len, QMI_GMS_MAX_CUST_VALUE_SIZE);
        ret = QMI_SDK_ERR_MALFORMED_RSP;
        goto end;
    }
    buf_position++;

    memcpy(out->cust_value, buf_position, out->cust_value_len);

    buf_position += out->cust_value_len;

    ec_mtoh(&(out->cust_attr), sizeof(out->cust_attr), buf_position, sizeof(out->cust_attr));

    if (cust_id_check_len != out->cust_id_len ||
            memcmp(cust_id_check, out->cust_id, cust_id_check_len) != 0) {
        RLOGE("%s: cust_id in tlv 0x03 differs from tlv 0x04", __FUNCTION__);
        RLOGE("%s: tlv 0x03: '%s' (%d)\n", __FUNCTION__, cust_id_check, cust_id_check_len);
        RLOGE("%s: tlv 0x04: '%s' (%d)\n", __FUNCTION__, out->cust_id, out->cust_id_len);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_DMS_SET_CUST_FEATURES (517 - 0x0205)
 */

QMI_SDK_error_t telit_gms_dms_set_cust_features_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        gms_dms_set_cust_features_v2_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GMS_DMS_SET_CUST_FEATURES_V2_request_0x02_t *tlv_0x02;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    if (input->cust_value_len > QMI_GMS_MAX_CUST_VALUE_SIZE) {
        RLOGE("%s: value is too long (max %d bytes allowed)\n",
                __FUNCTION__, QMI_GMS_MAX_CUST_VALUE_SIZE);
        goto end;
    }

    req_ctx->msg_id = QMI_GMS_DMS_SET_CUST_FEATURES;
    req_ctx->svc_id = QMI_SVC_GMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = input->cust_id_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    memcpy(buf_position, input->cust_id, input->cust_id_len);

    buf_position += tlv_sz;

    /* pack mandatory TLV 0x02 */
    tlv_sz = sizeof(*tlv_0x02) + input->cust_value_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_GMS_DMS_SET_CUST_FEATURES_V2_request_0x02_t *)buf_position;

    COPY_HTOM(tlv_0x02->value_length, input->cust_value_len);
    buf_position = (uint8_t *)(&tlv_0x02->value_length);
    buf_position += sizeof(tlv_0x02->value_length);
    memcpy(buf_position, input->cust_value, input->cust_value_len);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gms_dms_set_cust_features_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GMS_DMS_GET_PC_INFO (518 - 0x0206)
 */

QMI_SDK_error_t telit_gms_dms_get_pc_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_DMS_GET_PC_INFO,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_dms_get_pc_info_unpack(uint8_t *rsp, uint16_t len,
        gms_dms_get_pc_info_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GMS_DMS_GET_PC_INFO_response_0x03_t *tlv_0x03;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_DMS_GET_PC_INFO_response_0x03_t);

    hdr++;
    tlv_0x03 = (QMI_GMS_DMS_GET_PC_INFO_response_0x03_t *)hdr;
    COPY_MTOH(out->operating_mode, tlv_0x03->operating_mode);

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_GMS_DMS_GET_PC_INFO_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_DMS_GET_PC_INFO_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_GMS_DMS_GET_PC_INFO_response_0x10_t *)hdr;
        COPY_MTOH(out->lpm_flag, tlv_0x10->lpm_flag);
        out->lpm_flag_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_GMS_DMS_GET_PC_INFO_response_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_DMS_GET_PC_INFO_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_GMS_DMS_GET_PC_INFO_response_0x11_t *)hdr;
        COPY_MTOH(out->w_disable, tlv_0x11->w_disable);
        out->w_disable_available = true;
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_GMS_DMS_GET_PC_INFO_response_0x12_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_DMS_GET_PC_INFO_response_0x12_t);

        hdr++;
        tlv_0x12 = (QMI_GMS_DMS_GET_PC_INFO_response_0x12_t *)hdr;
        COPY_MTOH(out->power_off_mode, tlv_0x12->power_off_mode);
        out->power_off_mode_available = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_GMS_DMS_GET_PC_INFO_response_0x13_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_DMS_GET_PC_INFO_response_0x13_t);

        hdr++;
        tlv_0x13 = (QMI_GMS_DMS_GET_PC_INFO_response_0x13_t *)hdr;
        COPY_MTOH(out->persistent_lpm, tlv_0x13->persistent_lpm);
        out->persistent_lpm_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_DMS_GET_MTU (519 - 0x0207)
 */

QMI_SDK_error_t telit_gms_dms_get_mtu_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_DMS_GET_MTU,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_dms_get_mtu_unpack(uint8_t *rsp, uint16_t len,
        gms_dms_get_mtu_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GMS_DMS_GET_MTU_response_0x03_t *tlv_0x03;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_DMS_GET_MTU_response_0x03_t);

    hdr++;
    tlv_0x03 = (QMI_GMS_DMS_GET_MTU_response_0x03_t *)hdr;
    COPY_MTOH(out->mtu_size_3gpp_available, tlv_0x03->mtu_size_3gpp_available);
    COPY_MTOH(out->mtu_size_3gpp, tlv_0x03->mtu_size_3gpp);
    COPY_MTOH(out->mtu_size_hrpd_available, tlv_0x03->mtu_size_hrpd_available);
    COPY_MTOH(out->mtu_size_hrpd, tlv_0x03->mtu_size_hrpd);
    COPY_MTOH(out->mtu_size_ehrpd_available, tlv_0x03->mtu_size_ehrpd_available);
    COPY_MTOH(out->mtu_size_ehrpd, tlv_0x03->mtu_size_ehrpd);
    COPY_MTOH(out->mtu_size_usb_available, tlv_0x03->mtu_size_usb_available);
    COPY_MTOH(out->mtu_size_usb, tlv_0x03->mtu_size_usb);

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_DMS_SET_MTU (520 - 0x0208)
 */

QMI_SDK_error_t telit_gms_dms_set_mtu_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        gms_dms_set_mtu_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GMS_DMS_SET_MTU_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_GMS_DMS_SET_MTU;
    req_ctx->svc_id = QMI_SVC_GMS;
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

    tlv_0x01 = (QMI_GMS_DMS_SET_MTU_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->size, input->size);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gms_dms_set_mtu_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GMS_DMS_FWSWITCH_INFO_EX (528 - 0x0210)
 */

QMI_SDK_error_t telit_gms_dms_fwswitch_info_ex_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_DMS_FWSWITCH_INFO_EX,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_dms_fwswitch_info_ex_unpack(uint8_t *rsp, uint16_t len,
        gms_dms_fwswitch_info_ex_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GMS_DMS_FWSWITCH_INFO_EX_request_0x03_t *tlv_0x03;
    uint8_t *buf_position;
    uint8_t i;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_DMS_FWSWITCH_INFO_EX_request_0x03_t);

    hdr++;
    tlv_0x03 = (QMI_GMS_DMS_FWSWITCH_INFO_EX_request_0x03_t *)hdr;
    COPY_MTOH(out->info_list_size, tlv_0x03->info_list_len);

    if (out->info_list_size > QMI_GMS_FWSWITCH_INFO_EX_LIST_MAX_SIZE) {
        RLOGE("%s: info_list_size %u > QMI_GMS_FWSWITCH_INFO_EX_LIST_MAX_SIZE, increase the define",
                __FUNCTION__, out->info_list_size);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    buf_position = (uint8_t *)(&(tlv_0x03->info_list_len));
    /* Moving to the start of the array */
    buf_position += sizeof(tlv_0x03->info_list_len);

    for (i = 0; i < out->info_list_size; i++) {
        QMI_GMS_DMS_FWSWITCH_INFO_EX_request_info_instance_t *fwswitch_ex_info =
                (QMI_GMS_DMS_FWSWITCH_INFO_EX_request_info_instance_t *)buf_position;

        COPY_MTOH(out->info_list[i].mno_id, fwswitch_ex_info->mno_id);
        COPY_MTOH(out->info_list[i].mno_name_len, fwswitch_ex_info->mno_name_len);
        buf_position = (uint8_t *)(&(fwswitch_ex_info->mno_name_len));
        buf_position += sizeof(fwswitch_ex_info->mno_name_len);
        memcpy(out->info_list[i].mno_name, buf_position, out->info_list[i].mno_name_len);
        buf_position += out->info_list[i].mno_name_len;
        ec_mtoh(&(out->info_list[i].mno_cfg_ver_len), sizeof(out->info_list[i].mno_cfg_ver_len),
                buf_position, sizeof(out->info_list[i].mno_cfg_ver_len));
        buf_position += sizeof(out->info_list[i].mno_cfg_ver_len);
        memcpy(out->info_list[i].mno_cfg_ver, buf_position, out->info_list[i].mno_cfg_ver_len);
        buf_position += out->info_list[i].mno_cfg_ver_len;
        /* Add the reserved fields */
        buf_position += 8;
        fwswitch_ex_info = (QMI_GMS_DMS_FWSWITCH_INFO_EX_request_info_instance_t *)buf_position;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_NAS_MODEM_STATUS (770 - 0x0302)
 */

QMI_SDK_error_t telit_gms_nas_modem_status_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_NAS_MODEM_STATUS,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_nas_modem_status_unpack(uint8_t *rsp, uint16_t len, nas_modem_status_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GMS_NAS_GET_MODEM_STATUS_response_0x03_t *tlv_0x03;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_MODEM_STATUS_response_0x03_t);

    hdr++;
    tlv_0x03 = (QMI_GMS_NAS_GET_MODEM_STATUS_response_0x03_t *)hdr;
    COPY_MTOH(out->temperature, tlv_0x03->temperature);
    COPY_MTOH(out->modem_mode, tlv_0x03->modem_mode);
    COPY_MTOH(out->system_mode, tlv_0x03->system_mode);
    COPY_MTOH(out->ims_reg_state, tlv_0x03->ims_reg_state);
    COPY_MTOH(out->ps_state, tlv_0x03->ps_state);

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_MODEM_STATUS_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_MODEM_STATUS_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_GMS_NAS_GET_MODEM_STATUS_response_0x10_t *)hdr;
        out->lte_info_available = true;
        COPY_MTOH(out->lte_info.band, tlv_0x10->band);
        COPY_MTOH(out->lte_info.bandwidth, tlv_0x10->bandwidth);
        COPY_MTOH(out->lte_info.rx_chan, tlv_0x10->rx_chan);
        COPY_MTOH(out->lte_info.tx_chan, tlv_0x10->tx_chan);
        COPY_MTOH(out->lte_info.emm_state, tlv_0x10->emm_state);
        COPY_MTOH(out->lte_info.emm_sub_state, tlv_0x10->emm_sub_state);
        COPY_MTOH(out->lte_info.emm_conn_state, tlv_0x10->emm_conn_state);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_MODEM_STATUS_response_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_MODEM_STATUS_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_GMS_NAS_GET_MODEM_STATUS_response_0x11_t *)hdr;
        out->nr5g_info_available = true;
        COPY_MTOH(out->nr5g_info.band, tlv_0x11->band);
        COPY_MTOH(out->nr5g_info.bandwidth, tlv_0x11->bandwidth);
        COPY_MTOH(out->nr5g_info.rx_chan, tlv_0x11->rx_chan);
        COPY_MTOH(out->nr5g_info.tx_chan, tlv_0x11->tx_chan);
        COPY_MTOH(out->nr5g_info.rssi, tlv_0x11->rssi);
        COPY_MTOH(out->nr5g_info.rsrp, tlv_0x11->rsrp);
        COPY_MTOH(out->nr5g_info.rsrq, tlv_0x11->rsrq);
        COPY_MTOH(out->nr5g_info.snr, tlv_0x11->snr);
        COPY_MTOH(out->nr5g_info.pci, tlv_0x11->pci);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_NAS_GET_LTECQI_INFO (771 - 0x0303)
 */

QMI_SDK_error_t telit_gms_nas_get_ltecqi_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_NAS_GET_LTECQI_INFO,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_nas_get_ltecqi_info_unpack(uint8_t *rsp,
        uint16_t len,
        gms_nas_lte_cqi_info_t *out)
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
        QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t);

        hdr++;
        tlv_0x10 = (QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t *)hdr;
        COPY_MTOH(out->pcc_cqi_info.cqi_0, tlv_0x10->cqi_0);
        COPY_MTOH(out->pcc_cqi_info.cqi_1, tlv_0x10->cqi_1);
        out->pcc_cqi_info_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t);

        hdr++;
        tlv_0x11 = (QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t *)hdr;
        COPY_MTOH(out->scc_0_cqi_info.cqi_0, tlv_0x11->cqi_0);
        COPY_MTOH(out->scc_0_cqi_info.cqi_1, tlv_0x11->cqi_1);
        out->scc_0_cqi_info_available = true;
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t);

        hdr++;
        tlv_0x12 = (QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t *)hdr;
        COPY_MTOH(out->scc_1_cqi_info.cqi_0, tlv_0x12->cqi_0);
        COPY_MTOH(out->scc_1_cqi_info.cqi_1, tlv_0x12->cqi_1);
        out->scc_1_cqi_info_available = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t);

        hdr++;
        tlv_0x13 = (QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t *)hdr;
        COPY_MTOH(out->scc_2_cqi_info.cqi_0, tlv_0x13->cqi_0);
        COPY_MTOH(out->scc_2_cqi_info.cqi_1, tlv_0x13->cqi_1);
        out->scc_2_cqi_info_available = true;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t);

        hdr++;
        tlv_0x14 = (QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t *)hdr;
        COPY_MTOH(out->scc_3_cqi_info.cqi_0, tlv_0x14->cqi_0);
        COPY_MTOH(out->scc_3_cqi_info.cqi_1, tlv_0x14->cqi_1);
        out->scc_3_cqi_info_available = true;
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    /* TLV 0x15 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t *tlv_0x15;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t);

        hdr++;
        tlv_0x15 = (QMI_GMS_NAS_GET_LTECQI_INFO_generic_info_t *)hdr;
        COPY_MTOH(out->scc_4_cqi_info.cqi_0, tlv_0x15->cqi_0);
        COPY_MTOH(out->scc_4_cqi_info.cqi_1, tlv_0x15->cqi_1);
        out->scc_4_cqi_info_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_NAS_GET_LTE_SCC_RX_INFO (772 - 0x0304)
 */

QMI_SDK_error_t telit_gms_nas_get_lte_scc_rx_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_NAS_GET_LTE_SCC_RX_INFO,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_nas_get_lte_scc_rx_info_unpack(uint8_t *rsp,
        uint16_t len,
        gms_nas_lte_scc_info_t *out)
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
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_info_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_info_t);

        hdr++;
        tlv_0x10 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_info_t *)hdr;
        COPY_MTOH(out->scc_0_info.sinr, tlv_0x10->sinr);
        COPY_MTOH(out->scc_0_info.num_instances, tlv_0x10->num_instances);
        out->scc_0_info_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x11 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_0_rx_0_info.rx_chain_index, tlv_0x11->rx_chain_index);
        COPY_MTOH(out->scc_0_rx_0_info.is_radio_tuned, tlv_0x11->is_radio_tuned);
        COPY_MTOH(out->scc_0_rx_0_info.rx_power, tlv_0x11->rx_power);
        COPY_MTOH(out->scc_0_rx_0_info.rsrp, tlv_0x11->rsrp);
        COPY_MTOH(out->scc_0_rx_0_info.rsrq, tlv_0x11->rsrq);
        out->scc_0_rx_0_info_available = true;
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x12 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_0_rx_1_info.rx_chain_index, tlv_0x12->rx_chain_index);
        COPY_MTOH(out->scc_0_rx_1_info.is_radio_tuned, tlv_0x12->is_radio_tuned);
        COPY_MTOH(out->scc_0_rx_1_info.rx_power, tlv_0x12->rx_power);
        COPY_MTOH(out->scc_0_rx_1_info.rsrp, tlv_0x12->rsrp);
        COPY_MTOH(out->scc_0_rx_1_info.rsrq, tlv_0x12->rsrq);
        out->scc_0_rx_1_info_available = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x13 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_0_rx_2_info.rx_chain_index, tlv_0x13->rx_chain_index);
        COPY_MTOH(out->scc_0_rx_2_info.is_radio_tuned, tlv_0x13->is_radio_tuned);
        COPY_MTOH(out->scc_0_rx_2_info.rx_power, tlv_0x13->rx_power);
        COPY_MTOH(out->scc_0_rx_2_info.rsrp, tlv_0x13->rsrp);
        COPY_MTOH(out->scc_0_rx_2_info.rsrq, tlv_0x13->rsrq);
        out->scc_0_rx_2_info_available = true;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x14 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_0_rx_3_info.rx_chain_index, tlv_0x14->rx_chain_index);
        COPY_MTOH(out->scc_0_rx_3_info.is_radio_tuned, tlv_0x14->is_radio_tuned);
        COPY_MTOH(out->scc_0_rx_3_info.rx_power, tlv_0x14->rx_power);
        COPY_MTOH(out->scc_0_rx_3_info.rsrp, tlv_0x14->rsrp);
        COPY_MTOH(out->scc_0_rx_3_info.rsrq, tlv_0x14->rsrq);
        out->scc_0_rx_3_info_available = true;
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    /* TLV 0x15 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_info_t *tlv_0x15;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_info_t);

        hdr++;
        tlv_0x15 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_info_t *)hdr;
        COPY_MTOH(out->scc_1_info.sinr, tlv_0x15->sinr);
        COPY_MTOH(out->scc_1_info.num_instances, tlv_0x15->num_instances);
        out->scc_1_info_available = true;
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    /* TLV 0x16 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x16;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x16 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_1_rx_0_info.rx_chain_index, tlv_0x16->rx_chain_index);
        COPY_MTOH(out->scc_1_rx_0_info.is_radio_tuned, tlv_0x16->is_radio_tuned);
        COPY_MTOH(out->scc_1_rx_0_info.rx_power, tlv_0x16->rx_power);
        COPY_MTOH(out->scc_1_rx_0_info.rsrp, tlv_0x16->rsrp);
        COPY_MTOH(out->scc_1_rx_0_info.rsrq, tlv_0x16->rsrq);
        out->scc_1_rx_0_info_available = true;
    }

    hdr = search_TLV(msg, 0x17, payload_len);
    /* TLV 0x17 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x17 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_1_rx_1_info.rx_chain_index, tlv_0x17->rx_chain_index);
        COPY_MTOH(out->scc_1_rx_1_info.is_radio_tuned, tlv_0x17->is_radio_tuned);
        COPY_MTOH(out->scc_1_rx_1_info.rx_power, tlv_0x17->rx_power);
        COPY_MTOH(out->scc_1_rx_1_info.rsrp, tlv_0x17->rsrp);
        COPY_MTOH(out->scc_1_rx_1_info.rsrq, tlv_0x17->rsrq);
        out->scc_1_rx_1_info_available = true;
    }

    hdr = search_TLV(msg, 0x18, payload_len);
    /* TLV 0x18 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x18;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x18 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_1_rx_2_info.rx_chain_index, tlv_0x18->rx_chain_index);
        COPY_MTOH(out->scc_1_rx_2_info.is_radio_tuned, tlv_0x18->is_radio_tuned);
        COPY_MTOH(out->scc_1_rx_2_info.rx_power, tlv_0x18->rx_power);
        COPY_MTOH(out->scc_1_rx_2_info.rsrp, tlv_0x18->rsrp);
        COPY_MTOH(out->scc_1_rx_2_info.rsrq, tlv_0x18->rsrq);
        out->scc_1_rx_2_info_available = true;
    }

    hdr = search_TLV(msg, 0x19, payload_len);
    /* TLV 0x19 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x19;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x19 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_1_rx_3_info.rx_chain_index, tlv_0x19->rx_chain_index);
        COPY_MTOH(out->scc_1_rx_3_info.is_radio_tuned, tlv_0x19->is_radio_tuned);
        COPY_MTOH(out->scc_1_rx_3_info.rx_power, tlv_0x19->rx_power);
        COPY_MTOH(out->scc_1_rx_3_info.rsrp, tlv_0x19->rsrp);
        COPY_MTOH(out->scc_1_rx_3_info.rsrq, tlv_0x19->rsrq);
        out->scc_1_rx_3_info_available = true;
    }

    hdr = search_TLV(msg, 0x1A, payload_len);
    /* TLV 0x1A is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_info_t *tlv_0x1A;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_info_t);

        hdr++;
        tlv_0x1A = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_info_t *)hdr;
        COPY_MTOH(out->scc_2_info.sinr, tlv_0x1A->sinr);
        COPY_MTOH(out->scc_2_info.num_instances, tlv_0x1A->num_instances);
        out->scc_2_info_available = true;
    }

    hdr = search_TLV(msg, 0x1B, payload_len);
    /* TLV 0x1B is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x1B;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x1B = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_2_rx_0_info.rx_chain_index, tlv_0x1B->rx_chain_index);
        COPY_MTOH(out->scc_2_rx_0_info.is_radio_tuned, tlv_0x1B->is_radio_tuned);
        COPY_MTOH(out->scc_2_rx_0_info.rx_power, tlv_0x1B->rx_power);
        COPY_MTOH(out->scc_2_rx_0_info.rsrp, tlv_0x1B->rsrp);
        COPY_MTOH(out->scc_2_rx_0_info.rsrq, tlv_0x1B->rsrq);
        out->scc_2_rx_0_info_available = true;
    }

    hdr = search_TLV(msg, 0x1C, payload_len);
    /* TLV 0x1C is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x1C;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x1C = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_2_rx_1_info.rx_chain_index, tlv_0x1C->rx_chain_index);
        COPY_MTOH(out->scc_2_rx_1_info.is_radio_tuned, tlv_0x1C->is_radio_tuned);
        COPY_MTOH(out->scc_2_rx_1_info.rx_power, tlv_0x1C->rx_power);
        COPY_MTOH(out->scc_2_rx_1_info.rsrp, tlv_0x1C->rsrp);
        COPY_MTOH(out->scc_2_rx_1_info.rsrq, tlv_0x1C->rsrq);
        out->scc_2_rx_1_info_available = true;
    }

    hdr = search_TLV(msg, 0x1D, payload_len);
    /* TLV 0x1D is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x1D;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x1D = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_2_rx_2_info.rx_chain_index, tlv_0x1D->rx_chain_index);
        COPY_MTOH(out->scc_2_rx_2_info.is_radio_tuned, tlv_0x1D->is_radio_tuned);
        COPY_MTOH(out->scc_2_rx_2_info.rx_power, tlv_0x1D->rx_power);
        COPY_MTOH(out->scc_2_rx_2_info.rsrp, tlv_0x1D->rsrp);
        COPY_MTOH(out->scc_2_rx_2_info.rsrq, tlv_0x1D->rsrq);
        out->scc_2_rx_2_info_available = true;
    }

    hdr = search_TLV(msg, 0x1E, payload_len);
    /* TLV 0x1E is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x1E;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x1E = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_2_rx_3_info.rx_chain_index, tlv_0x1E->rx_chain_index);
        COPY_MTOH(out->scc_2_rx_3_info.is_radio_tuned, tlv_0x1E->is_radio_tuned);
        COPY_MTOH(out->scc_2_rx_3_info.rx_power, tlv_0x1E->rx_power);
        COPY_MTOH(out->scc_2_rx_3_info.rsrp, tlv_0x1E->rsrp);
        COPY_MTOH(out->scc_2_rx_3_info.rsrq, tlv_0x1E->rsrq);
        out->scc_2_rx_3_info_available = true;
    }

    hdr = search_TLV(msg, 0x1F, payload_len);
    /* TLV 0x1F is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_info_t *tlv_0x1F;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_info_t);

        hdr++;
        tlv_0x1F = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_info_t *)hdr;
        COPY_MTOH(out->scc_3_info.sinr, tlv_0x1F->sinr);
        COPY_MTOH(out->scc_3_info.num_instances, tlv_0x1F->num_instances);
        out->scc_3_info_available = true;
    }

    hdr = search_TLV(msg, 0x20, payload_len);
    /* TLV 0x20 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x20;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x20 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_3_rx_0_info.rx_chain_index, tlv_0x20->rx_chain_index);
        COPY_MTOH(out->scc_3_rx_0_info.is_radio_tuned, tlv_0x20->is_radio_tuned);
        COPY_MTOH(out->scc_3_rx_0_info.rx_power, tlv_0x20->rx_power);
        COPY_MTOH(out->scc_3_rx_0_info.rsrp, tlv_0x20->rsrp);
        COPY_MTOH(out->scc_3_rx_0_info.rsrq, tlv_0x20->rsrq);
        out->scc_3_rx_0_info_available = true;
    }

    hdr = search_TLV(msg, 0x21, payload_len);
    /* TLV 0x21 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x21;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x21 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_3_rx_1_info.rx_chain_index, tlv_0x21->rx_chain_index);
        COPY_MTOH(out->scc_3_rx_1_info.is_radio_tuned, tlv_0x21->is_radio_tuned);
        COPY_MTOH(out->scc_3_rx_1_info.rx_power, tlv_0x21->rx_power);
        COPY_MTOH(out->scc_3_rx_1_info.rsrp, tlv_0x21->rsrp);
        COPY_MTOH(out->scc_3_rx_1_info.rsrq, tlv_0x21->rsrq);
        out->scc_3_rx_1_info_available = true;
    }

    hdr = search_TLV(msg, 0x22, payload_len);
    /* TLV 0x22 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x22;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x22 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_3_rx_2_info.rx_chain_index, tlv_0x22->rx_chain_index);
        COPY_MTOH(out->scc_3_rx_2_info.is_radio_tuned, tlv_0x22->is_radio_tuned);
        COPY_MTOH(out->scc_3_rx_2_info.rx_power, tlv_0x22->rx_power);
        COPY_MTOH(out->scc_3_rx_2_info.rsrp, tlv_0x22->rsrp);
        COPY_MTOH(out->scc_3_rx_2_info.rsrq, tlv_0x22->rsrq);
        out->scc_3_rx_2_info_available = true;
    }

    hdr = search_TLV(msg, 0x23, payload_len);
    /* TLV 0x23 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *tlv_0x23;

        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t);

        hdr++;
        tlv_0x23 = (QMI_GMS_NAS_GET_LTE_SCC_RX_INFO_generic_instance_t *)hdr;
        COPY_MTOH(out->scc_3_rx_3_info.rx_chain_index, tlv_0x23->rx_chain_index);
        COPY_MTOH(out->scc_3_rx_3_info.is_radio_tuned, tlv_0x23->is_radio_tuned);
        COPY_MTOH(out->scc_3_rx_3_info.rx_power, tlv_0x23->rx_power);
        COPY_MTOH(out->scc_3_rx_3_info.rsrp, tlv_0x23->rsrp);
        COPY_MTOH(out->scc_3_rx_3_info.rsrq, tlv_0x23->rsrq);
        out->scc_3_rx_3_info_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_NAS_IND_REGISTER (773 - 0x0305)
 */

QMI_SDK_error_t telit_gms_nas_ind_register_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gms_nas_indication_register_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_NAS_INDICATION_REGISTER_request_0x01_t *tlv_0x01;
    QMI_NAS_INDICATION_REGISTER_request_0x02_t *tlv_0x02;
    QMI_NAS_INDICATION_REGISTER_request_0x03_t *tlv_0x03;
    QMI_NAS_INDICATION_REGISTER_request_0x04_t *tlv_0x04;
    QMI_NAS_INDICATION_REGISTER_request_0x05_t *tlv_0x05;
    QMI_NAS_INDICATION_REGISTER_request_0x06_t *tlv_0x06;
    QMI_NAS_INDICATION_REGISTER_request_0x07_t *tlv_0x07;
    QMI_NAS_INDICATION_REGISTER_request_0x08_t *tlv_0x08;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_GMS_NAS_IND_REGISTER;
    req_ctx->svc_id = QMI_SVC_GMS;
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

    tlv_0x01 = (QMI_NAS_INDICATION_REGISTER_request_0x01_t *)buf_position;
    tlv_0x01->lte_esm_ul = input->lte_esm_ul ? 1 : 0;

    buf_position += tlv_sz;

    /* add mandatory tlv 0x02 */
    tlv_sz = sizeof(*tlv_0x02);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_NAS_INDICATION_REGISTER_request_0x02_t *)buf_position;
    tlv_0x02->lte_esm_dl = input->lte_esm_dl ? 1 : 0;

    buf_position += tlv_sz;

    /* add mandatory tlv 0x03 */
    tlv_sz = sizeof(*tlv_0x03);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x03;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x03 = (QMI_NAS_INDICATION_REGISTER_request_0x03_t *)buf_position;
    tlv_0x03->lte_emm_ul = input->lte_emm_ul ? 1 : 0;

    buf_position += tlv_sz;

    /* add mandatory tlv 0x04 */
    tlv_sz = sizeof(*tlv_0x04);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x04;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x04 = (QMI_NAS_INDICATION_REGISTER_request_0x04_t *)buf_position;
    tlv_0x04->lte_emm_dl = input->lte_emm_dl ? 1 : 0;

    buf_position += tlv_sz;

    /* add mandatory tlv 0x05 */
    tlv_sz = sizeof(*tlv_0x05);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x05;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x05 = (QMI_NAS_INDICATION_REGISTER_request_0x05_t *)buf_position;
    tlv_0x05->gsm_umts_ul = input->gsm_umts_ul ? 1 : 0;

    buf_position += tlv_sz;

    /* add mandatory tlv 0x06 */
    tlv_sz = sizeof(*tlv_0x06);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x06;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x06 = (QMI_NAS_INDICATION_REGISTER_request_0x06_t *)buf_position;
    tlv_0x06->gsm_umts_dl = input->gsm_umts_dl ? 1 : 0;

    buf_position += tlv_sz;

    /* add mandatory tlv 0x07 */
    tlv_sz = sizeof(*tlv_0x07);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x07;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x07 = (QMI_NAS_INDICATION_REGISTER_request_0x07_t *)buf_position;
    tlv_0x07->rank_indicator = input->rank_indicator ? 1 : 0;

    buf_position += tlv_sz;

    /* add mandatory tlv 0x08 */
    tlv_sz = sizeof(*tlv_0x08);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x08;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x08 = (QMI_NAS_INDICATION_REGISTER_request_0x08_t *)buf_position;
    tlv_0x08->timer = input->timer ? 1 : 0;

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gms_nas_ind_register_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GMS_NAS_GET_BAND_CAP (777 - 0x0309)
 */

QMI_SDK_error_t telit_gms_nas_get_band_cap_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_NAS_GET_BAND_CAP,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_nas_get_band_cap_unpack(uint8_t *rsp, uint16_t len, gms_nas_band_capability_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GMS_NAS_GET_BAND_CAP_response_0x03_t *tlv_0x03;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_BAND_CAP_response_0x03_t);

    hdr++;
    tlv_0x03 = (QMI_GMS_NAS_GET_BAND_CAP_response_0x03_t *)hdr;
    COPY_MTOH(out->variant, tlv_0x03->variant);
    COPY_MTOH(out->band_pref, tlv_0x03->band_pref);
    COPY_MTOH(out->bits_1_64, tlv_0x03->bits_1_64);
    COPY_MTOH(out->bits_65_128, tlv_0x03->bits_65_128);
    COPY_MTOH(out->bits_129_192, tlv_0x03->bits_129_192);
    COPY_MTOH(out->bits_193_256, tlv_0x03->bits_193_256);

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_NAS_GET_ENDC_CA_INFO (783 - 0x030f)
 */

QMI_SDK_error_t telit_gms_nas_get_endc_ca_info_pack(pack_qmi_t *req_ctx, void *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_NAS_GET_ENDC_CA_INFO,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_nas_get_endc_ca_info_unpack(uint8_t *rsp, uint16_t len,
        gms_nas_endc_ca_info_t *out)
{
    QMI_SDK_error_t ret;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len;
    uint8_t *msg;

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    memset(out, 0, sizeof(*out));

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_ENDC_CA_INFO_response_0x10_t *tlv_0x10;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_ENDC_CA_INFO_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_GMS_NAS_GET_ENDC_CA_INFO_response_0x10_t *)hdr;
        COPY_MTOH(out->pcc_ca_info.band_class, tlv_0x10->band_class);
        COPY_MTOH(out->pcc_ca_info.channel, tlv_0x10->channel);
        COPY_MTOH(out->pcc_ca_info.dl_bw, tlv_0x10->dl_bw);
        COPY_MTOH(out->pcc_ca_info.pci, tlv_0x10->pci);
        COPY_MTOH(out->pcc_ca_info.rsrp, tlv_0x10->rsrp);
        COPY_MTOH(out->pcc_ca_info.rssi, tlv_0x10->rssi);
        COPY_MTOH(out->pcc_ca_info.rsrq, tlv_0x10->rsrq);
        COPY_MTOH(out->pcc_ca_info.sinr, tlv_0x10->sinr);
        COPY_MTOH(out->pcc_ca_info.tac, tlv_0x10->tac);

        out->pcc_ca_info_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t *tlv_0x11;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t);

        hdr++;
        tlv_0x11 = (QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t *)hdr;
        COPY_MTOH(out->scc_0_ca_info.band_class, tlv_0x11->band_class);
        COPY_MTOH(out->scc_0_ca_info.channel, tlv_0x11->channel);
        COPY_MTOH(out->scc_0_ca_info.dl_bw, tlv_0x11->dl_bw);
        COPY_MTOH(out->scc_0_ca_info.pci, tlv_0x11->pci);
        COPY_MTOH(out->scc_0_ca_info.rsrp, tlv_0x11->rsrp);
        COPY_MTOH(out->scc_0_ca_info.rssi, tlv_0x11->rssi);
        COPY_MTOH(out->scc_0_ca_info.rsrq, tlv_0x11->rsrq);
        COPY_MTOH(out->scc_0_ca_info.sinr, tlv_0x11->sinr);
        COPY_MTOH(out->scc_0_ca_info.state, tlv_0x11->state);

        out->scc_0_ca_info_available = true;
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t *tlv_0x12;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t);

        hdr++;
        tlv_0x12 = (QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t *)hdr;
        COPY_MTOH(out->scc_1_ca_info.band_class, tlv_0x12->band_class);
        COPY_MTOH(out->scc_1_ca_info.channel, tlv_0x12->channel);
        COPY_MTOH(out->scc_1_ca_info.dl_bw, tlv_0x12->dl_bw);
        COPY_MTOH(out->scc_1_ca_info.pci, tlv_0x12->pci);
        COPY_MTOH(out->scc_1_ca_info.rsrp, tlv_0x12->rsrp);
        COPY_MTOH(out->scc_1_ca_info.rssi, tlv_0x12->rssi);
        COPY_MTOH(out->scc_1_ca_info.rsrq, tlv_0x12->rsrq);
        COPY_MTOH(out->scc_1_ca_info.sinr, tlv_0x12->sinr);
        COPY_MTOH(out->scc_1_ca_info.state, tlv_0x12->state);

        out->scc_1_ca_info_available = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t *tlv_0x13;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t);

        hdr++;
        tlv_0x13 = (QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t *)hdr;
        COPY_MTOH(out->scc_2_ca_info.band_class, tlv_0x13->band_class);
        COPY_MTOH(out->scc_2_ca_info.channel, tlv_0x13->channel);
        COPY_MTOH(out->scc_2_ca_info.dl_bw, tlv_0x13->dl_bw);
        COPY_MTOH(out->scc_2_ca_info.pci, tlv_0x13->pci);
        COPY_MTOH(out->scc_2_ca_info.rsrp, tlv_0x13->rsrp);
        COPY_MTOH(out->scc_2_ca_info.rssi, tlv_0x13->rssi);
        COPY_MTOH(out->scc_2_ca_info.rsrq, tlv_0x13->rsrq);
        COPY_MTOH(out->scc_2_ca_info.sinr, tlv_0x13->sinr);
        COPY_MTOH(out->scc_2_ca_info.state, tlv_0x13->state);

        out->scc_2_ca_info_available = true;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t *tlv_0x14;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t);

        hdr++;
        tlv_0x14 = (QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t *)hdr;
        COPY_MTOH(out->scc_3_ca_info.band_class, tlv_0x14->band_class);
        COPY_MTOH(out->scc_3_ca_info.channel, tlv_0x14->channel);
        COPY_MTOH(out->scc_3_ca_info.dl_bw, tlv_0x14->dl_bw);
        COPY_MTOH(out->scc_3_ca_info.pci, tlv_0x14->pci);
        COPY_MTOH(out->scc_3_ca_info.rsrp, tlv_0x14->rsrp);
        COPY_MTOH(out->scc_3_ca_info.rssi, tlv_0x14->rssi);
        COPY_MTOH(out->scc_3_ca_info.rsrq, tlv_0x14->rsrq);
        COPY_MTOH(out->scc_3_ca_info.sinr, tlv_0x14->sinr);
        COPY_MTOH(out->scc_3_ca_info.state, tlv_0x14->state);

        out->scc_3_ca_info_available = true;
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    /* TLV 0x15 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t *tlv_0x15;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t);

        hdr++;
        tlv_0x15 = (QMI_GMS_NAS_GET_ENDC_CA_INFO_generic_SCC_info_t *)hdr;
        COPY_MTOH(out->scc_4_ca_info.band_class, tlv_0x15->band_class);
        COPY_MTOH(out->scc_4_ca_info.channel, tlv_0x15->channel);
        COPY_MTOH(out->scc_4_ca_info.dl_bw, tlv_0x15->dl_bw);
        COPY_MTOH(out->scc_4_ca_info.pci, tlv_0x15->pci);
        COPY_MTOH(out->scc_4_ca_info.rsrp, tlv_0x15->rsrp);
        COPY_MTOH(out->scc_4_ca_info.rssi, tlv_0x15->rssi);
        COPY_MTOH(out->scc_4_ca_info.rsrq, tlv_0x15->rsrq);
        COPY_MTOH(out->scc_4_ca_info.sinr, tlv_0x15->sinr);
        COPY_MTOH(out->scc_4_ca_info.state, tlv_0x15->state);

        out->scc_4_ca_info_available = true;
    }

    hdr = search_TLV(msg, 0x16, payload_len);
    /* TLV 0x16 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_ENDC_CA_INFO_response_0x16_t *tlv_0x16;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_ENDC_CA_INFO_response_0x16_t);

        hdr++;
        tlv_0x16 = (QMI_GMS_NAS_GET_ENDC_CA_INFO_response_0x16_t *)hdr;
        COPY_MTOH(out->cc_5g_info.endc_band, tlv_0x16->endc_band);
        COPY_MTOH(out->cc_5g_info.endc_bandwidth, tlv_0x16->endc_bandwidth);
        COPY_MTOH(out->cc_5g_info.endc_channel, tlv_0x16->endc_channel);
        COPY_MTOH(out->cc_5g_info.endc_ul_channel, tlv_0x16->endc_ul_channel);
        COPY_MTOH(out->cc_5g_info.endc_pci, tlv_0x16->endc_pci);
        COPY_MTOH(out->cc_5g_info.endc_rsrp, tlv_0x16->endc_rsrp);
        COPY_MTOH(out->cc_5g_info.endc_rssi, tlv_0x16->endc_rssi);
        COPY_MTOH(out->cc_5g_info.endc_rsrq, tlv_0x16->endc_rsrq);
        COPY_MTOH(out->cc_5g_info.endc_sinr, tlv_0x16->endc_sinr);

        out->cc_5g_info_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_NAS_GET_NR5G_CA_INFO (790 - 0x0316)
 */

QMI_SDK_error_t telit_gms_nas_get_nr5g_ca_info_pack(pack_qmi_t *req_ctx, void *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_NAS_GET_NR5G_CA_INFO,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_nas_get_nr5g_ca_info_unpack(uint8_t *rsp, uint16_t len,
        gms_nas_nr5g_ca_info_t *out)
{
    QMI_SDK_error_t ret;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len;
    uint8_t *msg;

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    memset(out, 0, sizeof(*out));

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_NR5G_CA_INFO_response_0x10_t *tlv_0x10;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_NR5G_CA_INFO_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_GMS_NAS_GET_NR5G_CA_INFO_response_0x10_t *)hdr;
        COPY_MTOH(out->nr5g_ca_pcc_info.band_class, tlv_0x10->band_class);
        COPY_MTOH(out->nr5g_ca_pcc_info.channel, tlv_0x10->channel);
        COPY_MTOH(out->nr5g_ca_pcc_info.dl_bw, tlv_0x10->dl_bw);
        COPY_MTOH(out->nr5g_ca_pcc_info.pci, tlv_0x10->pci);
        COPY_MTOH(out->nr5g_ca_pcc_info.rsrp, tlv_0x10->rsrp);
        COPY_MTOH(out->nr5g_ca_pcc_info.rssi, tlv_0x10->rssi);
        COPY_MTOH(out->nr5g_ca_pcc_info.rsrq, tlv_0x10->rsrq);
        COPY_MTOH(out->nr5g_ca_pcc_info.sinr, tlv_0x10->sinr);
        COPY_MTOH(out->nr5g_ca_pcc_info.tac, tlv_0x10->tac);

        out->nr5g_ca_pcc_info_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t *tlv_0x11;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t);

        hdr++;
        tlv_0x11 = (QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t *)hdr;
        COPY_MTOH(out->nr5g_ca_scc0_info.band_class, tlv_0x11->band_class);
        COPY_MTOH(out->nr5g_ca_scc0_info.channel, tlv_0x11->channel);
        COPY_MTOH(out->nr5g_ca_scc0_info.dl_bw, tlv_0x11->dl_bw);
        COPY_MTOH(out->nr5g_ca_scc0_info.pci, tlv_0x11->pci);
        COPY_MTOH(out->nr5g_ca_scc0_info.rsrp, tlv_0x11->rsrp);
        COPY_MTOH(out->nr5g_ca_scc0_info.rssi, tlv_0x11->rssi);
        COPY_MTOH(out->nr5g_ca_scc0_info.rsrq, tlv_0x11->rsrq);
        COPY_MTOH(out->nr5g_ca_scc0_info.sinr, tlv_0x11->sinr);
        COPY_MTOH(out->nr5g_ca_scc0_info.state, tlv_0x11->state);

        out->nr5g_ca_scc0_info_available = true;
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t *tlv_0x12;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t);

        hdr++;
        tlv_0x12 = (QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t *)hdr;
        COPY_MTOH(out->nr5g_ca_scc1_info.band_class, tlv_0x12->band_class);
        COPY_MTOH(out->nr5g_ca_scc1_info.channel, tlv_0x12->channel);
        COPY_MTOH(out->nr5g_ca_scc1_info.dl_bw, tlv_0x12->dl_bw);
        COPY_MTOH(out->nr5g_ca_scc1_info.pci, tlv_0x12->pci);
        COPY_MTOH(out->nr5g_ca_scc1_info.rsrp, tlv_0x12->rsrp);
        COPY_MTOH(out->nr5g_ca_scc1_info.rssi, tlv_0x12->rssi);
        COPY_MTOH(out->nr5g_ca_scc1_info.rsrq, tlv_0x12->rsrq);
        COPY_MTOH(out->nr5g_ca_scc1_info.sinr, tlv_0x12->sinr);
        COPY_MTOH(out->nr5g_ca_scc1_info.state, tlv_0x12->state);

        out->nr5g_ca_scc1_info_available = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t *tlv_0x13;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t);

        hdr++;
        tlv_0x13 = (QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t *)hdr;
        COPY_MTOH(out->nr5g_ca_scc2_info.band_class, tlv_0x13->band_class);
        COPY_MTOH(out->nr5g_ca_scc2_info.channel, tlv_0x13->channel);
        COPY_MTOH(out->nr5g_ca_scc2_info.dl_bw, tlv_0x13->dl_bw);
        COPY_MTOH(out->nr5g_ca_scc2_info.pci, tlv_0x13->pci);
        COPY_MTOH(out->nr5g_ca_scc2_info.rsrp, tlv_0x13->rsrp);
        COPY_MTOH(out->nr5g_ca_scc2_info.rssi, tlv_0x13->rssi);
        COPY_MTOH(out->nr5g_ca_scc2_info.rsrq, tlv_0x13->rsrq);
        COPY_MTOH(out->nr5g_ca_scc2_info.sinr, tlv_0x13->sinr);
        COPY_MTOH(out->nr5g_ca_scc2_info.state, tlv_0x13->state);

        out->nr5g_ca_scc2_info_available = true;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t *tlv_0x14;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t);

        hdr++;
        tlv_0x14 = (QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t *)hdr;
        COPY_MTOH(out->nr5g_ca_scc3_info.band_class, tlv_0x14->band_class);
        COPY_MTOH(out->nr5g_ca_scc3_info.channel, tlv_0x14->channel);
        COPY_MTOH(out->nr5g_ca_scc3_info.dl_bw, tlv_0x14->dl_bw);
        COPY_MTOH(out->nr5g_ca_scc3_info.pci, tlv_0x14->pci);
        COPY_MTOH(out->nr5g_ca_scc3_info.rsrp, tlv_0x14->rsrp);
        COPY_MTOH(out->nr5g_ca_scc3_info.rssi, tlv_0x14->rssi);
        COPY_MTOH(out->nr5g_ca_scc3_info.rsrq, tlv_0x14->rsrq);
        COPY_MTOH(out->nr5g_ca_scc3_info.sinr, tlv_0x14->sinr);
        COPY_MTOH(out->nr5g_ca_scc3_info.state, tlv_0x14->state);

        out->nr5g_ca_scc3_info_available = true;
    }

    hdr = search_TLV(msg, 0x15, payload_len);
    /* TLV 0x15 is optional */
    if (hdr) {
        QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t *tlv_0x15;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t);

        hdr++;
        tlv_0x15 = (QMI_GMS_NAS_GET_NR5G_CA_INFO_generic_SCC_info_t *)hdr;
        COPY_MTOH(out->nr5g_ca_scc4_info.band_class, tlv_0x15->band_class);
        COPY_MTOH(out->nr5g_ca_scc4_info.channel, tlv_0x15->channel);
        COPY_MTOH(out->nr5g_ca_scc4_info.dl_bw, tlv_0x15->dl_bw);
        COPY_MTOH(out->nr5g_ca_scc4_info.pci, tlv_0x15->pci);
        COPY_MTOH(out->nr5g_ca_scc4_info.rsrp, tlv_0x15->rsrp);
        COPY_MTOH(out->nr5g_ca_scc4_info.rssi, tlv_0x15->rssi);
        COPY_MTOH(out->nr5g_ca_scc4_info.rsrq, tlv_0x15->rsrq);
        COPY_MTOH(out->nr5g_ca_scc4_info.sinr, tlv_0x15->sinr);
        COPY_MTOH(out->nr5g_ca_scc4_info.state, tlv_0x15->state);

        out->nr5g_ca_scc4_info_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_NAS_SET_MCCBLOCK (794 - 0x031A)
 */

QMI_SDK_error_t telit_gms_nas_set_mccblock_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        gms_nas_mccblock_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GMS_NAS_MCCBLOCK_generic_mcc_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_GMS_NAS_SET_MCCBLOCK;
    req_ctx->svc_id = QMI_SVC_GMS;
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

    tlv_0x01 = (QMI_GMS_NAS_MCCBLOCK_generic_mcc_t *)buf_position;
    COPY_HTOM(tlv_0x01->mcc, input->mcc1);

    buf_position += tlv_sz;

    if (input->mcc2_available) {
        QMI_GMS_NAS_MCCBLOCK_generic_mcc_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_GMS_NAS_MCCBLOCK_generic_mcc_t *)buf_position;
        COPY_HTOM(tlv_0x10->mcc, input->mcc2);

        buf_position += tlv_sz;
    }

    if (input->mcc3_available) {
        QMI_GMS_NAS_MCCBLOCK_generic_mcc_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_GMS_NAS_MCCBLOCK_generic_mcc_t *)buf_position;
        COPY_HTOM(tlv_0x11->mcc, input->mcc3);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gms_nas_set_mccblock_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 *  QMI_GMS_NAS_GET_MCCBLOCK (795 - 0x031B)
 */

QMI_SDK_error_t telit_gms_nas_get_mccblock_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_NAS_GET_MCCBLOCK,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_nas_get_mccblock_unpack(uint8_t *rsp,
        uint16_t len,
        gms_nas_mccblock_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GMS_NAS_MCCBLOCK_generic_mcc_t *tlv_0x03;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_MCCBLOCK_generic_mcc_t);

    hdr++;
    tlv_0x03 = (QMI_GMS_NAS_MCCBLOCK_generic_mcc_t *)hdr;
    COPY_MTOH(out->mcc1, tlv_0x03->mcc);

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_GMS_NAS_MCCBLOCK_generic_mcc_t *tlv_0x10;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_MCCBLOCK_generic_mcc_t);

        hdr++;
        tlv_0x10 = (QMI_GMS_NAS_MCCBLOCK_generic_mcc_t *)hdr;
        COPY_MTOH(out->mcc2, tlv_0x10->mcc);

        out->mcc2_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_GMS_NAS_MCCBLOCK_generic_mcc_t *tlv_0x11;
        CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_MCCBLOCK_generic_mcc_t);

        hdr++;
        tlv_0x11 = (QMI_GMS_NAS_MCCBLOCK_generic_mcc_t *)hdr;
        COPY_MTOH(out->mcc3, tlv_0x11->mcc);

        out->mcc3_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_LOC_GET_AUTO_START (4097 - 0x1001)
 */

QMI_SDK_error_t telit_gms_loc_get_auto_start_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_LOC_GET_AUTO_START,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_loc_get_auto_start_unpack(uint8_t *rsp,
        uint16_t len,
        gms_loc_get_auto_start_resp_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GMS_LOC_GET_AUTO_START_response_0x03_t *tlv_0x03;
    QMI_GMS_LOC_GET_AUTO_START_response_0x04_t *tlv_0x04;
    QMI_GMS_LOC_GET_AUTO_START_response_0x05_t *tlv_0x05;
    QMI_GMS_LOC_GET_AUTO_START_response_0x06_t *tlv_0x06;
    QMI_GMS_LOC_GET_AUTO_START_response_0x07_t *tlv_0x07;
    QMI_GMS_LOC_GET_AUTO_START_response_0x08_t *tlv_0x08;
    QMI_GMS_LOC_GET_AUTO_START_response_0x09_t *tlv_0x09;
    QMI_GMS_LOC_GET_AUTO_START_response_0x0A_t *tlv_0x0A;
    QMI_GMS_LOC_GET_AUTO_START_response_0x0B_t *tlv_0x0B;
    QMI_GMS_LOC_GET_AUTO_START_response_0x0C_t *tlv_0x0C;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_AUTO_START_response_0x03_t);

    hdr++;
    tlv_0x03 = (QMI_GMS_LOC_GET_AUTO_START_response_0x03_t *)hdr;
    COPY_MTOH(out->function, tlv_0x03->function);

    FIND_MANDATORY_TLV(msg, 0x04, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_AUTO_START_response_0x04_t);

    hdr++;
    tlv_0x04 = (QMI_GMS_LOC_GET_AUTO_START_response_0x04_t *)hdr;
    COPY_MTOH(out->function_reported, tlv_0x04->function_reported);

    FIND_MANDATORY_TLV(msg, 0x05, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_AUTO_START_response_0x05_t);

    hdr++;
    tlv_0x05 = (QMI_GMS_LOC_GET_AUTO_START_response_0x05_t *)hdr;
    COPY_MTOH(out->fix_type, tlv_0x05->fix_type);

    FIND_MANDATORY_TLV(msg, 0x06, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_AUTO_START_response_0x06_t);

    hdr++;
    tlv_0x06 = (QMI_GMS_LOC_GET_AUTO_START_response_0x06_t *)hdr;
    COPY_MTOH(out->fix_type_reported, tlv_0x06->fix_type_reported);

    FIND_MANDATORY_TLV(msg, 0x07, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_AUTO_START_response_0x07_t);

    hdr++;
    tlv_0x07 = (QMI_GMS_LOC_GET_AUTO_START_response_0x07_t *)hdr;
    COPY_MTOH(out->max_time, tlv_0x07->max_time);

    FIND_MANDATORY_TLV(msg, 0x08, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_AUTO_START_response_0x08_t);

    hdr++;
    tlv_0x08 = (QMI_GMS_LOC_GET_AUTO_START_response_0x08_t *)hdr;
    COPY_MTOH(out->max_time_reported, tlv_0x08->max_time_reported);

    FIND_MANDATORY_TLV(msg, 0x09, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_AUTO_START_response_0x09_t);

    hdr++;
    tlv_0x09 = (QMI_GMS_LOC_GET_AUTO_START_response_0x09_t *)hdr;
    COPY_MTOH(out->max_dist, tlv_0x09->max_dist);

    FIND_MANDATORY_TLV(msg, 0x0A, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_AUTO_START_response_0x0A_t);

    hdr++;
    tlv_0x0A = (QMI_GMS_LOC_GET_AUTO_START_response_0x0A_t *)hdr;
    COPY_MTOH(out->max_dist_reported, tlv_0x0A->max_dist_reported);

    FIND_MANDATORY_TLV(msg, 0x0B, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_AUTO_START_response_0x0B_t);

    hdr++;
    tlv_0x0B = (QMI_GMS_LOC_GET_AUTO_START_response_0x0B_t *)hdr;
    COPY_MTOH(out->fix_rate, tlv_0x0B->fix_rate);

    FIND_MANDATORY_TLV(msg, 0x0C, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_AUTO_START_response_0x0C_t);

    hdr++;
    tlv_0x0C = (QMI_GMS_LOC_GET_AUTO_START_response_0x0C_t *)hdr;
    COPY_MTOH(out->fix_rate_reported, tlv_0x0C->fix_rate_reported);

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_LOC_SET_AUTO_START (4098 - 0x1002)
 */

QMI_SDK_error_t telit_gms_loc_set_auto_start_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        gms_loc_set_auto_start_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GMS_LOC_SET_AUTO_START_request_0x01_t *tlv_0x01;
    QMI_GMS_LOC_SET_AUTO_START_request_0x02_t *tlv_0x02;
    QMI_GMS_LOC_SET_AUTO_START_request_0x03_t *tlv_0x03;
    QMI_GMS_LOC_SET_AUTO_START_request_0x04_t *tlv_0x04;
    QMI_GMS_LOC_SET_AUTO_START_request_0x05_t *tlv_0x05;
    QMI_GMS_LOC_SET_AUTO_START_request_0x06_t *tlv_0x06;
    QMI_GMS_LOC_SET_AUTO_START_request_0x07_t *tlv_0x07;
    QMI_GMS_LOC_SET_AUTO_START_request_0x08_t *tlv_0x08;
    QMI_GMS_LOC_SET_AUTO_START_request_0x09_t *tlv_0x09;
    QMI_GMS_LOC_SET_AUTO_START_request_0x0A_t *tlv_0x0A;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_GMS_LOC_SET_AUTO_START;
    req_ctx->svc_id = QMI_SVC_GMS;
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

    tlv_0x01 = (QMI_GMS_LOC_SET_AUTO_START_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->function, input->function);

    buf_position += tlv_sz;

    /* pack TLV 0x02 */
    tlv_sz = sizeof(*tlv_0x02);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_GMS_LOC_SET_AUTO_START_request_0x02_t *)buf_position;
    COPY_HTOM(tlv_0x02->set_function, input->set_function);

    buf_position += tlv_sz;

    /* pack TLV 0x03 */
    tlv_sz = sizeof(*tlv_0x03);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x03;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x03 = (QMI_GMS_LOC_SET_AUTO_START_request_0x03_t *)buf_position;
    COPY_HTOM(tlv_0x03->fix_type, input->fix_type);

    buf_position += tlv_sz;

    /* pack TLV 0x04 */
    tlv_sz = sizeof(*tlv_0x04);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x04;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x04 = (QMI_GMS_LOC_SET_AUTO_START_request_0x04_t *)buf_position;
    COPY_HTOM(tlv_0x04->set_fix_type, input->set_fix_type);

    buf_position += tlv_sz;

    /* pack TLV 0x05 */
    tlv_sz = sizeof(*tlv_0x05);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x05;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x05 = (QMI_GMS_LOC_SET_AUTO_START_request_0x05_t *)buf_position;
    COPY_HTOM(tlv_0x05->max_time, input->max_time);

    buf_position += tlv_sz;

    /* pack TLV 0x06 */
    tlv_sz = sizeof(*tlv_0x06);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x06;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x06 = (QMI_GMS_LOC_SET_AUTO_START_request_0x06_t *)buf_position;
    COPY_HTOM(tlv_0x06->set_max_time, input->set_max_time);

    buf_position += tlv_sz;

    /* pack TLV 0x07 */
    tlv_sz = sizeof(*tlv_0x07);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x07;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x07 = (QMI_GMS_LOC_SET_AUTO_START_request_0x07_t *)buf_position;
    COPY_HTOM(tlv_0x07->max_dist, input->max_dist);

    buf_position += tlv_sz;

    /* pack TLV 0x08 */
    tlv_sz = sizeof(*tlv_0x08);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x08;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x08 = (QMI_GMS_LOC_SET_AUTO_START_request_0x08_t *)buf_position;
    COPY_HTOM(tlv_0x08->set_max_dist, input->set_max_dist);

    buf_position += tlv_sz;

    /* pack TLV 0x09 */
    tlv_sz = sizeof(*tlv_0x09);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x09;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x09 = (QMI_GMS_LOC_SET_AUTO_START_request_0x09_t *)buf_position;
    COPY_HTOM(tlv_0x09->fix_rate, input->fix_rate);

    buf_position += tlv_sz;

    /* pack TLV 0x0A */
    tlv_sz = sizeof(*tlv_0x0A);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x0A;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x0A = (QMI_GMS_LOC_SET_AUTO_START_request_0x0A_t *)buf_position;
    COPY_HTOM(tlv_0x0A->set_fix_rate, input->set_fix_rate);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gms_loc_set_auto_start_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GMS_LOC_SET_NMEA_TYPE (4099 - 0x1003)
 */

QMI_SDK_error_t telit_gms_loc_set_nmea_type_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        gms_loc_set_nmea_type_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GMS_LOC_SET_NMEA_TYPE_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_GMS_LOC_SET_NMEA_TYPE;
    req_ctx->svc_id = QMI_SVC_GMS;
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

    tlv_0x01 = (QMI_GMS_LOC_SET_NMEA_TYPE_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->nmea_mode_ind, input->nmea_mode_ind);

    buf_position += tlv_sz;

    /* pack optional TLV 0x10 */
    if (input->nmea_type_available) {
        QMI_GMS_LOC_SET_NMEA_TYPE_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)(buf_position);
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_GMS_LOC_SET_NMEA_TYPE_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->nmea_type, input->nmea_type);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gms_loc_set_nmea_type_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GMS_LOC_SET_GNSS_SESSION (4103 - 0x1007)
 */

QMI_SDK_error_t telit_gms_loc_set_gnss_session_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        gms_loc_gnss_session_mode_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_GMS_LOC_generic_gnss_session_mode_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        goto end;
    }

    req_ctx->msg_id = QMI_GMS_LOC_SET_GNSS_SESSION;
    req_ctx->svc_id = QMI_SVC_GMS;
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

    tlv_0x01 = (QMI_GMS_LOC_generic_gnss_session_mode_t *)buf_position;
    COPY_HTOM(tlv_0x01->gnss_session_mode, input->gnss_session_mode);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_gms_loc_set_gnss_session_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_GMS_LOC_GET_GNSS_SESSION (4104 - 0x1008)
 */

QMI_SDK_error_t telit_gms_loc_get_gnss_session_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_LOC_GET_GNSS_SESSION,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_loc_get_gnss_session_unpack(uint8_t *rsp,
        uint16_t len,
        gms_loc_gnss_session_mode_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GMS_LOC_generic_gnss_session_mode_t *tlv_0x03;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_generic_gnss_session_mode_t);

    hdr++;
    tlv_0x03 = (QMI_GMS_LOC_generic_gnss_session_mode_t *)hdr;
    COPY_MTOH(out->gnss_session_mode, tlv_0x03->gnss_session_mode);

end:
    return ret;
}

/*****************************************************************************
 * QMI_GMS_LOC_GET_ACQ_POSITION (4105 - 0x1009)
 */

QMI_SDK_error_t telit_gms_loc_get_acq_position_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_GMS_LOC_GET_ACQ_POSITION,
            QMI_SVC_GMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_gms_loc_get_acq_position_unpack(uint8_t *rsp,
        uint16_t len,
        gms_loc_acquired_position_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GMS_LOC_GET_ACQ_POSITION_response_0x03_t *tlv_0x03;
    QMI_GMS_LOC_GET_ACQ_POSITION_response_0x06_t *tlv_0x06;
    QMI_GMS_LOC_GET_ACQ_POSITION_response_0x07_t *tlv_0x07;
    QMI_GMS_LOC_GET_ACQ_POSITION_response_0x08_t *tlv_0x08;
    QMI_GMS_LOC_GET_ACQ_POSITION_response_0x09_t *tlv_0x09;
    QMI_GMS_LOC_GET_ACQ_POSITION_response_0x0A_t *tlv_0x0A;
    QMI_GMS_LOC_GET_ACQ_POSITION_response_0x0B_t *tlv_0x0B;
    QMI_GMS_LOC_GET_ACQ_POSITION_response_0x0D_t *tlv_0x0D;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_ACQ_POSITION_response_0x03_t);

    hdr++;
    tlv_0x03 = (QMI_GMS_LOC_GET_ACQ_POSITION_response_0x03_t *)hdr;
    COPY_MTOH(out->utc_time, tlv_0x03->utc_time);

    ret = get_string(msg, payload_len, 0x04, MAX_STRING_SIZE, out->latitude);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->latitude_len = strlen(out->latitude);

    ret = get_string(msg, payload_len, 0x05, MAX_STRING_SIZE, out->longitude);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->longitude_len = strlen(out->longitude);

    FIND_MANDATORY_TLV(msg, 0x06, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_ACQ_POSITION_response_0x06_t);

    hdr++;
    tlv_0x06 = (QMI_GMS_LOC_GET_ACQ_POSITION_response_0x06_t *)hdr;
    out->horizontal_dop = tlv_0x06->horizontal_dop;

    FIND_MANDATORY_TLV(msg, 0x07, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_ACQ_POSITION_response_0x07_t);

    hdr++;
    tlv_0x07 = (QMI_GMS_LOC_GET_ACQ_POSITION_response_0x07_t *)hdr;
    out->altitude_wrt_sea_level = tlv_0x07->altitude_wrt_sea_level;

    FIND_MANDATORY_TLV(msg, 0x08, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_ACQ_POSITION_response_0x08_t);

    hdr++;
    tlv_0x08 = (QMI_GMS_LOC_GET_ACQ_POSITION_response_0x08_t *)hdr;
    out->fix_type = tlv_0x08->fix_type;

    FIND_MANDATORY_TLV(msg, 0x09, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_ACQ_POSITION_response_0x09_t);

    hdr++;
    tlv_0x09 = (QMI_GMS_LOC_GET_ACQ_POSITION_response_0x09_t *)hdr;
    out->heading = tlv_0x09->heading;

    FIND_MANDATORY_TLV(msg, 0x0A, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_ACQ_POSITION_response_0x0A_t);

    hdr++;
    tlv_0x0A = (QMI_GMS_LOC_GET_ACQ_POSITION_response_0x0A_t *)hdr;
    out->horizontal_speed_km = tlv_0x0A->horizontal_speed_km;

    FIND_MANDATORY_TLV(msg, 0x0B, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_ACQ_POSITION_response_0x0B_t);

    hdr++;
    tlv_0x0B = (QMI_GMS_LOC_GET_ACQ_POSITION_response_0x0B_t *)hdr;
    out->horizontal_speed_kn = tlv_0x0B->horizontal_speed_kn;

    ret = get_string(msg, payload_len, 0x0C, MAX_STRING_SIZE, out->utc_time_date);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->utc_time_date_len = strlen(out->utc_time_date);

    FIND_MANDATORY_TLV(msg, 0x0D, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_LOC_GET_ACQ_POSITION_response_0x0D_t);

    hdr++;
    tlv_0x0D = (QMI_GMS_LOC_GET_ACQ_POSITION_response_0x0D_t *)hdr;
    COPY_MTOH(out->num_gps_sv_in_view, tlv_0x0D->num_gps_sv_in_view);

end:
    return ret;
}

/*****************************************************************************
 * QMI GMS indications
 */

static QMI_SDK_error_t telit_gms_nas_ota_msg_report_unpack(uint8_t *rsp, uint16_t len,
        nas_gms_ota_message_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x01_t *tlv_0x01;
    QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x02_t *tlv_0x02;
    QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x04_t *tlv_0x04;
    QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x05_t *tlv_0x05;
    QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x06_t *tlv_0x06;
    QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x07_t *tlv_0x07;

    payload_len = len;
    msg = rsp;

    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x01_t *)hdr;
    COPY_MTOH(out->msg_type, tlv_0x01->msg_type);

    FIND_MANDATORY_TLV(msg, 0x02, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x02_t);

    hdr++;
    tlv_0x02 = (QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x02_t *)hdr;
    COPY_MTOH(out->data_size, tlv_0x02->data_size);

    if (out->data_size > QMI_GMS_OTA_MSG_MAX_CONTENT_SIZE) {
        /* This is a malformed response, since dictated by the protocol (see doc) */
        RLOGE("%s: Data length exceeds expected maximum value (%d > %d)",
                __FUNCTION__, out->data_size, QMI_GMS_OTA_MSG_MAX_CONTENT_SIZE);
        ret = QMI_SDK_ERR_MALFORMED_RSP;
        goto end;
    }

    ret = get_string(msg, payload_len, 0x03, QMI_GMS_OTA_MSG_MAX_CONTENT_SIZE, (char *)out->data_buf);

    FIND_MANDATORY_TLV(msg, 0x04, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x04_t);

    hdr++;
    tlv_0x04 = (QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x04_t *)hdr;
    COPY_MTOH(out->release, tlv_0x04->release);

    FIND_MANDATORY_TLV(msg, 0x05, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x05_t);

    hdr++;
    tlv_0x05 = (QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x05_t *)hdr;
    COPY_MTOH(out->major, tlv_0x05->major);

    FIND_MANDATORY_TLV(msg, 0x06, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x06_t);

    hdr++;
    tlv_0x06 = (QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x06_t *)hdr;
    COPY_MTOH(out->minor, tlv_0x06->minor);

    FIND_MANDATORY_TLV(msg, 0x07, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x07_t);

    hdr++;
    tlv_0x07 = (QMI_GMS_NAS_OTA_MSG_REPORT_indication_0x07_t *)hdr;
    COPY_MTOH(out->time, tlv_0x07->time);

end:
    return (QMI_SDK_error_t)ret;
}

QMI_SDK_error_t telit_gms_indication_handler(uint16_t msg_id,
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
        case QMI_GMS_NAS_OTA_MSG_REPORT_IND:
            if (out_len < sizeof(nas_gms_ota_message_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(nas_gms_ota_message_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_gms_nas_ota_msg_report_unpack(msg, payload_len, (nas_gms_ota_message_t *)out);
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
