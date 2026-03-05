#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <netinet/in.h>

#include <telit_sdk.h>
#include <telit_sdk_wds.h>
#include <tqcm.h>
#include <tqcm_wds.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

static void print_get_runtime_response(wds_profile_t *out_get)
{
    if (out_get->profile_id_available) {
        RLOGI("pdp_type %u, ", out_get->profile_id.type);
        RLOGI("profile index %u, ", out_get->profile_id.index);
    }

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
        RLOGI("%x:%x:%x:%x:%x:%x:%x:%x/%u",
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
        RLOGI("%x:%x:%x:%x:%x:%x:%x:%x/%u",
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
        RLOGI("%x:%x:%x:%x:%x:%x:%x:%x",
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
        RLOGI("%x:%x:%x:%x:%x:%x:%x:%x",
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
            RLOGI("%x:%x:%x:%x:%x:%x:%x:%x",
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

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_wds;
    uint16_t sdk_error;
    wds_start_network_resp_t out = { 0 };
    wds_start_network_info_t input = { 0 };
    wds_runtime_settings_info_t input_get = { 0 };
    wds_profile_t out_get = { 0 };
    uint8_t index;

    telit_set_log_level(LOG_INFO);

    if (argc < 2) {
        RLOGE("Profile index should be provided");
        ret = EXIT_FAILURE;
        goto end;
    } else if (argc == 2) {
        dev = default_rmnet_dev;
        index = (uint8_t)atoi(argv[1]);
    } else {
        dev = argv[1];
        index = (uint8_t)atoi(argv[2]);
    }

    ret = tqcm_init(dev, &ctx);
    if (ret != 0) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_init_client_context(QMI_SVC_WDS, &ctx, &ctx_wds, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_wds.client_id);

    input.profile_index_3gpp_available = true;
    input.profile_index_3gpp = index;

    RLOGI("Starting network with profile %u", index);

    ret = tqcm_wds_start_network_interface_send(&ctx_wds, &input, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }

        if (out.call_end_reason_available) {
            RLOGI("Call end reason %u", out.call_end_reason);
        }

        if (out.verbose_call_end_reason_available) {
            RLOGI("Verbose call end reason type %u", out.verbose_call_end_reason.call_end_reason_type);
            RLOGI("Verbose call end reason %u", out.verbose_call_end_reason.call_end_reason);
        }

        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    sleep(1);

    RLOGI("Network successfully started, getting runtime settings");

    input_get.requested_settings = 0b111111111111111111;

    ret = tqcm_wds_get_runtime_settings_send(&ctx_wds, &input_get, &out_get, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    print_get_runtime_response(&out_get);

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_wds) != 0) {
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
