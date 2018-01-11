
# Postgresql libpqxx
find_package(PkgConfig)
pkg_check_modules(LIBPQXX libpqxx)

#dude_please_dump_cmake_variables()
#dump_cmake_variables_matching(".*PQXX.*")

# We need this exception handling, libpqxx does throw stuff.
# I'm worried though: last time I caught something was a cold  o_O`
# I was playing with C++ at the time, like 10 months ago or so.
# (I'm a bit bored today, I wonder if this project will go somewhere,
#  or will be lost in the abyss of my past, like so many others).

if (NOT LIBPQXX_FOUND)
  message( WARNING "Dude: Couldn't find Postgresql libpqxx." )
  message( WARNING "Dude:  ` skipping target 'clong-02'." )
else()
  message( STATUS "Dude: Found Postgresql libpqxx." )
  message( STATUS "Dude:  ` adding build target 'clong-02', ok." )

  set(LLVM_ENABLE_EH ON)

  add_llvm_loadable_module(
    clong-04
      clong.cpp
        PLUGIN_TOOL clang)

  target_compile_options(     clong-04 PUBLIC ${LIBPQXX_CFLAGS} )
  target_include_directories( clong-04 PUBLIC ${LIBPQXX_INCLUDE_DIRS} )
  target_link_libraries(      clong-04        ${LIBPQXX_LIBRARIES} )
endif()
