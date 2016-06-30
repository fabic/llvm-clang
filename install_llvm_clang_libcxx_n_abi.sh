#!/bin/bash
#
# FabiC.2016-06-20
#
# http://blog.fabic.net/diary/2016/06/20/build-llvm-libcxx-and-abi

here=$(cd `dirname "$0"` && pwd)

echo "+--- $0"
echo "|"
echo "| This script builds LLVM's libcxx & libcxxabi"
echo "|"

cd "$here/llvm-clang" || exit 1

localdir="$(cd ../local/ && pwd)"

echo "| Entered `pwd`/"

echo "|"
echo "| Git submodule checkout llvm, libcxx, libcxxabi..."
echo "|"

if ! git submodule update --init llvm libcxx libcxxabi ;
then
  retv=$?
  echo "| FAIL: Git submodule exited with status : $retv"
  echo "+-"
  exit $retv
else
  echo "| Done with Git submodule checkout, ok."
  echo "+-"
fi


# Sub-projects are symlinked there :
if [ ! -d llvm/projects ]; then
	echo "| Oups! LLVM sub-dir. 'llvm/projects/' does not exist, exiting..."
	exit 127
fi


echo "|"
echo "| Dropping 2 symlinks libcxx & libcxxabi under 'llvm/projects/' :"

  ln -sfvn ../../libcxx    llvm/projects/ &&
  ln -sfvn ../../libcxxabi llvm/projects/ &&
  ls -l llvm/projects/libcxx*


if [ -d build ]; then
  echo "| (REMOVING existing temporary build/ directory)."
  rm -rf build
fi

echo "|"
echo "| Entering temporary build/ directory."

if ! mkdir build || ! cd build/ ;
then
  echo "| FAILED to create and/or enter the temporary build/ dir."
  exit 126
else
  echo "| Ok, current dir. is '`pwd`'"
fi

echo "|"
echo "| Running CMake..."
echo "|"

time \
	cmake \
	  -DCMAKE_BUILD_TYPE=Release \
	  -DCMAKE_INSTALL_PREFIX="$localdir" \
	  -G Ninja \
	  ../llvm

retv=$?
if [ $retv -ne 0 ]; then
  echo "| CMake failed, exit status $retv"
  exit $retv
fi

echo "|"
echo "| Running Ninja..."
echo "|"

time \
  ninja -j4

retv=$?
if [ $retv -ne 0 ]; then
  echo "| Build failed, Ninja exited with status $retv"
  exit $retv
fi

echo "+--- FINISHED, exit status: $retv"

exit $retv
# vim: et sw=2 ts=2
