#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <inttypes.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>
#include <telit_sdk_ctl.h>
#include <telit_sdk_qrtr.h>

#include "tqcm.h"
#include "tqcm_private.h"

static response_item_t *get_response_item(write_queue_item_t *item)
{
    response_item_t *resp;

    resp = malloc(sizeof(*resp));
    if (!resp) {
        RLOGE("%s: resp out of memory", __FUNCTION__);
        goto end;
    }

    resp->buf = (uint8_t *)malloc(item->buf_len);
    if (!resp->buf) {
        RLOGE("%s: resp->buf out of memory", __FUNCTION__);
        free(resp);
        resp = NULL;
        goto end;
    }

    memcpy(resp->buf, item->buf, item->buf_len);
    resp->buf_len = item->buf_len;
    resp->token = item->token;

end:
    return resp;
}

/* Right now the item should be removed by user (cb or sync function) */
static int decode_qrtr_response(device_list_t *dev,
        unpack_qmi_t *rsp_ctx,
        qrtr_service_item_t *qrtr_srvice_item,
        uint8_t *payload,
        uint16_t payload_len)
{
    int ret = 0;

    switch (rsp_ctx->type) {
        /* received QMI response message */
        case QMI_MSG_RESPONSE: {
            write_queue_item_t *item = NULL;

            RLOGD("%s: Response msg_id 0x%x", __FUNCTION__, rsp_ctx->msg_id);

            /* protect write queue, buffer and write itself */
            LOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);

            /* search queue for message with related transaction id */
            TAILQ_FOREACH(item, &dev->write_tailq_head, entries) {
                if (item->ctx_client) {
                    if (item->ctx_client->service_id == qrtr_srvice_item->service_id &&
                            item->ctx_client->sock == qrtr_srvice_item->sock &&
                            item->ctx_client->transaction_id == rsp_ctx->xid) {

                        /* Found */
                        item->buf = calloc(payload_len, sizeof(uint8_t));
                        if (!item->buf) {
                            RLOGE("%s: Out of memory", __FUNCTION__);
                            ret = ENOMEM;
                            UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
                            goto end;
                        }

                        item->buf_len = payload_len;
                        memcpy(item->buf, payload, item->buf_len);

                        /* Signal semaphore? */
                        if (item->sync_mutex) {
                            sem_post(item->sync_mutex);
                        }
                        UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);

                        /* Callback? */
                        if (item->cb) {
                            response_item_t *response;

                            /* Since it is async, remove item from write queue */
                            LOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
                            TAILQ_REMOVE(&dev->write_tailq_head, item, entries);
                            UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);

                            /* Create a response_item */
                            response = get_response_item(item);
                            if (!response) {
                                ret = ENOMEM;
                            } else {
                                item->cb(response);
                            }
                            free(item);
                        }

                        goto end;
                    }
                }
            }

            /* Not found, simply ignore the response */
            if (!item) {
                UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
            }
            break;
        }

        /* received QMI indication message */
        case QMI_MSG_INDICATION: {
            indication_callback_item_t *item;

            RLOGD("%s: Indication msg_id 0x%x", __FUNCTION__, rsp_ctx->msg_id);

            LOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
            TAILQ_FOREACH(item, &dev->indication_tailq_head, entries) {
                if (item->service_id == qrtr_srvice_item->service_id &&
                        item->sock == qrtr_srvice_item->sock) {
                    indication_item_t *ind = NULL;

                    ind = calloc(1, sizeof(*ind));
                    if (!ind) {
                        RLOGE("%s: Out of memory", __FUNCTION__);
                        ret = ENOMEM;
                        UNLOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
                        goto end;
                    }

                    ind->msg_id = rsp_ctx->msg_id;
                    ind->buf_len = payload_len;
                    ind->buf = calloc(payload_len, sizeof(uint8_t));
                    if (!ind->buf) {
                        RLOGE("%s: Out of memory", __FUNCTION__);
                        free(ind);
                        ret = ENOMEM;
                        UNLOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
                        goto end;
                    }
                    memcpy(ind->buf, payload, payload_len);

                    /* Callback should free both response buffer and write_queue_item_t */
                    item->cb(ind);
                }
            }
            UNLOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
            break;
        }

        /* Unknow message type. neither response nor indication. */
        /* skip it. This shouldn't happen */
        default:
            RLOGD("%s: QMI message type : %d", __FUNCTION__, rsp_ctx->type);
            ret = EPROTO;
            break;
    }

end:
    return ret;
}

/* Right now the item should be removed by user (cb or sync function) */
static int decode_response(device_list_t *dev,
        unpack_qmi_t *rsp_ctx,
        uint8_t svc,
        uint8_t cid,
        uint8_t *payload,
        uint16_t payload_len)
{
    int ret = 0;

    switch (rsp_ctx->type) {
        /* received QMI response message */
        case QMI_MSG_RESPONSE: {
            write_queue_item_t *item = NULL;

            RLOGD("%s: Response msg_id 0x%x", __FUNCTION__, rsp_ctx->msg_id);

            /* protect write queue, buffer and write itself */
            LOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);

            /* search queue for message with related transaction id */
            TAILQ_FOREACH(item, &dev->write_tailq_head, entries) {
                if (item->ctx_client) {
                    if (item->ctx_client->service_id == svc &&
                            item->ctx_client->client_id == cid &&
                            item->xid == rsp_ctx->xid) {

                        /* Found */
                        item->buf = calloc(payload_len, sizeof(uint8_t));
                        if (!item->buf) {
                            RLOGE("%s: Out of memory", __FUNCTION__);
                            ret = ENOMEM;
                            UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
                            goto end;
                        }

                        item->buf_len = payload_len;
                        memcpy(item->buf, payload, item->buf_len);

                        /* Signal semaphore? */
                        if (item->sync_mutex) {
                            sem_post(item->sync_mutex);
                            UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
                            goto end;
                        }

                        /* Callback? */
                        if (item->cb) {
                            response_item_t *response;

                            /* Since it is async, remove item from write queue */
                            TAILQ_REMOVE(&dev->write_tailq_head, item, entries);

                            /* Create a response_item */
                            response = get_response_item(item);
                            if (!response) {
                                ret = ENOMEM;
                            } else {
                                item->cb(response);
                            }
                            free(item);
                            UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
                        }

                        goto end;
                    }
                }
            }

            /* Not found, simply ignore the response */
            if (!item) {
                UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
            }
            break;
        }

        /* received QMI indication message */
        case QMI_MSG_INDICATION: {
            indication_callback_item_t *item;

            RLOGD("%s: Indication msg_id 0x%x", __FUNCTION__, rsp_ctx->msg_id);

            LOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
            TAILQ_FOREACH(item, &dev->indication_tailq_head, entries) {
                if (item->service_id == svc &&
                        item->client_id == cid) {
                    indication_item_t *ind = NULL;

                    ind = calloc(1, sizeof(*ind));
                    if (!ind) {
                        RLOGE("%s: Out of memory", __FUNCTION__);
                        ret = ENOMEM;
                        UNLOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
                        goto end;
                    }

                    ind->msg_id = rsp_ctx->msg_id;
                    ind->buf_len = payload_len;
                    ind->buf = calloc(payload_len, sizeof(uint8_t));
                    if (!ind->buf) {
                        RLOGE("%s: Out of memory", __FUNCTION__);
                        free(ind);
                        ret = ENOMEM;
                        UNLOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
                        goto end;
                    }
                    memcpy(ind->buf, payload, payload_len);

                    /* Callback should free both response buffer and write_queue_item_t */
                    item->cb(ind);
                }
            }
            UNLOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
            break;
        }

        /* Unknow message type. neither response nor indication. */
        /* skip it. This shouldn't happen */
        default:
            RLOGD("%s: QMI message type : %d", __FUNCTION__, rsp_ctx->type);
            ret = EPROTO;
            break;
    }

end:
    return ret;
}

static void *read_thread_error(device_list_t *dev)
{
    /* Suppose that error cannot be recovered: invoke clients' callbacks
     * and terminate thread
     */
    error_callback_item_t *item;

    LOCK_MUTEX_NOT_RECOVERABLE(dev->error_mutex);
    TAILQ_FOREACH(item, &dev->error_tailq_head, entries) {
        item->cb(NULL);
    }
    UNLOCK_MUTEX_NOT_RECOVERABLE(dev->error_mutex);

    return NULL;
}

void *read_thread_start(void *arg)
{
    int ret;
    struct pollfd p_fd[2];
    unpack_qmi_t rsp_ctx;
    device_list_t *dev = (device_list_t *)arg;

    RLOGD("%s: Read thread started", __FUNCTION__);

    /* Management device */
    p_fd[0].fd = dev->management_fd;
    p_fd[0].events = POLLIN;
    /* rmnet device */
    p_fd[1].fd = dev->dev_fd;
    p_fd[1].events = POLLIN;

    /* Read indefinitely */
    while (1) {
        /* Infinite timeout */
        ret = poll(p_fd, 2, -1);
        if (ret == -1) {
            RLOGE("%s: Failed poll: %d, %s", __FUNCTION__, errno, strerror(errno));

            return read_thread_error(dev);
        }

        if (p_fd[0].revents & POLLIN) {
            /* No need to read, since thread is going to die */
            RLOGD("%s: Signaled to close thread", __FUNCTION__);
            break;
        }

        if (p_fd[1].revents & POLLIN) {
            uint8_t svc;
            uint8_t cid;

            ret = (int)read(p_fd[1].fd, dev->resp_buffer, QMI_MSG_MAX);
            if (ret > 0) {
                uint8_t *payload;
                uint16_t payload_len;

                dev->resp_len = (uint16_t)ret;
                RLOGD("%s: ret = %d, resp_len = %u", __FUNCTION__, ret, dev->resp_len);

                ret = telit_helper_qmux_get_svc_cli_id(dev->resp_buffer, dev->resp_len, &svc, &cid);

                if (ret != QMI_SDK_ERR_NONE) {
                    RLOGE("%s: failed to get service and client id", __FUNCTION__);
                    continue;
                }

                /* TODO: manage partial or more than a complete message reading */
                ret = skip_qmux_header(dev->resp_buffer, dev->resp_len, &payload, &payload_len);

                if (ret != QMI_SDK_ERR_NONE) {
                    RLOGE("%s: failed to skip qmux header", __FUNCTION__);
                    continue;
                }

                RLOGD("%s: payload_len = %u, payload = %p, dev->resp_buffer = %p",
                        __FUNCTION__, payload_len, payload, dev->resp_buffer);

                telit_helper_get_resp_ctx((uint8_t)svc, payload, payload_len, &rsp_ctx);

                ret = decode_response(dev, &rsp_ctx, svc, cid, payload, payload_len);

                if (ret != 0) {
                    /* TODO: invoke callback to signal users an error in decoding ?
                     * Maybe only after a few retries?
                     */
                    RLOGE("%s: error in decoding response", __FUNCTION__);
                    continue;
                }
            } else {
                RLOGE("%s: Poll successful, but 0 bytes read", __FUNCTION__);
            }
        } else {
            write_queue_item_t *item = NULL;

            RLOGE("%s: One of POLLHUP, POLLERR or POLLNVAL happened", __FUNCTION__);
            /* It does not make sense to wait for timeouts if there are pending requests
             * so, unblock everything. The related unpack functions should fail in an
             * ordered way */
            LOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
            TAILQ_FOREACH(item, &dev->write_tailq_head, entries) {
                if (item->sync_mutex)
                    sem_post(item->sync_mutex);
            }
            UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);

            return read_thread_error(dev);
        }
    }

    return NULL;
}

void *read_qrtr_thread_start(void *arg)
{
    int ret;
    unpack_qmi_t rsp_ctx;
    device_list_t *dev = (device_list_t *)arg;
    int i;

    RLOGD("%s: Read qrtr thread started", __FUNCTION__);

    /* Management device */
    dev->p_fd[0].fd = dev->management_fd;
    dev->p_fd[0].events = POLLIN;

    /* Read indefinitely */
    while (1) {
        int elements = 0;
        int signaled_elements;
        /* Infinite timeout */
        signaled_elements = poll(dev->p_fd, TQCM_QRTR_MAX_SERVICES + 1, -1);
        if (signaled_elements == -1) {
            RLOGE("%s: Failed poll: %d, %s", __FUNCTION__, errno, strerror(errno));

            return read_thread_error(dev);
        }

        if (dev->p_fd[0].revents & POLLIN) {
            uint64_t ev;
            uint8_t reason;
            int ret_read;

            RLOGD("%s: Signaled management device", __FUNCTION__);
            LOCK_MUTEX_NOT_RECOVERABLE(dev->qrtr_svc_management_mutex);
            ret_read = read(dev->p_fd[0].fd, &ev, sizeof(uint64_t));
            reason = dev->management_reason;
            UNLOCK_MUTEX_NOT_RECOVERABLE(dev->qrtr_svc_management_mutex);
            if (ret_read != sizeof(ev)) {
                RLOGE("%s: read error on  event code %" PRIu64 ", terminating thread", __FUNCTION__, ev);
                return NULL;
            }

            if (reason == MANAGEMENT_FD_CLOSE) {
                RLOGD("%s: request to terminate thread", __FUNCTION__);
                return NULL;
            } else if (reason == MANAGEMENT_FD_ADD) {
                RLOGD("%s: pausing thread to add/remove fd", __FUNCTION__);
            } else {
                RLOGE("%s: unexpected management reason %d, terminating thread", __FUNCTION__,
                        dev->management_reason);
                return NULL;
            }

            /* Only management_fd signaled? */
            if (signaled_elements == 1) {
                continue;
            }
        }

        /* Check who triggered the poll */
        for (i = 1; i < TQCM_QRTR_MAX_SERVICES + 1; i++) {
            uint16_t rsp_len = QMI_MSG_MAX;

            if (elements == signaled_elements) {
                break;
            }

            if (dev->p_fd[i].fd == -1) {
                continue;
            }

            if (dev->p_fd[i].revents & POLLIN) {
                /* qrtr_services needs (i - 1) since the first element of the poll_fd array is the management fd */
                ret =
                        telit_qrtr_recvfrom(dev->qrtr_services[i - 1].sock, dev->qrtr_services[i - 1].node,
                        &(dev->qrtr_services[i - 1].port), dev->resp_buffer, &rsp_len);

                if (ret == 0) {
                    RLOGD("%s: Poll successful, but 0 bytes read", __FUNCTION__);
                } else if (ret > 0) {
                    dev->resp_len = rsp_len;
                    RLOGD("%s: resp_len = %u, resp = %p",
                            __FUNCTION__, dev->resp_len, dev->resp_buffer);

                    telit_helper_get_resp_ctx(dev->qrtr_services[i - 1].service_id, dev->resp_buffer,
                            dev->resp_len, &rsp_ctx);

                    ret = decode_qrtr_response(dev, &rsp_ctx, &(dev->qrtr_services[i - 1]), dev->resp_buffer,
                            dev->resp_len);

                    if (ret != 0) {
                        /* TODO: invoke callback to signal users an error in decoding ?
                         * Maybe only after a few retries?
                         */
                        RLOGE("%s: error in decoding response", __FUNCTION__);
                        continue;
                    }
                } else {
                    RLOGE("%s: telit_qrtr_recvfrom < 0, p_fd[%d], dev->p_fd[i].fd = %d", __FUNCTION__, i,
                            dev->p_fd[i].fd);
                }
                elements++;
            } else if (dev->p_fd[i].revents & POLLHUP ||
                    dev->p_fd[i].revents & POLLERR ||
                    dev->p_fd[i].revents & POLLNVAL) {
                RLOGE("%s: One of POLLHUP, POLLERR or POLLNVAL happened %d", __FUNCTION__,
                        dev->p_fd[i].revents);
                elements++;
                /* TODO: understand if this should be removed or not */
                return read_thread_error(dev);
            }
        }
    }

    return NULL;
}
