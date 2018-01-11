### DUMP ALL VARIABLES (DEBUG) ##############################################
##
# http://stackoverflow.com/questions/9298278/cmake-print-out-all-accessible-variables-in-a-script

function(dude_please_dump_cmake_variables)
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName} = ${${_variableName}}")
  endforeach()
endfunction()

function(dude_dump_cmake_variables_matching regex)
  get_cmake_property(varnames VARIABLES)
  foreach (vname ${varnames})
    if ("${vname}" MATCHES "${regex}")
      message(STATUS "${vname} = ${${vname}}")
    endif()
  endforeach()
endfunction()

##
### ^ CMake defined variables debug dump. ###################################
