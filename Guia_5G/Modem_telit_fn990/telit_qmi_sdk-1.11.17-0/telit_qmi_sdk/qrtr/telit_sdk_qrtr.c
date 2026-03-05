#include <endian.h>
#include <errno.h>
#if defined TELIT_QRTR
#include <linux/qrtr.h>
#endif
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "telit_sdk.h"
#include "telit_sdk_qrtr.h"


#if defined TELIT_QRTR
int telit_qrtr_get_client_for_svc(int svc, int *sock, int *node, int *port)
{
    int ret = -1;
    struct qrtr_ctrl_pkt pkt = {};
    struct sockaddr_qrtr addr = {};
    socklen_t addrlen;
    struct timeval tv;

    *node = -1;
    *port = -1;

    /* Open QRTR socket */
    *sock = socket(AF_QIPCRTR, SOCK_DGRAM, 0);
    if (*sock < 0) {
        RLOGE("socket(): %s", strerror(errno));
        goto end;
    }

    /* Get socket address (mostly family and node) */
    addrlen = sizeof(addr);
    ret = getsockname(*sock, (struct sockaddr *) &addr, &addrlen);
    if (ret) {
        RLOGE("getsockname(): %s", strerror(errno));
        goto cleanup;
    }

    if (addr.sq_family != AF_QIPCRTR || addrlen != sizeof(addr)) {
        RLOGE("getsockname checks");
        goto cleanup;
    }

    /* Configure Timeout */
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    ret = setsockopt(*sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (ret < 0) {
        RLOGE("setsocopt(): %s", strerror(errno));
        goto cleanup;
    }

    /* Pack request for Port ID */
    pkt.cmd = htole32(QRTR_TYPE_NEW_LOOKUP);
    pkt.server.service = htole32(svc);
    addr.sq_port = QRTR_PORT_CTRL;
    ret = sendto(*sock, &pkt, sizeof(pkt), 0, (struct sockaddr *) &addr, sizeof(addr));
    if (ret < 0) {
        RLOGE("sendto(): %s", strerror(errno));
        goto cleanup;
    }

    while ((ret = recvfrom(*sock, &pkt, sizeof(pkt), 0, (struct sockaddr *) &addr, &addrlen)) > 0) {
        unsigned int type = le32toh(pkt.cmd);

        if (ret < (int)sizeof(pkt) || type != QRTR_TYPE_NEW_SERVER) {
            continue;
        }

        if (!pkt.server.service && !pkt.server.instance &&
                !pkt.server.node && !pkt.server.port) {
            break;
        }

        *node = le32toh(pkt.server.node);
        *port = le32toh(pkt.server.port);

        RLOGD("service '%d' found at node:'%d', port:'%d'", svc, *node, *port);
        break;
    }

    if (*node < 0 || *port < 0) {
        RLOGE("could not find service '%d'", svc);
        goto cleanup;
    }

    return 0;

cleanup:
    close(*sock);
    *sock = -1;
end:
    return -1;
}
#else
int telit_qrtr_get_client_for_svc(int svc __unused, int *sock __unused,
        int *node __unused, int *port __unused)
{
    return -1;
}
#endif

#if defined TELIT_QRTR
int telit_qrtr_sendto(int sock, int node, int port, void *buf, uint16_t len)
{
    struct sockaddr_qrtr addr = {};

    addr.sq_family = AF_QIPCRTR;
    addr.sq_node = htole32(node);
    addr.sq_port = htole32(port);

    return sendto(sock, buf, len, 0, (struct sockaddr *) &addr, sizeof(addr));
}
#else
int telit_qrtr_sendto(int sock __unused, int node __unused,
        int port __unused, void *buf __unused, uint16_t len __unused)
{
    return -1;
}
#endif

#if defined TELIT_QRTR
int telit_qrtr_recvfrom(int sock, int node, int *port, void *buf, uint16_t *len)
{
    int ret = -1;
    const uint32_t buflen = *len;
    struct sockaddr_qrtr addr = {};
    socklen_t addrlen = sizeof(addr);
    unsigned int u_node = (unsigned int)node;

    *len = 0;
    do {
        ret = recvfrom(sock, buf, buflen, 0, (struct sockaddr *) &addr, &addrlen);
        if (ret < 0) {
            if (errno == EAGAIN) {  /* resource temporarly unavailable */
                ret = 0;
            } else {
                RLOGE("recvfrom() error %d:  %s\n", errno, strerror(errno));
            }
            break;
        }

        if (u_node != le32toh(addr.sq_node)) {
            RLOGD("Not the expected node (%d != %d)\n",
                    node, le32toh(addr.sq_node));
            continue;
        }

        if (port) {
            *port = le32toh(addr.sq_port);
        }

        RLOGD("read %zu bytes, from node:%d, port:%d", ret, addr.sq_node, addr.sq_port);
        *len = ret;
        break;

    } while (ret > 0);
    return ret;
}
#else
int telit_qrtr_recvfrom(int sock __unused, int node __unused,
        int *port __unused, void *buf __unused, uint16_t *len __unused)
{
    return -1;
}
#endif

int telit_is_qrtr_supported()
{
#if defined TELIT_QRTR
    return 1;
#else
    return 0;
#endif
}

