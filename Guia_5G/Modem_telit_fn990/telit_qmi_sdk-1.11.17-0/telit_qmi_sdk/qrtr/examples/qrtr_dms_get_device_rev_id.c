#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "telit_sdk.h"
#include "telit_sdk_qrtr.h"
#include "telit_sdk_dms.h"

int main(int argc __unused, char **argv __unused)
{
    int rtn;
    int sock, node, port;
    pack_qmi_t req_ctx = {};
    dms_get_fw_revision_t out = {};
    uint8_t rsp[QMI_MSG_MAX] = {0};
    uint8_t req[QMI_MSG_MAX] = {0};
    uint16_t req_len = QMI_MSG_MAX;
    uint16_t rsp_len = QMI_MSG_MAX;

    telit_set_log_level(LOG_INFO);

    rtn = telit_qrtr_get_client_for_svc(QMI_SVC_DMS, &sock, &node, &port);
    if (rtn < 0) {
        return rtn;
    }

    req_ctx.xid = 1;
    telit_dms_get_device_rev_id_pack(&req_ctx, req, &req_len);

    RLOGI("sending request...");
    rtn = telit_qrtr_sendto(sock, node, port, req, req_len);
    if (rtn < 0) {
        RLOGE("qrtr_sendto(): %s", strerror(errno));
        goto cleanup;
    }

    rtn = telit_qrtr_recvfrom(sock, node, &port, rsp, &rsp_len);
    if (rtn < 0) {
        RLOGE("qrtr_recvfrom");
        goto cleanup;
    }

    RLOGI("received response");
    rtn = telit_dms_get_device_rev_id_unpack(rsp, rsp_len, &out);
    if (rtn != QMI_SDK_ERR_NONE) {
        RLOGI("unpack fail %d ", rtn);
    } else {
        RLOGI("unpack OK");
        RLOGI("DMS firmware amss rev %s, len %u", out.amss, out.amss_len);
        RLOGI("DMS firmware boot rev %s, len %u", out.boot, out.boot_len);
        RLOGI("DMS firmware pri rev %s, len %u", out.pri, out.pri_len);
    }

cleanup:
    close(sock);

    return rtn;
}
