#ifndef TELIT_SDK_PRIVATE_LOC_H_
#define TELIT_SDK_PRIVATE_LOC_H_

#include <math.h>

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_LOC_REG_EVENTS (33 - 0x0021)
 */

/* Structure to describe request TLV 0x01 for QMI_LOC_REG_EVENTS */
typedef struct {
    uint64_t event_mask;
} QMI_LOC_REG_EVENTS_request_0x01_t;

/*****************************************************************************
 * QMI_LOC_START (34 - 0x0022)
 * QMI_LOC_STOP (35 - 0x0023)
 */

/* Structure to describe request TLV 0x01 for QMI_LOC_START */
typedef struct {
    uint8_t session_id;
} QMI_LOC_generic_session_id_t;

/* Structure to describe request TLV 0x10 for QMI_LOC_START */
typedef struct {
    uint32_t recurrence;
} QMI_LOC_START_request_0x10_t;

/* Structure to describe request TLV 0x11 for QMI_LOC_START */
typedef struct {
    uint32_t accuracy;
} QMI_LOC_START_request_0x11_t;

/*****************************************************************************
 * QMI_LOC_INJECT_UTC_TIME (56 - 0x0038)
 */

/* Structure to describe request TLV 0x01 for QMI_LOC_INJECT_UTC_TIME */
typedef struct {
    uint64_t time_utc;
} QMI_LOC_INJECT_UTC_TIME_request_0x01_t;

/* Structure to describe request TLV 0x02 for QMI_LOC_INJECT_UTC_TIME */
typedef struct {
    uint32_t time_unc;
} QMI_LOC_INJECT_UTC_TIME_request_0x02_t;

/*****************************************************************************
 * QMI_LOC_SET_NMEA_TYPES (62 - 0x003E)
 */

/* Structure to describe request TLV 0x01 for QMI_LOC_SET_NMEA_TYPES */
typedef struct {
    uint32_t nmea_sentence_type;
} QMI_LOC_SET_NMEA_TYPES_request_0x01_t;

/*****************************************************************************
 * QMI_LOC_DELETE_ASSIST_DATA (68 - 0x0044)
 */

/* Structure to describe request TLV 0x01 for QMI_LOC_DELETE_ASSIST_DATA */
typedef struct {
    uint8_t delete_all_flag;
} QMI_LOC_DELETE_ASSIST_DATA_request_0x01_t;

typedef struct
{
    uint16_t sv_id;
    uint32_t system;
    uint8_t sv_info_mask;
} sv_info_instance_t;

/* Structure to describe request TLV 0x10 for QMI_LOC_DELETE_ASSIST_DATA */
typedef struct {
    uint8_t sv_info_list_len;
    /* This array must be the size specified by bds_sv_info_list_len */
    /* sv_info_instance_t sv_info_list[sv_info_list_len]; */
} QMI_LOC_DELETE_ASSIST_DATA_request_0x10_t;

/* Structure to describe request TLV 0x11 for QMI_LOC_DELETE_ASSIST_DATA */
typedef struct {
    uint64_t gnss_mask;
} QMI_LOC_DELETE_ASSIST_DATA_request_0x11_t;

/* Structure to describe request TLV 0x12 for QMI_LOC_DELETE_ASSIST_DATA */
typedef struct {
    uint32_t cell_db_mask;
} QMI_LOC_DELETE_ASSIST_DATA_request_0x12_t;

/* Structure to describe request TLV 0x13 for QMI_LOC_DELETE_ASSIST_DATA */
typedef struct {
    uint32_t clock_info_mask;
} QMI_LOC_DELETE_ASSIST_DATA_request_0x13_t;

typedef struct {
    uint16_t gnss_sv_id;
    uint8_t sv_info_mask;
} generic_sv_info_instance_t;

/* Structure to describe request TLV 0x14 for QMI_LOC_DELETE_ASSIST_DATA */
typedef struct {
    uint8_t sv_info_list_len;
    /* This array must be the size specified by sv_info_list_len */
    /* sv_info_instance_t sv_info_list[sv_info_list_len]; */
} QMI_LOC_DELETE_ASSIST_DATA_generic_sv_info_t;

/*****************************************************************************
 * QMI_LOC_SET_OPERATION_MODE (74 - 0x004A)
 */

/* Structure to describe request TLV 0x01 for QMI_LOC_SET_OPERATION_MODE */
typedef struct {
    uint32_t operation_mode;
} QMI_LOC_SET_OPERATION_MODE_request_0x01_t;

/* Structure to describe request TLV 0x10 for QMI_LOC_SET_OPERATION_MODE */
typedef struct {
    uint32_t min_interval;
} QMI_LOC_SET_OPERATION_MODE_request_0x10_t;

/*****************************************************************************
 * QMI_LOC_SET_ENGINE_LOCK (58 - 0x003A)
 * QMI_LOC_GET_ENGINE_LOCK (59 - 0x003B)
 */

/* Structure to describe lock_status TLV QMI_LOC_SET_ENGINE_LOCK and QMI_LOC_GET_ENGINE_LOCK */
typedef struct {
    uint32_t lock_status;
} QMI_LOC_generic_lock_status_t;

/* Structure to describe lock_type TLV QMI_LOC_SET_ENGINE_LOCK and QMI_LOC_GET_ENGINE_LOCK */
typedef struct {
    uint32_t lock_type;
} QMI_LOC_generic_lock_type_t;

/* Structure to describe subscription type TLV for QMI_LOC_SET_ENGINE_LOCK and QMI_LOC_GET_ENGINE_LOCK */
typedef struct {
    uint32_t sub_type;
} QMI_LOC_generic_subscription_type_t;

/* Structure to describe lock client TLV for QMI_LOC_SET_ENGINE_LOCK and QMI_LOC_GET_ENGINE_LOCK */
typedef struct {
    uint64_t lock_client;
} QMI_LOC_generic_lock_client_t;

/*****************************************************************************
 * QMI_LOC_DELETE_GNSS_SERVICE_DATA (166 - 0x00A6)
 */

/* Structure to describe indication TLV 0x01 for QMI_LOC_DELETE_GNSS_SERVICE_DATA */
typedef struct {
    uint8_t delete_all_flag;
} QMI_LOC_DELETE_GNSS_SERVICE_DATA_0x01_t;

/* Structure to describe generic 32 bit mask TLV for QMI_LOC_DELETE_GNSS_SERVICE_DATA */
typedef struct {
    uint32_t mask_32;
} QMI_LOC_generic_mask_32_t;

/* Structure to describe indication TLV 0x13 for QMI_LOC_DELETE_GNSS_SERVICE_DATA */
typedef struct {
    uint32_t system_mask;
    uint32_t delete_satellite_data_mask;
} QMI_LOC_DELETE_GNSS_SERVICE_DATA_0x13_t;

/* Structure to describe generic 64 bit mask TLV for QMI_LOC_DELETE_GNSS_SERVICE_DATA */
typedef struct {
    uint64_t mask_64;
} QMI_LOC_generic_mask_64_t;

/*****************************************************************************
 * QMI_LOC_REGISTER_MASTER_CLIENT (186 - 0x00BA)
 */

/* Structure to describe TLV 0x01 for QMI_LOC_REGISTER_MASTER_CLIENT request and response */
typedef struct {
    uint32_t value;
} QMI_LOC_REGISTER_MASTER_CLIENT_generic_0x01_t;

/*****************************************************************************
 * QMI LOC indications
 */

/* Structure to describe indication TLV 0x01 for QMI_LOC_DELETE_ASSIST_DATA_IND,
 * QMI_LOC_SET_NMEA_TYPES_IND, QMI_LOC_SET_OPERATION_MODE_IND */
typedef struct {
    uint32_t status;
} QMI_LOC_generic_status_indication_0x01_t;

/* Structure to describe generic double values for QMI_LOC_EVENT_POSITION_REPORT_IND */
typedef struct {
    double_t value;
} QMI_LOC_generic_double_value_t;

/* Structure to describe generic float values for QMI_LOC_EVENT_POSITION_REPORT_IND */
typedef struct {
    float_t value;
} QMI_LOC_generic_float_value_t;

/* Structure to describe generic uint8_t values for QMI_LOC_EVENT_POSITION_REPORT_IND */
typedef struct {
    uint8_t value;
} QMI_LOC_generic_uint8_value_t;

/* Structure to describe generic uint8_t values for QMI_LOC_EVENT_POSITION_REPORT_IND */
typedef struct {
    uint16_t value;
} QMI_LOC_generic_uint16_value_t;

/* Structure to describe generic uint32_t values for QMI_LOC_EVENT_POSITION_REPORT_IND */
typedef struct {
    uint32_t value;
} QMI_LOC_generic_uint32_value_t;

/* Structure to describe generic uint64_t values for QMI_LOC_EVENT_POSITION_REPORT_IND */
typedef struct {
    uint64_t value;
} QMI_LOC_generic_uint64_value_t;

/* Structure to describe TLV 0x24 for QMI_LOC_EVENT_POSITION_REPORT_IND */
typedef struct {
    float PDOP;
    float HDOP;
    float VDOP;
} QMI_LOC_EVENT_POSITION_REPORT_indication_0x24_t;

/* Structure to describe TLV 0x27 for QMI_LOC_EVENT_POSITION_REPORT_IND */
typedef struct {
    uint16_t gps_week;
    uint32_t gps_time_of_week;
} QMI_LOC_EVENT_POSITION_REPORT_indication_0x27_t;

/* Structure to describe TLV 0x2A for QMI_LOC_EVENT_POSITION_REPORT_IND */
typedef struct {
    uint32_t sensor;
    uint32_t aiding_indicator;
} QMI_LOC_EVENT_POSITION_REPORT_indication_0x2A_t;

/* Structure to describe TLV 0x2C for QMI_LOC_EVENT_POSITION_REPORT_IND */
typedef struct {
    uint8_t gnss_sv_used_size;
    /* This array must be the size specified by gnss_sv_used_size */
    /* uint16_t gnss_sv_used[gnss_sv_used_size]; */
} QMI_LOC_EVENT_POSITION_REPORT_indication_0x2C_t;

typedef struct {
    uint32_t validity_mask;
    uint32_t system;
    uint16_t gnss_sv_id;
    uint8_t health_status;
    uint32_t sv_status;
    uint8_t sv_info_mask;
    float elevation;
    float azimuth;
    float snr;
} sv_instance_t;

typedef struct {
    uint32_t validity_mask;
    uint32_t system;
    uint16_t gnss_sv_id;
    uint8_t health_status;
    uint32_t sv_status;
    uint8_t sv_info_mask;
    float elevation;
    float azimuth;
    float snr;
    uint8_t glo_frequency;
} sv_ext_instance_t;

/* Structure to describe TLV 0x10 for QMI_LOC_EVENT_INJECT_TIME_REQ_IND */
typedef struct {
    uint32_t delay_threshold;
    uint8_t server_urls_size;
    /* This array must be the size specified by server_urls_size */
    /* server_url server_urls[server_urls_size]; */
} QMI_LOC_EVENT_INJECT_TIME_REQ_indication_0x10_t;

/* Structure to describe TLV 0x10 for QMI_LOC_EVENT_GNSS_SV_INFO_IND */
typedef struct {
    uint8_t sv_info_size;
    /* This array must be the size specified by sv_info_size */
    /* sv_instance_t sv_info[sv_info_size]; */
} QMI_LOC_EVENT_GNSS_SV_INFO_indication_0x10_t;

/* Structure to describe TLV 0x11 for QMI_LOC_EVENT_GNSS_SV_INFO_IND */
typedef struct {
    uint8_t sv_ext_info_size;
    /* This array must be the size specified by sv_info_size */
    /* sv_instance_t sv_info[sv_info_size]; */
} QMI_LOC_EVENT_GNSS_SV_INFO_indication_0x11_t;

/* Structure to describe TLV 0x01 for QMI_LOC_GET_TTFF_FOXCONN_IND */
typedef struct {
    uint32_t ttff;
} QMI_LOC_GET_TTFF_FOXCONN_IND_indication_0x10_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_LOC_H_ */
