#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>
#include <string.h>
#if defined __WINDOWS_QMI__
#include "portable_endian.h"
#else
#include <endian.h>
#endif

#include "telit_sdk.h"
#include "telit_sdk_private.h"
#include "telit_sdk_private_dms.h"
#include "telit_sdk_dms.h"

/*****************************************************************************
 * QMI_DMS_SET_EVENT_REPORT (1 - 0x0001)
 */

QMI_SDK_error_t telit_dms_set_event_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        dms_set_event_report_info_t *input)
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

    req_ctx->msg_id = QMI_DMS_SET_EVENT_REPORT;
    req_ctx->svc_id = QMI_SVC_DMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    if (input->operating_mode != QMI_IND_ACTION_NONE) {
        QMI_DMS_SET_EVENT_REPORT_request_0x14_t *tlv_0x14;
        uint32_t hdr_sz = sizeof(*hdr);
        uint16_t tlv_sz = sizeof(*tlv_0x14);

        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x14;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x14 = (QMI_DMS_SET_EVENT_REPORT_request_0x14_t *)buf_position;
        tlv_0x14->report_oprt_mode_state = input->operating_mode;
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_dms_set_event_report_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_DMS_GET_DEVICE_CAP (32 - 0x0020)
 */

QMI_SDK_error_t telit_dms_get_device_cap_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_DMS_GET_DEVICE_CAP,
            QMI_SVC_DMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_dms_get_device_cap_unpack(uint8_t *rsp, uint16_t len, dms_device_caps_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_DMS_GET_DEVICE_CAP_response_0x01_t *tlv_0x01;
    uint8_t *radio_list;
    uint8_t i;

    memset(out, '\0', sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_DMS_GET_DEVICE_CAP_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_DMS_GET_DEVICE_CAP_response_0x01_t *)hdr;

    COPY_MTOH(out->tx_rate, tlv_0x01->max_tx_channel_rate);
    COPY_MTOH(out->rx_rate, tlv_0x01->max_rx_channel_rate);
    COPY_MTOH(out->data_svc_caps, tlv_0x01->data_service_capability);
    COPY_MTOH(out->sim_caps, tlv_0x01->sim_capability);
    COPY_MTOH(out->radio_interfaces_size, tlv_0x01->radio_if_list_size);
    /* No need to check radio_interfaces_size, since it's uint8_t always <= QMI_DMS_DEVICE_CAPS_MAX_RADIO_IF */
    radio_list = (uint8_t *)(&(tlv_0x01->radio_if_list_size));
    /* Moving to the start of the list */
    radio_list += sizeof(tlv_0x01->radio_if_list_size);
    for (i = 0; i < out->radio_interfaces_size; i++) {
        /* Not using COPY_MTOH due to uint8_t types */
        out->radio_interfaces[i] = *radio_list;
        radio_list++;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_DMS_GET_DEVICE_MFR (33 - 0x0021)
 */

QMI_SDK_error_t telit_dms_get_device_mfr_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_DMS_GET_DEVICE_MFR,
            QMI_SVC_DMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_dms_get_device_mfr_unpack(uint8_t *rsp,
        uint16_t len,
        dms_get_manufacturer_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;

    memset(out, '\0', sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    ret = get_string(msg, payload_len, 0x01, MAX_STRING_SIZE, out->manufacturer);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->manufacturer_len = strlen(out->manufacturer);

end:
    return ret;
}

/*****************************************************************************
 * QMI_DMS_GET_DEVICE_MODEL_ID (34 - 0x0022)
 */

QMI_SDK_error_t telit_dms_get_device_model_id_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_DMS_GET_DEVICE_MODEL_ID,
            QMI_SVC_DMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_dms_get_device_model_id_unpack(uint8_t *rsp,
        uint16_t len,
        dms_get_model_id_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;

    memset(out, '\0', sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    ret = get_string(msg, payload_len, 0x01, MAX_STRING_SIZE, out->model_id);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->model_id_len = strlen(out->model_id);

end:
    return ret;
}

/*****************************************************************************
 * QMI_DMS_GET_DEVICE_REV_ID (35 - 0x0023)
 */

QMI_SDK_error_t telit_dms_get_device_rev_id_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_DMS_GET_DEVICE_REV_ID,
            QMI_SVC_DMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_dms_get_device_rev_id_unpack(uint8_t *rsp,
        uint16_t len,
        dms_get_fw_revision_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;

    memset(out, '\0', sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    ret = get_string(msg, payload_len, 0x01, MAX_STRING_SIZE, out->amss);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->amss_len = strlen(out->amss);

    ret = get_string(msg, payload_len, 0x10, MAX_STRING_SIZE, out->boot);
    /* Optional TLV */
    if ((ret != QMI_SDK_ERR_MISSING_TLV) && (ret != QMI_SDK_ERR_NONE)) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->boot_len = strlen(out->boot);

    ret = get_string(msg, payload_len, 0x11, MAX_STRING_SIZE, out->pri);
    /* Optional TLV */
    if ((ret != QMI_SDK_ERR_MISSING_TLV) && (ret != QMI_SDK_ERR_NONE)) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->pri_len = strlen(out->pri);

    /* Forcing QMI_SDK_ERR_NONE due to optional TLVs missing */
    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

/*****************************************************************************
 * QMI_DMS_GET_MSISDN (36 - 0x0024)
 */

QMI_SDK_error_t telit_dms_get_msisdn_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_DMS_GET_MSISDN,
            QMI_SVC_DMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_dms_get_msisdn_unpack(uint8_t *rsp, uint16_t len, dms_voice_number_info_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;

    memset(out, '\0', sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    ret = get_string(msg, payload_len, 0x01, MAX_STRING_SIZE, out->voice_number);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->voice_number_len = strlen(out->voice_number);

    ret = get_string(msg, payload_len, 0x10, MAX_STRING_SIZE, out->mobile_id);
    /* Optional TLV */
    if ((ret != QMI_SDK_ERR_MISSING_TLV) && (ret != QMI_SDK_ERR_NONE)) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->mobile_id_len = strlen(out->mobile_id);

    /* Forcing QMI_SDK_ERR_NONE due to optional TLVs missing */
    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

/*****************************************************************************
 * QMI_DMS_GET_DEVICE_SERIAL_NUMBERS (37 - 0x0025)
 */

QMI_SDK_error_t telit_dms_get_device_serial_numbers_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_DMS_GET_DEVICE_SERIAL_NUMBERS,
            QMI_SVC_DMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_dms_get_device_serial_numbers_unpack(uint8_t *rsp,
        uint16_t len,
        dms_get_serial_numbers_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;

    memset(out, '\0', sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    /* Optional TLV */
    ret = get_string(msg, payload_len, 0x10, MAX_STRING_SIZE, out->esn);
    if ((ret != QMI_SDK_ERR_MISSING_TLV) && (ret != QMI_SDK_ERR_NONE)) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->esn_len = strlen(out->esn);

    ret = get_string(msg, payload_len, 0x11, MAX_STRING_SIZE, out->imei);
    /* Optional TLV */
    if ((ret != QMI_SDK_ERR_MISSING_TLV) && (ret != QMI_SDK_ERR_NONE)) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->imei_len = strlen(out->imei);

    ret = get_string(msg, payload_len, 0x12, MAX_STRING_SIZE, out->meid);
    /* Optional TLV */
    if ((ret != QMI_SDK_ERR_MISSING_TLV) && (ret != QMI_SDK_ERR_NONE)) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->meid_len = strlen(out->meid);

    ret = get_string(msg, payload_len, 0x13, MAX_STRING_SIZE, out->imei_svn);
    /* Optional TLV */
    if ((ret != QMI_SDK_ERR_MISSING_TLV) && (ret != QMI_SDK_ERR_NONE)) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->imei_svn_len = strlen(out->imei_svn);

    /* Forcing QMI_SDK_ERR_NONE due to optional TLVs missing */
    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

/*****************************************************************************
 * QMI_DMS_GET_DEVICE_HARDWARE_REV (44 - 0x002C)
 */

QMI_SDK_error_t telit_dms_get_device_hardware_rev_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_DMS_GET_DEVICE_HARDWARE_REV,
            QMI_SVC_DMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_dms_get_device_hardware_rev_unpack(uint8_t *rsp,
        uint16_t len,
        dms_get_revision_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;

    memset(out, '\0', sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    ret = get_string(msg, payload_len, 0x01, MAX_STRING_SIZE, out->revision);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect string", __FUNCTION__);
        goto end;
    }
    out->revision_len = strlen(out->revision);

end:
    return ret;
}

/*****************************************************************************
 * QMI_DMS_GET_OPERATING_MODE (45 - 0x002D)
 */

QMI_SDK_error_t telit_dms_get_operating_mode_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_DMS_GET_OPERATING_MODE,
            QMI_SVC_DMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_dms_get_operating_mode_unpack(uint8_t *rsp, uint16_t len, dms_power_info_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_DMS_GET_OPERATING_MODE_response_0x01_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_DMS_GET_OPERATING_MODE_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_DMS_GET_OPERATING_MODE_response_0x01_t *)hdr;
    COPY_MTOH(out->power_mode, tlv_0x01->operating_mode);

    hdr = search_TLV(msg, 0x10, payload_len);
    /* TLV 0x10 is optional */
    if (hdr) {
        QMI_DMS_GET_OPERATING_MODE_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_DMS_GET_OPERATING_MODE_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_DMS_GET_OPERATING_MODE_response_0x10_t *)hdr;
        out->offline_reason_available = true;
        COPY_MTOH(out->offline_reason, tlv_0x10->offline_reason);
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* TLV 0x11 is optional */
    if (hdr) {
        QMI_DMS_GET_OPERATING_MODE_response_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_DMS_GET_OPERATING_MODE_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_DMS_GET_OPERATING_MODE_response_0x11_t *)hdr;
        COPY_MTOH(out->hw_controlled_mode, tlv_0x11->hardware_controlled_mode);
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_DMS_SET_OPERATING_MODE (46 - 0x002E)
 */

QMI_SDK_error_t telit_dms_set_operating_mode_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        dms_power_info_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_DMS_SET_OPERATING_MODE_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_DMS_SET_OPERATING_MODE;
    req_ctx->svc_id = QMI_SVC_DMS;
    req_ctx->timeout = QMI_TIMEOUT_MEDIUM;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_DMS_SET_OPERATING_MODE_request_0x01_t *)(buf_position);
    COPY_HTOM(tlv_0x01->power_mode, input->power_mode);

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_dms_set_operating_mode_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_DMS_GET_TIME (47 - 0x002F)
 */

QMI_SDK_error_t telit_dms_get_time_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_DMS_GET_TIME,
            QMI_SVC_DMS,
            QMI_TIMEOUT_SMALL);
}

#define DMS_GET_TIME_TIME_COUNT_SIZE 6
QMI_SDK_error_t telit_dms_get_time_unpack(uint8_t *rsp, uint16_t len, dms_get_time_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_DMS_GET_TIME_response_0x01_t *tlv_0x01;
    uint16_t time_source;
    uint8_t *ptr;
    uint8_t *ptr_out;
    uint8_t i;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_DMS_GET_TIME_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_DMS_GET_TIME_response_0x01_t *)hdr;

    ptr = (uint8_t *)tlv_0x01;
    for (i = 0; i < sizeof(*tlv_0x01); i++)
        RLOGD("%s: payload[%u] 0x%02X", __FUNCTION__, i, *(ptr + i));

    ptr_out = (uint8_t *)&out->time_count;

    /* not converting time_count because specs say little endian */
    for (i = 0; i < DMS_GET_TIME_TIME_COUNT_SIZE; i++) {
        ptr_out[i] = ptr[i];
    }
    ptr_out[6] = 0x00;
    ptr_out[7] = 0x00;

    RLOGD("%s: out->time_count: %" PRIu64, __FUNCTION__, out->time_count);
    for (i = 0; i < sizeof(out->time_count); i++)
        RLOGD("%s: out->time_count[%u] 0x%02X", __FUNCTION__, i, *(ptr_out + i));

    ptr += DMS_GET_TIME_TIME_COUNT_SIZE;
    time_source = (uint16_t)*ptr;

    COPY_MTOH(out->time_source, time_source);

    RLOGD("%s: out->time_source: %u", __FUNCTION__, out->time_source);

    hdr = search_TLV(msg, 0x10, payload_len);
    /* Optional */
    if (hdr) {
        QMI_DMS_GET_TIME_response_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_DMS_GET_TIME_response_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_DMS_GET_TIME_response_0x10_t *)hdr;
        COPY_MTOH(out->sys_time_in_ms, tlv_0x10->sys_time_in_ms);
        out->sys_time_in_ms_available = true;
    }

    hdr = search_TLV(msg, 0x11, payload_len);
    /* Optional */
    if (hdr) {
        QMI_DMS_GET_TIME_response_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_DMS_GET_TIME_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_DMS_GET_TIME_response_0x11_t *)hdr;
        COPY_MTOH(out->user_time_in_ms, tlv_0x11->user_time_in_ms);
        out->user_time_in_ms_available = true;
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_DMS_GET_BAND_CAPABILITY (69 - 0x0045)
 */

QMI_SDK_error_t telit_dms_get_band_capability_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_DMS_GET_BAND_CAPABILITY,
            QMI_SVC_DMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_dms_get_band_capability_unpack(uint8_t *rsp, uint16_t len, dms_band_capabilities_t *out)
{
    QMI_SDK_error_t ret;
    uint16_t payload_len;
    uint8_t *msg;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_DMS_GET_BAND_CAPABILITY_response_0x01_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    ret = validate_unpack(rsp, len, out, true, &payload_len, &msg);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: validate unpack failed", __FUNCTION__);
        goto end;
    }

    /* Defaulting to an invalid value */
    out->band_capabilities = 0xFFFFFFFFFFFFFFFF;

    FIND_MANDATORY_TLV(msg, 0x01, payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_DMS_GET_BAND_CAPABILITY_response_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_DMS_GET_BAND_CAPABILITY_response_0x01_t *)hdr;

    COPY_MTOH(out->band_capabilities, tlv_0x01->band_capability);

    hdr = search_TLV(msg, 0x11, payload_len);
    /* Optional */
    if (hdr) {
        QMI_DMS_GET_BAND_CAPABILITY_response_0x11_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_DMS_GET_BAND_CAPABILITY_response_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_DMS_GET_BAND_CAPABILITY_response_0x11_t *)hdr;
        COPY_MTOH(out->tds_band_capabilities, tlv_0x11->tds_band_capability);
        out->tds_band_capabilities_available = true;
    }

    hdr = search_TLV(msg, 0x12, payload_len);
    /* Optional */
    if (hdr) {
        QMI_DMS_GET_BAND_CAPABILITY_response_0x12_t *tlv_0x12;
        uint16_t *lte_band_item;
        uint8_t i;

        CHECK_UNPACK_TLV_SIZE(QMI_DMS_GET_BAND_CAPABILITY_response_0x12_t);

        hdr++;
        tlv_0x12 = (QMI_DMS_GET_BAND_CAPABILITY_response_0x12_t *)hdr;

        COPY_MTOH(out->supported_lte_bands_capabilities_size, tlv_0x12->supported_lte_bands_size);
        if (out->supported_lte_bands_capabilities_size > QMI_DMS_GET_BAND_CAPS_MAX_SUPPORTED_LTE_BANDS) {
            RLOGE(
                    "%s: supported_lte_bands_capabilities_size = %u > QMI_DMS_GET_BAND_CAPS_MAX_SUPPORTED_LTE_BANDS, increase the define",
                    __FUNCTION__, out->supported_lte_bands_capabilities_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        lte_band_item = (uint16_t *)(&(tlv_0x12->supported_lte_bands_size));
        /* Moving to the start of the list */
        lte_band_item++;  /* the pointer is a uint16_t so we are incrementing 2 bytes here */
        for (i = 0; i < out->supported_lte_bands_capabilities_size; i++) {
            COPY_MTOH(out->supported_lte_bands[i], lte_band_item[i]);
        }
    }

    hdr = search_TLV(msg, 0x13, payload_len);
    /* Optional */
    if (hdr) {
        QMI_DMS_GET_BAND_CAPABILITY_response_0x13_t *tlv_0x13;
        uint16_t *nr5g_band_item;
        uint8_t i;

        CHECK_UNPACK_TLV_SIZE(QMI_DMS_GET_BAND_CAPABILITY_response_0x13_t);

        hdr++;
        tlv_0x13 = (QMI_DMS_GET_BAND_CAPABILITY_response_0x13_t *)hdr;

        COPY_MTOH(out->supported_nr5g_bands_capabilities_size, tlv_0x13->supported_nr5g_bands_size);
        if (out->supported_nr5g_bands_capabilities_size > QMI_DMS_GET_BAND_CAPS_MAX_SUPPORTED_NR5G_BANDS) {
            RLOGE(
                    "%s: supported_nr5g_bands_capabilities_size = %u > QMI_DMS_GET_BAND_CAPS_MAX_SUPPORTED_NR5G_BANDS, increase the define",
                    __FUNCTION__, out->supported_nr5g_bands_capabilities_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        nr5g_band_item = (uint16_t *)(&(tlv_0x13->supported_nr5g_bands_size));
        /* Moving to the start of the list */
        nr5g_band_item++;  /* the pointer is a uint16_t so we are incrementing 2 bytes here */
        for (i = 0; i < out->supported_nr5g_bands_capabilities_size; i++) {
            COPY_MTOH(out->supported_nr5g_bands[i], nr5g_band_item[i]);
        }
    }

end:
    return ret;
}

/*****************************************************************************
 * QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION (94 - 0x005e)
 */

QMI_SDK_error_t telit_dms_trigger_modem_activity_info_calculation_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        dms_modem_activity_info_calculation_t *input)
{
    QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION_request_0x10_t *tlv_0x10;
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint8_t *original_buffer;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION;
    req_ctx->svc_id = QMI_SVC_DMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x10);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x10;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x10 = (QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION_request_0x10_t *)(buf_position);
    COPY_HTOM(tlv_0x10->enable_statistics, input->enable_statistics);

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_dms_trigger_modem_activity_info_calculation_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_DMS_GET_MODEM_ACTIVITY_INFO (95 - 0x005f)
 */

QMI_SDK_error_t telit_dms_get_modem_activity_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_DMS_GET_MODEM_ACTIVITY_INFO,
            QMI_SVC_DMS,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_dms_get_modem_activity_info_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_DMS_DEVICE_POWER_INFO (115 - 0x0073)
 */

QMI_SDK_error_t telit_dms_device_power_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        dms_modem_device_power_info_t *input)
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

    req_ctx->msg_id = QMI_DMS_DEVICE_POWER_INFO;
    req_ctx->svc_id = QMI_SVC_DMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    /* Add TLV 0x10 */
    if (input->power_save_mode_status_available) {
        QMI_DMS_DEVICE_POWER_INFO_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_DMS_DEVICE_POWER_INFO_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->power_save_mode_status, input->power_save_mode_status);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x11 */
    if (input->battery_charge_state_available) {
        QMI_DMS_DEVICE_POWER_INFO_request_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_DMS_DEVICE_POWER_INFO_request_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->battery_charge_state, input->battery_charge_state);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x12 */
    if (input->battery_capacity_available) {
        QMI_DMS_DEVICE_POWER_INFO_request_0x12_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_DMS_DEVICE_POWER_INFO_request_0x12_t *)buf_position;
        COPY_HTOM(tlv_0x12->battery_capacity, input->battery_capacity);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x13 */
    if (input->battery_level_available) {
        QMI_DMS_DEVICE_POWER_INFO_request_0x13_t *tlv_0x13;

        tlv_sz = sizeof(*tlv_0x13);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x13 = (QMI_DMS_DEVICE_POWER_INFO_request_0x13_t *)buf_position;
        COPY_HTOM(tlv_0x13->battery_level, input->battery_level);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x14 */
    if (input->battery_level_index_available) {
        QMI_DMS_DEVICE_POWER_INFO_request_0x14_t *tlv_0x14;

        tlv_sz = sizeof(*tlv_0x14);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x14;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x14 = (QMI_DMS_DEVICE_POWER_INFO_request_0x14_t *)buf_position;
        COPY_HTOM(tlv_0x14->battery_level_index, input->battery_level_index);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_dms_device_power_info_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_DMS_SET_AT_IND_REPORT (21852 - 0x555c)
 */

QMI_SDK_error_t telit_dms_set_at_ind_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        dms_at_ind_report_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_DMS_SET_AT_IND_REPORT_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_DMS_SET_AT_IND_REPORT;
    req_ctx->svc_id = QMI_SVC_DMS;
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

    tlv_0x01 = (QMI_DMS_SET_AT_IND_REPORT_request_0x01_t *)(buf_position);
    COPY_HTOM(tlv_0x01->at_command_resp_enable, input->at_command_resp_enable);

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_dms_set_at_ind_report_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_DMS_SEND_AT_COMMAND (21853 - 0x555d)
 */

QMI_SDK_error_t telit_dms_send_at_command_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        dms_at_command_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_DMS_SEND_AT_COMMAND_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    if (input->at_command_len > QMI_DMS_AT_COMMAND_MAX_LEN) {
        RLOGE("%s: at_command_len = %u > QMI_DMS_AT_COMMAND_MAX_LEN, increase the define",
                __FUNCTION__, input->at_command_len);
        ret = QMI_SDK_ERR_INTERNAL;
        goto end;
    }

    req_ctx->msg_id = QMI_DMS_SEND_AT_COMMAND;
    req_ctx->svc_id = QMI_SVC_DMS;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01) + input->at_command_len;
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)(buf_position);
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_DMS_SEND_AT_COMMAND_request_0x01_t *)(buf_position);
    /* Foxconn screwed up... */
    tlv_0x01->at_command_len = htobe16(input->at_command_len);
    buf_position += sizeof(tlv_0x01->at_command_len);
    memcpy(buf_position, input->at_command, input->at_command_len);

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_dms_send_at_command_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI DMS indications
 */

static QMI_SDK_error_t telit_dms_handle_event_ind(uint16_t *payload_len, uint8_t *msg,
        uint8_t *out, uint16_t out_len)
{
    dms_ind_event_report_t *ev_rep = (dms_ind_event_report_t *) out;
    QMI_SDK_error_t ret;
    QMI_raw_content_header_t *hdr = NULL;

    if (out_len < sizeof(*ev_rep)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }

    memset(ev_rep, 0, sizeof(*ev_rep));

    hdr = search_TLV(msg, 0x14, *payload_len);
    if (hdr) {
        QMI_DMS_EVENT_REPORT_indication_0x14_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_DMS_EVENT_REPORT_indication_0x14_t *)hdr;
        ev_rep->operating_mode_available = true;
        COPY_MTOH(ev_rep->operating_mode, tlv->operating_mode);
    }

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

static QMI_SDK_error_t telit_dms_handle_modem_activity_info_ind(uint16_t *payload_len, uint8_t *msg,
        uint8_t *out, uint16_t out_len)
{
    dms_modem_activity_info_t *ev_rep = (dms_modem_activity_info_t *) out;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_SDK_error_t ret;

    if (out_len < sizeof(*ev_rep)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }

    memset(ev_rep, 0, sizeof(*ev_rep));

    hdr = search_TLV(msg, 0x10, *payload_len);
    if (hdr) {
        QMI_DMS_MODEM_ACTIVITY_INFO_indication_0x10_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_DMS_MODEM_ACTIVITY_INFO_indication_0x10_t *)hdr;
        ev_rep->idle_mode_duration_available = true;
        COPY_MTOH(ev_rep->idle_mode_duration, tlv->idle_mode_duration);
    }

    hdr = search_TLV(msg, 0x11, *payload_len);
    if (hdr) {
        QMI_DMS_MODEM_ACTIVITY_INFO_indication_0x11_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_DMS_MODEM_ACTIVITY_INFO_indication_0x11_t *)hdr;
        ev_rep->sleep_mode_duration_available = true;
        COPY_MTOH(ev_rep->sleep_mode_duration, tlv->sleep_mode_duration);
    }

    hdr = search_TLV(msg, 0x12, *payload_len);
    if (hdr) {
        QMI_DMS_MODEM_ACTIVITY_INFO_indication_0x12_t *tlv;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_DMS_MODEM_ACTIVITY_INFO_indication_0x12_t *)hdr;
        ev_rep->rx_mode_duration_available = true;
        COPY_MTOH(ev_rep->rx_mode_duration, tlv->rx_mode_duration);
    }

    hdr = search_TLV(msg, 0x13, *payload_len);
    if (hdr) {
        QMI_DMS_MODEM_ACTIVITY_INFO_indication_0x13_t *tlv;
        tx_mode_duration_t *instance;
        uint8_t *tx_duration_list;

        CHECK_UNPACK_TLV_SIZE(*tlv);

        hdr++;
        tlv = (QMI_DMS_MODEM_ACTIVITY_INFO_indication_0x13_t *)hdr;

        COPY_MTOH(ev_rep->tx_mode_duration_size, tlv->tx_mode_duration_size);
        if (ev_rep->tx_mode_duration_size > QMI_DMS_MODEM_ACTIVITY_MAX_SUPPORTED_TX_MODES) {
            RLOGE(
                    "%s: tx_mode_duration_size = %u > QMI_DMS_MODEM_ACTIVITY_MAX_SUPPORTED_TX_MODES, increase the define",
                    __FUNCTION__, ev_rep->tx_mode_duration_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        tx_duration_list = (uint8_t *)(&(tlv->tx_mode_duration_size));
        /* Moving to the start of the list */
        tx_duration_list += sizeof(tlv->tx_mode_duration_size);
        for (int i = 0; i < ev_rep->tx_mode_duration_size; i++) {
            instance = (tx_mode_duration_t *)tx_duration_list;
            COPY_MTOH(ev_rep->tx_mode_duration[i].tx_mode_level, instance->tx_mode_level);
            COPY_MTOH(ev_rep->tx_mode_duration[i].tx_mode_duration, instance->tx_mode_duration);
            tx_duration_list += sizeof(*instance);
        }
    }

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

static QMI_SDK_error_t telit_dms_handle_at_command_ind(uint16_t *payload_len, uint8_t *msg,
        uint8_t *out, uint16_t out_len)
{
    QMI_SDK_error_t ret;
    dms_at_command_t *at_command_answer =  (dms_at_command_t *) out;
    QMI_DMS_SEND_AT_COMMAND_request_0x01_t *tlv_0x01;
    QMI_raw_content_header_t *hdr = NULL;
    uint8_t *buf_position;

    if (out_len < sizeof(*at_command_answer)) {
        RLOGE("%s: invalid buffer size", __FUNCTION__);
        ret = QMI_SDK_ERR_BUFFER_SZ;
        goto end;
    }

    memset(at_command_answer, 0, sizeof(*at_command_answer));

    FIND_MANDATORY_TLV(msg, 0x01, *payload_len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_DMS_SEND_AT_COMMAND_request_0x01_t);
    /* Foxconn screwed-up also this...
     * This could have been a string TLV, but they added a magic number 0x04
     * before the string, without adding any other field indicating the length
     * So to retrieve the size we should take the tlv length and removing 1 for
     * the magic number */
    at_command_answer->at_command_len = le16toh(hdr->length) - 1;

    hdr++;
    tlv_0x01 = (QMI_DMS_SEND_AT_COMMAND_request_0x01_t *)hdr;
    buf_position = (uint8_t *)tlv_0x01;
    /* Skipping the magic number 0x04 */
    buf_position++;
    memcpy(at_command_answer->at_command, buf_position, at_command_answer->at_command_len);

    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

QMI_SDK_error_t telit_dms_indication_handler(uint16_t msg_id,
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
        case QMI_DMS_EVENT_IND:
            ret = telit_dms_handle_event_ind(&payload_len, msg, out, out_len);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_DMS_MODEM_ACTIVITY_INFO_IND:
            ret = telit_dms_handle_modem_activity_info_ind(&payload_len, msg, out, out_len);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_DMS_AT_COMMAND_IND:
            ret = telit_dms_handle_at_command_ind(&payload_len, msg, out, out_len);
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
