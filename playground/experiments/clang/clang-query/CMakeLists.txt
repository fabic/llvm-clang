

llvm_map_components_to_libnames(
    LLVM_CLANG_LIBS
      LineEditor
      Support
  )

add_library(
  testClangQuery
    SHARED
    Query.cpp
    QueryParser.cpp
  )

target_link_libraries(
  testClangQuery

    ${LLVM_CLANG_LIBS}

    clangAST
    clangASTMatchers
    clangBasic
    clangDynamicASTMatchers
    clangFrontend
  )

add_subdirectory(tool)
