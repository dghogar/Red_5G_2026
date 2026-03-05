#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>

#include <telit_sdk.h>
#include <telit_sdk_wds.h>
#include <telit_sdk_wda.h>

#include "dc_qmi_wwan.h"
#include "dc_utils.h"
#include "dc_tqcm.h"
#include "dc_netdevice.h"

static int add_qmap_interface_qmi_wwan(struct input *input_args, int mux_id)
{
    int ret = 0;
    char mux_id_str[MAX_STRING_SIZE] = {0};
    char sysfs_netdevice[MAX_STRING_SIZE] = {0};

    snprintf(sysfs_netdevice, MAX_STRING_SIZE, "/sys/class/net/%s/qmi/add_mux", input_args->netdevice);
    /* Create qmap network interface */
    snprintf(mux_id_str, MAX_STRING_SIZE, "%d", mux_id);
    if (write_sysfs_file(sysfs_netdevice, mux_id_str, strlen(mux_id_str))) {
        ret = -1;
        RLOGE("%s: unable to setup qmap network interface with id %d",
                __FUNCTION__, mux_id);
    }

    return ret;
}

static int del_qmap_interface_qmi_wwan(char *netdevice, int mux_id)
{
    int ret = 0;
    char mux_id_str[MAX_STRING_SIZE] = {0};
    char sysfs_netdevice[MAX_STRING_SIZE] = {0};

    snprintf(sysfs_netdevice, MAX_STRING_SIZE, "/sys/class/net/%s/qmi/del_mux", netdevice);
    /* Remove qmap network interface */
    snprintf(mux_id_str, MAX_STRING_SIZE, "%d", mux_id);
    if (write_sysfs_file(sysfs_netdevice, mux_id_str, strlen(mux_id_str))) {
        ret = -1;
        RLOGE("%s: unable to remove qmap network interface with id %d",
                __FUNCTION__, mux_id);
    }

    return ret;
}

#define QMI_WWAN_PREFIX "qmimux"

static char *get_qmap_prefix_qmi_wwan()
{
    return QMI_WWAN_PREFIX;
}

static int check_qmap_interface_existence_qmi_wwan(char *netdevice)
{
    char sysfs_netdevice[MAX_STRING_SIZE] = {0};
    char sysfs_out[MAX_STRING_SIZE] = {0};

    snprintf(sysfs_netdevice, MAX_STRING_SIZE, "/sys/class/net/%s/qmi/add_mux",
            netdevice);
    if (read_sysfs_file(sysfs_netdevice, sysfs_out, MAX_STRING_SIZE)) {
        RLOGE("%s: unable to read add_mux sysfs file");
    }

    return strlen(sysfs_out);
}

#define QMI_WWAN_NETDEVICE_PATH_PREFIX "/sys/class/usbmisc/"
#define QMI_WWAN_NETDEVICE_PATH_SUFFIX "/device/net/"

static int get_network_interface_qmi_wwan(char *device, char **netdevice)
{
    int ret = 0;
    char path[PATH_MAX + 1] = {0};

    if (realpath(device, path)) {
        char *device_wo_path;
        char netdevice_dir_path[PATH_MAX + 1] = {0};
        DIR *d;
        struct dirent *dir;

        device_wo_path = strrchr(path, '/');
        if (device_wo_path == NULL) {
            RLOGE("%s: failed to resolve device name for %s",
                    __FUNCTION__, device);
            ret = -1;
            goto end;
        }

        if (strlen(QMI_WWAN_NETDEVICE_PATH_PREFIX) +
                strlen(device_wo_path) +
                strlen(QMI_WWAN_NETDEVICE_PATH_SUFFIX) > PATH_MAX) {
            RLOGE("%s: netdevice path too long", __FUNCTION__);
            ret = -1;
            goto end;
        }

        strcat(netdevice_dir_path, QMI_WWAN_NETDEVICE_PATH_PREFIX);
        strcat(netdevice_dir_path, device_wo_path);
        strcat(netdevice_dir_path, QMI_WWAN_NETDEVICE_PATH_SUFFIX);

        d = opendir(netdevice_dir_path);
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                /* Check if dir is . or .. */
                if (strlen(dir->d_name) <= 2) {
                    continue;
                }
                /* There should be just one netdevice */
                *netdevice = strdup(dir->d_name);
                break;
            }
        } else {
            RLOGE("%s: unable to open %s",
                    __FUNCTION__, netdevice_dir_path);
            ret = -1;
            goto end;
        }

        if (d) {
            closedir(d);
        }

        if (!netdevice || strlen(*netdevice) == 0) {

            RLOGE("%s: unable to find netdevice", __FUNCTION__);
            ret = -1;
        }
    } else {
        RLOGE("%s: unable to resolve real path", __FUNCTION__);
        ret = -1;
    }

end:
    return ret;
}

static int set_mtu_qmi_wwan(char *netdevice, uint32_t mtu)
{
    return netdevice_set_mtu(netdevice, mtu);
}

static int enable_raw_ip_qmi_wwan(char *netdevice, bool enable)
{
    int ret = 0;
    char raw_ip_str[MAX_STRING_SIZE] = {0};
    char sysfs_netdevice[MAX_STRING_SIZE] = {0};

    snprintf(sysfs_netdevice, MAX_STRING_SIZE, "/sys/class/net/%s/qmi/raw_ip", netdevice);
    snprintf(raw_ip_str, MAX_STRING_SIZE, "%c", enable ? 'Y' : 'N');
    if (write_sysfs_file(sysfs_netdevice, raw_ip_str, strlen(raw_ip_str))) {
        ret = -1;
        RLOGE("%s: unable to set raw_ip to %s", __FUNCTION__, raw_ip_str);
    }

    return ret;
}

static int bind_mux_qmi_wwan(tqcm_client_context_t *ctx_wds, uint8_t mux_id, uint32_t iface_id)
{
    return req_bind_mux(ctx_wds, mux_id, QMI_WDS_DATA_EP_TYPE_HSUSB,
            (iface_id == 0xFFFFFFFF) ? 0x02 : iface_id);
}

static int set_data_format_qmi_wwan(tqcm_context_t *ctx, uint8_t qmap,
        uint32_t max_dl_aggr_size, uint32_t max_dl_datagram_number,
        uint32_t dl_padding, uint32_t *dl_data_aggregation_current_max_size,
        uint32_t iface_id)
{
    return req_set_data_format(ctx, qmap,
            max_dl_aggr_size, max_dl_datagram_number, dl_padding,
            QMI_WDA_DATA_EP_TYPE_HSUSB,
            (iface_id == 0xFFFFFFFF) ? 0x02 : iface_id,
            dl_data_aggregation_current_max_size);
}

static int open_hw_data_port_qmi_wwan(tqcm_context_t *ctx __unused)
{
    /* No need to configure data port with DPM for qmi_wwan */
    return 0;
}

void create_dc_ops_qmi_wwan(struct dc_ops_s *dc_ops)
{
    dc_ops->add_qmap_interface = add_qmap_interface_qmi_wwan;
    dc_ops->del_qmap_interface = del_qmap_interface_qmi_wwan;
    dc_ops->get_qmap_prefix = get_qmap_prefix_qmi_wwan;
    dc_ops->check_qmap_interface_existence = check_qmap_interface_existence_qmi_wwan;
    dc_ops->get_network_interface = get_network_interface_qmi_wwan;
    dc_ops->set_mtu = set_mtu_qmi_wwan;
    dc_ops->enable_raw_ip = enable_raw_ip_qmi_wwan;
    dc_ops->bind_mux = bind_mux_qmi_wwan;
    dc_ops->set_data_format = set_data_format_qmi_wwan;
    dc_ops->open_hw_data_port = open_hw_data_port_qmi_wwan;
}
