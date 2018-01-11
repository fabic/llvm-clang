
set(LLVM_ENABLE_EH ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

get_filename_component(ThisModuleName "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
set(ThisModuleName "clong-${ThisModuleName}")
message(STATUS "Dude! I'm ${ThisModuleName}.")

# TODO: move these LMDB setup stuff out of here...
# TODO: and have INTERFACE... IMPORT ...
find_library(StaticLibLMDB lmdb)
find_file(LibLMDB_Header lmdb/lmdb.h)

if ( StaticLibLMDB MATCHES "-NOTFOUND$" )
  message(STATUS "Dude: Couldn't find LMDB's static liblmdb.a")
elseif( LibLMDB_Header MATCHES "-NOTFOUND$" )
  message(STATUS "Dude: Couldn't find LMDB's header file 'lmdb.h' (!)")
else()
  message(STATUS "Dude: Found LMDB static lib.: ${StaticLibLMDB}")
  message(STATUS "Dude: `-> Header is: ${LibLMDB_Header}")
  message(STATUS "Dude: `-> proceeding with target '${ThisModuleName}'")

  add_llvm_loadable_module(
    ${ThisModuleName}
      main.cpp
      ClongPluginASTAction.cpp
      Clong.cpp
        clong-translation-unit-decl.cpp
        clong-decl.cpp
        clong-namespace-decl.cpp
        clong-typedefs.cpp
        clong-lmdb.cpp
        lmdb.cpp
        PPCallbacksTracker.cpp
        Repository.cpp
        TypeResolver.cpp
      $<TARGET_OBJECTS:PimplioTerminal>
      $<TARGET_OBJECTS:TypePrinter>
    PLUGIN_TOOL clang
    )

  target_link_libraries( ${ThisModuleName}
      DudeHackRoutines
      ${StaticLibLMDB}
    )

# target_compile_options(     ${ThisModuleName} PUBLIC ${LIBxxxx_CFLAGS} )
# target_include_directories( ${ThisModuleName} PUBLIC ${LIBxxxx_INCLUDE_DIRS} )

endif()
