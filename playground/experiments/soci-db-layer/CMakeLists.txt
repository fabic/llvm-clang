
#
## Note that SOCI now provides an experimental SOCI.cmake module.
## (that I couldn't get to do its job -_-)
#

set( SOCI_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/soci )

# if (NOT IS_DIRECTORY ${SOCI_SOURCES_DIR}/include/soci/soci.h)
#   message( STATUS "Dude: Could not find SOCI sources at ${SOCI_SOURCES_DIR} (maybe run `git submodule update --init` ?)" )
# endif()

## Grrr... there's no pkg-config for MySQL.
# find_package(PkgConfig)
# pkg_check_modules(MySQL mysql)
# if (NOT MySQL_FOUND)
#   message (STATUS "Dude: Could not find package MySQL.")
# endif()
## Fallback to manual setup, hopefully CMake will pick it up from /usr.
find_file( MySqlMainHeader mysql/mysql.h )
find_library( LibMysqlClient libmysqlclient.so )

# Note: we provide HINTS for the case
# where we did not run make/ninja install.
find_file( LibSociMainHeaderFile soci/soci.h
  HINTS ${SOCI_SOURCES_DIR}/include ${SOCI_SOURCES_DIR}/build/include )

find_file( LibSociConfigHeader soci/soci-config.h
  HINTS ${SOCI_SOURCES_DIR}/include ${SOCI_SOURCES_DIR}/build/include)

find_library( LibSociCore  libsoci_core.so   HINTS ${SOCI_SOURCES_DIR}/build/lib)
find_library( LibSociMysql libsoci_mysql.so  HINTS ${SOCI_SOURCES_DIR}/build/lib)

if ( LibSociCore MATCHES "-NOTFOUND$" )
  message( STATUS "Dude: Could not find SOCI's shared library `libsoci_core.so`")
elseif ( LibSociMysql MATCHES "-NOTFOUND$")
  message( STATUS "Dude: Could not find SOCI's shared library `libsoci_mysql.so` (MySQL backend impl.)")
elseif ( MySqlMainHeader MATCHES "-NOTFOUND$")
  message (STATUS "Dude: Could not header file mysql.h")
else()
  message( STATUS "Dude: Found `libmysqlclient.so` at ${LibMysqlClient}")
  message( STATUS "Dude: Found `libsoci_core.so` at ${LibSociCore}")
  message( STATUS "Dude: Found `libsoci_mysql.so` at ${LibSociMysql}")
  message( STATUS "Dude: soci/soci.h would be ${LibSociMainHeaderFile}")
  message( STATUS "Dude: soci/soci-config.h would be ${LibSociConfigHeader}")
  message( STATUS "Dude: mysql/mysql.h would be ${MySqlMainHeader}")

  # Actual include/soci/ headers dir.
  get_filename_component( LibSociInclude "${LibSociMainHeaderFile}" DIRECTORY)
  get_filename_component( LibSociInclude "${LibSociInclude}" DIRECTORY)
  include_directories( ${LibSociInclude} )

  # For `soci/soci-config.h` it may happen that it resides in the build/ sub-dir.
  # if we didn't run `make/ninja install`.
  get_filename_component( LibSociInclude "${LibSociConfigHeader}" DIRECTORY)
  get_filename_component( LibSociInclude "${LibSociInclude}" DIRECTORY)
  include_directories( ${LibSociInclude} )

  # Typically this yields /usr/include/mysql
  get_filename_component( MySqlIncludeDir "${MySqlMainHeader}" DIRECTORY)
  include_directories( ${MySqlIncludeDir} )

  # Maybe for ex. local/lib64, or vendor/soci/build/lib
  get_filename_component( SociLibraryDir "${LibSociCore}" DIRECTORY)
  link_libraries(
    -L${SociLibraryDir}
    -Wl,-rpath=${SociLibraryDir}  # FIXME ?
    -lsoci_core -lsoci_mysql
    -ldl -lmysqlclient
    )

  add_executable( soci-mysql-example-01 soci-mysql-example-01.cpp )
  target_link_libraries( soci-mysql-example-01 )
endif()

