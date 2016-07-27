
cmake_minimum_required(VERSION 3.3)

project(FabicExperiments)

# Entails -I. & -I../experiments etc... for current dir. searched first.
# EDIT/2016-07-27: done from the top-level CMakeLists.txt, project-wide.
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

# TODO: add ../src/ as include directory.

add_subdirectory(X11)
add_subdirectory(misc)
add_subdirectory(clang)
