#!/bin/bash
#
# 2017-02-16 fabic

rewt="$( cd `dirname "$0"`/.. && pwd )"

# Relative path from a sub-directory and up to rewt.
rel=${PWD#$rewt}
rel=${rel//[^\/]/}
rel=${rel//\//..\/}
rel=${rel%%/}

args=( "$@" )

cd "$here" || exit 1

if ! grep --color=always -rHnPi "${args[@]}" "$rel/lib/musl/" ;
then
  retv=$?
  echo
  echo "# ERROR: grep exit status is non-zero: $retv"
  exit $retv
fi

