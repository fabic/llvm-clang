
cmake_minimum_required(VERSION 3.3)

# See included from ../CMakeLists.txt

#project(SymfModules)

# So called "modules", shared libraries in effect, typically
# have a .cpp along with its companion .hpp in the same directory.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(http)

# TODO ?
#add_subdirectory(asio)
#add_subdirectory(clang)

