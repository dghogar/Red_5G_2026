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

#define OLD_PIN "0000"
#define NEW_PIN "0000"

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
    uim_read_record_t input;
    uim_read_record_info_t out;

    memset(&input, 0, sizeof(input));
    memset(&out, 0, sizeof(out));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    input.file_id = 0x6F07;
    input.file_path_len = 2 * sizeof(uint16_t);
    input.file_path[0] = 0x3F00;
    input.file_path[1] = 0x7f21;
    input.session_information.session_type = QMI_UIM_SESSION_TYPE_PRIMARY_GW_PROVISIONING;
    input.session_information.aid_len = 0;

    /* UNCOMMENT THIS FOR RESPONSE AS INDICATION */

    input.response_as_indication_available = true;
    input.indication_token = 0x00FFFFFF;

    telit_set_log_level(LOG_INFO);

    rtn = telit_uim_read_record_pack(&req_ctx, req, &req_size, &input);

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
                        rtn = telit_uim_read_record_unpack(rsp, rsp_size, &out);

                        if (rtn != QMI_SDK_ERR_NONE) {
                            RLOGE("%s unpack fail %d ", SVC_STR, rtn);
                        } else {
                            RLOGI("%s unpack ok", SVC_STR);
                            if (out.card_result_available) {
                                RLOGI("Card result: sw1:%X, sw2:%X",
                                        out.card_result.sw1,
                                        out.card_result.sw2);
                            }

                            RLOGI("Read result: len: %u", out.read_result.len);

                            if (out.read_result_available && out.read_result.len) {
                                telit_print_buffer(out.read_result.content, out.read_result.len, LOG_INFO);
                            }
                        }
                    } else {
                        RLOGI("xid %d", xid);
                    }
                    break;

                case QMI_MSG_INDICATION: {
                    uint8_t ind[6000];
                    RLOGD("%s Indication message, msg_id 0x%x", SVC_STR, rsp_ctx.msg_id);

                    if (telit_uim_indication_handler(rsp_ctx.msg_id,
                            rsp, rsp_size, ind, 6000) == QMI_SDK_ERR_NONE) {
                        switch (rsp_ctx.msg_id) {
                            case QMI_UIM_READ_TRANSPARENT_IND: {
                                uim_read_record_info_t *ev = (uim_read_record_info_t *)ind;

                                if (ev->card_result_available) {
                                    RLOGI("Card result: sw1:%X, sw2:%X",
                                            ev->card_result.sw1,
                                            ev->card_result.sw2);
                                }

                                if (ev->response_as_indication_available) {
                                    RLOGI("Token: 0x%x", ev->indication_token);
                                }

                                RLOGI("Read result: len: %u", ev->read_result.len);
                                if (ev->read_result_available && ev->read_result.len) {
                                    telit_print_buffer(ev->read_result.content, ev->read_result.len,
                                            LOG_INFO);
                                }
                                break;
                            }
                            default:
                                RLOGI("Indication not managed");
                                break;
                        }
                    }

                    break;
                }

                default:
                    RLOGD("Unknown QMI message type: %d", rsp_ctx.type);
                    break;
            }

            got_expected_message = true;
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
