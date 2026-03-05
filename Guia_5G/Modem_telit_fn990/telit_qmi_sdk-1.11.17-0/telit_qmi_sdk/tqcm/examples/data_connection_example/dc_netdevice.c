#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <telit_sdk.h>

#include "telit_data_connection.h"
#include "dc_utils.h"

uint8_t netdevice_get_ipv4_prefix_from_mask(uint32_t mask)
{
    uint8_t prefix;

    /* See https://www.ietf.org/rfc/rfc1878.txt */
    switch ((mask >> 8) & 0xFF) {
        case 255:
            switch (mask & 0xFF) {
                case 254:
                    prefix = 31;
                    break;
                case 252:
                    prefix = 30;
                    break;
                case 248:
                    prefix = 29;
                    break;
                case 240:
                    prefix = 28;
                    break;
                case 224:
                    prefix = 27;
                    break;
                case 192:
                    prefix = 26;
                    break;
                case 128:
                    prefix = 25;
                    break;
                case 0:
                    prefix = 24;
                    break;
                case 255:
                default:
                    prefix = 32;
                    break;
            }
            break;
        case 254:
            prefix = 23;
            break;
        case 252:
            prefix = 22;
            break;
        case 248:
            prefix = 21;
            break;
        case 240:
            prefix = 20;
            break;
        case 224:
            prefix = 19;
            break;
        case 192:
            prefix = 18;
            break;
        case 128:
            prefix = 17;
            break;
        case 0:
            prefix = 16;
            break;
        default:
            prefix = 32;
            break;
    }

    return prefix;
}

int netdevice_configure_address(char *netdevice, char *address)
{
    int ret = 0;
    char cmd[MAX_STRING_SIZE * 2] = {0};

    snprintf(cmd, MAX_STRING_SIZE * 2, "ip addr add %s dev %s",
            address, netdevice);

    if (execute_cmd(cmd)) {
        BAIL_OUT("failed command execution");
    }

end:
    return ret;
}

int netdevice_configure_route(char *netdevice, char *gateway)
{
    int ret = 0;
    char cmd[MAX_STRING_SIZE * 2] = {0};

    if (strstr(gateway, ".")) {
        snprintf(cmd, MAX_STRING_SIZE * 2, "ip route add default via %s dev %s proto static",
                gateway, netdevice);
    } else if (strstr(gateway, ":")) {
        snprintf(cmd, MAX_STRING_SIZE * 2, "ip -6 route add default via %s dev %s proto static",
                gateway, netdevice);
    }

    if (execute_cmd(cmd)) {
        BAIL_OUT("failed configuring route");
    }

end:
    return ret;
}

int netdevice_configure_dns(char *netdevice, char *dns)
{
    int ret = 0;
    char cmd[MAX_STRING_SIZE * 2] = {0};

    if (strstr(dns, ".")) {
        snprintf(cmd, MAX_STRING_SIZE * 2, "systemd-resolve --interface %s --set-dns=%s",
                netdevice, dns);
    }

    if (execute_cmd(cmd)) {
        BAIL_OUT("failed command execution");
    }

end:
    return ret;
}

int netdevice_set_mtu(char *netdevice, uint32_t mtu)
{
    int ret = 0;
    char cmd[MAX_STRING_SIZE] = {0};

    snprintf(cmd, MAX_STRING_SIZE, "ip link set %s mtu %u",
            netdevice, mtu);
    if (execute_cmd(cmd)) {
        BAIL_OUT("failed set mtu command execution");
    }

end:
    return ret;
}

int netdevice_set_link(char *netdevice, uint8_t up)
{
    int ret = 0;
    char cmd[MAX_STRING_SIZE] = {0};

    snprintf(cmd, MAX_STRING_SIZE, "ip link set %s %s",
            netdevice, up ? "up" : "down");
    if (execute_cmd(cmd)) {
        BAIL_OUT("failed set mtu command execution");
    }

end:
    return ret;
}

int netdevice_dispose(char *netdevice)
{
    int ret = 0;
    char cmd[MAX_STRING_SIZE] = {0};

    snprintf(cmd, MAX_STRING_SIZE, "ip addr flush %s",
            netdevice);
    if (execute_cmd(cmd)) {
        BAIL_OUT("failed command execution");
    }

    snprintf(cmd, MAX_STRING_SIZE, "ip link set %s down",
            netdevice);
    if (execute_cmd(cmd)) {
        BAIL_OUT("failed command execution");
    }

end:
    return ret;
}

int netdevice_set_ra(char *netdevice, int enable)
{
    int ret = 0;
    char cmd[MAX_STRING_SIZE] = {0};

    snprintf(cmd, MAX_STRING_SIZE, "sysctl -w net.ipv6.conf.%s.accept_ra=%d",
            netdevice, enable);
    if (execute_cmd(cmd)) {
        BAIL_OUT("failed command execution");
    }

end:
    return ret;
}
