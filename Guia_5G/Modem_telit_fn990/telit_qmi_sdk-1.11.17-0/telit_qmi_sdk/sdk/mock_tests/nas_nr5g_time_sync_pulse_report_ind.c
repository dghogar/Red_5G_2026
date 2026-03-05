/* QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND mock */

#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_nas.h>
#include <telit_sdk_ctl.h>

uint8_t mock_test_buffer_1[] = {
    0x01, 0x1B, 0x00, 0x01, 0x03, 0x01,
    0x10, 0x01, 0x00, 0x20, 0x01, 0x0F, 0x00,
    0x10, 0x08, 0x00, 0x02, 0x00, 0x12, 0x31, 0x00, 0x00, 0x10, 0x00,
    0x14, 0x01, 0x00, 0x10
};
nas_nr5g_time_sync_pulse_report_t mock_test_expected_1 = {
    .gps_time_available = true,
    .gps_time = 4503600450633730,
    .leapseconds_available = true,
    .leapseconds = 16
};

uint8_t mock_test_buffer_2[] = {
    0x01, 0x21, 0x00, 0x01, 0x03, 0x01,
    0x10, 0x01, 0x00, 0x20, 0x01, 0x15, 0x00,
    0x11, 0x04, 0x00, 0x00, 0x02, 0x00, 0x00,
    0x12, 0x04, 0x00, 0x00, 0x10, 0x10, 0x00,
    0x13, 0x04, 0x00, 0x00, 0x00, 0x10, 0x10
};
nas_nr5g_time_sync_pulse_report_t mock_test_expected_2 = {
    .sfn_available = true,
    .sfn = 512,
    .nta_available = true,
    .nta = 1052672,
    .nta_offset_available = true,
    .nta_offset = 269484032
};

uint8_t mock_test_buffer_3[] = {
    0x01, 0x17, 0x00, 0x01, 0x03, 0x01,
    0x10, 0x01, 0x00, 0x20, 0x01, 0x0B, 0x00,
    0x15, 0x08, 0x00, 0x02, 0x00, 0x05, 0x10, 0x00, 0x00, 0x10, 0x00,
};
nas_nr5g_time_sync_pulse_report_t mock_test_expected_3 = {
    .utc_time_available = true,
    .utc_time = 4503599896133634
};

int main(int argc __unused, char **argv __unused)
{
    nas_nr5g_time_sync_pulse_report_t ind;
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

    rtn = telit_nas_indication_handler(QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND,
            payload, payload_len,
            (uint8_t *)&ind, sizeof(ind));
    if (rtn != QMI_SDK_ERR_NONE || memcmp(&ind, &mock_test_expected_1, sizeof(ind))) {
        RLOGE("fail mock_test_expected_1", rtn);
        goto end;
    }

    memset(&ind, '\0', sizeof(ind));
    rtn = skip_qmux_header(mock_test_buffer_2, sizeof(mock_test_buffer_2),
            &payload, &payload_len);
    if (rtn != QMI_SDK_ERR_NONE) {
        RLOGE("failed to skip qmux header mock_test_buffer_2", rtn);
        goto end;
    }

    rtn = telit_nas_indication_handler(QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND,
            payload, payload_len,
            (uint8_t *)&ind, sizeof(ind));
    if (rtn != QMI_SDK_ERR_NONE || memcmp(&ind, &mock_test_expected_2, sizeof(ind))) {
        RLOGE("fail mock_test_expected_2", rtn);
        goto end;
    }

    memset(&ind, '\0', sizeof(ind));
    rtn = skip_qmux_header(mock_test_buffer_3, sizeof(mock_test_buffer_3),
            &payload, &payload_len);
    if (rtn != QMI_SDK_ERR_NONE) {
        RLOGE("failed to skip qmux header mock_test_buffer_3", rtn);
        goto end;
    }

    rtn = telit_nas_indication_handler(QMI_NAS_NR5G_TIME_SYNC_PULSE_REPORT_IND,
            payload, payload_len,
            (uint8_t *)&ind, sizeof(ind));
    if (rtn != QMI_SDK_ERR_NONE || memcmp(&ind, &mock_test_expected_3, sizeof(ind))) {
        RLOGE("fail mock_test_expected_3", rtn);
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
