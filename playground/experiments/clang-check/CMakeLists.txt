
cmake_minimum_required(VERSION 3.3)

list(APPEND CMAKE_MODULE_PATH
  # "${CMAKE_CURRENT_SOURCE_DIR}/../local/share/clang/cmake"
  "${CMAKE_SOURCE_DIR}/../local/share/clang/cmake"
  "${CMAKE_SOURCE_DIR}/../local/share/llvm/cmake"
  )

if (true)
	#set(LLVM_ENABLE_RTTI ON)

  find_package(LLVM  REQUIRED 3.8.1 )
  find_package(Clang REQUIRED 3.8.1 )

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

  #target_compile_options(clang-check PUBLIC -flto -v)

  target_link_libraries(
    clang-check
    clangAST
    clangBasic
    clangDriver
    clangFrontend
    clangRewriteFrontend
    clangStaticAnalyzerFrontend
    clangTooling
    #clangToolingCore
    LLVMOption
    LLVMSupport
    LLVMX86CodeGen
    LLVMX86AsmPrinter
    LLVMX86AsmParser
    LLVMX86Desc
    LLVMX86Info
    LLVMX86Disassembler
    )


  install(TARGETS
    clang-check
    RUNTIME DESTINATION bin)
endif (true)
