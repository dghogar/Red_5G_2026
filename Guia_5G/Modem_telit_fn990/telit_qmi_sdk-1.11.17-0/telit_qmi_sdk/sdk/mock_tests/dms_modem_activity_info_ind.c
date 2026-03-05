/* QMI_DMS_MODEM_ACTIVITY_INFO_IND mock */

#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_dms.h>
#include <telit_sdk_ctl.h>

uint8_t mock_test_buffer_1[] = {
    0x01, 0x35, 0x00, 0x01, 0x02, 0x01,
    0x10, 0x01, 0x00, 0x5f, 0x00, 0x29, 0x00,
    0x10, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x11, 0x04, 0x00, 0x00, 0xAA, 0x00, 0x00,
    0x12, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x13, 0x11, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x32, 0x00,
};
dms_modem_activity_info_t mock_test_expected_1 = {
    .idle_mode_duration_available = true,
    .idle_mode_duration = 16,
    .sleep_mode_duration_available = true,
    .sleep_mode_duration = 43520,
    .rx_mode_duration_available = true,
    .rx_mode_duration = 0,
    .tx_mode_duration_size = 2,
    .tx_mode_duration[0].tx_mode_level = QMI_DMS_TX_MODE_LEVEL_ONE,
    .tx_mode_duration[0].tx_mode_duration = 80,
    .tx_mode_duration[1].tx_mode_level = QMI_DMS_TX_MODE_LEVEL_THREE,
    .tx_mode_duration[1].tx_mode_duration = 3276800,
};

uint8_t mock_test_buffer_2[] = {
    0x01, 0x13, 0x00, 0x01, 0x02, 0x01,
    0x10, 0x01, 0x00, 0x5f, 0x00, 0x07, 0x00,
    0x12, 0x04, 0x00, 0x00, 0x00, 0x00, 0x12,
};
dms_modem_activity_info_t mock_test_expected_2 = {
    .rx_mode_duration_available = true,
    .rx_mode_duration = 301989888,
};

int main(int argc __unused, char **argv __unused)
{
    dms_modem_activity_info_t ind;
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

    rtn = telit_dms_indication_handler(QMI_DMS_MODEM_ACTIVITY_INFO_IND,
            payload, payload_len,
            (uint8_t *)&ind, sizeof(ind));
    if (rtn != QMI_SDK_ERR_NONE || memcmp(&ind, &mock_test_expected_1, sizeof(ind))) {
        RLOGE("fail mock_test_expected_1", rtn);
        goto end;
    }

    rtn = skip_qmux_header(mock_test_buffer_2, sizeof(mock_test_buffer_2),
            &payload, &payload_len);
    if (rtn != QMI_SDK_ERR_NONE) {
        RLOGE("failed to skip qmux header mock_test_buffer_2", rtn);
        goto end;
    }

    rtn = telit_dms_indication_handler(QMI_DMS_MODEM_ACTIVITY_INFO_IND,
            payload, payload_len,
            (uint8_t *)&ind, sizeof(ind));
    if (rtn != QMI_SDK_ERR_NONE || memcmp(&ind, &mock_test_expected_2, sizeof(ind))) {
        RLOGE("fail mock_test_expected_2", rtn);
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
