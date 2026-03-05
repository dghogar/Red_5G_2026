#include <stdint.h>
#include <stddef.h>
#include <string.h>
#if defined __WINDOWS_QMI__
#include "portable_endian.h"
#else
#include <endian.h>
#endif

#include "telit_sdk.h"
#include "telit_sdk_private.h"
#include "telit_sdk_private_loc.h"
#include "telit_sdk_loc.h"

/*****************************************************************************
 * QMI_LOC_REG_EVENTS (33 - 0x0021)
 */

QMI_SDK_error_t telit_loc_reg_events_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_event_register_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *original_buffer;
    uint8_t *buf_position;
    QMI_LOC_REG_EVENTS_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_LOC_REG_EVENTS;
    req_ctx->svc_id = QMI_SVC_LOC;
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

    tlv_0x01 = (QMI_LOC_REG_EVENTS_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->event_mask, input->event_mask);

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_loc_reg_events_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_LOC_START (34 - 0x0022)
 */

QMI_SDK_error_t telit_loc_start_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        loc_start_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_LOC_generic_session_id_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_LOC_START;
    req_ctx->svc_id = QMI_SVC_LOC;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    /* Add mandatory TLV 0x01 */
    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_LOC_generic_session_id_t *)buf_position;
    COPY_HTOM(tlv_0x01->session_id, input->session_id);

    buf_position += tlv_sz;

    /* Add TLV 0x10 */
    if (input->recurrence_available) {
        QMI_LOC_START_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_LOC_START_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->recurrence, input->recurrence);

        buf_position += tlv_sz;
    }

    /* Add TLV 0x11 */
    if (input->accuracy_available) {
        QMI_LOC_START_request_0x11_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_LOC_START_request_0x11_t *)buf_position;
        COPY_HTOM(tlv_0x11->accuracy, input->accuracy);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_loc_start_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_LOC_STOP (35 - 0x0023)
 */

QMI_SDK_error_t telit_loc_stop_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        loc_stop_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_LOC_generic_session_id_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_LOC_STOP;
    req_ctx->svc_id = QMI_SVC_LOC;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    /* Add mandatory TLV 0x01 */
    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_LOC_generic_session_id_t *)buf_position;

    COPY_HTOM(tlv_0x01->session_id, input->session_id);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_loc_stop_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_LOC_INJECT_UTC_TIME (56 - 0x0038)
 */

QMI_SDK_error_t telit_loc_inject_utc_time_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_utc_time_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_LOC_INJECT_UTC_TIME_request_0x01_t *tlv_0x01;
    QMI_LOC_INJECT_UTC_TIME_request_0x02_t *tlv_0x02;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_LOC_INJECT_UTC_TIME;
    req_ctx->svc_id = QMI_SVC_LOC;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    /* Add mandatory TLV 0x01 */
    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_LOC_INJECT_UTC_TIME_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->time_utc, input->time_utc);

    buf_position += tlv_sz;

    tlv_sz = sizeof(*tlv_0x02);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x02;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x02 = (QMI_LOC_INJECT_UTC_TIME_request_0x02_t *)buf_position;
    COPY_HTOM(tlv_0x02->time_unc, input->time_unc);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_loc_inject_utc_time_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_LOC_INJECT_POSITION (57 - 0x0039)
 */

QMI_SDK_error_t telit_loc_inject_position_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_position_t *input)
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

    req_ctx->msg_id = QMI_LOC_INJECT_POSITION;
    req_ctx->svc_id = QMI_SVC_LOC;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->latitude_available) {
        QMI_LOC_generic_double_value_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_LOC_generic_double_value_t *)buf_position;
        tlv_0x10->value = input->latitude;
        buf_position += tlv_sz;
    }

    if (input->longitude_available) {
        QMI_LOC_generic_double_value_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_LOC_generic_double_value_t *)buf_position;
        tlv_0x11->value = input->longitude;
        buf_position += tlv_sz;
    }

    if (input->hor_unc_circular_available) {
        QMI_LOC_generic_float_value_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_LOC_generic_float_value_t *)buf_position;
        tlv_0x12->value = input->hor_unc_circular;
        buf_position += tlv_sz;
    }

    if (input->position_source_available) {
        QMI_LOC_generic_uint32_value_t *tlv_0x1D;

        tlv_sz = sizeof(*tlv_0x1D);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x1D;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x1D = (QMI_LOC_generic_uint32_value_t *)buf_position;
        COPY_HTOM(tlv_0x1D->value, input->position_source);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_loc_inject_position_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_LOC_SET_NMEA_TYPES (62 - 0x003E)
 */

QMI_SDK_error_t telit_loc_set_nmea_types_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        loc_nmea_types_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_LOC_SET_NMEA_TYPES_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_LOC_SET_NMEA_TYPES;
    req_ctx->svc_id = QMI_SVC_LOC;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    /* Add mandatory TLV 0x01 */
    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_LOC_SET_NMEA_TYPES_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->nmea_sentence_type, input->nmea_sentence_type);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_loc_set_nmea_types_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_LOC_DELETE_ASSIST_DATA (68 - 0x0044)
 */

QMI_SDK_error_t telit_loc_delete_assist_data_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        loc_delete_assist_data_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_LOC_DELETE_ASSIST_DATA_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_LOC_DELETE_ASSIST_DATA;
    req_ctx->svc_id = QMI_SVC_LOC;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    /* Add mandatory TLV 0x01 */
    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_LOC_DELETE_ASSIST_DATA_request_0x01_t *)buf_position;

    if (input->delete_all_flag) {
        /* if this flag is set, all the other information contained in the
         * optional fields for this message are ignored */
        tlv_0x01->delete_all_flag = 0x01;
    } else {
        tlv_0x01->delete_all_flag = 0x00;

        buf_position += tlv_sz;

        /* Add optional TLV 0x10 */
        if (input->sv_info_available) {
            QMI_LOC_DELETE_ASSIST_DATA_request_0x10_t *tlv_0x10;
            sv_info_instance_t *sv_info_inst = NULL;
            uint8_t i;

            if (input->sv_info.sv_info_size > QMI_LOC_MAX_GNSS_INFO) {
                RLOGE("%s: sv_info_size = %u > QMI_LOC_MAX_GNSS_INFO, increase the define",
                        __FUNCTION__, input->sv_info.sv_info_size);
                ret = QMI_SDK_ERR_INTERNAL;
                goto end;
            }

            tlv_sz = sizeof(*tlv_0x10) +
                    (sizeof(*sv_info_inst) * input->sv_info.sv_info_size);
            CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

            hdr = (QMI_raw_content_header_t *)buf_position;
            hdr->type_id = 0x10;
            hdr->length = htole16(tlv_sz);

            buf_position += hdr_sz;

            tlv_0x10 = (QMI_LOC_DELETE_ASSIST_DATA_request_0x10_t *)buf_position;
            COPY_HTOM(tlv_0x10->sv_info_list_len, input->sv_info.sv_info_size);
            buf_position += sizeof(*tlv_0x10);
            sv_info_inst = (sv_info_instance_t *)buf_position;

            for (i = 0; i < input->sv_info.sv_info_size; i++) {
                COPY_HTOM(sv_info_inst[i].sv_id, input->sv_info.sv_inst[i].gnss_sv_id);
                COPY_HTOM(sv_info_inst[i].system, input->sv_info.sv_inst[i].system);
                COPY_HTOM(sv_info_inst[i].sv_info_mask, input->sv_info.sv_inst[i].sv_mask);
            }

            buf_position += sizeof(*sv_info_inst) * input->sv_info.sv_info_size;
        }

        /* Add optional TLV 0x11 */
        if (input->gnss_info_available) {
            QMI_LOC_DELETE_ASSIST_DATA_request_0x11_t *tlv_0x11;

            tlv_sz = sizeof(*tlv_0x11);
            CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

            hdr = (QMI_raw_content_header_t *)buf_position;
            hdr->type_id = 0x11;
            hdr->length = htole16(tlv_sz);

            buf_position += hdr_sz;

            tlv_0x11 = (QMI_LOC_DELETE_ASSIST_DATA_request_0x11_t *)buf_position;
            COPY_HTOM(tlv_0x11->gnss_mask, input->gnss_info);
            buf_position += tlv_sz;
        }

        /* Add optional TLV 0x12 */
        if (input->cell_db_info_available) {
            QMI_LOC_DELETE_ASSIST_DATA_request_0x12_t *tlv_0x12;

            tlv_sz = sizeof(*tlv_0x12);
            CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

            hdr = (QMI_raw_content_header_t *)buf_position;
            hdr->type_id = 0x12;
            hdr->length = htole16(tlv_sz);

            buf_position += hdr_sz;

            tlv_0x12 = (QMI_LOC_DELETE_ASSIST_DATA_request_0x12_t *)buf_position;
            COPY_HTOM(tlv_0x12->cell_db_mask, input->cell_db_info);

            buf_position += tlv_sz;
        }

        /* Add optional TLV 0x13 */
        if (input->clock_info_available) {
            QMI_LOC_DELETE_ASSIST_DATA_request_0x13_t *tlv_0x13;

            tlv_sz = sizeof(*tlv_0x13);
            CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

            hdr = (QMI_raw_content_header_t *)buf_position;
            hdr->type_id = 0x13;
            hdr->length = htole16(tlv_sz);

            buf_position += hdr_sz;

            tlv_0x13 = (QMI_LOC_DELETE_ASSIST_DATA_request_0x13_t *)buf_position;
            COPY_HTOM(tlv_0x13->clock_info_mask, input->clock_info);

            buf_position += tlv_sz;
        }

        /* Add optional TLV 0x14 */
        if (input->bds_sv_info_available) {
            QMI_LOC_DELETE_ASSIST_DATA_generic_sv_info_t *tlv_0x14;
            generic_sv_info_instance_t *bds_sv_info_inst = NULL;
            uint8_t i = 0;

            if (input->bds_sv_info.bds_sv_info_size > QMI_LOC_MAX_GNSS_INFO) {
                RLOGE("%s: bds_sv_info_size = %u > QMI_LOC_MAX_GNSS_INFO, increase the define",
                        __FUNCTION__, input->bds_sv_info.bds_sv_info_size);
                ret = QMI_SDK_ERR_INTERNAL;
                goto end;
            }

            tlv_sz = sizeof(*tlv_0x14) +
                    (sizeof(*bds_sv_info_inst) * input->bds_sv_info.bds_sv_info_size);
            CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

            hdr = (QMI_raw_content_header_t *)buf_position;
            hdr->type_id = 0x14;
            hdr->length = htole16(tlv_sz);

            buf_position += hdr_sz;

            tlv_0x14 = (QMI_LOC_DELETE_ASSIST_DATA_generic_sv_info_t *)buf_position;
            COPY_HTOM(tlv_0x14->sv_info_list_len, input->bds_sv_info.bds_sv_info_size);
            buf_position += sizeof(*tlv_0x14);
            bds_sv_info_inst = (generic_sv_info_instance_t *)buf_position;

            for (i = 0; i < input->bds_sv_info.bds_sv_info_size; i++) {
                COPY_HTOM(bds_sv_info_inst[i].gnss_sv_id, input->bds_sv_info.bds_sv_inst[i].gnss_sv_id);
                COPY_HTOM(bds_sv_info_inst[i].sv_info_mask, input->bds_sv_info.bds_sv_inst[i].sv_mask);
            }

            buf_position += sizeof(*bds_sv_info_inst) * input->bds_sv_info.bds_sv_info_size;
        }

        /* Add optional TLV 0x15 */
        if (input->gal_sv_info_available) {
            QMI_LOC_DELETE_ASSIST_DATA_generic_sv_info_t *tlv_0x15;
            generic_sv_info_instance_t *gal_sv_info_inst = NULL;
            uint8_t i = 0;

            if (input->gal_sv_info.gal_sv_info_size > QMI_LOC_MAX_GNSS_INFO) {
                RLOGE("%s: gal_sv_info_size = %u > QMI_LOC_MAX_GNSS_INFO, increase the define",
                        __FUNCTION__, input->gal_sv_info.gal_sv_info_size);
                ret = QMI_SDK_ERR_INTERNAL;
                goto end;
            }

            tlv_sz = sizeof(*tlv_0x15) +
                    (sizeof(*gal_sv_info_inst) * input->gal_sv_info.gal_sv_info_size);
            CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

            hdr = (QMI_raw_content_header_t *)buf_position;
            hdr->type_id = 0x15;
            hdr->length = htole16(tlv_sz);

            buf_position += hdr_sz;

            tlv_0x15 = (QMI_LOC_DELETE_ASSIST_DATA_generic_sv_info_t *)buf_position;
            COPY_HTOM(tlv_0x15->sv_info_list_len, input->gal_sv_info.gal_sv_info_size);
            buf_position += sizeof(*tlv_0x15);
            gal_sv_info_inst = (generic_sv_info_instance_t *)buf_position;

            for (i = 0; i < input->gal_sv_info.gal_sv_info_size; i++) {
                COPY_HTOM(gal_sv_info_inst[i].gnss_sv_id, input->gal_sv_info.gal_sv_inst[i].gal_sv_id);
                COPY_HTOM(gal_sv_info_inst[i].sv_info_mask, input->gal_sv_info.gal_sv_inst[i].gal_sv_mask);
            }
        }
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_loc_delete_assist_data_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_LOC_SET_OPERATION_MODE (74 - 0x004A)
 */

QMI_SDK_error_t telit_loc_set_operation_mode_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        loc_operation_mode_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_LOC_SET_OPERATION_MODE_request_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_LOC_SET_OPERATION_MODE;
    req_ctx->svc_id = QMI_SVC_LOC;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    /* Add mandatory TLV 0x01 */
    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_LOC_SET_OPERATION_MODE_request_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->operation_mode, input->operation_mode);

    buf_position += tlv_sz;

    /* Add optional TLV 0x10 */
    if (input->min_interval_available) {
        QMI_LOC_SET_OPERATION_MODE_request_0x10_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_LOC_SET_OPERATION_MODE_request_0x10_t *)buf_position;
        COPY_HTOM(tlv_0x10->min_interval, input->min_interval);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_loc_set_operation_mode_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_LOC_GET_OPERATION_MODE (75 - 0x004B)
 */

QMI_SDK_error_t telit_loc_get_operation_mode_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_LOC_GET_OPERATION_MODE,
            QMI_SVC_LOC,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_loc_get_operation_mode_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_LOC_GET_TTFF_FOXCONN (4103 - 0x1007)
 */

QMI_SDK_error_t telit_loc_get_ttff_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len)
{
    return generic_pack_request(req_ctx, req, len,
            QMI_LOC_GET_TTFF_FOXCONN,
            QMI_SVC_LOC,
            QMI_TIMEOUT_SMALL);
}

QMI_SDK_error_t telit_loc_get_ttff_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_LOC_SET_ENGINE_LOCK (58 - 0x003A)
 */

QMI_SDK_error_t telit_loc_set_engine_lock_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        loc_set_engine_lock_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_LOC_generic_lock_type_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_LOC_SET_ENGINE_LOCK;
    req_ctx->svc_id = QMI_SVC_LOC;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    /* Add mandatory TLV 0x01 */
    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_LOC_generic_lock_type_t *)buf_position;
    COPY_HTOM(tlv_0x01->lock_type, input->lock_type);

    buf_position += tlv_sz;

    /* Add optional TLV 0x10 */
    if (input->subscription_type_available) {
        QMI_LOC_generic_subscription_type_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_LOC_generic_subscription_type_t *)buf_position;
        COPY_HTOM(tlv_0x10->sub_type, input->subscription_type);
        buf_position += tlv_sz;
    }

    /* Add optional TLV 0x11 */
    if (input->lock_client_available) {
        QMI_LOC_generic_lock_client_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_LOC_generic_lock_client_t *)buf_position;
        COPY_HTOM(tlv_0x11->lock_client, input->lock_client);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_loc_set_engine_lock_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_LOC_GET_ENGINE_LOCK (59 - 0x003B)
 */

QMI_SDK_error_t telit_loc_get_engine_lock_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        loc_get_engine_lock_t *input)
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

    req_ctx->msg_id = QMI_LOC_GET_ENGINE_LOCK;
    req_ctx->svc_id = QMI_SVC_LOC;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    /* Add optional TLV 0x10 */
    if (input->subscription_type_available) {
        QMI_LOC_generic_subscription_type_t *tlv_0x10;

        hdr_sz = sizeof(*hdr);
        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_LOC_generic_subscription_type_t *)buf_position;
        COPY_HTOM(tlv_0x10->sub_type, input->subscription_type);
    }

    add_header(req_ctx, payload_len, original_buffer, len);
    ret = QMI_SDK_ERR_NONE;

end:
    return ret;
}

QMI_SDK_error_t telit_loc_get_engine_lock_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI_LOC_DELETE_GNSS_SERVICE_DATA (166 - 0x00A6)
 */

QMI_SDK_error_t telit_loc_delete_gnss_service_data_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        loc_delete_gnss_service_data_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_LOC_DELETE_GNSS_SERVICE_DATA_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_LOC_DELETE_GNSS_SERVICE_DATA;
    req_ctx->svc_id = QMI_SVC_LOC;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    /* Add mandatory TLV 0x01 */
    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_LOC_DELETE_GNSS_SERVICE_DATA_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->delete_all_flag, input->delete_all);

    buf_position += tlv_sz;

    /* Add optional TLV 0x10 */
    if (input->clock_info_mask_available) {
        QMI_LOC_generic_mask_32_t *tlv_0x10;

        tlv_sz = sizeof(*tlv_0x10);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x10;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x10 = (QMI_LOC_generic_mask_32_t *)buf_position;
        COPY_HTOM(tlv_0x10->mask_32, input->clock_info_mask);
        buf_position += tlv_sz;
    }

    /* Add optional TLV 0x11 */
    if (input->cell_db_data_mask_available) {
        QMI_LOC_generic_mask_32_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_LOC_generic_mask_32_t *)buf_position;
        COPY_HTOM(tlv_0x11->mask_32, input->cell_db_data_mask);
        buf_position += tlv_sz;
    }

    /* Add optional TLV 0x12 */
    if (input->common_data_mask_available) {
        QMI_LOC_generic_mask_32_t *tlv_0x12;

        tlv_sz = sizeof(*tlv_0x12);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x12;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x12 = (QMI_LOC_generic_mask_32_t *)buf_position;
        COPY_HTOM(tlv_0x12->mask_32, input->common_data_mask);
        buf_position += tlv_sz;
    }

    /* Add optional TLV 0x13 */
    if (input->satellite_sys_data_available) {
        QMI_LOC_DELETE_GNSS_SERVICE_DATA_0x13_t *tlv_0x13;

        tlv_sz = sizeof(*tlv_0x13);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x13;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x13 = (QMI_LOC_DELETE_GNSS_SERVICE_DATA_0x13_t *)buf_position;
        COPY_HTOM(tlv_0x13->system_mask, input->satellite_sys_data.system_mask);
        COPY_HTOM(tlv_0x13->delete_satellite_data_mask, input->satellite_sys_data.delete_satellite_data_mask);
        buf_position += tlv_sz;
    }

    /* Add optional TLV 0x14 */
    if (input->ext_clock_info_mask_available) {
        QMI_LOC_generic_mask_64_t *tlv_0x14;

        tlv_sz = sizeof(*tlv_0x14);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x14;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x14 = (QMI_LOC_generic_mask_64_t *)buf_position;
        COPY_HTOM(tlv_0x14->mask_64, input->ext_clock_info_mask);
    }

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_loc_delete_gnss_service_data_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}


/*****************************************************************************
 * QMI_LOC_REGISTER_MASTER_CLIENT (186 - 0x00BA)
 */

QMI_SDK_error_t telit_loc_reg_master_client_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        loc_master_client_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint16_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    QMI_LOC_REGISTER_MASTER_CLIENT_generic_0x01_t *tlv_0x01;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !len || !input) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_LOC_REGISTER_MASTER_CLIENT;
    req_ctx->svc_id = QMI_SVC_LOC;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    /* Add mandatory TLV 0x01 */
    hdr_sz = sizeof(*hdr);
    tlv_sz = sizeof(*tlv_0x01);
    CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

    hdr = (QMI_raw_content_header_t *)buf_position;
    hdr->type_id = 0x01;
    hdr->length = htole16(tlv_sz);

    buf_position += hdr_sz;

    tlv_0x01 = (QMI_LOC_REGISTER_MASTER_CLIENT_generic_0x01_t *)buf_position;
    COPY_HTOM(tlv_0x01->value, input->key);

    add_header(req_ctx, payload_len, original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_loc_reg_master_client_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

/*****************************************************************************
 * QMI LOC indications
 */

static QMI_SDK_error_t telit_loc_status_ind_unpack(uint8_t *rsp, uint16_t len,
        loc_status_ind_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_LOC_generic_status_indication_0x01_t *tlv_0x01;

    out->status = 0xFF;

    FIND_MANDATORY_TLV(rsp, 0x01, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_status_indication_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_LOC_generic_status_indication_0x01_t *)hdr;

    COPY_MTOH(out->status, tlv_0x01->status);

end:
    return ret;
}

static QMI_SDK_error_t telit_loc_position_report_ind_unpack(uint8_t *rsp, uint16_t len,
        loc_position_report_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_LOC_generic_status_indication_0x01_t *tlv_0x01;
    QMI_LOC_generic_session_id_t *tlv_0x02;

    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(rsp, 0x01, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_status_indication_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_LOC_generic_status_indication_0x01_t *)hdr;
    COPY_MTOH(out->session_status, tlv_0x01->status);

    FIND_MANDATORY_TLV(rsp, 0x02, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_session_id_t);

    hdr++;
    tlv_0x02 = (QMI_LOC_generic_session_id_t *)hdr;
    COPY_MTOH(out->session_id, tlv_0x02->session_id);

    hdr = search_TLV(rsp, 0x10, len);
    if (hdr) {
        QMI_LOC_generic_double_value_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_double_value_t);

        hdr++;
        tlv_0x10 = (QMI_LOC_generic_double_value_t *)hdr;

        out->latitude = tlv_0x10->value;
        out->latitude_available = true;
    }

    hdr = search_TLV(rsp, 0x11, len);
    if (hdr) {
        QMI_LOC_generic_double_value_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_double_value_t);

        hdr++;
        tlv_0x11 = (QMI_LOC_generic_double_value_t *)hdr;

        out->longitude = tlv_0x11->value;
        out->longitude_available = true;
    }

    hdr = search_TLV(rsp, 0x12, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x12 = (QMI_LOC_generic_float_value_t *)hdr;

        out->hor_unc_circular = tlv_0x12->value;
        out->hor_unc_circular_available = true;
    }

    hdr = search_TLV(rsp, 0x13, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x13;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x13 = (QMI_LOC_generic_float_value_t *)hdr;

        out->hor_unc_ellipse_semi_minor = tlv_0x13->value;
        out->hor_unc_ellipse_semi_minor_available = true;
    }

    hdr = search_TLV(rsp, 0x14, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x14;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x14 = (QMI_LOC_generic_float_value_t *)hdr;

        out->hor_unc_ellipse_semi_major = tlv_0x14->value;
        out->hor_unc_ellipse_semi_major_available = true;
    }

    hdr = search_TLV(rsp, 0x15, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x15;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x15 = (QMI_LOC_generic_float_value_t *)hdr;

        out->hor_unc_ellipse_orient_azimuth = tlv_0x15->value;
        out->hor_unc_ellipse_orient_azimuth_available = true;
    }

    hdr = search_TLV(rsp, 0x16, len);
    if (hdr) {
        QMI_LOC_generic_uint8_value_t *tlv_0x16;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_uint8_value_t);

        hdr++;
        tlv_0x16 = (QMI_LOC_generic_uint8_value_t *)hdr;

        COPY_MTOH(out->hor_confidence, tlv_0x16->value);
        out->hor_confidence_available = true;
    }

    hdr = search_TLV(rsp, 0x17, len);
    if (hdr) {
        QMI_LOC_generic_uint32_value_t *tlv_0x17;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_uint32_value_t);

        hdr++;
        tlv_0x17 = (QMI_LOC_generic_uint32_value_t *)hdr;

        COPY_MTOH(out->hor_reliability, tlv_0x17->value);
        out->hor_reliability_available = true;
    }

    hdr = search_TLV(rsp, 0x18, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x18;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x18 = (QMI_LOC_generic_float_value_t *)hdr;

        out->speed_horizontal = tlv_0x18->value;
        out->speed_horizontal_available = true;
    }

    hdr = search_TLV(rsp, 0x19, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x19;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x19 = (QMI_LOC_generic_float_value_t *)hdr;

        out->speed_unc = tlv_0x19->value;
        out->speed_unc_available = true;
    }

    hdr = search_TLV(rsp, 0x1A, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x1A;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x1A = (QMI_LOC_generic_float_value_t *)hdr;

        out->altitude_wrt_ellipsoid = tlv_0x1A->value;
        out->altitude_wrt_ellipsoid_available = true;
    }

    hdr = search_TLV(rsp, 0x1B, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x1B;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x1B = (QMI_LOC_generic_float_value_t *)hdr;

        out->altitude_wrt_mean_sea_level = tlv_0x1B->value;
        out->altitude_wrt_mean_sea_level_available = true;
    }

    hdr = search_TLV(rsp, 0x1C, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x1C;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x1C = (QMI_LOC_generic_float_value_t *)hdr;

        out->vert_unc = tlv_0x1C->value;
        out->vert_unc_available = true;
    }

    hdr = search_TLV(rsp, 0x1D, len);
    if (hdr) {
        QMI_LOC_generic_uint8_value_t *tlv_0x1D;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_uint8_value_t);

        hdr++;
        tlv_0x1D = (QMI_LOC_generic_uint8_value_t *)hdr;

        COPY_MTOH(out->vert_confidence, tlv_0x1D->value);
        out->vert_confidence_available = true;
    }

    hdr = search_TLV(rsp, 0x1E, len);
    if (hdr) {
        QMI_LOC_generic_uint32_value_t *tlv_0x1E;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_uint32_value_t);

        hdr++;
        tlv_0x1E = (QMI_LOC_generic_uint32_value_t *)hdr;

        COPY_MTOH(out->vert_reliability, tlv_0x1E->value);
        out->vert_reliability_available = true;
    }

    hdr = search_TLV(rsp, 0x1F, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x1F;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x1F = (QMI_LOC_generic_float_value_t *)hdr;

        out->speed_vertical = tlv_0x1F->value;
        out->speed_vertical_available = true;
    }

    hdr = search_TLV(rsp, 0x20, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x20;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x20 = (QMI_LOC_generic_float_value_t *)hdr;

        out->heading = tlv_0x20->value;
        out->heading_available = true;
    }

    hdr = search_TLV(rsp, 0x21, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x21;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x21 = (QMI_LOC_generic_float_value_t *)hdr;

        out->heading_unc = tlv_0x21->value;
        out->heading_unc_available = true;
    }

    hdr = search_TLV(rsp, 0x22, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x22;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x22 = (QMI_LOC_generic_float_value_t *)hdr;

        out->magnetic_deviation = tlv_0x22->value;
        out->magnetic_deviation_available = true;
    }

    hdr = search_TLV(rsp, 0x23, len);
    if (hdr) {
        QMI_LOC_generic_uint32_value_t *tlv_0x23;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_uint32_value_t);

        hdr++;
        tlv_0x23 = (QMI_LOC_generic_uint32_value_t *)hdr;

        COPY_MTOH(out->technology_mask, tlv_0x23->value);
        out->technology_mask_available = true;
    }

    hdr = search_TLV(rsp, 0x24, len);
    if (hdr) {
        QMI_LOC_EVENT_POSITION_REPORT_indication_0x24_t *tlv_0x24;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_EVENT_POSITION_REPORT_indication_0x24_t);

        hdr++;
        tlv_0x24 = (QMI_LOC_EVENT_POSITION_REPORT_indication_0x24_t *)hdr;

        out->precision_diluition.PDOP = tlv_0x24->PDOP;
        out->precision_diluition.HDOP = tlv_0x24->HDOP;
        out->precision_diluition.VDOP = tlv_0x24->VDOP;
        out->precision_diluition_available = true;
    }

    hdr = search_TLV(rsp, 0x25, len);
    if (hdr) {
        QMI_LOC_generic_uint64_value_t *tlv_0x25;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_uint64_value_t);

        hdr++;
        tlv_0x25 = (QMI_LOC_generic_uint64_value_t *)hdr;

        COPY_MTOH(out->utc_timestamp, tlv_0x25->value);
        out->utc_timestamp_available = true;
    }

    hdr = search_TLV(rsp, 0x26, len);
    if (hdr) {
        QMI_LOC_generic_uint8_value_t *tlv_0x26;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_uint8_value_t);

        hdr++;
        tlv_0x26 = (QMI_LOC_generic_uint8_value_t *)hdr;

        COPY_MTOH(out->leap_seconds, tlv_0x26->value);
        out->leap_seconds_available = true;
    }

    hdr = search_TLV(rsp, 0x27, len);
    if (hdr) {
        QMI_LOC_EVENT_POSITION_REPORT_indication_0x27_t *tlv_0x27;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_EVENT_POSITION_REPORT_indication_0x27_t);

        hdr++;
        tlv_0x27 = (QMI_LOC_EVENT_POSITION_REPORT_indication_0x27_t *)hdr;

        COPY_MTOH(out->gps_time.gps_week, tlv_0x27->gps_week);
        COPY_MTOH(out->gps_time.gps_time_of_week, tlv_0x27->gps_time_of_week);
        out->gps_time_available = true;
    }

    hdr = search_TLV(rsp, 0x28, len);
    if (hdr) {
        QMI_LOC_generic_float_value_t *tlv_0x28;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

        hdr++;
        tlv_0x28 = (QMI_LOC_generic_float_value_t *)hdr;

        out->time_unc = tlv_0x28->value;
        out->time_unc_available = true;
    }

    hdr = search_TLV(rsp, 0x29, len);
    if (hdr) {
        QMI_LOC_generic_uint32_value_t *tlv_0x29;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_uint32_value_t);

        hdr++;
        tlv_0x29 = (QMI_LOC_generic_uint32_value_t *)hdr;

        COPY_MTOH(out->time_source, tlv_0x29->value);
        out->time_source_available = true;
    }

    hdr = search_TLV(rsp, 0x2A, len);
    if (hdr) {
        QMI_LOC_EVENT_POSITION_REPORT_indication_0x2A_t *tlv_0x2A;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_EVENT_POSITION_REPORT_indication_0x2A_t);

        hdr++;
        tlv_0x2A = (QMI_LOC_EVENT_POSITION_REPORT_indication_0x2A_t *)hdr;

        COPY_MTOH(out->sensor_data_usage.sensor, tlv_0x2A->sensor);
        COPY_MTOH(out->sensor_data_usage.aiding_indicator, tlv_0x2A->aiding_indicator);
        out->sensor_data_usage_available = true;
    }

    hdr = search_TLV(rsp, 0x2B, len);
    if (hdr) {
        QMI_LOC_generic_uint32_value_t *tlv_0x2B;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_uint32_value_t);

        hdr++;
        tlv_0x2B = (QMI_LOC_generic_uint32_value_t *)hdr;

        COPY_MTOH(out->fix_id, tlv_0x2B->value);
        out->fix_id_available = true;
    }

    hdr = search_TLV(rsp, 0x2C, len);
    if (hdr) {
        QMI_LOC_EVENT_POSITION_REPORT_indication_0x2C_t *tlv_0x2C;
        uint16_t *gnss_sv = NULL;
        uint8_t *skip = NULL;
        uint8_t i;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_EVENT_POSITION_REPORT_indication_0x2C_t);

        hdr++;
        tlv_0x2C = (QMI_LOC_EVENT_POSITION_REPORT_indication_0x2C_t *)hdr;
        COPY_MTOH(out->gnss_sv_used_size, tlv_0x2C->gnss_sv_used_size);

        if (out->gnss_sv_used_size  > QMI_LOC_MAX_GNSS_SV_USED) {
            RLOGE("%s: gnss_sv_used_size = %u > QMI_LOC_MAX_GNSS_SV_USED, increase the define",
                    __FUNCTION__, out->gnss_sv_used_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x2C->gnss_sv_used_size));
        skip += sizeof(tlv_0x2C->gnss_sv_used_size);
        gnss_sv = (uint16_t *)skip;

        if (out->gnss_sv_used_size > 0) {
            for (i = 0; i < out->gnss_sv_used_size; i++) {
                COPY_MTOH(out->gnss_sv_used[i], gnss_sv[i]);
            }
        } else {
            RLOGW("%s: no instances available", __FUNCTION__);
        }
    }

    hdr = search_TLV(rsp, 0x2D, len);
    if (hdr) {
        QMI_LOC_generic_uint8_value_t *tlv_0x2D;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_uint8_value_t);

        hdr++;
        tlv_0x2D = (QMI_LOC_generic_uint8_value_t *)hdr;

        COPY_MTOH(out->altitude_assumed, tlv_0x2D->value);
        out->altitude_assumed_available = true;
    }

end:
    return ret;
}

static QMI_SDK_error_t telit_loc_gnss_sv_ind_unpack(uint8_t *rsp, uint16_t len, loc_gnss_sv_info_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_LOC_generic_uint8_value_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(rsp, 0x01, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_uint8_value_t);

    hdr++;
    tlv_0x01 = (QMI_LOC_generic_uint8_value_t *)hdr;
    COPY_MTOH(out->altitude_assumed, tlv_0x01->value);

    hdr = search_TLV(rsp, 0x10, len);
    if (hdr) {
        QMI_LOC_EVENT_GNSS_SV_INFO_indication_0x10_t *tlv_0x10;
        sv_instance_t *sv_info = NULL;
        uint8_t *skip = NULL;
        uint8_t i;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_EVENT_GNSS_SV_INFO_indication_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_LOC_EVENT_GNSS_SV_INFO_indication_0x10_t *)hdr;
        COPY_MTOH(out->sv_info_size, tlv_0x10->sv_info_size);

        if (out->sv_info_size  > QMI_LOC_MAX_SV) {
            RLOGE("%s: sv_info_size = %u > QMI_LOC_MAX_SV, increase the define",
                    __FUNCTION__, out->sv_info_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x10->sv_info_size));
        skip += sizeof(tlv_0x10->sv_info_size);
        sv_info = (sv_instance_t *)skip;

        if (out->sv_info_size > 0) {
            for (i = 0; i < out->sv_info_size; i++) {
                uint32_t validity_mask;

                COPY_MTOH(validity_mask, sv_info[i].validity_mask);

                if (validity_mask & 0x00000001) {
                    COPY_MTOH(out->sv_info[i].system, sv_info[i].system);
                    out->sv_info[i].system_available = true;
                }
                if (validity_mask & 0x00000002) {
                    COPY_MTOH(out->sv_info[i].gnss_sv_id, sv_info[i].gnss_sv_id);
                    out->sv_info[i].gnss_sv_id_available = true;
                }
                if (validity_mask & 0x00000004) {
                    COPY_MTOH(out->sv_info[i].health_status, sv_info[i].health_status);
                    out->sv_info[i].health_status_available = true;
                }
                if (validity_mask & 0x00000008) {
                    COPY_MTOH(out->sv_info[i].sv_status, sv_info[i].sv_status);
                    out->sv_info[i].sv_status_available = true;
                }
                if (validity_mask & 0x00000010) {
                    COPY_MTOH(out->sv_info[i].sv_info_mask, sv_info[i].sv_info_mask);
                    out->sv_info[i].sv_info_mask_available = true;
                }
                if (validity_mask & 0x00000020) {
                    out->sv_info[i].elevation = sv_info[i].elevation;
                    out->sv_info[i].elevation_available = true;
                }
                if (validity_mask & 0x00000040) {
                    out->sv_info[i].azimuth = sv_info[i].azimuth;
                    out->sv_info[i].azimuth_available = true;
                }
                if (validity_mask & 0x00000080) {
                    out->sv_info[i].snr = sv_info[i].snr;
                    out->sv_info[i].snr_available = true;
                }
            }
        } else {
            RLOGW("%s: no instances available", __FUNCTION__);
        }
    }

    hdr = search_TLV(rsp, 0x11, len);
    if (hdr) {
        QMI_LOC_EVENT_GNSS_SV_INFO_indication_0x11_t *tlv_0x11;
        sv_ext_instance_t *sv_ext_info = NULL;
        uint8_t *skip = NULL;
        uint8_t i;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_EVENT_GNSS_SV_INFO_indication_0x11_t);

        hdr++;
        tlv_0x11 = (QMI_LOC_EVENT_GNSS_SV_INFO_indication_0x11_t *)hdr;
        COPY_MTOH(out->sv_ext_info_size, tlv_0x11->sv_ext_info_size);

        if (out->sv_ext_info_size  > QMI_LOC_MAX_SV) {
            RLOGE("%s: sv_ext_info_size = %u > QMI_LOC_MAX_SV, increase the define",
                    __FUNCTION__, out->sv_ext_info_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }

        skip = (uint8_t *)(&(tlv_0x11->sv_ext_info_size));
        skip += sizeof(tlv_0x11->sv_ext_info_size);
        sv_ext_info = (sv_ext_instance_t *)skip;

        if (out->sv_ext_info_size > 0) {
            for (i = 0; i < out->sv_ext_info_size; i++) {
                uint32_t validity_mask;

                COPY_MTOH(validity_mask, sv_ext_info[i].validity_mask);

                if (validity_mask & 0x00000001) {
                    COPY_MTOH(out->sv_ext_info[i].system, sv_ext_info[i].system);
                    out->sv_ext_info[i].system_available = true;
                }
                if (validity_mask & 0x00000002) {
                    COPY_MTOH(out->sv_ext_info[i].gnss_sv_id, sv_ext_info[i].gnss_sv_id);
                    out->sv_ext_info[i].gnss_sv_id_available = true;
                }
                if (validity_mask & 0x00000004) {
                    COPY_MTOH(out->sv_ext_info[i].health_status, sv_ext_info[i].health_status);
                    out->sv_ext_info[i].health_status_available = true;
                }
                if (validity_mask & 0x00000008) {
                    COPY_MTOH(out->sv_ext_info[i].sv_status, sv_ext_info[i].sv_status);
                    out->sv_ext_info[i].sv_status_available = true;
                }
                if (validity_mask & 0x00000010) {
                    COPY_MTOH(out->sv_ext_info[i].sv_info_mask, sv_ext_info[i].sv_info_mask);
                    out->sv_ext_info[i].sv_info_mask_available = true;
                }
                if (validity_mask & 0x00000020) {
                    out->sv_ext_info[i].elevation = sv_ext_info[i].elevation;
                    out->sv_ext_info[i].elevation_available = true;
                }
                if (validity_mask & 0x00000040) {
                    out->sv_ext_info[i].azimuth = sv_ext_info[i].azimuth;
                    out->sv_ext_info[i].azimuth_available = true;
                }
                if (validity_mask & 0x00000080) {
                    out->sv_ext_info[i].snr = sv_ext_info[i].snr;
                    out->sv_ext_info[i].snr_available = true;
                }
                COPY_MTOH(out->sv_ext_info[i].glo_frequency, sv_ext_info[i].glo_frequency);
                out->sv_ext_info[i].glo_frequency_available = true;
            }
        } else {
            RLOGW("%s: no instances available", __FUNCTION__);
        }
    }

end:
    return ret;
}

static QMI_SDK_error_t telit_loc_ttff_ind_unpack(uint8_t *rsp, uint16_t len,
        loc_ttff_ind_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_LOC_GET_TTFF_FOXCONN_IND_indication_0x10_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(rsp, 0x01, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_LOC_GET_TTFF_FOXCONN_IND_indication_0x10_t);

    hdr++;
    tlv_0x01 = (QMI_LOC_GET_TTFF_FOXCONN_IND_indication_0x10_t *)hdr;
    COPY_MTOH(out->ttff, tlv_0x01->ttff);

end:
    return ret;
}

static QMI_SDK_error_t telit_loc_nmea_ind_unpack(uint8_t *rsp, uint16_t len,
        loc_nmea_ind_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;

    memset(out->nmea_string, '\0', QMI_LOC_NMEA_STRING_MAX_LEN);

    ret = get_string(rsp, len, 0x01, QMI_LOC_NMEA_STRING_MAX_LEN, out->nmea_string);

    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: unable to collect NMEA string", __FUNCTION__);
    }
    out->nmea_string_len = strlen(out->nmea_string);

    return ret;
}

static QMI_SDK_error_t telit_loc_inject_time_req_ind_unpack(uint8_t *rsp, uint16_t len,
        loc_inject_time_req_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    uint8_t *buf_position;

    memset(out, '\0', sizeof(*out));

    hdr = search_TLV(rsp, 0x10, len);
    if (hdr) {
        QMI_LOC_EVENT_INJECT_TIME_REQ_indication_0x10_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_EVENT_INJECT_TIME_REQ_indication_0x10_t);

        hdr++;
        tlv_0x10 = (QMI_LOC_EVENT_INJECT_TIME_REQ_indication_0x10_t *)hdr;
        COPY_MTOH(out->time_server_info.delay_threshold, tlv_0x10->delay_threshold);
        COPY_MTOH(out->time_server_info.server_urls_size, tlv_0x10->server_urls_size);
        if (out->time_server_info.server_urls_size > QMI_LOC_MAX_SERVER_URL) {
            RLOGE("%s: server_urls_size = %u > QMI_LOC_MAX_SERVER_URL, increase the define",
                    __FUNCTION__, out->time_server_info.server_urls_size);
            ret = QMI_SDK_ERR_INTERNAL;
            goto end;
        }
        buf_position = &tlv_0x10->server_urls_size;
        buf_position += sizeof(tlv_0x10->server_urls_size);

        for (int i = 0; i < out->time_server_info.server_urls_size; i++) {
            COPY_MTOH(out->time_server_info.server_urls[i].server_url_len,
                    buf_position);
            buf_position += sizeof(out->time_server_info.server_urls[i].server_url_len);
            memcpy(out->time_server_info.server_urls[i].server_url, buf_position,
                    out->time_server_info.server_urls[i].server_url_len);
            buf_position += out->time_server_info.server_urls[i].server_url_len;
        }

        out->time_server_info_available = true;
    }

end:
    return ret;
}

static QMI_SDK_error_t telit_loc_inject_position_req_ind_unpack(uint8_t *rsp, uint16_t len,
        loc_inject_position_req_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_LOC_generic_double_value_t *tlv_0x01;
    QMI_LOC_generic_double_value_t *tlv_0x02;
    QMI_LOC_generic_float_value_t *tlv_0x03;
    QMI_LOC_generic_uint64_value_t *tlv_0x04;

    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(rsp, 0x01, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_double_value_t);

    hdr++;
    tlv_0x01 = (QMI_LOC_generic_double_value_t *)hdr;
    out->latitude = tlv_0x01->value;

    FIND_MANDATORY_TLV(rsp, 0x02, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_double_value_t);

    hdr++;
    tlv_0x02 = (QMI_LOC_generic_double_value_t *)hdr;
    out->longitude = tlv_0x02->value;

    FIND_MANDATORY_TLV(rsp, 0x03, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_float_value_t);

    hdr++;
    tlv_0x03 = (QMI_LOC_generic_float_value_t *)hdr;
    out->hor_unc_circular = tlv_0x03->value;

    FIND_MANDATORY_TLV(rsp, 0x04, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_uint64_value_t);

    hdr++;
    tlv_0x04 = (QMI_LOC_generic_uint64_value_t *)hdr;
    out->timestamp_utc = tlv_0x04->value;

end:
    return ret;
}

static QMI_SDK_error_t telit_loc_engine_lock_ind_unpack(uint8_t *rsp, uint16_t len, loc_lock_status_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_LOC_generic_lock_status_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(rsp, 0x01, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_lock_status_t);

    hdr++;
    tlv_0x01 = (QMI_LOC_generic_lock_status_t *)hdr;
    COPY_MTOH(out->status, tlv_0x01->lock_status);

    hdr = search_TLV(rsp, 0x10, len);
    if (hdr) {
        QMI_LOC_generic_lock_type_t *tlv_0x10;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_lock_type_t);

        hdr++;
        tlv_0x10 = (QMI_LOC_generic_lock_type_t *)hdr;
        COPY_MTOH(out->lock_type, tlv_0x10->lock_type);

        out->lock_type_available = true;
    }

    hdr = search_TLV(rsp, 0x11, len);
    if (hdr) {
        QMI_LOC_generic_subscription_type_t *tlv_0x11;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_subscription_type_t);

        hdr++;
        tlv_0x11 = (QMI_LOC_generic_subscription_type_t *)hdr;
        COPY_MTOH(out->subscription_type, tlv_0x11->sub_type);

        out->subscription_type_available = true;
    }

    hdr = search_TLV(rsp, 0x12, len);
    if (hdr) {
        QMI_LOC_generic_lock_client_t *tlv_0x12;

        CHECK_UNPACK_TLV_SIZE(QMI_LOC_generic_lock_client_t);

        hdr++;
        tlv_0x12 = (QMI_LOC_generic_lock_client_t *)hdr;
        COPY_MTOH(out->lock_client, tlv_0x12->lock_client);

        out->lock_client_available = true;
    }

end:
    return ret;
}

static QMI_SDK_error_t telit_loc_reg_master_client_ind_unpack(uint8_t *rsp, uint16_t len,
        loc_master_client_status_t *out)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_NONE;
    QMI_raw_content_header_t *hdr = NULL;
    QMI_LOC_REGISTER_MASTER_CLIENT_generic_0x01_t *tlv_0x01;

    memset(out, 0, sizeof(*out));

    FIND_MANDATORY_TLV(rsp, 0x01, len, hdr);

    CHECK_UNPACK_TLV_SIZE(QMI_LOC_REGISTER_MASTER_CLIENT_generic_0x01_t);

    hdr++;
    tlv_0x01 = (QMI_LOC_REGISTER_MASTER_CLIENT_generic_0x01_t *)hdr;
    COPY_MTOH(out->status, tlv_0x01->value);

end:
    return ret;
}

QMI_SDK_error_t telit_loc_indication_handler(uint16_t msg_id,
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
        case QMI_LOC_EVENT_POSITION_REPORT_IND:
            if (out_len < sizeof(loc_position_report_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(loc_position_report_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_loc_position_report_ind_unpack(msg, payload_len,
                    (loc_position_report_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_LOC_EVENT_GNSS_SV_INFO_IND:
            if (out_len < sizeof(loc_gnss_sv_info_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(loc_gnss_sv_info_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_loc_gnss_sv_ind_unpack(msg, payload_len,
                    (loc_gnss_sv_info_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_LOC_EVENT_NMEA_IND:
            if (out_len < sizeof(loc_nmea_ind_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(loc_nmea_ind_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_loc_nmea_ind_unpack(msg, payload_len,
                    (loc_nmea_ind_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_LOC_EVENT_INJECT_TIME_REQ_IND:
            if (out_len < sizeof(loc_inject_time_req_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(loc_inject_time_req_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_loc_inject_time_req_ind_unpack(msg, payload_len,
                    (loc_inject_time_req_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_LOC_EVENT_INJECT_POSITION_REQ_IND:
            if (out_len < sizeof(loc_inject_position_req_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(loc_inject_position_req_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_loc_inject_position_req_ind_unpack(msg, payload_len,
                    (loc_inject_position_req_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_LOC_INJECT_UTC_TIME_IND:
        case QMI_LOC_INJECT_POSITION_IND:
        case QMI_LOC_DELETE_ASSIST_DATA_IND:
        case QMI_LOC_SET_NMEA_TYPES_IND:
        case QMI_LOC_SET_OPERATION_MODE_IND:
        case QMI_LOC_GET_OPERATION_MODE_IND:
        case QMI_LOC_DELETE_GNSS_SERVICE_DATA:
            if (out_len < sizeof(loc_status_ind_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(loc_status_ind_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_loc_status_ind_unpack(msg, payload_len,
                    (loc_status_ind_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_LOC_GET_TTFF_FOXCONN:
            if (out_len < sizeof(loc_ttff_ind_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(loc_ttff_ind_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_loc_ttff_ind_unpack(msg, payload_len,
                    (loc_ttff_ind_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_LOC_SET_ENGINE_LOCK_IND:
        case QMI_LOC_GET_ENGINE_LOCK_IND:
            if (out_len < sizeof(loc_lock_status_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(loc_lock_status_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_loc_engine_lock_ind_unpack(msg, payload_len,
                    (loc_lock_status_t *)out);
            if (ret != QMI_SDK_ERR_NONE) {
                goto end;
            }
            break;
        case QMI_LOC_REGISTER_MASTER_CLIENT_IND:
            if (out_len < sizeof(loc_master_client_status_t)) {
                RLOGE("%s: SIZE out_len %u, struct %zu\n",
                        __FUNCTION__, out_len, sizeof(loc_master_client_status_t));
                ret = QMI_SDK_ERR_BUFFER_SZ;
                goto end;
            }

            ret = telit_loc_reg_master_client_ind_unpack(msg, payload_len,
                    (loc_master_client_status_t *)out);
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
