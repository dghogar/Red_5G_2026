/**
 * @page telitqmisdk Telit QMI SDK
 * Telit QMI SDK provides an API for encoding/decoding QMI messages
 *
 * Generic API usage:
 *
 * - Encoding
 * 1. Create the QMI request context (see #pack_qmi_t)
 * 2. Call the pack request function with the context and the input data: optional input values to be considered should have the related bool type to be set or, for strings, the string length is > 0
 *
 * - Decoding
 * 1. Call telit_helper_get_resp_ctx to retrieve response context and response data with the QMI response as returned by the modem
 * 2. Call the unpack response function with the qmi message buffer returned by telit_helper_get_resp_ctx: optional values in the output struct are valid only if the related bool type is set or, for string, the string length is > 0
 *
 */

/**
 * @file telit_sdk.h
 * @brief SDK core functions
 */

#ifndef TELIT_SDK_H_
#define TELIT_SDK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#if defined(ANDROID)
#include <utils/Log.h>
#elif defined (__WINDOWS_QMI__)
#define LOG_EMERG   0
#define LOG_ALERT   1
#define LOG_CRIT    2
#define LOG_ERR     3
#define LOG_WARNING 4
#define LOG_NOTICE  5
#define LOG_INFO    6
#define LOG_DEBUG   7

#define __unused
#else
#include <syslog.h>

#define __unused __attribute__((unused))
#endif

#define MAX_STRING_SIZE 255
#define QMI_MSG_MAX 65535

#define IPV6_ADDR_SIZE 8

#if !defined(ANDROID)
#define RLOGD(format, ...) telit_log_print(LOG_DEBUG, format, ## __VA_ARGS__)
#define RLOGW(format, ...) telit_log_print(LOG_WARNING, format, ## __VA_ARGS__)
#define RLOGI(format, ...) telit_log_print(LOG_INFO, format, ## __VA_ARGS__)
#define RLOGE(format, ...) telit_log_print(LOG_ERR, format, ## __VA_ARGS__)
#endif

typedef uint8_t enum8_t;
typedef uint16_t enum16_t;
typedef uint32_t enum32_t;
typedef int32_t enum32s_t;
typedef uint64_t enum64_t;

/**
 * @brief enum to describe QMI message types
 */
typedef enum {
    /** Message is a response */
    QMI_MSG_RESPONSE   = 2,
    /** Message is an indication */
    QMI_MSG_INDICATION = 4,
} qmi_msg_type_t;

/**
 * @brief qmi request context
 */
typedef struct {
    /** Transaction ID */
    uint16_t xid;
    /** Recommended timeout in seconds */
    int timeout;
    /** Message ID */
    uint16_t msg_id;
    /** Service ID */
    uint8_t svc_id;
} pack_qmi_t;

/**
 * @brief qmi response context
 */
typedef struct {
    /** QMI message type, see #qmi_msg_type_t */
    qmi_msg_type_t type;
    /** Message ID */
    uint16_t msg_id;
    /** Transaction ID */
    uint16_t xid;
} unpack_qmi_t;

/**
 * @brief Size-bounded string copying.
 *
 * The strlcpy() function copy strings. It is designed to be safer, more
 * consistent, and less error prone replacement for strncpy. Unlike that
 * function, strlcpy() take the full size of the buffer (not just the length)
 * and guarantee to NUL-terminate the result (as long as size is larger than 0).
 * Note that a byte for the NUL should be included in size. Also note that
 * strlcpy() only operate on true “C” strings. This means that for strlcpy() src
 * must be NUL-terminated.
 *
 * The strlcpy() function copies up to size - 1 characters from the
 * NUL-terminated string src to dst, NUL-terminating the result.
 *
 * @param dst a buffer where the string is copied
 * @param src a string to be copied
 * @param size the size of the destination buffer
 * @return length of the string to be copied
 */
#ifndef HAVE_STRLCPY
size_t strlcpy(char *dst, const char *src, size_t size);
#endif

/**
 * @brief This function returns Telit SDK version
 */
const char *telit_get_version();

/**
 * @brief This function fills the response context
 * @param[in] svc service number
 * @param[in] rsp response buffer
 * @param[in] len response len
 * @param[out] ctx response context
 */
const char *telit_helper_get_resp_ctx(uint8_t svc,
        uint8_t *rsp,
        uint32_t len,
        unpack_qmi_t *ctx);

/**
 * @brief QMI SDK API error codes
 *
 * QMI protocol errors have an offset of 1000
 */
typedef enum {
    QMI_SDK_ERR_ENUM_BEGIN         = -1,

    /** Success */
    QMI_SDK_ERR_NONE               = 0,
    /** Internal error */
    QMI_SDK_ERR_INTERNAL           = 1,
    /** Invalid argument */
    QMI_SDK_ERR_INVALID_ARG        = 2,
    /** Buffer too small for holding request/response */
    QMI_SDK_ERR_BUFFER_SZ          = 3,
    /** Malformed response received */
    QMI_SDK_ERR_MALFORMED_RSP      = 4,
    /** Invalid/error response received */
    QMI_SDK_ERR_INVALID_RSP        = 5,
    /** Unsupported operation */
    QMI_SDK_ERR_UNSUPPORTED        = 6,

    QMI_SDK_ERR_ERR_ENUM_END,

    /** (Telit SDK internal) Missing TLV */
    QMI_SDK_ERR_MISSING_TLV        = 100,
    /** (Telit SDK internal) QMI protocol error */
    QMI_SDK_ERR_QMI_PROTOCOL_ERROR = 101,

    /* Offset from which mapped QMI error codes start from (see tQMIErrors) */
    QMI_SDK_ERR_ERR_QMI_OFFSET     = 1000
} QMI_SDK_error_t;

/**
 * @brief This function returns #QMI_SDK_error_t
 * @param[in] rsp response buffer
 * @param[in] len response len
 * @param[out] svc service id
 * @param[out] cid client id
 */
QMI_SDK_error_t telit_helper_qmux_get_svc_cli_id(uint8_t *rsp, uint32_t len,
        uint8_t *svc, uint8_t *cid);

/**
 * @brief Enum to describe QMI IP family
 *
 */
typedef enum {
    /** IPv4 */
    QMI_IP_FAMILY_V4          = 0x04,
    /** IPv6 */
    QMI_IP_FAMILY_V6          = 0x06,
    /** Unspecified */
    QMI_IP_FAMILY_UNSPECIFIED = 0x08,
} QMI_ip_family_t;

/**
 * @brief Enum to describe network type
 *
 */
typedef enum {
    /** Unknown */
    QMI_NETWORK_TYPE_UNKNOWN = 0x00,
    /** 3GPP */
    QMI_NETWORK_TYPE_3GPP    = 0x01,
    /** 3GPP2 */
    QMI_NETWORK_TYPE_3GPP2   = 0x02,
} QMI_network_type_t;

/**
 * @brief Constants to describe QMI bands
 *
 */
/** Band class 0, A-system */
#define QMI_BAND_CLASS_0_A          (1ULL << 0)
/** Band class 0, B-system */
#define QMI_BAND_CLASS_0_B          (1ULL << 1)
/** Band class 1, all blocks */
#define QMI_BAND_CLASS_1            (1ULL << 2)
/** Band class 2 */
#define QMI_BAND_CLASS_2            (1ULL << 3)
/** Band class 3, A-system */
#define QMI_BAND_CLASS_3_A          (1ULL << 4)
/** Band class 4, all blocks */
#define QMI_BAND_CLASS_4            (1ULL << 5)
/** Band class 5, all blocks */
#define QMI_BAND_CLASS_5            (1ULL << 6)
/** GSM DCS band */
#define QMI_BAND_GSM_DCS            (1ULL << 7)
/** GSM Extended GSM (E-GSM) band (900) */
#define QMI_BAND_GSM_E_GSM          (1ULL << 8)
/** GSM Primary GSM (P-GSM) band (900) */
#define QMI_BAND_GSM_PRIMARY_GSM    (1ULL << 9)
/** Band class 6 */
#define QMI_BAND_CLASS_6            (1ULL << 10)
/** Band class 7 */
#define QMI_BAND_CLASS_7            (1ULL << 11)
/** Band class 8 */
#define QMI_BAND_CLASS_8            (1ULL << 12)
/** Band class 9 */
#define QMI_BAND_CLASS_9            (1ULL << 13)
/** Band class 10 */
#define QMI_BAND_CLASS_10           (1ULL << 14)
/** Band class 11 */
#define QMI_BAND_CLASS_11           (1ULL << 15)
/** GSM 450 band */
#define QMI_BAND_GSM_450            (1ULL << 16)
/** GSM 480 band */
#define QMI_BAND_GSM_480            (1ULL << 17)
/** GSM 750 band */
#define QMI_BAND_GSM_750            (1ULL << 18)
/** GSM 850 band */
#define QMI_BAND_GSM_850            (1ULL << 19)
/** GSM railways GSM band (900) */
#define QMI_BAND_GSM_RAILWAYS_900   (1ULL << 20)
/** GSM PCS band (1900) */
#define QMI_BAND_GSM_PCS_1900       (1ULL << 21)
/** WCDMA (Europe, Japan, and China) 2100 band */
#define QMI_BAND_WCDMA_2100         (1ULL << 22)
/** WCDMA US PCS 1900 band */
#define QMI_BAND_WCDMA_PCS_1900     (1ULL << 23)
/** WCDMA (Europe and China) DCS 1800 band */
#define QMI_BAND_WCDMA_DCS_1800     (1ULL << 24)
/** WCDMA US 1700 band */
#define QMI_BAND_WCDMA_US_1700      (1ULL << 25)
/** WCDMA US 850 band */
#define QMI_BAND_WCDMA_US_850       (1ULL << 26)
/** QWCDMA Japan 800 band */
#define QMI_BAND_QWCDMA_JP_800      (1ULL << 27)
/** Band class 12 */
#define QMI_BAND_CLASS_12           (1ULL << 28)
/** Band class 14 */
#define QMI_BAND_CLASS_14           (1ULL << 29)
/* Bit 30 skipped */
/** Band class 15 */
#define QMI_BAND_CLASS_15           (1ULL << 31)
/* Bits 32 to 47 skipped */
/** WCDMA Europe 2600 band */
#define QMI_BAND_WCDMA_EU_2600      (1ULL << 48)
/** WCDMA Europe and Japan 900 band */
#define QMI_BAND_WCDMA_EU_JP_900    (1ULL << 49)
/** WCDMA Japan 1700 band */
#define QMI_BAND_WCDMA_JP_1700      (1ULL << 50)
/* Bits 51 to 55 skipped */
/** Band class 16 */
#define QMI_BAND_CLASS_16           (1ULL << 56)
/** Band class 17 */
#define QMI_BAND_CLASS_17           (1ULL << 57)
/** Band class 18 */
#define QMI_BAND_CLASS_18           (1ULL << 58)
/** Band class 19 */
#define QMI_BAND_CLASS_19           (1ULL << 59)
/** WCDMA Japan 850 band */
#define QMI_BAND_WCDMA_JP_850       (1ULL << 60)
/** WCDMA 1500 band */
#define QMI_BAND_WCDMA_1500         (1ULL << 61)

/**
 * @brief QMI services codes
 */
typedef enum {
    /** 0 - 0x00 Control service */
    QMI_SVC_CONTROL = 0x00,
    /** 1 - 0x01 Wireless data service */
    QMI_SVC_WDS     = 0x01,
    /** 2 - 0x02 Device management service */
    QMI_SVC_DMS     = 0x02,
    /** 3 - 0x03 Network access service */
    QMI_SVC_NAS     = 0x03,
    /** 4 - 0x04 Quality of service service */
    QMI_SVC_QOS     = 0x04,
    /** 5 - 0x05 Wireless messaging service */
    QMI_SVC_WMS     = 0x05,

    /** 9 - 0x09 Voice service */
    QMI_SVC_VOICE   = 0x09,

    /** 10 - 0x0A Card Application Toolkit */
    QMI_SVC_CAT     = 0x0A,

    /** 11 - 0x0B User Identity Module service */
    QMI_SVC_UIM     = 0x0B,

    /** 16 - 0x10 Location service */
    QMI_SVC_LOC     = 0x10,

    /** 24 - 0x18 Thermal mitigation device service */
    QMI_SVC_TMD     = 0x18,

    /** 26 - 0x1A Wireless data administrative service */
    QMI_SVC_WDA     = 0x1A,

    /** 47 - 0x2F Data port mapper service */
    QMI_SVC_DPM     = 0x2F,

    /** 230 - 0xE6 Telit implementation of OEM FTP FOTA APIs */
    QMI_SVC_FOTA    = 0xE6,
    /** 231 - 0xE7 Telit General Modem Service */
    QMI_SVC_GMS     = 0xE7,
    /** 232 - 0xE8 Telit General Application Service */
    QMI_SVC_GAS     = 0xE8,

    /** 237 - 0xED Telit AT Relay Service */
    QMI_SVC_ATR     = 0xED,
} QMI_service_t;

/**
 * @brief QMI protocol errors
 */
typedef enum {
    QMI_ERR_NONE                         = 0x00,
    QMI_ERR_MALFORMED_MSG                = 0x01,
    QMI_ERR_NO_MEMORY                    = 0x02,
    QMI_ERR_INTERNAL                     = 0x03,
    QMI_ERR_ABORTED                      = 0x04,
    QMI_ERR_CLIENT_IDS_EXHAUSTED         = 0x05,
    QMI_ERR_UNABORTABLE_TRANSACTION      = 0x06,
    QMI_ERR_INVALID_CLIENT_ID            = 0x07,
    QMI_ERR_NO_THRESHOLDS                = 0x08,
    QMI_ERR_INVALID_HANDLE               = 0x09,
    QMI_ERR_INVALID_PROFILE              = 0x0A,
    QMI_ERR_INVALID_PINID                = 0x0B,
    QMI_ERR_INCORRECT_PIN                = 0x0C,
    QMI_ERR_NO_NETWORK_FOUND             = 0x0D,
    QMI_ERR_CALL_FAILED                  = 0x0E,
    QMI_ERR_OUT_OF_CALL                  = 0x0F,
    QMI_ERR_NOT_PROVISIONED              = 0x10,
    QMI_ERR_MISSING_ARG                  = 0x11,
    QMI_ERR_ARG_TOO_LONG                 = 0x13,
    QMI_ERR_INVALID_TX_ID                = 0x16,
    QMI_ERR_DEVICE_IN_USE                = 0x17,
    QMI_ERR_OP_NETWORK_UNSUPPORTED       = 0x18,
    QMI_ERR_OP_DEVICE_UNSUPPORTED        = 0x19,
    QMI_ERR_NO_EFFECT                    = 0x1A,
    QMI_ERR_NO_FREE_PROFILE              = 0x1B,
    QMI_ERR_INVALID_PDP_TYPE             = 0x1C,
    QMI_ERR_INVALID_TECH_PREF            = 0x1D,
    QMI_ERR_INVALID_PROFILE_TYPE         = 0x1E,
    QMI_ERR_INVALID_SERVICE_TYPE         = 0x1F,
    QMI_ERR_INVALID_REGISTER_ACTION      = 0x20,
    QMI_ERR_INVALID_PS_ATTACH_ACTION     = 0x21,
    QMI_ERR_AUTHENTICATION_FAILED        = 0x22,
    QMI_ERR_PIN_BLOCKED                  = 0x23,
    QMI_ERR_PIN_PERM_BLOCKED             = 0x24,
    QMI_ERR_SIM_NOT_INITIALIZED          = 0x25,
    QMI_ERR_MAX_QOS_REQUESTS_IN_USE      = 0x26,
    QMI_ERR_INCORRECT_FLOW_FILTER        = 0x27,
    QMI_ERR_NETWORK_QOS_UNAWARE          = 0x28,
    QMI_ERR_INVALID                      = 0x29,
    QMI_ERR_REQUESTED_NUM_UNSUPPORTED    = 0x2A,
    QMI_ERR_INTERFACE_NOT_FOUND          = 0x2B,
    QMI_ERR_FLOW_SUSPENDED               = 0x2C,
    QMI_ERR_INVALID_DATA_FORMAT          = 0x2D,
    QMI_ERR_GENERAL                      = 0x2E,
    QMI_ERR_UNKNOWN                      = 0x2F,
    QMI_ERR_INVALID_ARG                  = 0x30,
    QMI_ERR_INVALID_INDEX                = 0x31,
    QMI_ERR_NO_ENTRY                     = 0x32,
    QMI_ERR_DEVICE_STORAGE_FULL          = 0x33,
    QMI_ERR_DEVICE_NOT_READY             = 0x34,
    QMI_ERR_NETWORK_NOT_READY            = 0x35,
    QMI_ERR_CAUSE_CODE                   = 0x36,
    QMI_ERR_MESSAGE_NOT_SENT             = 0x37,
    QMI_ERR_MESSAGE_DELIVERY_FAILURE     = 0x38,
    QMI_ERR_INVALID_MESSAGE_ID           = 0x39,
    QMI_ERR_ENCODING                     = 0x3A,
    QMI_ERR_AUTHENTICATION_LOCK          = 0x3B,
    QMI_ERR_INVALID_TRANSITION           = 0x3C,
    QMI_ERR_NOT_A_MCAST_IFACE            = 0x3D,
    QMI_ERR_MAX_MCAST_REQUESTS_IN_USE    = 0x3E,
    QMI_ERR_INVALID_MCAST_HANDLE         = 0x3F,
    QMI_ERR_INVALID_IP_FAMILY_PREF       = 0x40,
    QMI_ERR_SESSION_INACTIVE             = 0x41,
    QMI_ERR_SESSION_INVALID              = 0x42,
    QMI_ERR_SESSION_OWNERSHIP            = 0x43,
    QMI_ERR_INSUFFICIENT_RESOURCES       = 0x44,
    QMI_ERR_DISABLED                     = 0x45,
    QMI_ERR_INVALID_OPERATION            = 0x46,
    QMI_ERR_INVALID_QMI_CMD              = 0x47,
    QMI_ERR_TPDU_TYPE                    = 0x48,
    QMI_ERR_SMSC_ADDR                    = 0x49,
    QMI_ERR_INFO_UNAVAILABLE             = 0x4A,
    QMI_ERR_SEGMENT_TOO_LONG             = 0x4B,
    QMI_ERR_SEGMENT_ORDER                = 0x4C,
    QMI_ERR_BUNDLING_NOT_SUPPORTED       = 0x4D,
    QMI_ERR_OP_PARTIAL_FAILURE           = 0x4E,
    QMI_ERR_POLICY_MISMATCH              = 0x4F,
    QMI_ERR_SIM_FILE_NOT_FOUND           = 0x50,
    QMI_ERR_EXTENDED_INTERNAL            = 0x51,
    QMI_ERR_ACCESS_DENIED                = 0x52,
    QMI_ERR_HARDWARE_RESTRICTED          = 0x53,
    QMI_ERR_ACK_NOT_SENT                 = 0x54,
    QMI_ERR_INJECT_TIMEOUT               = 0x55,
    QMI_ERR_INCOMPATIBLE_STATE           = 0x5A,
    QMI_ERR_FDN_RESTRICT                 = 0x5B,
    QMI_ERR_SUPS_FAILURE_CAUSE           = 0x5C,
    QMI_ERR_NO_RADIO                     = 0x5D,
    QMI_ERR_NOT_SUPPORTED                = 0x5E,
    QMI_ERR_NO_SUBSCRIPTION              = 0x5F,
    QMI_ERR_CARD_CALL_CONTROL_FAILED     = 0x60,
    QMI_ERR_NETWORK_ABORTED              = 0x61,
    QMI_ERR_MSG_BLOCKED                  = 0x62,
    QMI_ERR_INVALID_SESSION_TYPE         = 0x64,
    QMI_ERR_INVALID_PB_TYPE              = 0x65,
    QMI_ERR_NO_SIM                       = 0x66,
    QMI_ERR_PB_NOT_READY                 = 0x67,
    QMI_ERR_PIN_RESTRICTION              = 0x68,
    QMI_ERR_PIN2_RESTRICTION             = 0x69,
    QMI_ERR_PUK_RESTRICTION              = 0x6A,
    QMI_ERR_PUK2_RESTRICTION             = 0x6B,
    QMI_ERR_PB_ACCESS_RESTRICTED         = 0x6C,
    QMI_ERR_PB_DELETE_IN_PROG            = 0x6D,
    QMI_ERR_PB_TEXT_TOO_LONG             = 0x6E,
    QMI_ERR_PB_NUMBER_TOO_LONG           = 0x6F,
    QMI_ERR_PB_HIDDEN_KEY_RESTRICTION    = 0x70,
    QMI_ERR_PB_NOT_AVAILABLE             = 0x71,
    QMI_ERR_DEVICE_MEMORY_ERROR          = 0x72,
    QMI_ERR_NO_PERMISSION                = 0x73,
    QMI_ERR_TOO_SOON                     = 0x74,
    QMI_ERR_TIME_NOT_ACQUIRED            = 0x75,
    QMI_ERR_OP_IN_PROGRESS               = 0x76,
    QMI_ERR_FW_WRITE_FAILED              = 0x184,
    QMI_ERR_FW_INFO_READ_FAILED          = 0x185,
    QMI_ERR_FW_FILE_NOT_FOUND            = 0x186,
    QMI_ERR_FW_DIR_NOT_FOUND             = 0x187,
    QMI_ERR_FW_ALREADY_ACTIVATED         = 0x188,
    QMI_ERR_FW_CANNOT_GENERIC_IMAGE      = 0x189,
    QMI_ERR_FW_FILE_OPEN_FAILED          = 0x190,
    QMI_ERR_FW_UPDATE_DISCONTINOUS_FRAME = 0x191,
    QMI_ERR_FW_UPDATE_FAILED             = 0x192,
    QMI_ERR_AUTO_SIM_ALREADY_ENABLED     = 0x193,
    QMI_ERR_UNLISTED                     = 0xFFFF,
} QMI_protocol_error_t;

/**
 * @brief Returns the QMI Error definition as string
 *
 * @param error a QMI_protocol_error_t value
 *
 * @return a string
 */
char *qmi_strerror(const QMI_protocol_error_t error);

/**
 * @brief Enum to describe Generic Event Action
 *
 */
typedef enum {
    /** Disable item */
    QMI_IND_ACTION_RESET = 0,
    /** Enable item */
    QMI_IND_ACTION_SET   = 1,
    /** Do not change item setting */
    QMI_IND_ACTION_NONE  = 2,
} QMI_indication_action_t;

#if !defined(ANDROID)
/**
 * @brief Enum to describe Generic Event Action
 * @param[in] level log level according to the following values:<br>
 * <br>
 * 3 (LOG_ERR) - Error conditions<br>
 * 4 (LOG_WARNING) - Warning conditions<br>
 * 6 (LOG_INFO) - Informational<br>
 * 7 (LOG_DEBUG) - Debug-level messages<br>
 *
 */
int telit_set_log_level(int level);

/**
 * @brief This function set syslog as log output
 * @param[in] enable true to enable syslog output
 */
void telit_set_syslog(bool enable);

/**
 * @brief Logging function
 * @param[in] level logging level, see telit_set_log_level for values
 * @param[in] fmt format string
 */
void telit_log_print(int level, char *fmt, ...);
#else
#define LOG_ERR 3
#define LOG_WARNING 4
#define LOG_INFO 6
#define LOG_DEBUG 7
#endif

/**
 * @brief This function prints a buffer at debug level (16 bytes per row)
 * @param[in] buffer buffer to be printed
 * @param[in] len actual length of the buffer
 * @param[in] level logging level
 */
void telit_print_buffer(uint8_t *buffer, uint32_t len, int level);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_H_ */
