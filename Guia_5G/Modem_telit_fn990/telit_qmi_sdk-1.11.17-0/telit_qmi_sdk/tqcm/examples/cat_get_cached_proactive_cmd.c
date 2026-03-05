#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <telit_sdk.h>
#include <telit_sdk_cat.h>
#include <tqcm.h>
#include <tqcm_cat.h>
#include <telit_sdk_test_helpers.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

void print_proactive_cmd(cat_generic_proactive_command_t *cmd)
{
    RLOGI("ref id: %u", cmd->uim_ref_id);
    RLOGI("cmd sz: %u", cmd->cmd_sz);
    telit_print_buffer(cmd->cmd, cmd->cmd_sz, LOG_INFO);
}

int main(int argc, char **argv)
{
    cat_cached_proactive_cmd_input_t input;
    cat_cached_proactive_cmd_info_t out;
    tqcm_client_context_t ctx_cat;
    int ret = EXIT_SUCCESS;
    uint16_t sdk_error;

    telit_set_log_level(LOG_INFO);

    if (argc == 1) {
        dev = default_rmnet_dev;
    } else {
        dev = argv[1];
    }

    if (tqcm_init(dev, &ctx)) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    if (tqcm_init_client_context(QMI_SVC_CAT, &ctx, &ctx_cat, NULL)) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto tqcm_disposal;
    }

    RLOGI("QMI_CAT_CACHED_COMMAND_ID_SETUP_MENU");
    input.command_id = QMI_CAT_CACHED_COMMAND_ID_SETUP_MENU;
    ret = tqcm_cat_get_cached_proactive_cmd_send(&ctx_cat,
            &input, &out, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
            if (sdk_error != QMI_ERR_INFO_UNAVAILABLE) {
                ret = EXIT_FAILURE;
                goto context_disposal;
            }
        } else {
            ret = EXIT_FAILURE;
            goto context_disposal;
        }
    }
    if (out.setup_menu_event_available) {
        print_proactive_cmd(&out.setup_menu_event);
    }

    RLOGI("QMI_CAT_CACHED_COMMAND_ID_SETUP_EVENT_LIST");
    input.command_id = QMI_CAT_CACHED_COMMAND_ID_SETUP_EVENT_LIST;
    ret = tqcm_cat_get_cached_proactive_cmd_send(&ctx_cat,
            &input, &out, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
            if (sdk_error != QMI_ERR_INFO_UNAVAILABLE) {
                ret = EXIT_FAILURE;
                goto context_disposal;
            }
        } else {
            ret = EXIT_FAILURE;
            goto context_disposal;
        }
    }
    if (out.setup_event_list_raw_event_available) {
        print_proactive_cmd(&out.setup_event_list_raw_event);
    }

    RLOGI("QMI_CAT_CACHED_COMMAND_ID_SETUP_IDLE_TEXT");
    input.command_id = QMI_CAT_CACHED_COMMAND_ID_SETUP_IDLE_TEXT;
    ret = tqcm_cat_get_cached_proactive_cmd_send(&ctx_cat,
            &input, &out, &sdk_error, NULL, 0);
    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
            if (sdk_error != QMI_ERR_INFO_UNAVAILABLE) {
                ret = EXIT_FAILURE;
                goto context_disposal;
            }
        } else {
            ret = EXIT_FAILURE;
            goto context_disposal;
        }
    }
    if (out.setup_idle_mode_text_event_available) {
        print_proactive_cmd(&out.setup_idle_mode_text_event);
    }

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_cat) != 0) {
        RLOGE("Failed context disposal");
        ret = EXIT_FAILURE;
    }

tqcm_disposal:
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
