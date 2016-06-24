#!/bin/sh

here=$( cd `dirname "$0"` && pwd )

echo "+-- $0"

cd "$here/misc/cpp-netlib/" &&
test -e .git &&
git describe &&
git show-branch --current `git rev-parse --abbrev-ref --symbolic-full-name @{u}` &&
git status &&
echo "| Ok, proceeding..." || exit 127

if [ -d build ]; then
    echo "| Removing the build/ dir."
    rm -rf build
fi

echo "|"
echo "| Entering build/ dir."
echo "|"

mkdir build &&
cd build || exit 126

echo "|"
echo "| About to invoke CMake with various arguments."
echo "|"
echo "| NOTE: see also _their_ build.sh script @ '`pwd`/../build.sh'"
echo "|"

# Note that having the env. var. BOOST_ROOT won't do it, we have to pass the arg. -DBOOST_ROOT=... (unfortunately).
# Note: see also their build.sh shell script.
#cmake -DCMAKE_BUILD_TYPE=Debug -DBOOST_ROOT=$BOOST_ROOT ..
#-DBOOST_INCLUDEDIR="$BOOST_ROOT/include" \
#-DBOOST_LIBRARYDIR="$BOOST_ROOT/lib"     \

time \
    cmake .. \
        -DCMAKE_BUILD_TYPE=Debug  \
        -DBOOST_ROOT="$BOOST_ROOT"  \
        -DBOOST_INCLUDEDIR="$BOOST_ROOT/include" \
        -DBOOST_LIBRARYDIR="$BOOST_ROOT/lib"     \
        -DCMAKE_CXX_FLAGS="-std=c++1y -stdlib=libc++" \
        -DBUILD_SHARED_LIBS=ON          \
        -DCPP-NETLIB_ENABLE_HTTPS=OFF   \
        -DCPP-NETLIB_BUILD_EXAMPLES=OFF \
        -DCPP-NETLIB_BUILD_DOCS=OFF     \
        -DUri_BUILD_TESTS=OFF           \
        -DUri_BUILD_DOCS=OFF            \
        -DUri_DISABLE_LIBCXX=OFF

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
# vim: et sw=4 ts=4
