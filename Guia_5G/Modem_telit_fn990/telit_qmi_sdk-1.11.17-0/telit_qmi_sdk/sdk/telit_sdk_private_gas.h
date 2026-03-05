#ifndef TELIT_SDK_PRIVATE_GAS_H_
#define TELIT_SDK_PRIVATE_GAS_H_

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_GAS_DMS_USB_CFG_SET (515 - 0x0203)
 * QMI_GAS_DMS_USB_CFG_GET (516 - 0x0204)
 */

/* Structure to describe generic usb cfg PID */
typedef struct {
    uint32_t pid;
} QMI_GAS_DMS_generic_usb_cfg_pid_t;

/* Structure to describe generic usb cfg hsic support */
typedef struct {
    uint8_t hsic;
} QMI_GAS_DMS_generic_usb_cfg_hsic_t;

/* Structure to describe generic usb persistence support */
typedef struct {
    uint8_t persistence;
} QMI_GAS_DMS_generic_usb_cfg_persistence_t;

/* Structure to describe generic immediate enablement */
typedef struct {
    uint8_t immediate;
} QMI_GAS_DMS_generic_usb_cfg_immediate_t;

/* Structure to describe generic reboot needed */
typedef struct {
    uint8_t reboot;
} QMI_GAS_DMS_generic_usb_cfg_reboot_t;

/*****************************************************************************
 * QMI_GAS_DMS_ACTIVE_FW (518 - 0x0206)
 * QMI_GAS_DMS_GET_FW (520 - 0x0208)
 * QMI_GAS_DMS_CLEAR_FW (521 - 0x0209)
 */

/* Structure to describe QMI GAS generic firmware string */
typedef struct {
    uint8_t len;
    /* char string[100]; */
} QMI_GAS_DMS_generic_fw_string_t;

/* Structure to describe QMI GAS generic slot index */
typedef struct {
    uint8_t slot_index;
} QMI_GAS_DMS_generic_fw_slot_index_t;

/* Structure to describe request TLV 0x13 for QMI_GAS_DMS_ACTIVE_FW */
typedef struct {
    uint8_t auto_sim_enable;
} QMI_GAS_DMS_ACTIVE_FW_request_0x13_t;

/* Structure to describe generic QMI GAS operate mode */
typedef struct {
    uint8_t operate_mode;
} QMI_GAS_DMS_generic_fw_operate_mode_t;

/* Structure to describe generic firmware info */
typedef struct {
    uint8_t index;
    uint8_t name_len;
    /* char name[100]; */
    uint8_t versions_len;
    /* char versions[100]; */
    uint8_t pri_rev_len;
    /* char pri_rev[100]; */
} QMI_GAS_DMS_generic_fw_info_t;

/* Structure to describe TLV 0x14 for QMI_GAS_DMS_GET_FW response */
typedef struct {
    uint8_t auto_sim_status;
} QMI_GAS_DMS_GET_FW_response_0x14_t;

/*****************************************************************************
 * QMI_GAS_DMS_RESET_INFO_IND_REG (522 - 0x020A)
 */

/* Structure to describe reset info indication register request TLV 0x01 */
typedef struct {
    uint8_t reset_info_enable;
} QMI_GAS_DMS_SET_RESET_INFO_IND_REGISTER_request_0x01_t;

/*****************************************************************************
 * QMI_GAS_DMS_GET_RESET_INFO (523 - 0x020B)
 */

/* Structure to describe generic reset info */
typedef struct {
    uint8_t type;
    uint8_t source;
} QMI_GAS_DMS_generic_reset_info_t;

/*****************************************************************************
 * QMI_GAS_DMS_SET_CRASH_ACTION (524 - 0x020C)
 * QMI_GAS_DMS_GET_CRASH_ACTION (530 - 0x0212)
 */

/* Structure to describe generic crash action */
typedef struct {
    uint8_t crash_action;
} QMI_GAS_DMS_generic_crash_action_t;

/* Structure to describe crash info request TLV 0x01 */
typedef struct {
    uint8_t clear;
} QMI_GAS_DMS_GET_CRASH_INFO_request_0x01_t;

/* Structure to describe crash info response TLV 0x03 */
typedef struct {
    uint8_t crash_status;
} QMI_GAS_DMS_GET_CRASH_INFO_response_0x03_t;

/* Structure to describe crash info response TLV 0x10 */
typedef struct {
    uint8_t excep_info_sw_ver_len;
    /* char   excep_info_sw_ver[16]; this array must the of the length of excep_info_sw_ver_len and max 16 bytes */
    uint8_t excep_info_date_len;
    /* char   excep_info_date[excep_info_date_len]; array size should be excep_info_date_len and max 16 bytes */
    uint8_t excep_info_time_len;
    /* char   excep_info_time[excep_info_time_len]; array size should be excep_info_time_len and max 16 bytes */
    uint32_t excep_info_line;
    uint8_t excep_info_filename_len;
    /* char   excep_info_filename[excep_info_filename_len]; array size should be excep_info_filename_len and max 32 bytes */
    uint8_t excep_info_msg_len;
    /* char   excep_info_msg[excep_info_msg_len]; array size should be excep_info_msg_len and max 256 bytes */
    uint32_t excep_info_crash_total_cnt;
} QMI_GAS_DMS_GET_CRASH_INFO_response_0x10_t;

/*****************************************************************************
 * QMI_GAS_DMS_SET_USB_SWITCH (531 - 0x0213)
 * QMI_GAS_DMS_GET_USB_SWITCH (532 - 0x0214)
 */

/* Structure to describe generic USB switch config */
typedef struct {
    uint32_t config;
} QMI_GAS_DMS_generic_usb_switch_config_t;

/*****************************************************************************
 * QMI_GAS_DMS_SET_USB_EXT (553 - 0x0229)
 * QMI_GAS_DMS_GET_USB_EXT (554 - 0x022A)
 */

/* Structure to describe generic USB ext TLV */
typedef struct {
    uint8_t mode;
} QMI_GAS_DMS_generic_usb_ext_t;

/*****************************************************************************
 * QMI_GAS_PSM_GET_WDISA_CFG (58372 - 0xE404)
 * QMI_GAS_PSM_SET_WDISA_CFG (58373 - 0xE405)
 */

/* Structure to describe generic WDISA config */
typedef struct {
    uint32_t w_disable_cfg;
} QMI_GAS_PSM_generic_WDISA_config_t;

/*****************************************************************************
 * QMI_GAS_LOG_SET_REDIRECT (58624 - 0xE500)
 */

/* Structure to describe TLV 0x01 for QMI_GAS_LOG_SET_REDIRECT request */
typedef struct {
    uint32_t enable_redirect;
} QMI_GAS_LOG_request_0x01_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_GAS_H_ */
