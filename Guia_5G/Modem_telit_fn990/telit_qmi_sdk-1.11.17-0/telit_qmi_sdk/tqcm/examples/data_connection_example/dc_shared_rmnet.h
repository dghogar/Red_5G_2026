#ifndef DC_SHARED_RMNET_H_
#define DC_SHARED_RMNET_H_

#include "telit_data_connection.h"

int add_qmap_interface_shared_rmnet(struct input *input_args, int mux_id);
int del_qmap_interface_shared_rmnet(char *netdevice, int mux_id);
char *get_qmap_prefix_shared_rmnet();
int check_qmap_interface_existence_shared_rmnet(char *netdevice);
int get_network_interface_shared_rmnet(char *device, char **netdevice);
int set_mtu_qmi_shared_rmnet(char *netdevice, uint32_t mtu);
int enable_raw_ip_shared_rmnet(char *netdevice, bool enable);
int bind_mux_shared_rmnet(tqcm_client_context_t *ctx_wds, uint8_t mux_id,
        uint32_t iface_id);
int set_data_format_shared_rmnet(tqcm_context_t *ctx, uint8_t qmap,
        uint32_t max_dl_aggr_size, uint32_t max_dl_datagram_number,
        uint32_t dl_padding, uint32_t *dl_data_aggregation_current_max_size,
        uint32_t iface_id);

#endif  /* DC_SHARED_RMNET_H_ */
