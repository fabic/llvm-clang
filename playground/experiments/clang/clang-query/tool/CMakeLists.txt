
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(pg-clang-query ClangQuery.cpp)

target_link_libraries(pg-clang-query
  clangAST
  clangASTMatchers
  clangBasic
  clangDynamicASTMatchers
  clangFrontend
  clangTooling

  #clangQuery
  pgClangQuery
  )

install(TARGETS pg-clang-query RUNTIME DESTINATION bin)
