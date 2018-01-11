#!/bin/bash
#

here=$( cd `dirname "$0"` && pwd )

cd "$here" || exit 1

../bin/run_ctags.sh \
  "$here" \
  $(cd ../local/include/ && pwd) \
  /usr/include/xcb \
  /usr/include/X11 \
  $(cd ../local/boost-1.61.0-clang/include/boost/ && pwd) \
  $(cd ../misc/cpp-netlib/boost/ && pwd) \
  /usr/include/glibmm-2.4 \
  /usr/include/gdkmm-3.0 \
  /usr/include/gtkmm-3.0 \
  /usr/include/pangomm-1.4 \
  "$@"

retv=$?

echo "+-- $0 : END, exit status: $retv"

exit $retv
