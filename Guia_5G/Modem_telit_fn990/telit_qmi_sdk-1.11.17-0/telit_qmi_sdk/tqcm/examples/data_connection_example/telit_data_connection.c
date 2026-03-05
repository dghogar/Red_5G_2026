#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <limits.h>

#include <netinet/in.h>

#include <telit_sdk.h>
#include <telit_sdk_wds.h>
#include <tqcm.h>
#include <tqcm_wds.h>

#include "telit_data_connection.h"
#include "dc_utils.h"
#include "dc_qmi_wwan.h"
#include "dc_qrtr.h"
#include "dc_wwanqmi.h"
#include "dc_netdevice.h"
#include "dc_tqcm.h"

#define USAGE "[OPTIONS] where options are:\n\n" \
        "MANDATORY:\n" \
        "  -c, --conf <configuration file>  Data connection definitions configuration\n" \
        "                                   file.\n" \
        "OPTIONAL:\n" \
        "  -n, --netdevice <net device>  Network interface name.\n" \
        "  -d, --device <device path>    QMI enabled device (default /dev/cdc-wdm0, QRTR\n" \
        "                                for Qualcomm ipcr).\n" \
        "  --qmap                        For using qmap.\n" \
        "  -s, --max-dl-aggr-size        When using qmap to suggest the maximum downlink\n" \
        "                                aggregated size.\n" \
        "  -m, --max-dl-datagram-number  When using qmap to suggest the maximum number of\n" \
        "                                downling datagrams.\n" \
        "  -p, --dl-padding              When using qmap to set the minimum downlink\n" \
        "                                padding between aggregated packets.\n" \
        "  -i, --interface-id            Peripheral interface number \n" \
        "                                (default 0x2 for USB, 0x4 for PCIe).\n" \
        "  -u, --mtu                     When not using qmap to set the mtu of the master\n" \
        "                                network interface.\n" \
        "  --debug                       For enabling debug output.\n"

static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;
uint32_t quirks = 0;

struct dc_ops_s dc_ops = {0};

#define QUIRK_NEED_PROFILE 1

struct input input_args = {0};

#define DATA_CONN_ARR_SZ 8
struct data_connection data_connection_arr[DATA_CONN_ARR_SZ];
struct wds_data_conn wds_data_conn_arr[DATA_CONN_ARR_SZ];

static struct option long_options[] =
{
    /* These options set a flag. */
    {"debug", no_argument, &input_args.debug, 1},
    {"qmap", no_argument, &input_args.qmap, 1},
    /* These options don’t set a flag */
    {"conf", required_argument, NULL, 'c'},
    {"device", required_argument, NULL, 'd'},
    {"netdevice", required_argument, NULL, 'n'},
    {"max-dl-aggr-size", required_argument, NULL, 's'},
    {"max-dl-datagram-number", required_argument, NULL, 'm'},
    {"dl-padding", required_argument, NULL, 'p'},
    {"interface-id", required_argument, NULL, 'i'},
    {"mtu", required_argument, NULL, 'u'},
    {0, 0, 0, 0}
};

static sem_t data_connection_sem;
static int data_stop = 0;

static int identify_device(char *device)
{
    int ret = 0;
    char path[PATH_MAX + 1] = {0};

    if (strstr(device, "QRTR")) {
        /* QRTR is not a real device, so skip realpath */
        create_dc_ops_qrtr(&dc_ops);
        goto end;
    }

    if (realpath(device, path)) {
        if (strstr(path, "cdc-wdm")) {
            create_dc_ops_qmi_wwan(&dc_ops);
        } else if (strstr(path, "wwan")) {
            create_dc_ops_wwanqmi(&dc_ops);
        } else if (strstr(path, "mhi")) {
            /* out-of-tree mhi_uci driver, at high-level basically the same than wwan */
            create_dc_ops_wwanqmi(&dc_ops);
        } else {
            RLOGE("%s: device type %s unmanaged", __FUNCTION__, device);
            ret = -1;
        }
    } else {
        RLOGE("%s: unable to resolve real path", __FUNCTION__);
        ret = -1;
    }

end:
    return ret;
}

static void ctrl_c_handler(int sig __unused)
{
    data_stop = 1;

    sem_post(&data_connection_sem);
}

#define CHECK_I_NOT_0 \
        do { \
            if (i == 0) { \
                RLOGE("name must be the first field"); \
                goto end; \
            } \
        } while (0)

static int parse_configuration_file(char *f)
{
    FILE *stream;
    char *line = NULL;
    char *token;
    size_t len = 0;
    ssize_t nread;
    uint8_t i = 0;

    stream = fopen(f, "r");
    if (stream == NULL) {
        RLOGE("%s: unable to open file %s", __FUNCTION__, f);
        return -1;
    }

    memset(data_connection_arr, 0, sizeof(data_connection_arr));

    while ((nread = getline(&line, &len, stream)) != -1) {
        /* Skip comments starting with # */
        if (line[0] == '#') {
            continue;
        }
        RLOGD("%s", line);

        token = strtok(line, ":");
        if (token != NULL) {
            if (!strcmp(token, "name")) {
                token = strtok(NULL, "\n");
                if (token != NULL) {
                    i++;
                    snprintf(data_connection_arr[i - 1].name, MAX_STRING_SIZE, "%s", token);
                }
            } else if (!strcmp(token, "type")) {
                CHECK_I_NOT_0;
                token = strtok(NULL, "\n");
                if (token != NULL) {
                    if (token[0] == '4') {
                        data_connection_arr[i - 1].type = QMI_IP_FAMILY_V4;
                    } else if (token[0] == '6') {
                        data_connection_arr[i - 1].type = QMI_IP_FAMILY_V6;
                    } else {
                        data_connection_arr[i - 1].type = QMI_IP_FAMILY_UNSPECIFIED;
                    }
                }
            } else if (!strcmp(token, "apn")) {
                CHECK_I_NOT_0;
                token = strtok(NULL, "\n");
                if (token != NULL) {
                    snprintf(data_connection_arr[i - 1].apn, MAX_STRING_SIZE, "%s", token);
                }
            } else if (!strcmp(token, "default-gateway")) {
                CHECK_I_NOT_0;
                token = strtok(NULL, "\n");
                if (token != NULL) {
                    if (token[0] == 'N' || token[0] == 'n') {
                        data_connection_arr[i - 1].default_gateway = 0;
                    } else {
                        data_connection_arr[i - 1].default_gateway = 1;
                    }
                }
            } else if (!strcmp(token, "dns")) {
                CHECK_I_NOT_0;
                token = strtok(NULL, "\n");
                if (token != NULL) {
                    if (token[0] == 'N' || token[0] == 'n') {
                        data_connection_arr[i - 1].dns = 0;
                    } else {
                        data_connection_arr[i - 1].dns = 1;
                    }
                }
            } else if (!strcmp(token, "profile")) {
                CHECK_I_NOT_0;
                token = strtok(NULL, "\n");
                if (token != NULL) {
                    data_connection_arr[i - 1].profile = strtoul(token, NULL, 0);
                }
            }
        }
    }

end:
    free(line);
    fclose(stream);

    return i;
}

static int sanity_check_dc_def(int sz)
{
    int ret = -1;
    int i;
    int gateway_ipv4_set = -1;
    int gateway_ipv6_set = -1;

    if (!input_args.qmap && sz > 2) {
        RLOGE("%s: found %d data connection definitions, but qmap is not set",
                __FUNCTION__, sz);
        goto end;
    } else if (!input_args.qmap && sz == 2) {
        if (data_connection_arr[0].type == data_connection_arr[1].type) {
            RLOGE("%s: %d data connection definitions with the same ip type are not allowed without qmap",
                    __FUNCTION__, sz);
            goto end;
        }
    }

    for (i = 0; i < sz; i++) {
        if (strlen(data_connection_arr[i].apn) && data_connection_arr[i].profile > 0) {
            RLOGE("%s: apn information and profile for definition %d are mutually exclusive",
                    __FUNCTION__, i + 1);
            goto end;
        }
        if (!strlen(data_connection_arr[i].apn) && (data_connection_arr[i].profile == 0)) {
            RLOGE("%s: apn for definition %d is empty", __FUNCTION__, i + 1);
            goto end;
        }
        if (data_connection_arr[i].default_gateway) {
            if (data_connection_arr[i].type == QMI_IP_FAMILY_UNSPECIFIED) {
                if (gateway_ipv4_set != -1 || gateway_ipv6_set != -1) {
                    RLOGE("a single default gateway should be set (gateway already set for definition %s)",
                            gateway_ipv4_set !=
                            -1 ? data_connection_arr[gateway_ipv4_set].name : data_connection_arr[
                                gateway_ipv6_set].name);
                    goto end;
                } else {
                    gateway_ipv4_set = i;
                    gateway_ipv6_set = i;
                }
            } else if (data_connection_arr[i].type == QMI_IP_FAMILY_V4) {
                if (gateway_ipv4_set != -1) {
                    RLOGE("a single default gateway should be set (gateway already set for definition %s)",
                            data_connection_arr[gateway_ipv4_set].name);
                    goto end;
                } else {
                    gateway_ipv4_set = i;
                }
            } else if (data_connection_arr[i].type == QMI_IP_FAMILY_V6) {
                if (gateway_ipv6_set != -1) {
                    RLOGE("a single default gateway should be set (gateway already set for definition %s)",
                            data_connection_arr[gateway_ipv6_set].name);
                    goto end;
                } else {
                    gateway_ipv6_set = i;
                }
            }
        }
    }

    ret = 0;
end:
    return ret;
}

static int parse_args(int argc, char **argv)
{
    int ret = 0;
    int c;

    input_args.iface_id = 0xFFFFFFFF;

    while (1) {
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "c:d:n:s:m:p:i:u:",
                long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1) {
            break;
        }

        switch (c) {
            case 0:
                /* Flag */
                break;

            case 'c':
                input_args.conf = optarg;
                break;

            case 'd':
                input_args.device = optarg;
                break;

            case 'n':
                input_args.netdevice = strdup(optarg);
                break;

            case 's':
                input_args.max_dl_aggr_size = strtoul(optarg, NULL, 0);
                break;

            case 'm':
                input_args.max_dl_datagram_number = strtoul(optarg, NULL, 0);
                break;

            case 'p':
                input_args.dl_padding = strtoul(optarg, NULL, 0);
                break;

            case 'i':
                input_args.iface_id = strtoul(optarg, NULL, 0);
                break;

            case 'u':
                input_args.mtu = strtoul(optarg, NULL, 0);
                break;

            case '?':
            default:
                ret = -1;
                goto end;
        }
    }

    if (input_args.debug) {
        telit_set_log_level(LOG_DEBUG);
    } else {
        telit_set_log_level(LOG_INFO);
    }

    RLOGD("%s: debug = %s", __FUNCTION__, input_args.debug ? "enabled" : "disabled");
    RLOGD("%s: qmap = %s", __FUNCTION__, input_args.qmap ? "enabled" : "disabled");
    RLOGD("%s: device = %s", __FUNCTION__, input_args.device);
    RLOGD("%s: netdevice = %s", __FUNCTION__, input_args.netdevice ? input_args.netdevice : "missing");
    RLOGD("%s: conf = %s", __FUNCTION__, input_args.conf ? input_args.conf : "missing");
    RLOGD("%s: max_dl_aggr_size = %u", __FUNCTION__, input_args.max_dl_aggr_size);
    RLOGD("%s: max_dl_datagram_number = %u", __FUNCTION__, input_args.max_dl_datagram_number);
    RLOGD("%s: dl_padding = %u", __FUNCTION__, input_args.dl_padding);
    RLOGD("%s: iface_id = %u", __FUNCTION__, input_args.iface_id);
    RLOGD("%s: mtu = %u", __FUNCTION__, input_args.mtu);

    /* Set functions according to the device type */
    if (identify_device(input_args.device)) {
        RLOGE("%s: unable to identify device %s", __FUNCTION__,
                input_args.device);
        ret = -1;
        goto end;
    }
    /* Try to get the network interface if not indicated */
    if (input_args.netdevice == NULL) {
        if (dc_ops.get_network_interface(input_args.device, &input_args.netdevice)) {
            RLOGE("%s: unable to identify network interface, re-run with -n <interface name>",
                    __FUNCTION__);
            ret = -1;
            goto end;
        }
    }

end:
    return ret;
}

static void data_call_cb(void *item)
{
    int ret;
    wds_ind_event_report_t out;
    indication_item_t *ind = (indication_item_t *)item;

    ret = telit_wds_indication_handler(ind->msg_id,
            ind->buf, ind->buf_len,
            (uint8_t *)&out, sizeof(out));
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("unpack fail %d ", ret);
    }

    switch (ind->msg_id) {
        case QMI_WDS_EVENT_REPORT_IND:
            if (out.data_call_status_available) {
                RLOGI("data call status %u", out.data_call_status);
                sem_post(&data_connection_sem);
            }
            break;
        default:
            RLOGD("Indication %u not managed", ind->msg_id);
            break;
    }

    free(ind->buf);
    free(ind);
}

static int identify_quirks()
{
    int ret = 0;
    char model_id[MAX_STRING_SIZE] = {0};

    ret = req_get_model_id(&ctx, model_id, MAX_STRING_SIZE);
    if (!ret) {
        if (strstr(model_id, "FN980") || strstr(model_id, "FN990")) {
            quirks |= QUIRK_NEED_PROFILE;
            RLOGD("QUIRK_NEED_PROFILE enabled");
        }
    } else {
        RLOGE("Unable to identify model");
    }

    return ret;
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    struct sigaction act;
    uint32_t rx_urb_size = 0;
    int qmap_id = 1;
    int dc_definitions;
    int wds_dc = 0;
    int i;

    RLOGI("Telit qmi SDK version: %s", telit_get_version());

    memset(&input_args, 0, sizeof(input_args));
    input_args.device = default_rmnet_dev;

    if (sem_init(&data_connection_sem, 0, 0)) {
        FAIL("sem_init failed");
    }

    if (parse_args(argc, argv)) {
        OUT("unrecognized argument");
    }
    if (!input_args.netdevice) {
        OUT("netdevice missing, but required");
    }
    if (!input_args.conf) {
        OUT("conf file missing, but required");
    }

    dc_definitions = parse_configuration_file(input_args.conf);
    if (dc_definitions == -1) {
        FAIL("Unable to parse configuration file");
    } else if (dc_definitions == 0) {
        FAIL("No data connection definition found");
    }

    if (sanity_check_dc_def(dc_definitions)) {
        FAIL("conf file malformed");
    }

    act.sa_handler = ctrl_c_handler;
    sigaction(SIGINT, &act, NULL);

    if (tqcm_init(input_args.device, &ctx)) {
        FAIL("Failed tqcm initialization");
    }

    /* Currently quirk identification is not blocking */
    identify_quirks();

    /* Start from link down, no ip address master netdevice */
    if (netdevice_dispose(input_args.netdevice)) {
        FAIL("unable to set netdevice down");
    }

    /* Generic QMAP check */
    if (input_args.qmap) {
        /* Check if existing qmap interfaces */
        if (dc_ops.check_qmap_interface_existence(input_args.netdevice)) {
            FAIL("qmap netdevice already set, please remove them and restart");
        }
        if (dc_ops.open_hw_data_port(&ctx)) {
            FAIL("Unable to open hw data port");
        }
        /* set qmap */
        if (dc_ops.set_data_format(&ctx, 1,
                input_args.max_dl_aggr_size, input_args.max_dl_datagram_number,
                input_args.dl_padding, &rx_urb_size, input_args.iface_id)) {
            FAIL("Failed setting qmap data format");
        }
        /* unset raw-ip for being able to set MTU */
        if (dc_ops.enable_raw_ip(input_args.netdevice, 0)) {
            FAIL("Unable to set raw-ip = N");
        }
        /* Change rx_urb_size through MTU */
        if (dc_ops.set_mtu(input_args.netdevice, rx_urb_size)) {
            FAIL("Unable to set MTU");
        }
    } else {
        if (dc_ops.open_hw_data_port(&ctx)) {
            FAIL("Unable to open hw data port");
        }
        if (dc_ops.set_data_format(&ctx, 0, 0, 0, 0, &rx_urb_size, input_args.iface_id)) {
            FAIL("Failed disabling data aggregation format");
        }
        /* Change rx_urb_size through MTU */
        if (dc_ops.set_mtu(input_args.netdevice, input_args.mtu ? input_args.mtu : 1500)) {
            FAIL("Unable to set MTU");
        }
        if (dc_ops.enable_raw_ip(input_args.netdevice, 1)) {
            FAIL("Unable to set raw-ip = Y");
        }
    }

    memset(wds_data_conn_arr, 0, sizeof(wds_data_conn_arr));

    for (i = 0; i < dc_definitions; i++) {
        int wds_iter = wds_dc;

        if (tqcm_init_client_context(QMI_SVC_WDS, &ctx, &wds_data_conn_arr[wds_iter].ctx_wds, data_call_cb)) {
            BAIL_OUT("Failed wds client context opening");
        }
        wds_dc++;

        if (input_args.qmap) {
            if (dc_ops.bind_mux(&wds_data_conn_arr[wds_iter].ctx_wds, qmap_id, input_args.iface_id)) {
                BAIL_OUT("Failed binding with secondary qmap");
            }
            if (dc_ops.add_qmap_interface(&input_args, qmap_id)) {
                BAIL_OUT("Unable to create qmap network interface");
            }
            wds_data_conn_arr[wds_iter].qmap_id = qmap_id;
            /* qmimux network interface starts from 0 */
            snprintf(wds_data_conn_arr[wds_iter].netdevice, MAX_STRING_SIZE, "%s%d",
                    dc_ops.get_qmap_prefix(), i);
            qmap_id++;
        } else {
            snprintf(wds_data_conn_arr[wds_iter].netdevice, MAX_STRING_SIZE, "%s", input_args.netdevice);
        }

        if (req_set_ip_family(&wds_data_conn_arr[wds_iter].ctx_wds, &data_connection_arr[i])) {
            BAIL_OUT("Failed setting ip family");
        }
        if (quirks & QUIRK_NEED_PROFILE) {
            if (req_create_profile(&data_connection_arr[i], &wds_data_conn_arr[wds_iter])) {
                BAIL_OUT("Unable to create profiles");
            }
        }
        if (req_start_network(&data_connection_arr[i], &wds_data_conn_arr[wds_iter])) {
            BAIL_OUT("Failed starting data connection");
        }
        if (req_set_event_report(&wds_data_conn_arr[wds_iter].ctx_wds)) {
            BAIL_OUT("Unable to set event report");
        }
        if (req_get_runtime_settings(&wds_data_conn_arr[wds_iter])) {
            BAIL_OUT("Failed get runtime settings");
        }
        if (netdevice_configure_address(wds_data_conn_arr[wds_iter].netdevice,
                wds_data_conn_arr[wds_iter].address)) {
            BAIL_OUT("Failed configuring network interface");
        }

        /* Delete gateway if it should not be set */
        if (!data_connection_arr[i].default_gateway) {
            memset(wds_data_conn_arr[wds_iter].gateway, '\0', MAX_STRING_SIZE);
        } else {
            /* Disable ra to avoid race condition for ipv6 gateway */
            if (strstr(wds_data_conn_arr[wds_iter].gateway, ":")) {
                netdevice_set_ra(wds_data_conn_arr[wds_iter].netdevice, 0);
            }
        }

        /* Delete DNS if it should not be set */
        if (!data_connection_arr[i].dns) {
            memset(wds_data_conn_arr[wds_iter].dns1, '\0', MAX_STRING_SIZE);
        }
    }

    /* Bring up master network interface*/
    netdevice_set_link(input_args.netdevice, 1);

    /* Bring up qmap network interfaces */
    for (i = 0; i < wds_dc; i++) {
        if (strcmp(input_args.netdevice, wds_data_conn_arr[i].netdevice)) {
            RLOGI("Netdevice: %s, gateway: %s", wds_data_conn_arr[i].netdevice, wds_data_conn_arr[i].gateway);
            if (netdevice_set_link(wds_data_conn_arr[i].netdevice, 1)) {
                BAIL_OUT("Failed setting netdevice link status");
            }
        }
        if (strlen(wds_data_conn_arr[i].gateway)) {
            if (netdevice_configure_route(wds_data_conn_arr[i].netdevice, wds_data_conn_arr[i].gateway)) {
                BAIL_OUT("Failed configuring route for gateway");
            }
        }
        if (strlen(wds_data_conn_arr[i].dns1)) {
            if (netdevice_configure_dns(wds_data_conn_arr[i].netdevice, wds_data_conn_arr[i].dns1)) {
                BAIL_OUT("Failed configuring dns1 for gateway");
            }

            if (strlen(wds_data_conn_arr[i].dns2)) {
                if (netdevice_configure_dns(wds_data_conn_arr[i].netdevice, wds_data_conn_arr[i].dns2)) {
                    BAIL_OUT("Failed configuring dns2 for gateway");
                }
            }
        }
    }

    RLOGI("Data connection setup done");
    RLOGI("Press ctrl+c to terminate data connections");

    while (!data_stop) {
        if (sem_wait(&data_connection_sem)) {
            if (errno != EINTR) {
                RLOGE("sem_wait %s", strerror(errno));
            }
        }

        /* data stop only if all the results are OUT_OF_CALL */
        for (i = 0; i < wds_dc; i++) {
            if (req_get_runtime_settings(&wds_data_conn_arr[i]) == 0) {
                break;
            }
            if (i == (wds_dc - 1)) {
                data_stop = 1;
            }
        }
    }

end:
    for (i = 0; i < wds_dc; i++) {
        if (req_stop_network_interface(&wds_data_conn_arr[i])) {
            RLOGE("Unable to stop data connection");
        }

        if (quirks & QUIRK_NEED_PROFILE) {
            req_delete_profile(&wds_data_conn_arr[i]);
        }

        if (tqcm_deinit_client_context(&ctx, &wds_data_conn_arr[i].ctx_wds)) {
            RLOGE("Failed context wds context disposal");
        }
    }

    netdevice_dispose(input_args.netdevice);

    /* We should cycle again after setting down master interface
     * in order to dispose qmimux interfaces */
    for (i = 0; i < wds_dc; i++) {
        if (wds_data_conn_arr[i].qmap_id) {
            dc_ops.del_qmap_interface(input_args.netdevice,
                    wds_data_conn_arr[i].qmap_id);
        }
    }

    /* Unconditionally enable ra again */
    netdevice_set_ra(input_args.netdevice, 1);

    if (dc_ops.enable_raw_ip(input_args.netdevice, 0)) {
        RLOGE("Unable to restore raw-ip");
    }

    /* Maybe export a public method to check if a context is active? */
    if (ctx.dev != NULL) {
        tqcm_deinit(&ctx);
    }

fail:
    sem_destroy(&data_connection_sem);

    if (input_args.netdevice) {
        free(input_args.netdevice);
    }
    exit(ret);
}
