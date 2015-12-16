#!/bin/sh

cd $(dirname "$0")/../misc/linice-kdb/ &&
find "`pwd`" -name "*.[chxsS]" -print | sort > cscope.files &&
cscope -b -q -k &&
ls -lhs cscope*

retv=$?
echo "Exit status: $retv"

exit $retv
