srcs = [
    'log.c',
    'telit_sdk_atr.c',
    'telit_sdk_cat.c',
    'telit_sdk_core.c',
    'telit_sdk_ctl.c',
    'telit_sdk_dms.c',
    'telit_sdk_dpm.c',
    'telit_sdk_fota.c',
    'telit_sdk_gas.c',
    'telit_sdk_gms.c',
    'telit_sdk_loc.c',
    'telit_sdk_nas.c',
    'telit_sdk_qos.c',
    'telit_sdk_tmd.c',
    'telit_sdk_uim.c',
    'telit_sdk_voice.c',
    'telit_sdk_wda.c',
    'telit_sdk_wds.c',
    'telit_sdk_wms.c',
    'telit_sms_helper_3gpp2.c',
    'telit_sms_helper.c',
    'telit_sms_helper_private.c',
]

if cc.has_function('strlcpy', prefix: '#include <string.h>')
  add_project_arguments('-DHAVE_STRLCPY', language : 'c')
else
  srcs += ['strlcpy.c']
endif

includes = [include_directories(['.', '../'])]

if get_option('shared')
  libtelitSDK = shared_library(
    'telitSDK',
    srcs,
    c_args: extra_c_args,
    include_directories: includes)
else
  libtelitSDK = static_library(
    'telitSDK',
    srcs,
    c_args: extra_c_args,
    include_directories: includes)
endif

libtelitSDK_dep = declare_dependency(
  include_directories: includes,
  link_with: libtelitSDK)

subdir('examples')
subdir('gobiusbnet_examples')
