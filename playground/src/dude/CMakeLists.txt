
#find_package(LLVM REQUIRED CONFIG)
find_package(LLVM CONFIG)
# find_package(Clang REQUIRED)

# This would typically append for ex. '/usr/lib/cmake/llvm/' to CMake search path.
# TODO: IF is directory...
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

# For `add_llvm_loadable_module()` notably.
include(AddLLVM)

message(STATUS "Dude: Found LLVM version ${LLVM_PACKAGE_VERSION}")
message(STATUS "Dude: Using LLVMConfig.cmake found at: ${LLVM_DIR}")

add_definitions( ${LLVM_DEFINITIONS} )
include_directories( ${LLVM_INCLUDE_DIRS} )

add_compile_options( -fPIC )

add_subdirectory( clang )
add_subdirectory( clong )
add_subdirectory( hacks )
add_subdirectory( util )
# add_subdirectory(surface)
add_subdirectory(xcbx)
# add_subdirectory(nstd)
add_subdirectory(dumbster)
add_subdirectory(frog)
