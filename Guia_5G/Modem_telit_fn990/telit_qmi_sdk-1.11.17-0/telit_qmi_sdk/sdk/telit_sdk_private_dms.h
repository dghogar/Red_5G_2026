#ifndef TELIT_SDK_PRIVATE_DMS_H_
#define TELIT_SDK_PRIVATE_DMS_H_

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_DMS_SET_EVENT_REPORT (1 - 0x0001)
 */

/* Structure to describe request TLV 0x13 for QMI_DMS_SET_EVENT_REPORT */
typedef struct {
    uint8_t report_activation_state;
} QMI_DMS_SET_EVENT_REPORT_request_0x13_t;

/* Structure to describe request TLV 0x14 for QMI_DMS_SET_EVENT_REPORT */
typedef struct {
    uint8_t report_oprt_mode_state;
} QMI_DMS_SET_EVENT_REPORT_request_0x14_t;

/*****************************************************************************
 * QMI_DMS_GET_DEVICE_CAP (32 - 0x0020)
 */

/* Structure to describe response TLV 0x01 for QMI_DMS_GET_DEVICE_CAP */
typedef struct {
    uint32_t max_tx_channel_rate;
    uint32_t max_rx_channel_rate;
    uint8_t data_service_capability;
    uint8_t sim_capability;
    uint8_t radio_if_list_size;

    /* This array must be the size specified by radio_if_list_size */
    /* uint8_t radio_if_list[radio_if_list_size]; */
} QMI_DMS_GET_DEVICE_CAP_response_0x01_t;

/*****************************************************************************
 * QMI_DMS_GET_OPERATING_MODE (45 - 0x002D)
 */

/* Structure to describe response TLV 0x01 for QMI_DMS_GET_OPERATING_MODE */
typedef struct {
    uint8_t operating_mode;
} QMI_DMS_GET_OPERATING_MODE_response_0x01_t;

/* Structure to describe response TLV 0x10 for QMI_DMS_GET_OPERATING_MODE */
typedef struct {
    uint16_t offline_reason;
} QMI_DMS_GET_OPERATING_MODE_response_0x10_t;

/* Structure to describe response TLV 0x11 for QMI_DMS_GET_OPERATING_MODE */
typedef struct {
    uint8_t hardware_controlled_mode;
} QMI_DMS_GET_OPERATING_MODE_response_0x11_t;

/*****************************************************************************
 * QMI_DMS_SET_OPERATING_MODE (46 - 0x002E)
 */

/* Structure to describe request TLV 0x01 for QMI_DMS_SET_OPERATING_MODE */
typedef struct {
    uint8_t power_mode;
} QMI_DMS_SET_OPERATING_MODE_request_0x01_t;

/*****************************************************************************
 * QMI_DMS_GET_TIME (47 - 0x002F)
 */

/* Structure to describe response TLV 0x01 for QMI_DMS_GET_TIME */
typedef struct {
    uint64_t time;
} QMI_DMS_GET_TIME_response_0x01_t;

/* Structure to describe response TLV 0x10 for QMI_DMS_GET_TIME */
typedef struct {
    uint64_t sys_time_in_ms;
} QMI_DMS_GET_TIME_response_0x10_t;

/* Structure to describe response TLV 0x11 for QMI_DMS_GET_TIME */
typedef struct {
    uint64_t user_time_in_ms;
} QMI_DMS_GET_TIME_response_0x11_t;

/*****************************************************************************
 * QMI_DMS_GET_BAND_CAPABILITY (69 - 0x0045)
 */

/* Structure to describe response TLV 0x01 for QMI_DMS_GET_BAND_CAPABILITY */
typedef struct {
    uint64_t band_capability;
} QMI_DMS_GET_BAND_CAPABILITY_response_0x01_t;

/* Structure to describe response TLV 0x11 for QMI_DMS_GET_BAND_CAPABILITY */
typedef struct {
    uint64_t tds_band_capability;
} QMI_DMS_GET_BAND_CAPABILITY_response_0x11_t;

/* Structure to describe response TLV 0x12 for QMI_DMS_GET_BAND_CAPABILITY */
typedef struct {
    uint16_t supported_lte_bands_size;

    /* This array must be the size specified by supported_lte_bands_size */
    /* uint16_t supported_lte_bands[supported_lte_bands_size]; */
} QMI_DMS_GET_BAND_CAPABILITY_response_0x12_t;

/* Structure to describe response TLV 0x13 for QMI_DMS_GET_BAND_CAPABILITY */
typedef struct {
    uint16_t supported_nr5g_bands_size;

    /* This array must be the size specified by supported_nr5g_bands_size */
    /* uint16_t supported_nr5g_bands[supported_nr5g_bands_size]; */
} QMI_DMS_GET_BAND_CAPABILITY_response_0x13_t;

/*****************************************************************************
 * QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION (94 - 0x005e)
 */

/* Structure to describe request TLV 0x10 for QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION */
typedef struct {
    uint8_t enable_statistics;
} QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION_request_0x10_t;

/*****************************************************************************
 * QMI_DMS_DEVICE_POWER_INFO (115 - 0x0073)
 */

/* Structure to describe request TLV 0x10 for QMI_DMS_DEVICE_POWER_INFO */
typedef struct {
    uint8_t power_save_mode_status;
} QMI_DMS_DEVICE_POWER_INFO_request_0x10_t;

/* Structure to describe request TLV 0x11 for QMI_DMS_DEVICE_POWER_INFO */
typedef struct {
    uint64_t battery_charge_state;
} QMI_DMS_DEVICE_POWER_INFO_request_0x11_t;

/* Structure to describe request TLV 0x12 for QMI_DMS_DEVICE_POWER_INFO */
typedef struct {
    uint32_t battery_capacity;
} QMI_DMS_DEVICE_POWER_INFO_request_0x12_t;

/* Structure to describe request TLV 0x13 for QMI_DMS_DEVICE_POWER_INFO */
typedef struct {
    uint8_t battery_level;
} QMI_DMS_DEVICE_POWER_INFO_request_0x13_t;

/* Structure to describe request TLV 0x14 for QMI_DMS_DEVICE_POWER_INFO */
typedef struct {
    uint8_t battery_level_index;
} QMI_DMS_DEVICE_POWER_INFO_request_0x14_t;

/*****************************************************************************
 * QMI_DMS_SET_AT_IND_REPORT (21852 - 0x555c)
 */

/* Structure to describe request TLV 0x01 for QMI_DMS_SET_AT_IND_REPORT */
typedef struct {
    uint8_t at_command_resp_enable;
} QMI_DMS_SET_AT_IND_REPORT_request_0x01_t;

/*****************************************************************************
 * QMI_DMS_SEND_AT_COMMAND (21853 - 0x555d)
 */

/* Structure to describe request TLV 0x01 for QMI_DMS_SET_OPERATING_MODE */
typedef struct {
    uint16_t at_command_len;

    /* This array must be the size specified by at_command_len */
    /* uint8_t at_command[at_command_len]; */
} QMI_DMS_SEND_AT_COMMAND_request_0x01_t;

/*****************************************************************************
 * QMI DMS indications
 */

/* Structure to describe indication TLV 0x13 for QMI_DMS_EVENT_REPORT_IND */
typedef struct {
    uint16_t activation_state;
} QMI_DMS_EVENT_REPORT_indication_0x13_t;

/* Structure to describe indication TLV 0x14 for QMI_DMS_EVENT_REPORT_IND */
typedef struct {
    uint8_t operating_mode;
} QMI_DMS_EVENT_REPORT_indication_0x14_t;

/* Structure to describe indication TLV 0x10 for QMI_DMS_MODEM_ACTIVITY_INFO_IND */
typedef struct {
    uint32_t idle_mode_duration;
} QMI_DMS_MODEM_ACTIVITY_INFO_indication_0x10_t;

/* Structure to describe indication TLV 0x11 for QMI_DMS_MODEM_ACTIVITY_INFO_IND */
typedef struct {
    uint32_t sleep_mode_duration;
} QMI_DMS_MODEM_ACTIVITY_INFO_indication_0x11_t;

/* Structure to describe indication TLV 0x12 for QMI_DMS_MODEM_ACTIVITY_INFO_IND */
typedef struct {
    uint32_t rx_mode_duration;
} QMI_DMS_MODEM_ACTIVITY_INFO_indication_0x12_t;

/* Structure to describe generic tx mode duration instance */
typedef struct {
    uint32_t tx_mode_level;
    uint32_t tx_mode_duration;
} tx_mode_duration_t;

/* Structure to describe indication TLV 0x13 for QMI_DMS_MODEM_ACTIVITY_INFO_IND */
typedef struct {
    uint8_t tx_mode_duration_size;
    /* This array must be the size specified by tx_mode_duration_size */
    /* tx_mode_duration_t tx_mode_duration[tx_mode_duration_size]; */
} QMI_DMS_MODEM_ACTIVITY_INFO_indication_0x13_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_DMS_H_ */
