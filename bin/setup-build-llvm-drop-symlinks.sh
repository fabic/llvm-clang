#!/bin/sh

echo "Symlinking LLVM stuff :"

ln -sfnv ../../clang llvm/tools/                    &&
ln -sfnv ../../clang-tools-extra/ clang/tools/extra &&
ln -sfnv ../../compiler-rt   llvm/projects/         &&
ln -sfnv ../../libcxx{,abi}  llvm/projects/         &&
ln -sfnv ../../lldb llvm/tools/                     &&
ln -sfnv ../../lld  llvm/tools/

retv=$?

ls -lt --color=auto clang/tools/extra llvm/tools/{clang,lldb,lld} llvm/projects/{compiler-rt,libcxx{,abi}}

if [ -d misc/linux-headers/include/ ]; then
    echo
    echo "Symlinking Linux headers from misc/linux-headers/include/ under local/include/"
    ( mkdir -pv local/include/ &&
      cd local/include/ &&
      ln -sfnv ../../misc/linux-headers/include/* . )
fi

exit $retv
