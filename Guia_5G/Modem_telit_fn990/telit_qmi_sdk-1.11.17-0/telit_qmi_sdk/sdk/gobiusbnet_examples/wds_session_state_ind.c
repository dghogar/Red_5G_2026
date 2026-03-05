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
    wds_start_network_info_t input;
    wds_start_network_resp_t out;

    memset(&input, 0, sizeof(input));
    memset(&out, 0, sizeof(out));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    input.profile_index_3gpp_available = true;
    input.profile_index_3gpp = INDEX;

    telit_set_log_level(LOG_INFO);

    rtn = telit_wds_start_network_interface_pack(&req_ctx, req, &req_size, &input);

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
                        rtn = telit_wds_start_network_interface_unpack(rsp, rsp_size, &out);

                        if (rtn != QMI_SDK_ERR_NONE) {
                            RLOGE("%s unpack fail %d ", SVC_STR, rtn);
                        } else {
                            RLOGI("%s unpack ok", SVC_STR);
                        }
                    } else {
                        RLOGI("xid %d", xid);
                    }
                    break;

                case QMI_MSG_INDICATION: {
                    uint8_t ind[4096];
                    RLOGD("%s Indication message, msgid 0x%x", SVC_STR, rsp_ctx.msg_id);

                    if (telit_wds_indication_handler(rsp_ctx.msg_id,
                            rsp, rsp_size, ind, 4096) == QMI_SDK_ERR_NONE) {
                        switch (rsp_ctx.msg_id) {
                            case QMI_WDS_EVENT_REPORT_IND: {
                                wds_ind_event_report_t *ev = (wds_ind_event_report_t *)ind;

                                if (ev->tx_ok_count_available) {
                                    RLOGI("tx_ok_count %u", ev->tx_ok_count);
                                }
                                if (ev->rx_ok_count_available) {
                                    RLOGI("rx_ok_count %u", ev->rx_ok_count);
                                }
                                if (ev->tx_ok_bytes_count_available) {
                                    RLOGI("tx_ok_bytes_count %lu", ev->tx_ok_bytes_count);
                                }
                                if (ev->rx_ok_bytes_count_available) {
                                    RLOGI("rx_ok_bytes_count %lu", ev->rx_ok_bytes_count);
                                }
                                if (ev->data_bearer_tech_available) {
                                    RLOGI("data_bearer_tech %u", ev->data_bearer_tech);
                                }
                                if (ev->mip_status_available) {
                                    RLOGI("mip_status %u", ev->mip_status);
                                }
                                if (ev->dormancy_status_available) {
                                    RLOGI("data_call_status %u", ev->dormancy_status);
                                }
                                if (ev->current_data_bearer_tech_available) {
                                    RLOGI("current_nw %u", ev->current_data_bearer_tech.current_nw);
                                    RLOGI("rat_mask %u", ev->current_data_bearer_tech.rat_mask);
                                    RLOGI("so_mask %u", ev->current_data_bearer_tech.so_mask);
                                }
                                if (ev->data_bearer_tech_ext_available) {
                                    RLOGI("technology %u", ev->data_bearer_tech_ext.technology);
                                    RLOGI("rat_mask %u", ev->data_bearer_tech_ext.rat_value);
                                    RLOGI("so_mask %lu", ev->data_bearer_tech_ext.so_mask);
                                }
                                if (ev->data_system_status_available) {
                                    uint8_t i;

                                    RLOGI("preferred network %u", ev->data_system_status.preferred_network);
                                    RLOGI("network_info_len %u", ev->data_system_status.network_info_size);
                                    for (i = 0; i < ev->data_system_status.network_info_size; i++) {
                                        RLOGI("network %u", ev->data_system_status.network_info[i].network);
                                        RLOGI("rat_mask %u", ev->data_system_status.network_info[i].rat_mask);
                                        RLOGI("so_mask %u", ev->data_system_status.network_info[i].so_mask);
                                    }
                                }
                                break;
                            }
                            case QMI_WDS_PKT_SRVC_STATUS_IND: {
                                wds_ind_session_state_t *ev = (wds_ind_session_state_t *)ind;

                                RLOGI("connection_status %u", ev->connection_status);
                                RLOGI("reconfiguration_required %d", ev->reconfiguration_required);
                                if (ev->call_end_reason_available) {
                                    RLOGI("call_end_reason %u", ev->call_end_reason);
                                }
                                if (ev->call_end_reason_verbose_available) {
                                    RLOGI("call_end_reason_type %u",
                                            ev->call_end_reason_verbose.call_end_reason_type);
                                    RLOGI("call_end_reason %u", ev->call_end_reason_verbose.call_end_reason);
                                }
                                if (ev->ip_family_available) {
                                    RLOGI("ip_family %u", ev->ip_family);
                                }
                                if (ev->tech_name_available) {
                                    RLOGI("ip_family %u", ev->tech_name);
                                }
                                if (ev->bearer_id_available) {
                                    RLOGI("ip_family %u", ev->bearer_id);
                                }

                                got_expected_message = true;

                                break;
                            }
                            default:
                                RLOGD("%s Indication 0x%x not managed", SVC_STR, rsp_ctx.msg_id);
                                break;
                        }
                    }

                    break;
                }

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
