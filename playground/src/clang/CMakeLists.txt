
find_package( LLVM  REQUIRED CONFIG )
find_package( Clang REQUIRED )
#find_package (Boost COMPONENTS REQUIRED log )

message(STATUS "Clong: Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Clong: Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_definitions( ${LLVM_DEFINITIONS} )

include_directories( ${LLVM_INCLUDE_DIRS} )

add_library(
  clong
  SHARED
  ClangAstConsumer.cpp
  )

#llvm_map_components_to_libnames(
#    LLVM_CLANG_LIBS
#    Support Option
#    X86CodeGen
#    X86AsmParser
#    X86Desc
#    X86Info
#)

target_link_libraries(
    clong
    ${Boost_LIBRARIES}
    clangAST
#    clangBasic
#    clangDriver
#    clangFrontend
#    clangRewriteFrontend
#    clangStaticAnalyzerFrontend
#    clangTooling
#    ${LLVM_CLANG_LIBS}
)

add_subdirectory(PrintFunctionNames)
add_subdirectory(find-all-symbols)
