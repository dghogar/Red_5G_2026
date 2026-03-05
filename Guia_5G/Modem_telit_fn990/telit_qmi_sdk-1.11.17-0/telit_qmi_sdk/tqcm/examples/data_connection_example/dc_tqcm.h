#ifndef DC_TQCM_H_
#define DC_TQCM_H_

#include "telit_data_connection.h"

int req_get_model_id(tqcm_context_t *ctx, char *model_id, uint8_t len);
int req_get_runtime_settings(struct wds_data_conn *data_conn);
int req_bind_mux(tqcm_client_context_t *ctx_wds,
        uint8_t mux_id, uint32_t ep_type, uint32_t iface_id);
int req_set_data_format(tqcm_context_t *ctx, uint8_t qmap,
        uint32_t max_dl_aggr_size, uint32_t max_dl_datagram_number,
        uint32_t dl_padding, uint32_t ep_type, uint32_t iface_id,
        uint32_t *dl_data_aggregation_current_max_size);
int req_set_ip_family(tqcm_client_context_t *ctx_wds, struct data_connection *data_conn_def);
int req_create_profile(struct data_connection *data_conn_def, struct wds_data_conn *wds_data_conn);
int req_delete_profile(struct wds_data_conn *wds_data_conn);
int req_start_network(struct data_connection *data_conn_def, struct wds_data_conn *wds_data_conn);
int req_stop_network_interface(struct wds_data_conn *data_conn);
int req_set_event_report(tqcm_client_context_t *ctx_wds);
int req_open_hw_data_port(tqcm_context_t *ctx);

#endif  /* DC_TQCM_H_ */
