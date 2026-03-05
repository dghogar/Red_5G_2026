#ifndef TELIT_SDK_QRTR_H_
#define TELIT_SDK_QRTR_H_

/**
 * @brief Get QRTR socket, node and port ID for a given QMI service ID.
 * @param[in] svc service number.
 * @param[out] sock the QRTR socket.
 * @param[out] node the QRTR node ID of the service.
 * @param[out] port the QRTR port ID of the service.
 */
int telit_qrtr_get_client_for_svc(int svc, int *sock, int *node, int *port);

/**
 * @brief Send a QMI buffer to the QRTR QMI service.
 * @param[in] sock the QRTR socket.
 * @param[in] node the QRTR node ID of the service.
 * @param[in] port the QRTR port ID of the service.
 * @param[in] buf the QMI message buffer.
 * @param[in] len the QMI message buffer length.
 */
int telit_qrtr_sendto(int sock, int node, int port, void *buf, uint16_t len);

/**
 * @brief Read a QMI buffer from the QRTR socket.
 * @param[in] sock the QRTR socket.
 * @param[in] node the QRTR node ID of the service.
 * @param[out] port the QRTR port ID of the service which the message belong to.
 * @param[out] buf the message buffer.
 * @param[in/out] len in:the buffer size, out: the QMI message length.
 */
int telit_qrtr_recvfrom(int sock, int node, int *port, void *buf, uint16_t *len);

/**
 * @brief Check if QRTR is supported.
 * @return 1 if QRTR is supported, otherwise 0
 */
int telit_is_qrtr_supported();

#endif /* TELIT_SDK_QRTR_H_ */
