#ifndef TELIT_DATA_CONNECTION_H_
#define TELIT_DATA_CONNECTION_H_

#include <tqcm.h>

#define OUT(err_str) \
        do { \
            RLOGI("Usage: %s %s", argv[0], USAGE); \
            RLOGE(err_str); ret = EXIT_FAILURE; \
            goto fail; \
        } while (0)

#define FAIL(err_str) \
        do { \
            RLOGE(err_str); ret = EXIT_FAILURE; \
            goto fail; \
        } while (0)

#define BAIL_OUT(err_str) \
        do { \
            RLOGE(err_str); ret = EXIT_FAILURE; goto end; \
        } while (0)

#define BAIL_OUT_QMI \
        do { \
            if (ret != 0) { \
                RLOGE("%s: send fail %d", __FUNCTION__, ret); \
                if (ret == EPROTO) { \
                    RLOGE("qmi protocol error 0x%04X", sdk_error); \
                } \
                ret = -1; \
                goto end; \
            } \
        } while (0)

struct input {
    int debug;
    int qmap;
    char *device;
    char *netdevice;
    char *conf;
    uint32_t max_dl_aggr_size;
    uint32_t max_dl_datagram_number;
    uint32_t dl_padding;
    uint32_t iface_id;
    uint16_t mtu;
};

struct wds_data_conn {
    uint8_t qmap_id;
    char netdevice[MAX_STRING_SIZE];
    char address[MAX_STRING_SIZE];
    char gateway[MAX_STRING_SIZE];
    char dns1[MAX_STRING_SIZE];
    char dns2[MAX_STRING_SIZE];
    tqcm_client_context_t ctx_wds;
    uint32_t handle;
    uint8_t profile_id;
};

struct data_connection {
    char name[MAX_STRING_SIZE];
    uint8_t type;
    char apn[MAX_STRING_SIZE];
    uint8_t default_gateway;
    uint8_t dns;
    uint8_t profile;
};

typedef int (*add_qmap_interface_t)(struct input *input_args, int mux_id);
typedef int (*del_qmap_interface_t)(char *netdevice, int mux_id);
typedef char *(*get_qmap_prefix_t)();
typedef int (*check_qmap_interface_existence_t)(char *netdevice);
typedef int (*get_network_interface_t)(char *device, char **netdevice);
typedef int (*set_mtu_t)(char *netdevice, uint32_t mtu);
typedef int (*enable_raw_ip_t)(char *netdevice, bool enable);
typedef int (*bind_mux_t)(tqcm_client_context_t *ctx_wds, uint8_t mux_id,
        uint32_t iface_id);
typedef int (*set_data_format_t)(tqcm_context_t *ctx, uint8_t qmap,
        uint32_t max_dl_aggr_size, uint32_t max_dl_datagram_number,
        uint32_t dl_padding, uint32_t *dl_data_aggregation_current_max_size,
        uint32_t iface_id);
typedef int (*open_hw_data_port_t)(tqcm_context_t *ctx);

struct dc_ops_s {
    add_qmap_interface_t add_qmap_interface;
    del_qmap_interface_t del_qmap_interface;
    get_qmap_prefix_t get_qmap_prefix;
    check_qmap_interface_existence_t check_qmap_interface_existence;
    get_network_interface_t get_network_interface;
    set_mtu_t set_mtu;
    enable_raw_ip_t enable_raw_ip;
    bind_mux_t bind_mux;
    set_data_format_t set_data_format;
    open_hw_data_port_t open_hw_data_port;
};

#endif  /* TELIT_DATA_CONNECTION_H_ */
