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
    wds_get_packet_statistics_mask_t input;
    wds_get_packet_statistics_t out;

    memset(&input, 0, sizeof(input));
    memset(&out, 0, sizeof(out));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    input.stats_mask = (0x00000001 |
            0x00000002 |
            0x00000004 |
            0x00000008 |
            0x00000010 |
            0x00000020 |
            0x00000040 |
            0x00000080 |
            0x00000100 |
            0x00000200);

    telit_set_log_level(LOG_INFO);

    rtn = telit_wds_get_pkt_statistics_pack(&req_ctx, req, &req_size, &input);

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
                        rtn = telit_wds_get_pkt_statistics_unpack(rsp, rsp_size, &out);

                        if (rtn != QMI_SDK_ERR_NONE) {
                            RLOGE("%s unpack fail %d ", SVC_STR, rtn);
                        } else {
                            RLOGI("%s unpack ok", SVC_STR);

                            if (out.tx_out_count_available) {
                                RLOGI("wds tx_out_count: %u", out.tx_out_count);
                            }
                            if (out.rx_out_count_available) {
                                RLOGI("wds rx_out_count: %u", out.rx_out_count);
                            }
                            if (out.tx_err_count_available) {
                                RLOGI("wds tx_err_count: %u", out.tx_err_count);
                            }
                            if (out.rx_err_count_available) {
                                RLOGI("wds rx_err_count: %u", out.rx_err_count);
                            }
                            if (out.tx_ofl_count_available) {
                                RLOGI("wds tx_ofl_count: %u", out.tx_ofl_count);
                            }
                            if (out.rx_ofl_count_available) {
                                RLOGI("wds rx_ofl_count: %u", out.rx_ofl_count);
                            }
                            if (out.tx_ok_bytes_count_available) {
                                RLOGI("wds tx_ok_bytes_count: %u", out.tx_ok_bytes_count);
                            }
                            if (out.rx_ok_bytes_count_available) {
                                RLOGI("wds rx_ok_bytes_count: %u", out.rx_ok_bytes_count);
                            }
                            if (out.last_call_tx_ok_bytes_count_available) {
                                RLOGI("wds last_call_tx_ok_bytes_count: %u", out.last_call_tx_ok_bytes_count);
                            }
                            if (out.last_call_rx_ok_bytes_count_available) {
                                RLOGI("wds last_call_rx_ok_bytes_count: %u", out.last_call_rx_ok_bytes_count);
                            }
                            if (out.tx_dropped_count_available) {
                                RLOGI("wds tx_dropped_count: %u", out.tx_dropped_count);
                            }
                            if (out.rx_dropped_count_available) {
                                RLOGI("wds rx_dropped_count: %u", out.rx_dropped_count);
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
