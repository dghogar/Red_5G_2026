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
#include "telit_sdk_wds.h"

#define QMI_GET_SERVICE_FILE_IOCTL 0x8BE0 + 1

/* change this define if QMI device name is different */
#define GOBINET_DEVICE "/dev/qcqmi0"

/** ATTENTION: change index **/
#define INDEX 3

#define SERVICE QMI_SVC_WDS
#define SVC_STR "WDS"

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
    wds_settings_profile_id_t input;
    wds_profile_t out;

    memset(&input, 0, sizeof(input));
    memset(&out, 0, sizeof(out));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    input.type = QMI_WDS_PROFILE_TYPE_3GPP;
    input.index = INDEX;

    telit_set_log_level(LOG_INFO);

    rtn = telit_wds_get_profile_settings_pack(&req_ctx, req, &req_size, &input);

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
                        rtn = telit_wds_get_profile_settings_unpack(rsp, rsp_size, &out);

                        if (rtn != QMI_SDK_ERR_NONE) {
                            RLOGE("%s unpack fail %d ", SVC_STR, rtn);

                            if (out.extended_error_code_available) {
                                RLOGI("Extended error code 0x%x ",
                                        out.extended_error_code.extended_error_code);
                            }
                        } else {
                            RLOGI("%s unpack ok", SVC_STR);
                            if (out.profile_settings.profile_name_len) {
                                RLOGI("Profile name: %s", out.profile_settings.profile_name);
                            }
                            if (out.profile_settings.pdp_type_available) {
                                RLOGI("pdp_type %u", out.profile_settings.pdp_type);
                            }
                            if (out.profile_settings.pdp_hdr_comp_type_available) {
                                RLOGI("pdp_hdr_comp_type %u", out.profile_settings.pdp_hdr_comp_type);
                            }
                            if (out.profile_settings.pdp_data_comp_type_available) {
                                RLOGI("pdp_data_comp_type %u", out.profile_settings.pdp_data_comp_type);
                            }
                            if (out.profile_settings.apn_len) {
                                RLOGI("apn len: %u", out.profile_settings.apn_len);
                                RLOGI("apn: %s", out.profile_settings.apn);
                            }
                            if (out.profile_settings.pri_dns_ipv4_addr_pref_available) {
                                RLOGI("pri_dns_ipv4_addr_pref %u",
                                        out.profile_settings.pri_dns_ipv4_addr_pref);
                            }
                            if (out.profile_settings.sec_dns_ipv4_addr_pref_available) {
                                RLOGI("sec_dns_ipv4_addr_pref %u",
                                        out.profile_settings.sec_dns_ipv4_addr_pref);
                            }
                            if (out.profile_settings.umts_req_qos_available) {
                                RLOGI("traffic_class %u", out.profile_settings.umts_req_qos.traffic_class);
                                RLOGI("max_uplink_bitrate %u",
                                        out.profile_settings.umts_req_qos.max_uplink_bitrate);
                                RLOGI("max_downlink_bitrate %u",
                                        out.profile_settings.umts_req_qos.max_downlink_bitrate);
                                RLOGI("guaranteed_uplink_bitrate %u",
                                        out.profile_settings.umts_req_qos.guaranteed_uplink_bitrate);
                                RLOGI("guaranteed_downlink_bitrate %u",
                                        out.profile_settings.umts_req_qos.guaranteed_downlink_bitrate);
                                RLOGI("qos_delivery_order %u",
                                        out.profile_settings.umts_req_qos.qos_delivery_order);
                                RLOGI("max_sdu_size %u", out.profile_settings.umts_req_qos.max_sdu_size);
                                RLOGI("sdu_error_ratio %u",
                                        out.profile_settings.umts_req_qos.sdu_error_ratio);
                                RLOGI("residual_bit_error_ratio %u",
                                        out.profile_settings.umts_req_qos.residual_bit_error_ratio);
                                RLOGI("delivery_erroneous_SDUs %u",
                                        out.profile_settings.umts_req_qos.delivery_erroneous_SDUs);
                                RLOGI("transfer_delay %u", out.profile_settings.umts_req_qos.transfer_delay);
                                RLOGI("traffic_handling_priority %u",
                                        out.profile_settings.umts_req_qos.traffic_handling_priority);
                            }
                            if (out.profile_settings.umts_min_qos_available) {
                                RLOGI("traffic_class %u", out.profile_settings.umts_min_qos.traffic_class);
                                RLOGI("max_uplink_bitrate %u",
                                        out.profile_settings.umts_min_qos.max_uplink_bitrate);
                                RLOGI("max_downlink_bitrate %u",
                                        out.profile_settings.umts_min_qos.max_downlink_bitrate);
                                RLOGI("guaranteed_uplink_bitrate %u",
                                        out.profile_settings.umts_min_qos.guaranteed_uplink_bitrate);
                                RLOGI("guaranteed_downlink_bitrate %u",
                                        out.profile_settings.umts_min_qos.guaranteed_downlink_bitrate);
                                RLOGI("qos_delivery_order %u",
                                        out.profile_settings.umts_min_qos.qos_delivery_order);
                                RLOGI("max_sdu_size %u", out.profile_settings.umts_min_qos.max_sdu_size);
                                RLOGI("sdu_error_ratio %u",
                                        out.profile_settings.umts_min_qos.sdu_error_ratio);
                                RLOGI("residual_bit_error_ratio %u",
                                        out.profile_settings.umts_min_qos.residual_bit_error_ratio);
                                RLOGI("delivery_erroneous_SDUs %u",
                                        out.profile_settings.umts_min_qos.delivery_erroneous_SDUs);
                                RLOGI("transfer_delay %u", out.profile_settings.umts_min_qos.transfer_delay);
                                RLOGI("traffic_handling_priority %u",
                                        out.profile_settings.umts_min_qos.traffic_handling_priority);
                            }
                            if (out.profile_settings.gprs_req_qos_available) {
                                RLOGI("precedence_class %u",
                                        out.profile_settings.gprs_req_qos.precedence_class);
                                RLOGI("delay_class %u", out.profile_settings.gprs_req_qos.delay_class);
                                RLOGI("reliability_class %u",
                                        out.profile_settings.gprs_req_qos.reliability_class);
                                RLOGI("peak_throughput_class %u",
                                        out.profile_settings.gprs_req_qos.peak_throughput_class);
                                RLOGI("mean_throughput_class %u",
                                        out.profile_settings.gprs_req_qos.mean_throughput_class);
                            }
                            if (out.profile_settings.gprs_min_qos_available) {
                                RLOGI("precedence_class %u",
                                        out.profile_settings.gprs_min_qos.precedence_class);
                                RLOGI("delay_class %u", out.profile_settings.gprs_min_qos.delay_class);
                                RLOGI("reliability_class %u",
                                        out.profile_settings.gprs_min_qos.reliability_class);
                                RLOGI("peak_throughput_class %u",
                                        out.profile_settings.gprs_min_qos.peak_throughput_class);
                                RLOGI("mean_throughput_class %u",
                                        out.profile_settings.gprs_min_qos.mean_throughput_class);
                            }
                            if (out.profile_settings.username_len) {
                                RLOGI("username: %s", out.profile_settings.username);
                            }
                            if (out.profile_settings.password_len) {
                                RLOGI("password: %s", out.profile_settings.password);
                            }
                            if (out.profile_settings.authentication_pref_available) {
                                RLOGI("authentication_pref %u", out.profile_settings.authentication_pref);
                            }
                            if (out.profile_settings.ipv4_addr_pref_available) {
                                RLOGI("ipv4_addr_pref %u", out.profile_settings.ipv4_addr_pref);
                            }
                            RLOGI("pcscf_addr_using_pco %u", out.profile_settings.pcscf_addr_using_pco);
                            if (out.profile_settings.pdp_access_control_flag_available) {
                                RLOGI("pdp_access_control_flag %u",
                                        out.profile_settings.pdp_access_control_flag);
                            }
                            RLOGI("pcscf_addr_using_dhcp %u", out.profile_settings.pcscf_addr_using_dhcp);
                            RLOGI("im_cn_flag %u", out.profile_settings.im_cn_flag);
                            if (out.profile_settings.tft_id1_available) {
                                RLOGI("filter_id %u", out.profile_settings.tft_id1.filter_id);
                                RLOGI("eval_id %u", out.profile_settings.tft_id1.eval_id);
                                RLOGI("ip_version %u", out.profile_settings.tft_id1.ip_version);
                                RLOGI("ip address: ");
                                for (int i = 0; i < IPV6_ADDR_MAX_SIZE; i++)
                                    RLOGI("%u ", out.profile_settings.tft_id1.source_ip[i]);
                                RLOGI("");
                                RLOGI("source_ip_mask %u", out.profile_settings.tft_id1.source_ip_mask);
                                RLOGI("next_header %u", out.profile_settings.tft_id1.next_header);
                                RLOGI("dest_port_range_start %u",
                                        out.profile_settings.tft_id1.dest_port_range_start);
                                RLOGI("dest_port_range_end %u",
                                        out.profile_settings.tft_id1.dest_port_range_end);
                                RLOGI("src_port_range_start %u",
                                        out.profile_settings.tft_id1.src_port_range_start);
                                RLOGI("src_port_range_end %u",
                                        out.profile_settings.tft_id1.src_port_range_end);
                                RLOGI("ipsec_spi %u", out.profile_settings.tft_id1.ipsec_spi);
                                RLOGI("tos_mask %u", out.profile_settings.tft_id1.tos_mask);
                                RLOGI("flow_label %u", out.profile_settings.tft_id1.flow_label);
                            }
                            if (out.profile_settings.tft_id2_available) {
                                RLOGI("filter_id %u", out.profile_settings.tft_id2.filter_id);
                                RLOGI("eval_id %u", out.profile_settings.tft_id2.eval_id);
                                RLOGI("ip_version %u", out.profile_settings.tft_id2.ip_version);
                                RLOGI("ip address: ");
                                for (int i = 0; i < IPV6_ADDR_MAX_SIZE; i++)
                                    RLOGI("%u ", out.profile_settings.tft_id2.source_ip[i]);
                                RLOGI("");
                                RLOGI("source_ip_mask %u", out.profile_settings.tft_id2.source_ip_mask);
                                RLOGI("next_header %u", out.profile_settings.tft_id2.next_header);
                                RLOGI("dest_port_range_start %u",
                                        out.profile_settings.tft_id2.dest_port_range_start);
                                RLOGI("dest_port_range_end %u",
                                        out.profile_settings.tft_id2.dest_port_range_end);
                                RLOGI("src_port_range_start %u",
                                        out.profile_settings.tft_id2.src_port_range_start);
                                RLOGI("src_port_range_end %u",
                                        out.profile_settings.tft_id2.src_port_range_end);
                                RLOGI("ipsec_spi %u", out.profile_settings.tft_id2.ipsec_spi);
                                RLOGI("tos_mask %u", out.profile_settings.tft_id2.tos_mask);
                                RLOGI("flow_label %u", out.profile_settings.tft_id2.flow_label);
                            }
                            if (out.profile_settings.pdp_context_available) {
                                RLOGI("pdp_context_available %u", out.profile_settings.pdp_context);
                            }
                            RLOGI("secondary_flag %u", out.profile_settings.secondary_flag);
                            if (out.profile_settings.primary_id_available) {
                                RLOGI("primary_id_available %u", out.profile_settings.primary_id);
                            }
                            if (out.profile_settings.ipv6_addr_pref_available) {
                                RLOGI("ip address pref: ");
                                for (int i = 0; i < IPV6_ADDR_SIZE; i++)
                                    RLOGI("%u ", out.profile_settings.ipv6_addr_pref[i]);
                                RLOGI("");
                            }
                            if (out.profile_settings.umts_req_qos_signaling_available) {
                                RLOGI("traffic_class %u",
                                        out.profile_settings.umts_req_qos_signaling.traffic_class);
                                RLOGI("max_uplink_bitrate %u",
                                        out.profile_settings.umts_req_qos_signaling.max_uplink_bitrate);
                                RLOGI("max_downlink_bitrate %u",
                                        out.profile_settings.umts_req_qos_signaling.max_downlink_bitrate);
                                RLOGI("guaranteed_uplink_bitrate %u",
                                        out.profile_settings.umts_req_qos_signaling.guaranteed_uplink_bitrate);
                                RLOGI("guaranteed_downlink_bitrate %u",
                                        out.profile_settings.umts_req_qos_signaling.
                                        guaranteed_downlink_bitrate);
                                RLOGI("qos_delivery_order %u",
                                        out.profile_settings.umts_req_qos_signaling.qos_delivery_order);
                                RLOGI("max_sdu_size %u",
                                        out.profile_settings.umts_req_qos_signaling.max_sdu_size);
                                RLOGI("sdu_error_ratio %u",
                                        out.profile_settings.umts_req_qos_signaling.sdu_error_ratio);
                                RLOGI("residual_bit_error_ratio %u",
                                        out.profile_settings.umts_req_qos_signaling.residual_bit_error_ratio);
                                RLOGI("delivery_erroneous_SDUs %u",
                                        out.profile_settings.umts_req_qos_signaling.delivery_erroneous_SDUs);
                                RLOGI("transfer_delay %u",
                                        out.profile_settings.umts_req_qos_signaling.transfer_delay);
                                RLOGI("traffic_handling_priority %u",
                                        out.profile_settings.umts_req_qos_signaling.traffic_handling_priority);
                            }
                            if (out.profile_settings.umts_min_qos_signaling_available) {
                                RLOGI("traffic_class %u",
                                        out.profile_settings.umts_min_qos_signaling.traffic_class);
                                RLOGI("max_uplink_bitrate %u",
                                        out.profile_settings.umts_min_qos_signaling.max_uplink_bitrate);
                                RLOGI("max_downlink_bitrate %u",
                                        out.profile_settings.umts_min_qos_signaling.max_downlink_bitrate);
                                RLOGI("guaranteed_uplink_bitrate %u",
                                        out.profile_settings.umts_min_qos_signaling.guaranteed_uplink_bitrate);
                                RLOGI("guaranteed_downlink_bitrate %u",
                                        out.profile_settings.umts_min_qos_signaling.
                                        guaranteed_downlink_bitrate);
                                RLOGI("qos_delivery_order %u",
                                        out.profile_settings.umts_min_qos_signaling.qos_delivery_order);
                                RLOGI("max_sdu_size %u",
                                        out.profile_settings.umts_min_qos_signaling.max_sdu_size);
                                RLOGI("sdu_error_ratio %u",
                                        out.profile_settings.umts_min_qos_signaling.sdu_error_ratio);
                                RLOGI("residual_bit_error_ratio %u",
                                        out.profile_settings.umts_min_qos_signaling.residual_bit_error_ratio);
                                RLOGI("delivery_erroneous_SDUs %u",
                                        out.profile_settings.umts_min_qos_signaling.delivery_erroneous_SDUs);
                                RLOGI("transfer_delay %u",
                                        out.profile_settings.umts_min_qos_signaling.transfer_delay);
                                RLOGI("traffic_handling_priority %u",
                                        out.profile_settings.umts_min_qos_signaling.traffic_handling_priority);
                            }
                            if (out.profile_settings.pri_dns_ipv6_addr_pref_available) {
                                RLOGI("pri_dns_ipv6_addr_pref_len: ");
                                for (int i = 0; i < IPV6_ADDR_SIZE; i++)
                                    RLOGI("%u ", out.profile_settings.pri_dns_ipv6_addr_pref[i]);
                                RLOGI("");
                            }
                            if (out.profile_settings.sec_dns_ipv6_addr_pref_available) {
                                RLOGI("sec_dns_ipv6_addr_pref_len: ");
                                for (int i = 0; i < IPV6_ADDR_SIZE; i++)
                                    RLOGI("%u ", out.profile_settings.sec_dns_ipv6_addr_pref[i]);
                                RLOGI("");
                            }
                            if (out.profile_settings.addr_allocation_pref_available) {
                                RLOGI("addr_allocation_pref %u", out.profile_settings.addr_allocation_pref);
                            }
                            if (out.profile_settings.lte_qos_params_available) {
                                RLOGI("qci %u", out.profile_settings.lte_qos_params.qci);
                                RLOGI("g_dl_bit_rate %u", out.profile_settings.lte_qos_params.g_dl_bit_rate);
                                RLOGI("max_dl_bit_rate %u",
                                        out.profile_settings.lte_qos_params.max_dl_bit_rate);
                                RLOGI("g_ul_bit_rate %u", out.profile_settings.lte_qos_params.g_ul_bit_rate);
                                RLOGI("max_ul_bit_rate %u",
                                        out.profile_settings.lte_qos_params.max_ul_bit_rate);
                            }
                            RLOGI("apn_disabled_flag %u", out.profile_settings.apn_disabled_flag);
                            if (out.profile_settings.pdn_inactivity_timeout_available) {
                                RLOGI("pdn_inactivity_timeout %u",
                                        out.profile_settings.pdn_inactivity_timeout);
                            }
                            if (out.profile_settings.apn_class_available) {
                                RLOGI("apn_class %u", out.profile_settings.apn_class);
                            }
                            RLOGI("support_emergency_calls %u", out.profile_settings.support_emergency_calls);
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
