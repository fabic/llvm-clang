#!/usr/bin/env bash

here=$(cd `dirname "$0"` && pwd)

cd "$here" || exit 1

builddir="build"

[ ! -z "$builddir" ] || exit 2

echo "+-- $0 $@"
echo "| Trying simple-build-cmake-project.sh [ninja|make] [rebuild] -- [...]"

if true && type -p simple-build-cmake-project.sh > /dev/null ;
then
  echo "| FOUND IT ! ( `type -p simple-build-cmake-project.sh` )"
  echo "+-"
  echo

  simple-build-cmake-project.sh "$@"
  retv=$?
else
  echo "+~~~> FOUND NO WAY TO BUILD YOUR STUFF, EXITING..."
  retv=$?
fi

# Display exit code.
if [ $retv -gt 0 ];
then
  echo
  echo "+~> "$0 $@" : FAILED, exit status : $retv"
  echo
  exit $retv
fi

