#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_nas.h>
#include <tqcm.h>
#include <tqcm_nas.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

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

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_nas;
    nas_serving_system_info_t out;
    uint16_t sdk_error;

    telit_set_log_level(LOG_INFO);

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

    ret = tqcm_init_client_context(QMI_SVC_NAS, &ctx, &ctx_nas, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_nas.client_id);

    ret = tqcm_nas_get_serving_system_send(&ctx_nas, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    print_serving_system(&out);

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
