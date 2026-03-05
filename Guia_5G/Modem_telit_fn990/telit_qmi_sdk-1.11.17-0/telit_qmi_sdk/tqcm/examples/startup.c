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

    telit_set_log_level(LOG_DEBUG);
    RLOGI("### STARTUP TEST ###");

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
