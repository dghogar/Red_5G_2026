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
    nas_tx_rx_info_t out;
    nas_radio_if_t input;

    memset(&input, 0, sizeof(input));
    memset(&out, 0, sizeof(out));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    input.radio_if = 0x08;  /* LTE */

    telit_set_log_level(LOG_INFO);

    rtn = telit_nas_get_tx_rx_info_pack(&req_ctx, req, &req_size, &input);

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
                        rtn = telit_nas_get_tx_rx_info_unpack(rsp, rsp_size, &out);

                        if (rtn != QMI_SDK_ERR_NONE) {
                            RLOGE("%s unpack pack fail %d ", SVC_STR, rtn);
                        } else {
                            if (out.rx_chain_0_info_available) {
                                RLOGI("Rx chain 0 info, is_radio_tuned %d",
                                        out.rx_chain_0_info.is_radio_tuned);
                                RLOGI("Rx chain 0 info, rx_pwr %d", out.rx_chain_0_info.rx_pwr);
                                RLOGI("Rx chain 0 info, ecio %d", out.rx_chain_0_info.ecio);
                                RLOGI("Rx chain 0 info, rscp %d", out.rx_chain_0_info.rscp);
                                RLOGI("Rx chain 0 info, rsrp %d", out.rx_chain_0_info.rsrp);
                                RLOGI("Rx chain 0 info, phase %d", out.rx_chain_0_info.phase);
                            }

                            if (out.rx_chain_1_info_available) {
                                RLOGI("Rx chain 1 info, is_radio_tuned %d",
                                        out.rx_chain_1_info.is_radio_tuned);
                                RLOGI("Rx chain 1 info, rx_pwr %d", out.rx_chain_1_info.rx_pwr);
                                RLOGI("Rx chain 1 info, ecio %d", out.rx_chain_1_info.ecio);
                                RLOGI("Rx chain 1 info, rscp %d", out.rx_chain_1_info.rscp);
                                RLOGI("Rx chain 1 info, rsrp %d", out.rx_chain_1_info.rsrp);
                                RLOGI("Rx chain 1 info, phase %d", out.rx_chain_1_info.phase);
                            }

                            if (out.tx_info_available) {
                                RLOGI("Tx info, is_in_traffic %d", out.tx_info.is_in_traffic);
                                RLOGI("Tx info, tx_pwr %d", out.tx_info.tx_pwr);
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
