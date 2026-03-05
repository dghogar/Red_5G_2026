#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/if_arp.h>
#include <linux/ipv6.h>
#include <arpa/inet.h>

#include <telit_sdk.h>
#include <telit_sdk_wda.h>
#include <telit_sdk_wds.h>
#include <tqcm.h>
#include <tqcm_wda.h>
#include <tqcm_wds.h>
#include <telit_sdk_test_helpers.h>

/**
 * Concurrent data connections example using Telit QMI SDK
 *
 * The purpose of this example is to show how to use Telit QMI SDK for creating
 * four concurrent data connections, by using qmap capabilities.
 *
 * The example creates the following data connections:
 *
 * Default
 * IMS
 * Admin
 * MMS
 *
 * APNs should be customized in the code below.
 *
 * Look for the keyword ATTENTION for all the elements to be customized.
 *
 * NOTE: the example is provided as it is and can't be considered a production level
 * application.
 */

/** ATTENTION
 *
 * The following items should be customized according to the use-case
 */
/** Modem control channel (created by the qmi_wwan driver) */
#define MODEM_CONTROL_PATH "/dev/cdc-wdm0"
/** Modem netdevice, parent of the qmap netdevices */
#define WWAN_NETDEVICE_NAME "wwan0"
/** List of APNs for each data connection type */
#define APN_DEFAULT "internet"
#define APN_IMS     "ims"
#define APN_ADMIN   "default"
#define APN_MMS     "internet4"
/** List of qmap netdevices name for each data connection type */
#define QMAP_IF_NAME_DEFAULT    "cellular_def"
#define QMAP_IF_NAME_IMS        "cellular_ims"
#define QMAP_IF_NAME_ADMIN      "cellular_adm"
#define QMAP_IF_NAME_MMS        "cellular_mms"
/** End of customizations */

#define QMI_IND_BUFFER_SIZE 6144

#define NETDEVICE_DOWN  0
#define NETDEVICE_UP    1

#define RMNET_MAX_DL_URB 16384
#define RMNET_DATA_TYPE "rmnet"
#define RMNET_INTERFACE_ID 2

/** List of information to be tracked for each data connection type */
/** Data connection #1: default (IPV4V6) */
/** IPV4 data connection handle: returned by QMI_WDS_START_DATA_NETWORK and required for termination */
uint32_t pkt_data_handle_default = 0;
/** IPV6 data connection handle: returned by QMI_WDS_START_DATA_NETWORK and required for termination */
uint32_t pkt_data_handle_default_ipv6 = 0;
/** qmap mux id associated to wds client in which the data connection is started */
#define QMAP_ID_DEFAULT 1
/** wds client for the ipv4 data connection */
tqcm_client_context_t ctx_wds_pdn_default = {0};
/** wds client for the ipv6 data connection */
tqcm_client_context_t ctx_wds_pdn_default_ipv6 = {0};
/* Data connection #2 ims (IPV4V6) */
uint32_t pkt_data_handle_ims = 0;
uint32_t pkt_data_handle_ims_ipv6 = 0;
#define QMAP_ID_IMS 2
tqcm_client_context_t ctx_wds_pdn_ims = {0};
tqcm_client_context_t ctx_wds_pdn_ims_ipv6 = {0};
/* Data connection #3 admin (IPV4V6) */
uint32_t pkt_data_handle_admin = 0;
uint32_t pkt_data_handle_admin_ipv6 = 0;
#define QMAP_ID_ADMIN 3
tqcm_client_context_t ctx_wds_pdn_admin = {0};
tqcm_client_context_t ctx_wds_pdn_admin_ipv6 = {0};
/* Data connection #4 mms (IPV4 )*/
uint32_t pkt_data_handle_mms = 0;
#define QMAP_ID_MMS 4
tqcm_client_context_t ctx_wds_pdn_mms = {0};

/** Telit QMI SDK global context, populated by init and required when initializing a client */
tqcm_context_t ctx = {0};
/** Semaphore used in the main thread loop */
static sem_t loop_sem;
/** Flag to control the application main thread flow */
static int data_stop = 0;

/** @BEGIN Netlink related functions and data are used for creating/disposing the qmap netdevices */

/** Netlink message data structure */
typedef struct {
    struct nlmsghdr msghdr;
    struct ifinfomsg ifreq;
    char _[1024];
    /* `_' exists to let the compiler allocate a sequential memory right after n and i,
       and it is never accessed directly via its name */
} nlmsg_t;

/**
 * @brief Netlink message initialization function
 *
 * @param[in] type Type of netlink request.
 * @param[in] extra_flags Extra flags besides NLM_F_REQUEST and NLM_F_ACK
 * @param[in] nlmsg pointer to a netlink message
 */
static void netlink_init(int type, int extra_flags, nlmsg_t *nlmsg)
{
    memset(nlmsg, 0, sizeof(*nlmsg));

    nlmsg->msghdr.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg));
    nlmsg->msghdr.nlmsg_type = type;
    nlmsg->msghdr.nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK | extra_flags;
    nlmsg->msghdr.nlmsg_seq = 1;
    nlmsg->ifreq.ifi_family = AF_UNSPEC;
    if (type != RTM_DELLINK) {
        nlmsg->ifreq.ifi_type = ARPHRD_RAWIP;
        nlmsg->ifreq.ifi_flags = 0;
        nlmsg->ifreq.ifi_change = 0xFFFFFFFF;
    }
}

/**
 * @brief Netlink socket open function
 *
 * @return -1 on error, otherwise a file descriptor
 */
static int netlink_open()
{
    return socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
}

/**
 * @brief Netlink message send function
 *
 * @param[in] sock socket file descriptor
 * @param[in] nlmsg pointer to a netlink message
 *
 * @return -1 on error, otherwise the number of bytes sent
 */
static int netlink_send(int sock, nlmsg_t *nlmsg)
{
    return send(sock, nlmsg, nlmsg->msghdr.nlmsg_len, 0);
}

/**
 * @brief Function to append a part to the netlink message
 *
 * @param[in,out] nh pointer to a netlink message header
 * @param[in] type type of the part to be appended
 * @param[in] data optional data of the part
 * @param[in] raw_data_length length of the optional data part
 */
static void append_netlink_msg(struct nlmsghdr *nh, int type, const void *data, int raw_data_length)
{
    struct rtattr *rta;
    int rta_length = RTA_LENGTH(raw_data_length);

    rta = (struct rtattr *)((char *)nh + NLMSG_ALIGN(nh->nlmsg_len));

    rta->rta_type = type;
    rta->rta_len = rta_length;
    nh->nlmsg_len =  NLMSG_ALIGN(nh->nlmsg_len) + RTA_ALIGN(rta_length);

    if (data)
        memcpy(RTA_DATA(rta), data, raw_data_length);
}

/**
 * @brief Function for creating a qmap netdevice
 *
 * @param[in] netdev_name modem parent netdevice name
 * @param[in] qmap_netdev_name qmap netdevice name
 * @param[in] mux_id qmap mux id
 *
 * @return -1 on error, 0 on success
 */
static int netlink_add_rmnet(char *netdev_name,
        char *qmap_netdev_name,
        __u16 mux_id)
{
    struct ifla_rmnet_flags flags;
    struct rtattr *linkinfo;
    struct rtattr *linkdata;
    __u32 if_index;
    nlmsg_t nlmsg;
    int sock;
    int ret;

    if_index = if_nametoindex(netdev_name);
    if (if_index == 0) {
        RLOGE("%s: unable to get netdev index, errno = %d",
                __FUNCTION__, errno);
        return -1;
    }

    sock = netlink_open();
    if (sock == -1) {
        RLOGE("%s: unable to open netlink socket", __FUNCTION__);
        return -1;
    }

    netlink_init(RTM_NEWLINK, NLM_F_EXCL | NLM_F_CREATE, &nlmsg);

    append_netlink_msg(&nlmsg.msghdr, IFLA_LINK, &if_index, sizeof(__u32));
    append_netlink_msg(&nlmsg.msghdr, IFLA_IFNAME, qmap_netdev_name,
            strlen(qmap_netdev_name));

    linkinfo = (struct rtattr *)((char *)&nlmsg.msghdr + NLMSG_ALIGN(nlmsg.msghdr.nlmsg_len));
    append_netlink_msg(&nlmsg.msghdr, IFLA_LINKINFO, NULL, 0);
    append_netlink_msg(&nlmsg.msghdr, IFLA_INFO_KIND, RMNET_DATA_TYPE, strlen(RMNET_DATA_TYPE));

    linkdata = (struct rtattr *)((char *)&nlmsg.msghdr + NLMSG_ALIGN(nlmsg.msghdr.nlmsg_len));
    append_netlink_msg(&nlmsg.msghdr, IFLA_INFO_DATA, NULL, 0);
    append_netlink_msg(&nlmsg.msghdr, IFLA_RMNET_MUX_ID, &mux_id, sizeof(__u16));

    flags.flags = RMNET_FLAGS_INGRESS_DEAGGREGATION;
    flags.mask = RMNET_FLAGS_INGRESS_DEAGGREGATION;
    append_netlink_msg(&nlmsg.msghdr, IFLA_RMNET_FLAGS, &flags, sizeof(struct ifla_rmnet_flags));

    linkinfo->rta_len = (int)((char *)&nlmsg.msghdr + NLMSG_ALIGN(nlmsg.msghdr.nlmsg_len) - (char *)linkinfo);
    linkdata->rta_len = (int)((char *)&nlmsg.msghdr + NLMSG_ALIGN(nlmsg.msghdr.nlmsg_len) - (char *)linkdata);

    ret = netlink_send(sock, &nlmsg);
    if (ret < 0)
        RLOGE("%s: unable to create qmap netdevice, err = %d", __FUNCTION__, ret);
    close(sock);

    return (ret > 0 ? 0 : -1);
}

/**
 * @brief Function for deleting a qmap netdevice
 *
 * @param[in] qmap_netdev_name qmap netdevice name
 *
 * @return -1 on error, 0 on success
 */
static int netlink_del_rmnet(char *qmap_netdev_name)
{
    __u32 if_index;
    nlmsg_t nlmsg;
    int sock;
    int ret;

    if_index = if_nametoindex(qmap_netdev_name);
    if (if_index == 0) {
        RLOGE("%s: unable to get netdev index, errno = %d",
                __FUNCTION__, errno);
        return -1;
    }

    sock = netlink_open();
    if (sock == -1) {
        RLOGE("%s: unable to open netlink socket", __FUNCTION__);
        return -1;
    }

    netlink_init(RTM_DELLINK, 0, &nlmsg);

    append_netlink_msg(&nlmsg.msghdr, IFLA_LINK, &if_index, sizeof(__u32));
    append_netlink_msg(&nlmsg.msghdr, IFLA_IFNAME, qmap_netdev_name,
            strlen(qmap_netdev_name));

    ret = netlink_send(sock, &nlmsg);
    if (ret < 0)
        RLOGE("%s: unable to delete qmap netdevice, err = %d", __FUNCTION__, ret);
    close(sock);

    return (ret > 0 ? 0 : -1);
}

int create_qmap_netdevices()
{
    if (netlink_add_rmnet(WWAN_NETDEVICE_NAME,
            QMAP_IF_NAME_DEFAULT, QMAP_ID_DEFAULT)) {
        RLOGE("Unable to create rmnet interface %s", QMAP_IF_NAME_DEFAULT);
        return -1;
    }
    if (netlink_add_rmnet(WWAN_NETDEVICE_NAME,
            QMAP_IF_NAME_IMS, QMAP_ID_IMS)) {
        RLOGE("Unable to create rmnet interface %s", QMAP_IF_NAME_IMS);
        return -1;
    }
    if (netlink_add_rmnet(WWAN_NETDEVICE_NAME,
            QMAP_IF_NAME_ADMIN, QMAP_ID_ADMIN)) {
        RLOGE("Unable to create rmnet interface %s", QMAP_IF_NAME_ADMIN);
        return -1;
    }
    if (netlink_add_rmnet(WWAN_NETDEVICE_NAME,
            QMAP_IF_NAME_MMS, QMAP_ID_MMS)) {
        RLOGE("Unable to create rmnet interface %s", QMAP_IF_NAME_MMS);
        return -1;
    }

    return 0;
}

int dispose_qmap_netdevices()
{
    int ret = 0;

    if (netlink_del_rmnet(QMAP_IF_NAME_DEFAULT)) {
        RLOGE("Unable to delete rmnet interface %s", QMAP_IF_NAME_DEFAULT);
        ret = -1;
    }
    if (netlink_del_rmnet(QMAP_IF_NAME_IMS)) {
        RLOGE("Unable to delete rmnet interface %s", QMAP_IF_NAME_IMS);
        ret = -1;
    }
    if (netlink_del_rmnet(QMAP_IF_NAME_ADMIN)) {
        RLOGE("Unable to delete rmnet interface %s", QMAP_IF_NAME_ADMIN);
        ret = -1;
    }
    if (netlink_del_rmnet(QMAP_IF_NAME_MMS)) {
        RLOGE("Unable to delete rmnet interface %s", QMAP_IF_NAME_MMS);
        ret = -1;
    }

    return ret;
}
/** @END netlink related functions */

/** @BEGIN sysfs helper functions */

/**
 * @brief Function for writing a string to a sysfs file
 *
 * @param[in] file file name
 * @param[in] buffer string to be written
 * @param[in] sz size of the string
 *
 * @return -1 on error, 0 on success
 */
int write_sysfs_file(char *file, char *buffer, ssize_t sz)
{
    int ret = 0;
    int fd;

    fd = open(file, O_WRONLY);
    if (fd < 0) {
        RLOGE("%s: unable to open %s, %s", __FUNCTION__,
                file, strerror(errno));
        return -1;
    }

    sz = pwrite(fd, buffer, sz, 0);
    if (sz < 0) {
        RLOGE("%s: unable to write to %s, %s", __FUNCTION__,
                file, strerror(errno));
        ret = -1;
    }

    close(fd);
    return ret;
}

/** @END sysfs helper functions */

/** @BEGIN Netdevice management functions */

/**
 * @brief Function for setting the netdevice state
 *
 * @param[in] state netdevice state, 0 = down, 1 = up
 * @param[in] if_name name of the netdevice
 *
 * @return false on error, true on success
 */
bool configure_network_interface_state(int state, char *if_name)
{
    bool ret = true;
    int sock_fd = -1;
    struct ifreq ifr;

    strncpy(ifr.ifr_name, if_name, IFNAMSIZ);

    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (ioctl(sock_fd, SIOCGIFFLAGS, &ifr) < 0) {
        RLOGE("%s: failed SIOCGIFFLAGS, %s", __FUNCTION__, strerror(errno));
        ret = false;
        goto end;
    }

    if (state)
        ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);
    else
        ifr.ifr_flags &= ~(IFF_UP | IFF_RUNNING);
    if (ioctl(sock_fd, SIOCSIFFLAGS, &ifr) < 0) {
        RLOGE("%s: failed SIOCSIFFLAGS, %s", __FUNCTION__, strerror(errno));
        ret = false;
    }

end:
    if (sock_fd != -1) {
        close(sock_fd);
    }

    return ret;
}

/**
 * @brief Function for setting the netdevice state
 *
 * @param[in] mtu value to be set to the netdevice as mtu
 * @param[in] if_name name of the netdevice
 *
 * @return false on error, true on success
 */
bool configure_network_interface_mtu(int mtu, char *if_name)
{
    bool ret = true;
    int sock_fd = -1;
    struct ifreq ifr;

    strncpy(ifr.ifr_name, if_name, IFNAMSIZ);
    ifr.ifr_mtu = mtu;

    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (ioctl(sock_fd, SIOCSIFMTU, &ifr) < 0) {
        RLOGE("%s: failed SIOCSIFMTU, %s", __FUNCTION__, strerror(errno));
        ret = false;
    }

    if (sock_fd != -1) {
        close(sock_fd);
    }

    return ret;
}

/**
 * @brief Function for setting the ipv4 address
 *
 * @param[in] netdevice name of the netdevice
 * @param[in] ipv4 ipv4 address in string format x.y.z.q
 * @param[in] netmask netmask in string format x.y.z.q
 *
 * @return false on error, true on success
 */
bool configure_ipv4_address(char *netdevice, char *ipv4, char *netmask)
{
    bool ret = false;
    int sock_fd;
    struct ifreq ifr = {0};
    struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;

    strncpy(ifr.ifr_name, netdevice, IFNAMSIZ);
    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock_fd == -1) {
        RLOGE("%s: failed socket opening", __FUNCTION__);
        return false;
    }

    if (ioctl(sock_fd, SIOCGIFINDEX, &ifr) < 0) {
        RLOGE("%s: failed SIOCGIFINDEX AF_INET for interface %s, %s,",
                __FUNCTION__, netdevice, strerror(errno));
        goto end;
    }

    ifr.ifr_addr.sa_family = AF_INET;
    if (inet_pton(AF_INET, ipv4, &addr->sin_addr) != 1) {
        RLOGE("%s: failed inet_pton, %s", __FUNCTION__, strerror(errno));
        goto end;
    }
    if (ioctl(sock_fd, SIOCSIFADDR, &ifr) < 0) {
        RLOGE("%s: failed SIOCSIFADDR, %s", __FUNCTION__, strerror(errno));
        goto end;
    }

    if (inet_pton(AF_INET, netmask, &addr->sin_addr) != 1) {
        RLOGE("%s: failed inet_pton, %s", __FUNCTION__, strerror(errno));
        goto end;
    }
    if (ioctl(sock_fd, SIOCSIFNETMASK, &ifr) < 0) {
        RLOGE("%s: failed SIOCSIFNETMASK, %s", __FUNCTION__, strerror(errno));
        goto end;
    }

    ret = true;
end:
    close(sock_fd);
    return ret;
}

/**
 * @brief Function for setting the ipv4 address
 *
 * @param[in] netdevice name of the netdevice
 * @param[in] ipv6 ipv4 address in string format :::::
 * @param[in] prefix_len ipv6 address prefix length
 *
 * @return false on error, true on success
 */
bool configure_ipv6_address(char *netdevice, char *ipv6, uint8_t prefix_len)
{
    bool ret = false;
    int sock_fd;
    struct ifreq ifr = {0};
    struct in6_ifreq ifr6 = {0};

    strncpy(ifr.ifr_name, netdevice, IFNAMSIZ);
    sock_fd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_IP);
    if (sock_fd == -1) {
        RLOGE("%s: failed socket opening", __FUNCTION__);
        return false;
    }

    if (ioctl(sock_fd, SIOCGIFINDEX, &ifr) < 0) {
        RLOGE("%s: failed SIOCGIFINDEX AF_INET6 for interface %s, %s,",
                __FUNCTION__, netdevice, strerror(errno));
        goto end;
    }

    if (inet_pton(AF_INET6, ipv6, &ifr6.ifr6_addr) != 1) {
        RLOGE("%s: failed inet_pton ipv6, %s", __FUNCTION__, strerror(errno));
        goto end;
    }
    ifr6.ifr6_prefixlen = prefix_len;
    ifr6.ifr6_ifindex = ifr.ifr_ifindex;
    if (ioctl(sock_fd, SIOCSIFADDR, &ifr6) < 0) {
        RLOGE("%s: failed SIOCSIFADDR ipv6, %s", __FUNCTION__, strerror(errno));
        goto end;
    }

    ret = true;
end:
    close(sock_fd);
    return ret;
}

/**
 * @brief Function for setting the ipv4 address
 *
 * @param[in] raw_ip 0 for 802.03 (NOT SUPPORTED), 1 for raw-ip
 * @param[in] netdevice name of the netdevice
 *
 * @return false on error, true on success
 */
int configure_qmi_wwan_raw_ip(uint8_t raw_ip, char *netdevice_name)
{
    char path[1024] = {'\0'};
    int ret = 0;

    if (raw_ip) {
        ret = snprintf(path, 1024, "/sys/class/net/%s/qmi/raw_ip", netdevice_name);
        if (ret > 0) {
            if (write_sysfs_file(path, raw_ip ? "Y" : "N", 1)) {
                RLOGE("%s: unable to set raw_ip to %u", __FUNCTION__, raw_ip);
                ret = -1;
                goto end;
            } else {
                memset(path, '\0', sizeof(path));
                ret = snprintf(path, 1024, "/sys/class/net/%s/qmi/pass_through", netdevice_name);
                if (ret > 0) {
                    if (write_sysfs_file(path, raw_ip ? "Y" : "N", 1)) {
                        RLOGE("%s: unable to set pass_through to %u", __FUNCTION__, raw_ip);
                        ret = -1;
                        goto end;
                    }
                } else {
                    RLOGE("%s: error in snprintf", __FUNCTION__);
                    goto end;
                }
            }
            ret = 0;
        } else
            RLOGE("%s: error in snprintf", __FUNCTION__);
    } else {
        ret = snprintf(path, 1024, "/sys/class/net/%s/qmi/pass_through", netdevice_name);
        if (ret > 0) {
            if (write_sysfs_file(path, raw_ip ? "Y" : "N", 1)) {
                RLOGE("%s: unable to set pass_through to %u", __FUNCTION__, raw_ip);
                ret = -1;
                goto end;
            } else {
                memset(path, '\0', sizeof(path));
                ret = snprintf(path, 1024, "/sys/class/net/%s/qmi/raw_ip", netdevice_name);
                if (ret > 0) {
                    if (write_sysfs_file(path, raw_ip ? "Y" : "N", 1)) {
                        RLOGE("%s: unable to set raw_ip to %u", __FUNCTION__, raw_ip);
                        ret = -1;
                        goto end;
                    }
                } else {
                    RLOGE("%s: error in snprintf", __FUNCTION__);
                    goto end;
                }
            }
            ret = 0;
        } else
            RLOGE("%s: error in snprintf", __FUNCTION__);
    }

end:
    return ret;
}

/** @END Netdevice management functions */

/** @BEGIN Modem QMI management functions */

/**
 * @brief Function for setting QMAP parameters to the modem
 *
 * @param[out] rx_urb_size maximum rx aggregated size set by the modem
 *
 * @return 0 on success, otherwise error
 */
int configure_modem_data_format(uint32_t *rx_urb_size)
{
    tqcm_client_context_t ctx_wda = {0};
    int ret;

    ret = tqcm_init_client_context(QMI_SVC_WDA,
            &ctx,
            &ctx_wda,
            NULL);
    if (ret != 0) {
        RLOGE("%s: failed WDA context opening", __FUNCTION__);
    } else {
        wda_data_format_t input = {0};
        wda_data_format_t out;
        uint16_t sdk_error;

        input.ul_data_aggregation_protocol_available = true;
        input.dl_data_aggregation_protocol_available = true;
        input.ul_data_aggregation_protocol = QMI_WDA_DATA_AGG_QMAP;
        input.dl_data_aggregation_protocol = QMI_WDA_DATA_AGG_QMAP;
        input.dl_data_aggregation_max_size_available = true;
        input.dl_data_aggregation_max_size = RMNET_MAX_DL_URB;
        input.dl_data_aggregation_max_datagrams_available = true;
        input.dl_data_aggregation_max_datagrams = 16;
        input.link_prot_available = true;
        input.link_prot = QMI_WDA_LINK_PROTOCOL_IP;
        input.peripheral_endpoint_available = true;
        input.peripheral_endpoint.ep_type = QMI_WDA_DATA_EP_TYPE_HSUSB;
        input.peripheral_endpoint.iface_id = RMNET_INTERFACE_ID;

        ret = tqcm_wda_set_data_format_send(&ctx_wda,
                &input,
                &out,
                &sdk_error,
                NULL,
                0);
        if (ret != 0) {
            RLOGE("%s: tqcm_wda_set_data_format_send fail %d", __FUNCTION__, ret);
            if (ret == EPROTO) {
                RLOGE("%s: qmi protocol error 0x%04X", __FUNCTION__, sdk_error);
                ret = sdk_error;
            }
        } else {
            if (!out.dl_data_aggregation_max_size_available)
                out.dl_data_aggregation_max_size = RMNET_MAX_DL_URB;
            else
                *rx_urb_size = out.dl_data_aggregation_max_size;
        }

        ret = tqcm_deinit_client_context(&ctx, &ctx_wda);
        if (ret)
            RLOGE("%s: failed wda context disposal, ret = %u", __FUNCTION__, ret);
    }

    return ret;
}

/**
 * @brief Function for binding a WDS client to a specific ip family
 *
 * @param[in] ip_family maximum rx aggregated size set by the modem
 * @param[in] ctx wds client
 *
 * @return 0 on success, otherwise error
 */
int set_ip_family(uint8_t ip_family, tqcm_client_context_t *ctx)
{
    wds_ip_family_t input_ip_family = {0};
    uint16_t sdk_error;
    int ret;

    input_ip_family.ip_preference = ip_family;

    ret = tqcm_wds_set_client_ip_family_pref_send(ctx,
            &input_ip_family,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("%s: tqcm_wds_set_client_ip_family_pref_send fail %d", __FUNCTION__, ret);
    }

    return ret;
}

/**
 * @brief Function for checking the features of a profile
 *
 * @param[in] ctx wds client
 * @param[in] profile_id id of the profile to be checked
 * @param[in] apn apn of the profile for matching
 * @param[in] pdp_type pdp type of the profile for matching, see #QMI_WDS_PDP_type_t
 * @param[in] pcscf_requested pcscf through pco enabled in profile
 *
 * @return true if matched, false otherwise
 */
static bool is_profile_matched(tqcm_client_context_t *ctx, uint16_t profile_id,
        char *apn, uint8_t pdp_type, bool pcscf_requested)
{
    wds_settings_profile_id_t input_profile = { 0 };
    wds_profile_t out_profile = { 0 };
    uint16_t sdk_error;
    int ret;

    input_profile.type = QMI_WDS_PROFILE_TYPE_3GPP;
    input_profile.index = profile_id;

    ret = tqcm_wds_get_profile_settings_send(ctx, &input_profile, &out_profile, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("%s: wds_get_profile_settings_send fail %d", __FUNCTION__, ret);
        if (ret == EPROTO) {
            RLOGE("%s: qmi protocol error 0x%04X", __FUNCTION__, sdk_error);
        }

        if (out_profile.extended_error_code_available) {
            RLOGI("%s: extended error code 0x%x ",
                    __FUNCTION__, out_profile.extended_error_code.extended_error_code);
        }

        return false;
    }

    if (!out_profile.profile_settings.pdp_type_available) {
        RLOGD("%s: %d: missing pdp_type", __FUNCTION__, __LINE__);
        return false;
    }

    if (out_profile.profile_settings.pdp_type != pdp_type) {
        RLOGD("%s: %d: pdp_type check fail", __FUNCTION__, __LINE__);
        return false;
    }

    if (apn && strlen(apn)) {
        if (!out_profile.profile_settings.apn_len) {
            RLOGD("%s: %d: apn check fail",
                    __FUNCTION__, __LINE__);
            return false;
        }
        if (strcmp(apn, out_profile.profile_settings.apn) != 0) {
            RLOGD("%s: %d: apn check fail",
                    __FUNCTION__, __LINE__);
            return false;
        }
    } else {
        if (out_profile.profile_settings.apn_len) {
            RLOGD("%s: %d: apn check fail",
                    __FUNCTION__, __LINE__);
            return false;
        }
    }

    if (out_profile.profile_settings.pcscf_addr_using_pco !=
            pcscf_requested) {
        RLOGD("%s: %d: pcscf check failed",
                __FUNCTION__, __LINE__);
        return false;
    }

    return true;
}

/**
 * @brief Function for checking if a profile is in the attach pdn list
 *
 * @param[in] ctx wds client
 * @param[in] profile_id id of the profile to be checked
 *
 * @return true if in list, false otherwise
 */
static bool is_in_attach_pdn_list(tqcm_client_context_t *ctx, uint16_t profile_id)
{
    wds_get_attach_pdn_list_t out_profile_list;
    uint16_t sdk_error;
    int ret;

    ret = tqcm_wds_get_lte_attach_pdn_list_send(ctx,
            &out_profile_list,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("%s: wds_get_lte_attach_pdn_list_send fail %d",
                __FUNCTION__, ret);
        if (ret == EPROTO)
            RLOGE("%s: qmi protocol error 0x%04X",
                    __FUNCTION__, sdk_error);
        RLOGE("%s: not able to check the attach pdn list for profile %u",
                __FUNCTION__, profile_id);
        return false;
    }

    for (int i = 0; i < out_profile_list.attach_pdn_list_size; i++) {
        if (out_profile_list.attach_pdn_list[i] == profile_id) {
            RLOGD("%s: profile %u found in the attach pdn list",
                    __FUNCTION__, profile_id);
            return true;
        }
    }

    return false;
}

/**
 * @brief Function for deleting a profile
 *
 * The profile is not deleted if it is in the attach pdn list, unless the force
 * flag is set.
 *
 * @param[in] ctx wds client
 * @param[out] out error if the operation fails
 * @param[in] force if set, delete the profile even if it is in the attach pdn list
 *
 * @return true if in list, false otherwise
 */
static int delete_profile(wds_settings_profile_id_t *input, wds_profile_ext_err_t *out,
        tqcm_client_context_t *ctx, bool force)
{
    int ret = QMI_SDK_ERR_NONE;
    uint16_t sdk_error;

    if (!is_in_attach_pdn_list(ctx, input->index) || force) {
        ret = tqcm_wds_delete_profile_send(ctx,
                input,
                out,
                &sdk_error,
                NULL,
                0);
        if (ret != 0) {
            RLOGE("%s: tqcm_wds_delete_profile_send fail %d", __FUNCTION__, ret);
            if (ret == EPROTO) {
                RLOGE("%s: qmi protocol error 0x%04X", __FUNCTION__, sdk_error);
                ret = sdk_error;
            }
        }
    } else {
        RLOGI("%s: profile %u found in the attach pdn list, not deleted",
                __FUNCTION__, input->index);
        ret = -1;
    }

    return ret;
}

/**
 * @brief Function for finding the first profile index with a specific apn
 *
 * The profile is not deleted if it is in the attach pdn list, unless the force
 * flag is set.
 *
 * @param[in] ctx wds client
 * @param[out] out error if the operation fails
 * @param[in] force if set, delete the profile even if it is in the attach pdn list
 *
 * @return -1 if not found, otherwise the profile index
 */
static int find_profile_by_apn(tqcm_client_context_t *ctx, char *apn)
{
    uint16_t sdk_error;
    int ret;
    wds_profile_list_input_t input_profile_list = { 0 };
    wds_profile_list_t out = { 0 };
    int i;

    input_profile_list.profile_type = QMI_WDS_PROFILE_TYPE_3GPP;
    input_profile_list.profile_type_available = true;
    ret = tqcm_wds_get_profile_list_send(ctx,
            &input_profile_list, &out, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("%s: wds_get_profile_list_send fail %d", __FUNCTION__, ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }

        if (out.extended_error_code_available) {
            RLOGI("%s: extended error code 0x%x ", __FUNCTION__,
                    out.extended_error_code.extended_error_code);
        }
        return -1;
    }

    for (i = 0; i < out.profile_list_size; i++) {
        wds_settings_profile_id_t input_profile = { 0 };
        wds_profile_t out_profile = { 0 };

        input_profile.type = QMI_WDS_PROFILE_TYPE_3GPP;
        input_profile.index = out.profile_list[i].profile_index;

        ret = tqcm_wds_get_profile_settings_send(ctx, &input_profile, &out_profile, &sdk_error, NULL, 0);
        if (ret != 0) {
            RLOGE("%s: wds_get_profile_settings_send fail %d", __FUNCTION__, ret);
            if (ret == EPROTO)
                RLOGE("%s: qmi protocol error 0x%04X", __FUNCTION__, sdk_error);

            if (out_profile.extended_error_code_available)
                RLOGI("%s: extended error code 0x%x ",
                        __FUNCTION__, out_profile.extended_error_code.extended_error_code);

            return -1;
        }

        if (apn && strlen(apn)) {
            if (!out_profile.profile_settings.apn_len) {
                RLOGD("%s: %d: apn check fail",
                        __FUNCTION__, __LINE__);
                continue;
            }
            if (strcmp(apn, out_profile.profile_settings.apn) != 0) {
                RLOGD("%s: %d: apn check fail",
                        __FUNCTION__, __LINE__);
                continue;
            }
        } else {
            if (out_profile.profile_settings.apn_len) {
                RLOGD("%s: %d: apn check fail",
                        __FUNCTION__, __LINE__);
                continue;
            }
        }
        break;
    }

    if (i == out.profile_list_size)
        return -1;

    return out.profile_list[i].profile_index;
}

/**
 * @brief Function for creating a new profile
 *
 * @param[in] input settings of the profile
 * @param[out] out result of the profile creation
 * @param[in] ctx wds client
 *
 * @return 0 on success, -1 otherwise
 */
static int create_profile(wds_profile_t *input, wds_profile_resp_t *out,
        tqcm_client_context_t *ctx)
{
    int ret;
    uint16_t sdk_error;

retry:
    ret = tqcm_wds_create_profile_send(ctx,
            input,
            out,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("%s: tqcm_wds_create_profile_send fail %d", __FUNCTION__, ret);
        if (ret == EPROTO) {
            RLOGE("%s: qmi protocol error 0x%04X", __FUNCTION__, sdk_error);
            ret = sdk_error;
        }

        if (out->extended_error_code_available) {
            RLOGE("%s: profile creation failed with reason %u",
                    __FUNCTION__, out->extended_error_code);
            if (out->extended_error_code == 18) {
                /* 18 is REG_RESULT_ERR_OUT_OF_PROFILES: we are going to delete
                 * one profile (the number is not important), but it should not
                 * be the attach profile: let's start from profile 4 */
                uint8_t profile_index_to_be_deleted = 4;
                wds_settings_profile_id_t input_delete_profile = {0};
                wds_profile_ext_err_t out_delete_profile;

                RLOGD("%s: trying to delete a profile %u",
                        __FUNCTION__, profile_index_to_be_deleted);

retry_delete:
                input_delete_profile.type = QMI_WDS_PROFILE_TYPE_3GPP;
                input_delete_profile.index = profile_index_to_be_deleted;

                if (delete_profile(&input_delete_profile, &out_delete_profile, ctx, false)) {
                    RLOGE("%s: unable to delete profile index [%u], profiles proliferation possible",
                            __FUNCTION__, profile_index_to_be_deleted);
                    if (profile_index_to_be_deleted++ < 10)
                        goto retry_delete;

                    ret = -1;
                    RLOGE("%s: bailing out", __FUNCTION__);
                    goto end;
                }

                RLOGI("%s: profile %u deleted, retrying creation",
                        __FUNCTION__, profile_index_to_be_deleted);
                /* Deleted, we can retry the profile creation */
                memset(out, 0, sizeof(*out));
                goto retry;
            }

            if (out->extended_error_code == 20) {
                /* extended_error_code == 20 is not available in any document we have,
                 * but it seems to show when trying to create a profile that is "almost"
                 * the same than another one (e.g. same apn, but different pcscf flag)
                 * Most of the modems do not show this issue, but SDX35 does
                 * So, we should delete the previous profile and the only meaningful
                 * criterion to find that is the apn */
                int profile_index_to_be_deleted;
                wds_settings_profile_id_t input_delete_profile = {0};
                wds_profile_ext_err_t out_delete_profile;

                profile_index_to_be_deleted = find_profile_by_apn(ctx, input->profile_settings.apn);
                if (profile_index_to_be_deleted == -1) {
                    RLOGE("%s: unable to find a similar previous profile to be deleted",
                            __FUNCTION__);
                    ret = -1;
                    goto end;
                }

                input_delete_profile.type = QMI_WDS_PROFILE_TYPE_3GPP;
                input_delete_profile.index = profile_index_to_be_deleted;

                if (delete_profile(&input_delete_profile, &out_delete_profile, ctx, true)) {
                    RLOGE("%s: unable to delete profile index [%u]",
                            __FUNCTION__, profile_index_to_be_deleted);
                    ret = -1;
                    goto end;
                }

                RLOGI("%s: profile %u deleted, retrying creation",
                        __FUNCTION__, profile_index_to_be_deleted);
                memset(out, 0, sizeof(*out));
                goto retry;
            }
        }
    }

end:
    return ret;
}

/**
 * @brief Function for getting the index of the first profile with the required settings
 *
 * @param[in] apn apn to be checked
 * @param[in] ctx wds client
 * @param[in] ip_type ip family type, see #QMI_WDS_PDP_type_t
 * @param[in] is_ims pcscf through pco flag to be checked
 *
 * @return -1 if not found, otherwise the profile index
 */
int find_profile_index(char *apn, tqcm_client_context_t *ctx,
        QMI_WDS_PDP_type_t ip_type, bool is_ims)
{
    int index = -1;
    int ret;
    wds_profile_list_input_t input_profile_list = {0};
    wds_profile_list_t out;
    uint16_t sdk_error;

    input_profile_list.profile_type = QMI_WDS_PROFILE_TYPE_3GPP;
    input_profile_list.profile_type_available = true;

    ret = tqcm_wds_get_profile_list_send(ctx, &input_profile_list,
            &out, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("%s: wds_get_profile_list_send fail %d", __FUNCTION__, ret);
        if (ret == EPROTO) {
            RLOGE("%s: qmi protocol error 0x%04X", __FUNCTION__, sdk_error);
        }

        if (out.extended_error_code_available) {
            RLOGI("%s: extended error code 0x%x ",
                    __FUNCTION__,
                    out.extended_error_code.extended_error_code);
        }
    } else {
        for (int i = 0; i < out.profile_list_size; i++) {
            if (is_profile_matched(ctx, out.profile_list[i].profile_index,
                    apn, ip_type, is_ims)) {
                index = out.profile_list[i].profile_index;
                RLOGI("%s: profile id %d already available", __FUNCTION__, index);
                break;
            }
        }
    }

    if (index == -1) {
        wds_profile_t input_profile = {0};
        wds_profile_resp_t out_profile;

        input_profile.profile_settings_available = true;
        input_profile.profile_id_available = true;
        input_profile.profile_id.type = QMI_WDS_PROFILE_TYPE_3GPP;
        input_profile.profile_settings.apn_len = strlen(apn);
        strlcpy(input_profile.profile_settings.apn, apn, sizeof(input_profile.profile_settings.apn));
        input_profile.profile_settings.pdp_type_available = true;
        input_profile.profile_settings.pdp_type = QMI_WDS_PDP_TYPE_IPV4V6;
        if (is_ims)
            input_profile.profile_settings.pcscf_addr_using_pco = true;

        if (create_profile(&input_profile, &out_profile, ctx)) {
            RLOGE("%s: unable to create profile, data connection aborted", __FUNCTION__);
            goto end;
        }
        index = out_profile.profile_id.index;
        RLOGI("%s: new profile %d created", __FUNCTION__, index);
    }

end:
    return index;
}

/**
 * @brief Function for getting the index of the first profile with the required settings
 *
 * @param[in] mux_id qmap id to be bound to the wds client
 * @param[in] ctx wds client
 *
 * @return -1 if not found, otherwise the profile index
 */
int bind_mux(uint8_t mux_id, tqcm_client_context_t *ctx)
{
    int ret;
    uint16_t sdk_error;
    wds_bind_mux_data_port_t input = {0};

    input.peripheral_endpoint_available = true;
    input.peripheral_endpoint.ep_type = QMI_WDS_DATA_EP_TYPE_HSUSB;
    input.peripheral_endpoint.iface_id = RMNET_INTERFACE_ID;
    input.mux_id_available = true;
    input.mux_id = mux_id;

    ret = tqcm_wds_bind_mux_data_port_send(ctx,
            &input,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("%s: tqcm_wds_bind_mux_data_port_send fail %d", __FUNCTION__, ret);
        if (ret == EPROTO) {
            RLOGE("%s: qmi protocol error 0x%04X", __FUNCTION__, sdk_error);
            ret = sdk_error;
        }
    }

    return ret;
}

/**
 * @brief Function for starting a data call
 *
 * @param[in] ctx wds client
 * @param[in] input call settings
 * @param[out] input call operation response
 *
 * @return 0 on success, otherwise error
 */
static int setup_data_call(tqcm_client_context_t *ctx,
        wds_start_network_info_t *input,
        wds_start_network_resp_t *out)
{
    uint16_t sdk_error;
    int ret;

    ret = tqcm_wds_start_network_interface_send(ctx,
            input,
            out,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("%s: tqcm_wds_start_network_interface_send fail %d", __FUNCTION__, ret);
        if (ret == EPROTO) {
            RLOGE("%s: qmi protocol error 0x%04X", __FUNCTION__, sdk_error);

        }
        if (out->call_end_reason_available) {
            RLOGE("%s: call end reason %u", __FUNCTION__,
                    out->call_end_reason);
        }

        if (out->verbose_call_end_reason_available) {
            RLOGE("%s: verbose call end reason type %u, code %u", __FUNCTION__,
                    out->verbose_call_end_reason.call_end_reason_type,
                    out->verbose_call_end_reason.call_end_reason);
        }
    }

    return ret;
}

/**
 * @brief Function for setting-up a data call
 *
 * @param[in] apn apn for the call
 * @param[in] ctx wds client
 * @param[in] ip_type ip family for the call, see #QMI_ip_family_t
 * @param[out] pkt_data_handle handle of the call
 * @param[in] is_ims true for enabling pcscf through pco flag
 *
 * @return true on success, false on error
 */
bool setup_data_connection(char *apn, tqcm_client_context_t *ctx,
        uint32_t *pkt_data_handle, bool is_ims)
{
    wds_start_network_info_t input = {0};
    wds_start_network_resp_t out;
    int index;

    /* Check if there's already a profile with the required settings,
     * if not find_profile_index will create a new one */
    index = find_profile_index(apn, ctx, QMI_WDS_PDP_TYPE_IPV4V6, is_ims);
    if (index == -1) {
        RLOGE("%s: unable to retrieve or create profile", __FUNCTION__);
        return false;
    }
    /* Start data connection */
    input.profile_index_3gpp_available = true;
    input.profile_index_3gpp = index;
    if (setup_data_call(ctx, &input, &out)) {
        RLOGE("%s: unable to setup data call", __FUNCTION__);
        return false;
    }
    if (out.pkt_data_handle_available) {
        *pkt_data_handle = out.pkt_data_handle;
        return true;
    }

    RLOGE("%s: packet data handle not available", __FUNCTION__);
    return false;
}

/**
 * @brief Function for terminating a data call
 *
 * @param[in] ctx wds client
 * @param[in] pkt_data_handle handle of the call to be terminated
 *
 * @return true on success, false on error
 */
static int terminate_data_connection(tqcm_client_context_t *ctx,
        uint32_t *pkt_data_handle)
{
    int ret = 0;
    uint16_t sdk_error;
    wds_stop_network_info_t input = {0};

    if (pkt_data_handle && *pkt_data_handle) {
        input.pkt_data_handle = *pkt_data_handle;

        ret = tqcm_wds_stop_network_interface_send(ctx,
                &input,
                &sdk_error,
                NULL,
                0);
        if (ret != 0) {
            RLOGE("%s: tqcm_wds_stop_network_interface_send fail %d", __FUNCTION__, ret);
            if (ret == EPROTO) {
                RLOGE("%s: qmi protocol error 0x%04X", __FUNCTION__, sdk_error);
            }
        }

        *pkt_data_handle = 0;
    }

    return ret;
}

/**
 * @brief Function for getting the settings of an ongoing data call
 *
 * @param[in] ctx wds client
 * @param[in] out settings of the data call
 *
 * @return true on success, false on error
 */
static int get_data_call_response(tqcm_client_context_t *ctx,
        wds_profile_t *out)
{
    int ret;
    wds_runtime_settings_info_t input = {0};
    uint16_t sdk_error;

    input.requested_settings = (QMI_WDS_SETTING_PDP_TYPE |
            QMI_WDS_SETTING_DNS_ADDRESS |
            QMI_WDS_SETTING_IP_ADDRESS |
            QMI_WDS_SETTING_GATEWAY_INFO |
            QMI_WDS_SETTING_PCSCF_ADDRESS |
            QMI_WDS_SETTING_MTU |
            QMI_WDS_SETTING_PCSCF_SERVER_ADDRESS_LIST);

    ret = tqcm_wds_get_runtime_settings_send(ctx,
            &input,
            out,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("%s: tqcm_wds_get_runtime_settings_send fail %d", __FUNCTION__, ret);
    }

    return ret;
}

/**
 * @brief Function for configuring the netdevice according to the ongoing data calls
 *
 * @param[in] netdevice netdevice name
 * @param[in] ctx wds client in which the ipv4 data call is ongoing (can be NULL)
 * @param[in] ctx_ipv6 wds client in which the ipv6 data call is ongoing (can be NULL)
 *
 * @return true on success, false on error
 */
bool configure_netdevice(char *netdevice,
        tqcm_client_context_t *ctx,
        tqcm_client_context_t *ctx_ipv6)
{
    bool ret = false;
    wds_profile_t data_call;
    uint32_t mtu = 0;
    uint32_t mtu_ipv6 = 0;

    if (ctx) {
        if (get_data_call_response(ctx, &data_call)) {
            RLOGE("%s: get_data_call_response ipv4", __FUNCTION__);
        } else {
            print_get_runtime_response(&data_call);
            if (data_call.profile_settings_available &&
                    data_call.profile_settings.ipv4_addr_pref_available &&
                    data_call.profile_settings.ipv4_subnet_mask_available) {
                char *ipv4 = NULL;
                char *netmask = NULL;

                if (asprintf(&ipv4, "%u.%u.%u.%u",
                        (data_call.profile_settings.ipv4_addr_pref >> 24) & 0xFF,
                        (data_call.profile_settings.ipv4_addr_pref >> 16) & 0xFF,
                        (data_call.profile_settings.ipv4_addr_pref >> 8) & 0xFF,
                        data_call.profile_settings.ipv4_addr_pref & 0xFF) <= 0) {
                    RLOGE("%s: failed asprintf", __FUNCTION__);
                    goto end;
                }
                if (asprintf(&netmask, "%u.%u.%u.%u",
                        (data_call.profile_settings.ipv4_subnet_mask_addr >> 24) & 0xFF,
                        (data_call.profile_settings.ipv4_subnet_mask_addr >> 16) & 0xFF,
                        (data_call.profile_settings.ipv4_subnet_mask_addr >> 8) & 0xFF,
                        data_call.profile_settings.ipv4_subnet_mask_addr & 0xFF) <= 0) {
                    RLOGE("%s: failed asprintf", __FUNCTION__);
                }

                if (ipv4 && netmask) {
                    if (!configure_ipv4_address(netdevice, ipv4, netmask)) {
                        RLOGE("%s: failed to configure netdevice", __FUNCTION__);
                    } else {
                        if (data_call.profile_settings.mtu_available) {
                            mtu = data_call.profile_settings.mtu;
                        } else {
                            RLOGW("%s: missing MTU, defaulting to 1500", __FUNCTION__);
                            mtu = 1500;
                        }

                        ret = true;
                    }
                }

                free(ipv4);
                free(netmask);
            }
            memset(&data_call, 0, sizeof(data_call));
        }
    }
    if (ctx_ipv6) {
        if (get_data_call_response(ctx_ipv6, &data_call)) {
            RLOGE("%s: get_data_call_response ipv6", __FUNCTION__);
        } else {
            print_get_runtime_response(&data_call);
            if (data_call.profile_settings_available &&
                    data_call.profile_settings.ipv6_addr_available) {
                char *ipv6 = NULL;

                if (asprintf(&ipv6, "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
                        ntohs(data_call.profile_settings.ipv6_addr.addr[0]),
                        ntohs(data_call.profile_settings.ipv6_addr.addr[1]),
                        ntohs(data_call.profile_settings.ipv6_addr.addr[2]),
                        ntohs(data_call.profile_settings.ipv6_addr.addr[3]),
                        ntohs(data_call.profile_settings.ipv6_addr.addr[4]),
                        ntohs(data_call.profile_settings.ipv6_addr.addr[5]),
                        ntohs(data_call.profile_settings.ipv6_addr.addr[6]),
                        ntohs(data_call.profile_settings.ipv6_addr.addr[7])) <= 0) {
                    RLOGE("%s: failed asprintf", __FUNCTION__);
                    goto end;
                }

                if (ipv6) {
                    if (!configure_ipv6_address(netdevice, ipv6,
                            data_call.profile_settings.ipv6_addr.prefix_len)) {
                        RLOGE("%s: failed to configure netdevice", __FUNCTION__);
                    } else {
                        if (data_call.profile_settings.mtu_available) {
                            mtu_ipv6 = data_call.profile_settings.mtu;
                        } else {
                            RLOGW("%s: missing MTU, defaulting to 1500", __FUNCTION__);
                            mtu = 1500;
                        }

                        ret = true;
                    }
                    free(ipv6);
                }
            }
        }
    }

    if (ret) {
        uint32_t max_mtu;
        int sock_fd;
        struct ifreq ifr;

        max_mtu = mtu > mtu_ipv6 ? mtu : mtu_ipv6;
        strncpy(ifr.ifr_name, netdevice, IFNAMSIZ);
        ifr.ifr_mtu = max_mtu;

        sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (sock_fd != -1) {
            if (ioctl(sock_fd, SIOCSIFMTU, &ifr) < 0) {
                RLOGE("%s: failed SIOCSIFMTU, %s", __FUNCTION__, strerror(errno));
                ret = false;
            }
            close(sock_fd);
        } else {
            RLOGE("%s: unable to set mtu", __FUNCTION__);
            ret = false;
        }

        if (!configure_network_interface_state(NETDEVICE_UP, netdevice)) {
            RLOGE("Unable to bring up netdevice %s", netdevice);
            ret = false;
        }
    }

end:
    return ret;
}

char call_status[][32] = {
    "",
    "Disconnected",
    "Connected",
    "Suspended",
    "Authenticating"
};

/**
 * @brief Function for checking the data call status
 *
 * @param[in] ctx wds client where to check the data call status
 * @param[in] name name of the netdevice where to check the data call status
 *
 * @return true on success, false on error
 */
int check_data_call_status(tqcm_client_context_t *ctx, char *name)
{
    int ret;
    wds_get_packet_service_status_t status;
    uint16_t sdk_error;

    ret = tqcm_wds_get_pkt_srvc_status_send(ctx, &status, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("%s: tqcm_wds_get_runtime_settings_send fail %d", __FUNCTION__, ret);
    } else {
        RLOGI("Call %s status: %s", name, call_status[status.conn_status]);
    }

    return ret;
}

/** @END Modem QMI management functions */

/** @brief example of callback function executed when a wds client receives an indication
 *
 * This callback is executed whenever an indication arrives in the wds client.
 * The example currently manages QMI_WDS_PKT_SRVC_STATUS_IND, which provides the
 * status of the data connection ongoing in the wds channel, but can be extened
 * to manage all the indications implemented in telit_wds_indication_handler.
 *
 * NOTE: for simpleness the same callback function is used for all the clients,
 * but clients can have different callbacks, implementing different behaviors.
 * Using the same callback does not cause reentrancy issues, since it can't be
 * called concurrently.
 *
 * NOTE: the callback should execute as quickly as possible, leaving long standing
 * jobs to workers.
 *
 * ATTENTION: the callback should be customized according to the use-case.
 *
 * @param[in] name indication
 */
void wds_indications(void *item)
{
    int ret;
    indication_item_t *ind = (indication_item_t *)item;
    static uint8_t out[QMI_IND_BUFFER_SIZE] = {0};

    ret = telit_wds_indication_handler(ind->msg_id,
            ind->buf, ind->buf_len,
            (uint8_t *)&out, sizeof(out));
    if (ret != 0) {
        RLOGE("%s: unpack fail %d ", __FUNCTION__, ret);
        goto end;
    }

    switch (ind->msg_id) {
        case QMI_WDS_PKT_SRVC_STATUS_IND: {
            wds_ind_session_state_t *session_state;

            session_state = (wds_ind_session_state_t *)out;
            RLOGI("%s: connection status = %u", __FUNCTION__,
                    session_state->connection_status);
            RLOGI("%s: reconfiguration required = %u", __FUNCTION__,
                    session_state->reconfiguration_required);
            if (session_state->call_end_reason_available)
                RLOGI("%s: call end reason = %u", __FUNCTION__,
                        session_state->call_end_reason);
            if (session_state->call_end_reason_verbose_available) {
                RLOGI("%s: call end type = %u", __FUNCTION__,
                        session_state->call_end_reason_verbose.call_end_reason_type);
                RLOGI("%s: call end reason = %u", __FUNCTION__,
                        session_state->call_end_reason_verbose.call_end_reason);
            }
            break;
        }
        default:
            RLOGI("%s: indication 0x%x not managed", __FUNCTION__, ind->msg_id);
            break;
    }

end:
    free(ind->buf);
    free(ind);
}

/** @brief function for opening all the wds required clients for three ipv4v6
 * data calls and one ipv4 data call
 *
 * Each data call type requires a dedicated wds client
 *
 */
bool wds_clients_init()
{
    if (tqcm_init_client_context(QMI_SVC_WDS,
            &ctx,
            &ctx_wds_pdn_default,
            wds_indications) ||
            tqcm_init_client_context(QMI_SVC_WDS,
            &ctx,
            &ctx_wds_pdn_default_ipv6,
            wds_indications) ||
            tqcm_init_client_context(QMI_SVC_WDS,
            &ctx,
            &ctx_wds_pdn_ims,
            wds_indications) ||
            tqcm_init_client_context(QMI_SVC_WDS,
            &ctx,
            &ctx_wds_pdn_ims_ipv6,
            wds_indications) ||
            tqcm_init_client_context(QMI_SVC_WDS,
            &ctx,
            &ctx_wds_pdn_admin,
            wds_indications) ||
            tqcm_init_client_context(QMI_SVC_WDS,
            &ctx,
            &ctx_wds_pdn_admin_ipv6,
            wds_indications) ||
            tqcm_init_client_context(QMI_SVC_WDS,
            &ctx,
            &ctx_wds_pdn_mms,
            wds_indications)) {
        RLOGE("%s: failed WDS contexts opening", __FUNCTION__);
        return false;
    }

    return true;
}

bool wds_client_deinit()
{
    if (tqcm_deinit_client_context(&ctx, &ctx_wds_pdn_default) ||
            tqcm_deinit_client_context(&ctx, &ctx_wds_pdn_default_ipv6) ||
            tqcm_deinit_client_context(&ctx, &ctx_wds_pdn_ims) ||
            tqcm_deinit_client_context(&ctx, &ctx_wds_pdn_ims_ipv6) ||
            tqcm_deinit_client_context(&ctx, &ctx_wds_pdn_admin) ||
            tqcm_deinit_client_context(&ctx, &ctx_wds_pdn_admin_ipv6) ||
            tqcm_deinit_client_context(&ctx, &ctx_wds_pdn_mms)) {
        RLOGE("%s: failed WDS contexts disposal", __FUNCTION__);
        return false;
    }

    return true;
}

/** @brief function for binding the qmap channels to the open wds clients
 *
 * Data calls with the same apn but different ip type share the same qmap
 * channel.
 *
 */
bool wds_clients_bind_qmap_channel()
{
    if (bind_mux(QMAP_ID_DEFAULT, &ctx_wds_pdn_default) ||
            bind_mux(QMAP_ID_DEFAULT, &ctx_wds_pdn_default_ipv6) ||
            bind_mux(QMAP_ID_IMS, &ctx_wds_pdn_ims) ||
            bind_mux(QMAP_ID_IMS, &ctx_wds_pdn_ims_ipv6) ||
            bind_mux(QMAP_ID_ADMIN, &ctx_wds_pdn_admin) ||
            bind_mux(QMAP_ID_ADMIN, &ctx_wds_pdn_admin_ipv6) ||
            bind_mux(QMAP_ID_MMS, &ctx_wds_pdn_mms)) {
        RLOGE("%s: failed qmap binding", __FUNCTION__);
        return false;
    }

    return true;
}

/** @brief function for setting the ip family to the various wds clients
 *
 * Once set the ip family type, only calls with that specific ip type can
 * be performed in that client.
 *
 */
bool wds_clients_ip_family_set()
{
    if (set_ip_family(QMI_IP_FAMILY_V4, &ctx_wds_pdn_default) ||
            set_ip_family(QMI_IP_FAMILY_V6, &ctx_wds_pdn_default_ipv6) ||
            set_ip_family(QMI_IP_FAMILY_V4, &ctx_wds_pdn_ims) ||
            set_ip_family(QMI_IP_FAMILY_V6, &ctx_wds_pdn_ims_ipv6) ||
            set_ip_family(QMI_IP_FAMILY_V4, &ctx_wds_pdn_admin) ||
            set_ip_family(QMI_IP_FAMILY_V6, &ctx_wds_pdn_admin_ipv6) ||
            set_ip_family(QMI_IP_FAMILY_V4, &ctx_wds_pdn_mms)) {
        RLOGE("%s: failed ip family setting", __FUNCTION__);
        return false;
    }

    return true;
}

/** @brief example of callback function executed when the SDK detects an unrecoverable
 * error on the device or its closure (e.g. due to a modem reboot).
 *
 * NOTE: the callback should execute as quickly as possible, leaving long standing
 * jobs to workers.
 *
 * ATTENTION: the callback should be customized according to the use-case.
 */
static int on_device_closed_or_error(void *item __unused)
{
    RLOGE("%s: QMI channel closed", __FUNCTION__);
    data_stop = 1;
    sem_post(&loop_sem);

    return 0;
}

/** @brief SIGINT handler
 *
 * NOTE: the handler should execute as quickly as possible, leaving long standing
 * jobs to workers.
 *
 * ATTENTION: the handler should be customized according to the use-case.
 */
static void ctrl_c_handler(int sig __unused)
{
    data_stop = 1;
    sem_post(&loop_sem);
}

int main(int argc __unused, char **argv __unused)
{
    int ret = EXIT_SUCCESS;
    struct sigaction act = {0};
    uint32_t rx_urb_size = 0;

    if (sem_init(&loop_sem, 0, 0)) {
        RLOGE("Failed semaphore initialize");
        exit(EXIT_FAILURE);
    }
    act.sa_handler = ctrl_c_handler;
    sigaction(SIGINT, &act, NULL);

    /* tqcm initialization */
    ret = tqcm_init(MODEM_CONTROL_PATH, &ctx);
    if (ret != 0) {
        RLOGE("Failed opening control channel (ret = %d)", ret);
        sem_destroy(&loop_sem);
        exit(EXIT_FAILURE);
    }
    /* Setting SDK error or closure callback */
    ret = tqcm_set_error_cb(&ctx, on_device_closed_or_error);

    /* Bring down wwan parent netdevice, for being able to create the qmap netdevices */
    if (!configure_network_interface_state(NETDEVICE_DOWN, WWAN_NETDEVICE_NAME)) {
        RLOGE("Unable to bring down netdevice %s", WWAN_NETDEVICE_NAME);
        ret = EXIT_FAILURE;
        goto bail_out;
    }
    /* Create qmap netdevice */
    if (create_qmap_netdevices()) {
        RLOGE("Unable to create rmnet interfaces");
        ret = EXIT_FAILURE;
        goto bail_out;
    }
    /* Configure modem qmap setings */
    if (configure_modem_data_format(&rx_urb_size)) {
        RLOGE("Unable to configure modem data format");
        ret = EXIT_FAILURE;
        goto end;
    }
    /* Configure wwan MTU according to the maximum rx aggregated size, in order
     * to change also the rx usb size */
    if (!configure_network_interface_mtu(rx_urb_size, WWAN_NETDEVICE_NAME)) {
        RLOGE("Unable to configure rmnet interface mtu %d", rx_urb_size);
        ret = EXIT_FAILURE;
        goto end;
    }
    /* Switch wwan netdevice to raw_ip and enable passthrough for the kernel
     * rmnet module */
    if (configure_qmi_wwan_raw_ip(1, WWAN_NETDEVICE_NAME)) {
        RLOGE("Unable to configure raw_ip mode");
        ret = EXIT_FAILURE;
        goto end;
    }
    /* Set wwan netdevice link up */
    if (!configure_network_interface_state(NETDEVICE_UP, WWAN_NETDEVICE_NAME)) {
        RLOGE("Unable to bring up netdevice %s", WWAN_NETDEVICE_NAME);
        ret = EXIT_FAILURE;
        goto end;
    }
    /* WDS clients initialization */
    if (!wds_clients_init()) {
        RLOGE("Unable to initialize wds clients");
        ret = EXIT_FAILURE;
        goto end;
    }
    /* qmap channel binding to WDS clients */
    if (!wds_clients_bind_qmap_channel()) {
        RLOGE("Unable to initialize bind qmap channel to wds clients");
        ret = EXIT_FAILURE;
        goto end;
    }
    /* ip family setting to WDS clients */
    if (!wds_clients_ip_family_set()) {
        RLOGE("Unable to set the ip family type to wds clients");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGI("*********************************************");
    RLOGI("* APN: %s, data call setup", APN_DEFAULT);
    RLOGI("*********************************************");
    /* Start ipv4 data call */
    if (!setup_data_connection(APN_DEFAULT, &ctx_wds_pdn_default,
            &pkt_data_handle_default, false))
        RLOGW("Failed apn = %s, QMI_IP_FAMILY_V4 data connection", APN_DEFAULT);
    /* Start ipv6 data call */
    if (!setup_data_connection(APN_DEFAULT, &ctx_wds_pdn_default_ipv6,
            &pkt_data_handle_default_ipv6, false))
        RLOGW("Failed apn = %s, QMI_IP_FAMILY_V6 data connection", APN_DEFAULT);
    /* Configure netdevice */
    if (pkt_data_handle_default || pkt_data_handle_default_ipv6)
        if (!configure_netdevice(QMAP_IF_NAME_DEFAULT,
                pkt_data_handle_default ? &ctx_wds_pdn_default : NULL,
                pkt_data_handle_default_ipv6 ? &ctx_wds_pdn_default_ipv6 : NULL))
            RLOGE("Failed configuring %s netdevice", QMAP_IF_NAME_DEFAULT);
    RLOGI("*********************************************");
    RLOGI("* APN: %s, data call setup", APN_IMS);
    RLOGI("*********************************************");
    if (!setup_data_connection(APN_IMS, &ctx_wds_pdn_ims,
            &pkt_data_handle_ims, true))
        RLOGW("Failed apn = %s, QMI_IP_FAMILY_V4 data connection", APN_IMS);
    if (!setup_data_connection(APN_IMS, &ctx_wds_pdn_ims_ipv6,
            &pkt_data_handle_ims_ipv6, true))
        RLOGW("Failed apn = %s, QMI_IP_FAMILY_V6 data connection", APN_IMS);
    if (pkt_data_handle_ims || pkt_data_handle_ims_ipv6)
        if (!configure_netdevice(QMAP_IF_NAME_IMS,
                pkt_data_handle_ims ? &ctx_wds_pdn_ims : NULL,
                pkt_data_handle_ims_ipv6 ? &ctx_wds_pdn_ims_ipv6 : NULL))
            RLOGE("Failed configuring %s netdevice", QMAP_IF_NAME_IMS);
    RLOGI("*********************************************");
    RLOGI("* APN: %s, data call setup", APN_ADMIN);
    RLOGI("*********************************************");
    if (!setup_data_connection(APN_ADMIN, &ctx_wds_pdn_admin,
            &pkt_data_handle_admin, true))
        RLOGW("Failed apn = %s, QMI_IP_FAMILY_V4 data connection", APN_ADMIN);
    if (!setup_data_connection(APN_ADMIN, &ctx_wds_pdn_admin_ipv6,
            &pkt_data_handle_admin_ipv6, true))
        RLOGW("Failed apn = %s, QMI_IP_FAMILY_V6 data connection", APN_ADMIN);
    if (pkt_data_handle_admin || pkt_data_handle_admin_ipv6)
        if (!configure_netdevice(QMAP_IF_NAME_ADMIN,
                pkt_data_handle_admin ? &ctx_wds_pdn_admin : NULL,
                pkt_data_handle_admin_ipv6 ? &ctx_wds_pdn_admin_ipv6 : NULL))
            RLOGE("Failed configuring %s netdevice", QMAP_IF_NAME_ADMIN);
    RLOGI("*********************************************");
    RLOGI("* APN: %s, data call setup", APN_MMS);
    RLOGI("*********************************************");
    if (!setup_data_connection(APN_MMS, &ctx_wds_pdn_mms,
            &pkt_data_handle_mms, true))
        RLOGW("Failed apn = %s, QMI_IP_FAMILY_V4 data connection", APN_MMS);
    if (pkt_data_handle_mms)
        if (!configure_netdevice(QMAP_IF_NAME_MMS,
                &ctx_wds_pdn_mms, NULL))
            RLOGE("Failed configuring %s netdevice", QMAP_IF_NAME_MMS);

    /* Check that at least one call has been successful */
    if (pkt_data_handle_default || pkt_data_handle_default_ipv6 ||
            pkt_data_handle_ims || pkt_data_handle_ims_ipv6 ||
            pkt_data_handle_admin || pkt_data_handle_admin_ipv6 ||
            pkt_data_handle_mms) {
        RLOGI("******************************************************");
        RLOGI("* Call setup done, send SIGINT (ctrl + c) to terminate");
        RLOGI("******************************************************");
        /* Wait user control to stop */
        while (!data_stop) {
            if (sem_wait(&loop_sem)) {
                if (errno != EINTR) {
                    RLOGE("Failed semaphore wait %s", strerror(errno));
                    ret = EXIT_FAILURE;
                    goto end;
                }
            }
            /* Check the status of the various data calls */
            check_data_call_status(&ctx_wds_pdn_default, "default IPV4");
            check_data_call_status(&ctx_wds_pdn_default_ipv6, "default IPV6");
            check_data_call_status(&ctx_wds_pdn_ims, "ims IPV4");
            check_data_call_status(&ctx_wds_pdn_ims_ipv6, "ims IPV6");
            check_data_call_status(&ctx_wds_pdn_admin, "admin IPV4");
            check_data_call_status(&ctx_wds_pdn_admin_ipv6, "admin IPV6");
            check_data_call_status(&ctx_wds_pdn_mms, "mms IPV4");
        }

        if (terminate_data_connection(&ctx_wds_pdn_default, &pkt_data_handle_default))
            RLOGE("Unable to terminate data call with handle %u", pkt_data_handle_default);
        if (terminate_data_connection(&ctx_wds_pdn_default_ipv6, &pkt_data_handle_default_ipv6))
            RLOGE("Unable to terminate data call with handle %u", pkt_data_handle_default_ipv6);

        if (terminate_data_connection(&ctx_wds_pdn_ims, &pkt_data_handle_ims))
            RLOGE("Unable to terminate data call with handle %u", pkt_data_handle_ims);
        if (terminate_data_connection(&ctx_wds_pdn_ims_ipv6, &pkt_data_handle_ims_ipv6))
            RLOGE("Unable to terminate data call with handle %u", pkt_data_handle_ims_ipv6);

        if (terminate_data_connection(&ctx_wds_pdn_admin, &pkt_data_handle_admin))
            RLOGE("Unable to terminate data call with handle %u", pkt_data_handle_admin);
        if (terminate_data_connection(&ctx_wds_pdn_admin_ipv6, &pkt_data_handle_admin_ipv6))
            RLOGE("Unable to terminate data call with handle %u", pkt_data_handle_admin_ipv6);

        if (terminate_data_connection(&ctx_wds_pdn_mms, &pkt_data_handle_mms))
            RLOGE("Unable to terminate data call with handle %u", pkt_data_handle_mms);
    } else {
        RLOGE("Unable to start any call");
    }

end:
    RLOGI("Closing app");
    if (!wds_client_deinit()) {
        RLOGE("Failed context wds context disposal");
        ret = EXIT_FAILURE;
    } else {
        /* Give some time to properly dispose the channels */
        sleep(1);
    }
    /* Dispose qmap netdevices */
    if (dispose_qmap_netdevices()) {
        RLOGE("Unable to delete rmnet interfaces");
        ret = EXIT_FAILURE;
    }
    /* Bring down wwan parent netdevice */
    if (!configure_network_interface_state(NETDEVICE_DOWN, WWAN_NETDEVICE_NAME)) {
        RLOGE("Unable to bring down netdevice %s", WWAN_NETDEVICE_NAME);
        ret = EXIT_FAILURE;
    }
    /* Disable passthrough and raw-ip*/
    if (configure_qmi_wwan_raw_ip(0, WWAN_NETDEVICE_NAME)) {
        RLOGE("Unable to configure raw_ip mode");
        ret = EXIT_FAILURE;
    }
bail_out:
    sem_destroy(&loop_sem);
    /* SDK deinitialization */
    tqcm_deinit(&ctx);
    exit(ret);
}
