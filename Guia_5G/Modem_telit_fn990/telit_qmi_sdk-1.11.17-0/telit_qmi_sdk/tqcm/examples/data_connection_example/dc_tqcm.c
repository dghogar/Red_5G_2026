#include <stddef.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include <telit_sdk.h>
#include <telit_sdk_dms.h>
#include <telit_sdk_wds.h>
#include <telit_sdk_wda.h>
#include <telit_sdk_dpm.h>

#include <tqcm.h>
#include <tqcm_dms.h>
#include <tqcm_wds.h>
#include <tqcm_wda.h>
#include <tqcm_dpm.h>

#include "telit_data_connection.h"
#include "dc_netdevice.h"

int req_get_model_id(tqcm_context_t *ctx, char *model_id, uint8_t len)
{
    int ret = 0;
    uint16_t sdk_error;
    tqcm_client_context_t ctx_dms;
    dms_get_model_id_t out;

    ret = tqcm_init_client_context(QMI_SVC_DMS, ctx, &ctx_dms, NULL);
    if (ret) {
        RLOGE("Failed DMS client context opening");
        return ret;
    }

    ret = tqcm_dms_get_device_model_id_send(&ctx_dms,
            &out,
            &sdk_error,
            NULL,
            0);
    BAIL_OUT_QMI;

    strncpy(model_id, out.model_id, len);

end:
    if (tqcm_deinit_client_context(ctx, &ctx_dms)) {
        RLOGE("Failed dms context disposal");
    }

    return ret;
}

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

    if (out_get->profile_settings.ip_family_available) {
        RLOGI("IP version %u", out_get->profile_settings.ip_family);
    }

    if (out_get->profile_settings.mtu_available) {
        RLOGI("MTU %u", out_get->profile_settings.mtu);
    }
}

int req_get_runtime_settings(struct wds_data_conn *data_conn)
{
    int ret;
    uint16_t sdk_error;
    wds_runtime_settings_info_t input = {0};
    wds_profile_t out;

    input.requested_settings = 0b111111111111111111;

    ret = tqcm_wds_get_runtime_settings_send(&data_conn->ctx_wds, &input, &out, &sdk_error, NULL, 0);
    BAIL_OUT_QMI;

    print_get_runtime_response(&out);

    if (out.profile_settings.ipv4_addr_pref_available) {
        snprintf(data_conn->address, MAX_STRING_SIZE, "%u.%u.%u.%u/%u",
                (out.profile_settings.ipv4_addr_pref >> 24) & 0xFF,
                (out.profile_settings.ipv4_addr_pref >> 16) & 0xFF,
                (out.profile_settings.ipv4_addr_pref >> 8) & 0xFF,
                out.profile_settings.ipv4_addr_pref & 0xFF,
                netdevice_get_ipv4_prefix_from_mask(out.profile_settings.ipv4_subnet_mask_addr));
    }

    if (out.profile_settings.ipv6_addr_available) {
        snprintf(data_conn->address, MAX_STRING_SIZE, "%x:%x:%x:%x:%x:%x:%x:%x/%u",
                ntohs(out.profile_settings.ipv6_addr.addr[0]),
                ntohs(out.profile_settings.ipv6_addr.addr[1]),
                ntohs(out.profile_settings.ipv6_addr.addr[2]),
                ntohs(out.profile_settings.ipv6_addr.addr[3]),
                ntohs(out.profile_settings.ipv6_addr.addr[4]),
                ntohs(out.profile_settings.ipv6_addr.addr[5]),
                ntohs(out.profile_settings.ipv6_addr.addr[6]),
                ntohs(out.profile_settings.ipv6_addr.addr[7]),
                out.profile_settings.ipv6_addr.prefix_len);
    }

    if (!strlen(data_conn->address)) {
        BAIL_OUT("failed retrieving ip address");
    }

    if (out.profile_settings.ipv4_gateway_addr_available) {
        snprintf(data_conn->gateway, MAX_STRING_SIZE, "%u.%u.%u.%u",
                (out.profile_settings.ipv4_gateway_addr >> 24) & 0xFF,
                (out.profile_settings.ipv4_gateway_addr >> 16) & 0xFF,
                (out.profile_settings.ipv4_gateway_addr >> 8) & 0xFF,
                out.profile_settings.ipv4_gateway_addr & 0xFF);
    }

    if (out.profile_settings.ipv6_gw_addr_available) {
        snprintf(data_conn->gateway, MAX_STRING_SIZE, "%x:%x:%x:%x:%x:%x:%x:%x",
                ntohs(out.profile_settings.ipv6_gw_addr.addr[0]),
                ntohs(out.profile_settings.ipv6_gw_addr.addr[1]),
                ntohs(out.profile_settings.ipv6_gw_addr.addr[2]),
                ntohs(out.profile_settings.ipv6_gw_addr.addr[3]),
                ntohs(out.profile_settings.ipv6_gw_addr.addr[4]),
                ntohs(out.profile_settings.ipv6_gw_addr.addr[5]),
                ntohs(out.profile_settings.ipv6_gw_addr.addr[6]),
                ntohs(out.profile_settings.ipv6_gw_addr.addr[7]));
    }

    if (!strlen(data_conn->gateway)) {
        BAIL_OUT("failed retrieving gateway");
    }

    /* DNS retrieval is not mandatory */
    if (out.profile_settings.pri_dns_ipv4_addr_pref_available) {
        snprintf(data_conn->dns1, MAX_STRING_SIZE, "%u.%u.%u.%u",
                (out.profile_settings.pri_dns_ipv4_addr_pref >> 24) & 0xFF,
                (out.profile_settings.pri_dns_ipv4_addr_pref >> 16) & 0xFF,
                (out.profile_settings.pri_dns_ipv4_addr_pref >> 8) & 0xFF,
                out.profile_settings.pri_dns_ipv4_addr_pref & 0xFF);
    }

    if (out.profile_settings.sec_dns_ipv4_addr_pref_available) {
        snprintf(data_conn->dns2, MAX_STRING_SIZE, "%u.%u.%u.%u",
                (out.profile_settings.sec_dns_ipv4_addr_pref >> 24) & 0xFF,
                (out.profile_settings.sec_dns_ipv4_addr_pref >> 16) & 0xFF,
                (out.profile_settings.sec_dns_ipv4_addr_pref >> 8) & 0xFF,
                out.profile_settings.sec_dns_ipv4_addr_pref & 0xFF);
    }

    if (out.profile_settings.pri_dns_ipv6_addr_available) {
        snprintf(data_conn->dns1, MAX_STRING_SIZE, "%x:%x:%x:%x:%x:%x:%x:%x",
                ntohs(out.profile_settings.pri_dns_ipv6_addr[0]),
                ntohs(out.profile_settings.pri_dns_ipv6_addr[1]),
                ntohs(out.profile_settings.pri_dns_ipv6_addr[2]),
                ntohs(out.profile_settings.pri_dns_ipv6_addr[3]),
                ntohs(out.profile_settings.pri_dns_ipv6_addr[4]),
                ntohs(out.profile_settings.pri_dns_ipv6_addr[5]),
                ntohs(out.profile_settings.pri_dns_ipv6_addr[6]),
                ntohs(out.profile_settings.pri_dns_ipv6_addr[7]));
    }

    if (out.profile_settings.sec_dns_ipv6_addr_available) {
        snprintf(data_conn->dns2, MAX_STRING_SIZE, "%x:%x:%x:%x:%x:%x:%x:%x",
                ntohs(out.profile_settings.sec_dns_ipv6_addr[0]),
                ntohs(out.profile_settings.sec_dns_ipv6_addr[1]),
                ntohs(out.profile_settings.sec_dns_ipv6_addr[2]),
                ntohs(out.profile_settings.sec_dns_ipv6_addr[3]),
                ntohs(out.profile_settings.sec_dns_ipv6_addr[4]),
                ntohs(out.profile_settings.sec_dns_ipv6_addr[5]),
                ntohs(out.profile_settings.sec_dns_ipv6_addr[6]),
                ntohs(out.profile_settings.sec_dns_ipv6_addr[7]));
    }

end:
    return ret;
}

int req_bind_mux(tqcm_client_context_t *ctx_wds,
        uint8_t mux_id, uint32_t ep_type, uint32_t iface_id)
{
    int ret;
    uint16_t sdk_error;
    wds_bind_mux_data_port_t input = {0};

    input.peripheral_endpoint_available = true;
    input.peripheral_endpoint.ep_type = ep_type;
    input.peripheral_endpoint.iface_id = iface_id;
    input.mux_id_available = true;
    input.mux_id = mux_id;

    ret = tqcm_wds_bind_mux_data_port_send(ctx_wds, &input, &sdk_error, NULL, 0);
    BAIL_OUT_QMI;

end:
    return ret;
}

int req_set_data_format(tqcm_context_t *ctx, uint8_t qmap,
        uint32_t max_dl_aggr_size, uint32_t max_dl_datagram_number,
        uint32_t dl_padding, uint32_t ep_type, uint32_t iface_id,
        uint32_t *dl_data_aggregation_current_max_size)
{
    int ret;
    uint16_t sdk_error;
    tqcm_client_context_t ctx_wda = {0};
    wda_data_format_t input = {0};
    wda_data_format_t out;

    ret = tqcm_init_client_context(QMI_SVC_WDA, ctx, &ctx_wda, NULL);
    if (ret) {
        RLOGE("Failed WDA client context opening");
        return ret;
    }

    input.ul_data_aggregation_protocol_available = true;
    input.dl_data_aggregation_protocol_available = true;
    if (qmap) {
        input.ul_data_aggregation_protocol = QMI_WDA_DATA_AGG_QMAP;
        input.dl_data_aggregation_protocol = QMI_WDA_DATA_AGG_QMAP;

        if (max_dl_aggr_size) {
            input.dl_data_aggregation_max_size_available = true;
            input.dl_data_aggregation_max_size = max_dl_aggr_size;
        }
        if (max_dl_datagram_number) {
            input.dl_data_aggregation_max_datagrams_available = true;
            input.dl_data_aggregation_max_datagrams = max_dl_datagram_number;
        }
        if (dl_padding) {
            input.dl_minimum_padding_available = true;
            input.dl_minimum_padding = dl_padding;
        }
    } else {
        input.ul_data_aggregation_protocol = QMI_WDA_DATA_AGG_DISABLED;
        input.dl_data_aggregation_protocol = QMI_WDA_DATA_AGG_DISABLED;
    }
    input.link_prot_available = true;
    input.link_prot = QMI_WDA_LINK_PROTOCOL_IP;
    input.peripheral_endpoint_available = true;
    input.peripheral_endpoint.ep_type = ep_type;
    input.peripheral_endpoint.iface_id = iface_id;

    ret = tqcm_wda_set_data_format_send(&ctx_wda,
            &input,
            &out,
            &sdk_error,
            NULL,
            0);
    BAIL_OUT_QMI;

    if (out.dl_data_aggregation_max_size_available) {
        *dl_data_aggregation_current_max_size = out.dl_data_aggregation_max_size;
    } else {
        ret = -1;
        BAIL_OUT_QMI;
    }

end:
    if (tqcm_deinit_client_context(ctx, &ctx_wda)) {
        RLOGE("Failed wda context disposal");
    }

    return ret;
}

int req_set_ip_family(tqcm_client_context_t *ctx_wds, struct data_connection *data_conn_def)
{
    int ret;
    uint16_t sdk_error;
    wds_ip_family_t input = {0};

    /* If a profile is used the type information is already available in the profile definition */
    if (data_conn_def->profile > 0) {
        return QMI_SDK_ERR_NONE;
    }

    input.ip_preference = data_conn_def->type;

    ret = tqcm_wds_set_client_ip_family_pref_send(ctx_wds, &input, &sdk_error, NULL, 0);
    BAIL_OUT_QMI;

end:
    return ret;
}

int req_create_profile(struct data_connection *data_conn_def, struct wds_data_conn *wds_data_conn)
{
    int ret;
    uint16_t sdk_error;
    wds_profile_resp_t out;
    wds_profile_t input = {0};

    input.profile_settings_available = true;
    input.profile_settings.pdp_type_available = true;
    switch (data_conn_def->type) {
        case QMI_IP_FAMILY_V4:
            input.profile_settings.pdp_type = QMI_WDS_PDP_TYPE_IPV4;
            break;
        case QMI_IP_FAMILY_V6:
            input.profile_settings.pdp_type = QMI_WDS_PDP_TYPE_IPV6;
            break;
        default:
            input.profile_settings.pdp_type = QMI_WDS_PDP_TYPE_IPV4V6;
    }
    input.profile_settings.apn_len = strlen(data_conn_def->apn);
    memcpy(input.profile_settings.apn, data_conn_def->apn, input.profile_settings.apn_len);

    ret = tqcm_wds_create_profile_send(&wds_data_conn->ctx_wds, &input, &out, &sdk_error, NULL, 0);
    BAIL_OUT_QMI;

    if (out.profile_id_available) {
        wds_data_conn->profile_id = out.profile_id.index;
    }

end:
    if (out.extended_error_code_available) {
        RLOGE("Profile creation failed with reason %u", out.extended_error_code);
    }

    return ret;
}

int req_delete_profile(struct wds_data_conn *wds_data_conn)
{
    int ret;
    uint16_t sdk_error;
    wds_profile_ext_err_t out;
    wds_settings_profile_id_t input = {
        .type = QMI_WDS_PROFILE_TYPE_3GPP,
        .index = wds_data_conn->profile_id
    };

    ret = tqcm_wds_delete_profile_send(&wds_data_conn->ctx_wds,
            &input, &out, &sdk_error, NULL, 0);
    if (ret < 0) {
        RLOGE("Unable to delete profile %u", wds_data_conn->profile_id);
    }

    return ret;
}

int req_start_network(struct data_connection *data_conn_def, struct wds_data_conn *wds_data_conn)
{
    int ret;
    uint16_t sdk_error;
    wds_start_network_resp_t out;
    wds_start_network_info_t input = {0};

    if (data_conn_def->profile > 0) {
        input.profile_index_3gpp_available = true;
        input.profile_index_3gpp = data_conn_def->profile;
    } else {
        if (wds_data_conn->profile_id > 0) {
            input.profile_index_3gpp_available = true;
            input.profile_index_3gpp = wds_data_conn->profile_id;
        } else {
            input.apn_len = strlen(data_conn_def->apn);
            memcpy(input.apn, data_conn_def->apn, input.apn_len);
            input.ip_family_preference_available = true;
            input.ip_family_preference = data_conn_def->type;
        }
    }

    ret = tqcm_wds_start_network_interface_send(&wds_data_conn->ctx_wds, &input, &out, &sdk_error, NULL, 0);
    BAIL_OUT_QMI;

    if (out.pkt_data_handle_available) {
        wds_data_conn->handle = out.pkt_data_handle;
    }

end:
    if (out.call_end_reason_available) {
        RLOGE("Call end reason %u", out.call_end_reason);
    }

    if (out.verbose_call_end_reason_available) {
        RLOGE("Verbose call end reason type %u", out.verbose_call_end_reason.call_end_reason_type);
        RLOGE("Verbose call end reason %u", out.verbose_call_end_reason.call_end_reason);
    }

    return ret;
}

int req_stop_network_interface(struct wds_data_conn *data_conn)
{
    int ret;
    uint16_t sdk_error;
    wds_stop_network_info_t input = {0};

    input.pkt_data_handle = data_conn->handle;

    ret = tqcm_wds_stop_network_interface_send(&data_conn->ctx_wds, &input, &sdk_error, NULL, 0);
    BAIL_OUT_QMI;

end:
    return ret;
}

int req_set_event_report(tqcm_client_context_t *ctx_wds)
{
    int ret;
    uint16_t sdk_error;
    wds_set_event_report_info_t input = {0};

    input.report_data_call_status_change = QMI_IND_ACTION_SET;

    ret = tqcm_wds_set_event_report_send(ctx_wds, &input, &sdk_error, NULL, 0);
    BAIL_OUT_QMI;

end:
    return ret;
}

int req_open_hw_data_port(tqcm_context_t *ctx)
{
    int ret;
    uint16_t sdk_error;
    tqcm_client_context_t ctx_dpm = {0};
    dpm_open_port_t input = {0};

    ret = tqcm_init_client_context(QMI_SVC_DPM, ctx, &ctx_dpm, NULL);
    if (ret) {
        RLOGE("Failed DPM client context opening");
        return ret;
    }

    input.hardware_data_port_list_size = 1;
    input.hardware_data_port_list[0].peripheral_endpoint.ep_type = 0x03;
    input.hardware_data_port_list[0].peripheral_endpoint.iface_id = 4;
    input.hardware_data_port_list[0].pair_type.consumer_pipe_num = 1;
    input.hardware_data_port_list[0].pair_type.producer_pipe_num = 14;

    ret = tqcm_dpm_open_port_send(&ctx_dpm,
            &input,
            &sdk_error,
            NULL,
            0);
    BAIL_OUT_QMI;

end:
    if (tqcm_deinit_client_context(ctx, &ctx_dpm)) {
        RLOGE("Failed dpm context disposal");
    }

    return ret;
}

