#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/eventfd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>
#include <telit_sdk_wds.h>
#include <telit_sdk_dms.h>
#include <telit_sdk_nas.h>
#include <telit_sdk_uim.h>
#include <telit_sdk_voice.h>
#include <telit_sdk_wms.h>
#include <telit_sdk_voice.h>
#include <telit_sdk_loc.h>
#include <telit_sdk_cat.h>
#include <telit_sdk_qrtr.h>
#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_ctl.h"
#include "tqcm_wds.h"
#include "tqcm_dms.h"
#include "tqcm_nas.h"
#include "tqcm_wms.h"
#include "tqcm_voice.h"
#include "tqcm_uim.h"
#include "tqcm_loc.h"
#include "tqcm_wda.h"
#include "tqcm_dpm.h"
#include "tqcm_fota.h"
#include "tqcm_gms.h"
#include "tqcm_gas.h"
#include "tqcm_atr.h"
#include "tqcm_cat.h"

static device_list_t dev_list[MAX_DEV_N] = {};

static int is_device_qrtr(char *dev_name)
{
    if (strcmp(dev_name, "QRTR") == 0) {
        return 1;
    }

    return 0;
}

/* ATTENTION: should be called with write mutex locked */
static int tqcm_dev_write(device_list_t *dev, uint16_t write_len)
{
    int ret = 0;
    ssize_t written;
    ssize_t total_written = 0;

    while (total_written < write_len) {
        written = write(dev->dev_fd,
                dev->write_buffer + total_written,
                write_len - total_written);
        if (written < 0) {
            ret = errno;
            RLOGE("%s: write failed %d, %s", __FUNCTION__, ret, strerror(ret));
            goto end;
        }
        total_written += written;
    }

end:
    return ret;
}

int tqcm_generic_send(tqcm_client_context_t *ctx,
        pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t req_len,
        tqcm_callback_t cb,
        uint64_t token,
        void **buf,
        uint16_t *buf_len)
{
    int ret = 0;
    write_queue_item_t *item = NULL;
    sem_t mutex;
    uint16_t write_len = QMI_MSG_MAX;
    device_list_t *dev;

    if (!ctx || !ctx->tqcm_ctx || !ctx->tqcm_ctx->dev ||
            ctx->service_id != req_ctx->svc_id) {
        RLOGE("%s: Invalid service context", __FUNCTION__);
        return EINVAL;
    }

    dev = (device_list_t *)ctx->tqcm_ctx->dev;

    item = calloc(1, sizeof(*item));
    if (!item) {
        RLOGE("Not enough memory");
        return ENOMEM;
    }

    item->ctx_client = ctx;
    item->cb = cb;
    item->token = token;
    item->xid = req_ctx->xid;
    if (cb == NULL) {
        /* Synchronous, initializing semaphore */
        if (sem_init(&mutex, 0, 0)) {
            ret = errno;
            RLOGE("%s: Unable to init semaphore %d: %s", __FUNCTION__, ret, strerror(errno));
            free(item);
            return ret;
        }
        item->sync_mutex = &mutex;
    }

    /* protect write queue, buffer and write itself */
    LOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);

    memset(dev->write_buffer, 0, write_len);
    if (!is_device_qrtr(dev->dev)) {
        ret = add_qmux_header(req_ctx,
                ctx->client_id,
                req, req_len,
                dev->write_buffer, &write_len);
        if (ret != QMI_SDK_ERR_NONE) {
            RLOGE("%s: add qmux header fail %d", __FUNCTION__, ret);
            UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
            goto dispose_item;
        }
    }

    /* put item in write queue */
    TAILQ_INSERT_TAIL(&dev->write_tailq_head, item, entries);

    if (is_device_qrtr(dev->dev)) {
        ret = telit_qrtr_sendto(ctx->sock, ctx->node, ctx->port, req, req_len);
        UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
        if (ret < 0) {
            goto remove_item;
        }
        /* Changing to 0 for async requests */
        ret = 0;
    } else {
        ret = tqcm_dev_write(dev, write_len);
        UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
        if (ret) {
            goto remove_item;
        }
    }

    /* Is sync call? */
    if (item->sync_mutex) {
        struct timespec ts;

        ret = clock_gettime(CLOCK_REALTIME, &ts);
        if (ret == -1) {
            ret = errno;
            RLOGE("%s: clock_gettime failure %d: %s", __FUNCTION__, ret, strerror(ret));
            goto remove_item;
        }

        ts.tv_sec += req_ctx->timeout;

        ret = sem_timedwait(item->sync_mutex, &ts);
        if (ret == -1) {
            ret = errno;

            if (ret == ETIMEDOUT) {
                RLOGE("%s: semaphore timed out", __FUNCTION__);
            } else {
                RLOGE("%s: sem_timedwait error %d: %s", __FUNCTION__, ret, strerror(ret));
            }

            goto remove_item;
        }

        /* write_queue_item_t buf and buf_len should be populated by read thread */
        /* read thread should return the buffer with all the headers stripped */
        *buf = item->buf;
        *buf_len = item->buf_len;

        /* now caller should take care of the result, so we can only
         * remove and free the queue item, but cannot free the content */
        LOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
        TAILQ_REMOVE(&dev->write_tailq_head, item, entries);
        UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
    }

remove_item:
    if (ret) {
        LOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
        TAILQ_REMOVE(&dev->write_tailq_head, item, entries);
        UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
    }

dispose_item:
    /* NOTE: if async, dispatcher remove and free item before calling cb */
    if (!cb) {
        LOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
        sem_destroy(item->sync_mutex);
        UNLOCK_MUTEX_NOT_RECOVERABLE(dev->write_mutex);
        free(item);
    } else {
        /* If async return operation in progress */
        if (!ret) {
            ret = EINPROGRESS;
        } else {
            free(item);
        }
    }

    return ret;
}

static int is_device_in_list(char *dev)
{
    uint8_t i;

    for (i = 0; i < MAX_DEV_N; i++) {
        if (strcmp(dev, dev_list[i].dev) == 0) {
            return i;
        }
    }

    return -1;
}

static int find_empty_slot()
{
    uint8_t i;

    for (i = 0; i < MAX_DEV_N; i++) {
        if (dev_list[i].dev_fd == 0) {
            return i;
        }
    }

    return -1;
}

static void tqcm_ctl_get_version_info_print(ctl_get_version_info_t *out)
{
    uint8_t i;

    for (i = 0; i < out->service_version_list_size; i++) {
        RLOGD("%s:, qmi_svc_type: %u, major_ver: %u, minor_ver: %u", __FUNCTION__,
                out->service_version_list[i].qmi_svc_type,
                out->service_version_list[i].major_ver,
                out->service_version_list[i].minor_ver);
    }
}

static void release_dev_list_item(device_list_t *dev_item)
{
    write_queue_item_t *item_write;
    indication_callback_item_t *item_ind;
    error_callback_item_t *item_err;
    uint64_t u = 1;
    ssize_t ret;

    RLOGD("%s: closing device %s", __FUNCTION__, dev_item->dev);

    LOCK_MUTEX_NOT_RECOVERABLE(dev_item->qrtr_svc_management_mutex);
    dev_item->management_reason = MANAGEMENT_FD_CLOSE;
    ret = write(dev_item->management_fd, &u, sizeof(u));
    if (ret < 0) {
        ret = errno;
        RLOGE("%s: write failed %zd, %s", __FUNCTION__, ret, strerror(ret));
    }
    UNLOCK_MUTEX_NOT_RECOVERABLE(dev_item->qrtr_svc_management_mutex);

    if (!is_device_qrtr(dev_item->dev)) {
        if (dev_item->dev_fd != 0) {
            close(dev_item->dev_fd);
            dev_item->dev_fd = 0;
        }
    }
    /* pthread_cancel is not defined in bionic, using the
     * management_fd to cancel the read thread */
    pthread_join(dev_item->thread, NULL);
    close(dev_item->management_fd);
    free(dev_item->ctl_ctx);
    free(dev_item->write_buffer);
    free(dev_item->resp_buffer);

    /* Clean write queue and release resources */
    LOCK_MUTEX_NOT_RECOVERABLE(dev_item->write_mutex);
    if (!TAILQ_EMPTY(&dev_item->write_tailq_head)) {
        TAILQ_FOREACH(item_write, &dev_item->write_tailq_head, entries) {
            /* If there are pending items in the write queue just unblock
             * the mutex to hope for an ordered unwinding */
            if (item_write->sync_mutex) {
                sem_post(item_write->sync_mutex);
            }
        }
    }
    UNLOCK_MUTEX_NOT_RECOVERABLE(dev_item->write_mutex);

    /* Clean indication queue and release resources */
    LOCK_MUTEX_NOT_RECOVERABLE(dev_item->indication_mutex);
    if (!TAILQ_EMPTY(&dev_item->indication_tailq_head)) {
        while ((item_ind = TAILQ_FIRST(&dev_item->indication_tailq_head))) {
            TAILQ_REMOVE(&dev_item->indication_tailq_head, item_ind, entries);
            free(item_ind);
        }
    }
    UNLOCK_MUTEX_NOT_RECOVERABLE(dev_item->indication_mutex);

    /* Clean error queue and release resources */
    LOCK_MUTEX_NOT_RECOVERABLE(dev_item->error_mutex);
    if (!TAILQ_EMPTY(&dev_item->error_tailq_head)) {
        while ((item_err = TAILQ_FIRST(&dev_item->error_tailq_head))) {
            TAILQ_REMOVE(&dev_item->error_tailq_head, item_err, entries);
            free(item_err);
        }
    }
    UNLOCK_MUTEX_NOT_RECOVERABLE(dev_item->error_mutex);

    pthread_mutex_destroy(&dev_item->indication_mutex);
    pthread_mutex_destroy(&dev_item->error_mutex);
    pthread_mutex_destroy(&dev_item->qrtr_svc_management_mutex);
    /* Before destroying the write_mutex, make sure that there are no more pending requests */
clear_pending_writes:
    LOCK_MUTEX_NOT_RECOVERABLE(dev_item->write_mutex);
    while (!TAILQ_EMPTY(&dev_item->write_tailq_head)) {
        UNLOCK_MUTEX_NOT_RECOVERABLE(dev_item->write_mutex);
        sleep(1);
        goto clear_pending_writes;
    }
    UNLOCK_MUTEX_NOT_RECOVERABLE(dev_item->write_mutex);
    pthread_mutex_destroy(&dev_item->write_mutex);

    memset(dev_item, 0, sizeof(*dev_item));
}

static int probe_modem(tqcm_context_t *tqcm_ctx)
{
    int ret = 0;
    uint8_t i;
    ctl_get_version_info_t out;
    device_list_t *dev;

    dev = (device_list_t *)tqcm_ctx->dev;

    if (!is_device_qrtr(dev->dev)) {
        for (i = 0; i < 10; i++) {
            RLOGD("%s: ctl get version info attempt %u", __FUNCTION__, i + 1);
            ret = tqcm_ctl_get_version_info_send(dev->ctl_ctx, &out,
                    NULL, 0);
            if (ret == 0) {
                tqcm_ctl_get_version_info_print(&out);
                break;
            }

            if (ret == ETIMEDOUT) {
                if (i == 9) {
                    RLOGE("%s: Modem unresponsive at probing", __FUNCTION__);
                    ret = ENOTRECOVERABLE;
                }
            } else {
                RLOGE("%s: Error when probing modem", __FUNCTION__);
                ret = ENOTRECOVERABLE;
                break;
            }
        }
    }

    return ret;
}

/* Create context and track device usage in internal struct */
int tqcm_init(char *device_name, tqcm_context_t *tqcm_ctx)
{
    int ret = 0;
    int slot;

    if (!device_name || !tqcm_ctx) {
        RLOGE("%s: Invalid argument", __FUNCTION__);
        return EINVAL;
    }

    if (is_device_qrtr(device_name) && !telit_is_qrtr_supported()) {
        RLOGE("QRTR is not supported in this kernel version");
        return EINVAL;
    }

    memset(tqcm_ctx, 0, sizeof(*tqcm_ctx));

    /* Check if device is already managed */
    if ((slot = is_device_in_list(device_name)) != -1) {
        tqcm_ctx->dev = &dev_list[slot];
    } else {
        device_list_t *dev;

        /* Not found, initializing */
        if ((slot = find_empty_slot()) == -1) {
            RLOGE("%s: Maximum number of devices reached", __FUNCTION__);
            ret = EPERM;
            goto end;
        }

        if (!is_device_qrtr(device_name)) {
            RLOGD("%s: opening device %s", __FUNCTION__, device_name);
            dev_list[slot].dev_fd = open(device_name, O_NONBLOCK | O_RDWR);
            if (dev_list[slot].dev_fd == -1) {
                ret = errno;
                RLOGE("%s: Failed opening device with error %d: %s",
                        __FUNCTION__, ret, strerror(ret));
                dev_list[slot].dev_fd = 0;
                goto end;
            }
        } else {
            /* TBD: add QMI sending for avoiding keeping opened the legacy MHI device */
            int i;

            /* Using a fake dev_fd */
            dev_list[slot].dev_fd = 1;
            memset(dev_list[slot].qrtr_services, 0, sizeof(dev_list[slot].qrtr_services));
            for (i = 0; i < TQCM_QRTR_MAX_SERVICES + 1; i++) {
                dev_list[slot].p_fd[i].fd = -1;
                dev_list[slot].p_fd[i].events = 0;
                dev_list[slot].p_fd[i].revents = 0;
            }
        }

        RLOGD("%s: creating management fd", __FUNCTION__);
        dev_list[slot].management_fd = eventfd(0, EFD_NONBLOCK);
        if (dev_list[slot].management_fd == -1) {
            ret = errno;
            RLOGE("%s: Failed creating management fd %d: %s",
                    __FUNCTION__, ret, strerror(ret));
            close(dev_list[slot].dev_fd);
            dev_list[slot].dev_fd = 0;
            goto end;
        }

        dev_list[slot].ctl_ctx = calloc(1, sizeof(tqcm_client_context_t));
        if (!dev_list[slot].ctl_ctx) {
            RLOGE("%s: Unable to allocate ctl_ctx", __FUNCTION__);
            ret = ENOMEM;
            goto clean_dev_list;
        }

        dev_list[slot].write_buffer = calloc(QMI_MSG_MAX, sizeof(uint8_t));
        if (!dev_list[slot].write_buffer) {
            RLOGE("%s: Unable to allocate write_buffer", __FUNCTION__);
            ret = ENOMEM;
            goto clean_dev_list;
        }

        dev_list[slot].resp_buffer = calloc(QMI_MSG_MAX, sizeof(uint8_t));
        if (!dev_list[slot].resp_buffer) {
            RLOGE("%s: Unable to allocate resp_buffer", __FUNCTION__);
            ret = ENOMEM;
            goto clean_dev_list;
        }

        ret = pthread_mutex_init(&dev_list[slot].write_mutex, NULL);
        if (ret != 0) {
            RLOGE("%s: Unable to create write mutex %u", __FUNCTION__, ret);
            goto clean_dev_list;
        }

        ret = pthread_mutex_init(&dev_list[slot].indication_mutex, NULL);
        if (ret != 0) {
            RLOGE("%s: Unable to create indication mutex %u", __FUNCTION__, ret);
            goto clean_dev_list;
        }

        ret = pthread_mutex_init(&dev_list[slot].error_mutex, NULL);
        if (ret != 0) {
            RLOGE("%s: Unable to create error mutex %u", __FUNCTION__, ret);
            goto clean_dev_list;
        }

        ret = pthread_mutex_init(&dev_list[slot].qrtr_svc_management_mutex, NULL);
        if (ret != 0) {
            RLOGE("%s: Unable to create qrtr_svc_management_mutex mutex %u", __FUNCTION__, ret);
            goto clean_dev_list;
        }

        TAILQ_INIT(&dev_list[slot].write_tailq_head);
        TAILQ_INIT(&dev_list[slot].indication_tailq_head);
        TAILQ_INIT(&dev_list[slot].error_tailq_head);

        strlcpy(dev_list[slot].dev, device_name, sizeof(dev_list[0].dev));

        /* Starting read thread: passing device list item
         * At this moment dev_list[slot] should be completely usable */
        if (is_device_qrtr(dev_list[slot].dev)) {
            ret = pthread_create(&dev_list[slot].thread, NULL,
                    read_qrtr_thread_start, &dev_list[slot]);
        } else {
            ret = pthread_create(&dev_list[slot].thread, NULL,
                    read_thread_start, &dev_list[slot]);
        }
        if (ret != 0) {
            RLOGE("%s: Unable to create read thread %u", __FUNCTION__, ret);
            goto clean_dev_list;
        }

        dev = &dev_list[slot];
        tqcm_ctx->dev = dev;

        if (!is_device_qrtr(dev_list[slot].dev)) {
            /* Initialize ctl context */
            ret = tqcm_init_client_context(QMI_SVC_CONTROL,
                    tqcm_ctx,
                    dev->ctl_ctx,
                    NULL);
            if (ret) {
                RLOGE("%s: Unable to init ctl context", __FUNCTION__);
                ret = ENOTRECOVERABLE;
                goto clean_dev_list;
            }
        }

        /* Start probing modem with ctl get version info request */
        ret = probe_modem(tqcm_ctx);
        if (ret) {
            goto clean_dev_list;
        }
    }

    dev_list[slot].users++;

clean_dev_list:
    if (ret) {
        if (dev_list[slot].users == 0) {
            release_dev_list_item(&dev_list[slot]);
            memset(tqcm_ctx, 0, sizeof(*tqcm_ctx));
        }
    }

end:
    return ret;
}

/* Destroy context and track device usage in internal struct */
int tqcm_deinit(tqcm_context_t *tqcm_ctx)
{
    int ret = 0;
    int slot;
    device_list_t *dev;

    if (!tqcm_ctx) {
        RLOGE("%s: Invalid argument", __FUNCTION__);
        return EINVAL;
    }

    /* Check if device is among those managed */
    dev = (device_list_t *)tqcm_ctx->dev;
    if ((slot = is_device_in_list(dev->dev)) == -1) {
        RLOGE("%s: Managed device not found", __FUNCTION__);
        ret = ENODEV;
        goto end;
    }

    if (tqcm_ctx->opened_svc_count) {
        RLOGE("%s: %u services still opened: LEAK", __FUNCTION__, tqcm_ctx->opened_svc_count);
    }

    dev_list[slot].users--;
    /* Check if no more users */
    if (!dev_list[slot].users) {
        /* Remove from managed device list */
        release_dev_list_item(&dev_list[slot]);
    }

    memset(tqcm_ctx, 0, sizeof(*tqcm_ctx));

end:
    return ret;
}

static int find_empty_qrtr_slot(struct pollfd *p_fd, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        if (p_fd[i].fd == -1) {
            return i;
        }
    }

    return -1;
}

int tqcm_init_client_context(uint8_t svc_id,
        tqcm_context_t *tqcm_ctx,
        tqcm_client_context_t *ctx_client,
        tqcm_callback_t indication_cb)
{
    int ret = 0;
    device_list_t *dev;

    if (!tqcm_ctx || !ctx_client) {
        RLOGE("%s: Invalid argument", __FUNCTION__);
        return EINVAL;
    }

    dev = (device_list_t *)tqcm_ctx->dev;

    memset(ctx_client, 0, sizeof(*ctx_client));
    ctx_client->tqcm_ctx = tqcm_ctx;
    ctx_client->service_id = svc_id;
    ctx_client->indication_cb = indication_cb;

    if (svc_id != QMI_SVC_CONTROL) {
        if (is_device_qrtr(dev->dev)) {
            int sock, node, port;
            uint64_t u = 1;
            int i = find_empty_qrtr_slot(&(dev->p_fd[1]), TQCM_QRTR_MAX_SERVICES);
            if (i < 0) {
                RLOGE("%s: Unable to find any qrtr empty slot", __FUNCTION__);
                ctx_client->service_id = 0;
                return ENOMEM;
            } else {
                /* Increasing since we are now considering also the management device */
                i++;
            }

            /* Found, lock the service management mutex, trigger the read thread and add socket to poll array */
            LOCK_MUTEX_NOT_RECOVERABLE(dev->qrtr_svc_management_mutex);
            dev->management_reason = MANAGEMENT_FD_ADD;
            ret = write(dev->management_fd, &u, sizeof(u));
            if (ret < 0) {
                UNLOCK_MUTEX_NOT_RECOVERABLE(dev->qrtr_svc_management_mutex);
                ret = errno;
                RLOGE("%s: write failed %d, %s", __FUNCTION__, ret, strerror(ret));
                ctx_client->service_id = 0;
                return ret;
            }

            ret = telit_qrtr_get_client_for_svc(svc_id, &sock, &node, &port);
            if (ret < 0) {
                RLOGE("%s: Unable to open service %u through qrtr", __FUNCTION__, svc_id);
                ctx_client->service_id = 0;
            } else {
                /* This client has a callback for indications? */
                if (indication_cb) {
                    indication_callback_item_t *item = NULL;
                    item = calloc(1, sizeof(*item));
                    if (!item) {
                        RLOGE("%s: Not enough memory", __FUNCTION__);
                        ctx_client->service_id = 0;
                        return ENOMEM;
                    }
                    /* Add indication callback to managed device indication cb lists*/
                    item->client_id = 0;
                    item->service_id = svc_id;
                    item->cb = indication_cb;
                    item->sock = sock;

                    LOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
                    TAILQ_INSERT_TAIL(&dev->indication_tailq_head, item, entries);
                    UNLOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
                }

                tqcm_ctx->opened_svc_count++;
                ctx_client->client_id = 0;
                ctx_client->sock = sock;
                ctx_client->node = node;
                ctx_client->port = port;
                dev->p_fd[i].fd = sock;
                dev->p_fd[i].events = POLLIN;
                dev->p_fd[i].revents = 0;
                /* qrtr_services array starts from 0, since there's no management fd*/
                dev->qrtr_services[i - 1].sock = sock;
                dev->qrtr_services[i - 1].node = node;
                dev->qrtr_services[i - 1].port = port;
                dev->qrtr_services[i - 1].service_id = svc_id;
            }
            UNLOCK_MUTEX_NOT_RECOVERABLE(dev->qrtr_svc_management_mutex);
        } else {
            ctl_get_client_id_t input;
            ctl_get_client_id_info_t out;

            input.qmi_svc_type = svc_id;
            ret = tqcm_ctl_get_client_id_send(dev->ctl_ctx,
                    &input,
                    &out,
                    NULL,
                    0);
            if (ret == 0) {
                /* This client has a callback for indications? */
                if (indication_cb) {
                    indication_callback_item_t *item = NULL;
                    item = calloc(1, sizeof(*item));
                    if (!item) {
                        RLOGE("%s: Not enough memory", __FUNCTION__);
                        ctx_client->service_id = 0;
                        return ENOMEM;
                    }
                    /* Add indication callback to managed device indication cb lists*/
                    item->client_id = out.client_id;
                    item->service_id = svc_id;
                    item->cb = indication_cb;

                    LOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
                    TAILQ_INSERT_TAIL(&dev->indication_tailq_head, item, entries);
                    UNLOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
                }

                tqcm_ctx->opened_svc_count++;
                ctx_client->client_id = out.client_id;
            } else {
                RLOGE("%s: Unable to open service %u through ctl", __FUNCTION__, svc_id);
                ctx_client->service_id = 0;
            }
        }
    }

    return ret;
}

static int find_qrtr_service_index(qrtr_service_item_t *item, int len, int sock, uint8_t service_id)
{
    int i;

    for (i = 0; i < len; i++) {
        if ((item[i].sock == sock) && (item[i].service_id == service_id)) {
            return i;
        }
    }

    return -1;
}

int tqcm_deinit_client_context(tqcm_context_t *tqcm_ctx,
        tqcm_client_context_t *ctx_client)
{
    int ret = 0;
    device_list_t *dev;
    tqcm_client_context_t empty_context = {0};

    if (!tqcm_ctx || !ctx_client) {
        RLOGE("%s: Invalid argument", __FUNCTION__);
        return EINVAL;
    }

    if (memcmp(ctx_client, &empty_context, sizeof(empty_context)) == 0) {
        RLOGD("%s: Context not initialized", __FUNCTION__);
        return ret;
    }

    dev = (device_list_t *)tqcm_ctx->dev;

    if (ctx_client->service_id != QMI_SVC_CONTROL) {
        if (is_device_qrtr(dev->dev)) {
            uint64_t u = 1;
            int i;
            int ret_write;

            LOCK_MUTEX_NOT_RECOVERABLE(dev->qrtr_svc_management_mutex);
            dev->management_reason = MANAGEMENT_FD_ADD;
            ret_write = write(dev->management_fd, &u, sizeof(u));
            if (ret_write < 0) {
                UNLOCK_MUTEX_NOT_RECOVERABLE(dev->qrtr_svc_management_mutex);
                ret = errno;
                RLOGE("%s: write failed %d, %s", __FUNCTION__, ret, strerror(ret));
                return ret;
            }

            i = find_qrtr_service_index(dev->qrtr_services, TQCM_QRTR_MAX_SERVICES, ctx_client->sock,
                    ctx_client->service_id);
            if (i < 0) {
                UNLOCK_MUTEX_NOT_RECOVERABLE(dev->qrtr_svc_management_mutex);
                RLOGE("%s: Unable to find the qrtr item", __FUNCTION__);
                return EFAULT;
            }

            /* The first item in the poll array is the management fd */
            dev->p_fd[i + 1].fd = -1;
            dev->p_fd[i + 1].events = 0;
            dev->p_fd[i + 1].revents = 0;
            memset(&dev->qrtr_services[i], 0, sizeof(dev->qrtr_services[i]));
            close(ctx_client->sock);
            UNLOCK_MUTEX_NOT_RECOVERABLE(dev->qrtr_svc_management_mutex);
            tqcm_ctx->opened_svc_count--;
        } else {
            ctl_get_client_id_info_t input, out;

            input.qmi_svc_type = ctx_client->service_id;
            input.client_id = ctx_client->client_id;
            ret = tqcm_ctl_release_client_id_send(dev->ctl_ctx,
                    &input,
                    &out,
                    NULL,
                    0);

            if (ret == 0) {
                tqcm_ctx->opened_svc_count--;
            } else {
                RLOGE("%s: Unable to close client id", __FUNCTION__);
            }
        }

        /* Always remove indication callback */
        if (ctx_client->indication_cb) {
            indication_callback_item_t *item;

            LOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
            TAILQ_FOREACH(item, &dev->indication_tailq_head, entries) {
                if (item->cb == ctx_client->indication_cb) {
                    TAILQ_REMOVE(&dev->indication_tailq_head, item, entries);
                    free(item);
                    break;
                }
            }
            UNLOCK_MUTEX_NOT_RECOVERABLE(dev->indication_mutex);
        }
    }

    memset(ctx_client, 0, sizeof(*ctx_client));

    return ret;
}

static QMI_SDK_error_t tqcm_generic_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *req_len,
        void *input,
        uint8_t svc_id,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (svc_id) {
        case QMI_SVC_WDS:
            ret = tqcm_wds_pack(req_ctx, req, req_len, input, msg_id);
            break;
        case QMI_SVC_DMS:
            ret = tqcm_dms_pack(req_ctx, req, req_len, input, msg_id);
            break;
        case QMI_SVC_NAS:
            ret = tqcm_nas_pack(req_ctx, req, req_len, input, msg_id);
            break;
        case QMI_SVC_WMS:
            ret = tqcm_wms_pack(req_ctx, req, req_len, input, msg_id);
            break;
        case QMI_SVC_VOICE:
            ret = tqcm_voice_pack(req_ctx, req, req_len, input, msg_id);
            break;
        case QMI_SVC_CAT:
            ret = tqcm_cat_pack(req_ctx, req, req_len, input, msg_id);
            break;
        case QMI_SVC_UIM:
            ret = tqcm_uim_pack(req_ctx, req, req_len, input, msg_id);
            break;
        case QMI_SVC_LOC:
            ret = tqcm_loc_pack(req_ctx, req, req_len, input, msg_id);
            break;
        case QMI_SVC_WDA:
            ret = tqcm_wda_pack(req_ctx, req, req_len, input, msg_id);
            break;
        case QMI_SVC_DPM:
            ret = tqcm_dpm_pack(req_ctx, req, req_len, input, msg_id);
            break;
        case QMI_SVC_FOTA:
            ret = tqcm_fota_pack(req_ctx, req, req_len, input, msg_id);
            break;
        case QMI_SVC_GMS:
            ret = tqcm_gms_pack(req_ctx, req, req_len, input, msg_id);
            break;
        case QMI_SVC_GAS:
            ret = tqcm_gas_pack(req_ctx, req, req_len, input, msg_id);
            break;
        case QMI_SVC_ATR:
            ret = tqcm_atr_pack(req_ctx, req, req_len, input, msg_id);
            break;
        default:
            RLOGE("%s: Service %u not found", __FUNCTION__, svc_id);
    }

    return ret;
}

static QMI_SDK_error_t tqcm_generic_unpack(void *resp,
        uint16_t resp_len,
        void *out,
        uint8_t svc_id,
        uint16_t msg_id)
{
    QMI_SDK_error_t ret = QMI_SDK_ERR_INVALID_ARG;

    switch (svc_id) {
        case QMI_SVC_WDS:
            ret = tqcm_wds_unpack(resp, resp_len, out, msg_id);
            break;
        case QMI_SVC_DMS:
            ret = tqcm_dms_unpack(resp, resp_len, out, msg_id);
            break;
        case QMI_SVC_NAS:
            ret = tqcm_nas_unpack(resp, resp_len, out, msg_id);
            break;
        case QMI_SVC_WMS:
            ret = tqcm_wms_unpack(resp, resp_len, out, msg_id);
            break;
        case QMI_SVC_VOICE:
            ret = tqcm_voice_unpack(resp, resp_len, out, msg_id);
            break;
        case QMI_SVC_CAT:
            ret = tqcm_cat_unpack(resp, resp_len, out, msg_id);
            break;
        case QMI_SVC_UIM:
            ret = tqcm_uim_unpack(resp, resp_len, out, msg_id);
            break;
        case QMI_SVC_LOC:
            ret = tqcm_loc_unpack(resp, resp_len, out, msg_id);
            break;
        case QMI_SVC_WDA:
            ret = tqcm_wda_unpack(resp, resp_len, out, msg_id);
            break;
        case QMI_SVC_DPM:
            ret = tqcm_dpm_unpack(resp, resp_len, out, msg_id);
            break;
        case QMI_SVC_FOTA:
            ret = tqcm_fota_unpack(resp, resp_len, out, msg_id);
            break;
        case QMI_SVC_GMS:
            ret = tqcm_gms_unpack(resp, resp_len, out, msg_id);
            break;
        case QMI_SVC_GAS:
            ret = tqcm_gas_unpack(resp, resp_len, out, msg_id);
            break;
        case QMI_SVC_ATR:
            ret = tqcm_atr_unpack(resp, resp_len, out, msg_id);
            break;
        default:
            RLOGE("%s: Service %u not found", __FUNCTION__, svc_id);
    }

    return ret;
}

int tqcm_generic_request(tqcm_client_context_t *ctx,
        void *input,
        void *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token,
        uint8_t svc_id,
        uint16_t msg_id)
{
    int ret = 0;
    pack_qmi_t req_ctx = {0};
    uint8_t req[QMI_REQ_MEDIUM];
    uint16_t req_len = QMI_REQ_MEDIUM;
    void *resp = NULL;
    uint16_t resp_len;

    if (!ctx || ctx->service_id != svc_id) {
        RLOGE("%s: Invalid service context", __FUNCTION__);
        return EINVAL;
    }

    *sdk_error = 0;
    req_ctx.xid = tqcm_update_transaction_id(ctx);

    ret = tqcm_generic_pack(&req_ctx, req, &req_len, input, svc_id, msg_id);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("%s: tqcm_generic_pack %d failure svc %u, msg %u",
                __FUNCTION__, ret, svc_id, msg_id);
        goto end;
    }

    ret = tqcm_generic_send(ctx, &req_ctx,
            req, req_len,
            cb, token,
            &resp, &resp_len);
    if (!ret && !cb) {
        /* Synchronous call */
        ret = tqcm_generic_unpack(resp, resp_len, out, svc_id, msg_id);
        if (ret != QMI_SDK_ERR_NONE) {
            RLOGE("%s: tqcm_generic_unpack %d failure svc %u, msg %u",
                    __FUNCTION__, ret, svc_id, msg_id);
            if (ret > QMI_SDK_ERR_ERR_QMI_OFFSET) {
                *sdk_error = (ret - QMI_SDK_ERR_ERR_QMI_OFFSET);
                ret = EPROTO;
            } else {
                *sdk_error = ret;
                ret = EPERM;
            }
        }

        free(resp);
    }

    /* For async call nothing more is needed, EINPROGRESS is returned */
end:
    return ret;
}

int tqcm_set_error_cb(tqcm_context_t *tqcm_ctx, tqcm_error_callback_t error_cb)
{
    int ret = 0;
    error_callback_item_t *item;
    device_list_t *dev;

    if (!tqcm_ctx || !error_cb) {
        RLOGE("%s: Invalid argument", __FUNCTION__);
        return EINVAL;
    }

    dev = (device_list_t *)tqcm_ctx->dev;

    item = calloc(1, sizeof(*item));
    if (!item) {
        RLOGE("%s: Not enough memory", __FUNCTION__);
        return ENOMEM;
    }

    /* Add error callback to managed device error cb lists*/
    item->cb = error_cb;

    LOCK_MUTEX_NOT_RECOVERABLE(dev->error_mutex);
    TAILQ_INSERT_TAIL(&dev->error_tailq_head, item, entries);
    UNLOCK_MUTEX_NOT_RECOVERABLE(dev->error_mutex);

    return ret;
}

uint16_t tqcm_update_transaction_id(tqcm_client_context_t *ctx)
{
    if ((ctx->service_id == QMI_SVC_CONTROL && ctx->transaction_id == 0xFF) ||
            (ctx->service_id != QMI_SVC_CONTROL && ctx->transaction_id == 0xFFFF)) {
        ctx->transaction_id = 1;
    } else {
        ctx->transaction_id++;
    }

    return ctx->transaction_id;
}

void tqcm_free_response_item(response_item_t *resp)
{
    if (!resp) {
        return;
    }

    free(resp->buf);
    free(resp);
}

const char *tqcm_get_version()
{
    return telit_get_version();
}
