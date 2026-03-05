#ifndef TELIT_SDK_LOC_H_
#define TELIT_SDK_LOC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

#include "telit_sdk.h"

/**
 * @file telit_sdk_loc.h
 * @brief LOC pack/unpack functions
 * Service ID: QMI_SVC_LOC (16)
 */

/**
 * @brief Enum to describe QMI LOC Message IDs
 */
typedef enum {
    /** 33 - 0x0021 Set LOC state report conditions */
    QMI_LOC_REG_EVENTS                    = 0x0021,
    /** 34 - 0x0022 Used by the control point to initiate a GPS session */
    QMI_LOC_START                         = 0x0022,
    /** 35 - 0x0023 Used by the control point to stop a GPS session */
    QMI_LOC_STOP                          = 0x0023,
    /** 36 - 0x0024 Send the position report to the control point */
    QMI_LOC_EVENT_POSITION_REPORT_IND     = 0x0024,
    /** 37 - 0x0025 Send a satellite report to the control point */
    QMI_LOC_EVENT_GNSS_SV_INFO_IND        = 0x0025,
    /** 38 - 0x0026 NMEA reports for position and satellites in view */
    QMI_LOC_EVENT_NMEA_IND                = 0x0026,

    /** 40 - 0x0028 Requests the control point to inject time information */
    QMI_LOC_EVENT_INJECT_TIME_REQ_IND     = 0x0028,

    /** 42 - 0x002A Requests the control point to inject a position */
    QMI_LOC_EVENT_INJECT_POSITION_REQ_IND = 0x002A,

    /** 56 - 0x0038 Injects UTC time in the location engine */
    QMI_LOC_INJECT_UTC_TIME               = 0x0038,
    /** 56 - 0x0038 Inject UTC time indication */
    QMI_LOC_INJECT_UTC_TIME_IND           = 0x0038,
    /** 57 - 0x0039 Injects a position to the location engine */
    QMI_LOC_INJECT_POSITION               = 0x0039,
    /** 57 - 0x0039 Injects position indication */
    QMI_LOC_INJECT_POSITION_IND           = 0x0039,

    /** 58 - 0x003A Sets the location engine lock */
    QMI_LOC_SET_ENGINE_LOCK               = 0x003A,
    /** 58 - 0x003A Set location engine lock indication */
    QMI_LOC_SET_ENGINE_LOCK_IND           = 0x003A,
    /** 59 - 0x003B Gets the location engine lock */
    QMI_LOC_GET_ENGINE_LOCK               = 0x003B,
    /** 59 - 0x003B Get location engine lock indication */
    QMI_LOC_GET_ENGINE_LOCK_IND           = 0x003B,

    /** 62 - 0x003E Set NMEA types */
    QMI_LOC_SET_NMEA_TYPES                = 0x003E,
    /** 62 - 0x003E Set NMEA types indication */
    QMI_LOC_SET_NMEA_TYPES_IND            = 0x003E,

    /** 68 - 0x0044 delete the location engine assistance data */
    QMI_LOC_DELETE_ASSIST_DATA            = 0x0044,
    /** 68 - 0x0044 Delete assist data indication */
    QMI_LOC_DELETE_ASSIST_DATA_IND        = 0x0044,

    /** 74 - 0x004A Tells the engine to use the specified operation mode while making the position fixes*/
    QMI_LOC_SET_OPERATION_MODE            = 0x004A,
    /** 74 - 0x004A Set operation mode indication */
    QMI_LOC_SET_OPERATION_MODE_IND        = 0x004A,
    /** 75 - 0x004B Get the current operation mode from the engine */
    QMI_LOC_GET_OPERATION_MODE            = 0x004B,
    /** 75 - 0x004B Get operation mode indication */
    QMI_LOC_GET_OPERATION_MODE_IND        = 0x004B,

    /** 166 - 0x00A6 Deletes the location engine service data from memory */
    QMI_LOC_DELETE_GNSS_SERVICE_DATA      = 0x00A6,
    /** 166 - 0x00A6 Delete GNSS Service Data indication */
    QMI_LOC_DELETE_GNSS_SERVICE_DATA_IND  = 0x00A6,

    /** 186 - 0x00BA Used by a control point to register itself as a master client */
    QMI_LOC_REGISTER_MASTER_CLIENT        = 0x00BA,
    /** 186 - 0x00BA Register master key indication */
    QMI_LOC_REGISTER_MASTER_CLIENT_IND    = 0x00BA,

    /** 4103 - 0x1007 Get time to first fix (Foxconn) */
    QMI_LOC_GET_TTFF_FOXCONN              = 0x1007,
    /** 4103 - 0x1007 Get time to first fix indication (Foxconn) */
    QMI_LOC_GET_TTFF_FOXCONN_IND          = 0x1007,
} QMI_LOC_message_t;

#define QMI_LOC_MAX_GNSS_INFO 100

/**
 * @brief QMI LOC events mask
 */
typedef enum {
    /** Mask to receive position report event indications. */
    QMI_LOC_EVENT_MASK_POSITION_REPORT                    = 0x00000001,
    /** Mask to receive satellite report event indications. These reports are sent at a 1 Hz rate. */
    QMI_LOC_EVENT_MASK_GNSS_SV_INFO                       = 0x00000002,
    /** Mask to receive NMEA reports for position and satellites in view. The report is at a 1 Hz rate. */
    QMI_LOC_EVENT_MASK_NMEA                               = 0x00000004,
    /** Mask to receive NI Notify/Verify request event indications. */
    QMI_LOC_EVENT_MASK_VERIFY_REQ                         = 0x00000008,
    /** Mask to receive time injection request event indications. */
    QMI_LOC_EVENT_MASK_INJECT_TIME_REQ                    = 0x00000010,
    /** Mask to receive predicted orbits request event indications. */
    QMI_LOC_EVENT_MASK_INJECT_PREDICTED_ORBITS_REQ        = 0x00000020,
    /** Mask to receive position injection request event indications. */
    QMI_LOC_EVENT_MASK_INJECT_POSITION_REQ                = 0x00000040,
    /** Mask to receive engine state report event indications. */
    QMI_LOC_EVENT_MASK_ENGINE_STATE                       = 0x00000080,
    /** Mask to receive fix session status report event indications. */
    QMI_LOC_EVENT_MASK_FIX_SESSION_STATE                  = 0x00000100,
    /** Mask to receive Wi-Fi position request event indications. */
    QMI_LOC_EVENT_MASK_WIFI_REQ                           = 0x00000200,
    /** Mask to receive notifications from the location engine indicating its readiness to accept data from the sensors (accelerometer, gyroscope, etc.). */
    QMI_LOC_EVENT_MASK_SENSOR_STREAMING_READY_STATUS      = 0x00000400,
    /** Mask to receive time sync requests from the GPS engine. Time sync enables the GPS engine to synchronize its clock with the sensor processor’s clock. */
    QMI_LOC_EVENT_MASK_TIME_SYNC_REQ                      = 0x00000800,
    /** Mask to receive Stationary Position Indicator (SPI) streaming report indications. */
    QMI_LOC_EVENT_MASK_SET_SPI_STREAMING_REPORT           = 0x00001000,
    /** Mask to receive location server requests. These requests are generated when the service wishes to establish a connection with a location server. */
    QMI_LOC_EVENT_MASK_LOCATION_SERVER_CONNECTION_REQ     = 0x00002000,
    /** Mask to receive notifications related to network-initiated Geofences. These events notify the client when a network-initiated Geofence is added, deleted, or edited. */
    QMI_LOC_EVENT_MASK_NI_GEOFENCE_NOTIFICATION           = 0x00004000,
    /** Mask to receive Geofencealerts. These alerts are generated to inform the client of the changes that may affect a Geofence, for example, if GPS is turned off or if the network is unavailable. */
    QMI_LOC_EVENT_MASK_GEOFENCE_GEN_ALERT                 = 0x00008000,
    /** Mask to receive notifications when a Geofence is breached. These events are generated when a UE enters or leaves the perimeter of a Geofence. This breach report is for a single Geofence. */
    QMI_LOC_EVENT_MASK_GEOFENCE_BREACH_NOTIFICATION       = 0x00010000,
    /** Mask to register for pedometer control requests from the location engine. The location engine sends this event to control the injection of pedometer reports. */
    QMI_LOC_EVENT_MASK_PEDOMETER_CONTROL                  = 0x00020000,
    /** Mask to register for motion data control requests from the location engine. The location engine sends this event to control the injection of motion data. */
    QMI_LOC_EVENT_MASK_MOTION_DATA_CONTROL                = 0x00040000,
    /** Mask to receive notification when a batch is full. The location engine sends this event to notify of Batch Full for ongoing batching session. */
    QMI_LOC_EVENT_MASK_BATCH_FULL_NOTIFICATION            = 0x00080000,
    /** Mask to receive position report indications along with an ongoin batching session. The location engine sends this event to notify the batched position report while a batching session is ongoing. */
    QMI_LOC_EVENT_MASK_LIVE_BATCHED_POSITION_REPORT       = 0x00100000,
    /** Mask to receive Wi-Fi Access Point (AP) data inject request event indications. */
    QMI_LOC_EVENT_MASK_WIFI_AP_DATA_REQ                   = 0x00200000,
    /** Mask to receive notifications when a Geofence is breached. These events are generated when a UE enters or leaves the perimeter of a Geofence. This breach notification is for multiple Geofences. Breaches from multiple Geofences are all batched and sent in the same notification. */
    QMI_LOC_EVENT_MASK_GEOFENCE_BATCH_BREACH_NOTIFICATION = 0x00400000,
    /** Mask to receive notifications from the location engine indicating its readiness to accept vehicle data (vehicle accelerometer, vehicle angular rate, vehicle odometry, etc.). */
    QMI_LOC_EVENT_MASK_VEHICLE_DATA_READY_STATUS          = 0x00800000,
    /** Mask to receive system clock and satellite measurement report events (system clock, SV time, Doppler, etc.). Reports are generated only for the GNSS satellite constellations that are enabled using QMI_LOC_SET_GnssCONSTELLReport_CONFIG. */
    QMI_LOC_EVENT_MASK_GNSS_MEASUREMENT_REPORT            = 0x01000000,
    /** Mask to receive satellite position reports as polynomials. Reports are generated only for the GNSS satellite constellations that are enabled using QMI_LOC_SET_GnssCONSTELLReport_CONFIG */
    QMI_LOC_EVENT_MASK_GNSS_SV_POLYNOMIAL_REPORT          = 0x02000000,
    /** Mask to receive notifications when a Geofence proximity is entered and exited. The proximity of a Geofence may be due to different contexts. These contexts are identified using the context ID in this indication. The context of a Geofence may contain Wi-Fi area ID lists, IBeacon lists, Cell-ID list, and so forth. */
    QMI_LOC_EVENT_MASK_GEOFENCE_PROXIMITY_NOTIFICATION    = 0x04000000,
    /** Mask to receive Generic Data Transport (GDT) session begin request event indications. */
    QMI_LOC_EVENT_MASK_GDT_UPLOAD_BEGIN_REQ               = 0x08000000,
    /** Mask to receive GDT session end request event indications. */
    QMI_LOC_EVENT_MASK_GDT_UPLOAD_END_REQ                 = 0x10000000,
    /** Mask to receive notifications when a Geofence is dwelled. These events are generated when a UE enters or leaves the perimete of a Geofence and dwells inside or outside for a specified time. This dwell notification is for multiple Geofences. Dwells from multiple Geofences are all batched and sent in the same notification. */
    QMI_LOC_EVENT_MASK_GEOFENCE_BATCH_DWELL_NOTIFICATION  = 0x20000000,
    /** Mask to receive requests for time zone information from the service. These events are generated when there is a need for time zone information in the service. */
    QMI_LOC_EVENT_MASK_GET_TIMEZONE_REQ                   = 0x40000000,
    /** Mask to receive asynchronous events related to batching. Multiple events can be registered by ORing the individual masks and sending them in this TLV. All unused bits in this mask must be set to 0. */
    QMI_LOC_EVENT_MASK_BATCHING_STATUS                    = 0x80000000,
} QMI_LOC_event_registration_t;

/**
 * @brief input type for QMI_LOC_REG_EVENTS request
 *
 */
typedef struct {
    /** Event mask, see #QMI_LOC_event_registration_t */
    enum64_t event_mask;
} loc_event_register_t;

/**
 * @brief Pack QMI_LOC_REG_EVENTS (33 - 0x0021) request
 *
 * This function packs a qmi request for setting the LOC
 * indications.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #loc_event_register_t
 */
QMI_SDK_error_t telit_loc_reg_events_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_event_register_t *input);

/**
 * @brief Unpack QMI_LOC_REG_EVENTS (33 - 0x0021) request
 *
 * This function unpacks the result of a QMI_LOC_REG_EVENTS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_reg_events_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI LOC recurrence types
 */
typedef enum {
    /** Request periodic position fixes */
    QMI_LOC_RECURRENCE_PERIODIC = 1,
    /** Request a single position fix */
    QMI_LOC_RECURRENCE_SINGLE   = 2,
} QMI_LOC_recurrence_type_t;

/**
 * @brief QMI LOC accuracy levels
 */
typedef enum {
    /** Low accuracy */
    QMI_LOC_ACCURACY_LOW  = 1,
    /** Medium accuracy */
    QMI_LOC_ACCURACY_MED  = 2,
    /** High accuracy */
    QMI_LOC_ACCURACY_HIGH = 3,
} QMI_LOC_accuracy_level_t;

/**
 * @brief QMI LOC Start parameters type
 *
 */
typedef struct {
    /** ID of the session as identified by the
     * control point. The session ID is reported
     * back in the position reports. The control
     * point must specify the same session ID
     * in the QMI_LOC_STOP_REQ message.
     * Range: 0 to 255 */
    uint8_t session_id;
    /** true if optional recurrence set */
    bool recurrence_available;
    /** Specifies the type of session in which the
     * control point is interested. If this TLV is
     * not specified, recurrence defaults to
     * SINGLE, see #QMI_LOC_recurrence_type_t */
    uint32_t recurrence;
    /** true if optional accuracy set */
    bool accuracy_available;
    /** Specifies the horizontal accuracy level
     * required by the control point. If not
     * specified, accuracy defaults to LOW, see
     * #QMI_LOC_accuracy_level_t */
    uint32_t accuracy;
} loc_start_t;

/**
 * @brief Pack QMI_LOC_START (34 - 0x0022) request
 *
 * This function packs a qmi request to initiate a GPS session
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #loc_start_t
 */
QMI_SDK_error_t telit_loc_start_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_start_t *input);

/**
 * @brief Unpack QMI_LOC_START (34 - 0x0022) request
 *
 * This function unpacks the result of a QMI_LOC_START request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_start_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI LOC Stop parameters type
 *
 */
typedef struct {
    /** ID of the session as identified by the
     * control point. The session ID is reported
     * back in the position reports. The control
     * point must specify the same session ID
     * in the QMI_LOC_STOP_REQ message.
     * Range: 0 to 255 */
    uint8_t session_id;
} loc_stop_t;

/**
 * @brief Pack QMI_LOC_STOP (35 - 0x0023) request
 *
 * This function packs a qmi request to stop a GPS session.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #loc_event_register_t
 */
QMI_SDK_error_t telit_loc_stop_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_stop_t *input);

/**
 * @brief Unpack QMI_LOC_STOP (35 - 0x0023) request
 *
 * This function unpacks the result of a QMI_LOC_STOP request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_stop_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_LOC_INJECT_UTC_TIME request
 *
 */
typedef struct {
    /** UTC time since Jan. 1, 1970 in milliseconds */
    uint64_t time_utc;
    /** Time uncertainty in milliseconds */
    uint32_t time_unc;
} loc_utc_time_t;

/**
 * @brief Pack QMI_LOC_INJECT_UTC_TIME (56 - 0x0038) request
 *
 * This function packs a qmi request to inject the UTC Time in the
 * location engine
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #loc_utc_time_t
 */
QMI_SDK_error_t telit_loc_inject_utc_time_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_utc_time_t *input);

/**
 * @brief Unpack QMI_LOC_INJECT_UTC_TIME (56 - 0x0038) request
 *
 * This function unpacks the result of a QMI_LOC_INJECT_UTC_TIME request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_inject_utc_time_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI LOC position sources
 */
typedef enum {
    /** GNSS */
    QMI_LOC_POSITION_SRC_GNSS                    = 0,
    /** Cell ID */
    QMI_LOC_POSITION_SRC_CELLID                  = 1,
    /** Enhanced Cell ID */
    QMI_LOC_POSITION_SRC_ENH_CELLID              = 2,
    /** Wi-Fi */
    QMI_LOC_POSITION_SRC_WIFI                    = 3,
    /** Terrestrial */
    QMI_LOC_POSITION_SRC_TERRESTRIAL             = 4,
    /** GNSS Terrestrial Hybrid */
    QMI_LOC_POSITION_SRC_GNSS_TERRESTRIAL_HYBRID = 5,
    /** Other source */
    QMI_LOC_POSITION_SRC_OTHER                   = 6,
    /** Dead reckoning engine */
    QMI_LOC_POSITION_SRC_DRE                     = 7,
} QMI_LOC_position_source_t;

/**
 * @brief input type for QMI_LOC_INJECT_POSITION request
 *
 */
typedef struct {
    /** set if latitude is available */
    bool latitude_available;
    /** Latitude (specified in WGS84 datum). Floating point type in degrees with range -90.0 to 90.0. Positive values indicate northern latitude, while negative shouthern */
    double latitude;
    /** set if longitude is available */
    bool longitude_available;
    /** Longitude (specified in WGS84 datum). Floating point type in degrees with range -180.0 to 180.0. Positive values indicate eastern latitude, while negative western */
    double longitude;
    /** set if hor_unc_circular is available */
    bool hor_unc_circular_available;
    /** Horizontal position uncertainty (circular), in meters */
    float_t hor_unc_circular;
    /** set if position_source is available */
    bool position_source_available;
    /** Source from which this position was obtained, see #QMI_LOC_position_source_t */
    enum32_t position_source;
} loc_position_t;

/**
 * @brief Pack QMI_LOC_INJECT_POSITION (57 - 0x0039) request
 *
 * This function packs a qmi request to inject the position to the
 * location engine
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #loc_position_t
 */
QMI_SDK_error_t telit_loc_inject_position_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_position_t *input);

/**
 * @brief Unpack QMI_LOC_INJECT_POSITION (57 - 0x0039) request
 *
 * This function unpacks the result of a QMI_LOC_INJECT_POSITION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_inject_position_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI LOC NMEA sentence types
 */
typedef enum {
    /** Enable GGA type */
    QMI_LOC_NMEA_MASK_GGA   = 0x00000001,
    /** Enable RMC type */
    QMI_LOC_NMEA_MASK_RMC   = 0x00000002,
    /** Enable GSV type */
    QMI_LOC_NMEA_MASK_GSV   = 0x00000004,
    /** Enable GSA type */
    QMI_LOC_NMEA_MASK_GSA   = 0x00000008,
    /** Enable VTG type */
    QMI_LOC_NMEA_MASK_VTG   = 0x00000010,
    /** Enable PQXFI type */
    QMI_LOC_NMEA_MASK_PQXFI = 0x00000020,
    /** Enable PSTIS type */
    QMI_LOC_NMEA_MASK_PSTIS = 0x00000040,
    /** Enable GLGSV type */
    QMI_LOC_NMEA_MASK_GLGSV = 0x00000080,
    /** Enable GNGSA type */
    QMI_LOC_NMEA_MASK_GNGSA = 0x00000100,
    /** Enable GNGNS type */
    QMI_LOC_NMEA_MASK_GNGNS = 0x00000200,
    /** Enable GARMC type */
    QMI_LOC_NMEA_MASK_GARMC = 0x00000400,
    /** Enable GAGSV type */
    QMI_LOC_NMEA_MASK_GAGSV = 0x00000800,
    /** Enable GAGSA type */
    QMI_LOC_NMEA_MASK_GAGSA = 0x00001000,
    /** Enable GAVTG type */
    QMI_LOC_NMEA_MASK_GAVTG = 0x00002000,
    /** Enable GAGGA type */
    QMI_LOC_NMEA_MASK_GAGGA = 0x00004000,
    /** Enable PQGSA type */
    QMI_LOC_NMEA_MASK_PQGSA = 0x00008000,
    /** Enable PQGSV type */
    QMI_LOC_NMEA_MASK_PQGSV = 0x00010000,
} QMI_LOC_nmea_sentence_type_t;

/**
 * @brief input type for QMI_LOC_SET_NMEA_TYPES request
 *
 */
typedef struct {
    /** Bitmasks of NMEA types to enable, see #QMI_LOC_nmea_sentence_type_t */
    enum32_t nmea_sentence_type;
} loc_nmea_types_t;

/**
 * @brief Pack QMI_LOC_SET_NMEA_TYPES (62 - 0x003E) request
 *
 * This function packs a qmi request to set the NMEA sentence types
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #loc_nmea_types_t
 */
QMI_SDK_error_t telit_loc_set_nmea_types_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_nmea_types_t *input);

/**
 * @brief Unpack QMI_LOC_SET_NMEA_TYPES (62 - 0x003E) request
 *
 * This function unpacks the result of a QMI_LOC_SET_NMEA_TYPES request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_set_nmea_types_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI LOC SV Systems
 */
typedef enum {
    /** GPS satellite */
    QMI_LOC_SV_SYSTEM_GPS     = 1,
    /** GALILEO satellite */
    QMI_LOC_SV_SYSTEM_GALILEO = 2,
    /** SBAS satellite */
    QMI_LOC_SV_SYSTEM_SBAS    = 3,
    /** COMPASS satellite */
    QMI_LOC_SV_SYSTEM_COMPASS = 4,
    /** GLONASS satellite */
    QMI_LOC_SV_SYSTEM_GLONASS = 5,
    /** BDS satellite */
    QMI_LOC_SV_SYSTEM_BDS     = 6,
    /** QZSS satellite */
    QMI_LOC_SV_SYSTEM_QZSS    = 7,
} QMI_LOC_SV_system_t;

/**
 * @brief QMI LOC SV info mask
 */
typedef enum {
    /** Delete ephemeris for the satellite */
    QMI_LOC_MASK_DELETE_EPHEMERIS = 0x01,
    /** Delete almanac for the satellite */
    QMI_LOC_MASK_DELETE_ALMANAC   = 0x02,
} QMI_LOC_SV_delete_mask_t;

/**
 * @brief QMI LOC sv info instance type
 *
 */
typedef struct {
    /** SV ID of the satellite whose data is to be deleted.
     * Range:
     * – For GPS: 1 to 32
     * – For SBAS: 33 to 64
     * – For GLONASS: 65 to 96 */
    uint16_t gnss_sv_id;
    /** Indicates to which constellation this SV belongs, see #QMI_LOC_SV_system_t */
    enum32_t system;
    /** Indicates what kind of data should be deleted, see #QMI_LOC_SV_delete_mask_t */
    uint8_t sv_mask;
} loc_sv_info_instance_t;

/**
 * @brief QMI LOC sv info type
 *
 */
typedef struct {
    /** number of instances of sv_inst */
    uint8_t sv_info_size;
    /** see #loc_sv_info_instance_t */
    loc_sv_info_instance_t sv_inst[QMI_LOC_MAX_GNSS_INFO];
} loc_sv_info_t;

/**
 * @brief QMI LOC GNSS data types
 */
typedef enum {
    /** Mask to delete GPS SVDIR */
    QMI_LOC_MASK_DELETE_GPS_SVDIR             = 0x00000001,
    /** Mask to delete GPS SVSTEER */
    QMI_LOC_MASK_DELETE_GPS_SVSTEER           = 0x00000002,
    /** Mask to delete GPS time */
    QMI_LOC_MASK_DELETE_GPS_TIME              = 0x00000004,
    /** Mask to delete almanac correlation */
    QMI_LOC_MASK_DELETE_GPS_ALM_CORR          = 0x00000008,
    /** Mask to delete GLONASS SVDIR */
    QMI_LOC_MASK_DELETE_GLO_SVDIR             = 0x00000010,
    /** Mask to delete GLONASS SVSTEER */
    QMI_LOC_MASK_DELETE_GLO_SVSTEER           = 0x00000020,
    /** Mask to delete GLONASS time */
    QMI_LOC_MASK_DELETE_GLO_TIME              = 0x00000040,
    /** Mask to delete GLONASS almanac correlation */
    QMI_LOC_MASK_DELETE_GLO_ALM_CORR          = 0x00000080,
    /** Mask to delete SBAS SVDIR */
    QMI_LOC_MASK_DELETE_SBAS_SVDIR            = 0x00000100,
    /** Mask to delete SBAS SVSTEER */
    QMI_LOC_MASK_DELETE_SBAS_SVSTEER          = 0x00000200,
    /** Mask to delete position estimate */
    QMI_LOC_MASK_DELETE_POSITION              = 0x00000400,
    /** Mask to delete time estimate */
    QMI_LOC_MASK_DELETE_TIME                  = 0x00000800,
    /** Mask to delete IONO */
    QMI_LOC_MASK_DELETE_IONO                  = 0x00001000,
    /** Mask to delete UTC estimate */
    QMI_LOC_MASK_DELETE_UTC                   = 0x00002000,
    /** Mask to delete SV health record */
    QMI_LOC_MASK_DELETE_HEALTH                = 0x00004000,
    /** Mask to delete SADATA */
    QMI_LOC_MASK_DELETE_SADATA                = 0x00008000,
    /** Mask to delete RTI */
    QMI_LOC_MASK_DELETE_RTI                   = 0x00010000,
    /** Mask to delete SV_NO_EXIST */
    QMI_LOC_MASK_DELETE_SV_NO_EXIST           = 0x00020000,
    /** Mask to delete frequency bias estimate */
    QMI_LOC_MASK_DELETE_FREQ_BIAS_EST         = 0x00040000,
    /** Mask to delete BDS SVDIR */
    QMI_LOC_MASK_DELETE_BDS_SVDIR             = 0x00080000,
    /** Mask to delete BDS SVSTEER */
    QMI_LOC_MASK_DELETE_BDS_SVSTEER           = 0x00100000,
    /** Mask to delete BDS time */
    QMI_LOC_MASK_DELETE_BDS_TIME              = 0x00200000,
    /** Mask to delete BDS almanac correlation */
    QMI_LOC_MASK_DELETE_BDS_ALM_CORR          = 0x00400000,
    /** Mask to delete GNSS SV blacklist GPS */
    QMI_LOC_MASK_DELETE_GNSS_SV_BLACKLIST_GPS = 0x00800000,
    /** Mask to delete GNSS SV blacklist GLO */
    QMI_LOC_MASK_DELETE_GNSS_SV_BLACKLIST_GLO = 0x01000000,
    /** Mask to delete GNSS SV blacklist BDS */
    QMI_LOC_MASK_DELETE_GNSS_SV_BLACKLIST_BDS = 0x02000000,
    /** Mask to delete GNSS SV blacklist GAL */
    QMI_LOC_MASK_DELETE_GNSS_SV_BLACKLIST_GAL = 0x04000000,
    /** Mask to delete GAL SVDIR */
    QMI_LOC_MASK_DELETE_GAL_SVDIR             = 0x08000000,
    /** Mask to delete GAL SVSTEER */
    QMI_LOC_MASK_DELETE_GAL_SVTEER            = 0x10000000,
    /** Mask to delete GAL time */
    QMI_LOC_MASK_DELETE_GAL_TIME              = 0x20000000,
    /** Mask to delete GAL almanac correlation */
    QMI_LOC_MASK_DELETE_GAL_ALM_CORR          = 0x40000000,
} QMI_LOC_delete_data_t;

/**
 * @brief QMI LOC Cell DB Data types
 */
typedef enum {
    QMI_LOC_DELETE_CELLDB_POS            = 0x00000001,
    QMI_LOC_DELETE_CELLDB_LATEST_GPS_POS = 0x00000002,
    QMI_LOC_DELETE_CELLDB_OTA_POS        = 0x00000004,
    QMI_LOC_DELETE_CELLDB_EXT_REF_POS    = 0x00000008,
    QMI_LOC_DELETE_CELLDB_TIMETAG        = 0x00000010,
    QMI_LOC_DELETE_CELLDB_CELLID         = 0x00000020,
    QMI_LOC_DELETE_CELLDB_CACHED_CELLID  = 0x00000040,
    QMI_LOC_DELETE_CELLDB_LAST_SRV_CELL  = 0x00000080,
    QMI_LOC_DELETE_CELLDB_CUR_SRV_CELL   = 0x00000100,
    QMI_LOC_DELETE_CELLDB_NEIGHBOR_INFO  = 0x00000200,
} QMI_LOC_delete_cell_db_data_t;

/**
 * @brief QMI LOC Clock Info
 */
typedef enum {
    /** Mask to delete time estimate from clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_TIME_EST         = 0x00000001,
    /** Mask to delete frequency estimate from clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_FREQ_EST         = 0x00000002,
    /** Mask to delete week number from clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_WEEK_NUMBER      = 0x00000004,
    /** Mask to delete RTC time from clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_RTC_TIME         = 0x00000008,
    /** Mask to delete time transfer from clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_TIME_TRANSFER    = 0x00000010,
    /** Mask to delete GPS time estimate from clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GPSTIME_EST      = 0x00000020,
    /** Mask to delete GLONASS time estimate from clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GLOTIME_EST      = 0x00000040,
    /** Mask to delete GLONASS day number from clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GLODAY_NUMBER    = 0x00000080,
    /** Mask to delete GLONASS four year number from clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GLO4YEAR_NUMBER  = 0x00000100,
    /** Mask to delete GLONASS RF GRP delay from clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GLO_RF_GRP_DELAY = 0x00000200,
    /** Mask to delete disable TT from clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_DISABLE_TT       = 0x00000400,
    /** Mask to delete a BDS time estimate from the clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GG_LEAPSEC       = 0x00000800,
    /** Mask to delete a BDS time estimate from the clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GG_GGTB          = 0x00001000,
    /** Mask to delete a BDS time estimate from the clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_BDSTIME_EST      = 0x00002000,
    /** Mask to delete Glonass-to-BDS time bias-related information from the clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GB_GBTB          = 0x00004000,
    /** Mask to delete BDS-to-GLONASS time bias-related information from the clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_BG_BGTB          = 0x00008000,
    /** Mask to delete the BDS week number from the clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_BDSWEEK_NUMBER   = 0x00010000,
    /** Mask to delete the BDS RF GRP delay from the clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_BDS_RF_GRP_DELAY = 0x00020000,
    /** Mask to delete a GAL time estimate from the clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GALTIME_EST      = 0x00040000,
    /** Mask to delete GAL-to-GPS time bias-related information from the clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GALTOGPS_TB      = 0x00080000,
    /** Mask to delete GAL-to-GLO time bias-related information from the clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GALTOGLO_TB      = 0x00100000,
    /** Mask to delete GAL-to-BDS time bias-related information from the clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GALTOBDS_TB      = 0x00200000,
    /** Mask to delete the GAL week number from the clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GALWEEK_NUMBER   = 0x00800000,
    /** Mask to delete the GAL RF GRP delay from the clock information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_GAL_RF_GRP_DELAY = 0x01000000,
} QMI_LOC_delete_clock_info_t;

/**
 * @brief QMI LOC SV Info Instance type
 *
 */
typedef struct {
    /** SV ID of the satellite whose data is to be deleted.
     * Range for BDS: 201 to 237 */
    uint16_t gnss_sv_id;
    /** Indicates if the ephemeris or almanac for a satellite is to be deleted, see #QMI_LOC_SV_delete_mask_t */
    uint8_t sv_mask;
} loc_bds_sv_info_instance_t;

/**
 * @brief QMI LOC SV Info type
 *
 */
typedef struct {
    /** number of instances of bds_sv_inst */
    uint8_t bds_sv_info_size;
    /** see #loc_bds_sv_info_instance_t */
    loc_bds_sv_info_instance_t bds_sv_inst[QMI_LOC_MAX_GNSS_INFO];
} loc_bds_sv_info_t;

/**
 * @brief QMI LOC GAL SV Info Instance type
 *
 */
typedef struct {
    /** SV ID of the satellite whose data is to be deleted.
     * Range for GAL: 301 to 336 */
    uint16_t gal_sv_id;
    /** Indicates if the ephemeris or almanac for a satellite is to be deleted, see #QMI_LOC_SV_delete_mask_t */
    enum8_t gal_sv_mask;
} loc_gal_sv_info_instance_t;

/**
 * @brief QMI GAL SV Info type
 *
 */
typedef struct {
    /** number of instances of gal_sv_inst */
    uint8_t gal_sv_info_size;
    /** see #loc_gal_sv_info_instance_t */
    loc_gal_sv_info_instance_t gal_sv_inst[QMI_LOC_MAX_GNSS_INFO];
} loc_gal_sv_info_t;

/**
 * @brief QMI LOC delete assist data type
 *
 */
typedef struct {
    /** Indicates whether all assistance data is to be deleted.
     *
     * Valid values:
     * - 0x01 (TRUE) – All assistance data is
     *                 to be deleted; if this flag is set, all the
     *                 other information contained in the
     *                 optional fields for this message are
     *                 ignored
     * - 0x00 (FALSE) – The optional fields in
     *                  the message are to be used to
     *                  determine which data is to be deleted
     */
    bool delete_all_flag;
    /** set if sv_info is available */
    bool sv_info_available;
    /** Delete SV Info, see #loc_sv_info_t */
    loc_sv_info_t sv_info;
    /** set if gnss_info is available */
    bool gnss_info_available;
    /** Delete GNSS Data, see #QMI_LOC_delete_data_t */
    enum64_t gnss_info;
    /** set if cell_db_info is available */
    bool cell_db_info_available;
    /** Delete Cell Database, see #QMI_LOC_delete_cell_db_data_t */
    enum32_t cell_db_info;
    /** set if clock_info is available */
    bool clock_info_available;
    /** Delete Clock Info, see #QMI_LOC_delete_clock_info_t */
    enum32_t clock_info;
    /** set if bds_sv_info is available */
    bool bds_sv_info_available;
    /** Delete BDS SV Info, see #loc_bds_sv_info_t */
    loc_bds_sv_info_t bds_sv_info;
    /** set if gal_sv_info is available */
    bool gal_sv_info_available;
    /** Delete GAL SV Info, see #loc_gal_sv_info_t */
    loc_gal_sv_info_t gal_sv_info;
} loc_delete_assist_data_t;

/**
 * @brief Pack QMI_LOC_DELETE_ASSIST_DATA (68 - 0x0044) request
 *
 * This command is used to delete assist data
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #loc_delete_assist_data_t
 */
QMI_SDK_error_t telit_loc_delete_assist_data_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        loc_delete_assist_data_t *input);

/**
 * @brief Unpack QMI_LOC_DELETE_ASSISTDATA (68 - 0x0044) request
 *
 * This function unpacks the result of a QMI_LOC_DELETE_ASSIST_DATA request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_delete_assist_data_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI LOC Operation modes
 */
typedef enum {
    /** Use the default engine mode */
    QMI_LOC_OPER_MODE_DEFAULT    = 1,
    /** Use the MS-based mode */
    QMI_LOC_OPER_MODE_MSB        = 2,
    /** Use the MS-assisted mode */
    QMI_LOC_OPER_MODE_MSA        = 3,
    /** Use Standalone mode */
    QMI_LOC_OPER_MODE_STANDALONE = 4,
    /** Use cell ID; this mode is only valid for GSM/UMTS networks */
    QMI_LOC_OPER_MODE_CELL_ID    = 5,
    /** Use WWAN measurements to calculate the position; if this mode is set, AFLT will be used for 1X networks and OTDOA will be used for LTE networks */
    QMI_LOC_OPER_MODE_WWAN       = 6,
} QMI_LOC_operation_mode_t;

/**
 * @brief input type for QMI_LOC_SET_OPERATION_MODE request
 *
 */
typedef struct {
    /** Preferred operation mode, see #QMI_LOC_operation_mode_t */
    enum32_t operation_mode;
    /** set if min_interval is available */
    bool min_interval_available;
    /** Minimum time interval, that must elapse between position reports, default 1000 ms */
    uint32_t min_interval;
} loc_operation_mode_t;

/**
 * @brief Pack QMI_LOC_SET_OPERATION_MODE (74 - 0x004A) request
 *
 * This function packs a qmi request to set the NMEA types.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #loc_operation_mode_t
 */
QMI_SDK_error_t telit_loc_set_operation_mode_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_operation_mode_t *input);

/**
 * @brief Unpack QMI_LOC_SET_OPERATION_MODE (74 - 0x004A) request
 *
 * This function unpacks the result of a QMI_LOC_SET_OPERATION_MODE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_set_operation_mode_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_LOC_GET_OPERATION_MODE (75 - 0x004B) request
 *
 * This function packs a qmi request to get the current operation mode
 * from the engine.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_loc_get_operation_mode_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_LOC_GET_OPERATION_MODE (75 - 0x004B) request
 *
 * This function unpacks the result of a QMI_LOC_GET_OPERATION_MODE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_get_operation_mode_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI LOC engine lock types
 */
typedef enum {
    /** Do not lock any position sessions */
    QMI_LOC_LOCK_NONE = 1,
    /** Lock application framework/AFW position sessions */
    QMI_LOC_LOCK_MI   = 2,
    /** Lock non-AFW/NFW position sessions */
    QMI_LOC_LOCK_MT   = 3,
    /** Lock all position sessions */
    QMI_LOC_LOCK_ALL  = 4,
} QMI_LOC_engine_lock_type_t;

/**
 * @brief QMI LOC engine subscription types
 */
typedef enum {
    /** Lock Dedicated Voice Subscription (DV sub) */
    QMI_LOC_LOCK_DV_SUB  = 1,
    /** Lock Dedicated Data Subscription (DD sub) */
    QMI_LOC_LOCK_DD_SUB  = 2,
    /** Lock all subscriptions */
    QMI_LOC_LOCK_ALL_SUB = 3,
} QMI_LOC_lock_sub_type_t;

/**
 * @brief QMI LOC engine lock clients
 */
typedef enum {
    /** Lock AFW client */
    QMI_LOC_LOCK_CLIENT_MASK_AFW        = 0x00000001,
    /** Lock NFW client */
    QMI_LOC_LOCK_CLIENT_MASK_NFW        = 0x00000002,
    /** Lock privileged client */
    QMI_LOC_LOCK_CLIENT_MASK_PRIVILEGED = 0x00000004,
} QMI_LOC_lock_client_t;

/**
 * @brief input type for QMI_LOC_SET_ENGINE_LOCK request
 *
 */
typedef struct {
    /** Type of lock, see #QMI_LOC_engine_lock_type_t */
    enum32_t lock_type;
    /** set if subscription_type is available */
    bool subscription_type_available;
    /** Subscription to which Lock Type should
     * be applied. If not specified, the default
     * value is QMI_LOC_LOCK_DD_SUB, see #QMI_LOC_lock_sub_type_t */
    enum32_t subscription_type;
    /** set if lock_client is available */
    bool lock_client_available;
    /** Lock client mask. If specified by the control point, the
     * client(s) are blocked to request position.
     * Note: lock_client overwrites lock_type, see #QMI_LOC_lock_client_t */
    enum64_t lock_client;
} loc_set_engine_lock_t;

/**
 * @brief Pack QMI_LOC_SET_ENGINE_LOCK (58 - 0x003A) request
 *
 * This function packs a qmi request to set the location engine lock.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #loc_set_engine_lock_t
 */
QMI_SDK_error_t telit_loc_set_engine_lock_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_set_engine_lock_t *input);

/**
 * @brief Unpack QMI_LOC_SET_ENGINE_LOCK (58 - 0x003A) request
 *
 * This function unpacks the result of a QMI_LOC_SET_ENGINE_LOCK request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_set_engine_lock_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_LOC_GET_ENGINE_LOCK request
 *
 */
typedef struct {
    /** set if subscription_type is available */
    bool subscription_type_available;
    /** Subscription associated with the requested engine lock state.
     * Only a subset of the enum is valid, QMI_LOC_LOCK_ALL_SUB is invalid.
     * If not specified, the default value is QMI_LOC_LOCK_DD_SUB, see #QMI_LOC_lock_sub_type_t */
    enum32_t subscription_type;
} loc_get_engine_lock_t;

/**
 * @brief Pack QMI_LOC_GET_ENGINE_LOCK (59 - 0x003B) request
 *
 * This function packs a qmi request to get the location engine lock.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #loc_get_engine_lock_t
 */
QMI_SDK_error_t telit_loc_get_engine_lock_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_get_engine_lock_t *input);

/**
 * @brief Unpack QMI_LOC_GET_ENGINE_LOCK (59 - 0x003B) request
 *
 * This function unpacks the result of a QMI_LOC_GET_ENGINE_LOCK request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_get_engine_lock_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_LOC_GET_TTFF_FOXCONN (4103 - 0x1007) request
 *
 * This function packs a qmi request to get the time to first fix
 * (Foxconn modems)
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_loc_get_ttff_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_LOC_GET_TTFF_FOXCONN (4103 - 0x1007) request
 *
 * This function unpacks the result of a QMI_LOC_GET_TTFF_FOXCONN request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_get_ttff_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_LOC_REGISTER_MASTER_CLIENT request
 *
 */
typedef struct {
    /** Master Client 32-bit Key */
    uint32_t key;
} loc_master_client_t;

/**
 * @brief QMI LOC master client status values
 */
typedef enum {
    /** Request was completed successfully - the associated control point is now the master client */
    QMI_LOC_REGISTER_MASTER_CLIENT_SUCCESS                    = 0,
    /** Request failed because the supplied master key is invalid */
    QMI_LOC_REGISTER_MASTER_CLIENT_INVALID_KEY                = 1,
    /** Request failed because there is already a master client registered */
    QMI_LOC_REGISTER_MASTER_CLIENT_ALREADY_HAVE_MASTER_CLIENT = 2,
    /** Request failed due to unforeseen internal error */
    QMI_LOC_REGISTER_MASTER_CLIENT_INTERNAL_FAILURE           = 3,
} QMI_LOC_master_client_status_t;

/**
 * @brief input type for QMI_LOC_REGISTER_MASTER_CLIENT request
 *
 */
typedef struct {
    /** Register Master Client Status, see #QMI_LOC_master_client_status_t */
    enum32_t status;
} loc_master_client_status_t;

/**
 * @brief Pack QMI_LOC_REGISTER_MASTER_CLIENT (186 - 0x00BA) request
 *
 * This function packs a qmi request used by a control point
 * to register itself as a master client
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #loc_master_client_t
 */
QMI_SDK_error_t telit_loc_reg_master_client_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_master_client_t *input);

/**
 * @brief Unpack QMI_LOC_REGISTER_MASTER_CLIENT (186 - 0x00BA) request
 *
 * This function unpacks the result of a QMI_LOC_REGISTER_MASTER_CLIENT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_reg_master_client_unpack(uint8_t *rsp,
        uint16_t len);

/**
 * @brief QMI LOC common data
 */
typedef enum {
    /** Position estimate; common for all GNSS types */
    QMI_LOC_DELETE_COMMON_MASK_POS           = 0x00000001,
    /** Reset all CLOCK_INFO mask */
    QMI_LOC_DELETE_COMMON_MASK_TIME          = 0x00000002,
    /** UTC estimate */
    QMI_LOC_DELETE_COMMON_MASK_UTC           = 0x00000004,
    /** RTI */
    QMI_LOC_DELETE_COMMON_MASK_RTI           = 0x00000008,
    /** Frequency bias estimate; common for all GNSS types */
    QMI_LOC_DELETE_COMMON_MASK_FREQ_BIAS_EST = 0x00000010,
} QMI_LOC_common_data_t;

/**
 * @brief QMI LOC system values
 */
typedef enum {
    /** System GPS data */
    QMI_LOC_SYSTEM_GPS   = 0x00000001,
    /** System GLONASS data */
    QMI_LOC_SYSTEM_GLO   = 0x00000002,
    /** System BDS data */
    QMI_LOC_SYSTEM_BDS   = 0x00000004,
    /** System Galileo data */
    QMI_LOC_SYSTEM_GAL   = 0x00000008,
    /** System QZSS data */
    QMI_LOC_SYSTEM_QZSS  = 0x00000010,
    /** System NavIC data */
    QMI_LOC_SYSTEM_NAVIC = 0x00000020,
} QMI_LOC_system_t;

/**
 * @brief QMI LOC satellite data
 */
typedef enum {
    /** Ephemeris */
    QMI_LOC_DELETE_DATA_MASK_EPHEMERIS   = 0x00000001,
    /** Almanac */
    QMI_LOC_DELETE_DATA_MASK_ALMANAC     = 0x00000002,
    /** SV health */
    QMI_LOC_DELETE_DATA_MASK_SVHEALTH    = 0x00000004,
    /** SV direction */
    QMI_LOC_DELETE_DATA_MASK_SVDIR       = 0x00000008,
    /** SV steer */
    QMI_LOC_DELETE_DATA_MASK_SVSTEER     = 0x00000010,
    /** Almanac correction */
    QMI_LOC_DELETE_DATA_MASK_ALM_CORR    = 0x00000020,
    /** Blacklist SVs */
    QMI_LOC_DELETE_DATA_MASK_BLACKLIST   = 0x00000040,
    /** Sensitivity assistance data */
    QMI_LOC_DELETE_DATA_MASK_SA_DATA     = 0x00000080,
    /** SV does not exist */
    QMI_LOC_DELETE_DATA_MASK_SV_NO_EXIST = 0x00000100,
    /** Ionosphere correction */
    QMI_LOC_DELETE_DATA_MASK_IONO        = 0x00000200,
    /** Reset satellite time */
    QMI_LOC_DELETE_DATA_MASK_TIME        = 0x00000400,
    /** Delete multiband data */
    QMI_LOC_DELETE_DATA_MASK_MB_DATA     = 0x00000800,
    /** Reset TGD (group delay) data */
    QMI_LOC_DELETE_DATA_MASK_TGD_DATA    = 0x00001000,
} QMI_LOC_satellite_data_t;

/**
 * @brief QMI LOC GNSS Service Data type
 *
 */
typedef struct {
    /** Indicates which satellite system’s data is
     * to delete. The control point can delete
     * multiple systems at a time, see #QMI_LOC_system_t */
    enum32_t system_mask;
    /** Requested bitmask of data to delete for
     * the specified satellite system, see #QMI_LOC_satellite_data_t */
    enum32_t delete_satellite_data_mask;
} loc_sat_sys_data_t;

/**
 * @brief QMI LOC extended clock info
 */
typedef enum {
    /** Delete NavIC-to-GPS time bias-related information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_NAVIC_TO_GPS_TB = 0x000001,
    /** Delete NavIC-to-GLO time bias-related information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_NAVIC_TO_GLO_TB = 0x000002,
    /** Delete NavIC-to-BDS time bias-related information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_NAVIC_TO_BDS_TB = 0x000004,
    /** Delete NavIC-to-GAL time bias-related information */
    QMI_LOC_MASK_DELETE_CLOCK_INFO_NAVIC_TO_GAL_TB = 0x000008,
} QMI_LOC_ext_clock_info_t;

/**
 * @brief input type for QMI_LOC_DELETE_GNSS_SERVICE_DATA request
 *
 */
typedef struct {
    /** Indicates whether to delete all GNSS service data.
     *
     * Values:
     * OxO1 (TRUE): Reset all constellations’ service data;
     *              if this flag is set, all the other information contained
     *              in the optional fields for this messag are ignored.
     * 0x00 (FALSE): The optional fields in the message are used to
     *               determine which data to delete */
    bool delete_all;
    /** set if clock_info_mask is available */
    bool clock_info_mask_available;
    /** Mask for the clock information service data
     * to delete, see #QMI_LOC_delete_clock_info_t */
    enum32_t clock_info_mask;
    /** set if cell_db_data_mask is available */
    bool cell_db_data_mask_available;
    /** Mask for the cell database service data to
     * delete; common for all GNSS types,
     * see #QMI_LOC_delete_cell_db_data_t */
    enum32_t cell_db_data_mask;
    /** set if common_data_mask is available */
    bool common_data_mask_available;
    /** Mask for the common service data to delete,
     * see #QMI_LOC_common_data_t */
    enum32_t common_data_mask;
    /** set if satellite_sys_data is available */
    bool satellite_sys_data_available;
    /** see #loc_sat_sys_data_t */
    loc_sat_sys_data_t satellite_sys_data;
    /** set if ext_clock_info_mask is available */
    bool ext_clock_info_mask_available;
    /** Extended mask for the clock information
     * service data to delete, see #QMI_LOC_ext_clock_info_t */
    enum64_t ext_clock_info_mask;
} loc_delete_gnss_service_data_t;

/**
 * @brief Pack QMI_LOC_DELETE_GNSS_SERVICE_DATA (166 - 0x00A6) request
 *
 * This function packs a qmi request to delete the location engine service data from memory.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #loc_delete_gnss_service_data_t
 */
QMI_SDK_error_t telit_loc_delete_gnss_service_data_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        loc_delete_gnss_service_data_t *input);

/**
 * @brief Unpack QMI_LOC_DELETE_GNSS_SERVICE_DATA (166 - 0x00A6) request
 *
 * This function unpacks the result of a QMI_LOC_DELETE_GNSS_SERVICE_DATA request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_loc_delete_gnss_service_data_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief QMI LOC indication status
 */
typedef enum {
    /** Request was completed successfully */
    QMI_LOC_SUCCESS                    = 0,
    /** Request failed because of a general failure */
    QMI_LOC_GENERAL_FAILURE            = 1,
    /** Request failed because it is not supported */
    QMI_LOC_UNSUPPORTED                = 2,
    /** Request failed because it contained invalid parameters */
    QMI_LOC_INVALID_PARAMETER          = 3,
    /** Request failed because the engine is busy */
    QMI_LOC_ENGINE_BUSY                = 4,
    /** Request failed because the phone is offline */
    QMI_LOC_PHONE_OFFLINE              = 5,
    /** Request failed because it timed out */
    QMI_LOC_TIMEOUT                    = 6,
    /** Request failed because an undefined configuration was requested */
    QMI_LOC_CONFIG_NOT_SUPPORTED       = 7,
    /** Request failed because the engine could not allocate sufficient memory for the request */
    QMI_LOC_INSUFFICIENT_MEMORY        = 8,
    /** Request failed because the maximum number of Geofences are already programmed */
    QMI_LOC_GEOFENCE_PROGRAMMED        = 9,
    /** Location service failed because of an XTRA version-based file format check failure */
    QMI_LOC_XTRA_VERSION_CHECK_FAILURE = 10,
    /** Request failed because the location service is disabled */
    QMI_LOC_GNSS_DISABLED              = 11,
} QMI_LOC_indication_status_t;

/**
 * @brief QMI LOC delete assist data ind type
 *
 */
typedef struct {
    /** Status of the indication, see #QMI_LOC_indication_status_t */
    enum32_t status;
} loc_status_ind_t;

#define QMI_LOC_NMEA_STRING_MAX_LEN 201

/**
 * @brief QMI LOC session status
 */
typedef enum {
    /** Session was successful */
    QMI_LOC_SESS_STATUS_SUCCESS         = 0,
    /** Session is still in progress */
    QMI_LOC_SESS_STATUS_IN_PROGRESS     = 1,
    /** Session failed */
    QMI_LOC_SESS_STATUS_GENERAL_FAILURE = 2,
    /** Session timed-out */
    QMI_LOC_SESS_STATUS_TIMEOUT         = 3,
    /** Session ended by the user */
    QMI_LOC_SESS_STATUS_USER_END        = 4,
    /** Session failed due to bad parameter in request */
    QMI_LOC_SESS_STATUS_BAD_PARAMETER   = 5,
    /** Session failed because the phone is offline */
    QMI_LOC_SESS_STATUS_PHONE_OFFLINE   = 6,
    /** Session failed because the engine is locked */
    QMI_LOC_SESS_STATUS_ENGINE_LOCKED   = 7,
} QMI_LOC_session_status_t;

/**
 * @brief QMI LOC reliability
 */
typedef enum {
    /** Reliability not set */
    QMI_LOC_RELIABILITY_NOT_SET  = 0,
    /** Reliability is very low */
    QMI_LOC_RELIABILITY_VERY_LOW = 1,
    /** Reliability is low */
    QMI_LOC_RELIABILITY_LOW      = 2,
    /** Reliability is medium */
    QMI_LOC_RELIABILITY_MEDIUM   = 3,
    /** Reliability is high */
    QMI_LOC_RELIABILITY_HIGH     = 4,
} QMI_LOC_reliability_t;

/**
 * @brief QMI LOC Technology used for a fix
 */
typedef enum {
    /** Satellites */
    QMI_LOC_TECH_MASK_SATELLITE                = 0x00000001,
    /** Cell towers */
    QMI_LOC_TECH_MASK_CELL_ID                  = 0x00000002,
    /** Wi-fi access points */
    QMI_LOC_TECH_MASK_WIFI                     = 0x00000004,
    /** Sensors */
    QMI_LOC_TECH_MASK_SENSORS                  = 0x00000008,
    /** Reference location */
    QMI_LOC_TECH_MASK_REFERENCE_LOCATION       = 0x00000010,
    /** Injected coarse position */
    QMI_LOC_TECH_MASK_INJECTED_COARSE_POSITION = 0x00000020,
    /** AFLT */
    QMI_LOC_TECH_MASK_AFLT                     = 0x00000040,
    /** GNSS and network provided measurements */
    QMI_LOC_TECH_MASK_HYBRID                   = 0x00000080,
} QMI_LOC_technology_t;

/**
 * @brief QMI LOC time source
 */
typedef enum {
    /** Invalid time */
    QMI_LOC_TIME_SRC_INVALID                        = 0,
    /** Time is set by the 1X system */
    QMI_LOC_TIME_SRC_NETWORK_TIME_TRANSFER          = 1,
    /** Time is set by WCDMA/GSM time tagging */
    QMI_LOC_TIME_SRC_NETWORK_TIME_TAGGING           = 2,
    /** Time is set by an external injection */
    QMI_LOC_TIME_SRC_NETWORK_EXTERNAL_INPUT         = 3,
    /** Time is set after decoding over-the-air GPS navigation data from one GPS satellite */
    QMI_LOC_TIME_SRC_NETWORK_TOW_DECODE             = 4,
    /** Time is set after decoding over-the-air GPS navigation data from multiple satellites */
    QMI_LOC_TIME_SRC_NETWORK_TOW_CONFIRMED          = 5,
    /** Both time of the week and the GPS week number are known */
    QMI_LOC_TIME_SRC_NETWORK_TOW_AND_WEEK_CONFIRMED = 6,
    /** Time is set by the position engine after the fix is obtained */
    QMI_LOC_TIME_SRC_NAV_SOLUTION                   = 7,
    /** Time is set by the position engine after performing SFT; this is done when the clock time uncertainty is large */
    QMI_LOC_TIME_SRC_SOLVE_FOR_TIME                 = 8,
    /** Time is set after decoding GLO satellites */
    QMI_LOC_TIME_SRC_GLO_TOW_DECODE                 = 9,
    /** Time is set after transforming the GPS to GLO time */
    QMI_LOC_TIME_SRC_TIME_TRANSFORM                 = 10,
    /** Time is set by the sleep time tag provided by the WCDMA network */
    QMI_LOC_TIME_SRC_WCDMA_SLEEP_TIME_TAGGING       = 11,
    /** Time is set by the sleep time tag provided by the GSM network */
    QMI_LOC_TIME_SRC_GSM_SLEEP_TIME_TAGGING         = 12,
    /** Source of the time is unknown */
    QMI_LOC_TIME_SRC_UNKNOWN                        = 13,
    /** Time is derived from the system clock (better known as the slow clock); GNSS time is maintained irrespective of the GNSS receiver state */
    QMI_LOC_TIME_SRC_SYSTEM_TIMETICK                = 14,
    /** Time is set after decoding QZSS satellites */
    QMI_LOC_TIME_SRC_QZSS_TOW_DECODE                = 15,
    /** Time is set after decoding BDS satellites */
    QMI_LOC_TIME_SRC_BDS_TOW_DECODE                 = 16,
    /** Time is set after decoding GAL satellites */
    QMI_LOC_TIME_SRC_GAL_TOW_DECODE                 = 16,
} QMI_LOC_time_source_t;

/**
 * @brief QMI LOC diluition of precision type
 *
 */
typedef struct {
    /** Position dilution of precision, range 1 (highest accuracy) to 50 (lowest accuracy) where PDOP = square root of (HDOP^2 +
       VDOP^2) */
    float_t PDOP;
    /** Horizontal dilution of precision, range 1 (highest accuracy) to 50 (lowest accuracy) */
    float_t HDOP;
    /** Vertical dilution of precision, range 1 (highest accuracy) to 50 (lowest accuracy) */
    float_t VDOP;
} loc_precision_diluition_t;

/**
 * @brief QMI LOC GPS time type
 *
 */
typedef struct {
    /** Current GPS week as calculated from midnight, Jan. 6, 1980 */
    uint16_t gps_week;
    /** Amount of time into the current GPS
       week, in milliseconds */
    uint32_t gps_time_of_week;
} loc_gps_time_t;

/**
 * @brief QMI LOC sensor used for a fix
 */
typedef enum {
    /** Accelerometer */
    QMI_LOC_SENSOR_ACCEL = 0x00000001,
    /** Gyroscope */
    QMI_LOC_SENSOR_GYRO  = 0x00000002,
} QMI_LOC_sensor_usage_t;

/**
 * @brief QMI LOC aiding indicator
 */
typedef enum {
    /** Heading */
    QMI_LOC_AIDED_HEADING  = 0x00000001,
    /** Speed */
    QMI_LOC_AIDED_SPEED    = 0x00000002,
    /** Position */
    QMI_LOC_AIDED_POSITION = 0x00000004,
    /** Velocity */
    QMI_LOC_AIDED_VELOCITY = 0x00000008,
} QMI_LOC_aiding_indicator_t;

/**
 * @brief QMI LOC sensor data usage type
 *
 */
typedef struct {
    /** sensor used in calculating the position, see #QMI_LOC_sensor_usage_t */
    enum32_t sensor;
    /** results aided by sensors, see #QMI_LOC_aiding_indicator_t */
    enum32_t aiding_indicator;
} loc_sensor_data_usage_t;

#define QMI_LOC_MAX_GNSS_SV_USED 64

/**
 * @brief QMI LOC position report type
 *
 */
typedef struct {
    /** Status of the indication, see #QMI_LOC_session_status_t */
    enum32_t session_status;
    /** ID of the session as identified by the control point, set in the start request */
    uint8_t session_id;
    /** set if latitude is available */
    bool latitude_available;
    /** Latitude (specified in WGS84 datum). Floating point type in degrees with range -90.0 to 90.0. Positive values indicate northern latitude, while negative shouthern */
    double_t latitude;
    /** set if longitude is available */
    bool longitude_available;
    /** Longitude (specified in WGS84 datum). Floating point type in degrees with range -180.0 to 180.0. Positive values indicate eastern latitude, while negative western */
    double_t longitude;
    /** set if hor_unc_circular is available */
    bool hor_unc_circular_available;
    /** Horizontal position uncertainty (circular), in meters */
    float_t hor_unc_circular;
    /** set if hor_unc_ellipse_semi_minor is available */
    bool hor_unc_ellipse_semi_minor_available;
    /** Semi-minor axis of horizontal elliptical uncertainty, in meters */
    float_t hor_unc_ellipse_semi_minor;
    /** set if hor_unc_ellipse_semi_major is available */
    bool hor_unc_ellipse_semi_major_available;
    /** Semi-major axis of horizontal elliptical uncertainty, in meters */
    float_t hor_unc_ellipse_semi_major;
    bool hor_unc_ellipse_orient_azimuth_available; /** set if hor_unc_ellipse_orient_azimuth is available */
    /** Elliptical horizontal uncertainty azimuth of orientation, in decimal degrees, range 0 to 180 */
    float_t hor_unc_ellipse_orient_azimuth;
    bool hor_confidence_available;                 /** set if hor_confidence is available */
    /** Horizontal uncertainty confidence. If both elliptical and horizontal uncertainties are specified in this message, the confidence corresponds to the elliptical uncertainty, unit percent, range 0 to 99 */
    uint8_t hor_confidence;
    bool hor_reliability_available;                /** set if hor_reliability is available */
    /** Horizontal position reliability, see #QMI_LOC_reliability_t */
    enum32_t hor_reliability;
    bool speed_horizontal_available;               /** set if speed_horizontal is available */
    /** Horizontal speed (meters/second) */
    float_t speed_horizontal;
    bool speed_unc_available;                      /** set if speed_unc is available */
    /** Speed uncertainty (meters/second) */
    float_t speed_unc;
    bool altitude_wrt_ellipsoid_available;         /** set if altitude_wrt_ellipsoid is available */
    /** Altitude with respect to the WGS84 ellipsoid, in meters, range -500 to 15883 */
    float_t altitude_wrt_ellipsoid;
    bool altitude_wrt_mean_sea_level_available;    /** set if altitude_wrt_mean_sea_level is available */
    /** Altitude with respect to mean sea level, in meters */
    float_t altitude_wrt_mean_sea_level;
    bool vert_unc_available;                       /** set if vert_unc is available */
    /** Vertical uncertainty, in meters */
    float_t vert_unc;
    bool vert_confidence_available;                /** set if vert_confidence is available */
    /** Vertical uncertainty confidence, unit percent, range 0 to 99 */
    uint8_t vert_confidence;
    bool vert_reliability_available;               /** set if vert_reliability is available */
    /** Horizontal position reliability, see #QMI_LOC_reliability_t */
    enum32_t vert_reliability;
    bool speed_vertical_available;                 /** set if speed_vertical is available */
    /** Vertical speed (meters/second) */
    float_t speed_vertical;
    bool heading_available;                        /** set if heading is available */
    /** Heading, in degrees, range 0 to 359.999 */
    float_t heading;
    bool heading_unc_available;                    /** set if heading_unc is available */
    /** Heading uncertainty, in degrees, range 0 to 359.999 */
    float_t heading_unc;
    bool magnetic_deviation_available;             /** set if magnetic_deviation is available */
    /** Difference between the bearing to true north and the bearing shown on a magnetic compass. The deviation is positive when the magnetic north is east of true north */
    float_t magnetic_deviation;
    bool technology_mask_available;                /** set if technology_mask is available */
    /** Technology used in computing the fix, see #QMI_LOC_technology_t */
    enum32_t technology_mask;
    bool precision_diluition_available;            /** set if precision_diluition is available */
    /** Diluition of precision, see #loc_precision_diluition_t */
    loc_precision_diluition_t precision_diluition;
    bool utc_timestamp_available;                  /** set if utc_timestamp is available */
    uint64_t utc_timestamp;                        /** Milliseconds since Jan. 1, 1970 */
    bool leap_seconds_available;                   /** set if leap_seconds is available */
    uint8_t leap_seconds;                          /** Leap second information */
    bool gps_time_available;                       /** set if gps_time is available */
    /** GPS time, see #loc_gps_time_t */
    loc_gps_time_t gps_time;
    bool time_unc_available;                       /** set if time_unc is available */
    /** Time uncertainty, in milliseconds */
    float_t time_unc;
    bool time_source_available;                    /** set if time_source is available */
    /** Time source, see #QMI_LOC_time_source_t */
    enum32_t time_source;
    bool sensor_data_usage_available;              /** set if sensor_data_usage is available */
    /** Indicates whether sensor data was used in computing the position */
    loc_sensor_data_usage_t sensor_data_usage;
    bool fix_id_available;                         /** set if fix_id is available */
    /** Fix count for the session. Starts with 0 and increments by one for each successive position report for a particular session */
    uint32_t fix_id;
    /** number of instances of gnss_sv_used */
    uint8_t gnss_sv_used_size;
    /** Each entry in the list contains the SV ID of a satellite used for calculating this position report. The following information is associated with each SV ID: GPS (1 to 32), GLONASS (65 to 96), QZSS (193 to 197), BDS (201 to 237), GAL (301 to 336) */
    uint16_t gnss_sv_used[QMI_LOC_MAX_GNSS_SV_USED];
    bool altitude_assumed_available; /** set if altitude_assumed is available */
    uint8_t altitude_assumed;        /** 0 if altitude is calculate, 1 if assumed */
} loc_position_report_t;

/**
 * @brief QMI LOC sv processing status
 */
typedef enum {
    /** SV is not being actively processed */
    QMI_LOC_SV_STATUS_IDLE   = 1,
    /** The system is searching for this SV */
    QMI_LOC_SV_STATUS_SEARCH = 2,
    /** SV is being tracked */
    QMI_LOC_SV_STATUS_TRACK  = 3,
} QMI_LOC_sv_proc_status_t;

/**
 * @brief QMI LOC satellite info type
 *
 */
typedef struct {
    /** set if system is available */
    bool system_available;
    /** constellation of the satellite, see #QMI_LOC_SV_system_t */
    enum32_t system;
    /** set if gnss_sv_id is available */
    bool gnss_sv_id_available;
    /** GNSS SV ID of the satellite. The following information is associated with each SV ID: GPS (1 to 32), GLONASS (65 to 96), QZSS (193 to 197), BDS (201 to 237), GAL (301 to 336) */
    uint16_t gnss_sv_id;
    /** set if health_status is available */
    bool health_status_available;
    /** health status, 0 = unhealthy, 1 = healthy */
    uint8_t health_status;
    /** set if sv_status is available */
    bool sv_status_available;
    /** SV processing status, see #QMI_LOC_sv_proc_status_t */
    uint32_t sv_status;
    /** set if sv_info_mask is available */
    bool sv_info_mask_available;
    /** 1 = SV info has ephemeris, 2 = SV info has almanac */
    uint8_t sv_info_mask;
    /** set if elevation is available */
    bool elevation_available;
    /** SV elevation angle, in degrees, range 0 to 90 */
    float elevation;
    /** set if azimuth is available */
    bool azimuth_available;
    /** SV azimuth angle, in degrees, range 0 to 360 */
    float azimuth;
    /** set if snr is available */
    bool snr_available;
    /** SV signal-to-noise ratio, in dB-Hz */
    float snr;
    /* set if glo_frequency is available */
    bool glo_frequency_available;
    /** GLONASS frequency number + 8. Valid only for GLONASS systems*/
    uint8_t glo_frequency;
} loc_sv_t;

#define QMI_LOC_MAX_SV 60

/**
 * @brief QMI LOC GNSS SV info type
 *
 */
typedef struct {
    uint8_t altitude_assumed;  /** 0 if altitude is calculate, 1 if assumed */
    /** number of instances of #loc_sv_t */
    uint8_t sv_info_size;
    /** list of #loc_sv_t */
    loc_sv_t sv_info[QMI_LOC_MAX_SV];
    /** number of instances of #loc_sv_t */
    uint8_t sv_ext_info_size;
    /** list of #loc_sv_t */
    loc_sv_t sv_ext_info[QMI_LOC_MAX_SV];
} loc_gnss_sv_info_t;

/**
 * @brief QMI LOC NMEA string type
 *
 */
typedef struct {
    /** length of nmea_string */
    uint8_t nmea_string_len;
    /** NMEA NULL-terminated string */
    char nmea_string[QMI_LOC_NMEA_STRING_MAX_LEN];
} loc_nmea_ind_t;

/**
 * @brief QMI LOC NMEA time to first fix type
 *
 */
typedef struct {
    /** time to first fix in milli-seconds */
    uint32_t ttff;
} loc_ttff_ind_t;

/**
 * @brief output type for QMI_LOC_SET_ENGINE_LOCK_IND and QMI_LOC_GET_ENGINE_LOCK_IND indications
 *
 */
typedef struct {
    /** Status of the Set/Get Engine Lock request, see #QMI_LOC_indication_status_t */
    enum32_t status;
    /** set if lock_type is available */
    bool lock_type_available;
    /** Type of lock, see #QMI_LOC_engine_lock_type_t */
    enum32_t lock_type;
    /** set if subscription_type is available */
    bool subscription_type_available;
    /** Subscription to which the above Lock
     * Type is associated with, see #QMI_LOC_lock_sub_type_t */
    enum32_t subscription_type;
    /** set if lock_client is available */
    bool lock_client_available;
    /** Lock client mask. The client(s) who have been locked to
     * request position, see #QMI_LOC_lock_client_t */
    enum64_t lock_client;
} loc_lock_status_t;

#define QMI_LOC_SERVER_URL_STRING_MAX_LEN 256

/**
 * @brief QMI LOC time server info type
 *
 */
typedef struct {
    /** length of server url */
    uint8_t server_url_len;
    /** Server URL NULL-terminated string */
    char server_url[QMI_LOC_SERVER_URL_STRING_MAX_LEN];
} loc_server_url_t;

#define QMI_LOC_MAX_SERVER_URL 10

/**
 * @brief QMI LOC time server info type
 *
 */
typedef struct {
    /** length of nmea_string */
    uint32_t delay_threshold;
    /** number of instances of #loc_server_url */
    uint8_t server_urls_size;
    /** list of #loc_server_url_t */
    loc_server_url_t server_urls[QMI_LOC_MAX_SERVER_URL];
} loc_time_server_info_t;

/**
 * @brief output type for QMI_LOC_EVENT_INJECT_TIME_REQ_IND
 *
 */
typedef struct {
    /** set if time_server_info is available */
    bool time_server_info_available;
    /** Time server info, see #loc_time_server_info_t */
    loc_time_server_info_t time_server_info;
} loc_inject_time_req_t;

/**
 * @brief output type for QMI_LOC_EVENT_INJECT_POSITION_REQ_IND
 *
 */
typedef struct {
    /** Latitude in floating point, units degrees, range -90.0 to 90.0,
     * positive values indicate northern, negative values indicate southern */
    double latitude;
    /** Longitude in floating point, units degrees, range -180.0 to 180.0,
     * positive values indicate eastern, negative values indicate western */
    double longitude;
    /** Horizontal position uncertainty (circular) in meters */
    float hor_unc_circular;
    /** UTC timestamp in milliseconds since Jan. 1, 1970 */
    uint64_t timestamp_utc;
} loc_inject_position_req_t;

/**
 * @brief Unpack QMI LOC indications
 *
 * This function unpacks the result of the following loc indications
 *
 * QMI_LOC_EVENT_POSITION_REPORT_IND     (36 - 0x0024)
 * QMI_LOC_EVENT_GNSS_SV_INFO_IND        (37 - 0x0025)
 * QMI_LOC_EVENT_NMEA_IND                (38 - 0x0026)
 * QMI_LOC_EVENT_INJECT_TIME_REQ_IND     (40 - 0x0028)
 * QMI_LOC_EVENT_INJECT_POSITION_REQ_IND (42 - 0x002A)
 * QMI_LOC_INJECT_UTC_TIME_IND           (56 - 0x0038)
 * QMI_LOC_SET_ENGINE_LOCK_IND           (58 - 0x003A)
 * QMI_LOC_GET_ENGINE_LOCK_IND           (59 - 0x003B)
 * QMI_LOC_SET_NMEA_TYPES_IND            (62 - 0x003E)
 * QMI_LOC_DELETE_ASSIST_DATA_IND        (68 - 0x0044)
 * QMI_LOC_SET_OPERATION_MODE_IND        (74 - 0x004A)
 * QMI_LOC_GET_OPERATION_MODE_IND        (75 - 0x004B)
 * QMI_LOC_DELETE_GNSS_SERVICE_DATA_IND  (166 - 0x00A6)
 * QMI_LOC_REGISTER_MASTER_CLIENT_IND    (186 - 0x00BA)
 * QMI_LOC_GET_TTFF_FOXCONN_IND          (4103 - 0x1007)
 *
 * @param[in] msg_id indication id
 * @param[in] rsp indication
 * @param[in] len indication length
 * @param[out] out type according to the following
 *      QMI_LOC_EVENT_POSITION_REPORT_IND     (0x0024) -> #loc_position_report_t
 *      QMI_LOC_EVENT_GNSS_SV_INFO_IND        (0x0025) -> #loc_gnss_sv_info_t
 *      QMI_LOC_EVENT_NMEA_IND                (0x0026) -> #loc_nmea_ind_t
 *      QMI_LOC_EVENT_INJECT_TIME_REQ_IND     (0x0028) -> #loc_inject_time_req_t
 *      QMI_LOC_EVENT_INJECT_POSITION_REQ_IND (0x002A) -> #loc_inject_position_req_t
 *      QMI_LOC_INJECT_UTC_TIME_IND           (0x0038) -> #loc_status_ind_t
 *      QMI_LOC_SET_ENGINE_LOCK_IND           (0x003A) -> #loc_lock_status_t
 *      QMI_LOC_GET_ENGINE_LOCK_IND           (0x003B) -> #loc_lock_status_t
 *      QMI_LOC_SET_NMEA_TYPES_IND            (0x003E) -> #loc_status_ind_t
 *      QMI_LOC_DELETE_ASSIST_DATA_IND        (0x0044) -> #loc_status_ind_t
 *      QMI_LOC_SET_OPERATION_MODE_IND        (0x004A) -> #loc_status_ind_t
 *      QMI_LOC_GET_OPERATION_MODE_IND        (0x004B) -> #loc_status_ind_t
 *      QMI_LOC_DELETE_GNSS_SERVICE_DATA_IND  (0x00A6) -> #loc_status_ind_t
 *      QMI_LOC_REGISTER_MASTER_CLIENT_IND    (0x00BA) -> #loc_master_client_status_t
 *      QMI_LOC_GET_TTFF_FOXCONN_IND          (0x1007) -> #loc_ttff_ind_t
 * @param[in] out_len buffer size
 */
QMI_SDK_error_t telit_loc_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len,
        uint8_t *out, uint16_t out_len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_LOC_H_ */
