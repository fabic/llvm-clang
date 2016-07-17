# /CMakeLists.txt (top-level LLVM/Clang/C++/playground)
cmake_minimum_required(VERSION 3.3)

project(FabicLLVMClang)

# For libcxx :
set(LIBCXX_CXX_ABI libcxxabi)
set(LIBCXX_CXX_ABI_INCLUDE_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/libcxxabi/include)

# For libcxxabi :
set(LIBCXXABI_LIBCXX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libcxx)
set(LIBCXXABI_USE_LLVM_UNWINDER ON)

if (false)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

set(BUILD_SHARED_LIBS ON)

set( LLVM_TARGETS_TO_BUILD "host;X86" CACHE STRING "Huh! FabiC override.")

set( LLVM_BUILD_DOCS       OFF )
set( LLVM_ENABLE_DOXYGEN   OFF )
set( LLVM_ENABLE_SPHINX    OFF )

set( LLVM_ENABLE_FFI       ON )
set( LLVM_ENABLE_EH        ON )
set( LLVM_ENABLE_RTTI      ON )
set( LLVM_ENABLE_CXX1Y     ON )

set( LLVM_ENABLE_LTO       ON )

# Optional custom built Binutils compiled with the Gold thing
# for LLVM LTO support.
if (IS_DIRECTORY ${FABIC_LOCAL_DIR}/local/include)
  set( LLVM_BINUTILS_INCDIR ${FABIC_LOCAL_DIR}/local/include )
endif()

add_subdirectory(llvm)

