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

/* ATTENTION: change pin according to your sim when running this test */
#define PIN "1958"

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
    uim_pin_protection_info_t input;
    uim_pin_resp_t out;

    memset(&input, 0, sizeof(input));
    memset(&out, 0, sizeof(out));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    input.session_information.session_type = QMI_UIM_SESSION_TYPE_PRIMARY_GW_PROVISIONING;
    input.session_information.aid_len = 0;
    input.pin_info.pin_id = QMI_UIM_PIN_ID_PIN1;
    input.pin_info.pin_operation = QMI_UIM_PIN_OPERATION_DISABLE;
    input.pin_info.pin_len = strlen(PIN);
    strlcpy(input.pin_info.pin, PIN, sizeof(input.pin_info.pin));

    /* UNCOMMENT THIS FOR RESPONSE AS INDICATION */
    /*
       input.response_as_indication_available = true;
       input.indication_token = 0x00FFFFFF;
     */

    telit_set_log_level(LOG_INFO);

    rtn = telit_uim_set_pin_protection_pack(&req_ctx, req, &req_size, &input);

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
                        rtn = telit_uim_set_pin_protection_unpack(rsp, rsp_size, &out);

                        if (rtn != QMI_SDK_ERR_NONE) {
                            RLOGE("%s unpack fail %d ", SVC_STR, rtn);

                            if (out.pin_retries_available) {
                                RLOGE("verify left %u, unblock left %u",
                                        out.retries_remaining.verify_left,
                                        out.retries_remaining.unblock_left);
                            }
                        } else {
                            RLOGI("%s unpack ok", SVC_STR);
                        }

                        got_expected_message = true;
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
                            case QMI_UIM_VERIFY_PIN_IND: {
                                uim_pin_resp_t *ev = (uim_pin_resp_t *)ind;

                                if (ev->pin_retries_available) {
                                    RLOGI("verify left %u, unblock left %u",
                                            out.retries_remaining.verify_left,
                                            out.retries_remaining.unblock_left);
                                }

                                if (ev->response_as_indication_available) {
                                    RLOGI("Token: 0x%x", ev->indication_token);
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
