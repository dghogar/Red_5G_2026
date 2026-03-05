#ifndef TELIT_SDK_PRIVATE_H_
#define TELIT_SDK_PRIVATE_H_

#define QMI_TIMEOUT_SMALL 5
#define QMI_TIMEOUT_MEDIUM 20
#define QMI_TIMEOUT_BIG 200

/* PACK raw data into a TLV */
#define PACK_TLV_ARRAY(TLV_ARRAY_LEN, SRC_ARRAY, SRC_ARRAY_LEN, MAX_LEN) { \
            uint16_t *array_ptr; \
            array_ptr = (uint16_t *)(&(TLV_ARRAY_LEN)); \
            array_ptr++;  /* Moving to the start of the string */ \
            memcpy(array_ptr, \
        SRC_ARRAY, \
        SRC_ARRAY_LEN > MAX_LEN ? \
        MAX_LEN : \
        SRC_ARRAY_LEN); \
}

/* UNPACK TLV raw data */
#define UNPACK_ARRAY(TLV_DATA_LEN, DATA_LEN, DATA, MAX_LEN) { \
            uint16_t *raw_data_ptr; \
            raw_data_ptr = (uint16_t *)(&(TLV_DATA_LEN)); \
            raw_data_ptr++;  /* Moving to the start of the string */ \
            memcpy(DATA, \
        raw_data_ptr, \
        DATA_LEN > MAX_LEN ? \
        MAX_LEN : \
        DATA_LEN); \
}

#define CHECK_PACK_TLV_SIZE(TLV_SIZE, HEADER_LEN, PAYLOAD_LEN, REAL_LEN) { \
            PAYLOAD_LEN += HEADER_LEN + TLV_SIZE; \
            ret = validate_pack(PAYLOAD_LEN, REAL_LEN); \
            if (ret) { \
                RLOGE("%s: validate pack failed", __FUNCTION__); \
                goto end; \
            } \
} \

#define CHECK_UNPACK_TLV_SIZE(TLV_STRUCT) { \
            uint16_t tlv_len = le16toh(hdr->length); \
            uint16_t data_structure_len = sizeof(TLV_STRUCT); \
            if (tlv_len < data_structure_len) { \
                RLOGE("%s: malformed TLV %s, returned %u, minimum %u", \
        __FUNCTION__, #TLV_STRUCT, tlv_len, data_structure_len); \
                ret = QMI_SDK_ERR_MALFORMED_RSP; \
                goto end; \
            } \
} \

#define COPY_HTOM(DST, SRC) { ec_htom(&(DST), sizeof(DST), &(SRC), sizeof(SRC)); }
#define COPY_MTOH(DST, SRC) { ec_mtoh(&(DST), sizeof(DST), &(SRC), sizeof(SRC)); }

#define FIND_MANDATORY_TLV(MESSAGE, TLV_ID, PAYLOAD_LEN, HEADER) { \
            HEADER = search_TLV(MESSAGE, TLV_ID, PAYLOAD_LEN); \
            if (!HEADER) { \
                RLOGE("%s: missing mandatory TLV", __FUNCTION__); \
                ret = QMI_SDK_ERR_INVALID_RSP; \
                goto end; \
            } \
} \

#pragma pack( push, 1 )

/* qmux header: to be used only with qmi_wwan based devices,
 * since Gobinet is managing qmux header in driver
 */
typedef struct QMI_qmux_header
{
    uint8_t if_type;       /* Always 0x01 */
    uint16_t length;       /* Including qmux header without if_type */
    uint8_t control_flags; /* 1 service, 0 control point */
    uint8_t service_type;  /* Service */
    uint8_t client_id;     /* Client id */
} QMI_qmux_header_t;

/* Structure to represent CTL Service Raw Transaction Header */
typedef struct QMI_CTL_raw_transaction_header
{
    uint8_t control_flags;
    uint8_t transaction_id;
} QMI_CTL_raw_transaction_header_t;

/* Structure to represent a QMI (control/service) content */
typedef struct QMI_raw_content_header
{
    uint8_t type_id;
    uint16_t length;
} QMI_raw_content_header_t;

/* Structure to represent a QMI Service Raw Transaction Header */
typedef struct QMI_service_raw_transaction_header
{
    uint8_t compound : 1;
    uint8_t response : 1;
    uint8_t indication : 1;
    uint8_t reserved : 5;
    uint16_t transaction_id;
} QMI_service_raw_transaction_header_t;

/* Structure to represent a QMI Raw Message Header */
typedef struct QMI_raw_message_header
{
    uint16_t msg_id;
    uint16_t len;

    /* This array must be the size specified by len */
    /* uint8_t TLV[len]; */
} QMI_raw_message_header_t;

#pragma pack( pop )

void add_header(const pack_qmi_t *context,
        uint16_t payloadlen,
        uint8_t *buffer,
        uint16_t *bufferLen);

QMI_SDK_error_t pack_request(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len);

QMI_SDK_error_t get_string(uint8_t *buf_in,
        uint16_t buf_len,
        uint8_t tlv_id,
        uint16_t str_sz,
        char *str);

uint16_t get_payload_size(uint8_t *buf);

QMI_SDK_error_t generic_pack_request(pack_qmi_t *req_ctx, uint8_t *req, uint16_t *len,
        uint16_t msg_id, uint8_t svc, int timeout);
QMI_SDK_error_t generic_unpack_request(uint8_t *rsp, uint16_t len);
QMI_SDK_error_t validate_message(uint8_t *rsp, uint16_t len, uint16_t *error);
QMI_SDK_error_t validate_unpack(uint8_t *rsp, uint16_t len, void *out, bool have_response,
        uint16_t *payload_len, uint8_t **msg);
QMI_SDK_error_t validate_pack(uint16_t payloadlen, uint16_t bufferLen);
void *skip_msg_header(void *buf);
void ec_mtoh(void *dest, size_t dest_size, void *src, size_t src_size);
void ec_htom(void *dest, size_t dest_size, void *src, size_t src_size);
QMI_raw_content_header_t *search_TLV(const uint8_t *buf, uint8_t tlv, uint32_t rsp_sz);

#endif  /* TELIT_SDK_PRIVATE_H_ */
