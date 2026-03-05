#!/usr/bin/env bash

set -eu -o pipefail

if [ -v MESON_SOURCE_ROOT ]; then
    cd "$MESON_SOURCE_ROOT"
fi

cp -r sdk sdk_docs
cp tqcm/*.h sdk_docs
rm sdk_docs/{tqcm_private.h,tqcm_ctl.h,*.c,telit_sdk_private*,telit_sms_helper_private.h}

doxygen telit_qmi_sdk_doxy

rm -r sdk_docs
