
# Entails -I. & -I../experiments etc... for current dir. searched first.
# EDIT/2016-07-27: done from the top-level CMakeLists.txt, project-wide.
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(EXCLUDE_FROM_ALL ON)

add_subdirectory(X11)
add_subdirectory(misc)

#add_subdirectory(clang)
# ^ broken 2017-02-21 w/ Clang 4.0.0
