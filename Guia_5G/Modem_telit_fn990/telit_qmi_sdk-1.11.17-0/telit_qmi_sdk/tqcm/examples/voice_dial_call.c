#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_voice.h>
#include <tqcm.h>
#include <tqcm_voice.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

static sem_t mutex_indication;

static void print_call_info(voice_call_t *out)
{
    if (out->call_id_available) {
        RLOGI("call ID %d", out->call_id);
    }
    if (out->end_reason_available) {
        RLOGI("end reason %d", out->end_reason);
    }
}

static void cb(void *item)
{
    int ret;
    indication_item_t *ind = (indication_item_t *)item;

    switch (ind->msg_id) {
        case QMI_VOICE_ALL_CALL_STATUS_IND: {
            voice_all_call_status_ind_t out;
            uint8_t i = 0;

            RLOGI("Received ALL_CALL_STATUS_IND");

            ret = telit_voice_indication_handler(ind->msg_id,
                    ind->buf, ind->buf_len,
                    (uint8_t *)&out, sizeof(out));

            if (ret != QMI_SDK_ERR_NONE) {
                RLOGE("unpack fail %d ", ret);
            }

            if (out.call_info_size > 0) {
                for (i = 0; i < out.call_info_size; i++) {
                    RLOGI("call_id %u", out.call_info[i].call_id);
                    RLOGI("call_state %u", out.call_info[i].call_state);
                    RLOGI("call_type %u", out.call_info[i].call_type);
                    RLOGI("direction %u", out.call_info[i].direction);
                    RLOGI("mode %u", out.call_info[i].mode);
                    RLOGI("is_mpty %u", out.call_info[i].is_mpty);
                    RLOGI("als %u", out.call_info[i].als);
                }
            }
            break;
        }
        case QMI_VOICE_CONFERENCE_INFO_IND: {
            voice_conference_info_ind_t out;

            RLOGI("Received CONFERENCE_INFO_IND");

            ret = telit_voice_indication_handler(ind->msg_id,
                    ind->buf, ind->buf_len,
                    (uint8_t *)&out, sizeof(out));

            if (ret != QMI_SDK_ERR_NONE) {
                RLOGE("unpack fail %d ", ret);
            }

            RLOGI("conference_xml_len %u", out.conference_xml_len);
            RLOGI("conference_xml %s", out.conference_xml);
            RLOGI("sequence %u", out.sequence);

            if (out.total_size_available > 0) {
                RLOGI("total document size %u", out.total_size);
            }

            if (out.call_id_available > 0) {
                RLOGI("call_id %u", out.call_id);
            }
            break;
        }
        case QMI_VOICE_CONFERENCE_JOIN_IND: {
            voice_conference_join_info_t out;

            RLOGI("Received CONFERENCE_JOIN_IND");

            ret = telit_voice_indication_handler(ind->msg_id,
                    ind->buf, ind->buf_len,
                    (uint8_t *)&out, sizeof(out));

            if (ret != QMI_SDK_ERR_NONE) {
                RLOGE("unpack fail %d ", ret);
            }

            RLOGI("call_id %u", out.call_id);
            RLOGI("uri_name %s", out.partecipant_info.uri_name);
            RLOGI("uri_description %s", out.partecipant_info.uri_description);
            break;
        }
        case QMI_VOICE_CONFERENCE_PARTICIPANT_UPDATE_IND: {
            voice_partecipant_info_t out;

            RLOGI("Received PARTICIPANT_UPDATE_IND");

            ret = telit_voice_indication_handler(ind->msg_id,
                    ind->buf, ind->buf_len,
                    (uint8_t *)&out, sizeof(out));

            if (ret != QMI_SDK_ERR_NONE) {
                RLOGE("unpack fail %d ", ret);
            }

            RLOGI("uri_name %s", out.uri_name);
            RLOGI("uri_description %s", out.uri_description);
            break;
        }
        case QMI_VOICE_SUPS_IND: {
            voice_supplementary_service_info_t out;

            RLOGI("Received VOICE_SUPS_IND");

            ret = telit_voice_indication_handler(ind->msg_id,
                    ind->buf, ind->buf_len,
                    (uint8_t *)&out, sizeof(out));

            if (ret != QMI_SDK_ERR_NONE) {
                RLOGE("unpack fail %d ", ret);
            }

            RLOGI("service_type %u", out.service_type);
            RLOGI("is_modified_by_call_control %u", out.is_modified_by_call_control);

            if (out.call_id_available > 0) {
                RLOGI("call_id %u", out.call_id);
            }
            break;
        }
        case QMI_VOICE_SUPS_NOTIFICATION_IND: {
            voice_sups_notification_t out;

            RLOGI("Received SUPS_NOTIFICATION_IND");

            ret = telit_voice_indication_handler(ind->msg_id,
                    ind->buf, ind->buf_len,
                    (uint8_t *)&out, sizeof(out));

            if (ret != QMI_SDK_ERR_NONE) {
                RLOGE("unpack fail %d ", ret);
            }

            RLOGI("call_id %u", out.call_id);
            RLOGI("notification_type %u", out.notification_type);
            break;
        }
        default:
            RLOGI("Indication not managed (%u)", ind->msg_id);
            break;
    }

    free(ind->buf);
    free(ind);

    sem_post(&mutex_indication);
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_voice;
    voice_call_number_t input;
    voice_indications_t ind;
    voice_call_t out;
    uint16_t sdk_error;

    telit_set_log_level(LOG_INFO);

    if (argc == 2) {
        dev = default_rmnet_dev;
        memcpy(input.calling_number, argv[1], QMI_VOICE_MAX_CALL_NUMBER_LEN);
    } else if (argc == 3) {
        dev = argv[1];
        memcpy(input.calling_number, argv[2], QMI_VOICE_MAX_CALL_NUMBER_LEN);
    } else {
        RLOGI("Calling number needed");
        goto end;
    }

    input.calling_number_len = strlen(input.calling_number);

    ret = tqcm_init(dev, &ctx);
    if (ret != 0) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_init_client_context(QMI_SVC_VOICE,
            &ctx,
            &ctx_voice,
            cb);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_voice.client_id);

    ind.reg_dtmf_events = QMI_IND_ACTION_NONE;
    ind.reg_voice_privacy_events = QMI_IND_ACTION_NONE;
    ind.supps_notification_events = QMI_IND_ACTION_NONE;
    ind.call_events = QMI_IND_ACTION_SET;
    ind.handover_events = QMI_IND_ACTION_NONE;
    ind.speech_events = QMI_IND_ACTION_NONE;
    ind.ussd_notification_events = QMI_IND_ACTION_NONE;
    ind.sups_events = QMI_IND_ACTION_NONE;
    ind.modification_events = QMI_IND_ACTION_NONE;
    ind.uus_events = QMI_IND_ACTION_NONE;
    ind.aoc_events = QMI_IND_ACTION_NONE;
    ind.conference_events = QMI_IND_ACTION_SET;
    ind.ext_brst_intl_events = QMI_IND_ACTION_NONE;
    ind.page_miss_events = QMI_IND_ACTION_NONE;
    ind.cc_result_events = QMI_IND_ACTION_NONE;
    ind.conf_participants_events = QMI_IND_ACTION_SET;
    ind.tty_info_events = QMI_IND_ACTION_NONE;
    ind.orig_fail_events = QMI_IND_ACTION_NONE;
    ind.vs_status_events = QMI_IND_ACTION_NONE;
    ind.audio_rat_change_events = QMI_IND_ACTION_NONE;
    ind.additional_call_info_events = QMI_IND_ACTION_SET;
    ind.ecall_status_events = QMI_IND_ACTION_NONE;
    ind.call_reestab_status_events = QMI_IND_ACTION_NONE;
    ind.vice_dialog_event = QMI_IND_ACTION_NONE;

    ret = tqcm_voice_indication_register_send(&ctx_voice,
            &ind,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    RLOGI("Calling number %s", input.calling_number);

    ret = tqcm_voice_dial_call_send(&ctx_voice,
            &input,
            &out,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    print_call_info(&out);

    if (sem_wait(&mutex_indication)) {
        RLOGE("Failed sem_wait");
        ret = EXIT_FAILURE;
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_voice) != 0) {
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
