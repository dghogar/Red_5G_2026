srcs = [
    'telit_sdk_test_helpers.c',
]

includes = [include_directories('.')]

if get_option('shared')
  libtesthelpers = shared_library(
    'testhelpers',
    srcs,
    c_args: extra_c_args,
    dependencies: libtelitSDK_dep)
else
  libtesthelpers = static_library(
    'testhelpers',
    srcs,
    c_args: extra_c_args,
    dependencies: libtelitSDK_dep)
endif

libtesthelpers_dep = declare_dependency(
  include_directories: includes,
  link_with: libtesthelpers)
