#!/bin/sh
#
# fabic/2016-06-30

here=$(cd `dirname "$0"` && pwd)

echo "+--- $0"
echo "|"
echo "| This script builds the whole LLVM Clang compiler"
echo "|"

cd "$here/llvm-clang" || exit 1

echo "| Entered `pwd`/"

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
else
  echo "| Done with Git submodule checkout, ok."
  echo "+-"
fi




472276  2016-06-29 20:05:36  . environment-clang.sh


472399  2016-06-30 09:05:48  mkdir -pv build2/ && cd build2/
472400  2016-06-30 09:06:29  time   cmake     -DCMAKE_BUILD_TYPE=Release     -DCMAKE_INSTALL_PREFIX=$(cd ../../local/ && pwd) ../llvm/ -G Ninja

472417  2016-06-30 09:08:43  time   ninja -l3.9 -k128 ; echo RETVAL=$?

472419  2016-06-30 12:28:37  make install
472281  2016-06-29 20:06:35  ln -siv ../../clang llvm/tools/
472282  2016-06-29 20:06:40  ln -siv ../../clang-tools-extra/ clang/tools/extra
472283  2016-06-29 20:06:44  ln -siv ../../compiler-rt   llvm/projects/
472284  2016-06-29 20:06:53  ln -siv ../../libcxx{,abi}  llvm/projects/
