#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>

#include <telit_sdk.h>
#include <telit_sdk_wds.h>
#include <telit_sdk_wda.h>

#include "dc_wwanqmi.h"
#include "dc_shared_rmnet.h"
#include "dc_utils.h"
#include "dc_tqcm.h"
#include "dc_netdevice.h"

static int open_hw_data_port_wwanqmi(tqcm_context_t *ctx __unused)
{
    /* Not needed since it is managed by the modem */
    return 0;
}

void create_dc_ops_wwanqmi(struct dc_ops_s *dc_ops)
{
    dc_ops->add_qmap_interface = add_qmap_interface_shared_rmnet;
    dc_ops->del_qmap_interface = del_qmap_interface_shared_rmnet;
    dc_ops->get_qmap_prefix = get_qmap_prefix_shared_rmnet;
    dc_ops->check_qmap_interface_existence = check_qmap_interface_existence_shared_rmnet;
    dc_ops->get_network_interface = get_network_interface_shared_rmnet;
    dc_ops->set_mtu = set_mtu_qmi_shared_rmnet;
    dc_ops->enable_raw_ip = enable_raw_ip_shared_rmnet;
    dc_ops->bind_mux = bind_mux_shared_rmnet;
    dc_ops->set_data_format = set_data_format_shared_rmnet;
    dc_ops->open_hw_data_port = open_hw_data_port_wwanqmi;
}

