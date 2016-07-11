#!/bin/bash

here=$( cd `dirname "$0"` && pwd )

echo "+-- $0"

cd "$here/misc/libunwind/" &&
test -e .git &&
git show-branch --current `git rev-parse --abbrev-ref --symbolic-full-name @{u}` &&
git status &&
echo "| Ok, proceeding..." || exit 127

echo "| autoreconf -i"

autoreconf -i

retv=$?
if [ $retv -gt 0 ]; then
	echo "| autoreconf failed with exit status $retv"
	exit $retv
fi

echo "| ./configure ..."

./configure --prefix=$here/local \
	--enable-debug --enable-cxx-exceptions --enable-coredump \
	--enable-debug-frame --enable-block-signals --enable-conservative-checks

retv=$?
if [ $retv -gt 0 ]; then
	echo "| ./configure ... failed with exit status $retv"
	exit $retv
fi

let processor_count=$( cat /proc/cpuinfo | grep -P '^processor\s*:\s*\d+' | wc -l )
let jobs_count="$processor_count - 1"
if [ "0$jobs_count" -lt 1 ]; then
	jobs_count=1
fi

echo "|"
echo "| Found out we have $processor_count CPUs ;"
echo "| we'll set -j$jobs_count (max. simultaneous jobs)."
echo "|"
echo "| Running make"
echo "|"

time \
	make -j$jobs_count

retv=$?
if [ $retv -gt 0 ]; then
	echo "| make failed with exit status $retv"
	exit $retv
fi

echo "|"
echo "| make install"
echo "|"

make install

retv=$?
if [ $retv -gt 0 ]; then
	echo "| make install failed with exit status $retv"
	exit $retv
fi

echo
echo "| FINISHED, exit status $retv"
echo

exit $retv
