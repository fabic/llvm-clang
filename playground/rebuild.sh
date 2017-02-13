#!/bin/bash

here=$(cd `dirname "$0"` && pwd)

cd "$here" || exit 1

builddir="BUILD"

[ ! -z "$builddir" ] || exit 2

if [ -d "$builddir" ];
then
  echo "| REMOVING build dir. '$builddir'"
  rm -rf "$builddir" || exit 3
fi

[ -x build.sh ] || exit 4

./build.sh "$@"

retv=$?

if [ $retv -gt 0 ];
then
  echo
  echo "+~> "$0 $@" : FAILED, build.sh exited w/ status $retv"
  echo
  exit $retv
fi

