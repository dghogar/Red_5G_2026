#ifndef TELIT_SDK_ATR_H_
#define TELIT_SDK_ATR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_atr.h
 * @brief ATR pack/unpack functions
 * Service ID: QMI_SVC_ATR (237)
 */

/**
 * @brief Enum to describe QMI ATR Message IDs
 */
typedef enum {
    /** 0 - 0x0000 Send AT command */
    QMI_ATR_CMD_SEND = 0x0000,
    /** 1 - 0x0001 AT command response indication */
    QMI_ATR_CMD_IND  = 0x0001,
} QMI_ATR_message_t;

#define QMI_ATR_MAX_CMD_LEN 4096

/**
 * @brief input type for QMI_ATR_CMD_SEND request
 *
 */
typedef struct {
    /** Length of cmd string */
    uint16_t cmd_len;
    /** AT command to be sent, terminated with '\r' */
    char cmd[QMI_ATR_MAX_CMD_LEN + 1];
} atr_cmd_t;

/**
 * @brief Pack QMI_ATR_CMD_SEND (0 - 0x0000) request
 *
 * This function packs a qmi request for sending an AT command
 * The reply is provided through indication QMI_ATR_CMD_IND
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #atr_cmd_t
 */
QMI_SDK_error_t telit_atr_cmd_send_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        atr_cmd_t *input);

/**
 * @brief Unpack QMI_ATR_CMD_SEND (0 - 0x0000) request
 *
 * This function unpacks the result of a QMI_ATR_CMD_SEND request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_atr_cmd_send_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Unpack atr indications
 *
 * This function unpacks the result of the following atr indications
 *
 * QMI_ATR_CMD_IND (1)
 *
 * @param[in] msg_id indication id
 * @param[in] rsp indication
 * @param[in] len indication length
 * @param[out] out type according to the following
 *      QMI_ATR_CMD_IND -> #atr_cmd_t
 * @param[in] out_len buffer size
 */
QMI_SDK_error_t telit_atr_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len,
        uint8_t *out, uint16_t out_len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_ATR_H_ */
