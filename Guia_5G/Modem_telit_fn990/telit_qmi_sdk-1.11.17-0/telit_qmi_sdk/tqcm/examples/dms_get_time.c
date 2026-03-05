#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <telit_sdk.h>
#include <telit_sdk_dms.h>
#include <tqcm.h>
#include <tqcm_dms.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_dms;
    dms_get_time_t out;
    uint16_t sdk_error;

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

    ret = tqcm_init_client_context(QMI_SVC_DMS,
            &ctx,
            &ctx_dms,
            NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_dms.client_id);

    ret = tqcm_dms_get_time_send(&ctx_dms,
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
    }

    RLOGI("DMS time_count %u", out.time_count);
    RLOGI("DMS time_source %u", out.time_source);
    if (out.sys_time_in_ms_available) {
        uint64_t ns_unix_time;
        struct timespec ts;
        struct tm stm;

        RLOGI("DMS sys_time_in_ms %lu", out.sys_time_in_ms);

        /* offset between gps epoch and unix epoch is 315964800 seconds */
        ns_unix_time = out.sys_time_in_ms * 1000000 + 315964800000000000ULL;
        ts.tv_sec = ns_unix_time / 1000000000ULL;
        ts.tv_nsec = ns_unix_time % 1000000000ULL;
        gmtime_r(&ts.tv_sec, &stm);
        RLOGI("Current time: %u-%u-%u %u:%u:%u", stm.tm_year + 1900,
                stm.tm_mon + 1,
                stm.tm_mday,
                stm.tm_hour,
                stm.tm_min,
                stm.tm_sec);
    }
    if (out.user_time_in_ms_available)
        RLOGI("DMS user_time_in_ms %lu", out.user_time_in_ms);

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_dms) != 0) {
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
