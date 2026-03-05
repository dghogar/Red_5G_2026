/* QMI_CTL_RELEASE_CLIENT_ID */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>

#include "telit_sdk.h"
#include "telit_sdk_ctl.h"

uint8_t rsp[QMI_MSG_MAX];
uint8_t req[QMI_MSG_MAX];
uint8_t dev_buf[QMI_MSG_MAX];

static unsigned xid = 1;

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";

static void hex_dump(uint8_t *buf, uint16_t len)
{
    uint16_t i;

    for (i = 0; i < len; i++) {
        fprintf(stderr, "0x%02X,", buf[i]);
    }
    fprintf(stderr, "\n");
}

static int decode_response(unpack_qmi_t *rsp_ctx, uint8_t *payload, uint16_t payload_len)
{
    int rtn;
    ctl_get_client_id_info_t out;

    memset(&out, 0, sizeof(out));

    switch (rsp_ctx->type) {
        /* received QMI response message */
        case QMI_MSG_RESPONSE:
            RLOGD("msg_id 0x%x", rsp_ctx->msg_id);

            /* since this is a response message, make sure transaction id */
            /* is matched. otherwise this is not the response we are waiting for */
            /* at any given time, there should be only one pending request. */
            if (rsp_ctx->xid == xid) {
                /* call unpack function to retreive information from */
                /* QMI TLV in response message */
                rtn = telit_ctl_release_client_id_unpack(payload, payload_len, &out);
                if (rtn != QMI_SDK_ERR_NONE) {
                    RLOGI("unpack fail %d ", rtn);
                    rtn = -1;
                } else {
                    RLOGI("unpack OK");
                    RLOGI("qmi_svc_type: %u", out.qmi_svc_type);
                    RLOGI("client_id: %u", out.client_id);

                    rtn = payload_len;
                }
            } else {
                /* the response is not for this API call, skip it and read again */
                RLOGD("xid %d", xid);
                rtn = 0;
            }
            break;

        /* received QMI indication message */
        case QMI_MSG_INDICATION:
            RLOGD("Indication message: ");
            RLOGD("msg_id 0x%x", rsp_ctx->msg_id);

            rtn = 0;
            break;

        /* Unknow message type. neither response nor indication. */
        /* skip it. This shouldn't happen */
        default:
            RLOGD("QMI message type : %d", rsp_ctx->type);
            rtn = 0;
            break;
    }

    return rtn;
}

int main(int argc, char **argv)
{
    int fd;
    bool exit_wait = false;
    pack_qmi_t req_ctx;
    unpack_qmi_t rsp_ctx;
    int rtn;
    uint16_t req_len = QMI_MSG_MAX;
    uint16_t rsp_len = QMI_MSG_MAX;
    uint16_t dev_buf_len = QMI_MSG_MAX;
    struct pollfd p_fd;
    ctl_get_client_id_info_t input;
    char *client_id;

    if (argc == 2) {
        dev = default_rmnet_dev;
        client_id = argv[1];
    } else {
        dev = argv[1];
        client_id = argv[2];
    }

    memset(&input, 0, sizeof(input));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    telit_set_log_level(LOG_DEBUG);

    input.qmi_svc_type = QMI_SVC_DMS;
    input.client_id = strtoul(client_id, NULL, 10);
    /* calling pack API for preparing a message to modem */
    /* pack routine should return exact size of a prepared message */
    /* in req_len. and the prepared message will be in req buffer */
    rtn = telit_ctl_release_client_id_pack(&req_ctx, req, &req_len, &input);
    if (rtn != QMI_SDK_ERR_NONE) {
        RLOGE("pack fail %d ", rtn);
        exit(EXIT_FAILURE);
    }
    hex_dump(req, req_len);

    rtn = add_qmux_header(&req_ctx,
            0,
            req, req_len,
            dev_buf, &dev_buf_len);
    if (rtn != QMI_SDK_ERR_NONE) {
        RLOGE("add qmux header fail %d ", rtn);
        exit(EXIT_FAILURE);
    }
    hex_dump(dev_buf, dev_buf_len);

    RLOGD("Opening device %s", dev);
    fd = open(dev, O_NONBLOCK | O_RDWR);
    if (fd == -1) {
        RLOGE("Failed opening device");
        strerror(errno);
        exit(EXIT_FAILURE);
    }

    rtn = write(fd, dev_buf, dev_buf_len);
    if (rtn != dev_buf_len) {
        RLOGE("Written %d of %u", rtn, dev_buf_len);
        goto close_fd;
    }

    p_fd.fd = fd;
    p_fd.events = POLLIN | POLLHUP | POLLERR | POLLNVAL;
    while (!exit_wait) {
        rtn = poll(&p_fd, 1, req_ctx.timeout * 1000);
        if (rtn == -1) {
            RLOGE("Failed poll");
            strerror(errno);
            goto close_fd;
        }
        if (rtn == 0) {
            RLOGE("Timeout");
            goto close_fd;
        }
        if (p_fd.revents & POLLIN) {
            rtn = read(fd, rsp, QMI_MSG_MAX);
            if (rtn > 0) {
                uint8_t *payload;
                uint16_t payload_len;

                rsp_len = rtn;
                RLOGI("rtn = %d, rsp_len = %u", rtn, rsp_len);
                hex_dump(rsp, rsp_len);

                rtn = skip_qmux_header(rsp, rsp_len,
                        &payload, &payload_len);
                if (rtn != QMI_SDK_ERR_NONE) {
                    RLOGE("failed to skip qmux header", rtn);
                    goto close_fd;
                }
                RLOGI("payload_len = %u, payload = %p, rsp = %p",
                        payload_len, payload, rsp);
                hex_dump(payload, payload_len);

                telit_helper_get_resp_ctx(req_ctx.svc_id, payload, payload_len, &rsp_ctx);

                rtn = decode_response(&rsp_ctx,
                        payload, payload_len);
                if (rtn == -1) {
                    RLOGE("error in decoding response");
                    goto close_fd;
                }

                if (rtn == 0) {
                    RLOGD("Still waiting for the desired event");
                    continue;
                }

                exit_wait = true;
            } else {
                RLOGE("Poll successful, but 0 bytes read");
            }
        } else {
            RLOGE("One of POLLHUP, POLLERR or POLLNVAL happened");
            goto close_fd;
        }
    }

close_fd:
    close(fd);

    return 0;
}
