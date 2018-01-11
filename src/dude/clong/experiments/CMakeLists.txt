# clong-clang-plugin/experiments  //  fabic.net
#
# Experiments for /me to discover how to tinkle with the Clang plugin system.

# TODO: Cleanup & fix those older experiments that no longer compile.
set(EXCLUDE_FROM_ALL ON)

##
## Automatically add sub-directories to CMake project.
##
file(GLOB _files_list
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
list(SORT _files_list)

set(_subdirs_list "")

foreach(subdir ${_files_list})
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt)
    add_subdirectory( ${subdir} )
    message(STATUS "Dude: Added experiment ${subdir}")
    list(APPEND _subdirs_list "${subdir}")
  elseif(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
    message(STATUS "Dude: Skipping ${subdir}/ as it contains no CMakeLists.txt")
  endif()
endforeach()

##
## Fetch all targets and set the output dir. to :
##   - 'lib/' for libraries ;
##   - 'bin/' for executables.
##
if (true)
  # This is more concise than the above, but returns absolute paths :
  # get_property(_subdirs_list
  #   DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  #   PROPERTY SUBDIRECTORIES)

  # We'll be collecting targets.
  set(_all_library_targets "")

  foreach(subdir ${_subdirs_list})
    get_property(_targets_list DIRECTORY "${subdir}" PROPERTY BUILDSYSTEM_TARGETS)

    if (_targets_list)
      list(SORT _targets_list)
      foreach(_target ${_targets_list})
        get_property(_target_type TARGET ${_target} PROPERTY TYPE)

        if (${_target_type} MATCHES "_LIBRARY$")
          set_property(TARGET ${_target}
            PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

          # ^ setting this property _also_ have the target installed at the
          #   appropriate location (upon `make install`).
          #   Btw. CMake won't accept the following :
          # install(TARGETS ${_target} DESTINATION lib)

          list(APPEND _all_library_targets ${_target})

          message(STATUS "FYI: ${subdir}: Changed output dir. of target '${_target}' ('${_target_type}').")

        elseif (${_target_type} STREQUAL "EXECUTABLE")
          set_property(TARGET ${_target}
            PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
          message(STATUS "FYI: ${subdir}: Changed output dir. of target '${_target}' ('${_target_type}').")

        else()
          message(STATUS "FYI: ${subdir}: Target '${_target}' is-a '${_target_type}'.")

        endif()
      endforeach()

    else()
      message(STATUS "FYI: ${subdir}: Has no targets.")
    endif()
  endforeach()
endif()

#
## Add a custom target for running the last of our Clang/plugin experiment
## against one of the tutu/test-XXX.cpp sample files.
#
if (true)
  # Find out the most recent `clong-XX` experiment.
  list(GET _all_library_targets -1 _last_clang_plugin)

  if (NOT _last_clang_plugin)
    message(WARNING "Dude: Couldn't find out which of your Clang plugin experiments is the most recent one." )
  else()
    get_property(_outdir  TARGET ${_last_clang_plugin} PROPERTY LIBRARY_OUTPUT_DIRECTORY)
    file(RELATIVE_PATH _outdir ${CMAKE_SOURCE_DIR} ${_outdir})

    # get_property(_libname TARGET ${_last_clang_plugin} PROPERTY LIBRARY_OUTPUT_NAME)
    # ^ FIXME: Returns "" empty string, expecting "clong-05.so".
    #   FIXME: Hard-coding '.so' extension.
    set(_libname "${_last_clang_plugin}.so")

    ## Search for the "most recent" tutu/test-XXX.cpp test file.
    set(_test_files "")
    set(_last_test_file "")

    file(GLOB _test_files
      RELATIVE ${CMAKE_SOURCE_DIR}
      ${CMAKE_SOURCE_DIR}/tutu/test-*.cpp)

    if (NOT _test_files)
      message(WARNING "Dude: Found _no_ test files under tutu/")
      message(WARNING "      ` NOT declaring custom command target.")
    else()
      list(SORT _test_files)
      list(GET _test_files -1 _last_test_file)

      # Clang compiler invocation :
      #  ` Execution command for add_custom_target() below
      #   (note that this must be a list ).
      set(_run_clang_with_plugin_command
        clang++ -fplugin=${_outdir}/${_libname}
          -Xclang -plugin-arg-clong -Xclang Hola
          -std=c++1z -fsyntax-only -O0 -c "${_last_test_file}")

      set(_run_last_plugin_target_name "run-${_last_clang_plugin}")

      add_custom_target( ${_run_last_plugin_target_name}
        #ALL
        DEPENDS ${_last_clang_plugin}
          COMMAND ${_run_clang_with_plugin_command}
          COMMAND echo -en "\\n\\e[94;7;1m Command was: \\e[0m\\e[37m" ${_run_clang_with_plugin_command} "\\e[0m\\n"
          COMMAND echo -en "\\n\\e[32;1;7m  HEY!  \\e[0m\\n"
          COMMAND echo -en "\\n\\e[2m  Relax, breathe, enjoy your stay here upon this earth...  \\e[0m\\n\\n"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "-- Running Clang with the most recent plugin experiment '${_libname}' : --"
        VERBATIM USES_TERMINAL
        )

      message(STATUS "~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~")
      message(STATUS "~~")
      message(STATUS "~~ Dude: Got target '${_last_clang_plugin}' as your most recent Clang plugin.")
      message(STATUS "~~  ` Added custom build target '${_run_last_plugin_target_name}' that runs :")
      message(STATUS "~~   `  ${_run_clang_with_plugin_command}")
      message(STATUS "~~    `")
      message(STATUS "~~     ` Ex.: \$ cmake --build build/ --target ${_run_last_plugin_target_name}")
      message(STATUS "~~")
      message(STATUS "~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~")
    endif()
  endif()
endif()

unset(_files_list)
unset(_subdirs_list)
