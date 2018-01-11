## clong-clang-plugin / 2017-12-16 / fabic.net

message(STATUS "~~ Dude: Ich bin Clong! ( ${CMAKE_CURRENT_SOURCE_DIR} ) ~~")

# Set target name based on the current dir. name.
# (i.e. 'clong')
get_filename_component(ThisModuleName "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
set(ThisModuleName "${ThisModuleName}")
message(STATUS "Dude! I'm ${ThisModuleName}.")

# Python is needed in order to produce a PrintFunctionNames.def file
# from the PrintFunctionNames.exports file. Found this somewhere in one of Clang's
# CMake files.
# TODO: Find out about this "exports" thing wrt. RTTI & EH ??
if (false)
  set(Python_ADDITIONAL_VERSIONS 2.7)
  include(FindPythonInterp)
  if(NOT PYTHONINTERP_FOUND)
    message(WARNING
      "Unable to find Python interpreter, required for builds and testing.
      Please install Python or specify the PYTHON_EXECUTABLE CMake variable.")
  else()
    # “If we don't need RTTI or EH, there's no reason to export anything
    #  from the plugin.”
    # FIXME: This breaks the build, linker says the .def file is unrecognizable file format.
    # FIXME: We don't need to export symbols anyway (do we ?)
    if (false)
      if( NOT LLVM_REQUIRES_RTTI )
        if( NOT LLVM_REQUIRES_EH )
          set(LLVM_EXPORTED_SYMBOL_FILE
            ${CMAKE_CURRENT_SOURCE_DIR}/PrintFunctionNames.exports)
          message(STATUS "Dude: Set LLVM_EXPORTED_SYMBOL_FILE = PrintFunctionNames.exports")
        endif()
      endif()
    endif()
  endif()
endif()

# ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~

#pkg_check_modules(LIBPQXX libpqxx)

# NOTE: See top-level ../../../CMakeLists.txt for the basic LLVM package setup.
if (NOT LLVM_PACKAGE_VERSION)
  message(STATUS "Dude: It appears LLVM wasn't found / setup (?)")
  message(STATUS "      ` SKIPPING target '${ThisModuleName}' (!)")
#elseif (NOT LIBPQXX_FOUND)
#  message( WARNING "Dude: Couldn't find Postgresql's libpqxx." )
#  message( WARNING "Dude:  ` SKIPPING target '${ThisModuleName}' (!)" )
else()
  message( STATUS "Dude: Got LLVM/Clang & Postgresql's libpqxx, ok." )
  message( STATUS "Dude:  ` adding build target '${ThisModuleName}'." )

  # See `lib/cmake/llvm/AddLLVM.cmake` where this gets used.
  set(LLVM_ENABLE_EH ON)

  add_definitions( ${LLVM_DEFINITIONS} )
  include_directories( ${LLVM_INCLUDE_DIRS} )

  # Nope!
  #set(CMAKE_INCLUDE_CURRENT_DIR ON)

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
        PQXXHelper.cpp
        PPCallbacksTracker.cpp
        Repository.cpp
        TypeResolver.cpp
      $<TARGET_OBJECTS:PimplioTerminal>
      $<TARGET_OBJECTS:TypePrinter>
    PLUGIN_TOOL clang
    )

  if (false)
    target_link_libraries( ${ThisModuleName} DudeHackRoutines
                                             ${LIBPQXX_LIBRARIES}
                                             lmdb )
    target_compile_options(     ${ThisModuleName} PUBLIC ${LIBPQXX_CFLAGS} )
    target_include_directories( ${ThisModuleName} PUBLIC ${LIBPQXX_INCLUDE_DIRS} )
  else()
    target_link_libraries( ${ThisModuleName} DudeHackRoutines
                                             pqxx
                                             lmdb )
  endif()

  set_property(TARGET ${ThisModuleName}
    PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
endif()

# ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~

add_subdirectory(experiments)

message(STATUS "~~ Dude: ^ Clong (end) ( ${CMAKE_CURRENT_SOURCE_DIR} ). ~~")
