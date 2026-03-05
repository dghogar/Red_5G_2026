#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>

#include <telit_sdk.h>
#include <telit_sdk_wds.h>
#include <telit_sdk_wda.h>

#include "dc_qrtr.h"
#include "dc_utils.h"
#include "dc_tqcm.h"
#include "dc_netdevice.h"

int add_qmap_interface_shared_rmnet(struct input *input_args, int mux_id)
{
    int ret = 0;
    char cmd_ip[MAX_STRING_SIZE] = {0};

    /* Create qmap network interface */
    snprintf(cmd_ip, MAX_STRING_SIZE, "ip link add link %s type rmnet mux_id %d", input_args->netdevice,
            mux_id);
    ret = system(cmd_ip);
    if (ret == -1) {
        RLOGE("%s: unable to setup qmap network interface with id %d", __FUNCTION__, mux_id);
    } else {
        if (WIFEXITED(ret) && WEXITSTATUS(ret) == EXIT_SUCCESS) {
            ret = 0;
        } else {
            RLOGE("%s: unable to create qmap netdevice, ip return code = %d",
                    __FUNCTION__, WEXITSTATUS(ret));
            ret = -1;
        }
    }

    return ret;
}

#define MHI_NETDEVICE_PATH "/sys/class/net/"

int del_qmap_interface_shared_rmnet(char *netdevice __unused, int mux_id __unused)
{
    /* Currently just removing all available rmnet netdevices */
    int ret = 0;
    DIR *d;
    struct dirent *dir;
    char cmd_ip[PATH_MAX + 1] = {0};

    d = opendir(MHI_NETDEVICE_PATH);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            /* Bail out when the first rmnet device is found */
            if (strstr(dir->d_name, "rmnet")) {
                /* Dispose qmap network interface */
                snprintf(cmd_ip, PATH_MAX, "ip link delete dev %s", dir->d_name);
                ret = system(cmd_ip);
                if (ret == -1) {
                    RLOGE("%s: unable to dispose qmap network interface %s",
                            __FUNCTION__, dir->d_name);
                    goto end;
                } else {
                    if (!(WIFEXITED(ret) && WEXITSTATUS(ret) == EXIT_SUCCESS)) {
                        RLOGE("%s: unable to dispose qmap netdevice, ip return code = %d",
                                __FUNCTION__, WEXITSTATUS(ret));
                        ret = -1;
                    }
                }
            }
        }
    } else {
        RLOGE("%s: unable to open %s",
                __FUNCTION__, MHI_NETDEVICE_PATH);
        ret = -1;
    }

end:
    if (d) {
        closedir(d);
    }

    return ret;
}

#define RMNET_PREFIX "rmnet"

char *get_qmap_prefix_shared_rmnet()
{
    return RMNET_PREFIX;
}

int check_qmap_interface_existence_shared_rmnet(char *netdevice __unused)
{
    int ret = 0;
    DIR *d;
    struct dirent *dir;

    d = opendir(MHI_NETDEVICE_PATH);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            /* Bail out when the first rmnet device is found */
            if (strstr(dir->d_name, "rmnet")) {
                ret = -1;
                break;
            }
        }
    } else {
        RLOGE("%s: unable to open %s",
                __FUNCTION__, MHI_NETDEVICE_PATH);
        ret = -1;
    }

    if (d) {
        closedir(d);
    }

    return ret;
}

int get_network_interface_shared_rmnet(char *device __unused, char **netdevice)
{
    int ret = 0;
    DIR *d;
    struct dirent *dir;

    d = opendir(MHI_NETDEVICE_PATH);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            /* Check if dir is . or .. */
            if (strlen(dir->d_name) <= 2) {
                continue;
            }
            /* Just catch the first mhi netdevice */
            if (strstr(dir->d_name, "mhi_hwip")) {
                *netdevice = strdup(dir->d_name);
                break;
            }
        }
    } else {
        RLOGE("%s: unable to open %s",
                __FUNCTION__, MHI_NETDEVICE_PATH);
        ret = -1;
    }

    if (d) {
        closedir(d);
    }

    if (!netdevice || strlen(*netdevice) == 0) {

        RLOGE("%s: unable to find netdevice", __FUNCTION__);
        ret = -1;
    }

    return ret;
}

int set_mtu_qmi_shared_rmnet(char *netdevice __unused, uint32_t mtu __unused)
{
    /* Not needed since mhi_ip_hw already has MTU properly set */
    return 0;
}

int enable_raw_ip_shared_rmnet(char *netdevice __unused, bool enable __unused)
{
    /* Not needed at the driver level since mhi_ip_hw has only raw_ip */
    return 0;
}

int bind_mux_shared_rmnet(tqcm_client_context_t *ctx_wds, uint8_t mux_id, uint32_t iface_id)
{
    return req_bind_mux(ctx_wds, mux_id, QMI_WDS_DATA_EP_TYPE_PCIE,
            (iface_id == 0xFFFFFFFF) ? 0x04 : iface_id);
}

int set_data_format_shared_rmnet(tqcm_context_t *ctx, uint8_t qmap,
        uint32_t max_dl_aggr_size, uint32_t max_dl_datagram_number,
        uint32_t dl_padding, uint32_t *dl_data_aggregation_current_max_size,
        uint32_t iface_id)
{
    return req_set_data_format(ctx, qmap,
            max_dl_aggr_size, max_dl_datagram_number, dl_padding,
            QMI_WDA_DATA_EP_TYPE_PCIE,
            (iface_id == 0xFFFFFFFF) ? 0x04 : iface_id,
            dl_data_aggregation_current_max_size);
}

