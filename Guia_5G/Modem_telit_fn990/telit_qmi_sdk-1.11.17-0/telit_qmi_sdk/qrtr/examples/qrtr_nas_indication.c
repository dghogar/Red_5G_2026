#include <errno.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>

#include "telit_sdk.h"
#include "telit_sdk_qrtr.h"
#include "telit_sdk_nas.h"

#define SVC_STR "NAS"
#define SVC QMI_SVC_NAS
#define XID 1

static void print_serving_system(nas_serving_system_info_t *out)
{
    uint8_t i;

    RLOGI("reg_state %u", out->serving_system.reg_state);
    RLOGI("ps_attach_state %u", out->serving_system.ps_attach_state);
    RLOGI("cs_attach_state %u", out->serving_system.cs_attach_state);
    RLOGI("sel_network %u", out->serving_system.sel_network);
    RLOGI("num_radio_interfaces %u", out->serving_system.radio_interface_size);
    for (i = 0; i < out->serving_system.radio_interface_size; i++)
        RLOGI("radio_interface[%d] = %u", i, out->serving_system.radio_interface[i]);

    if (out->current_PLMN_available) {
        RLOGI("mcc %u", out->current_PLMN.mcc);
        RLOGI("mnc %u", out->current_PLMN.mnc);
        RLOGI("net_descr_len %u", out->current_PLMN.net_descr_len);
        RLOGI("network_name %s", out->current_PLMN.network_name);
    }

    if (out->roam_indicator_value_available) {
        RLOGI("roam_indicator_val %u", out->roam_indicator_value);
    }

    for (i = 0; i < out->data_caps_size; i++) {
        RLOGI("data_caps[%d] = %u", i, out->data_caps[i]);
    }

    if (out->cdma_system_available) {
        RLOGI("cdma_system_id %u", out->cdma_system.cdma_system_id);
        RLOGI("cdma_network_id %u", out->cdma_system.cdma_network_id);
    }

    if (out->base_station_available) {
        RLOGI("cdma_base_station_id %u", out->base_station.cdma_base_station_id);
        RLOGI("cdma_base_station_latitude %d", out->base_station.cdma_base_station_latitude);
        RLOGI("cdma_base_station_longitude %d", out->base_station.cdma_base_station_longitude);
    }


    for (i = 0; i < out->roaming_indicators_size; i++) {
        RLOGI("radio_interface[%d] = %u", i, out->roaming_indicators[i].radio_interface);
        RLOGI("roam_indicator[%d] = %u", i, out->roaming_indicators[i].roam_indicator);
    }

    if (out->default_roam_ind_available) {
        RLOGI("default_roam_ind %d", out->default_roam_ind);
    }

    if (out->time_zone_3gpp2_available) {
        RLOGI("leap_seconds %d", out->time_zone_3gpp2.leap_seconds);
        RLOGI("daylight_saving %d", out->time_zone_3gpp2.daylight_saving);
        RLOGI("local_time_offset %d", out->time_zone_3gpp2.local_time_offset);
    }

    if (out->CDMA_p_rev_available) {
        RLOGI("CDMA_p_rev %d", out->CDMA_p_rev);
    }

    if (out->time_zone_3gpp_available) {
        RLOGI("_3gpp_time_zone %d", out->time_zone_3gpp);
    }

    if (out->network_DST_adjustment_3gpp_available) {
        RLOGI("_3gpp_network_DST_adjustment %d", out->network_DST_adjustment_3gpp);
    }

    if (out->location_area_code_available) {
        RLOGI("location_area_code %d", out->location_area_code);
    }

    if (out->cell_id_available) {
        RLOGI("cell_id %d", out->cell_id);
    }

    if (out->conc_svc_info_available) {
        RLOGI("conc_svc_info %d", out->conc_svc_info);
    }

    if (out->PRL_ind_available) {
        RLOGI("PRL_ind %d", out->PRL_ind);
    }

    if (out->DTM_ind_available) {
        RLOGI("DTM_ind %d", out->DTM_ind);
    }

    if (out->detailed_svc_info_available) {
        RLOGI("service_status %d", out->detailed_svc_info.service_status);
        RLOGI("service_caps %d", out->detailed_svc_info.service_caps);
        RLOGI("hdr_srv_status %d", out->detailed_svc_info.hdr_srv_status);
        RLOGI("hdr_hybrid %d", out->detailed_svc_info.hdr_hybrid);
        RLOGI("is_sys_forbidden %d", out->detailed_svc_info.is_sys_forbidden);
    }

    if (out->CDMA_system_info_ext_available) {
        RLOGI("MCC %d", out->CDMA_system_info_ext.MCC);
        RLOGI("imsi_11_12 %d", out->CDMA_system_info_ext.imsi_11_12);
    }

    if (out->hdr_personality_available) {
        RLOGI("hdr_personality %d", out->hdr_personality);
    }

    if (out->track_area_code_available) {
        RLOGI("track_area_code %d", out->track_area_code);
    }

    if (out->call_bar_status_available) {
        RLOGI("cs_bar_status %d", out->call_bar_status.cs_bar_status);
        RLOGI("ps_bar_status %d", out->call_bar_status.ps_bar_status);
    }
}


int main(int argc __unused, char **argv __unused)
{
    int rtn;
    int sock, node, port;
    uint8_t rsp[QMI_MSG_MAX] = {};
    uint8_t req[QMI_MSG_MAX] = {};
    uint16_t req_len = QMI_MSG_MAX;
    uint16_t rsp_len = QMI_MSG_MAX;
    struct pollfd event;
    pack_qmi_t req_ctx = {
        .xid = XID
    };
    nas_indication_register_t input = {
        .serving_system = QMI_IND_ACTION_SET,
        .sig_info = QMI_IND_ACTION_SET,
    };

    rtn = telit_qrtr_get_client_for_svc(SVC, &sock, &node, &port);
    if (rtn < 0) {
        return -1;
    }

    rtn = telit_nas_indication_register_pack(&req_ctx, req, &req_len, &input);
    if (rtn != QMI_SDK_ERR_NONE) {
        RLOGE("pack fail %d ", rtn);
        goto close;
    }

    RLOGI("sending request...");
    rtn = telit_qrtr_sendto(sock, node, port, req, req_len);
    if (rtn < 0) {
        goto close;
        return -1;
    }

    event.fd = sock;
    event.events = POLLIN;

    while (1) {
        unpack_qmi_t ctx = {};

        rtn = poll(&event, 1, 60 * 1000);
        if (rtn < 0) {
            if (rtn == EINTR) {
                continue;
            }
            RLOGE("poll error %d", rtn);
            break;
        }
        if (event.revents & POLLHUP) {
            RLOGI("poll hangup");
            break;
        }
        if (event.revents & POLLERR) {
            RLOGI("poll error");
            break;
        }

        if (!(event.revents & POLLIN)) {
            break;
        }

        memset(rsp, 0, rsp_len);
        rtn = telit_qrtr_recvfrom(sock, node, &port, rsp, &rsp_len);
        if (rtn < 0) {
            break;
        }

        telit_helper_get_resp_ctx(SVC, rsp, rsp_len, &ctx);
        RLOGD("context: type:%d, msg:%d, xid:%d, len:%d",
                ctx.type, ctx.msg_id, ctx.xid, rsp_len);

        switch (ctx.type) {
            case QMI_MSG_RESPONSE:
                RLOGD("%s response message, msgid 0x%x", SVC_STR, ctx.msg_id);
                if (ctx.xid == XID) {
                    rtn = telit_nas_indication_register_unpack(rsp, rsp_len);

                    if (rtn != QMI_SDK_ERR_NONE) {
                        RLOGE("%s unpack pack fail %d ", SVC_STR, rtn);
                    } else {
                        RLOGI("NAS set event report unpack ok");
                    }
                } else {
                    RLOGD("xid %d", XID);
                }
                break;

            case QMI_MSG_INDICATION:
                RLOGI("%s Indication message, msgid 0x%x", SVC_STR, ctx.msg_id);
                nas_serving_system_info_t out;

                rtn = telit_nas_indication_handler(ctx.msg_id,
                        rsp, rsp_len,
                        (uint8_t *)&out, sizeof(out));
                if (rtn != QMI_SDK_ERR_NONE) {
                    RLOGE("unpack fail %d ", rtn);
                }
                switch (ctx.msg_id) {
                    case QMI_NAS_EVENT_REPORT_IND:
                        RLOGI("got nas event report indication\n");
                        break;
                    case QMI_NAS_SERVING_SYSTEM_IND:
                        RLOGI("got nas serving system info indication\n");
                        print_serving_system(&out);
                        break;
                    case QMI_NAS_SIG_INFO_IND:
                        RLOGI("got nas sig info indication\n");
                        break;
                    default:
                        RLOGI("Indication not managed");
                        break;
                }
                break;
            default:
                RLOGD("Unknown QMI message type: %d", ctx.type);
                break;
        }
    }
close:
    close(sock);

    return 0;
}
