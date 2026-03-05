#ifndef TELIT_SDK_GAS_H_
#define TELIT_SDK_GAS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_gas.h
 * @brief GAS pack/unpack functions
 * Service ID: QMI_SVC_GAS (232)
 */

/**
 * @brief Enum to describe QMI GAS Message IDs
 */
typedef enum {
    /** 515 - 0x0203 Set USB composition */
    QMI_GAS_DMS_USB_CFG_SET        = 0x0203,
    /** 516 - 0x0204 Get USB composition information */
    QMI_GAS_DMS_USB_CFG_GET        = 0x0204,

    /** 518 - 0x0206 Activate specific modem fw */
    QMI_GAS_DMS_ACTIVE_FW          = 0x0206,

    /** 520 - 0x0208 Get stored modem f/w information */
    QMI_GAS_DMS_GET_FW             = 0x0208,
    /** 521 - 0x0209 Remove stored modem f/w */
    QMI_GAS_DMS_CLEAR_FW           = 0x0209,
    /** 522 - 0x020A Set reset info indication status */
    QMI_GAS_DMS_RESET_INFO_IND_REG = 0x020A,
    /** 522 - 0x020A Reset info indication */
    QMI_GAS_DMS_RESET_INFO_IND     = 0x020A,
    /** 523 - 0x020B Get reset info */
    QMI_GAS_DMS_GET_RESET_INFO     = 0x020B,
    /** 524 - 0x020C Set crash action */
    QMI_GAS_DMS_SET_CRASH_ACTION   = 0x020C,
    /** 525 - 0x020D Get crash info */
    QMI_GAS_DMS_GET_CRASH_INFO     = 0x020D,

    /** 527 - 0x020F Generate a crash */
    QMI_GAS_DMS_ERRGEN             = 0x020F,
    /** 528 - 0x0210 Get host fw version */
    QMI_GAS_DMS_GET_HOST_FWVER     = 0x0210,
    /** 529 - 0x0211 Get OEM fw version */
    QMI_GAS_DMS_GET_OEM_FWVER      = 0x0211,

    /** 530 - 0x0212 Get crash action */
    QMI_GAS_DMS_GET_CRASH_ACTION   = 0x0212,
    /** 531 - 0x0213 Set usb switch */
    QMI_GAS_DMS_SET_USB_SWITCH     = 0x0213,
    /** 532 - 0x0214 Get usb switch */
    QMI_GAS_DMS_GET_USB_SWITCH     = 0x0214,

    /** 553 - 0x0229 Set USB composition to normal mode or extension mode */
    QMI_GAS_DMS_SET_USB_EXT        = 0x0229,
    /** 554 - 0x022A Get current mode of USB composition */
    QMI_GAS_DMS_GET_USB_EXT        = 0x022A,

    /** 58372 - 0xE404 Get wake-up event mask */
    QMI_GAS_PSM_GET_WDISA_CFG      = 0xE404,
    /** 58373 - 0xE405 Set wake-up event mask */
    QMI_GAS_PSM_SET_WDISA_CFG      = 0xE405,

    /** 58624 - 0xE500 Redirect syslog to QXDM */
    QMI_GAS_LOG_SET_REDIRECT       = 0xE500,
} QMI_GAS_message_t;

#define QMI_GAS_MODEL_STR_LEN       5
#define QMI_GAS_SKU_STR_LEN         2
#define QMI_GAS_MAX_CARRIER_STR_LEN 3

/**
 * @brief QMI GAS carrier IDs
 */
typedef enum {
    /** No carrier */
    QMI_GAS_CARRIER_NONE                = 0,
    /** Generic carrier */
    QMI_GAS_CARRIER_GENERIC             = 1,
    /** Factory mode */
    QMI_GAS_CARRIER_FACTORY             = 2,

    /** AT&T */
    QMI_GAS_CARRIER_ATT                 = 4,
    /** Verizon */
    QMI_GAS_CARRIER_VERIZON             = 5,

    /** Sprint */
    QMI_GAS_CARRIER_SPRINT              = 11,

    /** Telstra */
    QMI_GAS_CARRIER_TELSTRA             = 107,
    /** China Unicom */
    QMI_GAS_CARRIER_CHINA_UNICOM        = 108,
    /** Telecom New Zealand */
    QMI_GAS_CARRIER_TELECOM_NEW_ZEALAND = 109,
    /** SK Telecom */
    QMI_GAS_CARRIER_SK_TELECOM          = 110,

    /** Tata Communications */
    QMI_GAS_CARRIER_TATA_COMMUNICATIONS = 112,

    /** KDDI */
    QMI_GAS_CARRIER_KDDI                = 115,

    /** China Telecom */
    QMI_GAS_CARRIER_CHINA_TELECOM       = 117,

    /** Vodafone */
    QMI_GAS_CARRIER_VODAFONE            = 202,
    /** T-Mobile */
    QMI_GAS_CARRIER_T_MOBILE            = 203,
    /** Orange */
    QMI_GAS_CARRIER_ORANGE              = 204,
    /** Telefonica */
    QMI_GAS_CARRIER_TELEFONICA          = 205,
    /** Telecom Italia */
    QMI_GAS_CARRIER_TELECOM_ITALIA      = 206,

    /** China Mobile */
    QMI_GAS_CARRIER_CHINA_MOBILE        = 211,

    /** Singapore Telecom */
    QMI_GAS_CARRIER_SINGAPORE_TELECOM   = 213,

    /** NTT Docomo */
    QMI_GAS_CARRIER_NTT_DOCOMO          = 216,

    /** Softbank */
    QMI_GAS_CARRIER_SOFTBANK            = 218,
    /** Korea Telecom */
    QMI_GAS_CARRIER_KOREA_TELECOM       = 219,
} QMI_GAS_carrier_t;

/**
 * @brief input type for QMI_GAS_DMS_USB_CFG_SET request, output type for QMI_GAS_DMS_USB_CFG_GET request
 *
 */
typedef struct {
    /** PID of the selected composition */
    uint32_t pid;
    /** Report if HSIC is enabled (always false) */
    uint8_t hsic;
    /** Persistence of the composition (only true supported) */
    bool persistence;
    /** Immediate setting of the composition (only false supported ) */
    bool immediate;
    /** Reboot needed to set composition (only true supported) */
    bool reboot;
} gas_dms_usb_cfg_info_t;

/**
 * @brief Pack QMI_GAS_DMS_USB_CFG_SET (515 - 0x0203) request
 *
 * This function packs a qmi request for setting the USB composition
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #gas_dms_usb_cfg_info_t
 */
QMI_SDK_error_t telit_gas_dms_usb_cfg_set_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gas_dms_usb_cfg_info_t *input);

/**
 * @brief Unpack QMI_GAS_DMS_USB_CFG_SET (515 - 0x0203) request
 *
 * This function unpacks the result of a QMI_GAS_DMS_USB_CFG_SET request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gas_dms_usb_cfg_set_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_GAS_DMS_USB_CFG_GET (516 - 0x0204) request
 *
 * This function packs a qmi request for getting USB composition info
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gas_dms_usb_cfg_get_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GAS_DMS_USB_CFG_GET (516 - 0x0204) response
 *
 * This function unpacks the result of a QMI_GAS_DMS_USB_CFG_GET request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gas_dms_usb_cfg_info_t
 */
QMI_SDK_error_t telit_gas_dms_usb_cfg_get_unpack(uint8_t *rsp, uint16_t len, gas_dms_usb_cfg_info_t *out);

#define QMI_GAS_FW_STRING_MAX_LEN 100

/**
 * @brief QMI GAS carrier name type
 *
 */
typedef struct {
    /** Length of name string */
    uint8_t name_len;
    /** carrier name which want to activate
     * Supported values:
     * - Generic
     * - Verizon
     * - ATT
     * - Sprint
     * - EU
     * - AU
     */
    char name[QMI_GAS_FW_STRING_MAX_LEN];
} gas_carrier_name_t;

/**
 * @brief QMI GAS version type
 *
 */
typedef struct {
    /** Length of versions string */
    uint8_t versions_len;
    /** fw version which want to activate */
    char versions[QMI_GAS_FW_STRING_MAX_LEN];
} gas_dms_versions_t;

/**
 * @brief QMI GAS auto-sim actions
 */
typedef enum {
    /** Enable automatic SIM */
    QMI_GAS_ENABLE_AUTO_SIM          = 1,
    /** One shot enable automatic SIM */
    QMI_GAS_ONE_SHOT_ENABLE_AUTO_SIM = 2,
    /** Disable automatic SIM */
    QMI_GAS_DISABLE_AUTO_SIM         = 3,
} QMI_GAS_auto_sim_action_t;

/**
 * @brief input type for QMI_GAS_DMS_ACTIVE_FW request
 *
 */
typedef struct {
    /** set if optional carrier_name is available */
    bool carrier_name_available;
    /** Carrier name structure */
    gas_carrier_name_t carrier_name;
    /** set if optional slot_index is available */
    bool slot_index_available;
    /** number of index which want to activate. (available range: 1 to 4) */
    uint8_t slot_index;
    /** set if optional version is available */
    bool version_available;
    /** Version structure */
    gas_dms_versions_t version;
    /** set if optional auto_sim_enable is available */
    bool auto_sim_enable_available;
    /** auto sim detection, see #QMI_GAS_auto_sim_action_t NOTE: In case of automatic SIM detection enabled, carrier switch is disabled */
    uint8_t auto_sim_enable;
} gas_dms_active_fw_t;

/**
 * @brief Pack QMI_GAS_DMS_ACTIVE_FW (518 - 0x0206) request
 *
 * This function packs a qmi request for activating specific modem fw
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #gas_dms_active_fw_t
 */
QMI_SDK_error_t telit_gas_dms_active_fw_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gas_dms_active_fw_t *input);

/**
 * @brief Unpack QMI_GAS_DMS_ACTIVE_FW (518 - 0x0206) request
 *
 * This function unpacks the result of a QMI_GAS_DMS_ACTIVE_FW request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gas_dms_active_fw_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI GAS operate modes for GAS_DMS_GET_FW
 */
typedef enum {
    /** Get the active fw */
    QMI_GAS_GET_FW_ACTIVE          = 0,
    /** Get all stored fw */
    QMI_GAS_GET_FW_ALL_STORED      = 1,
    /** Get fw according to a condition */
    QMI_GAS_GET_FW_SPECIFIC        = 2,
    /** Get the active fw with auto sim status */
    QMI_GAS_GET_FW_ACTIVE_AUTO_SIM = 3,
} QMI_GAS_get_fw_operate_mode_t;

/**
 * @brief QMI GAS operate modes for GAS_DMS_CLEAR_FW
 */
typedef enum {
    /** Clear all modem fw */
    QMI_GAS_CLEAR_ALL      = 0,
    /** Clear specific modem fw with condition */
    QMI_GAS_CLEAR_SPECIFIC = 1,
} QMI_GAS_clear_fw_operate_mode_t;

/**
 * @brief input type for QMI_GAS_DMS_GET_FW and QMI_GAS_DMS_CLEAR_FW requests
 *
 */
typedef struct {
    /** operate_mode
     *
     * for GAS_DMS_GET_FW see #QMI_GAS_get_fw_operate_mode_t
     * for GAS_DMS_CLEAR_FW see #QMI_GAS_clear_fw_operate_mode_t */
    uint8_t operate_mode;
    /** set if optional carrier_name is available */
    bool carrier_name_available;
    /** Carrier name structure */
    gas_carrier_name_t carrier_name;
    /** set if optional slot_index is available */
    bool slot_index_available;
    /** number of index which want to activate. (available range: 1 to 4) */
    uint8_t slot_index;
    /** set if optional version is available */
    bool version_available;
    /** Version structure */
    gas_dms_versions_t version;
} gas_dms_get_fw_t;

/**
 * @brief QMI GAS PRI revision type
 *
 */
typedef struct {
    /** Length of pri_rev string */
    uint8_t pri_rev_len;
    /** PRI revision */
    char pri_rev[QMI_GAS_FW_STRING_MAX_LEN];
} gas_pri_rev_t;

/**
 * @brief QMI GAS stored firmware info type
 *
 */
typedef struct {
    /** Index number stored firmware */
    uint8_t index;
    /** Len of carrier_str */
    uint8_t carrier_str_len;
    /** Carrier provider ID as string , see #QMI_GAS_carrier_t */
    char carrier_str[QMI_GAS_MAX_CARRIER_STR_LEN + 1];
    /** Len of model string */
    uint8_t model_len;
    /** Model string (e.g. LM940/LM960) */
    char model[QMI_GAS_MODEL_STR_LEN + 1];
    /** Len of sku string */
    uint8_t sku_len;
    /** chipset code (LM940:24, LM960:32) */
    char sku[QMI_GAS_SKU_STR_LEN + 1];
    /** Carrier name */
    gas_carrier_name_t carrier_name;
    /** Modem fw version */
    gas_dms_versions_t version;
    /** PRI revision */
    gas_pri_rev_t pri_rev;
} gas_stored_fw_info_t;

#define QMI_GAS_FW_MAX_NUMBER 4

/**
 * @brief QMI GAS auto-sim fw status
 */
typedef enum {
    /** Automatic SIM disabled */
    QMI_GAS_AUTO_SIM_DISABLED         = 0,
    /** Automatic SIM enabled */
    QMI_GAS_AUTO_SIM_ENABLED          = 1,
    /** One shot automatic SIM enabled */
    QMI_GAS_AUTO_SIM_ENABLED_ONE_SHOT = 2,
} QMI_GAS_fw_auto_sim_status_t;

/**
 * @brief output type for QMI_GAS_DMS_GET_FW request
 *
 */
typedef struct {
    /** operate_mode, see #QMI_GAS_get_fw_operate_mode_t */
    uint8_t operate_mode;
    /** Number of available element in fw_info array */
    uint8_t available_firmware;
    /** Firmware info array */
    gas_stored_fw_info_t fw_info[QMI_GAS_FW_MAX_NUMBER];
    /** true if auto_sim_status is available */
    bool auto_sim_status_available;
    /** Auto sim status, see #QMI_GAS_fw_auto_sim_status_t */
    uint8_t auto_sim_status;
} gas_dms_fw_info_t;

/**
 * @brief Pack QMI_GAS_DMS_GET_FW (520 - 0x0208) request
 *
 * This function packs a qmi request for getting stored modem fw information
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #gas_dms_get_fw_t
 */
QMI_SDK_error_t telit_gas_dms_get_fw_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gas_dms_get_fw_t *input);

/**
 * @brief Unpack QMI_GAS_DMS_GET_FW (520 - 0x0208) request
 *
 * This function unpacks the result of a QMI_GAS_DMS_GET_FW request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gas_dms_fw_info_t
 */
QMI_SDK_error_t telit_gas_dms_get_fw_unpack(uint8_t *rsp, uint16_t len, gas_dms_fw_info_t *out);

/**
 * @brief Pack QMI_GAS_DMS_CLEAR_FW (521 - 0x0209) request
 *
 * This function packs a qmi request to remove stored modem firmware
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #gas_dms_get_fw_t
 */
QMI_SDK_error_t telit_gas_dms_clear_fw_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gas_dms_get_fw_t *input);

/**
 * @brief Unpack QMI_GAS_DMS_CLEAR_FW (521 - 0x0209) request
 *
 * This function unpacks the result of a QMI_GAS_DMS_CLEAR_FW request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gas_dms_clear_fw_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_GAS_DMS_RESET_INFO_IND_REG request
 *
 */
typedef struct {
    /** Action for reset info, see #QMI_indication_action_t */
    QMI_indication_action_t reset_info_enable;
} gas_dms_reset_info_indication_register_t;

/**
 * @brief Pack QMI_GAS_DMS_RESET_INFO_IND_REG (522 - 0x020A) request
 *
 * This function packs a qmi request for changing reset info indications status.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #gas_dms_reset_info_indication_register_t
 */
QMI_SDK_error_t telit_gas_dms_reset_info_ind_reg_pack(pack_qmi_t *req_ctx, uint8_t *req,
        uint16_t *len,
        gas_dms_reset_info_indication_register_t *input);

/**
 * @brief Unpack QMI_GAS_DMS_RESET_INFO_IND_REG (522 - 0x020A) request
 *
 * This function unpacks the result of a QMI_GAS_DMS_RESET_INFO_IND_REG request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gas_dms_reset_info_ind_reg_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI GAS reset or power-down types
 */
typedef enum {
    /** Unknown */
    QMI_GAS_RESET_TYPE_UNKNOWN = 0,
    /** Warm */
    QMI_GAS_RESET_TYPE_WARM    = 1,
    /** Hard */
    QMI_GAS_RESET_TYPE_HARD    = 2,
    /** Crash */
    QMI_GAS_RESET_TYPE_CRASH   = 3,
    /** Power */
    QMI_GAS_RESET_TYPE_POWER   = 4,
} QMI_GAS_reset_type_t;

/**
 * @brief QMI GAS entities which initiated the reset or power-down
 */
typedef enum {
    /** Unknown */
    QMI_GAS_RESET_SOURCE_UNKNOWN              = 0,
    /** User requested */
    QMI_GAS_RESET_SOURCE_USER_REQUESTED       = 1,
    /** Hardware switch */
    QMI_GAS_RESET_SOURCE_HARDWARE_SWITCH      = 2,
    /** Temperature critical */
    QMI_GAS_RESET_SOURCE_TEMPERATURE_CRITICAL = 3,
    /** Voltage critical */
    QMI_GAS_RESET_SOURCE_VOLTAGE_CRITICAL     = 4,
    /** Configuration update */
    QMI_GAS_RESET_SOURCE_CONFIGURATION_UPDATE = 5,
    /** LWM2M */
    QMI_GAS_RESET_SOURCE_LWM2M                = 6,
    /** OMA-DM */
    QMI_GAS_RESET_SOURCE_OMADM                = 7,
    /** FOTA */
    QMI_GAS_RESET_SOURCE_FOTA                 = 8,
} QMI_GAS_reset_source_t;

/**
 * @brief output type for QMI_GAS_DMS_GET_RESET_INFO request
 *
 */
typedef struct {
    /** reset information, see #QMI_GAS_reset_type_t */
    uint8_t type;
    /** entity which initiated the reset or power down, see #QMI_GAS_reset_source_t */
    uint8_t source;
} gas_dms_reset_info_t;

/**
 * @brief Pack QMI_GAS_DMS_GET_RESET_INFO (523 - 0x020A) request
 *
 * This function packs a qmi request for getting reset info for the most recent device reset or power-down
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gas_dms_get_reset_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GAS_DMS_GET_RESET_INFO (523 - 0x020A) request
 *
 * This function unpacks the result of a QMI_GAS_DMS_GET_RESET_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gas_dms_reset_info_t
 */
QMI_SDK_error_t telit_gas_dms_get_reset_info_unpack(uint8_t *rsp, uint16_t len, gas_dms_reset_info_t *out);

/**
 * @brief QMI GAS crash actions
 */
typedef enum {
    /** Modem will reset after a crash and will stay in USB download mode with only DM port enumerated */
    QMI_GAS_CRASH_ACTION_USB_DL_MODE   = 0,
    /** Modem will reset and come back in ONLINE mode */
    QMI_GAS_CRASH_ACTION_RESET         = 1,
    /** USB Memory Download. With USB 2.0 */
    QMI_GAS_CRASH_ACTION_USB_DL_MODE_2 = 2,
    /** USB Memory Download. With USB 3.0 */
    QMI_GAS_CRASH_ACTION_USB_DL_MODE_3 = 3,
} QMI_GAS_crash_action_t;

/**
 * @brief input type for QMI_GAS_DMS_SET_CRASH_ACTION request
 *
 */
typedef struct {
    /** Action to be performed after a crash, see #QMI_GAS_crash_action_t */
    enum8_t crash_action;
} gas_dms_crash_action_t;

/**
 * @brief Pack QMI_GAS_DMS_SET_CRASH_ACTION (524 - 0x020C) request
 *
 * This function packs a qmi request for storing crash action mode
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #gas_dms_crash_action_t
 */
QMI_SDK_error_t telit_gas_dms_set_crash_action_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gas_dms_crash_action_t *input);

/**
 * @brief Unpack QMI_GAS_DMS_SET_CRASH_ACTION (524 - 0x020C) request
 *
 * This function unpacks the result of a QMI_GAS_DMS_SET_CRASH_ACTION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gas_dms_set_crash_action_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_GAS_DMS_GET_CRASH_INFO request
 *
 */
typedef struct {
    /** crash info indication registration.
          values:
       - 0 - do not clear crash data after response
       - 1 - clear crash data afer response */
    uint8_t clear;
} gas_dms_get_crash_info_t;

/**
 * @brief Pack QMI_GAS_DMS_GET_CRASH_INFO (525 - 0x020D) request
 *
 * This function packs a qmi request to get crash information
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] in request buffer size, out request len
 * @param[in] input #gas_dms_get_crash_info_t
 */
QMI_SDK_error_t telit_gas_dms_get_crash_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        gas_dms_get_crash_info_t *input);

#define QMI_GAS_SHORT_STRING_SIZE 16

/**
 * @brief QMI GAS crash information type
 */
typedef struct {
    /** Length of excep_info_sw_ver */
    uint8_t excep_info_sw_ver_len;
    /** software version name
     * (length limit 16 bytes) */
    char excep_info_sw_ver[QMI_GAS_SHORT_STRING_SIZE + 1];
    /** Length of excep_info_date */
    uint8_t excep_info_date_len;
    /** date information (length limit 16 bytes) */
    char excep_info_date[QMI_GAS_SHORT_STRING_SIZE + 1];
    /** Length of excep_info_time */
    uint8_t excep_info_time_len;
    /** time information. (UTC time. Not local time)
     * (length limit 16 bytes) */
    char excep_info_time[QMI_GAS_SHORT_STRING_SIZE + 1];
    /** line number information */
    uint32_t excep_info_line;
    /** Length of excep_info_filename */
    uint8_t excep_info_filename_len;
    /** File name information.
     * (length limit 32 bytes) */
    char excep_info_filename[QMI_GAS_SHORT_STRING_SIZE * 2 + 1];
    /** Length of excep_info_msg */
    uint8_t excep_info_msg_len;
    /** crash message information
    * (length limit 255 bytes) */
    char excep_info_msg[MAX_STRING_SIZE + 1];
    /** Total crash count */
    uint32_t excep_info_crash_total_cnt;
} gas_crash_information_t;

/**
 * @brief input type for QMI_GAS_DMS_GET_CRASH_INFO response
 *
 */
typedef struct {
    /** crash status.
          values:
       - 0 - no crash
       - 1 - crash has occurred */
    uint8_t crash_status;
    /** true if crash_info is available */
    uint8_t crash_information_available;
    /** see #gas_crash_information_t */
    gas_crash_information_t crash_info;
} gas_dms_get_crash_info_resp_t;

/**
 * @brief Unpack QMI_GAS_DMS_GET_CRASH_INFO (525 - 0x020D) response
 *
 * This function unpacks the result of a GAS_DMS_GET_CRASH_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gas_dms_get_crash_info_resp_t
 */
QMI_SDK_error_t telit_gas_dms_get_crash_info_unpack(uint8_t *rsp, uint16_t len,
        gas_dms_get_crash_info_resp_t *out);

/**
 * @brief Pack QMI_GAS_DMS_ERRGEN (527 - 0x020F) request
 *
 * This function packs a qmi request for generating a crash
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gas_dms_errgen_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GAS_DMS_ERRGEN (527 - 0x020F) request
 *
 * This function unpacks the result of a QMI_GAS_DMS_ERRGEN request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gas_dms_errgen_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_GAS_DMS_GET_HOST_FWVER (528 - 0x0210) request
 *
 * This function packs a qmi request for getting host firmware version
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gas_dms_get_host_fwver_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GAS_DMS_GET_HOST_FWVER (528 - 0x0210) response
 *
 * This function unpacks the result of a QMI_GAS_DMS_GET_HOST_FWVER request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out buffer to hold #gas_dms_versions_t struct
 */
QMI_SDK_error_t telit_gas_dms_get_host_fwver_unpack(uint8_t *rsp, uint16_t len, gas_dms_versions_t *out);

#define QMI_GAS_MAX_OEM_FIRMWARE_NAME 39

/**
 * @brief Pack QMI_GAS_DMS_GET_OEM_FWVER (529 - 0x0211) request
 *
 * This function packs a qmi request for getting OEM firmware version
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gas_dms_get_oem_fwver_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

typedef struct {
    char oem_fwver[QMI_GAS_MAX_OEM_FIRMWARE_NAME + 1];
    uint8_t oem_fwver_len;
    char reserved_ver_1[MAX_STRING_SIZE + 1];
    uint8_t reserved_ver_1_len;
    char reserved_ver_2[MAX_STRING_SIZE + 1];
    uint8_t reserved_ver_2_len;
} gas_dms_get_oem_fwver_resp_t;

/**
 * @brief Unpack QMI_GAS_DMS_GET_OEM_FWVER (529 - 0x0211) response
 *
 * This function unpacks the result of a QMI_GAS_DMS_GET_OEM_FWVER request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gas_dms_versions_t
 */
QMI_SDK_error_t telit_gas_dms_get_oem_fwver_unpack(uint8_t *rsp, uint16_t len,
        gas_dms_get_oem_fwver_resp_t *out);

/**
 * @brief Pack QMI_GAS_DMS_GET_CRASH_ACTION (530 - 0x0212) request
 *
 * This function packs a qmi request for getting the crash action mode from device
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gas_dms_get_crash_action_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GAS_DMS_GET_CRASH_ACTION (530 - 0x0212) request
 *
 * This function unpacks the result of a QMI_GAS_DMS_GET_CRASH_ACTION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gas_dms_crash_action_t
 */
QMI_SDK_error_t telit_gas_dms_get_crash_action_unpack(uint8_t *rsp, uint16_t len,
        gas_dms_crash_action_t *out);

/**
 * @brief input type for QMI_GAS_DMS_SET_USB_SWITCH request, output type for QMI_GAS_DMS_GET_USB_SWITCH request
 *
 */
typedef struct {
    /** USB switch configuration
     *
     * values:
     * - 0 Not configured
     * - 1 USB High Speed */
    uint32_t config;
} gas_dms_usb_switch_t;

/**
 * @brief Pack QMI_GAS_DMS_SET_USB_SWITCH (531 - 0x0213) request
 *
 * This function packs a qmi request to set USB speed mode
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #gas_dms_usb_switch_t
 */
QMI_SDK_error_t telit_gas_dms_set_usb_switch_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        gas_dms_usb_switch_t *input);

/**
 * @brief Unpack QMI_GAS_DMS_SET_USB_SWITCH (531 - 0x0213) request
 *
 * This function unpacks the result of a QMI_GAS_DMS_SET_USB_SWITCH request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gas_dms_set_usb_switch_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_GAS_DMS_GET_USB_SWITCH (532 - 0x0214) request
 *
 * This function packs a qmi request to get current USB speed mode
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gas_dms_get_usb_switch_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GAS_DMS_GET_USB_SWITCH (532 - 0x0214) request
 *
 * This function unpacks the result of a QMI_GAS_DMS_GET_USB_SWITCH request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gas_dms_usb_switch_t
 */
QMI_SDK_error_t telit_gas_dms_get_usb_switch_unpack(uint8_t *rsp, uint16_t len, gas_dms_usb_switch_t *out);

/**
 * @brief input type for QMI_GAS_DMS_SET_USB_EXT request, output type for QMI_GAS_DMS_GET_USB_EXT request
 *
 */
typedef struct {
    /** USB Extend configuration
     *
     * values:
     * - 0 Normal mode
     * - 1 Extension mode */
    uint8_t mode;
} gas_dms_usb_ext_t;

/**
 * @brief Pack QMI_GAS_DMS_SET_USB_EXT (553 - 0x0229) request
 *
 * This function packs a qmi request to set USB composition to normal mode or extension mode.
 * The value set by this command is available at next power-on.
 * This command can only be used when USB PID is 0x1065.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #gas_dms_usb_ext_t
 */
QMI_SDK_error_t telit_gas_dms_set_usb_ext_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        gas_dms_usb_ext_t *input);

/**
 * @brief Unpack QMI_GAS_DMS_SET_USB_EXT (553 - 0x0229) request
 *
 * This function unpacks the result of a QMI_GAS_DMS_SET_USB_EXT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gas_dms_set_usb_ext_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_GAS_DMS_GET_USB_EXT (554 - 0x022A) request
 *
 * This function packs a qmi request to get current mode of USB composition.
 * This command can only be used when USB PID is 0x1065.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gas_dms_get_usb_ext_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GAS_DMS_GET_USB_EXT (554 - 0x022A) request
 *
 * This function unpacks the result of a QMI_GAS_DMS_GET_USB_EXT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gas_dms_usb_ext_t
 */
QMI_SDK_error_t telit_gas_dms_get_usb_ext_unpack(uint8_t *rsp, uint16_t len, gas_dms_usb_ext_t *out);

/**
 * @brief QMI GAS PSM WDISA configuration
 */
typedef enum {
    /** Minimum functionality mode */
    QMI_GAS_WDISA_MINIMUM                 = 0,
    /** Power saving mode */
    QMI_GAS_WDISA_POWER_SAVING            = 1,
    /** Ignore on w_disable pin */
    QMI_GAS_WDISA_IGNORE_ON_W_DISABLE_PIN = 2,
    /** Enable dying gasp with W_DISABLE_N pin (activated on low edge) */
    QMI_GAS_WDISA_ENABLE_DYING_GASP       = 10,
} QMI_GAS_WDISA_config_t;

/**
 * @brief input type for QMI_GAS_PSM_SET_WDISA_CFG request, output type for QMI_GAS_PSM_GET_WDISA_CFG request
 *
 */
typedef struct {
    /** W_DISABLE_N configuration, see #QMI_GAS_WDISA_config_t */
    uint32_t w_disable_cfg;
} gas_psm_wdisa_cfg_t;

/**
 * @brief Pack QMI_GAS_PSM_GET_WDISA_CFG (58372 - 0xE404) request
 *
 * This function packs a qmi request to get W_DISABLE_N pin configuration
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gas_psm_get_wdisa_cfg_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GAS_PSM_GET_WDISA_CFG (58372 - 0xE404) request
 *
 * This function unpacks the result of a QMI_GAS_PSM_GET_WDISA_CFG request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gas_psm_wdisa_cfg_t
 */
QMI_SDK_error_t telit_gas_psm_get_wdisa_cfg_unpack(uint8_t *rsp, uint16_t len, gas_psm_wdisa_cfg_t *out);

/**
 * @brief Pack QMI_GAS_PSM_SET_WDISA_CFG (58373 - 0xE405) request
 *
 * This function packs a qmi request to set W_DISABLE_N pin configuration
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #gas_psm_wdisa_cfg_t
 */
QMI_SDK_error_t telit_gas_psm_set_wdisa_cfg_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        gas_psm_wdisa_cfg_t *input);

/**
 * @brief Unpack QMI_GAS_PSM_SET_WDISA_CFG (58373 - 0xE405) request
 *
 * This function unpacks the result of a QMI_GAS_PSM_SET_WDISA_CFG request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gas_psm_set_wdisa_cfg_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_GAS_LOG_SET_REDIRECT request
 *
 */
typedef struct {
    /** Flag to manage redirect according to the following values:
       - 0 - do not use redirection
       - 1 - redirect syslog to QXDM */
    uint32_t enable_redirect;
} gas_log_redirect_info_t;

/**
 * @brief Pack QMI_GAS_LOG_SET_REDIRECT (58624 - 0xE500) request
 *
 * This function packs a qmi request for managing syslog redirect
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #gas_log_redirect_info_t
 */
QMI_SDK_error_t telit_gas_log_set_redirect_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gas_log_redirect_info_t *input);

/**
 * @brief Unpack QMI_GAS_LOG_SET_REDIRECT (58624 - 0xE500) response
 *
 * This function unpacks the result of a QMI_GAS_LOG_SET_REDIRECT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gas_log_set_redirect_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Unpack gas indications
 *
 * This function unpacks the result of the following gas indications
 *
 * QMI_GAS_DMS_RESET_INFO_IND (522)
 *
 * @param[in] msg_id indication id
 * @param[in] rsp indication
 * @param[in] len indication length
 * @param[out] out type according to the following
 *      QMI_GAS_DMS_RESET_INFO_IND -> #gas_dms_reset_info_t
 * @param[in] out_len buffer size
 */
QMI_SDK_error_t telit_gas_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len,
        uint8_t *out, uint16_t out_len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_GAS_H_ */
