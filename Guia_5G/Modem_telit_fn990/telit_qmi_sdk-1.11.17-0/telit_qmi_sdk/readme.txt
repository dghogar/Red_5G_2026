Telit QMI SDK and TQCM

For build instructions, refer to Makefile or to the user guide

CHANGELOG

Version 1.11.17-0 (2025-03-05)

- Fix for requests sending issue with an invalid client context

Version 1.11.16-0 (2025-02-20)

- Fix compiler warning
- Fix docs folder building and cleaning

Version 1.11.15-0 (2025-02-17)

- Fix leak in tqcm_generic_send
- Fix possible race condition in decode response functions

Version 1.11.14-0 (2025-02-12)

- Fix additional compiler warnings

Version 1.11.13-0 (2025-01-31)

- Fix -Wextra and -Wall warnings

Version 1.11.12-0 (2025-01-24)

- Add support for QMI_UIM_LOGICAL_CHANNEL
- Add support for QMI_UIM_SEND_APDU

Version 1.11.11-0 (2025-01-10)

- Add support for QMI_UIM_CHANGE_PROVISIONING_SESSION

Version 1.11.10-0 (2024-11-22)

- Increase timeout for telit_voice_generic_get_service_pack

Version 1.11.9-0 (2024-11-21)

- Add support for QMI_CAT_SET/GET_CONFIGURATION
- Add support for QMI_CAT_GET_CACHED_PROACTIVE_CMD
- Add support for QMI_CAT_SEND_TR
- Add support for QMI_CAT_SEND_ENVELOPE_CMD
- Add support for QMI_CAT_EVENT_REPORT_IND

Version 1.11.8-0 (2024-10-16)

- Add pcscf info to QMI_WDS_GET_RUNTIME_SETTINGS

Version 1.11.7-0 (2024-10-08)

- sdk,nas: add plmn info to QMI_NAS_GET_SYSTEM_INFO

Version 1.11.6-0 (2024-09-12)

- Fix tqcm_nas_get_nr5g_rrc_utc_time return value
- Add support for QMI_NAS_NETWORK_TIME_IND
- Add support for QMI_NAS_GET_NETWORK_TIME

Version 1.11.5-0 (2024-06-24)

- sdk, nas: add TLVs 0x4E, 0x4F, 0x55 to NAS_GET_SYSTEM_INFO
- sdk: nas: add TLVs 0x2E, 0x2F to QMI_NAS_GET_CELL_LOCATION_INFO
- tqcm: data_connection_example: code cleaning

Version 1.11.4-0 (2024-06-17)

- tqcm: add iface_id argument to data connection example

Version 1.11.3-0 (2024-06-05)

- tqcm: fix surprisal device removal SIGSEGV when request pending
- sdk: add tlv 0x22 and 0x23 to QMI_NAS_INDICATION_REGISTER
- build: bugfixes and improvements

Version 1.11.2-0 (2024-05-08)

- wds: increased QMI_WDS_STOP_NETWORK timeout

Version 1.11.1-0 (2024-04-23)

- loc: add QMI_LOC_EVENT_GNSS_SV_INFO_IND extended satellite info TLV

Version 1.11.0-0 (2024-04-12)

- nas: add QMI_NAS_NR5G_RRC_UTC_TIME_UPDATE_IND support
- nas: add QMI_NAS_GET_NR5G_RRC_UTC_TIME support
- tqcm: fix null reference in decode_response item
- tqcm: fix possible null reference
- Fix handling of some errors
- Some code refactoring

Version 1.10.5-0 (2024-04-08)

- Code cleaning
- dms: fix telit_dms_get_band_capability_unpack
- nas: add NR5G SA/NSA TLVs to system_sel_pref

Version 1.10.4-0 (2024-02-16)

- nas: increase QMI_NAS_MAX_CELLS to 9
- Add QMI_DMS_DEVICE_POWER_INFO support
- Add QMI_NAS_LIMIT_SYS_INFO_IND_REPORTING support

Version 1.10.3-0 (2024-01-24)

- Add QMI_WDS_GET_DEFAULT_PROFILE_NUM
- Add QMI_WDS_GET_LTE_ATTACH_PDN_LIST support
- Add QMI_WDS_SET_LTE_ATTACH_PDN_LIST support
- sdk: increase max number of used satellites in loc_position_report_t
- Add position source to QMI_LOC_INJECT_POSITION
- Add QMI_LOC_EVENT_INJECT_POSITION_* support
- Add QMI_LOC_EVENT_INJECT_TIME_REQ_IND support
- Add QMI_LOC_INJECT_UTC_TIME support
- sdk: avoid search_TLV debug prints in Android
- loc: add min_interval to QMI_LOC_SET_OPERATION_MODE
- Build: bugfixes

Version 1.10.2-0 (2023-12-18)

- tqcm: add example for QMI_NAS_SYS_INFO indication
- tqcm: fix Meson support for building tqcm examples
- meson: build with gcc -O2 instead of -O3

Version 1.10.1-0 (2023-11-15)

- Add support for QMI_GMS_NAS_SET_MCCBLOCK and QMI_GMS_NAS_GET_MCCBLOCK

Version 1.10.0-0 (2023-11-09)

- tqcm: fix input in wds_bind_mux_data_port example
- tqcm: fix transaction id management between request and reply
- sdk: fix transaction id loading for CTL service
- sdk: add sys_info option to nas_indication_register request
- Add support for QMI_GMS_NAS_GET_NR5G_CA_INFO
- Add support for QMI_GMS_NAS_GET_ENDC_CA_INFO
- Add QMI_NAS_SYS_INFO_IND indication handler

Version 1.9.2-0 (2023-09-06)

- Add downlink minimum padding TLV in wda_set_data_format() command
- Add NR5G info to gms_nas_get_modem_status() command
- Add support for Alpine Linux (musl)
- Improve tqcm examples
- Refactor Makefiles
- Minor fixes
- Improve code style

Version 1.9.1-0 (2023-05-23)

- Fix for Windows build failure in function telit_nas_set_nr5g_sync_pulse_gen_pack

Version 1.9.0-0 (2023-05-04)

- Added QMI_DMS_TRIGGER_MODEM_ACTIVITY_INFO_CALCULATION,
QMI_DMS_GET_MODEM_ACTIVITY_INFO, QMI_DMS_MODEM_ACTIVITY_INFO_IND,
QMI_WMS_MEMORY_FULL_IND
- Increase timeout for QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE

Version 1.8.1-0 (2022-08-10)

- Added QMI_NAS_CONFIG_SIG_INFO2
- Fixed QMI_NAS_SIG_INFO_IND id

Version 1.8.0-0 (2022-06-17)

- Added QMI_WDS_INDICATION_REGISTER
- Added QMI_WDS_APN_PARAM_INFO_CHANGE_IND
- Increased timeout for set operating mode and voice get service requests
- Increased maximum number of wds profiles
- Added search type capabilites to WDS_GET_PROFILE_LIST
- Added QMI_UIM_POWER_DOWN/UP

Version 1.7.1-0 (2022-02-24)

- Added error code QMI_SDK_ERR_UNSUPPORTED for unsupported operations

Version 1.7.0-0 (2022-02-18)

- Added QMI_GAS_DMS_SET_USB_EXT and QMI_GAS_DMS_GET_USB_EXT support
- Added QMI_LOC_REGISTER_MASTER_CLIENT support
- Fixed buffer overflow for AT commands indication

Version 1.6.1-0 (2021-11-18)

- Moved data_connection.con to data connection example directory
- Missing header warning fixed

Version 1.6.0-0 (2021-11-12)

- Added AT Relay service support
- Added TQCM unified data connection example
- Removed previous data connection examples

Version 1.5.2-0 (2021-10-08)

- Implemented QMI_GMS_LOC_SET_GNSS_SESSION, QMI_GMS_LOC_GET_GNSS_SESSION, QMI_GMS_LOC_GET_ACQ_POSITION

Version 1.5.1-0 (2021-07-13)

- Implement QMI_GMS_DMS_FWSWITCH_INFO_EX

Version 1.5.0-0 (2021-06-16)

- Added fota, gas and gms services to tqcm
- Added voice, voice, qos, tmd, fota, gas and tcm services to Windows build
- Minor bugfixes

Version 1.4.2-0 (2021-03-24)

- Demote error message to warning in read_qrtr_thread_start

Version 1.4.1-0 (2021-03-17)

- Updated Android.bp
- Small cosmetic fixes

Version 1.4.0-0 (2021-02-03)

- Fixed wrong response decoding when only mandatory TLV available
- Fixed telit_dpm_open_port_pack
- Version format changed to: <major>.<minor>.<patch>-<custom>

Version 1.0.3.1 (2021-01-11)

- Fixed documentation related to QRTR

Version 1.0.3.0 (2021-01-11)

- PCIe and QRTR support added
- Windows support (experimental)

Version 1.0.2.1 (2020-10-19)

- Bugfixing

Version 1.0.2.0 (2020-05-09)

- Added Android support
- Added several QMI requests (WDA, VOICE)
- Added data connection example
- Added gobi related examples
- Bugfixing

Version 1.0.1.0 (2020-01-14)

- Refactor pack prototypes to use uint8_t* instead of void*
- Refactor folder names
- Removed latex generation for docs
- Minor fixes

Version 1.0.0.0 (2019-12-18)

- First release
