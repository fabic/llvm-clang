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
      -lLLVMAnalysis \
      -lLLVMAsmPrinter \
      -lLLVMBitReader \
      -lLLVMBitWriter \
      -lLLVMCodeGen \
      -lLLVMCore \
      -lLLVMInstCombine \
      -lLLVMInstrumentation \
      -lLLVMMC \
      -lLLVMMCDisassembler \
      -lLLVMMCParser \
      -lLLVMObject \
      -lLLVMOption \
      -lLLVMProfileData \
      -lLLVMScalarOpts \
      -lLLVMSelectionDAG \
      -lLLVMSupport \
      -lLLVMTarget \
      -lLLVMTransformUtils \
      -lLLVMX86AsmParser  \
      -lLLVMX86AsmPrinter  \
      -lLLVMX86CodeGen  \
      -lLLVMX86Desc  \
      -lLLVMX86Disassembler  \
      -lLLVMX86Info  \
      -lLLVMX86Utils \
      -lclangAST  \
      -lclangBasic  \
      -lclangDriver  \
      -lclangFrontend  \
      -lclangRewriteFrontend  \
      -lclangStaticAnalyzerFrontend  \
      -lclangTooling  \
      -lclangToolingCore  \
        -o clang-check  \
          ClangCheck.cpp

      # from the `ldd ...` output of the original clang-check binary :
      # -lLLVMAnalysis \
      # -lLLVMAsmPrinter \
      # -lLLVMBitReader \
      # -lLLVMBitWriter \
      # -lLLVMCodeGen \
      # -lLLVMCore \
      # -lLLVMInstCombine \
      # -lLLVMInstrumentation \
      # -lLLVMMC \
      # -lLLVMMCDisassembler \
      # -lLLVMMCParser \
      # -lLLVMObject \
      # -lLLVMOption \
      # -lLLVMProfileData \
      # -lLLVMScalarOpts \
      # -lLLVMSelectionDAG \
      # -lLLVMSupport \
      # -lLLVMTarget \
      # -lLLVMTransformUtils \
      # -lLLVMX86AsmParser  \
      # -lLLVMX86AsmPrinter  \
      # -lLLVMX86CodeGen  \
      # -lLLVMX86Desc  \
      # -lLLVMX86Disassembler  \
      # -lLLVMX86Info  \
      # -lLLVMX86Utils \
      # -lclangAST  \
      # -lclangASTMatchers \
      # -lclangAnalysis \
      # -lclangBasic  \
      # -lclangDriver  \
      # -lclangEdit \
      # -lclangFrontend  \
      # -lclangLex \
      # -lclangParse \
      # -lclangRewrite \
      # -lclangRewriteFrontend  \
      # -lclangSema \
      # -lclangSerialization \
      # -lclangStaticAnalyzerCheckers \
      # -lclangStaticAnalyzerCore \
      # -lclangStaticAnalyzerFrontend  \
      # -lclangTooling \
      # -lclangToolingCore  \


      # Tried with absolute paths :
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMAnalysis.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMAsmPrinter.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMBitReader.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMBitWriter.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMCodeGen.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMCore.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMInstCombine.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMInstrumentation.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMMC.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMMCDisassembler.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMMCParser.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMObject.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMOption.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMProfileData.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMScalarOpts.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMSelectionDAG.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMSupport.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMTarget.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMTransformUtils.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMX86AsmParser.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMX86AsmPrinter.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMX86CodeGen.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMX86Desc.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMX86Disassembler.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMX86Info.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libLLVMX86Utils.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangAST.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangASTMatchers.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangAnalysis.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangBasic.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangDriver.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangEdit.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangFrontend.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangLex.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangParse.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangRewrite.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangRewriteFrontend.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangSema.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangSerialization.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangStaticAnalyzerCheckers.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangStaticAnalyzerCore.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangStaticAnalyzerFrontend.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangTooling.so \
      # /media/g/home/fabi/dev/llvm/local/lib/libclangToolingCore.so \
