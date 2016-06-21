#!/bin/bash
#
# FabiC.2016-06-20
#
# http://blog.fabic.net/diary/2016/06/20/build-llvm-libcxx-and-abi

here=$(cd `dirname "$0"` && pwd)

echo "+--- $0"

git submodule update --init llvm libcxx libcxxabi

if [ ! -d llvm/projects ]; then
	echo "| Oups! LLVM sub-dir. 'llvm/projects/' does not exist, exiting..."
	exit 127
fi

echo "| Dropping 2 symlinks libcxx & libcxxabi under 'llvm/projects/'"

ln -sfvn ../../libcxx    llvm/projects/ &&
ln -sfvn ../../libcxxabi llvm/projects/ &&
ls -l llvm/projects/libcxx*

echo "|"

mkdir build &&
cd build/

echo "|"
echo "| Running CMake..."
echo "|"

cmake -G Ninja ../llvm

echo "|"
echo "| Running Ninja..."
echo "|"

time \
	ninja

retv=$?

echo "+--- FINISHED, exit status: $retv"

exit $retv
