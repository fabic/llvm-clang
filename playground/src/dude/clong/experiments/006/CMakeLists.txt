
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
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  add_llvm_loadable_module(
    clong-06
      clong.cpp
      Dummy.cpp
      TraverseTUnitAction.cpp
      TraverseTUnitConsumer.cpp
      PimplIOVisitor.cpp
      $<TARGET_OBJECTS:PimplioTerminal>
      $<TARGET_OBJECTS:TypePrinter>
        PLUGIN_TOOL clang)

  target_compile_options(     clong-06 PUBLIC ${LIBPQXX_CFLAGS} )
  target_include_directories( clong-06 PUBLIC ${LIBPQXX_INCLUDE_DIRS} )
  target_link_libraries(      clong-06        ${LIBPQXX_LIBRARIES} )

  target_link_libraries( clong-06
    -Wl,-init,_clong_so_init
    -Wl,-fini,_clong_so_fini
  )

endif()
