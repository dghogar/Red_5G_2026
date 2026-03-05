#ifndef TELIT_SDK_PRIVATE_CAT_H_
#define TELIT_SDK_PRIVATE_CAT_H_

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_CAT_SET_EVENT_REPORT (1 - 0x0001)
 */

/* Structure to describe request TLV 0x10 for QMI_CAT_SET_EVENT_REPORT */
typedef struct {
    uint32_t pc_evt_report_req_mask;
} QMI_CAT_generic_pc_evt_mask_t;

/* Structure to describe request TLV 0x13 for QMI_CAT_SET_EVENT_REPORT */
typedef struct {
    uint32_t pc_full_func_evt_report_req_mask;
} QMI_CAT_pc_full_func_evt_mask_t;

/*****************************************************************************
 * QMI_CAT_SEND_TR (33 - 0x0021)
 */

/* Structure to describe a generic terminal response */
typedef struct {
    uint8_t sw1;
    uint8_t sw2;
    uint8_t tr_response_sz;
    /* This array must be the size specified by tr_response_sz */
    /* uint8_t tr_response[tr_response_sz]; */
} QMI_CAT_generic_terminal_response_t;

/*****************************************************************************
 * QMI_CAT_SEND_ENVELOPE_CMD (34 - 0x0022)
 */

/* Structure to describe TLV 0x01 for QMI_CAT_SEND_ENVELOPE_CMD */
typedef struct {
    uint16_t env_cmd_type;
    uint16_t env_cmd_sz;
    /* This array must be the size specified by env_cmd_sz */
    /* uint8_t env_cmd[env_cmd_sz]; */
} QMI_CAT_SEND_ENVELOPE_CMD_request_0x01_t;

/*****************************************************************************
 * QMI_CAT_SET_CONFIGURATION (45 - 0x002D)
 * QMI_CAT_GET_CONFIGURATION (46 - 0x002E)
 */

/* Structure to describe the generic configuration mode */
typedef struct {
    uint8_t cat_config_mode;
} QMI_CAT_generic_configuration_mode_t;

/*****************************************************************************
 * QMI_CAT_GET_CACHED_PROACTIVE_CMD (47 - 0x002F)
 */

/* Structure to describe request TLV 0x01 for QMI_CAT_GET_CACHED_PROACTIVE_CMD */
typedef struct {
    uint32_t command_id;
} QMI_CAT_GET_CACHED_PROACTIVE_CMD_request_0x01_t;

/*****************************************************************************
 * QMI CAT indications
 */

/* Structure to describe generic the generic proactive command */
typedef struct {
    uint32_t uim_ref_id;
    uint16_t cmd_sz;
    /* This array must be the size specified by cmd_sz */
    /* uint8_t cmd[cmd_sz]; */
} QMI_CAT_generic_proactive_command_t;

/* Structure to describe indication TLV 0x1A for QMI_CAT_EVENT_REPORT_IND */
typedef struct {
    uint8_t proactive_session_end_type;
} QMI_CAT_EVENT_REPORT_indication_0x1A_t;

/* Structure to describe indication TLV 0x50 for QMI_CAT_EVENT_REPORT_IND */
typedef struct {
    uint8_t slot;
} QMI_CAT_EVENT_REPORT_indication_0x50_t;

/* Structure to describe indication TLV 0x68 for QMI_CAT_EVENT_REPORT_IND */
typedef struct {
    uint32_t rsp_type;
} QMI_CAT_EVENT_REPORT_indication_0x68_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_CAT_H_ */
