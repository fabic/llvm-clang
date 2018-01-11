
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
  message( WARNING "Dude:  ` skipping target 'clong-07'." )
else()

  message( STATUS "Dude: Found Postgresql libpqxx." )
  message( STATUS "Dude:  ` adding build target 'clong-07', ok." )

  set(LLVM_ENABLE_EH ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  add_llvm_loadable_module(
    clong-08
      clong.cpp
      Dummy.cpp
      TraverseTUnitAction.cpp
      TraverseTUnitConsumer.cpp
      PimplIOVisitor.cpp
      TypeResolver.cpp
      $<TARGET_OBJECTS:PimplioTerminal>
      $<TARGET_OBJECTS:TypePrinter>
      # "Visitors" :
        visitors/XBase.cpp
        visitors/XDecl.cpp
        visitors/XTranslationUnitDecl.cpp
        visitors/XNamedDecl.cpp
        visitors/XNamespaceDecl.cpp
        visitors/XTypeDecl.cpp
        visitors/XValueDecl.cpp
        visitors/XDeclaratorDecl.cpp
        visitors/XFieldDecl.cpp
        visitors/XFunctionDecl.cpp
        visitors/XVarDecl.cpp
    PLUGIN_TOOL clang
    )

  target_compile_options(     clong-08 PUBLIC ${LIBPQXX_CFLAGS} )
  target_include_directories( clong-08 PUBLIC ${LIBPQXX_INCLUDE_DIRS} )
  target_link_libraries(      clong-08        ${LIBPQXX_LIBRARIES} )

  target_link_libraries( clong-08
    -Wl,-init,_clong_so_init
    -Wl,-fini,_clong_so_fini
  )

endif()
