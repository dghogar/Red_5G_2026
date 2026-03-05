/* QMI_GMS_NAS_GET_MODEM_STATUS mock */

#include <string.h>

#include "telit_sdk.h"
#include "telit_sdk_nas.h"
#include "telit_sdk_gms.h"
#include <telit_sdk_ctl.h>
#include <telit_sdk_test_helpers.h>

uint8_t mock_test_buffer_1[] = {
    0x02, 0x01, 0x00, 0x02, 0x03, 0x1B, 0x00,
    0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x05, 0x00, 0x1C, 0x05, 0x09, 0x00, 0x00,
    0x10, 0x09, 0x00, 0x03, 0x05, 0x46, 0x05, 0x96, 0x4B, 0x03, 0x07, 0x00
};
nas_modem_status_t mock_test_expected_1 = {
    .temperature = 28,
    .modem_mode = 5,
    .system_mode = 9,
    .ims_reg_state = 0,
    .ps_state = 0,
    .lte_info_available = true,
    .lte_info.band = 3,
    .lte_info.bandwidth = 5,
    .lte_info.rx_chan = 1350,
    .lte_info.tx_chan = 19350,
    .lte_info.emm_state = 3,
    .lte_info.emm_sub_state = 7,
    .lte_info.emm_conn_state = 0
};

uint8_t mock_test_buffer_2[] = {
    0x02, 0x01, 0x00, 0x02, 0x03, 0x34, 0x00,
    0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x05, 0x00, 0x1B, 0x05, 0x09, 0x00, 0x00,
    0x10, 0x09, 0x00, 0x14, 0x03, 0x38, 0x18, 0x88, 0x5E, 0x03, 0x07, 0x00,
    0x11, 0x16, 0x00, 0x10, 0x00, 0x05, 0x00, 0x04, 0x03, 0x02, 0x01, 0x03, 0x03, 0x04, 0x05, 0x32,
    0x00, 0x43, 0x00, 0x34, 0x00, 0x56, 0x00, 0x49, 0x32
};
nas_modem_status_t mock_test_expected_2 = {
    .temperature = 27,
    .modem_mode = 5,
    .system_mode = 9,
    .ims_reg_state = 0,
    .ps_state = 0,
    .lte_info_available = true,
    .lte_info.band = 20,
    .lte_info.bandwidth = 3,
    .lte_info.rx_chan = 6200,
    .lte_info.tx_chan = 24200,
    .lte_info.emm_state = 3,
    .lte_info.emm_sub_state = 7,
    .lte_info.emm_conn_state = 0,
    .nr5g_info_available = true,
    .nr5g_info.band = 16,
    .nr5g_info.bandwidth = 5,
    .nr5g_info.rx_chan = 16909060,
    .nr5g_info.tx_chan = 84148995,
    .nr5g_info.rssi = 50,
    .nr5g_info.rsrp = 67,
    .nr5g_info.rsrq = 52,
    .nr5g_info.snr = 86,
    .nr5g_info.pci = 12873,
};

uint8_t mock_test_buffer_3[] = {
    0x02, 0x01, 0x00, 0x02, 0x03, 0x28, 0x00,
    0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x05, 0x00, 0x1C, 0x05, 0x0B, 0x00, 0x01,
    0x11, 0x16, 0x00, 0x4E, 0x00, 0x0C, 0x00, 0x00, 0xB7, 0x09, 0x00, 0x00, 0xB7, 0x09, 0x00, 0xA9,
    0xFF, 0x97, 0xFF, 0xEF, 0xFF, 0xBA, 0xFF, 0x7F, 0x02
};
nas_modem_status_t mock_test_expected_3 = {
    .temperature = 28,
    .modem_mode = 5,
    .system_mode = 11,
    .ims_reg_state = 0,
    .ps_state = 1,
    .nr5g_info_available = true,
    .nr5g_info.band = 78,
    .nr5g_info.bandwidth = 12,
    .nr5g_info.rx_chan = 636672,
    .nr5g_info.tx_chan = 636672,
    .nr5g_info.rssi = -87,
    .nr5g_info.rsrp = -105,
    .nr5g_info.rsrq = -17,
    .nr5g_info.snr = -70,
    .nr5g_info.pci = 639,
};

int main()
{
    nas_modem_status_t modem_status = {0};
    QMI_SDK_error_t rtn;
    int ret = -1;

    rtn = telit_gms_nas_modem_status_unpack(mock_test_buffer_1, sizeof(mock_test_buffer_1), &modem_status);
    if (rtn != QMI_SDK_ERR_NONE || memcmp(&modem_status, &mock_test_expected_1, sizeof(modem_status))) {
        RLOGE("fail mock_test_expected_1 %d", rtn);
        print_gms_nas_modem_status(&modem_status);
        goto end;
    }

    rtn = telit_gms_nas_modem_status_unpack(mock_test_buffer_2, sizeof(mock_test_buffer_2), &modem_status);
    if (rtn != QMI_SDK_ERR_NONE || memcmp(&modem_status, &mock_test_expected_2, sizeof(modem_status))) {
        RLOGE("fail mock_test_expected_2 %d", rtn);
        print_gms_nas_modem_status(&modem_status);
        goto end;
    }

    rtn = telit_gms_nas_modem_status_unpack(mock_test_buffer_3, sizeof(mock_test_buffer_3), &modem_status);
    if (rtn != QMI_SDK_ERR_NONE || memcmp(&modem_status, &mock_test_expected_3, sizeof(modem_status))) {
        RLOGE("fail mock_test_expected_3 %d", rtn);
        print_gms_nas_modem_status(&modem_status);
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
