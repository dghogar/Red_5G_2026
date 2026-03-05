#include <stdlib.h>
#include <unistd.h>

#include <telit_sdk.h>
#include <tqcm.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_dms;

    telit_set_log_level(LOG_DEBUG);
    RLOGI("### CONTEXT CREATION TEST ###");

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

    RLOGI("### INIT DONE ###");

    RLOGI("### CONTEXT CREATION ###");

    ret = tqcm_init_client_context(QMI_SVC_DMS,
            &ctx,
            &ctx_dms,
            NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGI("Acquired client id %u", ctx_dms.client_id);

    sleep(1);
    RLOGI("### CONTEXT DISPOSAL ###");
    ret = tqcm_deinit_client_context(&ctx, &ctx_dms);
    if (ret != 0) {
        RLOGE("Failed context disposal");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGI("### STARTING DEINIT ###");
    ret = tqcm_deinit(&ctx);
    if (ret != 0) {
        RLOGE("Failed tqcm deinitialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    /* Needed for valgrind complain about thread stack memory */
    sleep(1);
    RLOGI("### DEINIT DONE ###");

end:
    return ret;
}
