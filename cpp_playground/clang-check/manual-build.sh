#!/bin/sh
#
# Got this from the actual compilation of LLVM/Clang tree

here=$( cd `dirname "$0"` && pwd )

cd "$here" || exit 127

echo "# We're in `pwd`"

    # -Wl,-rpath,"\$ORIGIN/../lib"  \

clang++  \
  -v \
  -DCLANG_ENABLE_ARCMT  \
  -DCLANG_ENABLE_OBJC_REWRITER  \
  -DCLANG_ENABLE_STATIC_ANALYZER  \
  -DGTEST_HAS_RTTI=0  \
  -D_GNU_SOURCE  \
  -D__STDC_CONSTANT_MACROS  \
  -D__STDC_FORMAT_MACROS  \
  -D__STDC_LIMIT_MACROS  \
    -fPIC  \
    -fvisibility-inlines-hidden  \
    -Wall -W  \
    -Wno-unused-parameter -Wwrite-strings -Wcast-qual  \
    -Wmissing-field-initializers -pedantic -Wno-long-long  \
    -Wcovered-switch-default -Wnon-virtual-dtor  \
    -Wdelete-non-virtual-dtor -std=c++11 -fcolor-diagnostics  \
    -ffunction-sections -fdata-sections -fno-common  \
    -Woverloaded-virtual -Wno-nested-anon-types  \
    -O3 -DNDEBUG -fno-exceptions -fno-rtti  \
    -Wl,-allow-shlib-undefined -Wl,-O3 -Wl,--gc-sections  \
    -L"/media/g/home/fabi/dev/llvm/local/lib" \
    -Wl,-rpath,"/media/g/home/fabi/dev/llvm/local/lib"  \
      -lLLVMX86CodeGen  \
      -lLLVMX86AsmPrinter  \
      -lLLVMX86AsmParser  \
      -lLLVMX86Desc  \
      -lLLVMX86Info  \
      -lLLVMX86Disassembler  \
      -lLLVMOption  \
      -lLLVMSupport  \
      -lclangAST  \
      -lclangBasic  \
      -lclangDriver  \
      -lclangFrontend  \
      -lclangRewriteFrontend  \
      -lclangStaticAnalyzerFrontend  \
      -lclangTooling  \
        -o clang-check  \
          ClangCheck.cpp
