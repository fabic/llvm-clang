#!/bin/sh

here=$( cd `dirname "$0"` && pwd )

echo "+-- $0"

cd "$here/cpp-netlib/" &&
test -e .git &&
git describe &&
git show-branch --current `git rev-parse --abbrev-ref --symbolic-full-name @{u}` &&
git status &&
echo "| Ok, proceeding..." || exit 127

if [ -d build ]; then
	echo "| Removing the build/ dir."
	rm -rf build
fi

mkdir build &&
cd build || exit 126

# Note that having the env. var. BOOST_ROOT won't do it, we have to pass the arg. -DBOOST_ROOT=... (unfortunately).
# Note: see also their build.sh shell script.
#cmake -DCMAKE_BUILD_TYPE=Debug -DBOOST_ROOT=$BOOST_ROOT ..

time \
	cmake -DCMAKE_BUILD_TYPE=Debug ..

retv=$?

if [ $retv -gt 0 ]; then
	echo "| CMake failed with exit status $retv"
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

time \
	make -j$jobs_count

retv=$?

echo
echo "| FINISHED, exit status $retv"
echo

exit $retv
