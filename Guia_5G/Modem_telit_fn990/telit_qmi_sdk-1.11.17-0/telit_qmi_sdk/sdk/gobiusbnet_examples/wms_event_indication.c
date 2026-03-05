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
    wms_set_event_t input;

    memset(&input, 0, sizeof(input));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    input.report_mt_message = 0x01;
    input.report_mt_message_available = true;
    input.report_call_control_info = 0x01;
    input.report_call_control_info_available = true;
    input.report_mwi_message = 0x01;
    input.report_mwi_message_available = true;

    telit_set_log_level(LOG_INFO);

    rtn = telit_wms_set_event_report_pack(&req_ctx, req, &req_size, &input);

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
                    RLOGD("%s response message, msgid 0x%x", SVC_STR, rsp_ctx.msg_id);

                    if (rsp_ctx.xid == xid) {
                        rtn = telit_wms_set_event_report_unpack(rsp, rsp_size);

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

                    if (telit_wms_indication_handler(rsp_ctx.msg_id,
                            rsp, rsp_size, ind, 4096) == QMI_SDK_ERR_NONE) {
                        RLOGI("indication id %d", rsp_ctx.msg_id);

                        switch (rsp_ctx.msg_id) {
                            case QMI_WMS_EVENT_REPORT_IND: {
                                wms_ind_event_report_t *ev = (wms_ind_event_report_t *)ind;

                                if (ev->mt_message_set) {
                                    RLOGI("storage type %d", ev->mt_message.storage_type);
                                    RLOGI("storage index %d", ev->mt_message.storage_index);
                                }

                                if (ev->message_mode_set) {
                                    RLOGI("message mode %d", ev->message_mode);
                                }

                                if (ev->notification_type_set) {
                                    RLOGI("notification type %d", ev->notification_type.type);
                                    RLOGI("data len %d", ev->notification_type.size);
                                }

                                if (ev->etws_plmn_info_set) {
                                    RLOGI("mobile country code %d", ev->etws_plmn_info.mobile_country_code);
                                    RLOGI("mobile network code %d", ev->etws_plmn_info.mobile_network_code);
                                }

                                if (ev->smsc_address_set) {
                                    RLOGI("sms address len %d", ev->smsc_address.len);
                                }

                                if (ev->sms_on_ims_set) {
                                    RLOGI("sms on ims %d", ev->sms_on_ims);
                                }

                                got_expected_message = true;
                                break;
                            }

                            default:
                                RLOGE("unsupported event %d", rsp_ctx.msg_id);
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
