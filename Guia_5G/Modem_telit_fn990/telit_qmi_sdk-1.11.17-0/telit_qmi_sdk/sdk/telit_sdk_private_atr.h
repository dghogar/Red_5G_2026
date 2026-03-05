#ifndef TELIT_SDK_PRIVATE_ATR_H_
#define TELIT_SDK_PRIVATE_ATR_H_

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_ATR_CMD_SEND (0 - 0x0000)
 */

/* Structure to describe generic at cmd */
typedef struct {
    uint16_t cmd_len;
    /* char cmd[cmd_len]; */
} QMI_ATR_generic_cmd_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_ATR_H_ */
