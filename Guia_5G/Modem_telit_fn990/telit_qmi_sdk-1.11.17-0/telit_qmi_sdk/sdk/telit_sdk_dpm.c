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
#include "telit_sdk_private_dpm.h"
#include "telit_sdk_dpm.h"

/*****************************************************************************
 * QMI_DPM_OPEN_PORT (32 - 0x0020)
 */

QMI_SDK_error_t telit_dpm_open_port_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        dpm_open_port_t *input)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;
    QMI_raw_content_header_t *hdr = NULL;
    uint32_t payload_len = 0;
    uint8_t *buf_position;
    uint8_t *original_buffer;
    uint32_t hdr_sz;
    uint16_t tlv_sz;

    if (!req_ctx || !req || !input || !len) {
        RLOGE("%s: invalid arg", __FUNCTION__);
        goto end;
    }

    req_ctx->msg_id = QMI_DPM_OPEN_PORT;
    req_ctx->svc_id = QMI_SVC_DPM;
    req_ctx->timeout = QMI_TIMEOUT_SMALL;

    original_buffer = req;
    buf_position = skip_msg_header(req);

    hdr_sz = sizeof(*hdr);

    if (input->hardware_data_port_list_size) {
        QMI_DPM_generic_port_details_t *tlv_0x11;

        tlv_sz = sizeof(*tlv_0x11);
        CHECK_PACK_TLV_SIZE(tlv_sz, hdr_sz, payload_len, *len);

        hdr = (QMI_raw_content_header_t *)buf_position;
        hdr->type_id = 0x11;
        hdr->length = htole16(tlv_sz);

        buf_position += hdr_sz;

        tlv_0x11 = (QMI_DPM_generic_port_details_t *)buf_position;
        COPY_HTOM(tlv_0x11->size, input->hardware_data_port_list_size);
        COPY_HTOM(tlv_0x11->ep_type, input->hardware_data_port_list[0].peripheral_endpoint.ep_type);
        COPY_HTOM(tlv_0x11->iface_id, input->hardware_data_port_list[0].peripheral_endpoint.iface_id);
        COPY_HTOM(tlv_0x11->consumer_pipe_num, input->hardware_data_port_list[0].pair_type.consumer_pipe_num);
        COPY_HTOM(tlv_0x11->producer_pipe_num, input->hardware_data_port_list[0].pair_type.producer_pipe_num);
    }

    add_header(req_ctx, payload_len, (uint8_t *)original_buffer, len);

end:
    return ret;
}

QMI_SDK_error_t telit_dpm_open_port_unpack(uint8_t *rsp, uint16_t len)
{
    return generic_unpack_request(rsp, len);
}

