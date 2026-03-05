REM To be compiled with a VS Native Tools Command Prompt

@echo off

set name=telit-qmi
set OS=W

if "%1"=="clean"  (
    del %name%.lib
    goto end
)

set CFLAGS=-I. -I.. -D__WINDOWS_QMI__

cl %CFLAGS% -Folog.obj -FS -c log.c /TC
for %%s in (telit_sdk_*.c) do (
    cl %CFLAGS% -Fo%%~ns.obj -FS -c %%~ns.c /TC
)

link -lib -nologo -out:%name%.lib ^
    advapi32.lib setupapi.lib user32.lib shlwapi.lib Winmm.lib ^
    telit_sdk_core.obj telit_sdk_ctl.obj telit_sdk_dms.obj telit_sdk_loc.obj telit_sdk_nas.obj telit_sdk_uim.obj telit_sdk_wda.obj telit_sdk_wds.obj telit_sdk_wms.obj telit_sdk_voice.obj telit_sdk_qos.obj telit_sdk_tmd.obj telit_sdk_fota.obj telit_sdk_gas.obj telit_sdk_gms.obj telit_sdk_atr.obj log.obj

del *.obj
:end
