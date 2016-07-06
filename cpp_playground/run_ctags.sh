#!/bin/bash
#

here=$( cd `dirname "$0"` && pwd )

cd "$here" || exit 1

../bin/run_ctags.sh \
  "$here" \
  /usr/include/cairomm-1.0 \
  /usr/include/xcb \
  /usr/include/cairo \
  /usr/include/X11 \
  /usr/include/glibmm-2.4 \
  /usr/include/gdkmm-3.0 \
  /usr/include/gtkmm-3.0 \
  /usr/include/pangomm-1.4 \
  $(cd ../misc/cpp-netlib/boost/ && pwd) \
  $(cd ../local/include/ && pwd) \
  $(cd ../local/boost-1.61.0-clang/include/boost/ && pwd) \
  "$@"

retv=$?

echo "+-- $0 : END, exit status: $retv"

exit $retv
