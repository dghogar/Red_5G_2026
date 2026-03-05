srcs = [
  'telit_sdk_qrtr.c',
]

inc = include_directories(['../sdk'])
dep = dependency('threads')

if get_option('shared')
  libtqrtr = shared_library(
    'tqrtr',
    srcs,
    c_args: extra_c_args,
    include_directories: inc,
    dependencies: dep,
    link_with: libtelitSDK)
else
  libtqrtr = static_library(
    'tqrtr',
    srcs,
    c_args: extra_c_args,
    include_directories: inc,
    dependencies: dep,
    link_with: libtelitSDK)
endif

subdir('examples')

