/**
 * @mainpage TQCM - Telit Qualcomm Connection Manager
 * TQCM provides an API to control Qualcomm-based modems through the QMI protocol.
 * It provides an API for sending commands/retrieving information using Telit QMI SDK (see \ref telitqmisdk) together with a qmi capable device.
 *
 * @section tqcmapiusage API Usage
 *
 * - Initialize TQCM context with #tqcm_init
 * - Initialize a client for the desired QMI service with #tqcm_init_client_context
 *  - For @b synchronous behavior call the desired tqcm_<service>_<request>_send with a NULL value for the #tqcm_callback_t
 *  - For @b asynchronous behavior call the desired tqcm_<service>_<request>_send with a valid #tqcm_callback_t. The response will be delivered when available to the callback as a response_item_t reference. Response should then be freed when no more needed.
 * - Once a client is no more needed, deinitalize the client context with #tqcm_deinit_client_context
 * - Once the connection manager is no more needed deinitialize the context with #tqcm_deinit
 *
 */

/**
 * @file tqcm.h
 * @brief TQCM Public API
 */

#ifndef TQCM_H_
#define TQCM_H_

#include <semaphore.h>
#include <telit_sdk.h>

/**
 * @brief indication and asynchronous response callback type
 *
 * @attention it is not allowed to send additional requests inside the callback
 */
typedef void (*tqcm_callback_t) (void *item);

/**
 * @brief error callback type
 */
typedef int (*tqcm_error_callback_t) (void *item);

/**
 * @brief indication type, to be parsed by Telit SDK indication handlers
 */
typedef struct indication_item {
    /** Message ID of the indication: first argument of Telit QMI SDK indication handler */
    uint16_t msg_id;
    /** QMI indication buffer: second argument of Telit QMI SDK indication handler */
    uint8_t *buf;
    /** QMI indication buffer length: third argument of Telit QMI SDK indication handler */
    uint16_t buf_len;
} indication_item_t;

/**
 * @brief response type for asynchronous requests, to be parsed by Telit SDK unpack functions
 */
typedef struct response_item {
    /** Identifier of the asynchronous request: it should be used for linking a response to the related request */
    uint64_t token;
    /** QMI response buffer: first argument of Telit QMI SDK unpack function */
    uint8_t *buf;
    /** QMI response buffer length: second argument of Telit QMI SDK unpack function */
    uint16_t buf_len;
} response_item_t;

/**
 * @brief TQCM context type
 */
typedef struct tqcm_context {
    /** Opaque field used internally by the library */
    void *dev;
    /** Number of opened QMI services */
    uint8_t opened_svc_count;
} tqcm_context_t;

/**
 * @brief TQCM service client context type
 */
typedef struct tqcm_client_context {
    /** QMI service identifier */
    uint8_t service_id;
    /** QMI service client identifier, applies only to USB based rmnet or mhi qmi UCI */
    uint8_t client_id;
    /** Transaction identifier counter for this client */
    uint16_t transaction_id;
    /** Callback invoked when an indication for this client is available */
    tqcm_callback_t indication_cb;
    /** TQCM context */
    tqcm_context_t *tqcm_ctx;
    /** Socket for QRTR service */
    int sock;
    /** Node for QRTR service */
    int node;
    /** Port for QRTR service */
    int port;
} tqcm_client_context_t;

/**
 * @brief Initialize TQCM context
 *
 * This function initializes an empty TQCM context for a specific rmnet device:
 * this is the first step for using the API.
 *
 * When using a QRTR device, device_name should be QRTR.
 *
 * @attention memory for the context should be allocated by the caller
 *
 * @param[in] device_name actual device name to be managed or QRTR
 * @param[out] tqcm_ctx #tqcm_context_t to be initialized
 * @return 0 for success, otherwise an error defined in <errno.h>
 */
int tqcm_init(char *device_name, tqcm_context_t *tqcm_ctx);

/**
 * @brief Deinitialize a TQCM context
 *
 * This function deinitializes a TQCM context: this is the last step when the device
 * should no more be used.
 *
 * Before calling this function make sure to have closed all the open QMI services' clients,
 * to avoid leaking services' instances on the modem side.
 *
 * @attention memory for the context should be released by the caller
 *
 * @param[in] tqcm_ctx #tqcm_context_t to be deinitialized
 * @return 0 for success, otherwise an error defined in <errno.h>
 */
int tqcm_deinit(tqcm_context_t *tqcm_ctx);

/**
 * @brief Set a callback to be invoked for abnormal errors in the connection manager
 *
 * This function sets a callback to be invoked for abnormal errors in the connection
 * manager (e.g. device no more available, unable to read/write from/to the device...).
 *
 * @attention once this callback is invoked TQCM is no more guaranteed to be working properly,
 * so a deinit is mandatory. If the malfunction depends on the modem, before reinitializing
 * the connection manager other actions could be needed.
 *
 * @param[in] tqcm_ctx an initialized #tqcm_context_t
 * @param[in] error_cb a #tqcm_error_callback_t to be invoked in case of errors
 * @return 0 for success, otherwise an error defined in <errno.h>
 */
int tqcm_set_error_cb(tqcm_context_t *tqcm_ctx, tqcm_error_callback_t error_cb);

/**
 * @brief Initialize a client context for a specific QMI service
 *
 * This function initializes a client context for a specific QMI service.
 * If the client should receive indications, a #tqcm_callback_t callback should be set (otherwise NULL).
 *
 * @attention memory for the client context should be allocated by the caller
 *
 * @param[in] svc_id identifier of the QMI service to be opened (see telit.sdk.h)
 * @param[in] tqcm_ctx an initialized #tqcm_context_t
 * @param[out] ctx_client #tqcm_client_context_t to be initialized
 * @param[in] indication_cb a #tqcm_callback_t to be invoked when an indication is available
 * @return 0 for success, otherwise an error defined in <errno.h>
 */
int tqcm_init_client_context(uint8_t svc_id,
        tqcm_context_t *tqcm_ctx,
        tqcm_client_context_t *ctx_client,
        tqcm_callback_t indication_cb);

/**
 * @brief Deinitialize a client context
 *
 * This function deinitializes a client context when it should no more be used.
 *
 * @attention memory for the client context should be released by the caller
 *
 * @param[in] tqcm_ctx an initialized #tqcm_context_t
 * @param[out] ctx_client #tqcm_client_context_t to be deinitialized
 * @return 0 for success, otherwise an error defined in <errno.h>
 */
int tqcm_deinit_client_context(tqcm_context_t *tqcm_ctx,
        tqcm_client_context_t *ctx_client);

/**
 * @brief Free an asynchronous response
 *
 * This function frees an asynchronous response.
 *
 * @param[in] resp asynchronous response to be freed
 */
void tqcm_free_response_item(response_item_t *resp);

/**
 * @brief Returns TQCM version
 *
 * This function returns TQCM version.
 *
 * @return TQCM version
 */
const char *tqcm_get_version();

#endif  /* TQCM_H_ */
