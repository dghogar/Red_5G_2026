/* QMI_WMS_MEMORY_FULL_IND mock */

#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_wms.h>
#include <telit_sdk_ctl.h>

uint8_t mock_test_buffer_1[] = {
    0x01, 0x11, 0x00, 0x01, 0x05, 0x01,
    0x10, 0x01, 0x00, 0x3f, 0x00, 0x05, 0x00,
    0x01, 0x02, 0x00, 0x01, 0x01
};
wms_memory_full_info_t mock_test_expected_1 = {
    .storage_type = QMI_WMS_STORAGE_NV,
    .message_mode = QMI_WMS_MESSAGE_MODE_GW,
};

int main(int argc __unused, char **argv __unused)
{
    wms_memory_full_info_t ind;
    uint16_t payload_len;
    QMI_SDK_error_t rtn;
    uint8_t *payload;
    int ret = -1;

    rtn = skip_qmux_header(mock_test_buffer_1, sizeof(mock_test_buffer_1),
            &payload, &payload_len);
    if (rtn != QMI_SDK_ERR_NONE) {
        RLOGE("failed to skip qmux header mock_test_buffer_1", rtn);
        goto end;
    }

    rtn = telit_wms_indication_handler(QMI_WMS_MEMORY_FULL_IND,
            payload, payload_len,
            (uint8_t *)&ind, sizeof(ind));
    if (rtn != QMI_SDK_ERR_NONE || memcmp(&ind, &mock_test_expected_1, sizeof(ind))) {
        RLOGE("fail mock_test_expected_1", rtn);
        goto end;
    }

    ret = 0;

end:
    if (ret < 0) {
        RLOGE("FAIL");
    } else {
        RLOGE("SUCCESS");
    }
    return ret;
}
