srcs = [
  'tqcm_atr.c',
  'tqcm_cat.c',
  'tqcm_core.c',
  'tqcm_ctl.c',
  'tqcm_dispatcher.c',
  'tqcm_dms.c',
  'tqcm_dpm.c',
  'tqcm_fota.c',
  'tqcm_gas.c',
  'tqcm_gms.c',
  'tqcm_loc.c',
  'tqcm_nas.c',
  'tqcm_uim.c',
  'tqcm_voice.c',
  'tqcm_wda.c',
  'tqcm_wds.c',
  'tqcm_wms.c',
]

inc = ['.', '../sdk']
dep = dependency('threads')
to_link = [libtelitSDK]

if build_qrtr
  inc += '../qrtr'
  to_link += [libtqrtr]
endif

inc_dirs = include_directories(inc)

if get_option('shared')
  libtqcm = shared_library(
    'tqcm',
    srcs,
    c_args: extra_c_args,
    include_directories: inc_dirs,
    dependencies: dep,
    link_with: to_link)
else
  libtqcm = static_library(
    'tqcm',
    srcs,
    c_args: extra_c_args,
    include_directories: inc_dirs,
    dependencies: dep,
    link_with: to_link)
endif

libtqcm_dep = declare_dependency(
  include_directories: inc_dirs,
  link_with: to_link + libtqcm)

subdir('examples')
