#ifndef TELIT_SDK_GMS_H_
#define TELIT_SDK_GMS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

#include "telit_sdk.h"
#include "telit_sdk_wds.h"

/**
 * @file telit_sdk_gms.h
 * @brief GMS pack/unpack functions
 * Service ID: QMI_SVC_GMS (231)
 */

/**
 * @brief Enum to describe QMI GMS Message IDs
 */
typedef enum {
    /** 256 - 0x0100 Set 3GPP configure item information */
    QMI_GMS_WDS_3GPP_CONFIG_ITEM_SET = 0x0100,
    /** 257 - 0x0101 Get 3GPP configure item information */
    QMI_GMS_WDS_3GPP_CONFIG_ITEM_GET = 0x0101,

    /** 259 - 0x0103 Create profile with index */
    QMI_GMS_WDS_CREATE_PROFILE       = 0x0103,
    /** 260 - 0x0104 Trigger APN backup on current operator */
    QMI_GMS_WDS_PRESERVED_APN_BACKUP = 0x0104,

    /** 512 - 0x0200 Get dying gasp configuration */
    QMI_GMS_DMS_GET_DYING_GASP_CFG   = 0x0200,
    /** 513 - 0x0201 Set dying gasp configuration */
    QMI_GMS_DMS_SET_DYING_GASP_CFG   = 0x0201,
    /** 514 - 0x0202 Get dying gasp statistics */
    QMI_GMS_DMS_GET_DYING_GASP_STAT  = 0x0202,
    /** 515 - 0x0203 Clear dying gasp statistics */
    QMI_GMS_DMS_CLR_DYING_GASP_STAT  = 0x0203,
    /** 516 - 0x0204 Get customization feature settings */
    QMI_GMS_DMS_GET_CUST_FEATURES    = 0x0204,
    /** 517 - 0x0205 Set customization feature settings */
    QMI_GMS_DMS_SET_CUST_FEATURES    = 0x0205,
    /** 518 - 0x0206 Get device power control status information */
    QMI_GMS_DMS_GET_PC_INFO          = 0x0206,
    /** 519 - 0x0207 Get the MTU of modem side */
    QMI_GMS_DMS_GET_MTU              = 0x0207,
    /** 520 - 0x0208 Set the MTU of modem side */
    QMI_GMS_DMS_SET_MTU              = 0x0208,

    /** 528 - 0x0210 Show the available MNO configuration list with name and version */
    QMI_GMS_DMS_FWSWITCH_INFO_EX     = 0x0210,

    /** 770 - 0x0302 Get info on modem NAS and LTE status */
    QMI_GMS_NAS_MODEM_STATUS         = 0x0302,
    /** 771 - 0x0303 Get some variables for NAS LTE CQI */
    QMI_GMS_NAS_GET_LTECQI_INFO      = 0x0303,
    /** 772 - 0x0304 Get LTE SCC RX information */
    QMI_GMS_NAS_GET_LTE_SCC_RX_INFO  = 0x0304,
    /** 773 - 0x0305 Set the OTA message indication */
    QMI_GMS_NAS_IND_REGISTER         = 0x0305,
    /** 774 - 0x0306 OTA message indication */
    QMI_GMS_NAS_OTA_MSG_REPORT_IND   = 0x0306,

    /** 777 - 0x0309 Get WCDMA and LTE band capability */
    QMI_GMS_NAS_GET_BAND_CAP         = 0x0309,

    /** 783 - 0x030f Get the carrier aggregation information of LTE and 5G CC when EN-DC is activated */
    QMI_GMS_NAS_GET_ENDC_CA_INFO     = 0x030f,

    /** 790 - 0x0316 Get some variables for NAS NR5G CA in SA mode */
    QMI_GMS_NAS_GET_NR5G_CA_INFO     = 0x0316,

    /** 794 - 0x031A Set Mobile Country Code to block the PLMN */
    QMI_GMS_NAS_SET_MCCBLOCK         = 0x031A,
    /** 795 - 0x031B Get Mobile Country Code to block the PLMN */
    QMI_GMS_NAS_GET_MCCBLOCK         = 0x031B,

    /** 4097 - 0x1001 Get GNSS auto start settings */
    QMI_GMS_LOC_GET_AUTO_START       = 0x1001,
    /** 4098 - 0x1002 Get GNSS auto start settings */
    QMI_GMS_LOC_SET_AUTO_START       = 0x1002,
    /** 4099 - 0x1003 Set NMEA type setting */
    QMI_GMS_LOC_SET_NMEA_TYPE        = 0x1003,

    /** 4103 - 0x1007 Set GNSS session mode */
    QMI_GMS_LOC_SET_GNSS_SESSION     = 0x1007,
    /** 4104 - 0x1008 Get GNSS session mode */
    QMI_GMS_LOC_GET_GNSS_SESSION     = 0x1008,
    /** 4105 - 0x1009 Get acquired position */
    QMI_GMS_LOC_GET_ACQ_POSITION     = 0x1009,
} QMI_GMS_message_t;

/**
 * @brief GMS WDS default PDN states
 *
 */
typedef enum {
    /** PDN disabled */
    QMI_GMS_DEFAULT_PDN_STATE_DISABLED = 0x00,
    /** PDN enabled */
    QMI_GMS_DEFAULT_PDN_STATE_ENABLED  = 0x01,
    /** Invalid PDN state */
    QMI_GMS_DEFAULT_PDN_STATE_INVALID  = 0xFF,
} QMI_GMS_default_PDN_state_t;

/**
 * @brief GMS WDS 3GPP releases
 *
 */
typedef enum {
    /** Rel. 99 */
    QMI_GMS_3GPP_RELEASE_99      = 0x00,
    /** Rel. 5 */
    QMI_GMS_3GPP_RELEASE_5       = 0x01,
    /** Rel. 6 */
    QMI_GMS_3GPP_RELEASE_6       = 0x02,
    /** Rel. 7 */
    QMI_GMS_3GPP_RELEASE_7       = 0x03,
    /** Rel. 8 */
    QMI_GMS_3GPP_RELEASE_8       = 0x04,
    /** Rel. 9 */
    QMI_GMS_3GPP_RELEASE_9       = 0x05,
    /** Rel. 10 */
    QMI_GMS_3GPP_RELEASE_10      = 0x06,
    /** Rel. 11 */
    QMI_GMS_3GPP_RELEASE_11      = 0x07,
    /** Invalid release value */
    QMI_GMS_3GPP_RELEASE_INVALID = 0xFF,
} QMI_GMS_3GPP_release_t;

#define QMI_GMS_PROF_LIST_MAX_SIZE 5
#define QMI_GMS_LTE_ATTACH_PROF_LIST_MAX_SIZE 24

/**
 * @brief input type for QMI_GMS_WDS_3GPP_CONFIG_ITEM_SET request
 */
typedef struct {
    /** true if lte_attach_profile is set */
    bool lte_attach_profile_available;
    /** Value indicating the attached LTE Profile: range 1 - 24, 0xFFFF invalid data */
    uint16_t lte_attach_profile;
    /** profile_list length. 0 if profile_list is not set */
    uint16_t profile_list_size;
    /** Each element points to a single WORD value indicating profile index 0 is the total length of the profile list. The actual index order is used from index 1 to index 4. Profile values range 1 - 24, 0xFFFF invalid data */
    uint16_t profile_list[QMI_GMS_PROF_LIST_MAX_SIZE];
    /** true if default_PDN_enabled is set */
    bool default_PDN_enabled_available;
    /** Default PDN Enabled , see #QMI_GMS_default_PDN_state_t */
    enum8_t default_PDN_enabled;
    /** true if release_3gpp is set */
    bool release_3gpp_available;
    /** 3GPP release version, see #QMI_GMS_3GPP_release_t */
    enum8_t release_3gpp;
    /** Length of LTE Attach Profile List. 0 if attach_LTE_profile_list is not set */
    uint8_t attach_LTE_profile_list_size;
    /** indicating LTE Attach Profile List. Profile should exist */
    uint16_t attach_LTE_profile_list[QMI_GMS_LTE_ATTACH_PROF_LIST_MAX_SIZE];
} wds_3gpp_config_item_t;

/**
 * @brief Pack QMI_GMS_WDS_3GPP_CONFIG_ITEM_SET (256 - 0x0100) request
 *
 * This function packs a qmi request for setting the 3GPP configure item information
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_3gpp_config_item_t
 */
QMI_SDK_error_t telit_gms_wds_3gpp_config_item_set_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_3gpp_config_item_t *input);

/**
 * @brief Unpack QMI_GMS_WDS_3GPP_CONFIG_ITEM_SET (256 - 0x0100) request
 *
 * This function unpacks the result of a QMI_GMS_WDS_3GPP_CONFIG_ITEM_SET request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gms_wds_3gpp_config_item_set_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief output type for QMI_GMS_WDS_3GPP_CONFIG_ITEM_GET response
 *
 */
typedef struct {
    /** Value indicating the attached LTE Profile */
    uint16_t lte_attach_profile;
    /** profile_list length */
    uint16_t profile_list_size;
    /** Each element points to a single WORD value indicating profile index 0 is the total length of the profile list. The actual index order is used from index 1 to index 4. */
    uint16_t profile_list[QMI_GMS_PROF_LIST_MAX_SIZE];
    /** Default PDN Enabled , see #QMI_GMS_default_PDN_state_t */
    enum8_t default_PDN_enabled;
    /** 3GPP release version, see #QMI_GMS_3GPP_release_t */
    enum8_t release_3gpp;
    /** Length of LTE Attach Profile List */
    uint8_t attach_LTE_profile_list_size;
    /** indicating LTE Attach Profile List. */
    uint16_t attach_LTE_profile_list[QMI_GMS_LTE_ATTACH_PROF_LIST_MAX_SIZE];
} wds_3gpp_config_item_resp_t;

/**
 * @brief Pack QMI_GMS_WDS_3GPP_CONFIG_ITEM_GET (257 - 0x0101) request
 *
 * This function packs a qmi request to get 3GPP configure item information
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_wds_3gpp_config_item_get_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_WDS_3GPP_CONFIG_ITEM_GET (257 - 0x0101) request
 *
 * This function unpacks the result of a QMI_GMS_WDS_3GPP_CONFIG_ITEM_GET request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wds_3gpp_config_item_t
 */
QMI_SDK_error_t telit_gms_wds_3gpp_config_item_get_unpack(uint8_t *rsp, uint16_t len,
        wds_3gpp_config_item_resp_t *out);

/**
 * @brief Pack QMI_GMS_WDS_CREATE_PROFILE (259 - 0x0103) request
 *
 * This function packs a qmi request for creating a configured
 * profile with specified settings, including the index
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #wds_profile_t
 */
QMI_SDK_error_t telit_gms_wds_create_profile_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        wds_profile_t *input);

/**
 * @brief Unpack QMI_GMS_WDS_CREATE_PROFILE (259 - 0x0103) request
 *
 * This function unpacks the result of a QMI_GMS_WDS_CREATE_PROFILE request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #wds_profile_resp_t
 */
QMI_SDK_error_t telit_gms_wds_create_profile_unpack(uint8_t *rsp, uint16_t len, wds_profile_resp_t *out);

/**
 * @brief Pack QMI_GMS_WDS_PRESERVED_APN_BACKUP (260 - 0x0104) request
 *
 * This function packs a qmi request for triggering APN backup on current operator
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_wds_preserved_apn_backup_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_WDS_PRESERVED_APN_BACKUP (260 - 0x0104) request
 *
 * This function unpacks the result of a QMI_GMS_WDS_PRESERVED_APN_BACKUP request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gms_wds_preserved_apn_backup_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief output type for QMI_GMS_DMS_GET_DYING_GASP_CFG request, input for QMI_GMS_DMS_SET_DYING_GASP_CFG
 *
 */
typedef struct {
    /** sms destination number string length */
    uint8_t sms_destination_number_len;
    /** sms destination number as string of 8 bit ASCII characters (max 20 chars) */
    char sms_destination_number[MAX_STRING_SIZE];
    /** sms content string length */
    uint8_t sms_content_len;
    /** sms content as string of 8 bit ASCII characters (max 160 chars) */
    char sms_content[MAX_STRING_SIZE];
} dms_dying_gasp_cfg_t;

/**
 * @brief Pack QMI_GMS_DMS_GET_DYING_GASP_CFG (512 - 0x0200) request
 *
 * This function packs a qmi request for getting the dying gasp configuration
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_dms_get_dying_gasp_cfg_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_DMS_GET_DYING_GASP_CFG (512 - 0x0200) request
 *
 * This function unpacks the result of a QMI_GMS_DMS_GET_DYING_GASP_CFG request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #dms_dying_gasp_cfg_t
 */
QMI_SDK_error_t telit_gms_dms_get_dying_gasp_cfg_unpack(uint8_t *rsp, uint16_t len,
        dms_dying_gasp_cfg_t *out);

/**
 * @brief Pack QMI_GMS_DMS_SET_DYING_GASP_CFG (513 - 0x0201) request
 *
 * This function packs a qmi request for setting the dying gasp configuration
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #dms_dying_gasp_cfg_t
 */
QMI_SDK_error_t telit_gms_dms_set_dying_gasp_cfg_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        dms_dying_gasp_cfg_t *input);

/**
 * @brief Unpack QMI_GMS_DMS_SET_DYING_GASP_CFG (513 - 0x0201) request
 *
 * This function unpacks the result of a QMI_GMS_DMS_SET_DYING_GASP_CFG request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gms_dms_set_dying_gasp_cfg_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief output type for QMI_GMS_DMS_GET_DYING_GASP_STAT request
 *
 */
typedef struct {
    /** timestamp of the last time power loss was detected and dying gasp feature was triggered (UTC time in seconds since Jan 06, 1980, GPS epoch) */
    uint32_t timestamp;
    /** indicates whether device attempted to send SMS in the last power loss event, according to the following values:
     *
     * 0 - SMS not attempted
     * 1 - SMS attempted (network delivery not guaranteed)
     *
     */
    bool sms_attempted;
} dms_dying_gasp_stat_t;

/**
 * @brief Pack QMI_GMS_DMS_GET_DYING_GASP_STAT (514 - 0x0202) request
 *
 * This function packs a qmi request for getting the dying gasp configuration
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_dms_get_dying_gasp_stat_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_DMS_GET_DYING_GASP_STAT (514 - 0x0202) request
 *
 * This function unpacks the result of a QMI_GMS_DMS_GET_DYING_GASP_STAT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #dms_dying_gasp_stat_t
 */
QMI_SDK_error_t telit_gms_dms_get_dying_gasp_stat_unpack(uint8_t *rsp, uint16_t len,
        dms_dying_gasp_stat_t *out);

/**
 * @brief Pack QMI_GMS_DMS_CLR_DYING_GASP_STAT (515 - 0x0203) request
 *
 * This function packs a qmi request for clearing the dying gasp statistics
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_dms_clr_dying_gasp_stat_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_DMS_CLR_DYING_GASP_STAT (515 - 0x0203) request
 *
 * This function unpacks the result of a QMI_GMS_DMS_CLR_DYING_GASP_STAT request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gms_dms_clr_dying_gasp_stat_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_GMS_DMS_GET_CUST_FEATURES request
 *
 */
typedef struct {
    /** length of cust_id */
    uint8_t cust_id_len;
    /** name of the customization feature */
    char cust_id[MAX_STRING_SIZE];
} gms_dms_get_cust_features_v2_t;

/**
 * @brief Enum to describe GMS customization setting attribute
 */
typedef enum {
    /** Read-only attribute */
    QMI_GMS_ATTRIBUTE_READ_ONLY  = 0x00,
    /** Read/write attribute */
    QMI_GMS_ATTRIBUTE_READ_WRITE = 0x01,
} QMI_GMS_custom_feature_attribute_t;

#define QMI_GMS_MAX_CUST_VALUE_SIZE 8
/**
 * @brief output type for QMI_GMS_DMS_GET_CUST_FEATURES response
 *
 */
typedef struct {
    /** length of cust_id */
    uint8_t cust_id_len;
    /** name of the customization feature */
    char cust_id[MAX_STRING_SIZE];
    /** length of cust_value */
    uint8_t cust_value_len;
    /** customization setting value (Maximum 8 byte) */
    uint8_t cust_value[QMI_GMS_MAX_CUST_VALUE_SIZE];
    /** see #QMI_GMS_custom_feature_attribute_t */
    uint16_t cust_attr;
} gms_dms_get_cust_features_v2_resp_t;

/**
 * @brief Pack QMI_GMS_DMS_GET_CUST_FEATURES (516 - 0x0204) request
 *
 * This function packs a qmi request to get a custom feature configuration
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[out] input #gms_dms_get_cust_features_v2_t
 */
QMI_SDK_error_t telit_gms_dms_get_cust_features_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        gms_dms_get_cust_features_v2_t *input);

/**
 * @brief Unpack QMI_GMS_DMS_GET_CUST_FEATURES (516 - 0x0204) response
 *
 * This function unpacks a qmi respose about a custom feature configuration
 *
 * @param[out] rsp response
 * @param[out] len response length
 * @param[out] out #gms_dms_get_cust_features_v2_resp_t
 */
QMI_SDK_error_t telit_gms_dms_get_cust_features_unpack(uint8_t *rsp, uint16_t len,
        gms_dms_get_cust_features_v2_resp_t *out);

/**
 * @brief input type for QMI_GMS_DMS_SET_CUST_FEATURES request
 *
 */
typedef struct {
    /** length of cust_id */
    uint8_t cust_id_len;
    /** name of the customization feature */
    char cust_id[MAX_STRING_SIZE];
    /** length of cust_value */
    uint8_t cust_value_len;
    /** customization setting value (Maximum 8 byte) */
    uint8_t cust_value[QMI_GMS_MAX_CUST_VALUE_SIZE];
} gms_dms_set_cust_features_v2_t;

/**
 * @brief Pack QMI_GMS_DMS_SET_CUST_FEATURES (517 - 0x0205) request
 *
 * This function packs a qmi request to set a customization feature value
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[out] input #gms_dms_set_cust_features_v2_t
 */
QMI_SDK_error_t telit_gms_dms_set_cust_features_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        gms_dms_set_cust_features_v2_t *input);

/**
 * @brief Unpack QMI_GMS_DMS_SET_CUST_FEATURES (517 - 0x0205) request
 *
 * This function unpacks the result of a QMI_GMS_DMS_SET_CUST_FEATURES request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gms_dms_set_cust_features_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief GMS NAS modem modes
 *
 */
typedef enum {
    /** Powering-off */
    QMI_GMS_MODEM_MODE_POWERING_OFF    = 0x00,
    /** Factory test */
    QMI_GMS_MODEM_MODE_FACTORY_TEST    = 0x01,
    /** Offline */
    QMI_GMS_MODEM_MODE_OFFLINE         = 0x02,
    /** Offline AMPS */
    QMI_GMS_MODEM_MODE_OFFLINE_AMPS    = 0x03,
    /** Offline CDMA */
    QMI_GMS_MODEM_MODE_OFFLINE_CDMA    = 0x04,
    /** Online */
    QMI_GMS_MODEM_MODE_ONLINE          = 0x05,
    /** Low-power mode */
    QMI_GMS_MODEM_MODE_LOW_POWER_MODE  = 0x06,
    /** Resetting */
    QMI_GMS_MODEM_MODE_RESETTING       = 0x07,
    /** Network test */
    QMI_GMS_MODEM_MODE_NETWORK_TEST    = 0x08,
    /** Offline request */
    QMI_GMS_MODEM_MODE_OFFLINE_REQUEST = 0x09,
    /** Pseudo online */
    QMI_GMS_MODEM_MODE_PSEUDO_ONLINE   = 0x0A,
    /** Resetting modem */
    QMI_GMS_MODEM_MODE_RESETTING_MODEM = 0x0B,
    /** Unknown */
    QMI_GMS_MODEM_MODE_UNKNOWN         = 0xFF,
} QMI_GMS_modem_mode_t;

/**
 * @brief output type for QMI_GMS_DMS_GET_PC_INFO response
 *
 */
typedef struct {
    /** Selected operating mode, see #QMI_GMS_modem_mode_t */
    uint8_t operating_mode;
    /** true if optional lpm_flag is set */
    bool lpm_flag_available;
    /** LPM force flags
     *
     * Values (Bitmask):
     * bit0 - Tracks the state of W_DISABLE TLV, indicating state of the W_DISABLE switch
     * bit1 - Set if LPM is requested by host request, such as AT command, QMI or MBIM
     *        request. Cleared by a host request to return to online mode. Also set
     *        when the device is cold or warm booted in persistent LPM.
     * bit2 - Set when the device temperature is outside the valid operating range.
     *        Cleared if the temperature returns to the normal range.
     * bit3 - Set when the device voltage is outside the valid operating range.
     *        Cleared if the voltage returns to the normal range.
     * bit4 - Set on power up when BIOS locking is enabled.
     *        Cleared when the host has disabled the BIOS lock. (Not Supported)
     * bit 5- Set if the current device configuration does not match the GOBI
     *        image preference. (Not Supported)
     * bit6-31 - Additional LPM causes may be added to future products. (Not supported)
     */
    uint32_t lpm_flag;
    /** true if optional w_disable is set */
    bool w_disable_available;
    /** W_DISABLE switch position
     *
     * Values:
     * 0 - Switch set to ON position (activate, radio off)
     * 1 - Switch set to OFF position
     */
    uint8_t w_disable;
    /** true if optional power_off_mode is set */
    bool power_off_mode_available;
    /** Action taken when W_DISABLE is switched to the ON position.
     *
     * Values:0 - LPM
     * 1 - Shutdown (Not supported)
     * 2 - Ignore
     */
    uint8_t power_off_mode;
    /** true if optional persistent_lpm is set */
    bool persistent_lpm_available;
    /** LPM persistence
     *
     * Values:
     * 0 - Non-persistent LPM
     * 1 - Persistent LPM
     */
    uint8_t persistent_lpm;
} gms_dms_get_pc_info_resp_t;

/**
 * @brief Pack QMI_GMS_DMS_GET_PC_INFO (518 - 0x0206) request
 *
 * This function packs a qmi request to get device power control status information
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_dms_get_pc_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_DMS_GET_PC_INFO (518 - 0x0206) response
 *
 * This function unpacks the response of QMI_GMS_DMS_GET_PC_INFO request
 *
 * @param[out] rsp response
 * @param[out] len response length
 * @param[out] out #gms_dms_get_pc_info_resp_t
 */
QMI_SDK_error_t telit_gms_dms_get_pc_info_unpack(uint8_t *rsp, uint16_t len, gms_dms_get_pc_info_resp_t *out);

/**
 * @brief output type for QMI_GMS_DMS_GET_MTU response
 *
 */
typedef struct {
    /** set if mtu_size_3gpp is available */
    bool mtu_size_3gpp_available;
    /** MTU currenty used */
    uint16_t mtu_size_3gpp;
    /** set if mtu_size_hrpd is available */
    bool mtu_size_hrpd_available;
    /** hrpd MTU */
    uint16_t mtu_size_hrpd;
    /** set if mtu_size_ehrpd is available */
    bool mtu_size_ehrpd_available;
    /** ehrpd MTU */
    uint16_t mtu_size_ehrpd;
    /** set if mtu_size_usb is available */
    bool mtu_size_usb_available;
    /** MTU of rmnet interface */
    uint16_t mtu_size_usb;
} gms_dms_get_mtu_resp_t;

/**
 * @brief Pack QMI_GMS_DMS_GET_MTU (519 - 0x0207) request
 *
 * This function packs a qmi request to get the MTU sizes
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_dms_get_mtu_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_DMS_GET_MTU (519 - 0x0207) response
 *
 * This function unpacks the response of QMI_GMS_DMS_GET_MTU request
 *
 * @param[out] rsp response
 * @param[out] len response length
 * @param[out] out #gms_dms_get_mtu_resp_t
 */
QMI_SDK_error_t telit_gms_dms_get_mtu_unpack(uint8_t *rsp, uint16_t len, gms_dms_get_mtu_resp_t *out);

/**
 * @brief input type for QMI_GMS_DMS_SET_MTU request
 *
 */
typedef struct {
    /** MTU size to change */
    uint16_t size;
} gms_dms_set_mtu_t;

/**
 * @brief Pack QMI_GMS_DMS_SET_MTU (520 - 0x0208) request
 *
 * This function packs a qmi request for setting the MTU of modem side. If the
 * network receives an MTU, the device should not use a larger MTU setting
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #gms_dms_set_mtu_t
 */
QMI_SDK_error_t telit_gms_dms_set_mtu_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gms_dms_set_mtu_t *input);

/**
 * @brief Unpack QMI_GMS_DMS_SET_MTU (520 - 0x0208) request
 *
 * This function unpacks the result of a QMI_GMS_DMS_SET_MTU request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gms_dms_set_mtu_unpack(uint8_t *rsp, uint16_t len);

#define QMI_GMS_FWSWITCH_INFO_EX_LIST_MAX_SIZE 32
#define QMI_GMS_MNO_NAME_MAX_SIZE 33
#define QMI_GMS_MNO_CFG_VER_MAX_SIZE 17

/**
 * @brief fwswitch info type
 *
 */
typedef struct {
    /** Mobile Network Operator ID */
    uint8_t mno_id;
    /** Length of mno_name */
    uint8_t mno_name_len;
    /** Mobile Network Operator Name */
    char mno_name[QMI_GMS_MNO_NAME_MAX_SIZE];
    /** Length of mno_cfg_ver */
    uint8_t mno_cfg_ver_len;
    /** Mobile Network Operator version */
    char mno_cfg_ver[QMI_GMS_MNO_CFG_VER_MAX_SIZE];
} gms_dms_fwswitch_info_ex_t;

/**
 * @brief output type for QMI_GMS_DMS_FWSWITCH_INFO_EX response
 *
 */
typedef struct {
    /** info_list length */
    uint8_t info_list_size;
    /** List of #gms_dms_fwswitch_info_ex_t instances */
    gms_dms_fwswitch_info_ex_t info_list[QMI_GMS_FWSWITCH_INFO_EX_LIST_MAX_SIZE];
} gms_dms_fwswitch_info_ex_resp_t;

/**
 * @brief Pack QMI_GMS_DMS_FWSWITCH_INFO_EX (528 - 0x0210) request
 *
 * This function packs a qmi request for getting the available MNO
 * configuration list with name and version
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_dms_fwswitch_info_ex_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_DMS_FWSWITCH_INFO_EX (528 - 0x0210) request
 *
 * This function unpacks the result of a QMI_GMS_DMS_FWSWITCH_INFO_EX request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gms_dms_fwswitch_info_ex_resp_t
 */
QMI_SDK_error_t telit_gms_dms_fwswitch_info_ex_unpack(uint8_t *rsp, uint16_t len,
        gms_dms_fwswitch_info_ex_resp_t *out);

/**
 * @brief GMS NAS LTE bandwidth values
 *
 */
typedef enum {
    /** 1.4 MHz */
    QMI_GMS_LTE_BANDWIDTH_1_4_MHZ = 0x00,
    /** 3 MHz */
    QMI_GMS_LTE_BANDWIDTH_3_MHZ   = 0x01,
    /** 5 MHz */
    QMI_GMS_LTE_BANDWIDTH_5_MHZ   = 0x02,
    /** 10 MHz */
    QMI_GMS_LTE_BANDWIDTH_10_MHZ  = 0x03,
    /** 15 MHz */
    QMI_GMS_LTE_BANDWIDTH_15_MHZ  = 0x04,
    /** 20 MHz */
    QMI_GMS_LTE_BANDWIDTH_20_MHZ  = 0x05,
    /** Invalid */
    QMI_GMS_LTE_BANDWIDTH_INVALID = 0x06,
    /** Unknown */
    QMI_GMS_LTE_BANDWIDTH_UNKNOWN = 0xFF,
} QMI_GMS_LTE_bandwidth_t;

/**
 * @brief Enum to describe NAS NR5G Bandwidth values
 *
 */
typedef enum {
    /** 5 MHz */
    QMI_GMS_NR5G_BANDWIDTH_5_MHz   = 0x00,
    /** 10 MHz */
    QMI_GMS_NR5G_BANDWIDTH_10_MHz  = 0x01,
    /** 15 MHz */
    QMI_GMS_NR5G_BANDWIDTH_15_MHz  = 0x02,
    /** 20 MHz */
    QMI_GMS_NR5G_BANDWIDTH_20_MHz  = 0x03,
    /** 25 MHz */
    QMI_GMS_NR5G_BANDWIDTH_25_MHz  = 0x04,
    /** 30 MHz */
    QMI_GMS_NR5G_BANDWIDTH_30_MHz  = 0x05,
    /** 40 MHz */
    QMI_GMS_NR5G_BANDWIDTH_40_MHz  = 0x06,
    /** 50 MHz */
    QMI_GMS_NR5G_BANDWIDTH_50_MHz  = 0x07,
    /** 60 MHz */
    QMI_GMS_NR5G_BANDWIDTH_60_MHz  = 0x08,
    /** 70 MHz */
    QMI_GMS_NR5G_BANDWIDTH_70_MHz  = 0x09,
    /** 80 MHz */
    QMI_GMS_NR5G_BANDWIDTH_80_MHz  = 0x0A,
    /** 90 MHz */
    QMI_GMS_NR5G_BANDWIDTH_90_MHz  = 0x0B,
    /** 100 MHz */
    QMI_GMS_NR5G_BANDWIDTH_100_MHz = 0x0C,
    /** 200 MHz */
    QMI_GMS_NR5G_BANDWIDTH_200_MHz = 0x0D,
    /** 400 MHz */
    QMI_GMS_NR5G_BANDWIDTH_400_MHz = 0x0E,
} QMI_GMS_NR5G_bandwidth_t;

/**
 * @brief GMS NAS EMM states
 *
 */
typedef enum {
    /** None */
    QMI_GMS_EMM_STATE_NULL                           = 0,
    /** Deregistered */
    QMI_GMS_EMM_STATE_DEREGISTERED                   = 1,
    /** Registered, initiated */
    QMI_GMS_EMM_STATE_REGISTERED_INITIATED           = 2,
    /** Registered */
    QMI_GMS_EMM_STATE_REGISTERED                     = 3,
    /** Tracking area update initiated */
    QMI_GMS_EMM_STATE_TRACKING_AREA_UPDATE_INITIATED = 4,
    /** Service request initiated */
    QMI_GMS_EMM_STATE_SERVICE_REQUEST_INITIATED      = 5,
    /** Deregistered, initiated */
    QMI_GMS_EMM_STATE_DEREGISTERED_INITIATED         = 6,
} QMI_GMS_EMM_state_t;

/**
 * @brief GMS NAS EMM sub-states
 *
 */
typedef enum {
    /** Deregistered, no IMSI */
    QMI_GMS_EMM_SUB_STATE_DEREGISTERED_NO_IMSI               = 0,
    /** Deregistered PLMN search */
    QMI_GMS_EMM_SUB_STATE_DEREGISTERED_PLMN_SEARCH           = 1,
    /** Deregistered, attach needed */
    QMI_GMS_EMM_SUB_STATE_DEREGISTERED_ATTACH_NEEDED         = 2,
    /** Deregistered, no cell available */
    QMI_GMS_EMM_SUB_STATE_DEREGISTERED_NO_CELL_AVAILABLE     = 3,
    /** Deregistered, attempting to attach */
    QMI_GMS_EMM_SUB_STATE_DEREGISTERED_ATTEMPTING_TO_ATTACH  = 4,
    /** Deregistered, normal service */
    QMI_GMS_EMM_SUB_STATE_DEREGISTERED_NORMAL_SERVICE        = 5,
    /** Deregistered, limited service */
    QMI_GMS_EMM_SUB_STATE_DEREGISTERED_LIMITED_SERVICE       = 6,
    /** Registered, normal service */
    QMI_GMS_EMM_SUB_STATE_REGISTERED_NORMAL_SERVICE          = 7,
    /** Registered, update needed */
    QMI_GMS_EMM_SUB_STATE_REGISTERED_UPDATE_NEEDED           = 8,
    /** Registered, attempting to update */
    QMI_GMS_EMM_SUB_STATE_REGISTERED_ATTEMPTING_TO_UPDATE    = 9,
    /** Registered, no cell available */
    QMI_GMS_EMM_SUB_STATE_REGISTERED_NO_CELL_AVAILABLE       = 10,
    /** Registered, PLMN search */
    QMI_GMS_EMM_SUB_STATE_REGISTERED_PLMN_SEARCH             = 11,
    /** Registered, limited service */
    QMI_GMS_EMM_SUB_STATE_REGISTERED_LIMITED_SERVICE         = 12,
    /** Registered, attempting to update MM */
    QMI_GMS_EMM_SUB_STATE_REGISTERED_ATTEMPTING_TO_UPDATE_MM = 13,
    /** Registered, IMSI detach initiated */
    QMI_GMS_EMM_SUB_STATE_REGISTERED_IMSI_DETACH_INITIATED   = 14,
    /** Registered, internal sub-state */
    QMI_GMS_EMM_SUB_STATE_REGISTERED_INTERNAL_SUB_STATE      = 15,
} QMI_GMS_MM_sub_state_t;

/**
 * @brief GMS NAS EMM connection states
 *
 */
typedef enum {
    /** RRC idle */
    QMI_GMS_EMM_CONN_STATE_RRC_IDLE        = 0x00,
    /** Waiting RRCC-FM */
    QMI_GMS_EMM_CONN_STATE_WAITING_RRCC_FM = 0x01,
    /** RRCC connected */
    QMI_GMS_EMM_CONN_STATE_RRCC_CONNECTED  = 0x02,
    /** RRC idle */
    QMI_GMS_EMM_CONN_STATE_RRC_RELEASING   = 0x03,
    /** Unknown */
    QMI_GMS_EMM_CONN_STATE_UNKNOWN         = 0xFF,
} QMI_GMS_EMM_connection_state_t;

/**
 * @brief GMS NAS LTE modem status type
 */
typedef struct {
    /** LTE Band. Values: 1 ~ 40 (Band in decimal) 0xFF - Invalid */
    uint8_t band;
    /** BandWidth, see #QMI_GMS_LTE_bandwidth_t */
    enum8_t bandwidth;
    /** RX channel number in decimal.
     * E-UTRA absolute radio frequency channel number of the serving cell.
     *
     * Values: 0 ~ 65535
     *
     * 0xFFFF - Not Available */
    uint16_t rx_chan;
    /** TX channel number in decimal.
     * E-UTRA TX radio frequency channel.
     *
     * Values: 0 ~ 65535
     *
     * 0xFFFF - Not Available */
    uint16_t tx_chan;
    /** NAS Extended Mobility Management (EMM), see #QMI_GMS_EMM_state_t */
    enum8_t emm_state;
    /** NAS EMM substate, see #QMI_GMS_MM_sub_state_t */
    enum8_t emm_sub_state;
    /** MM Connected Mode State, see #QMI_GMS_EMM_connection_state_t */
    enum8_t emm_conn_state;
} nas_modem_status_lte_info_t;

/**
 * @brief GMS NAS system modes
 *
 */
typedef enum {
    /** No service */
    QMI_GMS_SYSTEM_MODE_NO_SERVICE = 0x00,
    /** AMPS */
    QMI_GMS_SYSTEM_MODE_AMPS       = 0x01,
    /** CDMA */
    QMI_GMS_SYSTEM_MODE_CDMA       = 0x02,
    /** GSM */
    QMI_GMS_SYSTEM_MODE_GSM        = 0x03,
    /** HDR */
    QMI_GMS_SYSTEM_MODE_HDR        = 0x04,
    /** WCDMA */
    QMI_GMS_SYSTEM_MODE_WCDMA      = 0x05,
    /** GPS */
    QMI_GMS_SYSTEM_MODE_GPS        = 0x06,
    /** TDSCDMA */
    QMI_GMS_SYSTEM_MODE_TDSCDMA    = 0x07,
    /** WLAN */
    QMI_GMS_SYSTEM_MODE_WLAN       = 0x08,
    /** LTE */
    QMI_GMS_SYSTEM_MODE_LTE        = 0x09,
    /** 5G_NSA */
    QMI_GMS_SYSTEM_MODE_NR5G_NSA   = 0x0A,
    /** 5G_SA */
    QMI_GMS_SYSTEM_MODE_NR5G_SA    = 0x0B,
    /** Unknown */
    QMI_GMS_SYSTEM_MODE_UNKNOWN    = 0xFF,
} QMI_GMS_system_mode_t;

/**
 * @brief GMS NAS IMS Registration States
 *
 */
typedef enum {
    /** No service */
    QMI_GMS_IMS_REG_STATE_NO_SERVICE   = 0x00,
    /** In progress */
    QMI_GMS_IMS_REG_STATE_IN_PROGRESS  = 0x01,
    /** Failed */
    QMI_GMS_IMS_REG_STATE_FAILED       = 0x02,
    /** Limited */
    QMI_GMS_IMS_REG_STATE_LIMITED      = 0x03,
    /** Full service */
    QMI_GMS_IMS_REG_STATE_FULL_SERVICE = 0x04,
    /** Unknown */
    QMI_GMS_IMS_REG_STATE_UNKNOWN      = 0xFF,
} QMI_GMS_IMS_registration_state_t;

/**
 * @brief GMS NAS packet-switched network states
 *
 */
typedef enum {
    /** Attached */
    QMI_GMS_PS_STATE_ATTACHED = 0x00,
    /** Detached */
    QMI_GMS_PS_STATE_DETACHED = 0x01,
    /** Unknown */
    QMI_GMS_PS_STATE_UNKNOWN  = 0xFF,
} QMI_GMS_PS_state_t;

/**
 * @brief GMS NAS NR5G modem status type
 *
 */
typedef struct {
    /**< NR5G Band */
    uint16_t band;
    /**< NR5G BandWidth, see #QMI_GMS_NR5G_bandwidth_t */
    enum16_t bandwidth;
    /**< NR5G ARFCN(DL) of the serving cell */
    uint32_t rx_chan;
    /**< NR5G ARFCN(UL) of the serving cell */
    uint32_t tx_chan;
    /**< Current NR5G RSSI in 1/10 dBm as measured by L1. Range : 0 to -120 dbm */
    int16_t rssi;
    /**< Current NR5G RSRP in 1/10 dBm as measured by L1. Range : -44 to -140 dbm */
    int16_t rsrp;
    /**< Current NR5G RSRQ in 1/10 dB as measured by L1. Range : -3 to -20 dbm */
    int16_t rsrq;
    /**< NR5G SNR level as a scaled integer in 1/10 dB.
     * For example -16 dB has a value of -160 and 24.6 dB has a value of 246.
     * Range of values: [-230, 400], which represents -23.0 dB to 40.0 dB */
    int16_t snr;
    /**< NR5G Physical Cell Id */
    uint16_t pci;
} nas_modem_status_nr5g_info_t;

/**
 * @brief output type for QMI_NAS_GET_MODEM_STATUS request
 *
 */
typedef struct {
    /** module temperature report in celsius degrees */
    int8_t temperature;
    /** modem operating mode, see #QMI_GMS_modem_mode_t */
    enum8_t modem_mode;
    /** Radio interface technology of the signal, see #QMI_GMS_system_mode_t */
    enum8_t system_mode;
    /** IMS registration state, see #QMI_GMS_IMS_registration_state_t */
    enum8_t ims_reg_state;
    /** PS attach state, see #QMI_GMS_PS_state_t */
    enum8_t ps_state;
    /** true if lte_info is set */
    bool lte_info_available;
    /** see #nas_modem_status_lte_info_t */
    nas_modem_status_lte_info_t lte_info;
    /**< true is optional nr5g_info is set */
    bool nr5g_info_available;
    /**< see #nas_modem_status_nr5g_info_t */
    nas_modem_status_nr5g_info_t nr5g_info;
} nas_modem_status_t;

/**
 * @brief Pack QMI_GMS_NAS_MODEM_STATUS (770 - 0x0302) request
 *
 * This function packs a qmi request for querying modem status
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_nas_modem_status_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_NAS_MODEM_STATUS (770 - 0x0302) request
 *
 * This function unpacks the result of a QMI_GMS_NAS_MODEM_STATUS request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #nas_modem_status_t
 */
QMI_SDK_error_t telit_gms_nas_modem_status_unpack(uint8_t *rsp, uint16_t len, nas_modem_status_t *out);

/**
 * @brief GMS CQI Information type
 *
 */
typedef struct {
    /** Channel Quality Indicator. Range: 0-15 */
    uint8_t cqi_0;
    /** Channel Quality Indicator. Range: 0-15 */
    uint8_t cqi_1;
} gms_nas_cqi_info_t;

/**
 * @brief output type for QMI_GMS_NAS_GET_LTECQI_INFO request
 *
 */
typedef struct {
    /** true if optional pcc_cqi_info is set */
    bool pcc_cqi_info_available;
    /** PCC CQI Information*/
    gms_nas_cqi_info_t pcc_cqi_info;
    /** true if optional scc_0_cqi_info is set */
    bool scc_0_cqi_info_available;
    /** SCC 0 CA Information */
    gms_nas_cqi_info_t scc_0_cqi_info;
    /** true if optional scc_1_cqi_info is set */
    bool scc_1_cqi_info_available;
    /** SCC 1 CA Information */
    gms_nas_cqi_info_t scc_1_cqi_info;
    /** true if optional scc_2_cqi_info is set */
    bool scc_2_cqi_info_available;
    /** SCC 2 CA Information */
    gms_nas_cqi_info_t scc_2_cqi_info;
    /** true if optional scc_3_cqi_info is set */
    bool scc_3_cqi_info_available;
    /** SCC 3 CA Information */
    gms_nas_cqi_info_t scc_3_cqi_info;
    /** true if optional scc_4_cqi_info is set */
    bool scc_4_cqi_info_available;
    /** SCC 4 CA Information */
    gms_nas_cqi_info_t scc_4_cqi_info;
} gms_nas_lte_cqi_info_t;

/**
 * @brief Pack QMI_GMS_NAS_GET_LTECQI_INFO (771 - 0x0303) request
 *
 * This command used to get some variables for NAS LTE CQI
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_nas_get_ltecqi_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_NAS_GET_LTECQI_INFO (771 - 0x0303) request
 *
 * This function unpacks the result of a QMI_GMS_NAS_GET_LTECQI_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gms_nas_lte_cqi_info_t
 */
QMI_SDK_error_t telit_gms_nas_get_ltecqi_info_unpack(uint8_t *rsp, uint16_t len, gms_nas_lte_cqi_info_t *out);

/**
 * @brief GMS SCC Information type
 *
 */
typedef struct {
    /** Measured SINR in dB. Values: 0 to 250 (-999 invalid) */
    int16_t sinr;
    /** Number of SCC RX instances */
    uint8_t num_instances;
} gms_nas_scc_info_t;

/**
 * @brief GMS SCC RX information type
 *
 */
typedef struct {
    /** Rx antenna path.
     *
     *  Values:
       -  0 - Primary Rx
       -  1 - Diversity Rx */
    uint8_t rx_chain_index;
    /** Rx path is tuned to a channel or Not.
     *
     *  Values:
       -  0 - Not tuned
       -  1 - Tuned */
    uint8_t is_radio_tuned;
    /** Rx power value in 1/10 dBm resolution.
     *  Values: 0 to -120 (-999 invalid) */
    int32_t rx_power;
    /** Current RSRP in 1/10 dBm as measured by L1.
     *  Values: -44 to -140 (-999 invalid) */
    int32_t rsrp;
    /** Current RSRQ in 1/10 dB as measured by L1.
     *  Values: -3 to -20 (-999 invalid) */
    int32_t rsrq;
} gms_nas_scc_rx_info_inst_t;

/**
 * @brief output type for QMI_GMS_NAS_GET_LTE_SCC_RX_INFO request
 *
 */
typedef struct {
    /** true if optional scc_0_info is set */
    bool scc_0_info_available;
    /** SCC 0 Information */
    gms_nas_scc_info_t scc_0_info;
    /** true if optional scc_0_rx_0_info is set */
    bool scc_0_rx_0_info_available;
    /** SCC 0 RX 0 Information */
    gms_nas_scc_rx_info_inst_t scc_0_rx_0_info;
    /** true if optional scc_0_rx_1_info is set */
    bool scc_0_rx_1_info_available;
    /** SCC 0 RX 1 Information */
    gms_nas_scc_rx_info_inst_t scc_0_rx_1_info;
    /** true if optional scc_0_rx_2_info is set */
    bool scc_0_rx_2_info_available;
    /** SCC 0 RX 2 Information */
    gms_nas_scc_rx_info_inst_t scc_0_rx_2_info;
    /** true if optional scc_0_rx_3_info is set */
    bool scc_0_rx_3_info_available;
    /** SCC 0 RX 3 Information */
    gms_nas_scc_rx_info_inst_t scc_0_rx_3_info;
    /** true if optional scc_1_info is set */
    bool scc_1_info_available;
    /** SCC 1 Information */
    gms_nas_scc_info_t scc_1_info;
    /** true if optional scc_1_rx_0_info is set */
    bool scc_1_rx_0_info_available;
    /** SCC 1 RX 0 Information */
    gms_nas_scc_rx_info_inst_t scc_1_rx_0_info;
    /** true if optional scc_1_rx_1_info is set */
    bool scc_1_rx_1_info_available;
    /** SCC 1 RX 1 Information */
    gms_nas_scc_rx_info_inst_t scc_1_rx_1_info;
    /** true if optional scc_1_rx_2_info is set */
    bool scc_1_rx_2_info_available;
    /** SCC 1 RX 2 Information */
    gms_nas_scc_rx_info_inst_t scc_1_rx_2_info;
    /** true if optional scc_1_rx_3_info is set */
    bool scc_1_rx_3_info_available;
    /** SCC 1 RX 3 Information */
    gms_nas_scc_rx_info_inst_t scc_1_rx_3_info;
    /** true if optional scc_2_info is set */
    bool scc_2_info_available;
    /** SCC 2 Information */
    gms_nas_scc_info_t scc_2_info;
    /** true if optional scc_2_rx_0_info is set */
    bool scc_2_rx_0_info_available;
    /** SCC 2 RX 0 Information */
    gms_nas_scc_rx_info_inst_t scc_2_rx_0_info;
    /** true if optional scc_2_rx_1_info is set */
    bool scc_2_rx_1_info_available;
    /** SCC 2 RX 1 Information */
    gms_nas_scc_rx_info_inst_t scc_2_rx_1_info;
    /** true if optional scc_2_rx_2_info is set */
    bool scc_2_rx_2_info_available;
    /** SCC 2 RX 2 Information */
    gms_nas_scc_rx_info_inst_t scc_2_rx_2_info;
    /** true if optional scc_2_rx_3_info is set */
    bool scc_2_rx_3_info_available;
    /** SCC 2 RX 3 Information */
    gms_nas_scc_rx_info_inst_t scc_2_rx_3_info;
    /** true if optional scc_3_info is set */
    bool scc_3_info_available;
    /** SCC 3 Information */
    gms_nas_scc_info_t scc_3_info;
    /** true if optional scc_3_rx_0_info is set */
    bool scc_3_rx_0_info_available;
    /** SCC 3 RX 0 Information */
    gms_nas_scc_rx_info_inst_t scc_3_rx_0_info;
    /** true if optional scc_3_rx_1_info is set */
    bool scc_3_rx_1_info_available;
    /** SCC 3 RX 1 Information */
    gms_nas_scc_rx_info_inst_t scc_3_rx_1_info;
    /** true if optional scc_3_rx_2_info is set */
    bool scc_3_rx_2_info_available;
    /** SCC 3 RX 2 Information */
    gms_nas_scc_rx_info_inst_t scc_3_rx_2_info;
    /** true if optional scc_3_rx_3_info is set */
    bool scc_3_rx_3_info_available;
    /** SCC 3 RX 3 Information */
    gms_nas_scc_rx_info_inst_t scc_3_rx_3_info;
} gms_nas_lte_scc_info_t;

/**
 * @brief Pack QMI_GMS_NAS_GET_LTE_SCC_RX_INFO (772 - 0x0304) request
 *
 * This command used to get LTE SCC RX information
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_nas_get_lte_scc_rx_info_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_NAS_GET_LTE_SCC_RX_INFO (772 - 0x0304) request
 *
 * This function unpacks the result of a QMI_GMS_NAS_GET_LTE_SCC_RX_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gms_nas_lte_scc_info_t
 */
QMI_SDK_error_t telit_gms_nas_get_lte_scc_rx_info_unpack(uint8_t *rsp, uint16_t len,
        gms_nas_lte_scc_info_t *out);

/**
 * @brief input type for QMI_GMS_NAS_INDICATION_REGISTER request
 *
 */
typedef struct {
    /** Enable or disable indicating LTE ESM uplink messages */
    bool lte_esm_ul;
    /** Enable or disable indicating LTE ESM downlink messages */
    bool lte_esm_dl;
    /** Enable or disable indicating LTE EMM uplink messages */
    bool lte_emm_ul;
    /** Enable or disable indicating LTE EMM downlink messages */
    bool lte_emm_dl;
    /** Enable or disable indicating GSM/UMTS uplink messages */
    bool gsm_umts_ul;
    /** Enable or disable indicating GSM/UMTS downlink messages */
    bool gsm_umts_dl;
    /** Enable or disable indicating Rank Indicator messages */
    bool rank_indicator;
    /** Enable or disable indicating Timer Indicator messages */
    bool timer;
} gms_nas_indication_register_t;

/**
 * @brief Pack QMI_GMS_NAS_IND_REGISTER (773 - 0x0305) request
 *
 * This function packs a qmi request for setting the GMS
 * indications.
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #gms_nas_indication_register_t
 */
QMI_SDK_error_t telit_gms_nas_ind_register_pack(pack_qmi_t *req_ctx,
        uint8_t *req, uint16_t *len,
        gms_nas_indication_register_t *input);

/**
 * @brief Unpack QMI_GMS_NAS_IND_REGISTER (773 - 0x0305) request
 *
 * This function unpacks the result of a QMI_GMS_NAS_IND_REGISTER request
 *
 * @param[in] rsp response
 * @param[in] len response length
 */
QMI_SDK_error_t telit_gms_nas_ind_register_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief GMS band variants
 *
 */
typedef enum {
    /** Generic */
    QMI_GMS_VARIANT_GENERIC = 0x00,
    /** AT&T*/
    QMI_GMS_VARIANT_ATT     = 0x01,
    /** Verizon */
    QMI_GMS_VARIANT_VZW     = 0x02,
    /** T-Mobile */
    QMI_GMS_VARIANT_TMUS    = 0x03,
    /** Invalid variant */
    QMI_GMS_VARIANT_PTCRB   = 0x04,
} QMI_GMS_band_variant_t;

/**
 * @brief output type for QMI_GMS_NAS_GET_BAND_CAP request
 *
 */
typedef struct {
    /** Variant name, see #QMI_GMS_band_variant_t */
    enum32_t variant;
    /** Bitmask representing the band preference, see QMI_BAND_* constants */
    uint64_t band_pref;
    /** Bits 1 to 64 of the 256-bit LTE E-UTRA Operating Band bitmask */
    uint64_t bits_1_64;
    /** Bits 65 to 128 of the 256-bit LTE E-UTRA Operating Band bitmask
     *
     * Eg:
     * value 0x2: band 66
     * value 0x42: band 66 + band 71 */
    uint64_t bits_65_128;
    /** Bits 129 to 192 of the 256-bit LTE E-UTRA Operating Band bitmask */
    uint64_t bits_129_192;
    /** Bits 193 to 256 of the 256-bit LTE E-UTRA OperatingBand bitmask */
    uint64_t bits_193_256;
} gms_nas_band_capability_t;

/**
 * @brief Pack QMI_GMS_NAS_GET_BAND_CAP (777 - 0x0309) request
 *
 * This command used to get WCDMA and LTE band capability
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_nas_get_band_cap_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_NAS_GET_BAND_CAP (777 - 0x0309) request
 *
 * This function unpacks the result of a QMI_GMS_NAS_GET_BAND_CAP request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gms_nas_band_capability_t
 */
QMI_SDK_error_t telit_gms_nas_get_band_cap_unpack(uint8_t *rsp, uint16_t len, gms_nas_band_capability_t *out);

/**
 * @brief Enum to describe LTE band calsses
 *
 */
typedef enum {
    QMI_GMS_CLASS_OF_BAND_1   = 120,
    QMI_GMS_CLASS_OF_BAND_2   = 121,
    QMI_GMS_CLASS_OF_BAND_3   = 122,
    QMI_GMS_CLASS_OF_BAND_4   = 123,
    QMI_GMS_CLASS_OF_BAND_5   = 124,
    QMI_GMS_CLASS_OF_BAND_6   = 125,
    QMI_GMS_CLASS_OF_BAND_7   = 126,
    QMI_GMS_CLASS_OF_BAND_8   = 127,
    QMI_GMS_CLASS_OF_BAND_9   = 128,
    QMI_GMS_CLASS_OF_BAND_10  = 129,
    QMI_GMS_CLASS_OF_BAND_11  = 130,
    QMI_GMS_CLASS_OF_BAND_12  = 131,
    QMI_GMS_CLASS_OF_BAND_13  = 132,
    QMI_GMS_CLASS_OF_BAND_14  = 133,
    QMI_GMS_CLASS_OF_BAND_17  = 134,
    QMI_GMS_CLASS_OF_BAND_33  = 135,
    QMI_GMS_CLASS_OF_BAND_34  = 136,
    QMI_GMS_CLASS_OF_BAND_35  = 137,
    QMI_GMS_CLASS_OF_BAND_36  = 138,
    QMI_GMS_CLASS_OF_BAND_37  = 139,
    QMI_GMS_CLASS_OF_BAND_38  = 140,
    QMI_GMS_CLASS_OF_BAND_39  = 141,
    QMI_GMS_CLASS_OF_BAND_40  = 142,
    QMI_GMS_CLASS_OF_BAND_18  = 143,
    QMI_GMS_CLASS_OF_BAND_19  = 144,
    QMI_GMS_CLASS_OF_BAND_20  = 145,
    QMI_GMS_CLASS_OF_BAND_21  = 146,
    QMI_GMS_CLASS_OF_BAND_24  = 147,
    QMI_GMS_CLASS_OF_BAND_25  = 148,
    QMI_GMS_CLASS_OF_BAND_41  = 149,
    QMI_GMS_CLASS_OF_BAND_42  = 150,
    QMI_GMS_CLASS_OF_BAND_43  = 151,
    QMI_GMS_CLASS_OF_BAND_23  = 152,
    QMI_GMS_CLASS_OF_BAND_26  = 153,
    QMI_GMS_CLASS_OF_BAND_32  = 154,
    QMI_GMS_CLASS_OF_BAND_125 = 155,
    QMI_GMS_CLASS_OF_BAND_126 = 156,
    QMI_GMS_CLASS_OF_BAND_127 = 157,
    QMI_GMS_CLASS_OF_BAND_28  = 158,
    QMI_GMS_CLASS_OF_BAND_29  = 159,
    QMI_GMS_CLASS_OF_BAND_30  = 160,
    QMI_GMS_CLASS_OF_BAND_66  = 161,
    QMI_GMS_CLASS_OF_BAND_250 = 162,
    QMI_GMS_CLASS_OF_BAND_46  = 163,
    QMI_GMS_CLASS_OF_BAND_27  = 164,
    QMI_GMS_CLASS_OF_BAND_31  = 165,
    QMI_GMS_CLASS_OF_BAND_47  = 166,
    QMI_GMS_CLASS_OF_BAND_48  = 167,
    QMI_GMS_CLASS_OF_BAND_71  = 168,
} QMI_GMS_nas_lte_band_class_t;

/**
 * @brief Enum to describe NR5G band calsses
 *
 */
typedef enum
{
    QMI_GMS_NR5G_BAND_1   = 250,
    QMI_GMS_NR5G_BAND_2   = 251,
    QMI_GMS_NR5G_BAND_3   = 252,
    QMI_GMS_NR5G_BAND_5   = 253,
    QMI_GMS_NR5G_BAND_7   = 254,
    QMI_GMS_NR5G_BAND_8   = 255,
    QMI_GMS_NR5G_BAND_20  = 256,
    QMI_GMS_NR5G_BAND_28  = 257,
    QMI_GMS_NR5G_BAND_38  = 258,
    QMI_GMS_NR5G_BAND_41  = 259,
    QMI_GMS_NR5G_BAND_50  = 260,
    QMI_GMS_NR5G_BAND_51  = 261,
    QMI_GMS_NR5G_BAND_66  = 262,
    QMI_GMS_NR5G_BAND_70  = 263,
    QMI_GMS_NR5G_BAND_71  = 264,
    QMI_GMS_NR5G_BAND_74  = 265,
    QMI_GMS_NR5G_BAND_75  = 266,
    QMI_GMS_NR5G_BAND_76  = 267,
    QMI_GMS_NR5G_BAND_77  = 268,
    QMI_GMS_NR5G_BAND_78  = 269,
    QMI_GMS_NR5G_BAND_79  = 270,
    QMI_GMS_NR5G_BAND_80  = 271,
    QMI_GMS_NR5G_BAND_81  = 272,
    QMI_GMS_NR5G_BAND_82  = 273,
    QMI_GMS_NR5G_BAND_83  = 274,
    QMI_GMS_NR5G_BAND_84  = 275,
    QMI_GMS_NR5G_BAND_85  = 276,
    QMI_GMS_NR5G_BAND_257 = 277,
    QMI_GMS_NR5G_BAND_258 = 278,
    QMI_GMS_NR5G_BAND_259 = 279,
    QMI_GMS_NR5G_BAND_260 = 280,
    QMI_GMS_NR5G_BAND_261 = 281,
    QMI_GMS_NR5G_BAND_12  = 282,
    QMI_GMS_NR5G_BAND_25  = 283,
    QMI_GMS_NR5G_BAND_34  = 284,
    QMI_GMS_NR5G_BAND_39  = 285,
    QMI_GMS_NR5G_BAND_40  = 286,
    QMI_GMS_NR5G_BAND_65  = 287,
    QMI_GMS_NR5G_BAND_86  = 288,
    QMI_GMS_NR5G_BAND_48  = 289,
    QMI_GMS_NR5G_BAND_14  = 290,
    QMI_GMS_NR5G_BAND_13  = 291,
    QMI_GMS_NR5G_BAND_18  = 292,
    QMI_GMS_NR5G_BAND_26  = 293,
    QMI_GMS_NR5G_BAND_30  = 294,
    QMI_GMS_NR5G_BAND_29  = 295,
    QMI_GMS_NR5G_BAND_53  = 296,
} QMI_GMS_nr5g_band_class_t;

/**
 * @brief PCC ENDC CA information struct
 *
 */
typedef struct {
    /**< LTE band class, see #QMI_GMS_nas_lte_band_class_t */
    enum8_t band_class;
    /**< E-UTRA absolute radio frequency channel number of the serving cell. Range: 0 to 68935. */
    uint32_t channel;
    /**< Bandwidth, see #QMI_GMS_LTE_bandwidth_t */
    enum8_t dl_bw;
    /**< Physical Cell Id. Range: 0 to 503 */
    uint16_t pci;
    /**< Current RSRP in 1/10 dBm as measured by L1. Range: -44 to -140 dbm */
    int32_t rsrp;
    /**< Current RSSI in 1/10 dBm as measured by L1. Range: 0 to -120 dbm */
    int32_t rssi;
    /**< Current RSRQ in 1/10 dB as measured by L1. Range: -3 to -20 db */
    int32_t rsrq;
    /**< Measured SINR in dB. Range: 0 to 250 */
    int32_t sinr;
    /**< Tracking area code information for LTE */
    uint16_t tac;
} gms_nas_pcc_endc_ca_info_t;

/**
 * @brief SCC ENDC CA information struct
 *
 */
typedef struct {
    /**< LTE band class, see #QMI_GMS_nas_lte_band_class_t */
    enum8_t band_class;
    /**< E-UTRA absolute radio frequency channel number of the serving cell. Range: 0 to 68935. */
    uint32_t channel;
    /**< Bandwidth, see #QMI_GMS_LTE_bandwidth_t */
    enum8_t dl_bw;
    /**< Physical Cell Id. Range: 0 to 503 */
    uint16_t pci;
    /**< Current RSRP in 1/10 dBm as measured by L1. Range: -44 to -140 dbm */
    int32_t rsrp;
    /**< Current RSSI in 1/10 dBm as measured by L1. Range: 0 to -120 dbm */
    int32_t rssi;
    /**< Current RSRQ in 1/10 dB as measured by L1. Range: -3 to -20 db */
    int32_t rsrq;
    /**< Measured SINR in dB. Range: 0 to 250 */
    int32_t sinr;
    /**< Current SCC 0 state.
     *
     * 0 - Init,
     * 1 - Configured,
     * 2 - Active */
    uint32_t state;
} gms_nas_scc_endc_ca_info_t;

/**
 * @brief 5G CC information struct
 *
 */
typedef struct {
    /**< ENDC active band, see #QMI_GMS_nr5g_band_class_t */
    int32_t endc_band;
    /**< ENDC Bandwidth (MHz). Range: 5 to 400 */
    uint16_t endc_bandwidth;
    /**< ENDC active downlink channel */
    int32_t endc_channel;
    /**< ENDC active uplink channel */
    int32_t endc_ul_channel;
    /**< ENDC Physical Cell Id */
    uint16_t endc_pci;
    /**< Current ENDC RSRP in 1/10 dBm as measured by L1 */
    int32_t endc_rsrp;
    /**< Current ENDC RSSI in 1/10 dBm as measured by L1 */
    int32_t endc_rssi;
    /**< Current ENDC RSRQ in 1/10 dB as measured by L1 */
    int32_t endc_rsrq;
    /**< Measured ENDC SINR in dB */
    int32_t endc_sinr;
} gms_nas_cc_5g_info_t;

/**
 * @brief output type for QMI_GMS_NAS_GET_ENDC_CA_INFO response
 *
 */
typedef struct {
    /**< set if pcc_ca_info is available */
    bool pcc_ca_info_available;
    /**< PCC CA information, see gms_#gms_nas_pcc_endc_ca_info_t */
    gms_nas_pcc_endc_ca_info_t pcc_ca_info;
    /**< set if scc_0_ca_info is available */
    bool scc_0_ca_info_available;
    /**< SCC 0 CA Information, see #gms_nas_scc_endc_ca_info_t */
    gms_nas_scc_endc_ca_info_t scc_0_ca_info;
    /**< set if scc_1_ca_info is available */
    bool scc_1_ca_info_available;
    /**< SCC 1 CA Information, see #gms_nas_scc_endc_ca_info_t */
    gms_nas_scc_endc_ca_info_t scc_1_ca_info;
    /**< set if scc_2_ca_info is available */
    bool scc_2_ca_info_available;
    /**< SCC 2 CA Information, see #gms_nas_scc_endc_ca_info_t */
    gms_nas_scc_endc_ca_info_t scc_2_ca_info;
    /**< set if scc_3_ca_info is available */
    bool scc_3_ca_info_available;
    /**< SCC 3 CA Information, see #gms_nas_scc_endc_ca_info_t */
    gms_nas_scc_endc_ca_info_t scc_3_ca_info;
    /**< set if scc_4_ca_info is available */
    bool scc_4_ca_info_available;
    /**< SCC 4 CA Information, see #gms_nas_scc_endc_ca_info_t */
    gms_nas_scc_endc_ca_info_t scc_4_ca_info;
    /**< set if cc_5g_info is available */
    bool cc_5g_info_available;
    /**< PCC CA information, see #gms_nas_cc_5g_info_t */
    gms_nas_cc_5g_info_t cc_5g_info;
} gms_nas_endc_ca_info_t;

/**
 * @brief Pack QMI_GMS_NAS_GET_ENDC_CA_INFO (783 - 0x030f) request
 *
 * This function packs a qmi request for getting carrier aggregation information
 * of LTE and 5G CC when EN-DC is activated
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_nas_get_endc_ca_info_pack(pack_qmi_t *req_ctx, void *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_NAS_GET_ENDC_CA_INFO (783 - 0x030f) request
 *
 * This function unpacks the result of a QMI_GMS_NAS_GET_ENDC_CA_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out buffer to hold gms_nas_endc_ca_info_t struct
 */
QMI_SDK_error_t telit_gms_nas_get_endc_ca_info_unpack(uint8_t *rsp, uint16_t len,
        gms_nas_endc_ca_info_t *out);

/**
 * @brief PCC NR5G CPHY CA information struct
 *
 */
typedef struct {
    /**< Active NR5G band class */
    uint16_t band_class;
    /**< NR5G absolute radio frequency channel number of the serving cell. Range: 0 to 65535 */
    uint32_t channel;
    /**< Bandwidth. Range: 0-400 */
    uint16_t dl_bw;
    /**< Physical Cell Id. Range */
    uint16_t pci;
    /**< Current RSRP in 1/10 dBm as measured by L1 */
    int32_t rsrp;
    /**< Current RSSI in 1/10 dBm as measured by L1 */
    int32_t rssi;
    /**< Current RSRQ in 1/10 dB as measured by L1 */
    int32_t rsrq;
    /**< Measured SINR in dB */
    int32_t sinr;
    /**< Tracking area code information for NR5G */
    uint32_t tac;
} gms_nas_pcc_nr5g_ca_info_t;

/**
 * @brief SCC NR5G CPHY CA information struct
 *
 */
typedef struct {
    /**< Active NR5G band class */
    uint16_t band_class;
    /**< NR5G absolute radio frequency channel number of the serving cell. Range: 0 to 65535 */
    uint32_t channel;
    /**< Bandwidth. Range: 0-400 */
    uint16_t dl_bw;
    /**< Physical Cell Id. Range */
    uint16_t pci;
    /**< Current RSRP in 1/10 dBm as measured by L1 */
    int32_t rsrp;
    /**< Current RSSI in 1/10 dBm as measured by L1 */
    int32_t rssi;
    /**< Current RSRQ in 1/10 dB as measured by L1 */
    int32_t rsrq;
    /**< Measured SINR in dB */
    int32_t sinr;
    /**< State of the secondary cell.
     *
     * 0 - Init,
     * 1 - Configured,
     * 2 - Active */
    uint32_t state;
} gms_nas_scc_nr5g_ca_info_t;

/**
 * @brief output type for QMI_GMS_NAS_GET_NR5G_CA_INFO response
 *
 */
typedef struct {
    /**< set if nr5g_ca_pcc_info is available */
    bool nr5g_ca_pcc_info_available;
    /**< NR5G PCC CA information, see #gms_nas_pcc_nr5g_ca_info_t */
    gms_nas_pcc_nr5g_ca_info_t nr5g_ca_pcc_info;
    /**< set if nr5g_ca_scc0_info is available */
    bool nr5g_ca_scc0_info_available;
    /**< NR5G SCC 0 CA Information, see #gms_nas_scc_nr5g_ca_info_t */
    gms_nas_scc_nr5g_ca_info_t nr5g_ca_scc0_info;
    /**< set if nr5g_ca_scc1_info is available */
    bool nr5g_ca_scc1_info_available;
    /**< NR5G SCC 1 CA Information, see #gms_nas_scc_nr5g_ca_info_t */
    gms_nas_scc_nr5g_ca_info_t nr5g_ca_scc1_info;
    /**< set if nr5g_ca_scc2_info is available */
    bool nr5g_ca_scc2_info_available;
    /**< NR5G SCC 2 CA Information, see #gms_nas_scc_nr5g_ca_info_t */
    gms_nas_scc_nr5g_ca_info_t nr5g_ca_scc2_info;
    /**< set if nr5g_ca_scc3_info is available */
    bool nr5g_ca_scc3_info_available;
    /**< NR5G SCC 3 CA Information, see #gms_nas_scc_nr5g_ca_info_t */
    gms_nas_scc_nr5g_ca_info_t nr5g_ca_scc3_info;
    /**< set if nr5g_ca_scc4_info is available */
    bool nr5g_ca_scc4_info_available;
    /**< NR5G SCC 4 CA Information, see #scc_nr5g_ca_info_t */
    gms_nas_scc_nr5g_ca_info_t nr5g_ca_scc4_info;
} gms_nas_nr5g_ca_info_t;

/**
 * @brief Pack QMI_GMS_NAS_GET_NR5G_CA_INFO (790 - 0x0316) request
 *
 * This function packs a qmi request for getting some variables for NAS NR5G CA in SA mode
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_nas_get_nr5g_ca_info_pack(pack_qmi_t *req_ctx, void *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_NAS_GET_NR5G_CA_INFO (790 - 0x0316) request
 *
 * This function unpacks the result of a QMI_GMS_NAS_GET_NR5G_CA_INFO request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out buffer to hold gms_nas_nr5g_ca_info_t struct
 */
QMI_SDK_error_t telit_gms_nas_get_nr5g_ca_info_unpack(uint8_t *rsp, uint16_t len,
        gms_nas_nr5g_ca_info_t *out);

/**
 * @brief input type for QMI_GMS_NAS_SET_MCCBLOCK request
 */
typedef struct {
    /** Mobile Country Code 1. Values: 0 to 999 ( 0 : No block the PLMN) */
    uint16_t mcc1;
    /**< set if mcc2 is available */
    bool mcc2_available;
    /** Mobile Country Code 2. Values: 0 to 999 ( 0 : No block the PLMN) */
    uint16_t mcc2;
    /**< set if mcc3 is available */
    bool mcc3_available;
    /** Mobile Country Code 3. Values: 0 to 999 ( 0 : No block the PLMN) */
    uint16_t mcc3;
} gms_nas_mccblock_t;

/**
 * @brief Pack QMI_GMS_NAS_SET_MCCBLOCK (794 - 0x031A) request
 *
 * This function packs a qmi request to set Mobile Country Code to block the PLMN
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[in] input #gms_nas_mccblock_t
 */
QMI_SDK_error_t telit_gms_nas_set_mccblock_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        gms_nas_mccblock_t *input);

/**
 * @brief Unpack QMI_GMS_NAS_SET_MCCBLOCK (794 - 0x031A) response
 *
 * This function unpacks the result of a QMI_GMS_NAS_SET_MCCBLOCK request
 *
 * @param[out] rsp response
 * @param[out] len response length
 */
QMI_SDK_error_t telit_gms_nas_set_mccblock_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_GMS_NAS_GET_MCCBLOCK (795 - 0x031B) request
 *
 * This function packs a qmi request to get Mobile Country Code to block the PLMN
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_nas_get_mccblock_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_NAS_GET_MCCBLOCK (795 - 0x031B) response
 *
 * This function unpacks the result of a QMI_GMS_NAS_GET_MCCBLOCK request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gms_nas_mccblock_t
 *
 * @return
 */
QMI_SDK_error_t telit_gms_nas_get_mccblock_unpack(uint8_t *rsp,
        uint16_t len,
        gms_nas_mccblock_t *out);

/**
 * @brief GMS LOC fix types
 */
typedef enum {
    /** Default engine mode */
    QMI_GMS_GNSS_FIX_TYPE_DEFAULT_ENGINE_MODE = 0x01,
    /** MS-based */
    QMI_GMS_GNSS_FIX_TYPE_MS_BASED            = 0x02,
    /** MS-assisted */
    QMI_GMS_GNSS_FIX_TYPE_MS_ASSISTED         = 0x03,
    /** Stand-alone */
    QMI_GMS_GNSS_FIX_TYPE_STANDALONE          = 0x04,
} QMI_GMS_GNSS_fix_type_t;

/**
 * @brief GMS LOC autostart configuration
 */
typedef enum {
    /** Autostart disabled */
    QMI_GMS_GNSS_AUTOSTART_DISABLED          = 0x00,
    /** Autostart at bootup */
    QMI_GMS_GNSS_AUTOSTART_BOOTUP            = 0x01,
    /** Autostart at NMEA port opening */
    QMI_GMS_GNSS_AUTOSTART_NMEA_PORT_OPENING = 0x02,
    /** Autostart not reported */
    QMI_GMS_GNSS_AUTOSTART_NOT_REPORTED      = 0xFF,
} QMI_GMS_GNSS_autostart_t;

/**
 * @brief GMS LOC NMEA type values
 */
typedef enum {
    /** Disable all NMEA sentences */
    QMI_GMS_NMEA_TYPE_DISABLE_NMEA = 0x00000000,
    /** Enable GPGGA */
    QMI_GMS_NMEA_TYPE_ENABLE_GPGGA = 0x00000001,
    /** Enable GPRMC */
    QMI_GMS_NMEA_TYPE_ENABLE_GPRMC = 0x00000002,
    /** Enable GPGSV */
    QMI_GMS_NMEA_TYPE_ENABLE_GPGSV = 0x00000004,
    /** Enable GPGSA */
    QMI_GMS_NMEA_TYPE_ENABLE_GPGSA = 0x00000008,
    /** Enable GPVTG */
    QMI_GMS_NMEA_TYPE_ENABLE_GPVTG = 0x00000010,
    /** Enable GPGLL */
    QMI_GMS_NMEA_TYPE_ENABLE_GPGLL = 0x00000020,
    /** Enable GLGSV */
    QMI_GMS_NMEA_TYPE_ENABLE_GLGSV = 0x00000040,
    /** Enable GNGSA */
    QMI_GMS_NMEA_TYPE_ENABLE_GNGSA = 0x00000080,
    /** Enable GNGNS */
    QMI_GMS_NMEA_TYPE_ENABLE_GNGNS = 0x00000100,
    /** Enable GARMC */
    QMI_GMS_NMEA_TYPE_ENABLE_GARMC = 0x00000400,
    /** Enable GAGSV */
    QMI_GMS_NMEA_TYPE_ENABLE_GAGSV = 0x00000800,
    /** Enable GAGSA */
    QMI_GMS_NMEA_TYPE_ENABLE_GAGSA = 0x00001000,
    /** Enable GAVTG */
    QMI_GMS_NMEA_TYPE_ENABLE_GAVTG = 0x00002000,
    /** Enable GAGGA */
    QMI_GMS_NMEA_TYPE_ENABLE_GAGGA = 0x00004000,
    /** Enable BDGSV */
    QMI_GMS_NMEA_TYPE_ENABLE_BDGSV = 0x00008000,
    /** Enable GPGNS */
    QMI_GMS_NMEA_TYPE_ENABLE_GPGNS = 0x00040000,
    /** Enable GLGNS */
    QMI_GMS_NMEA_TYPE_ENABLE_GLGNS = 0x00080000,
    /** Enable GNGSV */
    QMI_GMS_NMEA_TYPE_ENABLE_GNGSV = 0x00100000,
    /** Enable GAGNS */
    QMI_GMS_NMEA_TYPE_ENABLE_GAGNS = 0x00200000,
    /** Enable BDGSA */
    QMI_GMS_NMEA_TYPE_ENABLE_BDGSA = 0x00400000,
} QMI_GMS_NMEA_type_t;

/**
 * @brief output tpe for QMI_GMS_LOC_GET_AUTO_START request
 */
typedef struct {
    /** see #QMI_GMS_GNSS_autostart_t */
    enum8_t function;
    /** true if function is reported by the modem */
    uint8_t function_reported;
    /** see #QMI_GMS_GNSS_fix_type_t */
    enum8_t fix_type;
    /** true if fix_type is reported by the modem */
    uint8_t fix_type_reported;
    /** Maximum time in seconds allowed for the receiver to get a fix.
       valid range: 1 - 255 */
    uint8_t max_time;
    /** true if max_time is reported by the modem */
    uint8_t max_time_reported;
    /** Maximum uncertainty of a fix measured by distance in meters.
       valid range: 1 - 4294967280 (0xfffffff0) */
    uint32_t max_dist;
    /** true if max_dist is reported by the modem */
    uint8_t max_dist_reported;
    /** Time in seconds between fixes.
       valid range: 1 - 65535 */
    uint32_t fix_rate;
    /** true if fix_rate is reported by the modem */
    uint8_t fix_rate_reported;
} gms_loc_get_auto_start_resp_t;

/**
 * @brief Pack QMI_GMS_LOC_GET_AUTO_START (4097 - 0x1001) request
 *
 * This function packs a qmi request to get GNSS auto start settings
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_loc_get_auto_start_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_LOC_GET_AUTO_START (4097 - 0x1001) response
 *
 * This function unpacks the result of a QMI_GMS_LOC_GET_AUTO_START request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gms_loc_get_auto_start_resp_t
 *
 * @return
 */
QMI_SDK_error_t telit_gms_loc_get_auto_start_unpack(uint8_t *rsp,
        uint16_t len,
        gms_loc_get_auto_start_resp_t *out);

/**
 * @brief input type for QMI_GMS_LOC_SET_AUTO_START request
 */
typedef struct {
    /** see #QMI_GMS_GNSS_autostart_t */
    enum8_t function;
    /** true if function is set by the modem */
    uint8_t set_function;
    /** see #QMI_GMS_GNSS_fix_type_t */
    enum8_t fix_type;
    /** true if fix_type is set by the modem */
    uint8_t set_fix_type;
    /** Maximum time in seconds allowed for the receiver to get a fix.
       valid range: 1 - 255 */
    uint8_t max_time;
    /** true if max_time is set by the modem */
    uint8_t set_max_time;
    /** Maximum uncertainty of a fix measured by distance in meters.
       valid range: 1 - 4294967280 (0xfffffff0) */
    uint32_t max_dist;
    /** true if max_dist is set by the modem */
    uint8_t set_max_dist;
    /** Time in seconds between fixes.
       valid range: 1 - 65535 */
    uint32_t fix_rate;
    /** true if fix_rate is set by the modem */
    uint8_t set_fix_rate;
} gms_loc_set_auto_start_t;

/**
 * @brief Pack QMI_GMS_LOC_SET_AUTO_START (4098 - 0x1002) request
 *
 * This function packs a qmi request to set GNSS auto start settings
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[out] out #gms_loc_set_auto_start_t
 */
QMI_SDK_error_t telit_gms_loc_set_auto_start_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        gms_loc_set_auto_start_t *input);

/**
 * @brief Unpack QMI_GMS_LOC_SET_AUTO_START (4098 - 0x1002) response
 *
 * This function unpacks the result of a QMI_GMS_LOC_SET_AUTO_START request
 *
 * @param[out] rsp response
 * @param[out] len response length
 */
QMI_SDK_error_t telit_gms_loc_set_auto_start_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_GMS_LOC_SET_NMEA_TYPE request
 */
typedef struct {
    /** whether NMEA port is open or not.
     * values:
     * - 0 - NMEA streaming stop
     * - 1 - NMEA streaming start */
    uint8_t nmea_mode_ind;
    /** true if nmea_type is available */
    bool nmea_type_available;
    /** see #QMI_GMS_NMEA_type_t */
    enum32_t nmea_type;
} gms_loc_set_nmea_type_t;

/**
 * @brief Pack QMI_GMS_LOC_SET_NMEA_TYPE (4099 - 0x1003) request
 *
 * This function packs a qmi request to set NMEA type
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[out] out #gms_loc_set_nmea_type_t
 */
QMI_SDK_error_t telit_gms_loc_set_nmea_type_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        gms_loc_set_nmea_type_t *input);

/**
 * @brief Unpack QMI_GMS_LOC_SET_NMEA_TYPE (4099 - 0x1003) response
 *
 * This function unpacks the result of a QMI_GMS_LOC_SET_NMEA_TYPE request
 *
 * @param[out] rsp response
 * @param[out] len response length
 */
QMI_SDK_error_t telit_gms_loc_set_nmea_type_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief input type for QMI_GMS_LOC_SET_GNSS_SESSION request,
 * output type for QMI_GMS_LOC_GET_GNSS_SESSION
 */
typedef struct {
    /** GNSS session mode.
     * values:
     * - 0 - disabled
     * - 1 - enabled */
    uint8_t gnss_session_mode;
} gms_loc_gnss_session_mode_t;

/**
 * @brief Pack QMI_GMS_LOC_SET_GNSS_SESSION (4103 - 0x1007) request
 *
 * This function packs a qmi request to set GNSS session mode
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 * @param[out] out #gms_loc_gnss_session_mode_t
 */
QMI_SDK_error_t telit_gms_loc_set_gnss_session_pack(pack_qmi_t *req_ctx,
        uint8_t *req,
        uint16_t *len,
        gms_loc_gnss_session_mode_t *input);

/**
 * @brief Unpack QMI_GMS_LOC_SET_GNSS_SESSION (4103 - 0x1007) response
 *
 * This function unpacks the result of a QMI_GMS_LOC_SET_GNSS_SESSION request
 *
 * @param[out] rsp response
 * @param[out] len response length
 */
QMI_SDK_error_t telit_gms_loc_set_gnss_session_unpack(uint8_t *rsp, uint16_t len);

/**
 * @brief Pack QMI_GMS_LOC_GET_GNSS_SESSION (4104 - 0x1008) request
 *
 * This function packs a qmi request to get GNSS session status
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_loc_get_gnss_session_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_LOC_GET_GNSS_SESSION (4104 - 0x1008) response
 *
 * This function unpacks the result of a QMI_GMS_LOC_GET_GNSS_SESSION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gms_loc_gnss_session_mode_t
 *
 * @return
 */
QMI_SDK_error_t telit_gms_loc_get_gnss_session_unpack(uint8_t *rsp,
        uint16_t len,
        gms_loc_gnss_session_mode_t *out);

/**
 * @brief output type for QMI_GMS_LOC_GET_ACQ_POSITION
 */
typedef struct {
    /** UTC timestamp for location fix, milliseconds since January 1, 1970 */
    uint64_t utc_time;
    /** latitude string length */
    uint8_t latitude_len;
    /** string representing latitude in the form:
     * ddmm.mmmm(mm) N/S
     * dd - degrees 00..90
     * mm.mmmm(mm) - minutes 00.0000(00)..59.9999(99)
     * N/S - North / South */
    char latitude[MAX_STRING_SIZE];
    /** latitude string length */
    uint8_t longitude_len;
    /** string representing longitude in the form:
     * dddmm.mmmm(mm) E/W
     * ddd - degrees 00..180
     * mm.mmmm(mm) - minutes 00.0000(00)..59.9999(99)
     * E/W - East / West */
    char longitude[MAX_STRING_SIZE];
    /** x.x - Horizontal Diluition of Precision */
    float_t horizontal_dop;
    /** x.x - Altitude-mean-sea-level (geoid) in meters */
    float_t altitude_wrt_sea_level;
    /** Fix type according to the following values:
     * 0 - invalid fix
     * 1 - invalid fix
     * 2 - 2D fix
     * 3 - 3D fix */
    uint8_t fix_type;
    /** Course over ground in the form ddd.mm where
     * ddd - degrees 00..359
     * mm - minutes 00..59 */
    float_t heading;
    /** Speed over ground in km/hr */
    float_t horizontal_speed_km;
    /** Speed over ground in knots */
    float_t horizontal_speed_kn;
    /** Date string length */
    uint8_t utc_time_date_len;
    /** string representing latitude in the form ddmmyy where:
     * dd - day 01..31
     * mm - month 01..12
     * yy - year 00..99 - 2000 to 2099 */
    char utc_time_date[MAX_STRING_SIZE];
    /** Total number of satellites in use in range 00..99 */
    uint8_t num_gps_sv_in_view;
} gms_loc_acquired_position_t;

/**
 * @brief Pack QMI_GMS_LOC_GET_ACQ_POSITION (4105 - 0x1009) request
 *
 * This function packs a qmi request to get the acquired position
 *
 * @param[in] req_ctx request context
 * @param[out] req request
 * @param[in,out] len in request buffer size, out request len
 */
QMI_SDK_error_t telit_gms_loc_get_acq_position_pack(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

/**
 * @brief Unpack QMI_GMS_LOC_GET_ACQ_POSITION (4105 - 0x1009) response
 *
 * This function unpacks the result of a QMI_GMS_LOC_GET_ACQ_POSITION request
 *
 * @param[in] rsp response
 * @param[in] len response length
 * @param[out] out #gms_loc_acquired_position_t
 *
 * @return
 */
QMI_SDK_error_t telit_gms_loc_get_acq_position_unpack(uint8_t *rsp,
        uint16_t len,
        gms_loc_acquired_position_t *out);

/**
 * @brief GMS OTA message types
 *
 */
typedef enum {
    /** LTE ESM uplink */
    QMI_GMS_OTA_LTE_ESM_UPLINK        = 0,
    /** LTE ESM downlink*/
    QMI_GMS_OTA_LTE_ESM_DOWNLINK      = 1,
    /** LTE EMM uplink */
    QMI_GMS_OTA_LTE_EMM_UPLINK        = 2,
    /** LTE EMM downlink */
    QMI_GMS_OTA_LTE_EMM_DOWNLINK      = 3,
    /** GSM/UMTS uplink */
    QMI_GMS_OTA_LTE_GSM_UMTS_UPLINK   = 4,
    /** GSM/UMTS downlink */
    QMI_GMS_OTA_LTE_GSM_UMTS_DOWNLINK = 5,
} QMI_GMS_OTA_message_type_t;

#define QMI_GMS_OTA_MSG_MAX_CONTENT_SIZE 2048
/**
 * @brief output type for tQMI_GMS_NAS_OTA_MSG_REPORT_IND request
 *
 */
typedef struct {
    /** OTA message type, see #QMI_GMS_OTA_message_type_t */
    uint32_t msg_type;
    /** OTA message content length */
    uint16_t data_size;
    /** OTA message content */
    uint8_t data_buf[QMI_GMS_OTA_MSG_MAX_CONTENT_SIZE];
    /** NAS release */
    uint8_t release;
    /** NAS major version */
    uint8_t major;
    /** NAS minor version */
    uint8_t minor;
    /** Seconds in local time since Jan.6th 1980 00:00:00 UTC */
    uint64_t time;
} nas_gms_ota_message_t;

/**
 * @brief Unpack gms indications
 *
 * This function unpacks the result of the following gms indications
 *
 * QMI_GMS_NAS_OTA_MSG_REPORT_IND (774)
 *
 * @param[in] msg_id indication id
 * @param[in] rsp indication
 * @param[in] len indication length
 * @param[out] out type according to the following
 *      QMI_GMS_NAS_OTA_MSG_REPORT_IND (774)   -> #nas_gms_ota_message_t
 * @param[in] out_len buffer size
 */
QMI_SDK_error_t telit_gms_indication_handler(uint16_t msg_id,
        uint8_t *rsp, uint16_t len,
        uint8_t *out, uint16_t out_len);

#ifdef __cplusplus
}
#endif

#endif  /* TELIT_SDK_GMS_H_ */
