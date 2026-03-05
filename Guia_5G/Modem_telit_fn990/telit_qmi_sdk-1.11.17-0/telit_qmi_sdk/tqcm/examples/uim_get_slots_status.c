#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <telit_sdk.h>
#include <telit_sdk_uim.h>
#include <tqcm.h>
#include <tqcm_uim.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_uim;
    uim_slot_status_info_t out;
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

    ret = tqcm_init_client_context(QMI_SVC_UIM, &ctx, &ctx_uim, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_uim.client_id);

    ret = tqcm_uim_get_slots_status_send(&ctx_uim, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X, check if retries available", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    RLOGI("Got %u slots", out.slot_size);
    for (uint16_t i = 0; i < out.slot_size; i++) {
        RLOGI("slot %d", i);
        RLOGI("\t physical card status : %u", out.slot[i].physical_card_status);
        RLOGI("\t physical slot state  : %u", out.slot[i].physical_slot_state);
        RLOGI("\t logical slot         : %u", out.slot[i].logical_slot);
        RLOGI("\t iccid size         : %u\t", out.slot[i].iccid_len);
        for (int j = 0; j < out.slot[i].iccid_len; j++)
            RLOGI("0x%02X ", out.slot[i].iccid[j]);
        RLOGI("");
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_uim) != 0) {
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
