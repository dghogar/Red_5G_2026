#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <telit_sdk_private.h>

#include "tqcm.h"
#include "tqcm_private.h"
#include "tqcm_ctl.h"

int tqcm_ctl_get_client_id_send(tqcm_client_context_t *ctx,
        ctl_get_client_id_t *input,
        ctl_get_client_id_info_t *out,
        tqcm_callback_t cb,
        uint64_t token)
{
    int ret = 0;
    pack_qmi_t req_ctx = {0};
    uint8_t req[QMI_REQ_SMALL];
    uint16_t req_len = QMI_REQ_SMALL;
    void *resp = NULL;
    uint16_t resp_len;

    if (!ctx) {
        RLOGE("Invalid service context");
        return EINVAL;
    }

    req_ctx.xid = tqcm_update_transaction_id(ctx);

    ret = telit_ctl_get_client_id_pack(&req_ctx, req, &req_len, input);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("telit_ctl_get_client_id_pack fail %d", ret);
        goto end;
    }

    ret = tqcm_generic_send(ctx, &req_ctx,
            req, req_len,
            cb, token,
            &resp, &resp_len);
    if (!ret && out && !cb) {
        /* Synchronous call */
        ret = telit_ctl_get_client_id_unpack(resp, resp_len, out);
        if (ret != QMI_SDK_ERR_NONE) {
            RLOGE("unpack fail %d ", ret);
            ret = EPROTO;
        }

        free(resp);
    }

    /* For async call nothing more is needed, EINPROGRESS is returned */
end:
    return ret;
}

int tqcm_ctl_release_client_id_send(tqcm_client_context_t *ctx,
        ctl_get_client_id_info_t *input,
        ctl_get_client_id_info_t *out,
        tqcm_callback_t cb,
        uint64_t token)
{
    int ret = 0;
    pack_qmi_t req_ctx = {0};
    uint8_t req[QMI_REQ_SMALL];
    uint16_t req_len = QMI_REQ_SMALL;
    void *resp = NULL;
    uint16_t resp_len;

    if (!ctx) {
        RLOGE("Invalid service context");
        return EINVAL;
    }

    req_ctx.xid = tqcm_update_transaction_id(ctx);

    ret = telit_ctl_release_client_id_pack(&req_ctx, req, &req_len, input);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("telit_ctl_release_client_id_pack fail %d", ret);
        goto end;
    }

    ret = tqcm_generic_send(ctx, &req_ctx,
            req, req_len,
            cb, token,
            &resp, &resp_len);
    if (!ret && out && !cb) {
        /* Synchronous call */
        ret = telit_ctl_release_client_id_unpack(resp, resp_len, out);
        if (ret != QMI_SDK_ERR_NONE) {
            RLOGE("unpack fail %d ", ret);
            ret = EPROTO;
        }

        free(resp);
    }

    /* For async call nothing more is needed, EINPROGRESS is returned */
end:
    return ret;
}

int tqcm_ctl_get_version_info_send(tqcm_client_context_t *ctx,
        ctl_get_version_info_t *out,
        tqcm_callback_t cb,
        uint64_t token)
{
    int ret = 0;
    pack_qmi_t req_ctx = {0};
    uint8_t req[QMI_REQ_SMALL];
    uint16_t req_len = QMI_REQ_SMALL;
    void *resp = NULL;
    uint16_t resp_len;

    if (!ctx) {
        RLOGE("Invalid service context");
        return EINVAL;
    }

    req_ctx.xid = tqcm_update_transaction_id(ctx);

    ret = telit_ctl_get_version_info_pack(&req_ctx, req, &req_len);
    if (ret != QMI_SDK_ERR_NONE) {
        RLOGE("telit_ctl_get_version_info_pack fail %d", ret);
        goto end;
    }

    ret = tqcm_generic_send(ctx, &req_ctx,
            req, req_len,
            cb, token,
            &resp, &resp_len);
    if (!ret && out && !cb) {
        /* Synchronous call */
        ret = telit_ctl_get_version_info_unpack(resp, resp_len, out);
        if (ret != QMI_SDK_ERR_NONE) {
            RLOGE("unpack fail %d ", ret);
            ret = EPROTO;
        }

        free(resp);
    }

    /* For async call nothing more is needed, EINPROGRESS is returned */
end:
    return ret;
}
