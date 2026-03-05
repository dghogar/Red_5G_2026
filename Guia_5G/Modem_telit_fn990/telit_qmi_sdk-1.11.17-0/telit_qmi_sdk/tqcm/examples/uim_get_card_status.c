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

static void print_card_status(uim_card_status_info_t *out)
{
    uint8_t i, j, k;

    if (out->card_status_available) {
        RLOGI("index_gw_pri: %u", out->card_status.index_gw_pri);
        RLOGI("index_1x_pri: %u", out->card_status.index_1x_pri);
        RLOGI("index_gw_sec: %u", out->card_status.index_gw_sec);
        RLOGI("index_1x_sec: %u", out->card_status.index_1x_sec);
        RLOGI("num_slot: %u", out->card_status.num_slot);
        for (i = 0; i < out->card_status.num_slot; i++) {
            RLOGI("slot %u", i);
            RLOGI("\tcard_state: %u", out->card_status.slot[i].card_state);
            RLOGI("\tupin_state: %u", out->card_status.slot[i].upin_state);
            RLOGI("\tupin_retries: %u", out->card_status.slot[i].upin_retries);
            RLOGI("\tupuk_retries: %u", out->card_status.slot[i].upuk_retries);

            RLOGI("\terror: %u", out->card_status.slot[i].error);
            RLOGI("\tnum_app: %u", out->card_status.slot[i].num_app);
            for (j = 0; j < out->card_status.slot[i].num_app; j++) {
                RLOGI("app %u", j);
                RLOGI("\t\ttype: %u", out->card_status.slot[i].app[j].type);
                RLOGI("\t\tstate: %u", out->card_status.slot[i].app[j].state);
                RLOGI("\t\tperso_state: %u", out->card_status.slot[i].app[j].perso_state);
                RLOGI("\t\tperso_feature: %u", out->card_status.slot[i].app[j].perso_feature);
                RLOGI("\t\tperso_retries: %u", out->card_status.slot[i].app[j].perso_retries);
                RLOGI("\t\tperso_unblock_retries: %u", out->card_status.slot[i].app[j].perso_unblock_retries);
                RLOGI("\t\taid_len: %u", out->card_status.slot[i].app[j].aid_len);
                for (k = 0; k < out->card_status.slot[i].app[j].aid_len; k++)
                    RLOGI("0x%02X ", out->card_status.slot[i].app[j].aid[k]);
                RLOGI("\t\tuniv_pin: %u", out->card_status.slot[i].app[j].univ_pin);
                RLOGI("\t\tpin1_state: %u", out->card_status.slot[i].app[j].pin1_state);
                RLOGI("\t\tpin1_retries: %u", out->card_status.slot[i].app[j].pin1_retries);
                RLOGI("\t\tpuk1_retries: %u", out->card_status.slot[i].app[j].puk1_retries);
                RLOGI("\t\tpin2_state: %u", out->card_status.slot[i].app[j].pin2_state);
                RLOGI("\t\tpin2_retries: %u", out->card_status.slot[i].app[j].pin2_retries);
                RLOGI("\t\tpuk2_retries: %u", out->card_status.slot[i].app[j].puk2_retries);
            }
        }
    }

    if (out->hot_swap_available) {
        RLOGI("hot_swap_len: %u", out->hot_swap_status.hot_swap_len);
        for (uint8_t k = 0; k < out->hot_swap_status.hot_swap_len; k++)
            RLOGI("%u ", out->hot_swap_status.hot_swap[k]);
    }
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_uim;
    uim_card_status_info_t out;
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

    ret = tqcm_init_client_context(QMI_SVC_UIM,
            &ctx,
            &ctx_uim,
            NULL);
    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_uim.client_id);

    ret = tqcm_uim_get_card_status_send(&ctx_uim,
            &out,
            &sdk_error,
            NULL,
            0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X, check if retries available", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    print_card_status(&out);

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
