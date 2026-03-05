#ifndef TELIT_SDK_FOTA_H_
#define TELIT_SDK_FOTA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_fota.h
 * @brief FOTA pack/unpack functions
 * Service ID: QMI_SVC_FOTA (230)
 */

/**
 * @brief Enum to describe QMI FOTA Message IDs
 */
typedef enum {
    /** 64 - 0x0040 Set FTP server information */
    QMI_FOTA_OEM_FTP_SET_SERVER_INFO        = 0x0040,
    /** 65 - 0x0041 Get FTP server information */
    QMI_FOTA_OEM_FTP_GET_SERVER_INFO        = 0x0041,
    /** 66 - 0x0042 Start client-initiated session */
    QMI_FOTA_OEM_FTP_START_SESSION          = 0x0042,
    /** 67 - 0x0043 Stop current session */
    QMI_FOTA_OEM_FTP_STOP_SESSION           = 0x0043,
    /** 68 - 0x0044 Set FTP FOTA related settings */
    QMI_FOTA_OEM_FTP_SET_SETTINGS           = 0x0044,
    /** 69 - 0x0045 Get FTP FOTA related settings */
    QMI_FOTA_OEM_FTP_GET_SETTINGS           = 0x0045,
    /** 70 - 0x0046 Enable FTP FOTA notifications */
    QMI_FOTA_OEM_FTP_SET_EVENT_REPORT       = 0x0046,
    /** 71 - 0x0047 Receive user selection command */
    QMI_FOTA_OEM_FTP_SEND_SELECTION         = 0x0047,
    /** 72 - 0x0048 Indication for FTP OTA */
    QMI_FOTA_OEM_FTP_EVENT_REPORT_INDICATOR = 0x0048,
    /** 73 - 0x0049 Get information related to the current host odis */
    QMI_FOTA_OEM_ATT_GET_HOST_ODIS          = 0x0049,
    /** 74 - 0x004A Set information related to the current host odis to the NVM and OMA-DM application */
    QMI_FOTA_OEM_ATT_SET_HOST_ODIS          = 0x004A,
    /** 75 - 0x004B Get current FOTA session information */
    QMI_FOTA_OEM_FTP_GET_SESSION_INFO       = 0x004B,
} QMI_FOTA_message_t;

#define QMI_FOTA_MAX_URL_STRING_SIZE (800 + 1)
#define QMI_FOTA_MAX_USER_STRING_SIZE (256 + 100)
#define QMI_FOTA_MAX_PASSWORD_SIZE (32 + 1)

/**
 * @brief output type for QMI_FOTA_OEM_FTP_GET_SERVER_INFO request
 *
 */
typedef struct {
    /** ftp_url string length */
    uint16_t ftp_url_len;
    /** FTP Server URL */
    char ftp_url[QMI_FOTA_MAX_URL_STRING_SIZE];
    /** delta_descriptor_path string length */
    uint16_t delta_descriptor_path_len;
    /** Root path where fota delta descriptor is gathered
     * If this value is not defined, ftp home directory
     * will be used.
     * For example, if delta descriptor is uploaded to
     * the ftp server in the following path
     * FOTA_DELTA/<MODEL>/<CURRENT_SW_VER>/delta_descriptor.txt
     * this field shall be filled with the value "FOTA_DELTA" */
    char delta_descriptor_path[QMI_FOTA_MAX_URL_STRING_SIZE];
    /** set if ftp_port is available */
    bool ftp_port_available;
    /** FTP port */
    uint16_t ftp_port;
    /** user_name string length */
    uint16_t user_name_len;
    /** User Name */
    char user_name[QMI_FOTA_MAX_USER_STRING_SIZE];
    /** password string length */
    uint8_t password_len;
    /** Password
     * This value is digested with md5(password)
     * from the password received through the
     * SET message */
    char password[QMI_FOTA_MAX_PASSWORD_SIZE];
} fota_server_info_t;

/**
 * @brief Pack QMI_FOTA_OEM_FTP_SET_SERVER_INFO (64 - 0x0040) request
 *
 * This function packs a qmi request for setting ftp server
 * information before starting client-initiated session
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #fota_server_info_t
 */
QMI_SDK_error_t telit_fota_oem_ftp_set_server_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_server_info_t *input);

/**
 * @brief Unpack QMI_FOTA_OEM_FTP_SET_SERVER_INFO (64 - 0x0040) request
 *
 * This function unpacks the result of a QMI_FOTA_OEM_FTP_SET_SERVER_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_fota_oem_ftp_set_server_info_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_FOTA_OEM_FTP_GET_SERVER_INFO (65 - 0x0041) request
 *
 * This function packs a qmi request for for getting ftp server information
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_fota_oem_ftp_get_server_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_FOTA_OEM_FTP_GET_SERVER_INFO (65 - 0x0041) request
 *
 * This function unpacks the result of a QMI_FOTA_OEM_FTP_GET_SERVER_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #fota_server_info_t
 */
QMI_SDK_error_t telit_fota_oem_ftp_get_server_info_unpack(uint8_t *rsp, uint16_t len,
        fota_server_info_t *out);

/**
 * @brief input type for QMI_FOTA_OEM_FTP_START_SESSION request
 *
 */
typedef struct {
    /** Session type.
     *
     * Values:
       - 0x01 – FOTA, to check availability of FW Update */
    uint8_t session_type;
} fota_session_type_t;

/**
 * @brief QMI FOTA session response values
 *
 */
typedef enum {
    /** Available */
    QMI_FOTA_AVAILABLE                     = 0x00000001,
    /** Not Available */
    QMI_FOTA_NOT_AVAILABLE                 = 0x00000002,
    /** Check Timed out */
    QMI_FOTA_TIME_OUT                      = 0x00000003,
    /** Binary update failed */
    QMI_FOTA_BINARY_UPDATE_FAILED          = 0x00000008,
    /** Binary download failed */
    QMI_FOTA_BINARY_DOWNLOAD_FAILED        = 0x00000009,
    /** Invalid delta descriptor file */
    QMI_FOTA_INVALID_DELTA_DESCRIPTOR_FILE = 0x0000000A,
    /** Server Info not available */
    QMI_FOTA_SERVER_INFO_UNAVAILABLE       = 0x00001001,
    /** Invalid delta descriptor */
    QMI_FOTA_INVALID_DELTA_DESCRIPTOR      = 0x00001002,
    /** Invalid data */
    QMI_FOTA_INVALID_DATA                  = 0xffffffff,
} QMI_FOTA_session_response_t;

/**
 * @brief output type for QMI_FOTA_OEM_FTP_START_SESSION request
 *
 */
typedef struct {
    /** Session Response. OMA-DM availability CHECK
     * for the requested item, see #QMI_FOTA_session_response_t */
    uint32_t session_response;
} fota_session_status_t;

/**
 * @brief Pack QMI_FOTA_OEM_FTP_START_SESSION (66 - 0x0042) request
 *
 * This function packs a qmi request for starting client-initiated session
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #fota_session_type_t
 */
QMI_SDK_error_t telit_fota_oem_ftp_start_session_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_session_type_t *input);

/**
 * @brief Unpack QMI_FOTA_OEM_FTP_START_SESSION (66 - 0x0042) request
 *
 * This function unpacks the result of a QMI_FOTA_OEM_FTP_START_SESSION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #fota_session_status_t
 */
QMI_SDK_error_t telit_fota_oem_ftp_start_session_unpack(uint8_t *rsp, uint16_t len,
        fota_session_status_t *out);

/**
 * @brief Pack QMI_FOTA_OEM_FTP_STOP_SESSION (67 - 0x0043) request
 *
 * This function packs a qmi request for stopping client-initiated session
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #fota_session_type_t
 */
QMI_SDK_error_t telit_fota_oem_ftp_stop_session_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_session_type_t *input);

/**
 * @brief Unpack QMI_FOTA_OEM_FTP_STOP_SESSION (67 - 0x0043) request
 *
 * This function unpacks the result of a QMI_FOTA_OEM_FTP_STOP_SESSION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_fota_oem_ftp_stop_session_unpack(uint8_t *rsp, uint16_t len);

#define QMI_FOTA_RETRY_TIMERS 8
#define QMI_FOTA_MAX_APN_STRING_SIZE (49 + 1)
#define QMI_FOTA_MAX_USERNAME_STRING_SIZE (29 + 1)
#define QMI_FOTA_MAX_PASSWORD_STRING_SIZE QMI_FOTA_MAX_USERNAME_STRING_SIZE

/**
 * @brief QMI FOTA period_info type
 *
 */
typedef struct {
    /** Min period */
    uint32_t u_min;
    /** Max period */
    uint32_t u_max;
} fota_period_info_t;

/**
 * @brief input type for QMI_FOTA_OEM_FTP_SET_SETTINGS request
 *
 */
typedef struct {
    /** Firmware Auto Download
     *
     * Values:
       - 0x00 – FALSE
       - 0x01 – TRUE */
    uint8_t auto_download_firmware;
    /** indicating FOTA Automatic update
     *
     * Values:
       - 0x00 – Firmware auto update FALSE
       - 0x01 – Firmware auto update TRUE (if this value is set,
     *        the device is automatically rebooted) */
    uint8_t auto_update_firmware;
    /** set if polling_time is available */
    bool polling_time_available;
    /** Polling timer to connect to AVMS server
     * Values from 0 to 525600 (min).
     * 0 means disabled */
    uint32_t polling_time;
    /** set if fw_auto_sdm is available */
    bool fw_auto_sdm_available;
    /** OMA Automatic UI Alert Response (NOT supported) */
    uint8_t fw_auto_sdm;
    /** set if auto_connect is available */
    bool auto_connect_available;
    /** Auto connect Response (NOT supported) */
    uint8_t auto_connect;
    /** set if auto_reboot is available */
    bool auto_reboot_available;
    /** Auto reboot Response (NOT supported)
    * If auto_update_firmware parameter is set,
    * the device is automatically rebooted*/
    uint8_t auto_reboot;
    /** set if connection_retry_timers is available */
    bool connection_retry_timers_available;
    /** Connection Retry Timers (NOT supported) */
    uint16_t connection_retry_timers[QMI_FOTA_RETRY_TIMERS];
    /** length of str_apn string */
    uint8_t apn_len;
    /** APN Info (NOT supported) */
    char apn[QMI_FOTA_MAX_APN_STRING_SIZE];
    /** length of username string */
    uint8_t username_len;
    /** User Name (NOT supported) */
    char username[QMI_FOTA_MAX_USERNAME_STRING_SIZE];
    /** length of password string */
    uint16_t password_len;
    /** Password (NOT supported) */
    char password[QMI_FOTA_MAX_PASSWORD_STRING_SIZE];
    /** set if notif_store is available */
    bool notif_store_available;
    /** NotifStore (NOT supported) */
    uint8_t notif_store;
    /** set if period_info is available */
    bool period_info_available;
    /** Period Info (NOT supported) */
    fota_period_info_t period_info;
    /** set if oma_dm_enabled is available */
    bool oma_dm_enabled_available;
    /** OMA DM Enabeld.
     * This filed is used in QMI_FOTA_OEM_FTP_GET_SETTINGS only.
     * (NOT supported. This interface supports fota using only
     * FTP server) */
    uint32_t oma_dm_enabled;
} fota_settings_t;

/**
 * @brief Pack QMI_FOTA_OEM_FTP_SET_SETTINGS (68 - 0x0044) request
 *
 * This function packs a qmi request for setting ftp server
 * information before starting client-initiated session
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #fota_settings_t
 */
QMI_SDK_error_t telit_fota_oem_ftp_set_settings_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_settings_t *input);

/**
 * @brief Unpack QMI_FOTA_OEM_FTP_SET_SETTINGS (68 - 0x0044) request
 *
 * This function unpacks the result of a QMI_FOTA_OEM_FTP_SET_SETTINGS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_fota_oem_ftp_set_settings_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_FOTA_OEM_FTP_GET_SETTINGS (69 - 0x0045) request
 *
 * This function packs a qmi request for for getting ftp server information
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_fota_oem_ftp_get_settings_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_FOTA_OEM_FTP_GET_SETTINGS (69 - 0x0045) request
 *
 * This function unpacks the result of a QMI_FOTA_OEM_FTP_GET_SETTINGS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #fota_settings_t
 */
QMI_SDK_error_t telit_fota_oem_ftp_get_settings_unpack(uint8_t *rsp, uint16_t len,
        fota_settings_t *out);

/**
 * @brief input type for QMI_FOTA_OEM_FTP_SET_EVENT_REPORT request
 *
 */
typedef struct {
    uint8_t enable_event;  /* Enable event
                            * Value:
                            * 0x00 – disable event report
                            * 0x01 – enable event report */
} fota_set_event_report_t;

/**
 * @brief Pack QMI_FOTA_OEM_FTP_SET_EVENT_REPORT (70 - 0x0046) request
 *
 * This function packs a qmi request for setting the OEM_FTP_FOTA notifications.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #fota_set_event_report_t
 */
QMI_SDK_error_t telit_fota_oem_ftp_set_event_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_set_event_report_t *input);

/**
 * @brief Unpack QMI_FOTA_OEM_FTP_SET_EVENT_REPORT (70 - 0x0046) request
 *
 * This function unpacks the result of a QMI_FOTA_OEM_FTP_SET_EVENT_REPORT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_fota_oem_ftp_set_event_report_unpack(uint8_t *rsp, uint16_t len);

#define QMI_FOTA_MAX_REJECT_REASON_SIZE (100 + 1)

/**
 * @brief QMI FOTA user selection values
 *
 */
typedef enum {
    /** Accept */
    QMI_FOTA_SELECTION_ACCEPT = 0x01,
    /** Reject */
    QMI_FOTA_SELECTION_REJECT = 0x02,
    /** Defer */
    QMI_FOTA_SELECTION_DEFER  = 0x03,
} QMI_FOTA_user_selection_t;

/**
 * @brief input type for QMI_FOTA_OEM_FTP_SEND_SELECTION request
 *
 */
typedef struct {
    /** User Selection, see #QMI_FOTA_user_selection_t */
    uint8_t selection;
    /** set if defer_time is available */
    bool defer_time_available;
    /** Defer time in minutes
     * A value of 0 will cause the prompt to be resent
     * immediately */
    uint32_t defer_time;
    /** reject_reason length */
    uint8_t reject_reason_len;
    /** Reject Reason */
    char reject_reason[QMI_FOTA_MAX_REJECT_REASON_SIZE];
    /** set if client_perform_operation is available */
    bool client_perform_operation_available;
    /** Client operation flag after accept
     * (NOT supported since we do not have enough
     * information about this flag)
     *
     * Values:
       - 0x00 – if modem performs the operation
       - 0x01 – if client performs the operatio) */
    uint8_t client_perform_operation;
    /** set if package_id is available */
    bool package_id_available;
    /** Package ID
     * (NOT supported since we do not have enough
     * information about this flag) */
    uint8_t package_id;
} fota_selection_t;

/**
 * @brief Pack QMI_FOTA_OEM_FTP_SEND_SELECTION (71 - 0x0047) request
 *
 * This function packs a qmi request for sending user selection command
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #fota_selection_t
 */
QMI_SDK_error_t telit_fota_oem_ftp_send_selection_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_selection_t *input);

/**
 * @brief Unpack QMI_FOTA_OEM_FTP_SEND_SELECTION (71 - 0x0047) request
 *
 * This function unpacks the result of a QMI_FOTA_OEM_FTP_SEND_SELECTION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_fota_oem_ftp_send_selection_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI FOTA OEM ATT Host ODIS information
 */
typedef struct {
    uint8_t man_string_len;
    char man_string[MAX_STRING_SIZE + 1];
    uint8_t model_string_len;
    char model_string[MAX_STRING_SIZE + 1];
    uint8_t sw_ver_string_len;
    char sw_ver_string[MAX_STRING_SIZE + 1];
    uint8_t plasma_id_string_len;
    char plasma_id_string[MAX_STRING_SIZE + 1];
} fota_host_odis_t;

/**
 * @brief Pack QMI_FOTA_OEM_ATT_GET_HOST_ODIS (73 - 0x0049) request
 *
 * Get information related to the current host ODIS
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_fota_oem_att_get_host_odis_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_FOTA_OEM_ATT_GET_HOST_ODIS (73 - 0x0049) request
 *
 * This function unpacks the result of a QMI_FOTA_OEM_ATT_GET_HOST_ODIS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #fota_host_odis_t
 */
QMI_SDK_error_t telit_fota_oem_att_get_host_odis_unpack(uint8_t *rsp, uint16_t len, fota_host_odis_t *out);

/**
 * @brief Pack QMI_FOTA_OEM_ATT_SET_HOST_ODIS (74 - 0x004A) request
 *
 * Set information related to the current host ODIS to NVM and OMA-DM application
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #fota_host_odis_t
 */
QMI_SDK_error_t telit_fota_oem_att_set_host_odis_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        fota_host_odis_t *input);

/**
 * @brief Unpack QMI_FOTA_OEM_ATT_SET_HOST_ODIS (74 - 0x004A) request
 *
 * This function unpacks the result of a QMI_FOTA_OEM_ATT_SET_HOST_ODIS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_fota_oem_att_set_host_odis_unpack(uint8_t *rsp, uint16_t len);

#define QMI_FOTA_MAX_VERSION_STRING_SIZE (128 + 1)
#define QMI_FOTA_MAX_DESCRIPTION_STRING_SIZE (1024 + 1)

/**
 * @brief QMI FOTA binary types
 *
 */
typedef enum {
    /** Firmware */
    QMI_FOTA_FIRMWARE    = 1,
    /** User App (NOT supported) */
    QMI_FOTA_USER_APP    = 2,
    /** Legato Framework (NOT supported) */
    QMI_FOTA_USER_LEGATO = 3,
} QMI_FOTA_binary_type_t;

/**
 * @brief QMI FOTA session states
 *
 */
typedef enum {
    /** No binary update available */
    QMI_FOTA_STATE_NO_UPDATE_AVAILABLE           = 0x01,
    /** Query binary Download */
    QMI_FOTA_STATE_QUERY_DOWNLOAD                = 0x02,
    /** Binary Downloading */
    QMI_FOTA_STATE_DOWNLOADING                   = 0x03,
    /** Binary downloaded */
    QMI_FOTA_STATE_DOWNLOADED                    = 0x04,
    /** Query Binary Update */
    QMI_FOTA_STATE_QUERY_UPDATE                  = 0x05,
    /** Binary updating */
    QMI_FOTA_STATE_UPDATING                      = 0x06,
    /** Binary updated */
    QMI_FOTA_STATE_UPDATED                       = 0x07,
    /** Binary update failed */
    QMI_FOTA_STATE_UPDATE_FAILED                 = 0x08,
    /** Binary download failed */
    QMI_FOTA_STATE_DOWNLOAD_FAILED               = 0x09,
    /** Invalid delta descriptor file */
    QMI_FOTA_STATE_INVALID_DELTA_DESCRIPTOR_FILE = 0x0A,
} QMI_FOTA_binary_session_state_t;

/**
 * @brief QMI FOTA user input requests
 *
 */
typedef enum {
    /** No user input required, informational indication. */
    QMI_FOTA_USER_NO_INPUT = 0x01,
    /** Accept */
    QMI_FOTA_USER_ACCEPT   = 0x02,
    /** Reject */
    QMI_FOTA_USER_REJECT   = 0x03,
} QMI_FOTA_user_input_req_t;

/**
 * @brief QMI FOTA session info type
 *
 */
typedef struct {
    /** Binary Type, see #QMI_FOTA_binary_type_t */
    uint8_t binary_type;
    /** State, see #QMI_FOTA_binary_session_state_t */
    uint8_t state;
    /** Bit mask of available user inputs, see #QMI_FOTA_user_input_req_t */
    uint8_t user_input_request;
    /** Timeout for user input in minutes.
     * A value of 0 means no time-out. */
    uint16_t user_input_timeout;
    /** The size (in bytes) of the update package. */
    uint32_t package_size;
    /** The number of bytes being downloaded.
     * For downloading state, this value shall be > 0 and
     * incremented toward the package_size.
     * For other states, the value shall be 0 as it is meaningless. */
    uint32_t downloaded_size;
    /** Update status */
    uint16_t update_complete_status;
    /** field currently NOT supported */
    uint8_t severity;
} fota_update_info_t;

/**
 * @brief QMI FOTA connection request type
 *
 */
typedef struct {
    /** Bit mask of available user inputs, see #QMI_FOTA_user_input_req_t */
    uint8_t user_input_request;
    /** Timeout for user input in minutes.
     * A value of 0 means no time-out. */
    uint16_t user_input_timeout;
} fota_connection_request_t;

/**
 * @brief QMI FOTA session response values
 *
 */
typedef enum {
    /** Session succeeded */
    QMI_FOTA_SESSION_SUCCEEDED = 0x0000,
    /** Session cancelled */
    QMI_FOTA_SESSION_CANCELLED = 0x0005,
    /** Failed to download FOTA image */
    QMI_FOTA_SESSION_FAILED    = 0x7F14,
} QMI_FOTA_session_state_t;

/**
 * @brief output type for QMI_FOTA_OEM_FTP_EVENT_REPORT_INDICATOR indication
 *
 */
typedef struct {
    /** See session_info_t */
    fota_update_info_t update_info;
    /** length of str_version string */
    uint8_t version_len;
    /** FW Version string in ASCII */
    char version[QMI_FOTA_MAX_VERSION_STRING_SIZE];
    /** length of name string */
    uint8_t name_len;
    /** Package Name in UCS2 */
    char name[QMI_FOTA_MAX_VERSION_STRING_SIZE];
    /** length of description string */
    uint16_t description_len;
    /** Description of Update Package in ASCII */
    char description[QMI_FOTA_MAX_DESCRIPTION_STRING_SIZE];
    /** set if session_status is available */
    bool session_status_available;
    /** This field will set to the session status
     * for notifications that occur at the end of a session,
     * zero for all other notifications, see #QMI_FOTA_session_state_t */
    uint16_t session_status;
    /** set if connection_request is available */
    bool connection_request_available;
    /** Only used in indication, see fota_connection_request_t */
    fota_connection_request_t connection_request;
    /** set if update_package_status is available */
    bool update_package_status_available;
    /** This is used as a response event to the
     * QMI_FOTA_OEM_FTP_START_SESSION_REQ message, see #QMI_FOTA_session_response_t */
    uint32_t update_package_status;
} fota_session_info_t;

/**
 * @brief Pack QMI_FOTA_OEM_FTP_GET_SESSION_INFO (75 - 0x004B) request
 *
 * This function packs a qmi request for getting current FOTA session information
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #fota_session_type_t
 */
QMI_SDK_error_t telit_fota_get_session_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        fota_session_type_t *input);

/**
 * @brief Unpack QMI_FOTA_OEM_FTP_GET_SESSION_INFO (75 - 0x004B) request
 *
 * This function unpacks the result of a QMI_FOTA_OEM_FTP_GET_SESSION_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #fota_session_info_t
 */
QMI_SDK_error_t telit_fota_get_session_info_unpack(uint8_t *rsp, uint16_t len,
        fota_session_info_t *out);

/**
 * @brief Unpack fota indications
 *
 * This function unpacks the result of the following fota indications
 *
 * QMI_FOTA_OEM_FTP_EVENT_REPORT_INDICATOR (0x0048)
 *
 * @param[in] msg_id indication id
 * @param[in] rsp indication
 * @param[in] len indication length
 * @param[out] out type according to the following
 *      QMI_FOTA_OEM_FTP_EVENT_REPORT_INDICATOR (0x0048) (2) -> #fota_session_info_t
 * @param[in] out_len buffer size
 */
QMI_SDK_error_t telit_fota_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len,
        uint8_t *out, uint16_t out_len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_FOTA_H_ */
