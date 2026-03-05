#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <telit_sdk.h>
#include <telit_sdk_nas.h>
#include <tqcm.h>
#include <tqcm_nas.h>
#include <telit_sdk_test_helpers.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;
static sem_t stop_sem;

static void ctrl_c_handler(int sig __unused)
{
    sem_post(&stop_sem);
}

static void print_serving_system(nas_serving_system_info_t *out)
{
    uint8_t i;

    RLOGI("reg_state %u", out->serving_system.reg_state);
    RLOGI("ps_attach_state %u", out->serving_system.ps_attach_state);
    RLOGI("cs_attach_state %u", out->serving_system.cs_attach_state);
    RLOGI("sel_network %u", out->serving_system.sel_network);
    RLOGI("num_radio_interfaces %u", out->serving_system.radio_interface_size);
    for (i = 0; i < out->serving_system.radio_interface_size; i++)
        RLOGI("radio_interface[%d] = %u", i, out->serving_system.radio_interface[i]);

    if (out->current_PLMN_available) {
        RLOGI("mcc %u", out->current_PLMN.mcc);
        RLOGI("mnc %u", out->current_PLMN.mnc);
        RLOGI("net_descr_len %u", out->current_PLMN.net_descr_len);
        RLOGI("network_name %s", out->current_PLMN.network_name);
    }

    if (out->roam_indicator_value_available) {
        RLOGI("roam_indicator_val %u", out->roam_indicator_value);
    }

    for (i = 0; i < out->data_caps_size; i++) {
        RLOGI("data_caps[%d] = %u", i, out->data_caps[i]);
    }

    if (out->cdma_system_available) {
        RLOGI("cdma_system_id %u", out->cdma_system.cdma_system_id);
        RLOGI("cdma_network_id %u", out->cdma_system.cdma_network_id);
    }

    if (out->base_station_available) {
        RLOGI("cdma_base_station_id %u", out->base_station.cdma_base_station_id);
        RLOGI("cdma_base_station_latitude %d", out->base_station.cdma_base_station_latitude);
        RLOGI("cdma_base_station_longitude %d", out->base_station.cdma_base_station_longitude);
    }


    for (i = 0; i < out->roaming_indicators_size; i++) {
        RLOGI("radio_interface[%d] = %u", i, out->roaming_indicators[i].radio_interface);
        RLOGI("roam_indicator[%d] = %u", i, out->roaming_indicators[i].roam_indicator);
    }

    if (out->default_roam_ind_available) {
        RLOGI("default_roam_ind %d", out->default_roam_ind);
    }

    if (out->time_zone_3gpp2_available) {
        RLOGI("leap_seconds %d", out->time_zone_3gpp2.leap_seconds);
        RLOGI("daylight_saving %d", out->time_zone_3gpp2.daylight_saving);
        RLOGI("local_time_offset %d", out->time_zone_3gpp2.local_time_offset);
    }

    if (out->CDMA_p_rev_available) {
        RLOGI("CDMA_p_rev %d", out->CDMA_p_rev);
    }

    if (out->time_zone_3gpp_available) {
        RLOGI("_3gpp_time_zone %d", out->time_zone_3gpp);
    }

    if (out->network_DST_adjustment_3gpp_available) {
        RLOGI("_3gpp_network_DST_adjustment %d", out->network_DST_adjustment_3gpp);
    }

    if (out->location_area_code_available) {
        RLOGI("location_area_code %d", out->location_area_code);
    }

    if (out->cell_id_available) {
        RLOGI("cell_id %d", out->cell_id);
    }

    if (out->conc_svc_info_available) {
        RLOGI("conc_svc_info %d", out->conc_svc_info);
    }

    if (out->PRL_ind_available) {
        RLOGI("PRL_ind %d", out->PRL_ind);
    }

    if (out->DTM_ind_available) {
        RLOGI("DTM_ind %d", out->DTM_ind);
    }


    if (out->detailed_svc_info_available) {
        RLOGI("service_status %d", out->detailed_svc_info.service_status);
        RLOGI("service_caps %d", out->detailed_svc_info.service_caps);
        RLOGI("hdr_srv_status %d", out->detailed_svc_info.hdr_srv_status);
        RLOGI("hdr_hybrid %d", out->detailed_svc_info.hdr_hybrid);
        RLOGI("is_sys_forbidden %d", out->detailed_svc_info.is_sys_forbidden);
    }

    if (out->CDMA_system_info_ext_available) {
        RLOGI("MCC %d", out->CDMA_system_info_ext.MCC);
        RLOGI("imsi_11_12 %d", out->CDMA_system_info_ext.imsi_11_12);
    }

    if (out->hdr_personality_available) {
        RLOGI("hdr_personality %d", out->hdr_personality);
    }

    if (out->track_area_code_available) {
        RLOGI("track_area_code %d", out->track_area_code);
    }

    if (out->call_bar_status_available) {
        RLOGI("cs_bar_status %d", out->call_bar_status.cs_bar_status);
        RLOGI("ps_bar_status %d", out->call_bar_status.ps_bar_status);
    }
}

static void print_sig_info(nas_signal_info_t *out)
{
    if (out->signal_info_gsm_set) {
        RLOGI("GSM rssi %d", out->signal_info_gsm.rssi);
    }
    if (out->signal_info_wcdma_set) {
        RLOGI("WCDMA rssi %d", out->signal_info_wcdma.rssi);
        RLOGI("WCDMA ecio %u", out->signal_info_wcdma.ecio);
    }
    if (out->signal_info_lte_set) {
        RLOGI("LTE rssi %d", out->signal_info_lte.rssi);
        RLOGI("LTE rsrq %d", out->signal_info_lte.rsrq);
        RLOGI("LTE rsrp %d", out->signal_info_lte.rsrp);
        RLOGI("LTE snr %d", out->signal_info_lte.snr);
    }
    if (out->signal_info_nr5g_set) {
        RLOGI("NR5G rsrp %d", out->signal_info_nr5g.rsrp);
        RLOGI("NR5G snr %d", out->signal_info_nr5g.snr);
    }
}

static void print_sys_info(nas_system_info_t *out)
{
    print_nas_sys_info(out);
}

static void cb(void *item)
{
    indication_item_t *ind = (indication_item_t *)item;
    uint8_t indication_buf[2048] = {0};
    int ret;

    ret = telit_nas_indication_handler(ind->msg_id,
            ind->buf, ind->buf_len,
            indication_buf, sizeof(indication_buf));
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("unpack fail %d ", ret);
    } else {
        switch (ind->msg_id) {
            case QMI_NAS_EVENT_REPORT_IND:
                /* Nothing to do, just to avoid spamming the output, since it
                 * is by default enabled */
                break;
            case QMI_NAS_SIG_INFO_IND: {
                nas_signal_info_t *t = (nas_signal_info_t *)indication_buf;

                RLOGI("Received QMI_NAS_SIG_INFO_IND indication");
                print_sig_info(t);
                break;
            }
            case QMI_NAS_SYS_INFO_IND: {
                nas_system_info_t *t = (nas_system_info_t *)indication_buf;

                RLOGI("Received QMI_NAS_SYS_INFO_IND indication");
                print_sys_info(t);
                break;
            }
            case QMI_NAS_GET_SERVING_SYSTEM: {
                nas_serving_system_info_t *t = (nas_serving_system_info_t *)indication_buf;

                RLOGI("Received QMI_NAS_GET_SERVING_SYSTEM indication");
                print_serving_system(t);
                break;
            }
            default:
                RLOGI("Indication 0x%x not managed", ind->msg_id);
                break;
        }
    }

    free(ind->buf);
    free(ind);
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_nas;
    uint16_t sdk_error;
    nas_indication_register_t input;
    struct sigaction act;

    telit_set_log_level(LOG_INFO);
    act.sa_handler = ctrl_c_handler;
    sigaction(SIGINT, &act, NULL);

    if (argc == 1) {
        dev = default_rmnet_dev;
    } else {
        dev = argv[1];
    }

    if (sem_init(&stop_sem, 0, 0)) {
        RLOGE("sem_init failed");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_init(dev, &ctx);
    if (ret != 0) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_init_client_context(QMI_SVC_NAS, &ctx, &ctx_nas, cb);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_nas.client_id);

    input.serving_system = QMI_IND_ACTION_SET;
    input.sig_info = QMI_IND_ACTION_SET;
    input.sys_info = QMI_IND_ACTION_SET;

    ret = tqcm_nas_indication_register_send(&ctx_nas, &input, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    RLOGI("Please trigger some indications or CRTL^C to exit");

    sem_wait(&stop_sem);

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_nas) != 0) {
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
