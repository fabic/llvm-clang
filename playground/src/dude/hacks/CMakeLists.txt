
# TODO: make it a static library so that we can define dependent libraries with INTERFACE...
add_library( DudeHackCxaThrowException OBJECT hack_cxa_throw_exception.cpp )

# add_library(LocalLibunwindStatic STATIC IMPORTED)
# set_target_properties(LocalLibunwindStatic PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/local/lib/libunwind-x86_64.a)
# ^ should have worked, didn't.
# ^ TODO: find out how to declare and use imported library targets.

# add_library( DudeHackRoutines OBJECT dude/hacks/routines.cpp )
add_library( DudeHackRoutines STATIC routines.cpp )
# For targets that link against our DudeHackRoutines static lib.,
# the following statement will have CMake automatically link against
# those two statics that we need from libunwind :
target_link_libraries( DudeHackRoutines INTERFACE
    ${CMAKE_SOURCE_DIR}/local/lib/libunwind-x86_64.a
    ${CMAKE_SOURCE_DIR}/local/lib/libunwind.a
  )
