#ifndef _TELIT_SDK_PRIVATE_WMS_H_
#define _TELIT_SDK_PRIVATE_WMS_H_

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_WMS_SET_EVENT_REPORT (1 - 0x0001)
 */

/* Structure to describe SET EVENT request TLV 0x10 'Report MT Message' */
typedef struct {
    uint8_t report_mt_message;
} QMI_WMS_SET_EVENT_request_0x10_t;

/* Structure to describe SET EVENT request TLV 0x11 'Report call control info' */
typedef struct {
    uint8_t report_call_control_info;
} QMI_WMS_SET_EVENT_request_0x11_t;

/* Structure to describe SET EVENT request TLV 0x12 'Report MWI Message' */
typedef struct {
    uint8_t report_mwi_message;
} QMI_WMS_SET_EVENT_request_0x12_t;

/*****************************************************************************
 * QMI_WMS_RAW_SEND (32 - 0x0020)
 */

/* Structure to describe TLV 0x01 'Raw Message Data' */
typedef struct {
    uint8_t format;
    uint16_t raw_data_size;
    /*uint8_t raw_data[MAX_RAW_DATA_LEN]; */
} QMI_WMS_RAW_SEND_request_0x01_t;

/* Structure to describe TLV 0x10 'CDMA Force On DC' */
typedef struct {
    uint8_t force;
    uint8_t service_option;
} QMI_WMS_RAW_SEND_request_0x10_t;

/* Structure to describe TLV 0x11 'CDMA Follow On DC' */
typedef struct {
    uint8_t follow;
} QMI_WMS_RAW_SEND_request_0x11_t;

/* Structure to describe TLV 0x12 'GSM WCDMA Link Timer' */
typedef struct {
    uint8_t gsm_wcdma_link_timer;
} QMI_WMS_RAW_SEND_request_0x12_t;

/* Structure to describe TLV 0x13 'SMS on IMS' */
typedef struct {
    uint8_t sms_on_ims;
} QMI_WMS_RAW_SEND_request_0x13_t;

/* Structure to describe TLV 0x14 'Retry Message' */
typedef struct {
    uint8_t retry_message;
} QMI_WMS_RAW_SEND_request_0x14_t;

/* Structure to describe TLV 0x15 'Retry Message ID' */
typedef struct {
    uint32_t retry_message_id;
} QMI_WMS_RAW_SEND_request_0x15_t;

/* Structure to describe TLV 0x16 'Link Enable Mode' */
typedef struct {
    uint8_t link_enable_mode;
} QMI_WMS_RAW_SEND_request_0x16_t;

/* Structure to describe response TLV 0x01 'Message ID' */
typedef struct {
    uint16_t message_id;
} QMI_WMS_RAW_SEND_response_0x01_t;

/* Structure to describe response TLV 0x10 'CDMA Cause Code' */
typedef struct {
    uint16_t cause_code;
} QMI_WMS_RAW_SEND_response_0x10_t;

/* Structure to describe response TLV 0x11 'CDMA Error Class' */
typedef struct {
    uint8_t error_class;
} QMI_WMS_RAW_SEND_response_0x11_t;

/* Structure to describe response TLV 0x12 'GSM WCDMA Cause Info' */
typedef struct {
    uint16_t rp_cause;
    uint8_t tp_cause;
} QMI_WMS_RAW_SEND_response_0x12_t;

/* Structure to describe response TLV 0x13 'Message Delivery Failure Type' */
typedef struct {
    uint8_t message_delivery_failure_type;
} QMI_WMS_RAW_SEND_response_0x13_t;

/*****************************************************************************
 * QMI_WMS_RAW_WRITE (33 - 0x0021)
 */

/* Structure to describe request TLV 0x01 'Raw Message Data' */
typedef struct {
    uint8_t storage_type;
    uint8_t format;
    uint16_t raw_data_size;
} QMI_WMS_RAW_WRITE_request_0x01_t;

/* Structure to describe request TLV 0x10 'Message tag' */
typedef struct {
    uint8_t tag_type;
} QMI_WMS_RAW_WRITE_request_0x10_t;

/* Structure to describe response TLV 0x01 'Memory Index' */
typedef struct {
    uint32_t memory_index;
} QMI_WMS_RAW_WRITE_response_0x01_t;

/*****************************************************************************
 * QMI_WMS_RAW_READ (34 - 0x0022)
 */

/* Structure to describe request TLV 0x01 'Message Memory Storage ID' */
typedef struct {
    uint8_t storage_type;
    uint32_t memory_index;
} QMI_WMS_RAW_READ_request_0x01_t;

/* Structure to describe request TLV 0x10 'Message Mode' */
typedef struct {
    uint8_t message_mode;
} QMI_WMS_RAW_READ_request_0x10_t;

/* Structure to describe request TLV 0x11 'SMS on IMS' */
typedef struct {
    uint8_t sms_on_ims;
} QMI_WMS_RAW_READ_request_0x11_t;

/* Structure to describe response TLV 0x01 'Raw Message Data' */
typedef struct {
    uint8_t message_tag;
    uint8_t format;
    uint16_t raw_data_size;
    /*uint8_t raw_data[MAX_RAW_DATA_LEN]; */
} QMI_WMS_RAW_READ_response_0x01_t;

/*****************************************************************************
 * QMI_WMS_MODIFY_TAG (35 - 0x0023)
 */

/* Structure to describe MODIFY TAG request TLV 0x01 'WMS Message TAG' */
typedef struct {
    uint8_t storage_type;
    uint32_t storage_index;
    uint8_t tag_type;
} QMI_WMS_MODIFY_TAG_request_0x01_t;

/* Structure to describe MODIFY TAG request TLV 0x10 'Message Mode' */
typedef struct {
    uint8_t message_mode;
} QMI_WMS_MODIFY_TAG_request_0x10_t;

/*****************************************************************************
 * QMI_WMS_DELETE (36 - 0x0024)
 */

/* Structure to describe WMS DELETE request TLV 0x01 storage type */
typedef struct {
    uint8_t storage_type;
} QMI_WMS_DELETE_request_0x01_t;

/* Structure to describe WMS DELETE request TLV 0x10 'Memory Index' */
typedef struct {
    uint32_t memory_index;
} QMI_WMS_DELETE_request_0x10_t;

/* Structure to describe WMS DELETE request TLV 0x11 'Message tag' */
typedef struct {
    uint8_t tag_type;
} QMI_WMS_DELETE_request_0x11_t;

/* Structure to describe WMS DELETE request TLV 0x12 'Message Mode' */
typedef struct {
    uint8_t message_mode;
} QMI_WMS_DELETE_request_0x12_t;

/*****************************************************************************
 * QMI_WMS_LIST_MESSAGES (49 - 0x0031)
 */

/* Structure to describe WMS LIST MESSAGES request TLV 0x01 storage type */
typedef struct {
    uint8_t storage_type;
} QMI_WMS_LIST_MESSAGES_request_0x01_t;

/* Structure to describe WMS LIST MESSAGES request TLV 0x10 'Message tag' */
typedef struct {
    uint8_t requested_tag;
} QMI_WMS_LIST_MESSAGES_request_0x10_t;

/* Structure to describe WMS LIST MESSAGES request TLV 0x11 'Message Mode' */
typedef struct {
    uint8_t message_mode;
} QMI_WMS_LIST_MESSAGES_request_0x11_t;

/* Structure to describe WMS LIST MESSAGES response TLV 0x01 'Message List' */
typedef struct {
    uint32_t index;
    uint8_t tag_type;
} QMI_WMS_generic_list_messages_item_t;

typedef struct {
    uint32_t message_list_size;
    /*QMI_WMS_generic_list_messages_item_t list[message_list_size] */
} QMI_WMS_LIST_MESSAGES_response_0x01_t;

/*****************************************************************************
 * QMI_WMS_SET_ROUTES (50 - 0x0032)
 */

/* QMI_WMS_SET_ROUTES generic route */
typedef struct {
    uint8_t message_type;
    uint8_t message_class;
    uint8_t route_storage;
    uint8_t receipt_action;
} QMI_WMS_generic_route_t;

/* Structure to describe SET ROUTES request TLV 0x01 */
typedef struct {
    uint16_t routes_size;
    /*QMI_WMS_generic_route_t route[routes_size]; */
} QMI_WMS_SET_ROUTES_request_0x01_t;

/* Structure to describe SET ROUTES request TLV 0x10 */
typedef struct {
    uint8_t transfer_ind;
} QMI_WMS_SET_ROUTES_request_0x10_t;

/*****************************************************************************
 * QMI_WMS_GET_SMSC_ADDR (52 - 0x0034)
 */

typedef struct {
    char smsc_address_type[3];
    uint8_t smsc_address_length;
    /*char smsc_address_digits[smsc_address_length]; */
} QMI_WMS_GET_SMSC_ADDRESS_response_0x01_t;

/*****************************************************************************
 * QMI_WMS_SEND_ACK (55 - 0x0037)
 */

/* Structure to describe SEND ACK request TLV 0x01 'ACK Information' */
typedef struct {
    uint32_t transaction_id;
    uint8_t message_protocol;
    uint8_t success;
} QMI_WMS_SEND_ACK_request_0x01_t;

/*****************************************************************************
 * QMI WMS indications
 */

/* Structure to describe EVENT indication TLV 0x10 'Report MT Message' */
typedef struct {
    uint8_t storage_type;
    uint32_t storage_index;
} QMI_WMS_EVENT_IND_response_0x10_t;

/* Structure to describe EVENT indication TLV 0x11 'Transfer Route MT Message' */
typedef struct {
    uint8_t ack_indicator;
    uint32_t transaction_id;
    uint8_t format;
    uint16_t size;
    /*uint8_t *data; */
} QMI_WMS_EVENT_IND_response_0x11_t;

/* Structure to describe EVENT indication TLV 0x12 'Message Mode' */
typedef struct {
    uint8_t message_mode;
} QMI_WMS_EVENT_IND_response_0x12_t;

/* Structure to describe EVENT indication TLV 0x13 'ETWS Message' */
typedef struct {
    uint8_t notification_type;
    uint16_t size;
    /*uint8_t *data; */
} QMI_WMS_EVENT_IND_response_0x13_t;

/* Structure to describe EVENT indication TLV 0x14 'ETWS PLMN Information' */
typedef struct {
    uint16_t mobile_country_code;
    uint16_t mobile_network_code;
} QMI_WMS_EVENT_IND_response_0x14_t;

/* Structure to describe EVENT indication TLV 0x15 'SMSC Address' */
typedef struct {
    uint8_t len;
    /*uint8_t *data; */
} QMI_WMS_EVENT_IND_response_0x15_t;

/* Structure to describe EVENT indication TLV 0x16 'SMS on IMS' */
typedef struct {
    uint8_t sms_on_ims;
} QMI_WMS_EVENT_IND_response_0x16_t;

/* Structure to describe EVENT indication TLV 0x18 'Broadcast SMS RAT information' */
typedef struct {
    uint32_t bc_sms_rat_info;
} QMI_WMS_EVENT_IND_response_0x18_t;

/* Structure to describe MEMORY_FULL indication TLV 0x01 */
typedef struct {
    uint8_t storage_type;
    uint8_t message_mode;
} QMI_WMS_MEMORY_FULL_indication_0x01_t;
#pragma pack( pop )

#endif  /* _TELIT_SDK_PRIVATE_WMS_H_ */
