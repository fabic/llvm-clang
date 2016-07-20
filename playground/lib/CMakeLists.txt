# playground/lib/CMakeLists.txt

##
## See lib/fltk/CMake/options.cmake
##
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/fltk/CMakeLists.txt)
  message(STATUS "FLKT: Found, we'll build it along...")
#  option(OPTION_USE_POLL     "use poll if available" ON)
  option(OPTION_BUILD_SHARED_LIBS "Build shared libraries (in addition to static libraries)" ON)

  option(OPTION_BUILD_EXAMPLES "build example programs" OFF)
#  option(OPTION_BUILD_HTML_DOCUMENTATION "build html docs" ON)

#  option(OPTION_CAIRO "use lib Cairo" ON)
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
  message(STATUS "FTLK: not found.")
endif ()
