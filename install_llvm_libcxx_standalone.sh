#!/bin/bash
#
# FabiC.2016-06-30
#
# http://blog.fabic.net/diary/2016/06/20/build-llvm-libcxx-and-abi
#
# http://stackoverflow.com/questions/25840088/how-to-build-libcxx-and-libcxxabi-by-clang-on-centos-7

here=$(cd `dirname "$0"` && pwd)

echo "+--- $0"
echo "|"

  cd "$here/llvm-clang" || exit 1

  localdir=${1:-"$(cd ../local && pwd)"}

echo "| Entered `pwd`/"
echo "| \$localdir = '$localdir'"
echo "|"
echo "| This script builds LLVM's libcxx _out of_ the LLVM tree."
echo "|"
echo "| It will perform 3 steps :"
echo "|"
echo "|   1) build libcxx against libstdc++ (hence without libcxxabi)."
echo "|      (this one won't get installed, just built)"
echo "|   2) build libcxxabi against that temporary libcxx build."
echo "|   3) re-build libcxx against our fresh build of libcxxabi."
echo "|      (and install the whole stuff, under '$localdir')"
echo "|"

read -p "Ok to proceed ? (Ctrl-C to abort)"


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


###
## Step #1 : build libcxx without libcxxabi.
#

if true; then
  pushd .

    echo "+-"
    echo "| Step #1: First pass, building libcxx without libcxxabi"
    echo "|"

    cd libcxx || exit 127

    if [ -d build ]; then
      echo "| (REMOVING existing temporary `pwd`/build/ directory)."
      rm -rf build || exit 127
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
          -DLIBCXX_CXX_ABI=libstdc++ \
          -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_INSTALL_PREFIX="$localdir" \
          -DLLVM_PATH=../../llvm \
          -G Ninja \
          ..

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

    echo "| Step #1 finished temporary build of libcxx (without libcxxabi)."
    echo "| (Note that we're _NOT_ installing this one this time "
    echo "|       i.e. not running Ninja install )"
    echo "+-"

  popd
fi
# ^ end of step #1.
##


###
## Step #2 : building libcxxabi against the temporary libcxx
#
if true; then
  pushd .

    echo "+-"
    echo "| Step #2: building libcxxabi _against_ the temporary libcxx."
    echo "|"

    cd libcxxabi || exit 127

    if [ -d build ]; then
      echo "| (REMOVING existing temporary `pwd`/build/ directory)."
      rm -rf build || exit 127
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
          -DLIBCXXABI_LIBCXX_PATH=../../libcxx \
          -DLLVM_PATH=../../llvm \
          -G Ninja \
          ..

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

    echo "|"
    echo "| Running Ninja install..."
    echo "|"

      ninja install

      retv=$?
      if [ $retv -ne 0 ]; then
        echo "|"
        echo "| FAIL: Ninja install (of libcxxabi at step #2) failed, exit status $retv"
        echo "+-"
        exit $retv
      fi

    echo "| Step #2 finished build of libcxxabi against the temporary libcxx."
    echo "+-"

  popd
fi
# ^ end of step #2.
##


###
## Step #3 : re-building libcxx against libcxxabi
#
if true; then
  pushd .

    echo "+-"
    echo "| Step #3: re-building libcxx against libcxxabi."
    echo "|"

    cd libcxx || exit 127

    if [ -d build ]; then
      echo "| (REMOVING existing temporary `pwd`/build/ directory)."
      rm -rf build || exit 127
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
        -DLLVM_PATH=../../llvm \
        -DLIBCXX_CXX_ABI=libcxxabi \
        -DLIBCXX_CXX_ABI_INCLUDE_PATHS=../../libcxxabi/include \
        -DLIBCXX_CXX_ABI_LIBRARY_PATH=../../libcxxabi/build/lib \
        -DLIBCXXABI_USE_LLVM_UNWINDER=ON \
        -G Ninja \
        ..

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

    echo "|"
    echo "| Running Ninja install..."
    echo "|"

      ninja install

      retv=$?
      if [ $retv -ne 0 ]; then
        echo "|"
        echo "| FAIL: Ninja install (of libcxx at step #3) failed, exit status $retv"
        echo "+-"
        exit $retv
      fi

    echo "| Step #3 finished re-build of libcxx against libcxxabi."
    echo "+-"

  popd
fi
# ^ end of step #3.
##

echo "|"
echo "| Ok done, here's the output of \`ldd \"$localdir/lib/libc++.so.1\"\` :"

ldd "$localdir/lib/libc++.so.1" | \
  sed -e 's/^/|  &/'

echo "| ^ Hopefully you would see that it linked against libc++abi.so.1"
echo "|"
echo "+--- $0 : FINISHED, exit status: $retv"

exit $retv
# vim: et sw=2 ts=2
