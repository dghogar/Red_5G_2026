#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_fota.h>
#include <tqcm.h>
#include <tqcm_fota.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_fota;
    uint16_t sdk_error;
    fota_server_info_t out;

    telit_set_log_level(LOG_INFO);

    if (argc == 1) {
        dev = default_rmnet_dev;
    } else {
        dev = argv[1];
    }

    ret = tqcm_init(dev, &ctx);
    if (ret != 0) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_init_client_context(QMI_SVC_FOTA,
            &ctx,
            &ctx_fota,
            NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_fota.client_id);

    ret = tqcm_fota_oem_ftp_set_server_info_send(&ctx_fota,
            &out,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    } else {
        RLOGI("Success");
        if (out.ftp_url_len) {
            RLOGI("FTP url: %s", out.ftp_url);
        }
        if (out.delta_descriptor_path_len) {
            RLOGI("Delta descriptor path: %s", out.delta_descriptor_path);
        }
        if (out.ftp_port_available) {
            RLOGI("FTP port: %u", out.ftp_port);
        }
        if (out.user_name_len) {
            RLOGI("Username: %s", out.user_name);
        }
        if (out.password_len) {
            RLOGI("Password: %s", out.password);
        }
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_fota) != 0) {
        RLOGE("Failed context disposal");
        ret = EXIT_FAILURE;
    }

    if (tqcm_deinit(&ctx) != 0) {
        RLOGE("Failed tqcm deinitialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    /* Needed for valgrind complain about thread stack memory */
    sleep(1);

end:
    return ret;
}
