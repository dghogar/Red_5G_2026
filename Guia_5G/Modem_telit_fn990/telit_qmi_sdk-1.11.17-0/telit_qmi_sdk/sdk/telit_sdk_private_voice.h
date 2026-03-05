#ifndef TELIT_SDK_PRIVATE_VOICE_H
#define TELIT_SDK_PRIVATE_VOICE_H

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_VOICE_INDICATION_REGISTER (3 - 0x0003)
 */

/* Structure to describe generic event TLV for QMI_VOICE_INDICATION_REGISTER request */
typedef struct {
    uint8_t event;
} QMI_VOICE_INDICATION_REGISTER_event_t;

/*****************************************************************************
 * QMI_VOICE_DIAL_CALL (32 - 0x0020)
 */

/* Structure to describe TLV 0x11 for QMI_VOICE_DIAL_CALL request */
typedef struct {
    uint8_t clir_type;
} QMI_VOICE_DIAL_CALL_request_0x11_t;

/* Structure to describe generic call id TLV */
typedef struct {
    uint8_t call_id;
} QMI_VOICE_generic_call_id_t;

/* Structure to describe TLV 0x14 for QMI_VOICE_DIAL_CALL response */
typedef struct {
    uint16_t end_reason;
} QMI_VOICE_DIAL_CALL_response_0x14_t;

/*****************************************************************************
 * QMI_VOICE_END_CALL (33 - 0x0021)
 */

/* Structure to describe TLV 0x14 for QMI_VOICE_END_CALL request */
typedef struct {
    uint32_t end_reason;
} QMI_VOICE_END_CALL_request_0x10_t;

/*****************************************************************************
 * QMI_VOICE_ANSWER_CALL (34 - 0x0022)
 */

/* Structure to describe TLV 0x15 for QMI_VOICE_ANSWER_CALL request */
typedef struct {
    uint8_t reject_call;
} QMI_VOICE_ANSWER_CALL_request_0x15_t;

/* Structure to describe generic reject cause */
typedef struct {
    uint32_t reject_cause;
} QMI_VOICE_generic_reject_cause_t;

/*****************************************************************************
 * QMI_VOICE_BURST_DTMF (40 - 0x0028)
 */

/* Structure to describe TLV 0x01 for QMI_VOICE_BURST_DTMF request */
typedef struct {
    uint8_t call_id;
    uint8_t digit_cnt;
    /* char digit_buffer[digit_cnt]; */
} QMI_VOICE_BURST_DTMF_request_0x01_t;

/* Structure to describe TLV 0x10 for QMI_VOICE_BURST_DTMF request */
typedef struct {
    uint8_t dtmf_onlength;
    uint8_t dtmf_offlength;
} QMI_VOICE_BURST_DTMF_request_0x10_t;

/*****************************************************************************
 * QMI_VOICE_START_CONT_DTMF (41 - 0x0029)
 */

/* Structure to describe TLV 0x01 for QMI_VOICE_START_CONT_DTMF request */
typedef struct {
    uint8_t call_id;
    uint8_t digit;
} QMI_VOICE_START_CONT_DTMF_request_0x01_t;

/*****************************************************************************
 * QMI_VOICE_STOP_CONT_DTMF (42 - 0x002A)
 */

/* Structure to describe TLV 0x01 for QMI_VOICE_STOP_CONT_DTMF request */
typedef struct {
    uint8_t call_id;
} QMI_VOICE_STOP_CONT_DTMF_request_0x01_t;

/*****************************************************************************
 * QMI_VOICE_GET_ALL_CALL_INFO (47 - 0x002F)
 */

typedef struct {
    uint8_t call_id;
    uint8_t call_state;
    uint8_t call_type;
    uint8_t direction;
    uint8_t mode;
    uint8_t is_mpty;
    uint8_t als;
} QMI_VOICE_GET_ALL_CALL_INFO_generic_info_instance_t;

/* Structure to describe TLV 0x01 for tQMI_VOICE_ALL_CALL_STATUS_IND */
typedef struct {
    uint8_t num_of_instances;
    /* call_info_instance_t call_info[num_of_instances]; */
} QMI_VOICE_GET_ALL_CALL_INFO_generic_info_t;

typedef struct {
    uint8_t call_id;
    uint8_t number_pi;
    uint8_t number_len;
    /* char number[number_len]; */
} QMI_VOICE_array_remote_party_number_t;

/* Structure to describe TLV 0x11 for QMI_VOICE_GET_ALL_CALL_INFO request */
typedef struct {
    uint8_t num_of_instances;
    /* QMI_VOICE_array_remote_party_number_t array[num_of_instances]; */
} QMI_VOICE_GET_ALL_CALL_INFO_response_0x11_t;

typedef struct {
    uint8_t call_id;
    uint8_t name_pi;
    uint8_t coding;
    uint8_t name_len;
    /* char name[name_len]; */
} QMI_VOICE_array_remote_party_name_t;

/* Structure to describe TLV 0x12 for QMI_VOICE_GET_ALL_CALL_INFO request */
typedef struct {
    uint8_t num_of_instances;
    /* QMI_VOICE_array_remote_party_name_t array[num_of_instances]; */
} QMI_VOICE_GET_ALL_CALL_INFO_response_0x12_t;

typedef struct {
    uint8_t call_id;
    uint8_t uus_type;
    uint8_t uus_dcs;
    uint8_t uus_data_len;
    /*uint8_t uus_data[uus_data_len]; */
} QMI_VOICE_array_uus_info_t;

/* Structure to describe TLV 0x14 for QMI_VOICE_GET_ALL_CALL_INFO request */
typedef struct {
    uint8_t num_of_instances;
    /* QMI_VOICE_array_uus_info_t array[num_of_instances]; */
} QMI_VOICE_GET_ALL_CALL_INFO_response_0x14_t;

typedef struct {
    uint8_t call_id;
    uint8_t conn_num_pi;
    uint8_t conn_num_si;
    uint8_t conn_num_type;
    uint8_t conn_num_plan;
    uint8_t conn_num_len;
    /*char conn_num[conn_num_len]; */
} QMI_VOICE_array_conn_party_number_t;

/* Structure to describe TLV 0x1A for QMI_VOICE_GET_ALL_CALL_INFO request */
typedef struct {
    uint8_t num_of_instances;
    /* QMI_VOICE_array_conn_party_number_t array[num_of_instances]; */
} QMI_VOICE_GET_ALL_CALL_INFO_response_0x1A_t;

/*****************************************************************************
 * QMI_VOICE_MANAGE_CALLS (49 - 0x0031)
 */

/* Structure to describe TLV 0x01 for QMI_VOICE_MANAGE_CALLS request */
typedef struct {
    uint8_t sups_type;
} QMI_VOICE_MANAGE_CALLS_request_0x01_t;

/* Structure to describe TLV 0x10 for QMI_VOICE_MANAGE_CALLS response */
typedef struct {
    uint16_t failure_cause;
} QMI_VOICE_generic_failure_cause_t;

/*****************************************************************************
 * QMI_VOICE_SET_SUPS_SERVICE (51 - 0x0033)
 * QMI_VOICE_GET_CALL_WAITING (52 - 0x0034)
 */

/* Structure to describe TLV 0x01 for QMI_VOICE_SET_SUPS_SERVICE request */
typedef struct {
    uint8_t voice_service;
    uint8_t reason;
} QMI_VOICE_SET_SUPS_SERVICE_request_0x01_t;

/* Structure to describe generic service class */
typedef struct {
    uint8_t service_class;
} QMI_VOICE_generic_service_class_t;

/* Structure to describe TLV 0x13 for QMI_VOICE_SET_SUPS_SERVICE request */
typedef struct {
    uint8_t timer_value;
} QMI_VOICE_SET_SUPS_SERVICE_request_0x13_t;

/* Structure to describe TLV 0x14 for QMI_VOICE_SET_SUPS_SERVICE request */
typedef struct {
    uint8_t num_type;
    uint8_t num_plan;
} QMI_VOICE_SET_SUPS_SERVICE_request_0x14_t;

/* Structure to describe TLV 0x11 for QMI_VOICE_GET_CALL_WAITING request */
typedef struct {
    uint32_t service_class_ext;
} QMI_VOICE_generic_service_class_ext_t;

/* Structure to describe generic call barring number */
typedef struct {
    uint8_t barred_number_len;
    /* uint8_t barred_number[barred_number_len]; */
} QMI_VOICE_call_barring_generic_number_t;

/* Structure to describe TLV 0x16 for QMI_VOICE_SET_SUPS_SERVICE request */
typedef struct {
    uint8_t num_of_instances;
    /* QMI_VOICE_call_barring_generic_number_t call_barring_numbers[num_of_instances]; */
} QMI_VOICE_SET_SUPS_SERVICE_request_0x16_t;

/*****************************************************************************
 * QMI_VOICE_GET_CALL_BARRING (53 - 0x0035)
 */

/* Structure to describe generic service reason */
typedef struct {
    uint8_t reason;
} QMI_VOICE_generic_service_reason_t;

typedef struct {
    uint8_t active_status;
    uint8_t number_len;
    /* uint8_t number[number_len]; */
} QMI_VOICE_call_barring_number_instance_t;

typedef struct {
    uint32_t service_class_ext;
    uint8_t number_list_size;
    /* QMI_VOICE_call_barring_number_instance_t number_list[number_list_size]; */
} QMI_VOICE_call_barring_service_instance_t;

/* Structure to describe TLV 0x17 for QMI_VOICE_GET_CALL_BARRING response */
typedef struct {
    uint8_t call_barring_service_size;
    /* QMI_VOICE_call_barring_service_instance_t barring_services[call_barring_service_size] */
} QMI_VOICE_GET_CALL_BARRING_response_0x17_t;

/*****************************************************************************
 * QMI_VOICE_GET_CLIR (55 - 0x0037)
 */

/* Structure to describe generic provision status */
typedef struct {
    uint8_t active_status;
    uint8_t provision_status;
} QMI_VOICE_generic_provision_t;

/*****************************************************************************
 * QMI_VOICE_GET_CALL_FORWARDING (56 - 0x0038)
 */

typedef struct {
    uint8_t service_status;
    uint8_t service_class;
    uint8_t number_len;
    /* uint8_t number[number_len]; */
    /* uint8_t no_reply_timer; */
} QMI_VOICE_call_forwarding_info_instance_t;

/* Structure to describe TLV 0x10 for QMI_VOICE_GET_CALL_FORWARDING response */
typedef struct {
    uint8_t num_of_instances;
    /* QMI_VOICE_call_forwarding_info_instance_t call_forwarding_info[num_of_instances]; */
} QMI_VOICE_GET_CALL_FORWARDING_response_0x10_t;

/*****************************************************************************
 * QMI_VOICE_SET_CALL_BARRING_PASSWORD (57 - 0x0039)
 */

/* Structure to describe TLV 0x01 for QMI_VOICE_SET_CALL_BARRING_PASSWORD request */
typedef struct {
    uint8_t reason;
    char old_password[4];
    char new_password[4];
    char new_password_again[4];
} QMI_VOICE_SET_CALL_BARRING_PASSWORD_request_0x01_t;

/*****************************************************************************
 * QMI VOICE indications
 */

/* Structure to describe TLV 0x01 for QMI_VOICE_CONFERENCE_INFO_IND */
typedef struct {
    uint16_t conference_xml_len;
    /* uint8_t conference_xml[conference_xml_len]; */
} QMI_VOICE_CONFERENCE_INFO_IND_0x01_t;

/* Structure to describe TLV 0x02 for QMI_VOICE_CONFERENCE_INFO_IND */
typedef struct {
    uint32_t sequence;
} QMI_VOICE_CONFERENCE_INFO_IND_0x02_t;

/* Structure to describe TLV 0x10 for QMI_VOICE_CONFERENCE_INFO_IND */
typedef struct {
    uint32_t total_size;
} QMI_VOICE_CONFERENCE_INFO_IND_0x10_t;

/* Structure to describe TLV 0x02 for QMI_VOICE_CONFERENCE_INFO_IND */
typedef struct {
    uint8_t uri_name_len;
    /* uint16_t uri_name[uri_name_len]; */
    uint8_t uri_description_len;
    /* uint16_t uri_description[uri_description_len]; */
} QMI_VOICE_generic_partecipant_info_t;

/* Structure to describe TLV 0x01 for QMI_VOICE_SUPS_IND request */
typedef struct {
    uint8_t service_type;
    uint8_t is_modified_by_call_control;
} QMI_VOICE_SUPS_IND_0x01_t;

/* Structure to describe TLV 0x01 for QMI_VOICE_SUPS_NOTIFICATION_IND request */
typedef struct {
    uint8_t call_id;
    uint8_t notification_type;
} QMI_VOICE_SUPS_NOTIFICATION_IND_0x01_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_VOICE_H */
