
cmake_minimum_required(VERSION 3.3)

add_executable(basic-01 basic-01.cpp)

llvm_map_components_to_libnames(
    LLVM_CLANG_LIBS
      Support Option
      X86CodeGen
      X86AsmParser
      X86Desc
      X86Info
  )

target_link_libraries(
    basic-01
      clong
        clangAST
        clangBasic
        clangDriver
        clangFrontend
        clangRewriteFrontend
        clangStaticAnalyzerFrontend
        clangTooling
          ${LLVM_CLANG_LIBS}
  )
