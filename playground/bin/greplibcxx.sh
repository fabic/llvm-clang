#!/bin/bash
#
# 2017-02-17 fabic

here="$( cd `dirname "$0"` && pwd )"

args=( "$@" )

cd "$here" || exit 1

if ! grep --color=always -rHnPi "${args[@]}" ../../lib/libcxx{,abi}/ ;
then
  retv=$?
  echo
  echo "# ERROR: grep exit status is non-zero: $retv"
  exit $retv
fi

