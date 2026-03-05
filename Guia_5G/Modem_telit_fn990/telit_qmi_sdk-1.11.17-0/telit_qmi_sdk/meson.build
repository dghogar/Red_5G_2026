project('telit_qmi_sdk', 'c', default_options: ['buildtype=release', 'optimization=2'])

cc = meson.get_compiler('c')
extra_c_args = ['-D_GNU_SOURCE']
if get_option('buildtype').startswith('debug')
    add_project_arguments('-DEXIT_ON_ERROR', language: 'c')
endif
if get_option('stack-usage')
  if cc.has_argument('-fstack-usage')
    extra_c_args += ['-fstack-usage']
  endif
endif

# QRTR build only if kernel version >=5.0.0
build_qrtr = false
r = run_command('uname', '-r', check: true)
if r.returncode() != 0
  warning('could not get kernel version. Skipping qrtr build.')
else
  output = r.stdout().strip()
  kver = output.split('-')[0]
  build_qrtr = kver.version_compare('>=5.0.0')
  if build_qrtr
    message('Kernel', kver, 'is new enough to build QRTR')
    extra_c_args += ['-DTELIT_QRTR']
  else
    warning('Kernel', kver, 'is too old to build QRTR')
  endif
endif

subdir('sdk')
subdir('test_helpers')
subdir('sdk/mock_tests')
if build_qrtr
  subdir('qrtr')
endif
subdir('tqcm')

# inside build directory run: meson compile docs
script = 'makedocs.sh'
fs = import('fs')
if fs.is_file(script)
  run_target('docs', command: script)
endif
