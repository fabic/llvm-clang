
cmake_minimum_required(VERSION 3.3)

project(FabicExperiments)

# Entails -I. & -I../experiments etc... for current dir. searched first.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(X11)
add_subdirectory(misc)
add_subdirectory(clang-check)
