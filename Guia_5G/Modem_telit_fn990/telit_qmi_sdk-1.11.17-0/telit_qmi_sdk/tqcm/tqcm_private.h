#ifndef TQCM_PRIVATE_H_
#define TQCM_PRIVATE_H_

#include <inttypes.h>
#include <sys/queue.h>
#include <pthread.h>
#include <assert.h>
#include <poll.h>
#include "tqcm.h"

#define MAX_DEV_N 8

#define QMI_REQ_SMALL 256
#define QMI_REQ_MEDIUM 1024
#define TQCM_MAX_STRING_SIZE 256
#define TQCM_QRTR_MAX_SERVICES 128

#define MANAGEMENT_FD_CLOSE 0x1
#define MANAGEMENT_FD_ADD   0x2

/* Failing to lock a mutex is an unrecoverable error */
#define LOCK_MUTEX_NOT_RECOVERABLE(MUTEX) { \
            assert(pthread_mutex_lock(&(MUTEX)) == 0); \
}

/* Failing to unlock a mutex is an unrecoverable error */
#define UNLOCK_MUTEX_NOT_RECOVERABLE(MUTEX) { \
            assert(pthread_mutex_unlock(&(MUTEX)) == 0); \
}

typedef struct error_callback_item {
    tqcm_error_callback_t cb;
    TAILQ_ENTRY(error_callback_item) entries;  /* Needed by TAILQ */
} error_callback_item_t;

typedef struct indication_callback_item {
    tqcm_callback_t cb;
    uint8_t service_id;
    uint8_t client_id;
    int sock;
    TAILQ_ENTRY(indication_callback_item) entries;  /* Needed by TAILQ */
} indication_callback_item_t;

typedef struct write_queue_item {
    tqcm_client_context_t *ctx_client;     /* was tqcm_client_context_t */
    void *buf;                             /* out: qmi response buffer */
    uint16_t buf_len;                      /* out: qmi response buffer length */
    tqcm_callback_t cb;                    /* in: callback */
    uint64_t token;                        /* in: callback argument */
    sem_t *sync_mutex;                     /* in: mutex for sync behavior */
    uint16_t xid;                          /* in: transaction id for this request */
    TAILQ_ENTRY(write_queue_item) entries; /* Needed by TAILQ */
} write_queue_item_t;

typedef struct qrtr_service_item {
    int sock;
    int node;
    int port;
    uint8_t service_id;
} qrtr_service_item_t;

typedef struct device_list {
    char dev[TQCM_MAX_STRING_SIZE + 1];
    int dev_fd;
    struct pollfd p_fd[TQCM_QRTR_MAX_SERVICES + 1];
    qrtr_service_item_t qrtr_services[TQCM_QRTR_MAX_SERVICES];
    pthread_mutex_t qrtr_svc_management_mutex;  /* mutex for managing qrtr services */
    int management_fd;
    uint8_t management_reason;
    uint8_t users;
    pthread_t thread;
    void *ctl_ctx;                    /* was tqcm_service_context_t */
    uint8_t *resp_buffer;
    uint16_t resp_len;
    pthread_mutex_t write_mutex;      /* mutex for protecting write buffer */
    uint8_t *write_buffer;
    TAILQ_HEAD( , write_queue_item) write_tailq_head;
    pthread_mutex_t indication_mutex; /* mutex for protecting indication cb queue */
    TAILQ_HEAD( , indication_callback_item) indication_tailq_head;
    pthread_mutex_t error_mutex;      /* mutex for protecting error cb queue */
    TAILQ_HEAD( , error_callback_item) error_tailq_head;
} device_list_t;

void *read_thread_start(void *arg);
void *read_qrtr_thread_start(void *arg);

int tqcm_generic_send(tqcm_client_context_t *ctx,
        pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t req_len,
        tqcm_callback_t cb,
        uint64_t token,
        void **buf,
        uint16_t *buf_len);

int tqcm_generic_request(tqcm_client_context_t *ctx,
        void *input,
        void *out,
        uint16_t *sdk_error,
        tqcm_callback_t cb,
        uint64_t token,
        uint8_t svc_id,
        uint16_t msg_id);

uint16_t tqcm_update_transaction_id(tqcm_client_context_t *ctx);

#endif  /* TQCM_PRIVATE_H_ */
