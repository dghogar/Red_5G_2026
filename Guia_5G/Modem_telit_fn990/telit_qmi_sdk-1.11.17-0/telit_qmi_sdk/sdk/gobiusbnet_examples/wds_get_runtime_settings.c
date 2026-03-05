/* */
/* demo program for illustrating GobiNet driver usage */
/* */
/* (a) prepare a message */
/* (b) send to modem. */
/* (c) Wait for response from modem */
/* (d) Process received response message */
/* (e) close QMI service. */
/* */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <netinet/in.h>

#include "telit_sdk.h"
#include "telit_sdk_wds.h"

#define QMI_GET_SERVICE_FILE_IOCTL 0x8BE0 + 1

/* change this define if QMI device name is different */
#define GOBINET_DEVICE "/dev/qcqmi0"

#define SERVICE QMI_SVC_WDS
#define SVC_STR "WDS"

static int telit_client_fd_from_qcqmi(uint8_t svc, const char *qcqmi)
{
    int fd = open(qcqmi, O_RDWR);

    if (fd == -1) {
        RLOGE("%s: error in opening device %d - %s",
                __FUNCTION__, errno, strerror(errno));
        goto end;
    }

    if (ioctl(fd, QMI_GET_SERVICE_FILE_IOCTL, svc) != 0) {
        RLOGE("%s: error in ioctl", __FUNCTION__);
        close(fd);
        fd = -1;
    }

end:
    return fd;
}

int main()
{
    int svc = -1;
    pack_qmi_t req_ctx;
    unpack_qmi_t rsp_ctx;
    uint16_t xid = 1;
    int rtn;
    uint8_t rsp[QMI_MSG_MAX];
    uint8_t req[QMI_MSG_MAX];
    uint16_t req_size = QMI_MSG_MAX;
    uint16_t rsp_size = QMI_MSG_MAX;
    bool got_expected_message = false;
    wds_runtime_settings_info_t input;
    wds_profile_t out;

    memset(&input, 0, sizeof(input));
    memset(&out, 0, sizeof(out));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    input.requested_settings = 0b111111111111111111;

    telit_set_log_level(LOG_INFO);

    rtn = telit_wds_get_runtime_settings_pack(&req_ctx, req, &req_size, &input);

    if (rtn != QMI_SDK_ERR_NONE) {
        RLOGE("pack fail %d ", rtn);
        return -1;
    }

    if ((svc = telit_client_fd_from_qcqmi(SERVICE, GOBINET_DEVICE)) < 0) {
        RLOGE("%s Service Not Supported!", SVC_STR);
        return -1;
    }

    rtn = write(svc, req, req_size);

    if (rtn != req_size) {
        RLOGE("%s socket write failed!", SVC_STR);
        goto close_fd;
    }

    while (!got_expected_message) {
        rtn = read(svc, rsp, QMI_MSG_MAX);

        if (rtn > 0) {
            rsp_size = rtn;
        }

        RLOGD("rtn = %d, rspLen = %u", rtn, rsp_size);

        if ((rtn > 0) && (rsp_size > 0)) {
            telit_helper_get_resp_ctx(SERVICE, rsp, rsp_size, &rsp_ctx);

            switch (rsp_ctx.type) {
                case QMI_MSG_RESPONSE:
                    RLOGD("%s response message, msgid 0x%x", SVC_STR, rsp_ctx.msg_id);

                    if (rsp_ctx.xid == xid) {
                        rtn = telit_wds_get_runtime_settings_unpack(rsp, rsp_size, &out);

                        if (rtn != QMI_SDK_ERR_NONE) {
                            RLOGE("%s unpack fail %d ", SVC_STR, rtn);
                        } else {
                            uint8_t i;

                            if (out.profile_id_available) {
                                RLOGI("pdp_type %u, ", out.profile_id.type);
                                RLOGI("profile index %u, ", out.profile_id.index);
                            }

                            if (out.profile_settings.ipv4_addr_pref_available) {
                                uint32_t address = out.profile_settings.ipv4_addr_pref;

                                RLOGI("ipv4 address (%u) %u.%u.%u.%u",
                                        address,
                                        (address >> 24) & 0xFF,
                                        (address >> 16) & 0xFF,
                                        (address >> 8) & 0xFF,
                                        address & 0xFF);
                            }

                            if (out.profile_settings.ipv4_gateway_addr_available) {
                                uint32_t address_gw = out.profile_settings.ipv4_gateway_addr;

                                RLOGI("GW ipv4 address (%u) %u.%u.%u.%u",
                                        address_gw,
                                        (address_gw >> 24) & 0xFF,
                                        (address_gw >> 16) & 0xFF,
                                        (address_gw >> 8) & 0xFF,
                                        address_gw & 0xFF);
                            }

                            if (out.profile_settings.ipv4_subnet_mask_available) {
                                uint32_t subnet_mask = out.profile_settings.ipv4_subnet_mask_addr;

                                RLOGI("ipv4 subnet mask (%u) %u.%u.%u.%u",
                                        subnet_mask,
                                        (subnet_mask >> 24) & 0xFF,
                                        (subnet_mask >> 16) & 0xFF,
                                        (subnet_mask >> 8) & 0xFF,
                                        subnet_mask & 0xFF);
                            }

                            if (out.profile_settings.pri_dns_ipv4_addr_pref_available) {
                                uint32_t primary_dns = out.profile_settings.pri_dns_ipv4_addr_pref;

                                RLOGI("ipv4 primary dns (%u) %u.%u.%u.%u",
                                        primary_dns,
                                        (primary_dns >> 24) & 0xFF,
                                        (primary_dns >> 16) & 0xFF,
                                        (primary_dns >> 8) & 0xFF,
                                        primary_dns & 0xFF);
                            }

                            if (out.profile_settings.sec_dns_ipv4_addr_pref_available) {
                                uint32_t secondary_dns = out.profile_settings.sec_dns_ipv4_addr_pref;

                                RLOGI("ipv4 secondary dns (%u) %u.%u.%u.%u",
                                        secondary_dns,
                                        (secondary_dns >> 24) & 0xFF,
                                        (secondary_dns >> 16) & 0xFF,
                                        (secondary_dns >> 8) & 0xFF,
                                        secondary_dns & 0xFF);
                            }

                            if (out.profile_settings.ipv6_addr_available) {
                                RLOGI("ipv6 address: ");

                                for (i = 0; i < IPV6_ADDR_SIZE; i++) {
                                    RLOGI("%u ", ntohs(out.profile_settings.ipv6_addr.addr[i]));
                                }
                                RLOGI("Address prefix length: %u", out.profile_settings.ipv6_addr.prefix_len);
                            }

                            if (out.profile_settings.ipv6_gw_addr_available) {
                                RLOGI("ipv6 gw address: ");
                                for (i = 0; i < IPV6_ADDR_SIZE; i++) {
                                    RLOGI("%u ", ntohs(out.profile_settings.ipv6_gw_addr.addr[i]));
                                }
                                RLOGI("Address prefix length: %u",
                                        out.profile_settings.ipv6_gw_addr.prefix_len);
                            }

                            if (out.profile_settings.pri_dns_ipv6_addr_available) {
                                RLOGI("ipv6 primary dns address: ");
                                for (i = 0; i < IPV6_ADDR_SIZE; i++) {
                                    RLOGI("%u ", ntohs(out.profile_settings.pri_dns_ipv6_addr[i]));
                                }
                            }

                            if (out.profile_settings.sec_dns_ipv6_addr_available) {
                                RLOGI("ipv6 secondary dns address: ");
                                for (i = 0; i < IPV6_ADDR_SIZE; i++) {
                                    RLOGI("%u ", ntohs(out.profile_settings.sec_dns_ipv6_addr[i]));
                                }
                            }

                            if (out.profile_settings.ip_family_available) {
                                RLOGI("IP version %u", out.profile_settings.ip_family);
                            }
                        }

                        got_expected_message = true;
                    } else {
                        RLOGI("xid %d", xid);
                    }
                    break;

                case QMI_MSG_INDICATION:
                    RLOGD("%s Indication message, msgid 0x%x", SVC_STR, rsp_ctx.msg_id);
                    break;

                default:
                    RLOGD("Unknown QMI message type: %d", rsp_ctx.type);
                    break;
            }
        } else {
            sleep(1);
        }
    }

close_fd:

    if (svc >= 0) {
        close(svc);
    }

    return 0;
}
