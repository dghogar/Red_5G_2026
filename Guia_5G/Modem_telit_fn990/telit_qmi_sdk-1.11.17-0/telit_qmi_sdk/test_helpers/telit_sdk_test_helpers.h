#ifndef TELIT_SDK_TEST_HELPERS_H
#define TELIT_SDK_TEST_HELPERS_H

#include "telit_sdk.h"
#include "telit_sdk_gms.h"
#include <telit_sdk_nas.h>

void print_get_runtime_response(wds_profile_t *out_get);

void print_gms_nas_modem_status(nas_modem_status_t *output);

void print_nas_network_time(nas_network_time_t *out);

void print_nas_sys_info(nas_system_info_t *out);

/**
 * @brief print data provided by QMI_NAS_GET_NR5G_RRC_UTC_TIME or QMI_NAS_NR5G_RRC_UTC_TIME_UPDATE_IND
 *
 * @param[in] out #nas_nr5g_rrc_utc_time_t
 */
void print_nas_generic_nr5g_rrc_utc_time(nas_nr5g_rrc_utc_time_t *out);

int ascii_to_hex(char *ascii, int ascii_len, uint8_t *hex);

#endif /* TELIT_SDK_TEST_HELPERS_H */
