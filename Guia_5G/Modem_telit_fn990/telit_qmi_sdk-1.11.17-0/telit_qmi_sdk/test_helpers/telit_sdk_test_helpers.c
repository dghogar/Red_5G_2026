/* test helpers */

#include "telit_sdk_test_helpers.h"

#include <string.h>
#include <arpa/inet.h>

void print_get_runtime_response(wds_profile_t *out_get)
{
    if (out_get->profile_id_available) {
        RLOGI("pdp_type %u, ", out_get->profile_id.type);
        RLOGI("profile index %u, ", out_get->profile_id.index);
    }

    if (out_get->profile_settings_available) {
        if (out_get->profile_settings.ipv4_addr_pref_available) {
            uint32_t address = out_get->profile_settings.ipv4_addr_pref;

            RLOGI("ipv4 address (%u) %u.%u.%u.%u",
                    address,
                    (address >> 24) & 0xFF,
                    (address >> 16) & 0xFF,
                    (address >> 8) & 0xFF,
                    address & 0xFF);
        }

        if (out_get->profile_settings.ipv4_gateway_addr_available) {
            uint32_t address_gw = out_get->profile_settings.ipv4_gateway_addr;

            RLOGI("GW ipv4 address (%u) %u.%u.%u.%u",
                    address_gw,
                    (address_gw >> 24) & 0xFF,
                    (address_gw >> 16) & 0xFF,
                    (address_gw >> 8) & 0xFF,
                    address_gw & 0xFF);
        }

        if (out_get->profile_settings.ipv4_subnet_mask_available) {
            uint32_t subnet_mask = out_get->profile_settings.ipv4_subnet_mask_addr;

            RLOGI("ipv4 subnet mask (%u) %u.%u.%u.%u",
                    subnet_mask,
                    (subnet_mask >> 24) & 0xFF,
                    (subnet_mask >> 16) & 0xFF,
                    (subnet_mask >> 8) & 0xFF,
                    subnet_mask & 0xFF);
        }

        if (out_get->profile_settings.pri_dns_ipv4_addr_pref_available) {
            uint32_t primary_dns = out_get->profile_settings.pri_dns_ipv4_addr_pref;

            RLOGI("ipv4 primary dns (%u) %u.%u.%u.%u",
                    primary_dns,
                    (primary_dns >> 24) & 0xFF,
                    (primary_dns >> 16) & 0xFF,
                    (primary_dns >> 8) & 0xFF,
                    primary_dns & 0xFF);
        }

        if (out_get->profile_settings.sec_dns_ipv4_addr_pref_available) {
            uint32_t secondary_dns = out_get->profile_settings.sec_dns_ipv4_addr_pref;

            RLOGI("ipv4 secondary dns (%u) %u.%u.%u.%u",
                    secondary_dns,
                    (secondary_dns >> 24) & 0xFF,
                    (secondary_dns >> 16) & 0xFF,
                    (secondary_dns >> 8) & 0xFF,
                    secondary_dns & 0xFF);
        }

        if (out_get->profile_settings.pcscf_ipv4_address_size) {
            for (int i = 0; i < out_get->profile_settings.pcscf_ipv4_address_size; i++) {
                uint32_t pscfc_addr = out_get->profile_settings.pcscf_ipv4_address[i];

                RLOGI("PCSCF address (%u) %u.%u.%u.%u",
                        pscfc_addr,
                        (pscfc_addr >> 24) & 0xFF,
                        (pscfc_addr >> 16) & 0xFF,
                        (pscfc_addr >> 8) & 0xFF,
                        pscfc_addr & 0xFF);
            }
        }

        if (out_get->profile_settings.ipv6_addr_available) {
            RLOGI("ipv6 address: ");
            RLOGI("%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x/%u",
                    ntohs(out_get->profile_settings.ipv6_addr.addr[0]),
                    ntohs(out_get->profile_settings.ipv6_addr.addr[1]),
                    ntohs(out_get->profile_settings.ipv6_addr.addr[2]),
                    ntohs(out_get->profile_settings.ipv6_addr.addr[3]),
                    ntohs(out_get->profile_settings.ipv6_addr.addr[4]),
                    ntohs(out_get->profile_settings.ipv6_addr.addr[5]),
                    ntohs(out_get->profile_settings.ipv6_addr.addr[6]),
                    ntohs(out_get->profile_settings.ipv6_addr.addr[7]),
                    out_get->profile_settings.ipv6_addr.prefix_len);
        }

        if (out_get->profile_settings.ipv6_gw_addr_available) {
            RLOGI("ipv6 gw address: ");
            RLOGI("%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x/%u",
                    ntohs(out_get->profile_settings.ipv6_gw_addr.addr[0]),
                    ntohs(out_get->profile_settings.ipv6_gw_addr.addr[1]),
                    ntohs(out_get->profile_settings.ipv6_gw_addr.addr[2]),
                    ntohs(out_get->profile_settings.ipv6_gw_addr.addr[3]),
                    ntohs(out_get->profile_settings.ipv6_gw_addr.addr[4]),
                    ntohs(out_get->profile_settings.ipv6_gw_addr.addr[5]),
                    ntohs(out_get->profile_settings.ipv6_gw_addr.addr[6]),
                    ntohs(out_get->profile_settings.ipv6_gw_addr.addr[7]),
                    out_get->profile_settings.ipv6_gw_addr.prefix_len);
        }

        if (out_get->profile_settings.pri_dns_ipv6_addr_available) {
            RLOGI("ipv6 primary dns address: ");
            RLOGI("%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
                    ntohs(out_get->profile_settings.pri_dns_ipv6_addr[0]),
                    ntohs(out_get->profile_settings.pri_dns_ipv6_addr[1]),
                    ntohs(out_get->profile_settings.pri_dns_ipv6_addr[2]),
                    ntohs(out_get->profile_settings.pri_dns_ipv6_addr[3]),
                    ntohs(out_get->profile_settings.pri_dns_ipv6_addr[4]),
                    ntohs(out_get->profile_settings.pri_dns_ipv6_addr[5]),
                    ntohs(out_get->profile_settings.pri_dns_ipv6_addr[6]),
                    ntohs(out_get->profile_settings.pri_dns_ipv6_addr[7]));
        }

        if (out_get->profile_settings.sec_dns_ipv6_addr_available) {
            RLOGI("ipv6 secondary dns address: ");
            RLOGI("%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
                    ntohs(out_get->profile_settings.sec_dns_ipv6_addr[0]),
                    ntohs(out_get->profile_settings.sec_dns_ipv6_addr[1]),
                    ntohs(out_get->profile_settings.sec_dns_ipv6_addr[2]),
                    ntohs(out_get->profile_settings.sec_dns_ipv6_addr[3]),
                    ntohs(out_get->profile_settings.sec_dns_ipv6_addr[4]),
                    ntohs(out_get->profile_settings.sec_dns_ipv6_addr[5]),
                    ntohs(out_get->profile_settings.sec_dns_ipv6_addr[6]),
                    ntohs(out_get->profile_settings.sec_dns_ipv6_addr[7]));
        }

        if (out_get->profile_settings.pcscf_ipv6_addr_list_sz) {
            for (int i = 0; i < out_get->profile_settings.pcscf_ipv6_addr_list_sz; i++) {
                RLOGI("pcscf address %d: ", i);
                RLOGI("%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
                        ntohs(out_get->profile_settings.pcscf_ipv6_addr_list[i][0]),
                        ntohs(out_get->profile_settings.pcscf_ipv6_addr_list[i][1]),
                        ntohs(out_get->profile_settings.pcscf_ipv6_addr_list[i][2]),
                        ntohs(out_get->profile_settings.pcscf_ipv6_addr_list[i][3]),
                        ntohs(out_get->profile_settings.pcscf_ipv6_addr_list[i][4]),
                        ntohs(out_get->profile_settings.pcscf_ipv6_addr_list[i][5]),
                        ntohs(out_get->profile_settings.pcscf_ipv6_addr_list[i][6]),
                        ntohs(out_get->profile_settings.pcscf_ipv6_addr_list[i][7]));
            }
        }

        if (out_get->profile_settings.ip_family_available) {
            RLOGI("IP version %u", out_get->profile_settings.ip_family);
        }
    }
}

void print_gms_nas_modem_status(nas_modem_status_t *output)
{
    RLOGI("temperature = %d,", output->temperature);
    RLOGI("modem_mode = %d,", output->modem_mode);
    RLOGI("system_mode = %d,", output->system_mode);
    RLOGI("ims_reg_state = %d,", output->ims_reg_state);
    RLOGI("ps_state = %d,", output->ps_state);

    if (output->lte_info_available) {
        RLOGI("lte_info.band = %d,", output->lte_info.band);
        RLOGI("lte_info.bandwidth = %d,", output->lte_info.bandwidth);
        RLOGI("lte_info.rx_chan = %d,", output->lte_info.rx_chan);
        RLOGI("lte_info.tx_chan = %d,", output->lte_info.tx_chan);
        RLOGI("lte_info.emm_state = %d,", output->lte_info.emm_state);
        RLOGI("lte_info.emm_sub_state = %d,", output->lte_info.emm_sub_state);
        RLOGI("lte_info.emm_conn_state = %d,", output->lte_info.emm_conn_state);
    }

    if (output->nr5g_info_available) {
        RLOGI("nr5g_info.band = %d,", output->nr5g_info.band);
        RLOGI("nr5g_info.bandwidth = %d,", output->nr5g_info.bandwidth);
        RLOGI("nr5g_info.rx_chan = %d,", output->nr5g_info.rx_chan);
        RLOGI("nr5g_info.tx_chan = %d,", output->nr5g_info.tx_chan);
        RLOGI("nr5g_info.rssi = %d,", output->nr5g_info.rssi);
        RLOGI("nr5g_info.rsrp = %d,", output->nr5g_info.rsrp);
        RLOGI("nr5g_info.rsrq = %d,", output->nr5g_info.rsrq);
        RLOGI("nr5g_info.snr = %d,", output->nr5g_info.snr);
        RLOGI("nr5g_info.pci = %d,", output->nr5g_info.pci);
    }
}

void print_nas_generic_nr5g_rrc_utc_time(nas_nr5g_rrc_utc_time_t *out)
{
    if (out->utc_sib9_acquired_available) {
        RLOGI("utc_sib9_acquired: %s", out->utc_sib9_acquired ? "true" : "false");
    }
    if (out->r16_timing_info_available) {
        RLOGI("r16_timing_info: %s", out->r16_timing_info ? "true" : "false");
    }
    if (out->day_light_saving_time_available) {
        RLOGI("day_light_saving_time: %u", out->day_light_saving_time);
    }
    if (out->leap_seconds_available) {
        RLOGI("leap_seconds: %i", out->leap_seconds);
    }
    if (out->local_time_offset_available) {
        RLOGI("local_time_offset: %i", out->local_time_offset);
    }
    if (out->reference_time_info_available) {
        RLOGI("reference_time_info.ref_days: %u", out->reference_time_info.ref_days);
        RLOGI("reference_time_info.ref_seconds: %u", out->reference_time_info.ref_seconds);
        RLOGI("reference_time_info.ref_milliseconds: %u", out->reference_time_info.ref_milliseconds);
        RLOGI("reference_time_info.ref_ten_nano_seconds: %u",
                out->reference_time_info.ref_ten_nano_seconds);
    }
    if (out->uncertainty_available) {
        RLOGI("uncertainty: %u", out->uncertainty);
    }
    if (out->time_info_type_available) {
        RLOGI("time_info_type: %u", out->time_info_type);
    }
    if (out->reference_sfn_available) {
        RLOGI("reference_sfn: %u", out->reference_sfn);
    }
    if (out->abs_time_available) {
        RLOGI("abs_time: %lu", out->abs_time);
    }
    if (out->universal_time_available) {
        RLOGI("universal_time.year: %u", out->universal_time.year);
        RLOGI("universal_time.month: %u", out->universal_time.month);
        RLOGI("universal_time.day: %u", out->universal_time.day);
        RLOGI("universal_time.hour: %u", out->universal_time.hour);
        RLOGI("universal_time.minute: %u", out->universal_time.minute);
        RLOGI("universal_time.millisecond: %u", out->universal_time.millisecond);
        RLOGI("universal_time.day_of_week: %u", out->universal_time.day_of_week);
    }
}

void print_nas_network_time(nas_network_time_t *out)
{
    RLOGI("year %u", out->year);
    RLOGI("month %u", out->month);
    RLOGI("day %u", out->day);
    RLOGI("hour %u", out->hour);
    RLOGI("minute %u", out->minute);
    RLOGI("second %u", out->second);
    RLOGI("day_of_week %u", out->day_of_week);

    if (out->time_zone_available) {
        RLOGI("time_zone %u", out->time_zone);
    }

    if (out->daylt_sav_adj_available) {
        RLOGI("daylt_sav_adj %u", out->daylt_sav_adj);
    }

    if (out->radio_if_available) {
        RLOGI("radio_if %u", out->radio_if);
    }
}

void print_nas_sys_info(nas_system_info_t *out)
{
    if (out->hdr_service_status_info_available) {
        RLOGI("hdr preferred_data_path %u", out->hdr_service_status_info.preferred_data_path);
        RLOGI("hdr service_status %u", out->hdr_service_status_info.service_status);
    }
    if (out->gsm_service_status_info_available) {
        RLOGI("gsm preferred_data_path %u", out->gsm_service_status_info.preferred_data_path);
        RLOGI("gsm service_status %u", out->gsm_service_status_info.service_status);
        RLOGI("gsm true_service_status %u", out->gsm_service_status_info.true_service_status);
    }
    if (out->wcdma_service_status_info_available) {
        RLOGI("wcdma preferred_data_path %u", out->wcdma_service_status_info.preferred_data_path);
        RLOGI("wcdma service_status %u", out->wcdma_service_status_info.service_status);
        RLOGI("wcdma true_service_status %u", out->wcdma_service_status_info.true_service_status);
    }
    if (out->lte_service_status_info_available) {
        RLOGI("lte preferred_data_path %u", out->lte_service_status_info.preferred_data_path);
        RLOGI("lte service_status %u", out->lte_service_status_info.service_status);
        RLOGI("lte true_service_status %u", out->lte_service_status_info.true_service_status);
    }
    if (out->nr5g_service_status_info_available) {
        RLOGI("nr5g preferred_data_path %u", out->nr5g_service_status_info.preferred_data_path);
        RLOGI("nr5g service_status %u", out->nr5g_service_status_info.service_status);
        RLOGI("nr5g true_service_status %u", out->nr5g_service_status_info.true_service_status);
    }
    if (out->HDR_sys_info_available) {
        if (out->HDR_sys_info.srv_domain_available) {
            RLOGI("HDR srv_domain %u", out->HDR_sys_info.srv_domain);
        }

        if (out->HDR_sys_info.srv_capability_available) {
            RLOGI("HDR srv_capability %u", out->HDR_sys_info.srv_capability);
        }

        if (out->HDR_sys_info.roam_status_available) {
            RLOGI("HDR roam_status %u", out->HDR_sys_info.roam_status);
        }

        if (out->HDR_sys_info.is_sys_forbidden_available) {
            RLOGI("HDR is_sys_forbidden %u", out->HDR_sys_info.is_sys_forbidden);
        }

        if (out->HDR_sys_info.is_sys_prl_match_available) {
            RLOGI("HDR is_sys_prl_match %u", out->HDR_sys_info.is_sys_prl_match);
        }

        if (out->HDR_sys_info.hdr_personality_available) {
            RLOGI("HDR hdr_personality %u", out->HDR_sys_info.hdr_personality);
        }

        if (out->HDR_sys_info.hdr_active_prot_available) {
            RLOGI("HDR hdr_active_prot %u", out->HDR_sys_info.hdr_active_prot);
        }

        if (out->HDR_sys_info.is856_sys_id_available) {
            uint8_t i = 0;
            char log[32] = {0};
            for (i = 0; i < SYS_ID_SIZE; i++) {
                char tmp[4] = {0};
                RLOGI(tmp, "%02X ", out->HDR_sys_info.is856_sys_id[i]);
                strcat(log, tmp);
            }
            RLOGI("HDR is856_sys_id %s", log);
        }
    }
    if (out->gsm_sys_info_available) {
        if (out->gsm_sys_info.srv_domain_available) {
            RLOGI("GSM srv_domain %u", out->gsm_sys_info.srv_domain);
        }

        if (out->gsm_sys_info.srv_capability_available) {
            RLOGI("GSM srv_capability %u", out->gsm_sys_info.srv_capability);
        }

        if (out->gsm_sys_info.roam_status_available) {
            RLOGI("GSM roam_status %u", out->gsm_sys_info.roam_status);
        }

        if (out->gsm_sys_info.is_sys_forbidden_available) {
            RLOGI("GSM is_sys_forbidden %u", out->gsm_sys_info.is_sys_forbidden);
        }

        if (out->gsm_sys_info.lac_available) {
            RLOGI("GSM lac %u", out->gsm_sys_info.lac);
        }

        if (out->gsm_sys_info.cell_id_available) {
            RLOGI("GSM cell_id %u", out->gsm_sys_info.cell_id);
        }

        if (out->gsm_sys_info.reg_reject_info_available) {
            RLOGI("GSM reject_srv_domain %u", out->gsm_sys_info.reject_srv_domain);
            RLOGI("GSM rej_cause %u", out->gsm_sys_info.rej_cause);
        }
        if (out->gsm_sys_info.network_id_available) {
            RLOGI("GSM mcc %s", out->gsm_sys_info.mcc);

            if ((uint8_t)out->gsm_sys_info.mnc[2] == 0xff) {
                out->gsm_sys_info.mnc[2] = 0;
            }
            RLOGI("GSM mnc %s", out->gsm_sys_info.mnc);
        }
        if (out->gsm_sys_info.egprs_supp_available) {
            RLOGI("GSM egprs_supp %u", out->gsm_sys_info.egprs_supp);
        }

        if (out->gsm_sys_info.dtm_supp_available) {
            RLOGI("GSM dtm_supp %u", out->gsm_sys_info.dtm_supp);
        }

    }
    if (out->wcdma_sys_info_available) {
        if (out->wcdma_sys_info.srv_domain_available) {
            RLOGI("WCDMA srv_domain %u", out->wcdma_sys_info.srv_domain);
        }

        if (out->wcdma_sys_info.srv_capability_available) {
            RLOGI("WCDMA srv_capability %u", out->wcdma_sys_info.srv_capability);
        }

        if (out->wcdma_sys_info.roam_status_available) {
            RLOGI("WCDMA roam_status %u", out->wcdma_sys_info.roam_status);
        }

        if (out->wcdma_sys_info.is_sys_forbidden_available) {
            RLOGI("WCDMA is_sys_forbidden %u", out->wcdma_sys_info.is_sys_forbidden);
        }

        if (out->wcdma_sys_info.lac_available) {
            RLOGI("WCDMA lac %u", out->wcdma_sys_info.lac);
        }

        if (out->wcdma_sys_info.cell_id_available) {
            RLOGI("WCDMA cell_id %u", out->wcdma_sys_info.cell_id);
        }

        if (out->wcdma_sys_info.reg_reject_info_available) {
            RLOGI("WCDMA reject_srv_domain %u", out->wcdma_sys_info.reject_srv_domain);
            RLOGI("WCDMA rej_cause %u", out->wcdma_sys_info.rej_cause);
        }
        if (out->wcdma_sys_info.network_id_available) {
            RLOGI("WCDMA mcc %s", out->wcdma_sys_info.mcc);

            if ((uint8_t)out->wcdma_sys_info.mnc[2] == 0xff) {
                out->wcdma_sys_info.mnc[2] = 0;
            }
            RLOGI("WCDMA mnc %s", out->wcdma_sys_info.mnc);
        }
        if (out->wcdma_sys_info.hs_call_status_available) {
            RLOGI("WCDMA hs_call_status %u", out->wcdma_sys_info.hs_call_status);
        }

        if (out->wcdma_sys_info.hs_ind_available) {
            RLOGI("WCDMA hs_ind %u", out->wcdma_sys_info.hs_ind);
        }

        if (out->wcdma_sys_info.psc_available) {
            RLOGI("WCDMA psc %u", out->wcdma_sys_info.psc);
        }
    }
    if (out->lte_sys_info_available) {
        if (out->lte_sys_info.srv_domain_available) {
            RLOGI("LTE srv_domain %u", out->lte_sys_info.srv_domain);
        }

        if (out->lte_sys_info.srv_capability_available) {
            RLOGI("LTE srv_capability %u", out->lte_sys_info.srv_capability);
        }

        if (out->lte_sys_info.roam_status_available) {
            RLOGI("LTE roam_status %u", out->lte_sys_info.roam_status);
        }

        if (out->lte_sys_info.is_sys_forbidden_available) {
            RLOGI("LTE is_sys_forbidden %u", out->lte_sys_info.is_sys_forbidden);
        }

        if (out->lte_sys_info.lac_available) {
            RLOGI("LTE lac %u", out->lte_sys_info.lac);
        }

        if (out->lte_sys_info.cell_id_available) {
            RLOGI("LTE cell_id %u", out->lte_sys_info.cell_id);
        }

        if (out->lte_sys_info.reg_reject_info_available) {
            RLOGI("LTE reject_srv_domain %u", out->lte_sys_info.reject_srv_domain);
            RLOGI("LTE rej_cause %u", out->lte_sys_info.rej_cause);
        }
        if (out->lte_sys_info.network_id_available) {
            RLOGI("LTE mcc %s", out->lte_sys_info.mcc);

            if ((uint8_t)out->lte_sys_info.mnc[2] == 0xff) {
                out->lte_sys_info.mnc[2] = 0;
            }
            RLOGI("LTE mnc %s", out->lte_sys_info.mnc);
        }
        if (out->lte_sys_info.tac_available) {
            RLOGI("LTE tac %u", out->lte_sys_info.tac);
        }
    }
    if (out->nr5g_sys_info_available) {
        if (out->nr5g_sys_info.srv_domain_available) {
            RLOGI("nr5g srv_domain %u", out->nr5g_sys_info.srv_domain);
        }

        if (out->nr5g_sys_info.srv_capability_available) {
            RLOGI("nr5g srv_capability %u", out->nr5g_sys_info.srv_capability);
        }

        if (out->nr5g_sys_info.roam_status_available) {
            RLOGI("nr5g roam_status %u", out->nr5g_sys_info.roam_status);
        }

        if (out->nr5g_sys_info.is_sys_forbidden_available) {
            RLOGI("nr5g is_sys_forbidden %u", out->nr5g_sys_info.is_sys_forbidden);
        }

        if (out->nr5g_sys_info.lac_available) {
            RLOGI("nr5g lac %u", out->nr5g_sys_info.lac);
        }

        if (out->nr5g_sys_info.cell_id_available) {
            RLOGI("nr5g cell_id %u", out->nr5g_sys_info.cell_id);
        }

        if (out->nr5g_sys_info.reg_reject_info_available) {
            RLOGI("nr5g reject_srv_domain %u", out->nr5g_sys_info.reject_srv_domain);
            RLOGI("nr5g rej_cause %u", out->nr5g_sys_info.rej_cause);
        }
        if (out->nr5g_sys_info.network_id_available) {
            RLOGI("nr5g mcc %s", out->nr5g_sys_info.mcc);

            if ((uint8_t)out->nr5g_sys_info.mnc[2] == 0xff) {
                out->nr5g_sys_info.mnc[2] = 0;
            }
            RLOGI("nr5g mnc %s", out->nr5g_sys_info.mnc);
        }
        if (out->nr5g_sys_info.tac_available) {
            RLOGI("nr5g tac %u", out->nr5g_sys_info.tac);
        }
    }
    if (out->nr5g_cell_status_available) {
        RLOGI("nr5g_cell_status %u", out->nr5g_cell_status);
    }

    if (out->nr5g_tac_available) {
        RLOGI("nr5g_tac[0] %u", out->nr5g_tac[0]);
        RLOGI("nr5g_tac[1] %u", out->nr5g_tac[1]);
        RLOGI("nr5g_tac[2] %u", out->nr5g_tac[2]);
    }
    if (out->nr5g_pci_available) {
        RLOGI("nr5g_pci %u", out->nr5g_pci);
    }

    if (out->nr5g_cell_id_available) {
        RLOGI("nr5g_cell_id %u", out->nr5g_cell_id);
    }

    if (out->nr5g_arfcn_available) {
        RLOGI("nr5g_arfcn %u", out->nr5g_arfcn);
    }

    if (out->add_hdr_sys_info_available) {
        RLOGI("add_hdr_sys_info %u", out->add_hdr_sys_info);
    }

    if (out->add_gsm_sys_info_available) {
        RLOGI("add_gsm_sys_info cell_broadcast_cap %u", out->add_gsm_sys_info.cell_broadcast_cap);
        RLOGI("add_gsm_sys_info geo_sys_idx %u", out->add_gsm_sys_info.geo_sys_idx);
    }
    if (out->add_wcdma_sys_info_available) {
        RLOGI("add_wcdma_sys_info cell_broadcast_cap %u", out->add_wcdma_sys_info.cell_broadcast_cap);
        RLOGI("add_wcdma_sys_info geo_sys_idx %u", out->add_wcdma_sys_info.geo_sys_idx);
    }
    if (out->lte_geo_sys_idx_available) {
        RLOGI("add_lte_sys_info %u", out->lte_geo_sys_idx);
    }

    if (out->gsm_call_barring_sys_info_available) {
        RLOGI("gsm_call_barring_sys_info cs_bar_status %u", out->gsm_call_barring_sys_info.cs_bar_status);
        RLOGI("gsm_call_barring_sys_info ps_bar_status %u", out->gsm_call_barring_sys_info.ps_bar_status);
    }
    if (out->wcdma_call_barring_sys_info_available) {
        RLOGI("wcdma_call_barring_sys_info_available cs_bar_status %u",
                out->wcdma_call_barring_sys_info.cs_bar_status);
        RLOGI("wcdma_call_barring_sys_info_available ps_bar_status %u",
                out->wcdma_call_barring_sys_info.ps_bar_status);
    }
    RLOGI("lte_voice_support_sys_info %u", out->lte_voice_support_sys_info);
    if (out->gsm_cipher_domain_sys_info_available) {
        RLOGI("gsm_cipher_domain_sys_info %u", out->gsm_cipher_domain_sys_info);
    }

    if (out->wcdma_cipher_domain_sys_info_available) {
        RLOGI("wcdma_cipher_domain_sys_info %u", out->wcdma_cipher_domain_sys_info);
    }

    if (out->endc_availability_available) {
        RLOGI("endc_availability %u", out->endc_availability);
    }
    if (out->restrict_dcnr_available) {
        RLOGI("restrict_dcnr %u", out->restrict_dcnr);
    }
    if (out->plmn_infolist_r15_availability_available) {
        RLOGI("plmn_infolist_r15_availability %u", out->plmn_infolist_r15_availability);
    }

    if (out->sys_info_plmn_available) {
        RLOGI("radio_if %u", out->sys_info_plmn.radio_if);
        RLOGI("mcc %s", out->sys_info_plmn.mcc);
        if ((uint8_t)out->sys_info_plmn.mnc[2] == 0xFF) {
            out->sys_info_plmn.mnc[2] = '\0';
        }
        RLOGI("mnc %s", out->sys_info_plmn.mnc);
    }
}

static uint8_t hex_char_to_uint(char c)
{
    if (c >= '0' && c <= '9')
        return (uint8_t)(c - '0');
    if (c >= 'A' && c <= 'F')
        return (uint8_t)(c - 'A' + 10);
    if (c >= 'a' && c <= 'f')
        return (uint8_t)(c - 'a' + 10);

    return 0;
}

int ascii_to_hex(char *ascii, int ascii_len, uint8_t *hex)
{
    int i;

    for (i = 0; i < ascii_len; i += 2) {
        hex[i / 2] = ((hex_char_to_uint(ascii[i]) << 4) | hex_char_to_uint(ascii[i + 1]));
    }
    telit_print_buffer(hex, i / 2, LOG_DEBUG);

    return i / 2;
}
