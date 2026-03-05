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
#include "telit_sdk_loc.h"

#define QMI_GET_SERVICE_FILE_IOCTL 0x8BE0 + 1

/* change this define if QMI device name is different */
#define GOBINET_DEVICE "/dev/qcqmi0"

#define SERVICE QMI_SVC_LOC
#define SVC_STR "LOC"

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
    loc_event_register_t input;
    uint8_t count = 0;

    memset(&input, 0, sizeof(input));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    input.event_mask = QMI_LOC_EVENT_MASK_POSITION_REPORT |
            QMI_LOC_EVENT_MASK_NMEA;

    telit_set_log_level(LOG_DEBUG);

    rtn = telit_loc_reg_events_pack(&req_ctx, req, &req_size, &input);

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
        } else {
            rsp_size = 0;
        }

        RLOGD("rtn = %d, rsp_size = %u", rtn, rsp_size);

        if ((rtn > 0) && (rsp_size > 0)) {
            telit_helper_get_resp_ctx(SERVICE, rsp, rsp_size, &rsp_ctx);

            switch (rsp_ctx.type) {
                case QMI_MSG_RESPONSE:
                    RLOGD("%s response message, msg_id 0x%x", SVC_STR, rsp_ctx.msg_id);

                    if (rsp_ctx.xid == xid) {
                        rtn = telit_loc_reg_events_unpack(rsp, rsp_size);

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
                    RLOGD("%s Indication message, msg_id 0x%x", SVC_STR, rsp_ctx.msg_id);

                    if (telit_loc_indication_handler(rsp_ctx.msg_id,
                            rsp, rsp_size, ind, 4096) == QMI_SDK_ERR_NONE) {
                        switch (rsp_ctx.msg_id) {
                            case QMI_LOC_DELETE_ASSIST_DATA_IND:
                            case QMI_LOC_SET_NMEA_TYPES_IND:
                            case QMI_LOC_SET_OPERATION_MODE_IND: {
                                loc_status_ind_t *ev = (loc_status_ind_t *)ind;

                                RLOGI("status %d", ev->status);
                                break;
                            }
                            case QMI_LOC_EVENT_NMEA_IND: {
                                loc_nmea_ind_t *ev = (loc_nmea_ind_t *)ind;

                                RLOGI("nmea string: %s", ev->nmea_string);
                                break;
                            }
                            default:
                                RLOGI("Indication not managed");
                                break;
                        }

                        count++;

                        if (count >= 10) {
                            got_expected_message = true;
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
