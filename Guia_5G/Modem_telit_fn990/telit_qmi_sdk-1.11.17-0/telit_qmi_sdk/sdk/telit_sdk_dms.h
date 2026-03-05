#ifndef TELIT_SDK_DMS_H_
#define TELIT_SDK_DMS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "telit_sdk.h"

/**
 * @file telit_sdk_dms.h
 * @brief DMS pack/unpack functions
 * Service ID: QMI_SVC_DMS (2)
 */

/**
 * @brief Enum to describe QMI DMS Message IDs
 */
typedef enum {
    /** 01 - 0x0001 Enable events report */
    QMI_DMS_SET_EVENT_REPORT                        = 0x0001,
    /** 01 - 0x0001 Events report indication */
    QMI_DMS_EVENT_IND                               = 0x0001,

    /** 32 - 0x0020 Get device capabilities */
    QMI_DMS_GET_DEVICE_CAP                          = 0x0020,
    /** 33 - 0x0021 Get device manufacturer */
    QMI_DMS_GET_DEVICE_MFR                          = 0x0021,
    /** 34 - 0x0022 Get device model ID */
    QMI_DMS_GET_DEVICE_MODEL_ID                     = 0x0022,
    /** 35 - 0x0023 Get device revision ID */
    QMI_DMS_GET_DEVICE_REV_ID                       = 0x0023,
    /** 36 - 0x0024 Get assigned voice number */
    QMI_DMS_GET_MSISDN                              = 0x0024,
    /** 37 - 0x0025 Get ESN/IMEI/MEID */
    QMI_DMS_GET_DEVICE_SERIAL_NUMBERS               = 0x0025,

    /** 44 - 0x002C Get hardware revision of the device */
    QMI_DMS_GET_DEVICE_HARDWARE_REV                 = 0x002C,
    /** 45 - 0x002D Get operating mode */
    QMI_DMS_GET_OPERATING_MODE                      = 0x002D,
    /** 46 - 0x002E Set operating mode */
    QMI_DMS_SET_OPERATING_MODE                      = 0x002E,
    /** 47 - 0x002F Get timestamp from the device */
    QMI_DMS_GET_TIME                                = 0x002F,

    /** 69 - 0x0045 Get device band capability */
    QMI_DMS_GET_BAND_CAPABILITY                     = 0x0045,

    /** 94 - 0x005e Trigger the modem activity information calculation */
    QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION = 0x005e,
    /** 95 - 0x005f Query for the modem activity statistics */
    QMI_DMS_GET_MODEM_ACTIVITY_INFO                 = 0x005f,
    /** 95 - 0x005f Query for the modem activity statistics indication */
    QMI_DMS_MODEM_ACTIVITY_INFO_IND                 = 0x005f,

    /** 115 - 0x0073 Notify the modem about the power information on the UE */
    QMI_DMS_DEVICE_POWER_INFO                       = 0x0073,

    /** 21852 - 0x555c Enable AT commands response indication (UNDOCUMENTED) */
    QMI_DMS_SET_AT_IND_REPORT                       = 0x555c,
    /** 21853 - 0x555d Send AT command (UNDOCUMENTED) */
    QMI_DMS_SEND_AT_COMMAND                         = 0x555d,
    /** 21853 - 0x555d AT command answer indication (UNDOCUMENTED) */
    QMI_DMS_AT_COMMAND_IND                          = 0x555d,
} QMI_DMS_message_t;

/**
 * @brief input type for QMI_DMS_SET_EVENT_REPORT
 *
 */
typedef struct {
    /** Action for operating mode event, see #QMI_indication_action_t */
    QMI_indication_action_t operating_mode;
} dms_set_event_report_info_t;

/**
 * @brief Pack QMI_DMS_SET_EVENT_REPORT (1 - 0x0001) request
 *
 * This function packs a qmi request for setting the device management
 * state reporting conditions for the requesting control point.
 *
 * The following event reportings can be activated:
 *
 * Operating mode
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #dms_set_event_report_info_t
 */
QMI_SDK_error_t telit_dms_set_event_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        dms_set_event_report_info_t *input);

/**
 * @brief Unpack QMI_DMS_SET_EVENT_REPORT (1 - 0x0001) request
 *
 * This function unpacks the result of a QMI_DMS_SET_EVENT_REPORT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_dms_set_event_report_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI DMS Data Service Capabilities
 *
 */
typedef enum {
    /** No data services supported */
    QMI_DMS_DATA_CAP_NONE               = 0x00,
    /** Only circuit-switched data services supported */
    QMI_DMS_DATA_CAP_CS_ONLY            = 0x01,
    /** Only packet-switched data services supported */
    QMI_DMS_DATA_CAP_PS_ONLY            = 0x02,
    /** Simultaneous circuit-switched and packet-switched supported */
    QMI_DMS_DATA_CAP_SIMUL_CS_AND_PS    = 0x03,
    /** Non-simultaneous circuit-switched and packet-switched supported */
    QMI_DMS_DATA_CAP_NONSIMUL_CS_AND_PS = 0x04,
} QMI_DMS_data_service_capability_t;

/**
 * @brief QMI DMS Sim Capabilities
 *
 */
typedef enum {
    /** SIM is not supported */
    QMI_DMS_SIM_NOT_SUPPORTED = 0x01,
    /** SIM is supported */
    QMI_DMS_SIM_SUPPORTED     = 0x02,
} QMI_DMS_sim_capability_t;

/**
 * @brief QMI DMS Radio Interface types
 *
 */
typedef enum {
    /** CDMA2000 1X */
    QMI_DMS_RADIO_IF_1X      = 0x01,
    /** CDMA2000 HRPD (1xEV-DO) */
    QMI_DMS_RADIO_IF_1x_EVDO = 0x02,
    /** GSM */
    QMI_DMS_RADIO_IF_GSM     = 0x04,
    /** UMTS */
    QMI_DMS_RADIO_IF_UMTS    = 0x05,
    /** LTE */
    QMI_DMS_RADIO_IF_LTE     = 0x08,
    /** TDS */
    QMI_DMS_RADIO_IF_TDS     = 0x09,
    /** NR5G */
    QMI_DMS_RADIO_IF_NR5G    = 0x0A,
} QMI_DMS_radio_if_t;

#define QMI_DMS_DEVICE_CAPS_MAX_RADIO_IF 255
/**
 * @brief output type for QMI_DMS_GET_DEVICE_CAP request
 *
 */
typedef struct {
    /** Maximum Tx transmission rate in bits per second (bps) supported by the device. */
    uint32_t tx_rate;
    /** Maximum Rx transmission rate in bits per second (bps) supported by the device. */
    uint32_t rx_rate;
    /** Data service capabilities, see #QMI_DMS_data_service_capability_t */
    enum8_t data_svc_caps;
    /** Sim capabilities, see #QMI_DMS_sim_capability_t */
    enum8_t sim_caps;
    /** size of radio_interfaces */
    uint8_t radio_interfaces_size;
    /** Array of radio interfaces, see #QMI_DMS_radio_if_t */
    enum8_t radio_interfaces[QMI_DMS_DEVICE_CAPS_MAX_RADIO_IF];
} dms_device_caps_t;

/**
 * @brief Pack QMI_DMS_GET_DEVICE_CAP (32 - 0x0020) request
 *
 * This function packs a qmi request for requesting the device
 * capabilities
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_dms_get_device_cap_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_DMS_GET_DEVICE_CAP (32 - 0x0020) request
 *
 * This function unpacks the result of a QMI_DMS_GET_DEVICE_CAP request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #dms_device_caps_t
 */
QMI_SDK_error_t telit_dms_get_device_cap_unpack(uint8_t *rsp, uint16_t len, dms_device_caps_t *out);

/**
 * @brief output type for QMI_DMS_GET_DEVICE_MFR request
 *
 */
typedef struct {
    /** Manufacturer string length */
    uint8_t manufacturer_len;
    /** String with manufacturer */
    char manufacturer[MAX_STRING_SIZE];
} dms_get_manufacturer_t;

/**
 * @brief Pack QMI_DMS_GET_DEVICE_MFR (33 - 0x0021) request
 *
 * This function packs a qmi request for requesting the device
 * manufacturer
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_dms_get_device_mfr_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_DMS_GET_DEVICE_MFR (33 - 0x0021) request
 *
 * This function unpacks the result of a QMI_DMS_GET_DEVICE_MFR request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #dms_get_manufacturer_t
 */
QMI_SDK_error_t telit_dms_get_device_mfr_unpack(uint8_t *rsp, uint16_t len, dms_get_manufacturer_t *out);

/**
 * @brief output type for QMI_DMS_GET_DEVICE_MODEL_ID request
 *
 */
typedef struct {
    /** Model ID string length */
    uint8_t model_id_len;
    /** String with model id */
    char model_id[MAX_STRING_SIZE];
} dms_get_model_id_t;

/**
 * @brief Pack QMI_DMS_GET_DEVICE_MODEL_ID (34 - 0x0022) request
 *
 * This function packs a qmi request for requesting the device
 * model identification
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_dms_get_device_model_id_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_DMS_GET_DEVICE_MODEL_ID (34 - 0x0022) request
 *
 * This function unpacks the result of a QMI_DMS_GET_DEVICE_MODEL_ID request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #dms_get_model_id_t
 */
QMI_SDK_error_t telit_dms_get_device_model_id_unpack(uint8_t *rsp, uint16_t len, dms_get_model_id_t *out);

/**
 * @brief output type for QMI_DMS_GET_DEVICE_REV_ID
 *
 */
typedef struct {
    /** AMSS string length */
    uint8_t amss_len;
    /** boot string length */
    uint8_t boot_len;
    /** pri string length */
    uint8_t pri_len;
    /** String with AMSS revision */
    char amss[MAX_STRING_SIZE];
    /** String with boot revision */
    char boot[MAX_STRING_SIZE];
    /** String with PRI revision */
    char pri[MAX_STRING_SIZE];
} dms_get_fw_revision_t;

/**
 * @brief Pack QMI_DMS_GET_DEVICE_REV_ID (35 - 0x0023) request
 *
 * This function packs a qmi request for requesting the device
 * firmware revision identification
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_dms_get_device_rev_id_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_DMS_GET_DEVICE_REV_ID (35 - 0x0023) request
 *
 * This function unpacks the result of a QMI_DMS_GET_DEVICE_REV_ID request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #dms_get_fw_revision_t
 */
QMI_SDK_error_t telit_dms_get_device_rev_id_unpack(uint8_t *rsp, uint16_t len, dms_get_fw_revision_t *out);

/**
 * @brief output type for QMI_DMS_GET_MSISDN request
 *
 */
typedef struct {
    /** Voice number string length */
    uint8_t voice_number_len;
    /** Mobile ID number string length */
    uint8_t mobile_id_len;
    /** String containing the voice number of the device */
    char voice_number[MAX_STRING_SIZE];
    /** String containing the mobile ID number of the device */
    char mobile_id[MAX_STRING_SIZE];
} dms_voice_number_info_t;

/**
 * @brief Pack QMI_DMS_GET_MSISDN (36 - 0x0024) request
 *
 * This function packs a qmi request for requesting the voice
 * number of the device
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_dms_get_msisdn_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_DMS_GET_MSISDN (36 - 0x0024) request
 *
 * This function unpacks the result of a QMI_DMS_GET_MSISDN request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #dms_voice_number_info_t
 */
QMI_SDK_error_t telit_dms_get_msisdn_unpack(uint8_t *rsp, uint16_t len, dms_voice_number_info_t *out);

/**
 * @brief output type for QMI_DMS_GET_DEVICE_SERIAL_NUMBERS request
 *
 */
typedef struct {
    /** ESN string length */
    uint8_t esn_len;
    /** IMEI string length */
    uint8_t imei_len;
    /** MEID string length */
    uint8_t meid_len;
    /** IMEI software version string length */
    uint8_t imei_svn_len;
    /** String containing the ESN of the device. */
    char esn[MAX_STRING_SIZE];
    /** String containing the IMEI of the device. */
    char imei[MAX_STRING_SIZE];
    /** String containing the MEID of the device. */
    char meid[MAX_STRING_SIZE];
    /** String containing the IMEI software version number */
    char imei_svn[MAX_STRING_SIZE];
} dms_get_serial_numbers_t;

/**
 * @brief Pack QMI_DMS_GET_DEVICE_SERIAL_NUMBERS (37 - 0x0025) request
 *
 * This function packs a qmi request for requesting the serial
 * numbers of the device
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_dms_get_device_serial_numbers_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_DMS_GET_DEVICE_SERIAL_NUMBERS (37 - 0x0025) request
 *
 * This function unpacks the result of a QMI_DMS_GET_DEVICE_SERIAL_NUMBERS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #dms_get_serial_numbers_t
 */
QMI_SDK_error_t telit_dms_get_device_serial_numbers_unpack(uint8_t *rsp, uint16_t len,
        dms_get_serial_numbers_t *out);

/**
 * @brief output type for QMI_DMS_GET_DEVICE_HARDWARE_REV requests
 *
 */
typedef struct {
    /** revision string length */
    uint8_t revision_len;
    /** String with revision */
    char revision[MAX_STRING_SIZE];
} dms_get_revision_t;

/**
 * @brief Pack QMI_DMS_GET_DEVICE_HARDWARE_REV (44 - 0x002C) request
 *
 * This function packs a qmi request for requesting the device
 * hardware revision identification
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_dms_get_device_hardware_rev_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief QMI_DMS_GET_DEVICE_HARDWARE_REV (44 - 0x002C) request
 *
 * This function unpacks the result of a QMI_DMS_GET_DEVICE_HARDWARE_REV request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #dms_get_revision_t
 */
QMI_SDK_error_t telit_dms_get_device_hardware_rev_unpack(uint8_t *rsp, uint16_t len, dms_get_revision_t *out);

/**
 * @brief QMI DMS Operating Modes
 *
 */
typedef enum {
    /** Online */
    QMI_DMS_OP_MODE_ONLINE               = 0x00,
    /** Low power */
    QMI_DMS_OP_MODE_LOW_POWER            = 0x01,
    /** Factory test mode */
    QMI_DMS_OP_MODE_FACTORY_TEST         = 0x02,
    /** Offline */
    QMI_DMS_OP_MODE_OFFLINE              = 0x03,
    /** Resetting */
    QMI_DMS_OP_MODE_RESETTING            = 0x04,
    /** Shutting down */
    QMI_DMS_OP_MODE_SHUTTING             = 0x05,
    /** Persistent low power */
    QMI_DMS_OP_MODE_PERSISTENT_LOW_POWER = 0x06,
    /** Mode-only low power */
    QMI_DMS_OP_MODE_ONLY_LOW_POWER       = 0x07,
    /** Conducting network test for GSM/WCDMA */
    QMI_DMS_OP_MODE_NET_TEST_GW          = 0x08,
    /** Camp only */
    QMI_DMS_OP_MODE_CAMP_ONLY            = 0x09,
} QMI_DMS_operating_mode_t;

/**
 * @brief QMI DMS Operating Modes Offline Reasons
 *
 */
typedef enum {
    /** Host image misconfiguration */
    QMI_DMS_HOST_IMAGE_MISCONFIGURATION = 0x0001,
    /** PRI image misconfiguration */
    QMI_DMS_PRI_IMAGE_MISCONFIGURATION  = 0x0002,
    /** PRI version incompatible */
    QMI_DMS_PRI_VERSION_INCOMPATIBLE    = 0x0004,
    /** Device memory is full, cannot copy PRI information */
    QMI_DMS_DEVICE_MEMORY_FULL          = 0x0008,
} QMI_DMS_offline_reason_t;

/**
 * @brief input type for QMI_DMS_SET_OPERATING_MODE, output type for QMI_DMS_GET_OPERATING_MODE request
 *
 */
typedef struct {
    /** Operating mode, see #QMI_DMS_operating_mode_t */
    enum8_t power_mode;
    /** set if offline_reason is available **/
    bool offline_reason_available;
    /** Offline reason, see #QMI_DMS_offline_reason_t: valid only for QMI_DMS_GET_OPERATING_MODE */
    enum16_t offline_reason;
    /** Hardware-restricted mode: valid only for QMI_DMS_GET_OPERATING_MODE */
    bool hw_controlled_mode;
} dms_power_info_t;

/**
 * @brief Pack QMI_DMS_GET_OPERATING_MODE (45 - 0x002D) request
 *
 * This function packs a qmi request for requesting the operating
 * mode of the device
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_dms_get_operating_mode_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_DMS_GET_OPERATING_MODE (45 - 0x002D) request
 *
 * This function unpacks the result of a QMI_DMS_GET_OPERATING_MODE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #dms_power_info_t
 */
QMI_SDK_error_t telit_dms_get_operating_mode_unpack(uint8_t *rsp, uint16_t len, dms_power_info_t *out);

/**
 * @brief Pack QMI_DMS_SET_OPERATING_MODE (46 - 0x002E) request
 *
 * This function packs a qmi request for setting the operating
 * mode of the device
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #dms_power_info_t
 *
 */
QMI_SDK_error_t telit_dms_set_operating_mode_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        dms_power_info_t *input);

/**
 * @brief Unpack QMI_DMS_SET_OPERATING_MODE (46 - 0x002E) request
 *
 * This function unpacks the result of a QMI_DMS_SET_OPERATING_MODE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_dms_set_operating_mode_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI DMS time sources
 *
 */
typedef enum {
    /** 32 kHz device clock */
    QMI_DMS_TIME_SOURCE_DEVICE_CLOCK = 0x00,
    /** CDMA network */
    QMI_DMS_TIME_SOURCE_CDMA         = 0x01,
    /** HDR network */
    QMI_DMS_TIME_SOURCE_HDR          = 0x02,
} QMI_DMS_time_source_t;

/**
 * @brief output type for QMI_DMS_GET_TIME request
 *
 */
typedef struct {
    /** Count of 1.25 ms that have elapsed from the start of GPS Epoch
     * time (January 6, 1980). A 6-byte integer  in little-endian format */
    uint64_t time_count;
    /** Source of the timestamp, see #QMI_DMS_time_source_t */
    enum16_t time_source;
    /** true if sys_time_in_ms is set */
    bool sys_time_in_ms_available;
    /** Count of system time in milliseconds that have elapsed from the start of GPS Epoch time (Jan 6, 1980) */
    uint64_t sys_time_in_ms;
    /** true if user_time_in_ms is set */
    bool user_time_in_ms_available;
    /** Count of user time in milliseconds that have elapsed from the start of GPS Epoch time (Jan 6, 1980) */
    uint64_t user_time_in_ms;
} dms_get_time_t;

/**
 * @brief Pack QMI_DMS_GET_TIME (47 - 0x002F) request
 *
 * This function packs a qmi request for querying the current time
 * of the device.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_dms_get_time_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_DMS_GET_TIME (47 - 0x002F) request
 *
 * This function unpacks the result of a QMI_DMS_GET_TIME request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #dms_get_time_t
 */
QMI_SDK_error_t telit_dms_get_time_unpack(uint8_t *rsp, uint16_t len, dms_get_time_t *out);

#define QMI_DMS_GET_BAND_CAPS_MAX_SUPPORTED_LTE_BANDS 255
#define QMI_DMS_GET_BAND_CAPS_MAX_SUPPORTED_NR5G_BANDS 255

/**
 * @brief QMI DMS TDS band capability
 *
 */
typedef enum {
    /** TDS Band A 1900 to 1920 Mhz, 2010 to 2020 Mhz */
    QMI_DMS_TDS_BAND_1900_1920_2010_2020 = (1 << 0),
    /** TDS Band B 1850 to 1910 Mhz, 1930 to 1990 Mhz */
    QMI_DMS_TDS_BAND_1850_1910_1930_1990 = (1 << 1),
    /** TDS Band C 1910 to 1930 Mhz */
    QMI_DMS_TDS_BAND_1910_1930           = (1 << 2),
    /** TDS Band D 2570 to 2620 Mhz */
    QMI_DMS_TDS_BAND_2570_2620           = (1 << 3),
    /** TDS Band E 2300 to 2400 Mhz */
    QMI_DMS_TDS_BAND_2300_2400           = (1 << 4),
    /** TDS Band F 1880 to 1920 Mhz */
    QMI_DMS_TDS_BAND_1880_1920           = (1 << 5),
} QMI_DMS_tds_band_capability_t;

/**
 * @brief output type for QMI_DMS_GET_BAND_CAPABILITY request
 *
 */
typedef struct {
    /** Bitmask of bands supported by the device, see QMI_BAND_* constants */
    uint64_t band_capabilities;
    /** Indicate if TDS band capabilities is available */
    bool tds_band_capabilities_available;
    /** Bitmask of TDS bands supported by the device, see #QMI_DMS_tds_band_capability_t */
    uint64_t tds_band_capabilities;
    /** Number of lte bands supported by the device */
    uint16_t supported_lte_bands_capabilities_size;
    /** Array of supported LTE bands where each entry is a decimal representation of the LTE band number supported */
    uint16_t supported_lte_bands[QMI_DMS_GET_BAND_CAPS_MAX_SUPPORTED_LTE_BANDS];
    /** Number of nr5g bands supported by the device */
    uint16_t supported_nr5g_bands_capabilities_size;
    /** Array of supported NR5G bands where each entry is a decimal representation of the NR5G band number supported */
    uint16_t supported_nr5g_bands[QMI_DMS_GET_BAND_CAPS_MAX_SUPPORTED_NR5G_BANDS];
} dms_band_capabilities_t;

/**
 * @brief Pack QMI_DMS_GET_BAND_CAPABILITY (69 - 0x0045) request
 *
 * This function packs a qmi request for requesting device
 * bands capabilities
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_dms_get_band_capability_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_DMS_GET_BAND_CAPABILITY (69 - 0x0045) request
 *
 * This function unpacks the result of a QMI_DMS_GET_BAND_CAPABILITY request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #dms_band_capabilities_t
 */
QMI_SDK_error_t telit_dms_get_band_capability_unpack(uint8_t *rsp, uint16_t len,
        dms_band_capabilities_t *out);

/**
 * @brief input type for QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION
 *
 */
typedef struct {
    /** Statistics enablement status, 0 for disabling, 1 for enabling */
    uint8_t enable_statistics;
} dms_modem_activity_info_calculation_t;

/**
 * @brief Pack QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION (94 - 0x005e) request
 *
 * This function packs a qmi request for triggering modem activity
 * info calculation
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #dms_modem_activity_info_calculation_t
 *
 */
QMI_SDK_error_t telit_dms_trigger_modem_activity_info_calculation_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        dms_modem_activity_info_calculation_t *input);

/**
 * @brief Unpack QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION (94 - 0x005e) request
 *
 * This function unpacks the result of a QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_dms_trigger_modem_activity_info_calculation_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_DMS_GET_MODEM_ACTIVITY_INFO (95 - 0x005f) request
 *
 * This function packs a qmi request for getting the modem activity
 * information
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_dms_get_modem_activity_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_DMS_GET_MODEM_ACTIVITY_INFO (95 - 0x005f) request
 *
 * This function unpacks the result of a QMI_DMS_GET_MODEM_ACTIVITY_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_dms_get_modem_activity_info_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI DMS battery charge states
 *
 */
typedef enum {
    /** Battery not charging */
    DMS_MASK_BATTERY_CHARGING_OFF         = 0x00,
    /** AC charging */
    DMS_MASK_BATTERY_CHARGING_BY_AC       = 0x01,
    /** USB charging */
    DMS_MASK_BATTERY_CHARGING_BY_USB      = 0x02,
    /** Wireless charging */
    DMS_MASK_BATTERY_CHARGING_BY_WIRELESS = 0x04,
} QMI_DMS_battery_charge_state_mask_t;

/**
 * @brief input type for QMI_DMS_DEVICE_POWER_INFO
 *
 */
typedef struct {
    /** Set if power_save_mode_status is available */
    bool power_save_mode_status_available;
    /** Power Save mode status, 1 for on, 0 for off */
    uint8_t power_save_mode_status;
    /** Set if battery_charge_state is available */
    bool battery_charge_state_available;
    /** Battery charge state, see #QMI_DMS_battery_charge_state_mask_t */
    uint64_t battery_charge_state;
    /** Set if battery_capacity is available */
    bool battery_capacity_available;
    /** Battery capacity in mAh */
    uint32_t battery_capacity;
    /** Set if battery_level is available */
    bool battery_level_available;
    /** Battery level (percentage) */
    uint8_t battery_level;
    /** Set if battery_level_index is available */
    bool battery_level_index_available;
    /** Battery level index: 0 if < 10%, 1 if in range [11 - 50]%, 2 if in range [51 - 90]%, 3 if > 90% */
    uint8_t battery_level_index;
} dms_modem_device_power_info_t;

/**
 * @brief Pack QMI_DMS_DEVICE_POWER_INFO (115 - 0x0073) request
 *
 * This function packs a qmi request for notifying the modem about
 * the power information on the UE
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #dms_modem_device_power_info_t
 *
 */
QMI_SDK_error_t telit_dms_device_power_info_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        dms_modem_device_power_info_t *input);

/**
 * @brief Unpack QMI_DMS_DEVICE_POWER_INFO (115 - 0x0073) request
 *
 * This function unpacks the result of a QMI_DMS_DEVICE_POWER_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_dms_device_power_info_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_DMS_SET_AT_IND_REPORT request
 *
 */
typedef struct {
    /** 1 to enable AT command responses indication, 0 to disable */
    uint8_t at_command_resp_enable;
} dms_at_ind_report_t;

/**
 * @brief Pack QMI_DMS_SET_AT_IND_REPORT (21852 - 0x555c) request
 *
 * This function packs a qmi request for enabling or disabling AT command
 * responses as indications
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #dms_at_ind_report_t
 *
 */
QMI_SDK_error_t telit_dms_set_at_ind_report_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        dms_at_ind_report_t *input);

/**
 * @brief Unpack QMI_DMS_SET_AT_IND_REPORT (21852 - 0x555c) request
 *
 * This function unpacks the result of a QMI_DMS_SET_AT_IND_REPORT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_dms_set_at_ind_report_unpack(uint8_t *rsp, uint16_t len);

#define QMI_DMS_AT_COMMAND_MAX_LEN 2048

/**
 * @brief input type for QMI_DMS_SEND_AT_COMMAND request
 *
 */
typedef struct {
    /** AT command length */
    uint16_t at_command_len;
    /** AT command to be sent */
    char at_command[QMI_DMS_AT_COMMAND_MAX_LEN];
} dms_at_command_t;

/**
 * @brief Pack QMI_DMS_SEND_AT_COMMAND (21853 - 0x555d) request
 *
 * This function packs a qmi request for sending an AT command
 * whose response will be received as an indication
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #dms_power_info_t
 *
 */
QMI_SDK_error_t telit_dms_send_at_command_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        dms_at_command_t *input);

/**
 * @brief Unpack QMI_DMS_SEND_AT_COMMAND (21853 - 0x555d) request
 *
 * This function unpacks the result of a QMI_DMS_SEND_AT_COMMAND request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_dms_send_at_command_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief output type for QMI_DMS_EVENT_IND indication
 *
 */
typedef struct {
    /** set if operating_mode is available */
    bool operating_mode_available;
    /** modem operating mode, see #QMI_DMS_operating_mode_t */
    enum8_t operating_mode;
} dms_ind_event_report_t;

/**
 * @brief QMI DMS Tx Power Level
 *
 */
typedef enum {
    /** Level one */
    QMI_DMS_TX_MODE_LEVEL_ONE   = 0,
    /** Level two */
    QMI_DMS_TX_MODE_LEVEL_TWO   = 1,
    /** Level three */
    QMI_DMS_TX_MODE_LEVEL_THREE = 2,
    /** Level four */
    QMI_DMS_TX_MODE_LEVEL_FOUR  = 3,
    /** Level five */
    QMI_DMS_TX_MODE_LEVEL_FIVE  = 4,
} QMI_DMS_tx_mode_level_t;

/**
 * @brief QMI DMS tx mode duration instance type
 *
 */
typedef struct {
    /** Power level used by the modem in tx mode, see #QMI_DMS_tx_mode_level_t */
    QMI_DMS_tx_mode_level_t tx_mode_level;
    /** Duration for which the modem was active in tx mode (in milliseconds) */
    uint32_t tx_mode_duration;
} dms_tx_mode_duration_t;

#define QMI_DMS_MODEM_ACTIVITY_MAX_SUPPORTED_TX_MODES 5

/**
 * @brief output type for QMI_DMS_MODEM_ACTIVITY_INFO_IND indication
 *
 */
typedef struct {
    /** set if idle_mode_duration is available */
    bool idle_mode_duration_available;
    /** Duration for which the modem was awake but the traffic channel was released
     * (in milliseconds) */
    uint32_t idle_mode_duration;
    /** set if sleep_mode_duration is available */
    bool sleep_mode_duration_available;
    /** Duration for which the modem was in power collapse (in milliseconds) */
    uint32_t sleep_mode_duration;
    /** set if rx_mode_duration is available */
    bool rx_mode_duration_available;
    /** Duration for which the modem was active in rx mode (in milliseconds) */
    uint32_t rx_mode_duration;
    /** size of tx_mode_duration */
    uint8_t tx_mode_duration_size;
    /** Array of tx mode durations instances, see #dms_tx_mode_duration_t */
    dms_tx_mode_duration_t tx_mode_duration[QMI_DMS_MODEM_ACTIVITY_MAX_SUPPORTED_TX_MODES];
} dms_modem_activity_info_t;

/**
 * @brief Unpack dms indications
 *
 * This function unpacks the result of the following dms indications
 *
 * QMI_DMS_EVENT_IND (1)
 * QMI_DMS_MODEM_ACTIVITY_INFO_IND (95)
 * QMI_DMS_AT_COMMAND_IND (21853)
 *
 * @param[in] msg_id indication id
 * @param[in] rsp indication
 * @param[in] len indication length
 * @param[out] out type according to the following
 *      QMI_DMS_EVENT_IND               -> #dms_ind_event_report_t
 *      QMI_DMS_MODEM_ACTIVITY_INFO_IND -> #dms_modem_activity_info_t
 *      QMI_DMS_AT_COMMAND_IND          -> #dms_at_command_t
 * @param[in] out_len buffer size
 */
QMI_SDK_error_t telit_dms_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len,
        uint8_t *out, uint16_t out_len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_DMS_H_ */
