#
# http://llvm.org/docs/CMake.html#embedding-llvm-in-your-project
#

##
# Now performed from the top-level /CMakeLists.txt :
##
# list(APPEND CMAKE_MODULE_PATH
#     "${CMAKE_SOURCE_DIR}/../local/share/clang/cmake"
#     "${CMAKE_SOURCE_DIR}/../local/share/llvm/cmake"
#   )

find_package( LLVM  REQUIRED CONFIG )
find_package( Clang REQUIRED )

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_definitions( ${LLVM_DEFINITIONS} )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories( ${LLVM_INCLUDE_DIRS} )

add_subdirectory(clang-check)
add_subdirectory(clang-query)
