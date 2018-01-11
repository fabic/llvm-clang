
# This should include the local/cmake/SOCI.cmake file, which contains
# various "imported" targets, such as e.g. SOCI::soci_core.
set( SOCI_FOUND 0 )
if (false)
  include(SOCI)
  find_file( MySqlMainHeader mysql/mysql.h )
endif()

if (NOT SOCI_FOUND)
  message( STATUS "Dude: Couldn't find the SOCI database abstraction layer." )
else()
  if ( MySqlMainHeader MATCHES "-NOTFOUND$" )
    message( WARNING "Dude: Couldn't find MySQL header `mysql/mysql.h`" )
  else()
    # Would typically add /usr/include/mysql/
    get_filename_component( MySqlIncludeDir "${MySqlMainHeader}" DIRECTORY)
    include_directories( ${MySqlIncludeDir} )

    # Postgresql libpqxx
    find_package(PkgConfig)
    pkg_check_modules(LIBPQXX libpqxx)

    #dude_please_dump_cmake_variables()
    #dump_cmake_variables_matching(".*PQXX.*")

    # We need this exception handling, SOCI throws...
    # I'm worried though: last time I caught something was a cold  o_O`
    # I was playing with C++ at the time, like 10 months ago or so.
    # (I'm a bit bored today, I wonder if this project will go somewhere,
    #  or will be lost in the abyss of my past, like so many others).
    set(LLVM_ENABLE_EH ON)

    add_llvm_loadable_module(
      clong-03
        clong.cpp
          PLUGIN_TOOL clang)

    target_link_libraries( clong-03
        SOCI::soci_core
        SOCI::soci_mysql
        mysqlcppconn # FIXME: TEMP.
      )

    if (NOT LIBPQXX_FOUND)
      message( WARNING "Dude: Couldn't find Postgresql libpqxx." )
    else()
      message( STATUS "Dude: Found Postgresql libpqxx." )
      target_compile_options( clong-03 PUBLIC ${LIBPQXX_CFLAGS} )
      target_include_directories( clong-03 PUBLIC ${LIBPQXX_INCLUDE_DIRS} )
      target_link_libraries( clong-03 ${LIBPQXX_LIBRARIES} )
    endif()
  endif() # MySQL header not found.
endif() # SOCI_FOUND
