# /CMakeLists.txt (top-level LLVM/Clang/C++/playground)
cmake_minimum_required(VERSION 3.3)

project(FabicLLVMClangEtc)

set (FabicLLVMClangEtc_VERSION_MAJOR 0)
set (FabicLLVMClangEtc_VERSION_MINOR 1)

if (IS_DIRECTORY ${FABIC_LOCAL_DIR}/share/clang)
  list(APPEND CMAKE_MODULE_PATH
      "${FABIC_LOCAL_DIR}/share/clang/cmake"
      "${FABIC_LOCAL_DIR}/share/llvm/cmake"
    )
endif()

add_subdirectory(llvm-clang)
add_subdirectory(playground)


### DUMP ALL VARIABLES (DEBUG) ##############################################
##
# http://stackoverflow.com/questions/9298278/cmake-print-out-all-accessible-variables-in-a-script

  if (false)
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
     message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
  endif (false)
##
### ^ CMake defined variables debug dump. ###################################
