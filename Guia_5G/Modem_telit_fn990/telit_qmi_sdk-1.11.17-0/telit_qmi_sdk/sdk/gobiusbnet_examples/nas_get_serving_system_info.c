/* */
/* demo program for illustrating GobiNet driver usage */
/* */
/* (a) prepare a message */
/* (b) send to modem. */
/* (c) Wait for response from modem */
/* (d) Process received response message */
/* (e) close QMI service. */
/* */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "telit_sdk.h"
#include "telit_sdk_nas.h"

#define QMI_GET_SERVICE_FILE_IOCTL 0x8BE0 + 1

/* change this define if QMI device name is different */
#define GOBINET_DEVICE "/dev/qcqmi0"

#define SERVICE QMI_SVC_NAS
#define SVC_STR "NAS"

static int telit_client_fd_from_qcqmi(uint8_t svc, const char *qcqmi)
{
    int fd = open(qcqmi, O_RDWR);

    if (fd == -1) {
        RLOGE("%s: error in opening device %d - %s",
                __FUNCTION__, errno, strerror(errno));
        goto end;
    }

    if (ioctl(fd, QMI_GET_SERVICE_FILE_IOCTL, svc) != 0) {
        RLOGE("%s: error in ioctl", __FUNCTION__);
        close(fd);
        fd = -1;
    }

end:
    return fd;
}

int main()
{
    int svc = -1;
    pack_qmi_t req_ctx;
    unpack_qmi_t rsp_ctx;
    uint16_t xid = 1;
    int rtn;
    uint8_t rsp[QMI_MSG_MAX];
    uint8_t req[QMI_MSG_MAX];
    uint16_t req_size = QMI_MSG_MAX;
    uint16_t rsp_size = QMI_MSG_MAX;
    bool got_expected_message = false;
    nas_serving_system_info_t out;

    memset(&out, 0, sizeof(out));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    telit_set_log_level(LOG_INFO);

    rtn = telit_nas_get_serving_system_pack(&req_ctx, req, &req_size);

    if (rtn != QMI_SDK_ERR_NONE) {
        RLOGE("pack fail %d ", rtn);
        return -1;
    }

    if ((svc = telit_client_fd_from_qcqmi(SERVICE, GOBINET_DEVICE)) < 0) {
        RLOGE("%s Service Not Supported!", SVC_STR);
        return -1;
    }

    rtn = write(svc, req, req_size);

    if (rtn != req_size) {
        RLOGE("%s socket write failed!", SVC_STR);
        goto close_fd;
    }

    while (!got_expected_message) {
        rtn = read(svc, rsp, QMI_MSG_MAX);

        if (rtn > 0) {
            rsp_size = rtn;
        }

        RLOGD("rtn = %d, rspLen = %u", rtn, rsp_size);

        if ((rtn > 0) && (rsp_size > 0)) {
            telit_helper_get_resp_ctx(SERVICE, rsp, rsp_size, &rsp_ctx);

            switch (rsp_ctx.type) {
                case QMI_MSG_RESPONSE:
                    RLOGD("%s response message, msgid 0x%x", SVC_STR, rsp_ctx.msg_id);

                    if (rsp_ctx.xid == xid) {
                        rtn = telit_nas_get_serving_system_unpack(rsp, rsp_size, &out);

                        if (rtn != QMI_SDK_ERR_NONE) {
                            RLOGE("%s unpack pack fail %d ", SVC_STR, rtn);
                        } else {
                            int i;

                            RLOGI("reg_state %u, ", out.serving_system.reg_state);
                            RLOGI("ps_attach_state %u, ", out.serving_system.ps_attach_state);
                            RLOGI("cs_attach_state %u, ", out.serving_system.cs_attach_state);
                            RLOGI("sel_network %u, ", out.serving_system.sel_network);
                            RLOGI("num_radio_interfaces %u, ", out.serving_system.radio_interface_size);
                            for (i = 0; i < out.serving_system.radio_interface_size; i++)
                                RLOGI("radio_interface[%d] = %u, ", i, out.serving_system.radio_interface[i]);

                            if (out.current_PLMN_available) {
                                RLOGI("mcc %u, ", out.current_PLMN.mcc);
                                RLOGI("mnc %u, ", out.current_PLMN.mnc);
                                RLOGI("net_descr_length %u, ", out.current_PLMN.net_descr_len);
                                RLOGI("network_name %s, ", out.current_PLMN.network_name);
                            }

                            if (out.roam_indicator_value_available) {
                                RLOGI("roam_indicator_val %u, ", out.roam_indicator_value);
                            }

                            for (i = 0; i < out.data_caps_size; i++) {
                                RLOGI("data_caps[%d] = %u, ", i, out.data_caps[i]);
                            }

                            if (out.cdma_system_available) {
                                RLOGI("cdma_system_id %u, ", out.cdma_system.cdma_system_id);
                                RLOGI("cdma_network_id %u, ", out.cdma_system.cdma_network_id);
                            }

                            if (out.base_station_available) {
                                RLOGI("cdma_base_station_id %u, ", out.base_station.cdma_base_station_id);
                                RLOGI("cdma_base_station_latitude %d, ",
                                        out.base_station.cdma_base_station_latitude);
                                RLOGI("cdma_base_station_longitude %d, ",
                                        out.base_station.cdma_base_station_longitude);
                            }


                            for (i = 0; i < out.roaming_indicators_size; i++) {
                                RLOGI("radio_interface[%d] = %u, ", i,
                                        out.roaming_indicators[i].radio_interface);
                                RLOGI("roam_indicator[%d] = %u, ", i,
                                        out.roaming_indicators[i].roam_indicator);
                            }

                            if (out.default_roam_ind_available) {
                                RLOGI("default_roam_ind %d, ", out.default_roam_ind);
                            }

                            if (out.time_zone_3gpp2_available) {
                                RLOGI("leap_seconds %d, ", out.time_zone_3gpp2.leap_seconds);
                                RLOGI("daylight_savings %d, ", out.time_zone_3gpp2.daylight_saving);
                                RLOGI("local_time_offset %d, ", out.time_zone_3gpp2.local_time_offset);
                            }

                            if (out.CDMA_p_rev_available) {
                                RLOGI("CDMA_p_rev %d, ", out.CDMA_p_rev);
                            }

                            if (out.time_zone_3gpp_available) {
                                RLOGI("_3gpp_time_zone %d, ", out.time_zone_3gpp);
                            }

                            if (out.network_DST_adjustment_3gpp_available) {
                                RLOGI("_3gpp_network_DST_adjustment %d, ", out.network_DST_adjustment_3gpp);
                            }

                            if (out.location_area_code_available) {
                                RLOGI("location_area_code %d, ", out.location_area_code);
                            }

                            if (out.cell_id_available) {
                                RLOGI("cell_id %d, ", out.cell_id);
                            }

                            if (out.conc_svc_info_available) {
                                RLOGI("conc_svc_info %d, ", out.conc_svc_info);
                            }

                            if (out.PRL_ind_available) {
                                RLOGI("PRL_ind %d, ", out.PRL_ind);
                            }

                            if (out.DTM_ind_available) {
                                RLOGI("DTM_ind %d, ", out.DTM_ind);
                            }


                            if (out.detailed_svc_info_available) {
                                RLOGI("service_status %d, ", out.detailed_svc_info.service_status);
                                RLOGI("service_caps %d, ", out.detailed_svc_info.service_caps);
                                RLOGI("hdr_srv_status %d, ", out.detailed_svc_info.hdr_srv_status);
                                RLOGI("hdr_hybrid %d, ", out.detailed_svc_info.hdr_hybrid);
                                RLOGI("is_sys_forbidden %d, ", out.detailed_svc_info.is_sys_forbidden);
                            }

                            if (out.CDMA_system_info_ext_available) {
                                RLOGI("MCC %d, ", out.CDMA_system_info_ext.MCC);
                                RLOGI("imsi_11_12 %d, ", out.CDMA_system_info_ext.imsi_11_12);
                            }

                            if (out.hdr_personality_available) {
                                RLOGI("hdr_personality %d, ", out.hdr_personality);
                            }

                            if (out.track_area_code_available) {
                                RLOGI("track_area_code %d, ", out.track_area_code);
                            }

                            if (out.call_bar_status_available) {
                                RLOGI("cs_bar_status %d, ", out.call_bar_status.cs_bar_status);
                                RLOGI("ps_bar_status %d, ", out.call_bar_status.ps_bar_status);
                            }
                        }

                        got_expected_message = true;
                    } else {
                        RLOGI("xid %d", xid);
                    }
                    break;

                case QMI_MSG_INDICATION:
                    RLOGD("%s Indication message, msgid 0x%x", SVC_STR, rsp_ctx.msg_id);
                    break;

                default:
                    RLOGD("Unknown QMI message type: %d", rsp_ctx.type);
                    break;
            }
        } else {
            sleep(1);
        }
    }

close_fd:

    if (svc >= 0) {
        close(svc);
    }

    return 0;
}
