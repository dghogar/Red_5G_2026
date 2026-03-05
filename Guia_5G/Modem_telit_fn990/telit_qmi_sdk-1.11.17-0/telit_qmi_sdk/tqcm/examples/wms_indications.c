#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_wms.h>
#include <telit_sms_helper.h>
#include <tqcm.h>
#include <tqcm_wms.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

static sem_t mutex_indication;

static uint32_t storage_index;
static uint8_t storage_type;

/* Set this when testing */
#define DESTINATION_NUMBER ""

static void cb(void *item)
{
    int ret;
    indication_item_t *ind = (indication_item_t *)item;

    switch (ind->msg_id) {
        case QMI_WMS_EVENT_REPORT_IND: {
            wms_ind_event_report_t out;

            RLOGI("Received QMI_WMS_EVENT_REPORT_IND");

            ret = telit_wms_indication_handler(ind->msg_id,
                    ind->buf, ind->buf_len,
                    (uint8_t *)&out, sizeof(out));

            if (ret != QMI_SDK_ERR_NONE) {
                RLOGE("unpack fail %d ", ret);
            }

            if (out.mt_message_set) {
                storage_index = out.mt_message.storage_index;
                storage_type = out.mt_message.storage_type;
            }

            if (out.transfer_route_mt_message_set) {
                RLOGI("SMS from indication");
                if (out.smsc_address_set) {
                    telit_print_buffer(out.smsc_address.data, out.smsc_address.len, LOG_INFO);
                }
                telit_print_buffer(out.transfer_route_mt_message.data, out.transfer_route_mt_message.size,
                        LOG_INFO);
            }
            sem_post(&mutex_indication);
            break;
        }
        default:
            RLOGI("Indication not managed (%u)", ind->msg_id);
            break;
    }

    free(ind->buf);
    free(ind);
}

static int send_test_sms(tqcm_client_context_t *ctx)
{
    uint16_t sdk_error;
    wms_raw_send_t input = {0};
    wms_raw_send_resp_t out;
    telit_helper_sms_encode_t sms = {0};
    int32_t raw_data_size = 0;
    const char message[] = "Test send SMS from wms demo!";
    const char number[] = DESTINATION_NUMBER;

    sms.message_size = strlen(message);
    strcpy(sms.text_msg, message);
    memcpy(sms.dest_addr, (int8_t *)number, strlen(number));
    sms.status_report_requested = true;

    raw_data_size = telit_helper_sms_encode(&sms);
    if (!raw_data_size) {
        RLOGE("could not encode message");
        return -1;
    }

    input.format = QMI_WMS_MESSAGE_FORMAT_GWPP;
    input.raw_data_size = raw_data_size;
    memcpy(input.raw_data, sms.pdu_message, raw_data_size);

    if (tqcm_wms_raw_send_send(ctx,
            &input,
            &out,
            &sdk_error,
            NULL,
            0) != 0) {
        RLOGE("SMS sending failure");
        return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_wms;
    uint16_t sdk_error;
    wms_set_event_t wms_enable_ind = { 0 };
    wms_raw_read_t input_read;
    wms_raw_read_resp_t out_read;
    wms_set_routes_t input = { 0 };
    uint16_t i;

    if (sem_init(&mutex_indication, 0, 0)) {
        RLOGE("%s", strerror(errno));
        ret = EXIT_FAILURE;
        goto end;
    }

    telit_set_log_level(LOG_DEBUG);

    if (argc == 1) {
        dev = default_rmnet_dev;
    } else {
        dev = argv[1];
    }

    ret = tqcm_init(dev, &ctx);
    if (ret != 0) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_init_client_context(QMI_SVC_WMS, &ctx, &ctx_wms, cb);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_wms.client_id);

    /* SMS will be stored in UIM and no payload in indication */
    wms_enable_ind.report_mt_message_available = true;
    wms_enable_ind.report_mt_message = 1;

    ret = tqcm_wms_set_event_report_send(&ctx_wms, &wms_enable_ind, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X, check if retries available", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    /* Setting routes for storing */
    input.routes_size = 5;
    for (i = 0; i < input.routes_size; i++) {
        input.routes[i].message_type = 0x00;
        input.routes[i].message_class = i;
        input.routes[i].receipt_action = QMI_WMS_RECEIPT_ACTION_STORE_AND_NOTIFY;
    }

    ret = tqcm_wms_set_routes_send(&ctx_wms, &input, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    RLOGI("Waiting for SMS");

    if (sem_wait(&mutex_indication)) {
        RLOGE("Failed sem_wait");
        ret = EXIT_FAILURE;
    }

    /* SMS arrived, reading it */
    input_read.storage_type = storage_type;
    input_read.memory_index = storage_index;
    input_read.message_mode = QMI_WMS_MESSAGE_MODE_GW;
    input_read.sms_on_ims = 0x00;

    ret = tqcm_wms_raw_read_send(&ctx_wms, &input_read, &out_read, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X, check if retries available", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    RLOGI("SMS READ from UIM");
    telit_print_buffer(out_read.raw_data, out_read.raw_data_size, LOG_INFO);

    /* Setting routes for transfer only */
    input.routes_size = 5;
    for (i = 0; i < input.routes_size; i++) {
        input.routes[i].message_type = 0x00;
        input.routes[i].message_class = i;
        input.routes[i].receipt_action = QMI_WMS_RECEIPT_ACTION_TRANSFER_ONLY;
    }
    input.transfer_ind_available = true;
    input.transfer_ind = 0x01;

    ret = tqcm_wms_set_routes_send(&ctx_wms, &input, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    RLOGI("Waiting for another SMS");

    if (sem_wait(&mutex_indication)) {
        RLOGE("Failed sem_wait");
        ret = EXIT_FAILURE;
    }

    RLOGI("SEND SMS for STATUS REPORT");

    if (send_test_sms(&ctx_wms) != 0) {
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    RLOGI("Waiting for SMS status report");

    if (sem_wait(&mutex_indication)) {
        RLOGE("Failed sem_wait");
        ret = EXIT_FAILURE;
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_wms) != 0) {
        RLOGE("Failed context disposal");
        ret = EXIT_FAILURE;
    }

    if (tqcm_deinit(&ctx) != 0) {
        RLOGE("Failed tqcm deinitialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    /* Needed for valgrind complain about thread stack memory */
    sleep(1);

end:
    return ret;
}
