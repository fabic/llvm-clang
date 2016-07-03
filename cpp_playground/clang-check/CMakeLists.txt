
cmake_minimum_required(VERSION 3.3)

project(FabiCClangHacking)

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../local/share/clang/cmake")


/usr/bin/clang++
  -DCLANG_ENABLE_ARCMT
  -DCLANG_ENABLE_OBJC_REWRITER
  -DCLANG_ENABLE_STATIC_ANALYZER
  -DGTEST_HAS_RTTI=0
  -D_GNU_SOURCE
  -D__STDC_CONSTANT_MACROS
  -D__STDC_FORMAT_MACROS
  -D__STDC_LIMIT_MACROS
    -fPIC
    -fvisibility-inlines-hidden
    -Wall -W
    -Wno-unused-parameter -Wwrite-strings -Wcast-qual
    -Wmissing-field-initializers -pedantic -Wno-long-long
    -Wcovered-switch-default -Wnon-virtual-dtor
    -Wdelete-non-virtual-dtor -std=c++11 -fcolor-diagnostics
    -ffunction-sections -fdata-sections -fno-common
    -Woverloaded-virtual -Wno-nested-anon-types
    -O3 -DNDEBUG -fno-exceptions -fno-rtti
    -Wl,-allow-shlib-undefined -Wl,-O3 -Wl,--gc-sections
    -Wl,-rpath,"\$ORIGIN/../lib"
      -o clang-check
      -c clang-check/ClangCheck.cpp
         -lLLVMX86CodeGen.so
         -lLLVMX86AsmPrinter.so
         -lLLVMX86AsmParser.so
         -lLLVMX86Desc.so
         -lLLVMX86Info.so
         -lLLVMX86Disassembler.so
         -lLLVMOption.so
         -lLLVMSupport.so
         -lclangAST.so
         -lclangBasic.so
         -lclangDriver.so
         -lclangFrontend.so
         -lclangRewriteFrontend.so
         -lclangStaticAnalyzerFrontend.so
         -lclangTooling.so

find_package(Clang REQUIRED )

set( LLVM_LINK_COMPONENTS
  ${LLVM_TARGETS_TO_BUILD}
  Option
  Support
  )

add_executable(
  clang-check
  ClangCheck.cpp
  )

target_compile_definitions(
  clang-check
  PUBLIC
    # We need to emit 2 #define-s to get away with 2 errors raised
    # from Clang header `Support/DataTypes.h` complaining :
    #
    # “ error: Must #define __STDC_LIMIT_MACROS before #including Support/DataTypes.h
    #   error: Must #define __STDC_CONSTANT_MACROS before #including Support/DataTypes.h ”
    #
    # See also Clang's `local/lib/clang/3.8.1/include/stdint.h`.
    -D__STDC_CONSTANT_MACROS
    -D__STDC_LIMIT_MACROS
  )

target_link_libraries(
  clang-check
  clangAST
  clangBasic
  clangDriver
  clangFrontend
  clangRewriteFrontend
  clangStaticAnalyzerFrontend
  clangTooling
  LLVMOption
  LLVMSupport
  LLVMX86CodeGen.so
  LLVMX86AsmPrinter.so
  LLVMX86AsmParser.so
  LLVMX86Desc.so
  LLVMX86Info.so
  LLVMX86Disassembler.so
  )


install(TARGETS
  clang-check
  RUNTIME DESTINATION bin)
