examples = [
  'qrtr_dms_get_device_rev_id.c',
  'qrtr_nas_indication.c'
]

foreach src: examples
  name = src.split('.')[0]
  exe = executable(
    name,
    src,
    c_args: ['-D_GNU_SOURCE'],
    include_directories:include_directories(['../../sdk', '../../qrtr']),
    link_with: [libtelitSDK, libtqrtr])
endforeach
