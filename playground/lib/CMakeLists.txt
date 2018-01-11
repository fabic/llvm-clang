# cxx-playground/lib/CMakeLists.txt -- fabic.net

##
## See lib/fltk/CMake/options.cmake
##
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/fltk/CMakeLists.txt)
  message(STATUS "FLKT: Found, we'll build it along...")
#  option(OPTION_USE_POLL     "use poll if available" ON)
  option(OPTION_BUILD_SHARED_LIBS "Build shared libraries (in addition to static libraries)" ON)

  option(OPTION_BUILD_EXAMPLES "build example programs" OFF)
#  option(OPTION_BUILD_HTML_DOCUMENTATION "build html docs" ON)

  option(OPTION_CAIRO    "use lib Cairo" ON)
#  option(OPTION_CAIROEXT "use FLTK code instrumentation for cairo extended use" ON)

  option(OPTION_USE_GL      "use OpenGL"                OFF)
  option(OPTION_USE_THREADS "use multi-threading"       OFF)
  option(OPTION_LARGE_FILE  "enable large file support" OFF)

  ## option(OPTION_USE_SYSTEM_ZLIB    "use system zlib"    ON)
  ## option(OPTION_USE_SYSTEM_LIBJPEG "use system libjpeg" ON)
  ## option(OPTION_USE_SYSTEM_LIBPNG  "use system libpng"  ON)

#  option(OPTION_USE_XINERAMA "use lib Xinerama" OFF)
#  option(OPTION_USE_XFIXES   "use lib Xfixes"   OFF)
#  option(OPTION_USE_XCURSOR  "use lib Xcursor"  OFF)
#  option(OPTION_USE_XFT      "use lib Xft"      OFF)
#  option(OPTION_USE_XRENDER  "use lib Xrender"  OFF)
#  option(OPTION_USE_XDBE     "use lib Xdbe"     OFF)

  add_subdirectory(fltk)
else()
  message(STATUS "Dude: FTLK not found, ok.")
endif ()

##
## LMDB key-value store database from the OpenLDAP project.
##
## Note: it happens by pure coincidence, that the output dir.
##       will be build/lib/  (because _this_ dir. is 'lib/').
##       -> so we're not overriding this target's output dir.
##
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lmdb/libraries/liblmdb/mdb.c)
  message(STATUS "Dude! Found LMDB sources, adding custom library target for building it, ok.")
  add_library(lmdb STATIC lmdb/libraries/liblmdb/mdb.c
                          lmdb/libraries/liblmdb/midl.c )
  target_compile_options(lmdb PRIVATE -fPIC)
  target_include_directories(lmdb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/lmdb/libraries/liblmdb> )
  # ^ this will have dependent targets automagically have
  #   the correct -I include path be set for them.
else()
  message(STATUS "Dude, didn't find LMDB sources, skipping.")
  message(STATUS "      ` see ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt about this.")
endif()

##
## PostgreSQL libpqxx
##
## Custom build target for some obscur reasons.
##
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libpqxx/src/connection.cxx)
  message(STATUS "Dude! Found libpqxx sources, adding custom library target for building it, ok.")
  message(STATUS "      (I know this is inconventional)")

  pkg_check_modules(LIBPQ libpq)
  # dude_dump_cmake_variables_matching("PQ.*")

  if (NOT LIBPQ_FOUND)
    message(STATUS "Dude: (!!) Didn't find PostgreSQL's libpq   (!!)")
    message(STATUS "Dude: (!!) Skipping custom `libpqxx` target (!!)")
  else()
    add_library(pqxx STATIC
        libpqxx/src/binarystring.cxx
        libpqxx/src/connection_base.cxx
        libpqxx/src/connection.cxx
        libpqxx/src/cursor.cxx
        libpqxx/src/dbtransaction.cxx
        libpqxx/src/errorhandler.cxx
        libpqxx/src/except.cxx
        libpqxx/src/field.cxx
        libpqxx/src/largeobject.cxx
        libpqxx/src/nontransaction.cxx
        libpqxx/src/notification.cxx
        libpqxx/src/pipeline.cxx
        libpqxx/src/prepared_statement.cxx
        libpqxx/src/result.cxx
        libpqxx/src/robusttransaction.cxx
        libpqxx/src/row.cxx
        libpqxx/src/statement_parameters.cxx
        libpqxx/src/strconv.cxx
        libpqxx/src/subtransaction.cxx
        libpqxx/src/tablereader.cxx
        libpqxx/src/tablestream.cxx
        libpqxx/src/tablewriter.cxx
        libpqxx/src/transaction_base.cxx
        libpqxx/src/transaction.cxx
        libpqxx/src/util.cxx
      )

    target_compile_options(pqxx PRIVATE -fPIC
                                        -fvisibility=hidden
                                        -fvisibility-inlines-hidden
                                        ${LIBPQ_CFLAGS} )

    target_include_directories(pqxx PUBLIC
      ${LIBPQ_INCLUDE_DIRS}
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/libpqxx/include> )

    target_link_libraries(pqxx ${LIBPQ_LIBRARIES} )
  endif()
endif()
