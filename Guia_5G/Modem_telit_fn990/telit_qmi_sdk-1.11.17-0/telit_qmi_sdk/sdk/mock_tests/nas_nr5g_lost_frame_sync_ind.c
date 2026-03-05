/* QMI_NAS_NR5G_LOST_FRAME_SYNC_IND mock */

#include <telit_sdk.h>
#include <telit_sdk_nas.h>
#include <telit_sdk_ctl.h>

uint8_t mock_test_buffer_1[] = {
    0x01, 0x13, 0x00, 0x01, 0x03, 0x01,
    0x10, 0x01, 0x00, 0x21, 0x01, 0x07, 0x00,
    0x10, 0x04, 0x00, 0x02, 0x00, 0x00, 0x00
};

int main(int argc __unused, char **argv __unused)
{
    nas_nr5g_lost_frame_sync_t ind;
    uint16_t payload_len;
    QMI_SDK_error_t rtn;
    uint8_t *payload;
    int ret = -1;

    rtn = skip_qmux_header(mock_test_buffer_1, sizeof(mock_test_buffer_1),
            &payload, &payload_len);
    if (rtn != QMI_SDK_ERR_NONE) {
        RLOGE("failed to skip qmux header", rtn);
        goto end;
    }

    rtn = telit_nas_indication_handler(QMI_NAS_NR5G_LOST_FRAME_SYNC_IND,
            payload, payload_len,
            (uint8_t *)&ind, sizeof(ind));
    if (rtn != QMI_SDK_ERR_NONE) {
        goto end;
    }

    if (!ind.nr5g_sync_lost_reason_available) {
        RLOGE("nr5g_sync_lost_reason_available = false");
        goto end;
    }

    ret = (ind.nr5g_sync_lost_reason != QMI_NAS_NR5G_LOST_FRAME_SYNC_RESELECTION);

end:
    if (ret) {
        RLOGE("FAIL");
    } else {
        RLOGE("SUCCESS");
    }
    return ret;
}
