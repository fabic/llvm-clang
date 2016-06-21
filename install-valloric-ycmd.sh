#!/bin/bash
#
# FabiC.2016-06-21
#
# https://github.com/Valloric/ycmd
#
# https://packagecontrol.io/packages/C%2B%2BYouCompleteMe
# https://packagecontrol.io/packages/YcmdCompletion

here=$(cd `dirname "$0"` && pwd)

echo "+--- $0"
echo "|"
echo "| Valloric'r ycmd"
echo "|   « a code-completion & comprehension server »"
echo "|"
echo "| https://github.com/Valloric/ycmd"
echo "|"
echo "| For context-aware C++ auto-completion from Sublime Test 3 by means of any one of these plugins :"
echo "|"
echo "|    C++YouCompleteMe : https://packagecontrol.io/packages/C%2B%2BYouCompleteMe"
echo "|    YcmdCompletion   : https://packagecontrol.io/packages/YcmdCompletion"
echo "|"


if ! git submodule update --init --recursive misc/valloric-ycmd/ ; then
    echo "| Git submodule update failed with exit status $?"
    exit 127
fi


if ! cd misc/valloric-ycmd/ ; then
    echo "| Failed to ch. dir. into misc/valloric-ycmd/"
    exit 126
fi

echo "|"
echo "| Ok, about to invoke :"
echo "|"
echo "|     ./build.py --all"
echo "|"


time \
    ./build.py --all




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
