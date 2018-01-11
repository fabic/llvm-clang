
cmake_minimum_required(VERSION 3.3)

add_executable(
    pg-clang-check
    ClangCheck.cpp
  )

#target_include_directories(pg-clang-check PUBLIC ${LLVM_INCLUDE_DIRS})

# NOTE: Performed by parent CMakeLists.txt
#target_compile_definitions(
#  pg-clang-check
#  PUBLIC
#    # We need to emit 2 #define-s to get away with 2 errors raised
#    # from Clang header `Support/DataTypes.h` complaining :
#    #
#    # “ error: Must #define __STDC_LIMIT_MACROS before #including Support/DataTypes.h
#    #   error: Must #define __STDC_CONSTANT_MACROS before #including Support/DataTypes.h ”
#    #
#    # See also Clang's `local/lib/clang/3.8.1/include/stdint.h`.
#    #-D__STDC_CONSTANT_MACROS
#    #-D__STDC_LIMIT_MACROS
#    #
#    # EDIT: turns out this is achieved by :
#    ${LLVM_DEFINITIONS}
#  )

# LLVM/Clang must have been built w/ LLVM_ENABLE_RTTI=ON,
# else you may get around unresolved "typeinfo for ..." link-time
# problems with -fno-rtti :
#target_compile_options(pg-clang-check PUBLIC -fno-rtti)

# “ Find the libraries that correspond to the LLVM components
#   that we wish to use ” :
llvm_map_components_to_libnames(
    LLVM_CLANG_LIBS
      Support Option
      X86CodeGen
      X86AsmParser
      X86Desc
      X86Info
  )

# target_compile_options(pg-clang-check PUBLIC -flto)

target_link_libraries(
  pg-clang-check
    # "-Wl,-v,-plugin=LLVMgold.so"
    clangAST
    clangBasic
    clangDriver
    clangFrontend
    clangRewriteFrontend
    clangStaticAnalyzerFrontend
    clangTooling
    ${LLVM_CLANG_LIBS}
  )

install(TARGETS
  pg-clang-check
  RUNTIME DESTINATION bin)

