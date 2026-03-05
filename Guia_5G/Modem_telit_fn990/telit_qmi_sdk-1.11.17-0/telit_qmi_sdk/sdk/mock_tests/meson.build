mock_tests = [
  'dms_modem_activity_info_ind.c',
  'gms_nas_get_modem_status.c',
  'nas_get_nr5g_rrc_utc_time.c',
  'nas_nr5g_lost_frame_sync_ind.c',
  'nas_nr5g_time_sync_pulse_report_ind.c',
  'wms_memory_full_ind.c',
]

foreach src : mock_tests
  name = src.split('.')[0]
  exe = executable(
    name,
    src,
    include_directories: include_directories('../'),
    link_with:[libtelitSDK, libtesthelpers],
    dependencies: libtesthelpers_dep)
  test(name, exe)
endforeach
