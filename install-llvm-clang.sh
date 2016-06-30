#!/bin/sh
#
# fabic/2016-06-30

here=$(cd `dirname "$0"` && pwd)

. "$here/functions.sh"

echo "+--- $0"
echo "|"
echo "| This script builds the whole LLVM Clang compiler"
echo "|"

  cd "$here/llvm-clang" || exit 1

  localdir=${1:-"$(mkdir -p "$here/local" && cd "$here/local" && pwd)"}

echo "| Entered `pwd`/"
echo "| \$localdir = '$localdir'"

echo "|"
echo "| FYI: Here's the value of sensitive environment variables :"
echo "|"
echo "|   \$CC  = $CC"
echo "|   \$CXX = $CXX"
echo "|"
echo "|   \$CPATH = $CPATH"
echo "|   \$CPLUS_INCLUDE_PATH = $CPLUS_INCLUDE_PATH"
echo "|"
echo "|   \$LD_RUN_PATH     = $LD_RUN_PATH"
echo "|   \$LD_LIBRARY_PATH = $LD_LIBRARY_PATH"
echo "|"
echo "|   ^ please ensure that somehow... (TODO: talk!)"
echo "|"
echo "| Note that you may [re-]source the shell script '$here/environment-clang.sh'"
echo "| which may set things up correctly if you're lucky."
echo "+-"


  # Ask/warn early about an eventually existing bootstrap/ directory.
  if [ -d bootstrap ]; then
    read -p "| REMOVING existing temporary `pwd`/bootstrap/ directory."
    rm -rf bootstrap || exit 127
  fi

  # Ask/warn early about an eventually existing build/ directory.
  if [ -d build ]; then
    read -p "| REMOVING existing temporary `pwd`/build/ directory."
    rm -rf build || exit 127
  fi


echo
read -p "Ok to proceed ? (Ctrl-C to abort)"
echo


echo "|"
echo "| Git submodule checkout llvm clang clang-tools-extra compiler-rt libcxx libcxxabi..."
echo "|"

  if ! git submodule update --init \
    llvm clang clang-tools-extra compiler-rt \
    libcxx libcxxabi ;
  then
    retv=$?
    echo "| FAIL: Git submodule exited with status : $retv"
    echo "+-"
    exit $retv
  fi

echo "| Done with Git submodule checkout, ok."
echo "+-"

echo "|"
echo "| Dropping symlinks here'n'there :"
echo "|"
echo

  ln -sfnv ../../clang              llvm/tools/        &&
  ln -sfnv ../../clang-tools-extra/ clang/tools/extra  &&
  ln -sfnv ../../compiler-rt        llvm/projects/     &&
  ln -sfnv ../../libcxx{,abi}       llvm/projects/

  retv=$?
  echo
  if [ $retv -ne 0 ]; then
    echo "| Ooops! something went wrong with the symlinking, exiting."
    exit $retv
  fi


###
## Step #1 : Build a bootstrap version under bootstrap/

if true; then
  # TODO...
  # TODO...
  # TODO...
fi

# ^ end of step #1 bootstrap build.
##

if true; then
  pushd .

    echo "+-"
    echo "| Step #2 : Final build of LLVM/Clang."
    echo "|"
    echo "| Entering temporary build/ directory."

      if ! mkdir build || ! cd build/ ;
      then
        echo "+---> FAILED to create and/or enter the temporary build/ dir."
        exit 126
      fi

    echo "| Ok, current dir. is now '`pwd`'"
    echo "|"
    echo "+ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    echo "|"
    echo "| Running CMake..."
    echo "|"
    echo "| Note that we're passing the following -Dxxx CMake options :"
    echo "|   - LLVM_ENABLE_FFI=OFF       (default)"
    echo "|   - BUILD_SHARED_LIBS=ON      (defaults to OFF)"
    echo "|   - LLVM_BUILD_LLVM_DYLIB=OFF (defaults to OFF)"
    echo "|   - LLVM_TARGETS_TO_BUILD=\"host;X86\"  (defaults to 'all')"
    echo "|"
    echo "| See http://llvm.org/docs/CMake.html"
    echo "| See http://www.linuxfromscratch.org/blfs/view/svn/general/llvm.html"
    echo "|"
    echo "+ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~"
    echo

      time \
        cmake \
          -DCMAKE_BUILD_TYPE=Release         \
          -DCMAKE_INSTALL_PREFIX="$localdir" \
          -DLLVM_ENABLE_FFI=OFF     \
          -DBUILD_SHARED_LIBS=ON    \
          -DLLVM_TARGETS_TO_BUILD="host;X86" \
          -DLLVM_BUILD_DOCS=OFF     \
          -DLLVM_ENABLE_DOXYGEN=OFF \
          -DLLVM_ENABLE_SPHINX=OFF  \
          -G Ninja \
          ..

      retv=$?
      if [ $retv -ne 0 ]; then
        echo "| CMake failed, exit status $retv"
        exit $retv
      fi

    echo
    echo "+ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    echo "|"
    echo "| Running Ninja..."
    echo "|"
    echo "| (patience, this will take a while, like 1 hour or so)."
    echo "|"
    echo "+ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~"

    max_jobs=`how_many_cpus 2`
    max_sys_load=`max_load_level`
    keep_going=128

    echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
    echo "| FYI: Running Ninja with :"
    echo "|        -j$max_jobs      : max. simultaneous jobs,"
    echo "|    and -l$max_sys_load  : max. system load level."
    echo "|    and -k$keep_going    : max. error count (keep going as far as possible)."
    echo "+-"
    echo


      time \
        ninja             \
          -j$max_jobs     \
          -l$max_sys_load \
          -k$keep_going


      retv=$?
      if [ $retv -ne 0 ];
      then
        echo
        echo "+ ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !"
        echo "|"
        echo "| Build failed, Ninja exited with status $retv"
        echo "|"
        echo "+-- $0 ~~~> FAILED."
        echo
        exit $retv
      fi

    echo "|"
    echo "| Finished build of LLVM/Clang !  \\^^/"
    echo "|"
    echo "+-"
    echo


    echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
    echo "|"
    echo "| Running Ninja install..."
    echo "|"
    echo "| (your stuff should go under '$localdir',"
    echo "|  if everything is setup correctly here)."
    echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
    echo

      ninja install

      retv=$?
      if [ $retv -ne 0 ]; then
        echo
        echo "+ ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !"
        echo "|"
        echo "| FAIL: Ninja install failed with exit status $retv"
        echo "|"
        echo "+-"
        echo
        exit $retv
      fi

    echo
    echo "+"
    echo "|"
    echo "| Ninja install done ok ! (probably)  \\o/"
    echo "|"
    echo "+-"

  popd
fi


echo "|"
echo "+--- $0 : FINISHED, exit status: $retv"

exit $retv
# vim: et sw=2 ts=2

