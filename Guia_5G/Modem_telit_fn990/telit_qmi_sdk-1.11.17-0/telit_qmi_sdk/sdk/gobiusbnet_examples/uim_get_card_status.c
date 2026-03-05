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
#include "telit_sdk_uim.h"

#define QMI_GET_SERVICE_FILE_IOCTL 0x8BE0 + 1

/* change this define if QMI device name is different */
#define GOBINET_DEVICE "/dev/qcqmi0"

#define SERVICE QMI_SVC_UIM
#define SVC_STR "UIM"

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
    uim_card_status_info_t out;

    memset(&out, 0, sizeof(out));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    telit_set_log_level(LOG_INFO);

    rtn = telit_uim_get_card_status_pack(&req_ctx, req, &req_size);

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

        RLOGD("rtn = %d, rsp_size = %u", rtn, rsp_size);

        if ((rtn > 0) && (rsp_size > 0)) {
            telit_helper_get_resp_ctx(SERVICE, rsp, rsp_size, &rsp_ctx);

            switch (rsp_ctx.type) {
                case QMI_MSG_RESPONSE:
                    RLOGD("%s response message, msg_id 0x%x", SVC_STR, rsp_ctx.msg_id);

                    if (rsp_ctx.xid == xid) {
                        rtn = telit_uim_get_card_status_unpack(rsp, rsp_size, &out);

                        if (rtn != QMI_SDK_ERR_NONE) {
                            RLOGE("%s unpack fail %d ", SVC_STR, rtn);
                        } else {
                            RLOGI("%s unpack ok", SVC_STR);

                            if (out.card_status_available) {
                                RLOGI("index_gw_pri: %u", out.card_status.index_gw_pri);
                                RLOGI("index_1x_pri: %u", out.card_status.index_1x_pri);
                                RLOGI("index_gw_sec: %u", out.card_status.index_gw_sec);
                                RLOGI("index_1x_sec: %u", out.card_status.index_1x_sec);
                                RLOGI("num_slot: %u", out.card_status.num_slot);

                                for (uint16_t i = 0; i < out.card_status.num_slot; i++) {
                                    RLOGI("slot %u", i);
                                    RLOGI("\tcard_state: %u", out.card_status.slot[i].card_state);
                                    RLOGI("\tupin_state: %u", out.card_status.slot[i].upin_state);
                                    RLOGI("\tupin_retries: %u", out.card_status.slot[i].upin_retries);
                                    RLOGI("\tupuk_retries: %u", out.card_status.slot[i].upuk_retries);
                                    RLOGI("\terror: %u", out.card_status.slot[i].error);
                                    RLOGI("\tnum_app: %u", out.card_status.slot[i].num_app);

                                    for (uint16_t j = 0; j < out.card_status.slot[i].num_app; j++) {
                                        RLOGI("app %u", j);
                                        RLOGI("\t\ttype: %u", out.card_status.slot[i].app[j].type);
                                        RLOGI("\t\tstate: %u", out.card_status.slot[i].app[j].state);
                                        RLOGI("\t\tperso_state: %u",
                                                out.card_status.slot[i].app[j].perso_state);
                                        RLOGI("\t\tperso_feature: %u",
                                                out.card_status.slot[i].app[j].perso_feature);
                                        RLOGI("\t\tperso_retries: %u",
                                                out.card_status.slot[i].app[j].perso_retries);
                                        RLOGI("\t\tperso_unblock_retries: %u",
                                                out.card_status.slot[i].app[j].perso_unblock_retries);
                                        RLOGI("\t\taid_len: %u", out.card_status.slot[i].app[j].aid_len);

                                        telit_print_buffer(out.card_status.slot[i].app[j].aid,
                                                out.card_status.slot[i].app[j].aid_len,
                                                LOG_INFO);

                                        RLOGI("\t\tuniv_pin: %u", out.card_status.slot[i].app[j].univ_pin);
                                        RLOGI("\t\tpin1_state: %u",
                                                out.card_status.slot[i].app[j].pin1_state);
                                        RLOGI("\t\tpin1_retries: %u",
                                                out.card_status.slot[i].app[j].pin1_retries);
                                        RLOGI("\t\tpuk1_retries: %u",
                                                out.card_status.slot[i].app[j].puk1_retries);
                                        RLOGI("\t\tpin2_state: %u",
                                                out.card_status.slot[i].app[j].pin2_state);
                                        RLOGI("\t\tpin2_retries: %u",
                                                out.card_status.slot[i].app[j].pin2_retries);
                                        RLOGI("\t\tpuk2_retries: %u",
                                                out.card_status.slot[i].app[j].puk2_retries);
                                    }
                                }
                            }

                            if (out.hot_swap_available) {
                                RLOGI("hot_swap_len: %u", out.hot_swap_status.hot_swap_len);
                                telit_print_buffer(out.hot_swap_status.hot_swap,
                                        out.hot_swap_status.hot_swap_len,
                                        LOG_INFO);
                            }
                        }

                        got_expected_message = true;
                    } else {
                        RLOGI("xid %d", xid);
                    }
                    break;

                case QMI_MSG_INDICATION:
                    RLOGD("%s Indication message, msg_id 0x%x", SVC_STR, rsp_ctx.msg_id);
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
