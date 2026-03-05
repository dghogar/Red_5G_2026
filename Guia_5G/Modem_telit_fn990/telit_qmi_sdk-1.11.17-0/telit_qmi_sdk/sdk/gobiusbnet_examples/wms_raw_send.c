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
#include "telit_sdk_wms.h"
#include "telit_sms_helper.h"

#define QMI_GET_SERVICE_FILE_IOCTL 0x8BE0 + 1

/* change this define if QMI device name is different */
#define GOBINET_DEVICE "/dev/qcqmi0"

#define DEFAULT_MEMORY_INDEX 1

#define SERVICE QMI_SVC_WMS
#define SVC_STR "WMS"

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

int main(int argc __unused, char **argv __unused)
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
    wms_raw_send_t input;
    wms_raw_send_resp_t out;
    telit_helper_sms_encode_t sms = {0};
    int32_t raw_data_size = 0;
    const char message[] = "Test send SMS from wds demo!";
    const char number[] = "+001234567890";

    memset(&input, 0, sizeof(input));
    memset(&out, 0, sizeof(out));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    sms.message_size = strlen(message);
    strcpy(sms.text_msg, message);
    memcpy(sms.dest_addr, (int8_t *)number, strlen(number));

    raw_data_size = telit_helper_sms_encode(&sms);

    if (!raw_data_size) {
        RLOGE("could not encode message");
        return -1;
    }

    input.format = QMI_WMS_MESSAGE_FORMAT_GWPP;
    input.raw_data_size = raw_data_size;
    memcpy(input.raw_data, sms.pdu_message, raw_data_size);

    telit_set_log_level(LOG_INFO);

    rtn = telit_wms_raw_send_pack(&req_ctx, req, &req_size, &input);

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
                        rtn = telit_wms_raw_send_unpack(rsp, rsp_size, &out);

                        if (rtn != QMI_SDK_ERR_NONE) {
                            RLOGE("%s unpack fail %d ", SVC_STR, rtn);
                        } else {
                            RLOGI("%s unpack ok", SVC_STR);

                            RLOGI("message id: %d", out.message_id);
                            if (out.cause_code_available) {
                                RLOGI("cause code %d", out.cause_code);
                            }
                            if (out.error_class_available) {
                                RLOGI("error_class %d", out.error_class);
                            }
                            if (out.gw_cause_info_available) {
                                RLOGI("gw_cause_info RP cause %d", out.gw_cause_info.rp_cause);
                                RLOGI("gw_cause_info TP cause %d", out.gw_cause_info.tp_cause);
                            }
                            if (out.message_delivery_failure_type_available) {
                                RLOGI("message_delivery_failure_type %d", out.message_delivery_failure_type);
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
