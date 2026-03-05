#ifndef TELIT_SDK_PRIVATE_UIM_H_
#define TELIT_SDK_PRIVATE_UIM_H_

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_UIM_READ_TRANSPARENT (32 - 0x0020)
 * QMI_UIM_READ_RECORD (33 - 0x0021)
 */

/* Structure to describe a generic file */
typedef struct {
    uint16_t file_id;
    uint8_t path_len;
    /* This array must be the size specified by pin_len */
    /* uint8_t path[path_len]; */
} QMI_UIM_generic_file_id_t;

/* Structure to describe a generic read operation */
typedef struct {
    uint16_t offset;
    uint16_t length;
} QMI_UIM_generic_read_info_t;

/* Structure to describe a generic card result */
typedef struct {
    uint8_t sw1;
    uint8_t sw2;
} QMI_UIM_generic_card_result_t;

/* Structure to describe a generic read content */
typedef struct {
    uint16_t content_len;
    /* This array must be the size specified by pin_len */
    /* uint8_t content[content_len]; */
} QMI_UIM_generic_read_content_t;

/* Structure to describe a generic session */
typedef struct {
    uint8_t session_type;
    uint8_t aid_len;
    /* This array must be the size specified by aid_len */
    /* uint8_t aid[aid_len]; */
} QMI_UIM_generic_session_t;

/* Structure to describe a generic indication token */
typedef struct {
    uint32_t indication_token;
} QMI_UIM_generic_ind_token_t;

/*****************************************************************************
 * QMI_UIM_GET_FILE_ATTRIBUTES (36 - 0x0024)
 */

/* Structure to describe response TLV 0x11 for QMI_UIM_GET_FILE_ATTRIBUTES */
typedef struct {
    uint16_t file_size;
    uint16_t file_id;
    uint8_t file_type;
    uint16_t rec_size;
    uint16_t rec_count;
    uint8_t sec_read;
    uint16_t sec_read_mask;
    uint8_t sec_write;
    uint16_t sec_write_mask;
    uint8_t sec_increase;
    uint16_t sec_increase_mask;
    uint8_t sec_deactivate;
    uint16_t sec_deactivate_mask;
    uint8_t sec_activate;
    uint16_t sec_activate_mask;
    uint16_t raw_len;
    /* This array must be the size specified by raw_len */
    /* uint8_t raw_value[raw_len]; */
} QMI_UIM_GET_FILE_ATTRIBUTES_response_0x11_t;

/*****************************************************************************
 * QMI_UIM_SET_PIN_PROTECTION (37 - 0x0025)
 */

/* Structure to describe request TLV 0x02 for QMI_UIM_SET_PIN_PROTECTION */
typedef struct {
    uint8_t pin_id;
    uint8_t pin_operation;
    uint8_t pin_len;
    /* This array must be the size specified by pin_len */
    /* uint8_t pin[pin_len]; */
} QMI_UIM_SET_PIN_PROTECTION_request_0x02_t;

/* Structure to describe a generic key ref */
typedef struct {
    uint8_t pin_key_ref_id;
} QMI_UIM_PIN_generic_key_ref_t;

/* Structure to describe generic response TLV 0x10 for QMI_UIM_PIN */
typedef struct {
    uint8_t verify_left;
    uint8_t unblock_left;
} QMI_UIM_PIN_response_0x10_t;

/* Structure to describe generic response TLV 0x11 for QMI_UIM_PIN */
typedef struct {
    uint8_t enc_pin1_len;
    /* This array must be the size specified by enc_pin1_len */
    /* uint8_t enc_pin1[enc_pin1_len]; */
} QMI_UIM_PIN_response_0x11_t;

/*****************************************************************************
 * QMI_UIM_VERIFY_PIN (38 - 0x0026)
 */

/* Structure to describe request TLV 0x02 for QMI_UIM_VERIFY_PIN */
typedef struct {
    uint8_t pin_id;
    uint8_t pin_len;
    /* This array must be the size specified by pin_len */
    /* uint8_t pin[pin_len]; */
} QMI_UIM_VERIFY_PIN_request_0x02_t;

/* Structure to describe request TLV 0x10 for QMI_UIM_VERIFY_PIN */
typedef struct {
    uint8_t enc_pin1_len;
    /* This array must be the size specified by enc_pin1_len */
    /* uint8_t enc_pin1[enc_pin1_len]; */
} QMI_UIM_VERIFY_PIN_request_0x10_t;

/*****************************************************************************
 * QMI_UIM_UNBLOCK_PIN (39 - 0x0027)
 */

/* Structure to describe request TLV 0x02 for QMI_UIM_UNBLOCK_PIN */
typedef struct {
    uint8_t pin_id;
    uint8_t puk_len;
    /* The following are only indicated since are variable in size */
    /* uint8_t puk[puk_len]; */
    /* uint8_t new_pin_len; */
    /* uint8_t new_pin[new_pin_len]; */
} QMI_UIM_UNBLOCK_PIN_request_0x02_t;

/*****************************************************************************
 * QMI_UIM_CHANGE_PIN (40 - 0x0028)
 */

/* Structure to describe request TLV 0x02 for QMI_UIM_CHANGE_PIN */
typedef struct {
    uint8_t pin_id;
    uint8_t old_pin_len;
    /* The following are only indicated since are of variable size */
    /* uint8_t old_pin[old_pin_len]; */
    /* uint8_t new_pin_len; */
    /* uint8_t new_pin[new_pin_len]; */
} QMI_UIM_CHANGE_PIN_request_0x02_t;

/*****************************************************************************
 * QMI_UIM_EVENT_REG (46 - 0x002E)
 */

/* Structure to describe request QMI_UIM_EVENT_REG generic mask */
typedef struct {
    uint32_t bitmask;
} QMI_UIM_EVENT_REG_generic_mask_t;

/*****************************************************************************
 * QMI_UIM_GET_CARD_STATUS (47 - 0x002F)
 */

/* Helper struct for parsing app instances */
typedef struct {
    uint8_t univ_pin;
    uint8_t pin1_state;
    uint8_t pin1_retries;
    uint8_t puk1_retries;
    uint8_t pin2_state;
    uint8_t pin2_retries;
    uint8_t puk2_retries;
} QMI_UIM_GET_CARD_STATUS_request_app_instance_sub1_t;

/* Structure to describe a generic app instance */
typedef struct {
    uint8_t type;
    uint8_t state;
    uint8_t perso_state;
    uint8_t perso_feature;
    uint8_t perso_retries;
    uint8_t perso_unblock_retries;
    uint8_t aid_len;
    /* The following are only indicated since are variable in size */
    /* uint8_t aid[aid_len];
       uint8_t univ_pin;
       uint8_t pin1_state;
       uint8_t pin1_retries;
       uint8_t puk1_retries;
       uint8_t pin2_state;
       uint8_t pin2_retries;
       uint8_t puk2_retries; */
} QMI_UIM_GET_CARD_STATUS_request_app_instance_t;

/* Structure to describe a generic slot instance */
typedef struct  {
    uint8_t card_state;
    uint8_t upin_state;
    uint8_t upin_retries;
    uint8_t upuk_retries;
    uint8_t error;
    uint8_t num_app;
    /* This array must be the size specified by num_app */
    /* QMI_UIM_GET_CARD_STATUS_request_app_instance_t slot[num_app]; */
} QMI_UIM_GET_CARD_STATUS_request_slot_instance_t;

/* Structure to describe request TLV 0x10 for QMI_UIM_GET_CARD_STATUS */
typedef struct {
    uint16_t index_gw_pri;
    uint16_t index_1x_pri;
    uint16_t index_gw_sec;
    uint16_t index_1x_sec;
    uint8_t num_slot;
    /* This array must be the size specified by num_slot */
    /* QMI_UIM_GET_CARD_STATUS_request_slot_instance_t slot[num_slot]; */
} QMI_UIM_GET_CARD_STATUS_request_0x10_t;

/* Structure to describe request TLV 0x11 for QMI_UIM_GET_CARD_STATUS */
typedef struct {
    uint8_t hot_swap_len;
    /* This array must be the size specified by hot_swap_len */
    /* uint8_t hot_swap[hot_swap_len]; */
} QMI_UIM_GET_CARD_STATUS_request_0x11_t;

/*****************************************************************************
 * QMI_UIM_POWER_DOWN (48 - 0x0030)
 * QMI_UIM_POWER_UP (49 - 0x0031)
 */

/* Structure to describe generic slot */
typedef struct {
    uint8_t slot;
} QMI_UIM_generic_slot_t;

/*****************************************************************************
 * QMI_UIM_CHANGE_PROVISIONING_SESSION (56 - 0x0038)
 */

/* Structure to describe request TLV 0x01 for QMI_UIM_CHANGE_PROVISIONING_SESSION */
typedef struct {
    uint8_t session_type;
    uint8_t activate;
} QMI_UIM_CHANGE_PROVISIONING_SESSION_request_0x01_t;

/* Structure to describe request TLV 0x010 for QMI_UIM_CHANGE_PROVISIONING_SESSION */
typedef struct {
    uint8_t slot;
    uint8_t aid_len;
    /* This array must be the size specified by aid_len */
    /* uint8_t aid[aid_len]; */
} QMI_UIM_CHANGE_PROVISIONING_SESSION_request_0x10_t;

/*****************************************************************************
 * QMI_UIM_SEND_APDU (59 - 0x003B)
 */

/* Structure to describe generic APDU */
typedef struct {
    uint16_t len;
    /* This array must be the size specified by len */
    /* uint8_t apdu[len]; */
} QMI_UIM_generic_apdu_t;

/* Structure to describe request TLV 0x11 for QMI_UIM_SEND_APDU */
typedef struct {
    uint8_t procedure_bytes;
} QMI_UIM_SEND_APDU_request_0x11_t;

/* Structure to describe response TLV 0x11 for QMI_UIM_SEND_APDU */
typedef struct {
    uint16_t total_length;
    uint32_t token;
} QMI_UIM_SEND_APDU_response_0x11_t;

/*****************************************************************************
 * QMI_UIM_LOGICAL_CHANNEL (63 - 0x003F)
 */

/* Structure to describe a generic aid */
typedef struct {
    uint8_t aid_len;
    /* This array must be the size specified by aid_len */
    /* uint8_t aid[aid_len]; */
} QMI_UIM_generic_aid_t;

/* Structure to describe a generic channel id */
typedef struct {
    uint8_t channel_id;
} QMI_UIM_generic_channel_id_t;

/* Structure to describe response TLV 0x12 for QMI_UIM_LOGICAL_CHANNEL */
typedef struct {
    uint8_t len;
    /* This array must be the size specified by len */
    /* uint8_t resp[len]; */
} QMI_UIM_LOGICAL_CHANNEL_response_0x12_t;

/* Structure to describe response TLV 0x13 for QMI_UIM_LOGICAL_CHANNEL */
typedef struct {
    uint16_t len;
    /* This array must be the size specified by len */
    /* uint8_t resp[len]; */
} QMI_UIM_LOGICAL_CHANNEL_response_0x13_t;

/*****************************************************************************
 * QMI_UIM_SWITCH_SLOT (70 - 0x0046)
 */

/* Structure to describe request TLV 0x01 for QMI_UIM_SWITCH_SLOT */
typedef struct {
    uint8_t logical_slot;
} QMI_UIM_SWITCH_SLOT_request_0x01_t;

/* Structure to describe request TLV 0x02 for QMI_UIM_SWITCH_SLOT */
typedef struct {
    uint32_t physical_slot;
} QMI_UIM_SWITCH_SLOT_request_0x02_t;

/*****************************************************************************
 * QMI_UIM_GET_SLOTS_STATUS (71 - 0x0047)
 */

/* Structure to describe a generic slot status instance */
typedef struct {
    uint32_t physical_card_status;
    uint32_t physical_slot_state;
    uint8_t logical_slot;
    uint8_t iccid_len;
    /* This array must be the size specified by iccid_len */
    /* uint8_t iccid[MAX_STRING_SIZE]; */
} QMI_UIM_GET_SLOT_STATUS_response_slot_instance_t;

/* Structure to describe response TLV 0x10 for QMI_UIM_GET_SLOT_STATUS */
typedef struct {
    uint8_t slot_size;
    /* This array must be the size specified by pin_len */
    /* QMI_UIM_GET_SLOT_STATUS_response_slot_instance_t slot[slot_size]; */
} QMI_UIM_GET_SLOT_STATUS_response_0x10_t;

/*****************************************************************************
 * QMI UIM indications
 */

/* Structure to describe request TLV 0x11 for QMI_UIM_STATUS_CHANGE_IND */
typedef struct {
    uint8_t card_status_validity_len;
    /* This array must be the size specified by card_status_validity_len */
    /* uint8_t card_status_validity[card_status_validity_len]; */
} QMI_UIM_STATUS_CHANGE_IND_indication_0x11_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_UIM_H_ */
