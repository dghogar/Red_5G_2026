examples = [
  'ctl_get_client_id.c',
  'ctl_get_version_info.c',
  'ctl_release_client_id.c',
  'dms_get_fw_revision.c',
]

foreach src : examples
  name = src.split('.')[0]
  exe = executable(
    name,
    src,
    include_directories: include_directories('../'),
    link_with:libtelitSDK)
endforeach


