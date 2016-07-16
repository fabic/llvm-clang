
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(clang-query ClangQuery.cpp)

target_link_libraries(clang-query
  clangAST
  clangASTMatchers
  clangBasic
  clangDynamicASTMatchers
  clangFrontend
  clangTooling

  #clangQuery
  testClangQuery
  )

install(TARGETS clang-query RUNTIME DESTINATION bin)
