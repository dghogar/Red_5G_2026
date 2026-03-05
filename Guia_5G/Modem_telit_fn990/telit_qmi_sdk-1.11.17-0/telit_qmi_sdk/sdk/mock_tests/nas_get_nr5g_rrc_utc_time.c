/* QMI_NAS_GET_NR5G_RRC_UTC_TIME mock */

#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_nas.h>

#include <telit_sdk_test_helpers.h>

uint8_t mock_test_buffer_1[] = {
    0x02, 0x01, 0x00, 0x02, 0x03, 0x5E, 0x00,
    0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* Type 0x10 */
    0x10, 0x01, 0x00, 0x01,
    /* Type 0x11 */
    0x11, 0x01, 0x00, 0x01,
    /* Type 0x12 */
    0x12, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00,
    /* Type 0x13 */
    0x13, 0x02, 0x00, 0x32, 0x00,
    /* Type 0x14 */
    0x14, 0x02, 0x00, 0xE8, 0x03,
    /* Type 0x15 */
    0x15, 0x0E, 0x00, 0x07, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x20, 0x03, 0x7E, 0x11, 0x10, 0x00,
    /* Type 0x16 */
    0x16, 0x04, 0x00, 0x90, 0x5F, 0x01, 0x00,
    /* Type 0x17 */
    0x17, 0x04, 0x00, 0x13, 0xDE, 0x44, 0x77,
    /* Type 0x18 */
    0x18, 0x04, 0x00, 0x25, 0x5E, 0xB9, 0x3B,
    /* Type 0x19 */
    0x19, 0x08, 0x00, 0x26, 0xA2, 0x3A, 0x49, 0x56, 0x55, 0x0E, 0x8B,
    /* Type 0x1A */
    0x1A, 0x0A, 0x00, 0xE8, 0x4E, 0x0C, 0x1F, 0x17, 0x3B, 0x3A, 0xE7, 0x03, 0x06,
};
nas_nr5g_rrc_utc_time_t mock_test_expected_1 = {
    .utc_sib9_acquired_available = true,
    .utc_sib9_acquired = true,
    .r16_timing_info_available = true,
    .r16_timing_info = true,
    .day_light_saving_time_available = true,
    .day_light_saving_time = 5,
    .leap_seconds_available = true,
    .leap_seconds = 50,
    .local_time_offset_available = true,
    .local_time_offset = 1000,
    .reference_time_info_available = true,
    .reference_time_info.ref_days = 7,
    .reference_time_info.ref_seconds = 30,
    .reference_time_info.ref_milliseconds = 800,
    .reference_time_info.ref_ten_nano_seconds = 1053054,
    .uncertainty_available = true,
    .uncertainty = 90000,
    .time_info_type_available = true,
    .time_info_type = 2001002003,
    .reference_sfn_available = true,
    .reference_sfn = 1002004005,
    .abs_time_available = true,
    .abs_time = 10020040050030060070U,
    .universal_time_available = true,
    .universal_time.year = 20200,
    .universal_time.month = 12,
    .universal_time.day = 31,
    .universal_time.hour = 23,
    .universal_time.minute = 59,
    .universal_time.second = 58,
    .universal_time.millisecond = 999,
    .universal_time.day_of_week = 6,
};

uint8_t mock_test_buffer_2[] = {
    0x02, 0x01, 0x00, 0x02, 0x03, 0x25, 0x00,
    0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* Type 0x15 */
    0x15, 0x0E, 0x00, 0x05, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x96, 0x09, 0x1F, 0x00,
    /* Type 0x1A */
    0x1A, 0x0A, 0x00, 0x3A, 0x75, 0x07, 0x17, 0x0C, 0x25, 0x19, 0x9B, 0x02, 0x03,
};
nas_nr5g_rrc_utc_time_t mock_test_expected_2 = {
    .reference_time_info_available = true,
    .reference_time_info.ref_days = 5,
    .reference_time_info.ref_seconds = 40,
    .reference_time_info.ref_milliseconds = 300,
    .reference_time_info.ref_ten_nano_seconds = 2034070,
    .universal_time_available = true,
    .universal_time.year = 30010,
    .universal_time.month = 7,
    .universal_time.day = 23,
    .universal_time.hour = 12,
    .universal_time.minute = 37,
    .universal_time.second = 25,
    .universal_time.millisecond = 667,
    .universal_time.day_of_week = 3,
};

int main()
{
    nas_nr5g_rrc_utc_time_t time = {0};
    QMI_SDK_error_t rtn;
    int ret = -1;

    rtn = telit_nas_get_nr5g_rrc_utc_time_unpack(mock_test_buffer_1, sizeof(mock_test_buffer_1), &time);
    if (rtn != QMI_SDK_ERR_NONE || memcmp(&time, &mock_test_expected_1, sizeof(time))) {
        RLOGE("fail mock_test_expected_1 %d", rtn);
        print_nas_generic_nr5g_rrc_utc_time(&time);
        goto end;
    }

    rtn = telit_nas_get_nr5g_rrc_utc_time_unpack(mock_test_buffer_2, sizeof(mock_test_buffer_2), &time);
    if (rtn != QMI_SDK_ERR_NONE || memcmp(&time, &mock_test_expected_2, sizeof(time))) {
        RLOGE("fail mock_test_expected_2 %d", rtn);
        print_nas_generic_nr5g_rrc_utc_time(&time);
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
