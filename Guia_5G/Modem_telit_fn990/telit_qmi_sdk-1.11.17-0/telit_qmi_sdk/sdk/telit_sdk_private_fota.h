#ifndef TELIT_SDK_PRIVATE_FOTA_H_
#define TELIT_SDK_PRIVATE_FOTA_H_

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_SET_SERVER_INFO (64 - 0x0040)
 * QMI_FOTA_OEM_FTP_GET_SERVER_INFO (65 - 0x0041)
 */

/* Structure to describe generic FTP port */
typedef struct {
    uint16_t ftp_port;
} QMI_FOTA_generic_ftp_port_t;

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_START_SESSION (66 - 0x0042)
 * QMI_FOTA_OEM_FTP_STOP_SESSION (67 - 0x0043)
 */

/* Structure to describe generic session type */
typedef struct {
    uint8_t session_type;
} QMI_FOTA_generic_session_type_t;

/* Structure to describe TLV 0x03 for QMI_FOTA_OEM_FTP_START_SESSION response */
typedef struct {
    uint32_t session_response;
} QMI_FOTA_OEM_FTP_START_SESSION_response_0x03_t;

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_SET_SETTINGS (68 - 0x0044)
 * QMI_FOTA_OEM_FTP_GET_SETTINGS (69 - 0x0045)
 */

/* Structure to describe generic firmware auto download preference */
typedef struct {
    uint8_t auto_download_firmware;
} QMI_FOTA_fw_auto_download_t;

/* Structure to describe generic firmware auto update preference */
typedef struct {
    uint8_t auto_update_firmware;
} QMI_FOTA_fw_auto_update_t;

/* Structure to describe TLV 0x10 for QMI_FOTA_OEM_FTP_SET_SETTINGS request */
typedef struct {
    uint32_t value;
} QMI_FOTA_OEM_FTP_SET_SETTINGS_0x10_t;

/* Structure to describe generic OMA UI alert response */
typedef struct {
    uint8_t fw_auto_SDM;
} QMI_FOTA_fw_auto_SDM_t;

/* Structure to describe generic auto connect */
typedef struct {
    uint8_t auto_connect;
} QMI_FOTA_auto_connect_t;

/* Structure to describe generic auto reboot */
typedef struct {
    uint8_t auto_reboot;
} QMI_FOTA_auto_reboot_t;

/* Structure to describe TLV 0x14 for QMI_FOTA_OEM_FTP_SET_SETTINGS request */
typedef struct {
    uint16_t timers[8];
} QMI_FOTA_OEM_FTP_SET_SETTINGS_0x14_t;

/* Structure to describe generic notif store */
typedef struct {
    uint8_t notif_store;
} QMI_FOTA_notif_store_t;

/* Structure to describe generic period info */
typedef struct {
    uint32_t u_min;
    uint32_t u_max;
} QMI_FOTA_period_info_t;

/* Structure to describe TLV 0x1A for QMI_FOTA_OEM_FTP_GET_SETTINGS response */
typedef struct {
    uint32_t oma_dm_enabled;
} QMI_FOTA_OEM_FTP_GET_SETTINGS_response_0x1A_t;

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_SET_EVENT_REPORT (70 - 0x0046)
 */

/* Structure to describe TLV 0x01 for QMI_FOTA_OEM_FTP_SET_EVENT_REPORT request */
typedef struct {
    uint8_t enable_event;
} QMI_FOTA_OEM_FTP_SET_EVENT_REPORT_request_0x01_t;

/*****************************************************************************
 * QMI_FOTA_OEM_FTP_SEND_SELECTION (71 - 0x0047)
 */

/* Structure to describe TLV 0x01 for QMI_FOTA_OEM_FTP_SEND_SELECTION request */
typedef struct {
    uint8_t selection;
} QMI_FOTA_OEM_FTP_SEND_SELECTION_request_0x01_t;

/* Structure to describe TLV 0x10 for QMI_FOTA_OEM_FTP_SEND_SELECTION request */
typedef struct {
    uint32_t defer_time;
} QMI_FOTA_OEM_FTP_SEND_SELECTION_request_0x10_t;

/* Structure to describe TLV 0x12 for QMI_FOTA_OEM_FTP_SEND_SELECTION request */
typedef struct {
    uint8_t client_perform_operation_flag;
} QMI_FOTA_OEM_FTP_SEND_SELECTION_request_0x12_t;

/* Structure to describe TLV 0x13 for QMI_FOTA_OEM_FTP_SEND_SELECTION request */
typedef struct {
    uint8_t package_id;
} QMI_FOTA_OEM_FTP_SEND_SELECTION_request_0x13_t;

/*****************************************************************************
 * QMI FOTA indications
 */

/* Structure to describe generic binary update session info */
typedef struct {
    uint8_t binary_type;
    uint8_t state;
    uint8_t user_input_request;
    uint16_t user_input_timeout;
    uint32_t package_size;
    uint32_t downloaded_size;
    uint16_t update_complete_status;
    uint8_t severity;
} QMI_FOTA_OEM_FTP_generic_binary_session_info_t;

/* Structure to describe generic session notification */
typedef struct {
    uint16_t session_status;
} QMI_FOTA_OEM_FTP_generic_session_status_t;

/* Structure to describe generic user input */
typedef struct {
    uint8_t user_input_request;
    uint16_t user_input_timeout;
} QMI_FOTA_OEM_FTP_generic_user_input_t;

/* Structure to describe generic update status */
typedef struct {
    uint32_t update_package_status;
} QMI_FOTA_OEM_FTP_generic_update_status_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_FOTA_H_ */
