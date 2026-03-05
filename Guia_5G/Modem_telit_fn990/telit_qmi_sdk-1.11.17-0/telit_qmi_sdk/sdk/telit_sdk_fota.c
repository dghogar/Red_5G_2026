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
#include "telit_sdk_private_fota.h"
#include "telit_sdk_fota.h"

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_SET_SERVER_INFO (64 - 0x0040)
 */

QMI_SDK_error_t telit_fota_oem_ftp_set_server_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_server_info_t *input)
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

    if (input->ftp_url_len > QMI_FOTA_MAX_URL_STRING_SIZE - 1) {
        RLOGE("%s: ftp_url_len = %u > QMI_FOTA_MAX_URL_STRING_SIZE, increase the define",
                __FUNCTION__, input->ftp_url_len);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    if (input->delta_descriptor_path_len > QMI_FOTA_MAX_URL_STRING_SIZE - 1) {
        RLOGE("%s: delta_descriptor_path_len = %u > QMI_FOTA_MAX_URL_STRING_SIZE, increase the define",
                __FUNCTION__, input->delta_descriptor_path_len);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    if (input->user_name_len > QMI_FOTA_MAX_USER_STRING_SIZE - 1) {
        RLOGE("%s: user_name_len = %u > QMI_FOTA_MAX_USER_STRING_SIZE, increase the define",
                __FUNCTION__, input->user_name_len);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    if (input->password_len > QMI_FOTA_MAX_PASSWORD_SIZE - 1) {
        RLOGE("%s: password_len = %u > QMI_FOTA_MAX_PASSWORD_SIZE, increase the define",
                __FUNCTION__, input->password_len);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    req_ctx->msg_id = QMI_FOTA_OEM_FTP_SET_SERVER_INFO;
    req_ctx->svc_id = QMI_SVC_FOTA;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = input->ftp_url_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    memcpy(buf_position, input->ftp_url, input->ftp_url_len);

    buf_position += tlv_sz;

    if (input->delta_descriptor_path_len > 0) {
        tlv_sz = input->delta_descriptor_path_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        memcpy(buf_position, input->delta_descriptor_path, input->delta_descriptor_path_len);

        buf_position += tlv_sz;
    }

    if (input->ftp_port_available) {
        QMI_FOTA_generic_ftp_port_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_FOTA_generic_ftp_port_t *)buf_position;
        COPY_HTOM(tlv_0x11->ftp_port, input->ftp_port);

        buf_position += tlv_sz;
    }

    if (input->user_name_len > 0) {
        tlv_sz = input->user_name_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        memcpy(buf_position, input->user_name, input->user_name_len);

        buf_position += tlv_sz;
    }

    if (input->password_len > 0) {
        tlv_sz = input->password_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        memcpy(buf_position, input->password, input->password_len);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_fota_oem_ftp_set_server_info_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_GET_SERVER_INFO (65 - 0x0041)
 */

QMI_SDK_error_t telit_fota_oem_ftp_get_server_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_FOTA_OEM_FTP_GET_SERVER_INFO,
            QMI_SVC_FOTA,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_fota_oem_ftp_get_server_info_unpack(uint8_t *rsp, uint16_t len,
        fota_server_info_t *out)
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

    ret = get_string(msg, payload_len, 0x03, QMI_FOTA_MAX_URL_STRING_SIZE, out->ftp_url);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect FTP url", __FUNCTION__);
        goto end;
    }
    out->ftp_url_len = strlen(out->ftp_url);

    ret = get_string(msg, payload_len, 0x10, QMI_FOTA_MAX_URL_STRING_SIZE, out->delta_descriptor_path);
    if (ret == QMI_SDK_ERR_NONE) {
        out->delta_descriptor_path_len = strlen(out->delta_descriptor_path);
    } else if (ret != QMI_SDK_ERR_MISSING_TLV) {
        RLOGE("%s: unexpected error on optional TLV", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    if (hdr) {
        QMI_FOTA_generic_ftp_port_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_FOTA_generic_ftp_port_t);

        hdr++;
        tlv_0x11 = (QMI_FOTA_generic_ftp_port_t *)hdr;
        COPY_MTOH(out->ftp_port, tlv_0x11->ftp_port);
        out->ftp_port_available = true;
    }

    ret = get_string(msg, payload_len, 0x12, QMI_FOTA_MAX_USER_STRING_SIZE, out->user_name);
    if (ret == QMI_SDK_ERR_NONE) {
        out->user_name_len = strlen(out->user_name);
    } else if (ret != QMI_SDK_ERR_MISSING_TLV) {
        RLOGE("%s: unexpected error on optional TLV", __FUNCTION__);
        goto end;
    }

    ret = get_string(msg, payload_len, 0x13, QMI_FOTA_MAX_PASSWORD_SIZE, out->password);
    if (ret == QMI_SDK_ERR_NONE) {
        out->password_len = strlen(out->password);
    } else if (ret != QMI_SDK_ERR_MISSING_TLV) {
        RLOGE("%s: unexpected error on optional TLV", __FUNCTION__);
        goto end;
    }

    /* Forcing QMI_SDK_ERR_NONE due to optional TLVs missing */
    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_START_SESSION (66 - 0x0042)
 */

QMI_SDK_error_t telit_fota_oem_ftp_start_session_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_session_type_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_FOTA_generic_session_type_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_FOTA_OEM_FTP_START_SESSION;
    req_ctx->svc_id = QMI_SVC_FOTA;
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

    tlv_0x01 = (QMI_FOTA_generic_session_type_t *)buf_position;
    COPY_HTOM(tlv_0x01->session_type, input->session_type);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_fota_oem_ftp_start_session_unpack(uint8_t *rsp, uint16_t len,
        fota_session_status_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_FOTA_OEM_FTP_START_SESSION_response_0x03_t *tlv_0x03;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_FOTA_OEM_FTP_START_SESSION_response_0x03_t);

    hdr++;
    tlv_0x03 = (QMI_FOTA_OEM_FTP_START_SESSION_response_0x03_t *)hdr;
    COPY_MTOH(out->session_response, tlv_0x03->session_response);

end:
    return ret;
}

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_STOP_SESSION (67 - 0x0043)
 */

QMI_SDK_error_t telit_fota_oem_ftp_stop_session_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_session_type_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_FOTA_generic_session_type_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_FOTA_OEM_FTP_STOP_SESSION;
    req_ctx->svc_id = QMI_SVC_FOTA;
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

    tlv_0x01 = (QMI_FOTA_generic_session_type_t *)buf_position;
    COPY_HTOM(tlv_0x01->session_type, input->session_type);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_fota_oem_ftp_stop_session_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_SET_SETTINGS (68 - 0x0044)
 */

QMI_SDK_error_t telit_fota_oem_ftp_set_settings_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_settings_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_FOTA_fw_auto_download_t *tlv_0x01;
    QMI_FOTA_fw_auto_update_t *tlv_0x02;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    if (input->apn_len > QMI_FOTA_MAX_APN_STRING_SIZE - 1) {
        RLOGE("%s: apn_len = %u > QMI_FOTA_MAX_APN_STRING_SIZE, increase the define",
                __FUNCTION__, input->apn_len);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    if (input->username_len > QMI_FOTA_MAX_USERNAME_STRING_SIZE - 1) {
        RLOGE("%s: username_len = %u > QMI_FOTA_MAX_USERNAME_STRING_SIZE, increase the define",
                __FUNCTION__, input->username_len);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    if (input->password_len > QMI_FOTA_MAX_PASSWORD_STRING_SIZE - 1) {
        RLOGE("%s: password_len = %u > QMI_FOTA_MAX_PASSWORD_STRING_SIZE, increase the define",
                __FUNCTION__, input->password_len);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    req_ctx->msg_id = QMI_FOTA_OEM_FTP_SET_SETTINGS;
    req_ctx->svc_id = QMI_SVC_FOTA;
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

    tlv_0x01 = (QMI_FOTA_fw_auto_download_t *)buf_position;
    COPY_HTOM(tlv_0x01->auto_download_firmware, input->auto_download_firmware);

    buf_position += tlv_sz;

    tlv_sz = sizeof(*tlv_0x02);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_FOTA_fw_auto_update_t *)buf_position;
    COPY_HTOM(tlv_0x02->auto_update_firmware, input->auto_update_firmware);

    buf_position += tlv_sz;

    if (input->polling_time_available) {
        QMI_FOTA_OEM_FTP_SET_SETTINGS_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_FOTA_OEM_FTP_SET_SETTINGS_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->value, input->polling_time);

        buf_position += tlv_sz;
    }

    if (input->fw_auto_sdm_available) {
        QMI_FOTA_fw_auto_SDM_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_FOTA_fw_auto_SDM_t *)buf_position;
        COPY_HTOM(tlv_0x11->fw_auto_SDM, input->fw_auto_sdm);

        buf_position += tlv_sz;
    }

    if (input->auto_connect_available) {
        QMI_FOTA_auto_connect_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_FOTA_auto_connect_t *)buf_position;
        COPY_HTOM(tlv_0x12->auto_connect, input->auto_connect);

        buf_position += tlv_sz;
    }

    if (input->auto_reboot_available) {
        QMI_FOTA_auto_reboot_t *tlv_0x13;

        tlv_sz = sizeof(*tlv_0x13);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x13 = (QMI_FOTA_auto_reboot_t *)buf_position;
        COPY_HTOM(tlv_0x13->auto_reboot, input->auto_reboot);

        buf_position += tlv_sz;
    }

    if (input->connection_retry_timers_available) {
        QMI_FOTA_OEM_FTP_SET_SETTINGS_0x14_t *tlv_0x14;
        uint8_t i = 0;

        tlv_sz = sizeof(*tlv_0x14);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x14;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x14 = (QMI_FOTA_OEM_FTP_SET_SETTINGS_0x14_t *)buf_position;

        for (i = 0; i < QMI_FOTA_RETRY_TIMERS; i++)
            COPY_HTOM(tlv_0x14->timers[i], input->connection_retry_timers[i]);

        buf_position += tlv_sz;
    }

    if (input->apn_len > 0) {
        tlv_sz = input->apn_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x15;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        memcpy(buf_position, input->apn, tlv_sz);

        buf_position += tlv_sz;
    }

    if (input->username_len > 0) {
        tlv_sz = input->username_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x16;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        memcpy(buf_position, input->username, tlv_sz);

        buf_position += tlv_sz;
    }

    if (input->password_len > 0) {
        tlv_sz = input->password_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x17;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        memcpy(buf_position, input->password, tlv_sz);

        buf_position += tlv_sz;
    }

    if (input->notif_store_available) {
        QMI_FOTA_notif_store_t *tlv_0x18;

        tlv_sz = sizeof(*tlv_0x18);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x18;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x18 = (QMI_FOTA_notif_store_t *)buf_position;
        COPY_HTOM(tlv_0x18->notif_store, input->notif_store);

        buf_position += tlv_sz;
    }

    if (input->period_info_available) {
        QMI_FOTA_period_info_t *tlv_0x19;

        tlv_sz = sizeof(*tlv_0x19);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x19;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x19 = (QMI_FOTA_period_info_t *)buf_position;
        COPY_HTOM(tlv_0x19->u_min, input->period_info.u_min);
        COPY_HTOM(tlv_0x19->u_max, input->period_info.u_max);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_fota_oem_ftp_set_settings_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_GET_SETTINGS (69 - 0x0045)
 */

QMI_SDK_error_t telit_fota_oem_ftp_get_settings_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_FOTA_OEM_FTP_GET_SETTINGS,
            QMI_SVC_FOTA,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_fota_oem_ftp_get_settings_unpack(uint8_t *rsp, uint16_t len, fota_settings_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_FOTA_fw_auto_download_t *tlv_0x03;
    QMI_FOTA_fw_auto_update_t *tlv_0x04;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x03, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_FOTA_fw_auto_download_t);

    hdr++;
    tlv_0x03 = (QMI_FOTA_fw_auto_download_t *)hdr;

    COPY_MTOH(out->auto_download_firmware, tlv_0x03->auto_download_firmware);

    FIND_MANDATORY_TLV(msg, 0x04, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_FOTA_fw_auto_update_t);

    hdr++;
    tlv_0x04 = (QMI_FOTA_fw_auto_update_t *)hdr;

    COPY_MTOH(out->auto_update_firmware, tlv_0x04->auto_update_firmware);

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_FOTA_OEM_FTP_SET_SETTINGS_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_FOTA_OEM_FTP_SET_SETTINGS_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_FOTA_OEM_FTP_SET_SETTINGS_0x10_t *)hdr;

        COPY_MTOH(out->polling_time, tlv_0x10->value);
        out->polling_time_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_FOTA_fw_auto_SDM_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_FOTA_fw_auto_SDM_t);

        hdr++;
        tlv_0x11 = (QMI_FOTA_fw_auto_SDM_t *)hdr;
        COPY_MTOH(out->fw_auto_sdm, tlv_0x11->fw_auto_SDM);
        out->fw_auto_sdm_available = true;
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* TLV 0x12 is optional */
    if (hdr) {
        QMI_FOTA_auto_connect_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_FOTA_auto_connect_t);

        hdr++;
        tlv_0x12 = (QMI_FOTA_auto_connect_t *)hdr;
        COPY_MTOH(out->auto_connect, tlv_0x12->auto_connect);
        out->auto_connect_available = true;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* TLV 0x13 is optional */
    if (hdr) {
        QMI_FOTA_auto_reboot_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_FOTA_auto_reboot_t);

        hdr++;
        tlv_0x13 = (QMI_FOTA_auto_reboot_t *)hdr;
        COPY_MTOH(out->auto_reboot, tlv_0x13->auto_reboot);
        out->auto_reboot_available = true;
    }

    hdr = search_TLV(msg, 0x14, payload_len);
    /* TLV 0x14 is optional */
    if (hdr) {
        QMI_FOTA_OEM_FTP_SET_SETTINGS_0x14_t *tlv_0x14;
        uint8_t i = 0;

        CHECK_UNPACK_TLV_SIZE(QMI_FOTA_OEM_FTP_SET_SETTINGS_0x14_t);

        hdr++;
        tlv_0x14 = (QMI_FOTA_OEM_FTP_SET_SETTINGS_0x14_t *)hdr;

        for (i = 0; i < QMI_FOTA_RETRY_TIMERS; i++)
            COPY_MTOH(out->connection_retry_timers[i], tlv_0x14->timers[i]);
        out->connection_retry_timers_available = true;
    }

    ret = get_string(msg, payload_len, 0x15, QMI_FOTA_MAX_APN_STRING_SIZE, out->apn);
    if (ret == QMI_SDK_ERR_NONE) {
        out->apn_len = strnlen(out->apn, QMI_FOTA_MAX_APN_STRING_SIZE);
    } else if (ret != QMI_SDK_ERR_MISSING_TLV) {
        RLOGE("%s: unexpected error on optional TLV", __FUNCTION__);
        goto end;
    }

    ret = get_string(msg, payload_len, 0x16, QMI_FOTA_MAX_USERNAME_STRING_SIZE, out->username);
    if (ret == QMI_SDK_ERR_NONE) {
        out->username_len = strnlen(out->username, QMI_FOTA_MAX_USERNAME_STRING_SIZE);
    } else if (ret != QMI_SDK_ERR_MISSING_TLV) {
        RLOGE("%s: unexpected error on optional TLV", __FUNCTION__);
        goto end;
    }

    ret = get_string(msg, payload_len, 0x17, QMI_FOTA_MAX_PASSWORD_STRING_SIZE, out->password);
    if (ret == QMI_SDK_ERR_NONE) {
        out->password_len = strnlen(out->password, QMI_FOTA_MAX_PASSWORD_STRING_SIZE);
    } else if (ret != QMI_SDK_ERR_MISSING_TLV) {
        RLOGE("%s: unexpected error on optional TLV", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x18, payload_len);
    /* TLV 0x18 is optional */
    if (hdr) {
        QMI_FOTA_notif_store_t *tlv_0x18;

        CHECK_UNPACK_TLV_SIZE(QMI_FOTA_notif_store_t);

        hdr++;
        tlv_0x18 = (QMI_FOTA_notif_store_t *)hdr;
        COPY_MTOH(out->notif_store, tlv_0x18->notif_store);
        out->notif_store_available = true;
    }

    hdr = search_TLV(msg, 0x19, payload_len);
    /* TLV 0x19 is optional */
    if (hdr) {
        QMI_FOTA_period_info_t *tlv_0x19;

        CHECK_UNPACK_TLV_SIZE(QMI_FOTA_period_info_t);

        hdr++;
        tlv_0x19 = (QMI_FOTA_period_info_t *)hdr;
        COPY_MTOH(out->period_info.u_min, tlv_0x19->u_min);
        COPY_MTOH(out->period_info.u_max, tlv_0x19->u_max);
        out->period_info_available = true;
    }

    hdr = search_TLV(msg, 0x1A, payload_len);
    /* TLV 0x1A is optional */
    if (hdr) {
        QMI_FOTA_OEM_FTP_GET_SETTINGS_response_0x1A_t *tlv_0x1A;

        CHECK_UNPACK_TLV_SIZE(QMI_FOTA_OEM_FTP_GET_SETTINGS_response_0x1A_t);

        hdr++;
        tlv_0x1A = (QMI_FOTA_OEM_FTP_GET_SETTINGS_response_0x1A_t *)hdr;
        COPY_MTOH(out->oma_dm_enabled, tlv_0x1A->oma_dm_enabled);
        out->oma_dm_enabled_available = true;
    }

    /* Forcing QMI_SDK_ERR_NONE due to optional TLVs missing */
    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_SET_EVENT_REPORT (70 - 0x0046)
 */

QMI_SDK_error_t telit_fota_oem_ftp_set_event_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_set_event_report_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_FOTA_OEM_FTP_SET_EVENT_REPORT_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_FOTA_OEM_FTP_SET_EVENT_REPORT;
    req_ctx->svc_id = QMI_SVC_FOTA;
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

    tlv_0x01 = (QMI_FOTA_OEM_FTP_SET_EVENT_REPORT_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->enable_event, input->enable_event);

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_fota_oem_ftp_set_event_report_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_SEND_SELECTION (71 - 0x0047)
 */

QMI_SDK_error_t telit_fota_oem_ftp_send_selection_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_selection_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_FOTA_OEM_FTP_SEND_SELECTION_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    if (input->reject_reason_len > QMI_FOTA_MAX_REJECT_REASON_SIZE - 1) {
        RLOGE("%s: reject_reason_len = %u > QMI_FOTA_MAX_REJECT_REASON_SIZE, increase the define",
                __FUNCTION__, input->reject_reason_len);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    req_ctx->msg_id = QMI_FOTA_OEM_FTP_SEND_SELECTION;
    req_ctx->svc_id = QMI_SVC_FOTA;
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

    tlv_0x01 = (QMI_FOTA_OEM_FTP_SEND_SELECTION_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->selection, input->selection);

    buf_position += tlv_sz;

    if (input->defer_time_available) {
        QMI_FOTA_OEM_FTP_SEND_SELECTION_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);

        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_FOTA_OEM_FTP_SEND_SELECTION_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->defer_time, input->defer_time);

        buf_position += tlv_sz;
    }

    if (input->reject_reason_len > 0) {
        tlv_sz = input->reject_reason_len;
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        memcpy(buf_position, input->reject_reason, input->reject_reason_len);

        buf_position += tlv_sz;
    }

    if (input->client_perform_operation_available) {
        QMI_FOTA_OEM_FTP_SEND_SELECTION_request_0x12_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_FOTA_OEM_FTP_SEND_SELECTION_request_0x12_t *)buf_position;
        COPY_HTOM(tlv_0x12->client_perform_operation_flag, input->client_perform_operation);

        buf_position += tlv_sz;
    }

    if (input->package_id_available) {
        QMI_FOTA_OEM_FTP_SEND_SELECTION_request_0x13_t *tlv_0x13;

        tlv_sz = sizeof(*tlv_0x13);

        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x13 = (QMI_FOTA_OEM_FTP_SEND_SELECTION_request_0x13_t *)buf_position;
        COPY_HTOM(tlv_0x13->package_id, input->package_id);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_fota_oem_ftp_send_selection_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_FOTA_OEM_ATT_GET_HOST_ODIS (73 - 0x0049)
 */

QMI_SDK_error_t telit_fota_oem_att_get_host_odis_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_FOTA_OEM_ATT_GET_HOST_ODIS,
            QMI_SVC_FOTA,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_fota_oem_att_get_host_odis_unpack(uint8_t *rsp, uint16_t len, fota_host_odis_t *out)
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

    ret = get_string(msg, len, 0x03, MAX_STRING_SIZE + 1, out->man_string);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect source string", __FUNCTION__);
        goto end;
    }
    out->man_string_len = strnlen(out->man_string, MAX_STRING_SIZE);

    ret = get_string(msg, len, 0x04, MAX_STRING_SIZE + 1, out->model_string);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect source string", __FUNCTION__);
        goto end;
    }
    out->model_string_len = strnlen(out->model_string, MAX_STRING_SIZE);

    ret = get_string(msg, len, 0x05, MAX_STRING_SIZE + 1, out->sw_ver_string);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect source string", __FUNCTION__);
        goto end;
    }
    out->sw_ver_string_len = strnlen(out->sw_ver_string, MAX_STRING_SIZE);

    ret = get_string(msg, len, 0x06, MAX_STRING_SIZE + 1, out->plasma_id_string);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect source string", __FUNCTION__);
        goto end;
    }
    out->plasma_id_string_len = strnlen(out->plasma_id_string, MAX_STRING_SIZE);

end:
    return ret;
}

/*****************************************************************************
 * QMI_FOTA_OEM_ATT_SET_HOST_ODIS (74 - 0x004A)
 */

QMI_SDK_error_t telit_fota_oem_att_set_host_odis_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        fota_host_odis_t *input)
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

    if (!input->man_string_len ||
            !input->model_string_len ||
            !input->sw_ver_string_len ||
            !input->plasma_id_string_len) {
        RLOGE("%s: input strings cannot be null", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_FOTA_OEM_ATT_SET_HOST_ODIS;
    req_ctx->svc_id = QMI_SVC_FOTA;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = input->man_string_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *) buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    memcpy(buf_position, input->man_string, input->man_string_len);

    buf_position += tlv_sz;

    tlv_sz = input->model_string_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *) buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    memcpy(buf_position, input->model_string, input->model_string_len);

    buf_position += tlv_sz;

    tlv_sz = input->sw_ver_string_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *) buf_position;
    hdr->type_id = 0x03;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    memcpy(buf_position, input->sw_ver_string, input->sw_ver_string_len);

    buf_position += tlv_sz;

    tlv_sz = input->plasma_id_string_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *) buf_position;
    hdr->type_id = 0x04;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    memcpy(buf_position, input->plasma_id_string, input->plasma_id_string_len);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_fota_oem_att_set_host_odis_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_GET_SESSION_INFO (75 - 0x004B)
 */

QMI_SDK_error_t telit_fota_get_session_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_session_type_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_FOTA_generic_session_type_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_FOTA_OEM_FTP_GET_SESSION_INFO;
    req_ctx->svc_id = QMI_SVC_FOTA;
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

    tlv_0x01 = (QMI_FOTA_generic_session_type_t *)buf_position;
    COPY_HTOM(tlv_0x01->session_type, input->session_type);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

static QMI_SDK_error_t telit_fota_generic_session_info_unpack(uint8_t *rsp, uint16_t len,
        fota_session_info_t *out,
        bool indication)
{
    fota_session_info_t *ev_rep = (fota_session_info_t *) out;
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_FOTA_OEM_FTP_generic_binary_session_info_t *tlv_0x01;
    uint8_t mandatory_tlv = 0;

    memset(ev_rep, 0, sizeof(*out));

    if (indication) {
        RLOGD("%s: parsing indication", __FUNCTION__);

        payload_len = len;
        msg = rsp;
        mandatory_tlv = 0x01;
    } else {
        RLOGD("%s: parsing response", __FUNCTION__);

        ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
        if (ret != QMI_SDK_ERR_NONE) {
            RLOGE("%s: validate unpack failed", __FUNCTION__);
            goto end;
        }

        mandatory_tlv = 0x03;
    }

    FIND_MANDATORY_TLV(msg, mandatory_tlv, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_FOTA_OEM_FTP_generic_binary_session_info_t);

    hdr++;
    tlv_0x01 = (QMI_FOTA_OEM_FTP_generic_binary_session_info_t *)hdr;
    COPY_MTOH(ev_rep->update_info.binary_type, tlv_0x01->binary_type);
    COPY_MTOH(ev_rep->update_info.state, tlv_0x01->state);
    COPY_MTOH(ev_rep->update_info.user_input_request, tlv_0x01->user_input_request);
    COPY_MTOH(ev_rep->update_info.user_input_timeout, tlv_0x01->user_input_timeout);
    COPY_MTOH(ev_rep->update_info.package_size, tlv_0x01->package_size);
    COPY_MTOH(ev_rep->update_info.downloaded_size, tlv_0x01->downloaded_size);
    COPY_MTOH(ev_rep->update_info.update_complete_status, tlv_0x01->update_complete_status);
    COPY_MTOH(ev_rep->update_info.severity, tlv_0x01->severity);

    ret = get_string(msg, payload_len, 0x10, QMI_FOTA_MAX_VERSION_STRING_SIZE, ev_rep->version);
    if (ret == QMI_SDK_ERR_NONE) {
        ev_rep->version_len = strnlen(ev_rep->version, QMI_FOTA_MAX_VERSION_STRING_SIZE);
    } else if (ret != QMI_SDK_ERR_MISSING_TLV) {
        RLOGE("%s: unexpected error on optional TLV", __FUNCTION__);
        goto end;
    }

    ret = get_string(msg, payload_len, 0x11, QMI_FOTA_MAX_VERSION_STRING_SIZE, ev_rep->name);
    if (ret == QMI_SDK_ERR_NONE) {
        ev_rep->name_len = strnlen(ev_rep->name, QMI_FOTA_MAX_VERSION_STRING_SIZE);
    } else if (ret != QMI_SDK_ERR_MISSING_TLV) {
        RLOGE("%s: unexpected error on optional TLV", __FUNCTION__);
        goto end;
    }

    ret = get_string(msg, payload_len, 0x12, QMI_FOTA_MAX_DESCRIPTION_STRING_SIZE, ev_rep->description);
    if (ret == QMI_SDK_ERR_NONE) {
        ev_rep->description_len = strnlen(ev_rep->description, QMI_FOTA_MAX_DESCRIPTION_STRING_SIZE);
    } else if (ret != QMI_SDK_ERR_MISSING_TLV) {
        RLOGE("%s: unexpected error on optional TLV", __FUNCTION__);
        goto end;
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    if (hdr) {
        /* TLV 0x13 is optional */
        QMI_FOTA_OEM_FTP_generic_session_status_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_FOTA_OEM_FTP_generic_session_status_t);

        hdr++;
        tlv_0x13 = (QMI_FOTA_OEM_FTP_generic_session_status_t *)hdr;
        COPY_MTOH(ev_rep->session_status, tlv_0x13->session_status);

        ev_rep->session_status_available = true;
    }

    if (indication) {
        hdr = search_TLV(msg, 0x14, payload_len);
        if (hdr) {
            /* TLV 0x14 is optional */
            QMI_FOTA_OEM_FTP_generic_user_input_t *tlv_0x14;

            CHECK_UNPACK_TLV_SIZE(QMI_FOTA_OEM_FTP_generic_user_input_t);

            hdr++;
            tlv_0x14 = (QMI_FOTA_OEM_FTP_generic_user_input_t *)hdr;
            COPY_MTOH(ev_rep->connection_request.user_input_request, tlv_0x14->user_input_request);
            COPY_MTOH(ev_rep->connection_request.user_input_timeout, tlv_0x14->user_input_timeout);

            ev_rep->connection_request_available = true;
        }

        hdr = search_TLV(msg, 0x15, payload_len);
        if (hdr) {
            /* TLV 0x15 is optional */
            QMI_FOTA_OEM_FTP_generic_update_status_t *tlv_0x15;

            CHECK_UNPACK_TLV_SIZE(QMI_FOTA_OEM_FTP_generic_update_status_t);

            hdr++;
            tlv_0x15 = (QMI_FOTA_OEM_FTP_generic_update_status_t *)hdr;
            COPY_MTOH(ev_rep->update_package_status, tlv_0x15->update_package_status);

            ev_rep->update_package_status_available = true;
        }
    } else {
        hdr = search_TLV(msg, 0x14, payload_len);
        if (hdr) {
            /* TLV 0x14 is optional */
            QMI_FOTA_OEM_FTP_generic_update_status_t *tlv_0x14;

            CHECK_UNPACK_TLV_SIZE(QMI_FOTA_OEM_FTP_generic_update_status_t);

            hdr++;
            tlv_0x14 = (QMI_FOTA_OEM_FTP_generic_update_status_t *)hdr;
            COPY_MTOH(ev_rep->update_package_status, tlv_0x14->update_package_status);

            ev_rep->update_package_status_available = true;
        }
    }

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

QMI_SDK_error_t telit_fota_get_session_info_unpack(uint8_t *rsp, uint16_t len,
        fota_session_info_t *out)
{
    return telit_fota_generic_session_info_unpack(rsp, len, out, false);
}

/*****************************************************************************
 * QMI FOTA indications
 */

QMI_SDK_error_t telit_fota_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len __unused,
        uint8_t *out, uint16_t out_len __unused)
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
        case QMI_FOTA_OEM_FTP_EVENT_REPORT_INDICATOR:
            ret = telit_fota_generic_session_info_unpack(msg, payload_len, (fota_session_info_t *)out, true);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        default:
            RLOGW("%s: indication not managed (%d)", __FUNCTION__, msg_id);
            break;
    }

end:
    return ret;
}
