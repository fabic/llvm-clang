#!/bin/bash
#
# Output details about some of the environment variables.
#

here=$(dirname "${BASH_SOURCE[0]}")

Env=( CC CXX CFLAGS CXXFLAGS
      CPATH CPLUS_INCLUDE_PATH INCLUDE_PATH
      LD_RUN_PATH LD_LIBRARY_PATH LIBRARY_PATH
      BOOSTROOT BOOST_ROOT BOOST_INCLUDE_DIRS BOOST_LIBRARY_DIRS
      HOME PATH SSH_AUTH_SOCK
      CMAKE_MODULE_PATH PKG_CONFIG_PATH ACLOCAL ACLOCAL_PATH
    )

if true;
then
    echo
    echo "+--- ${BASH_SOURCE[0]} :"

    for e in ${Env[*]}; do
        if [ ! -z "${!e}" ]; then
            echo "$e = ${!e}"
        fi
    done \
        | while read line; do
            echo "| $line"
          done

    if false; then
        echo "|"
        echo "+--- $here/check-clang-include-paths.sh :"
        echo "|"
        $here/check-clang-include-paths.sh |& sed -e 's/^/|    /'
        echo "|"
        echo "+---"
        echo "| \$CC  = $CC   shall be $(type -p "$CC")   (found in \$PATH by \`type -p \"$CC\"\`)."
        echo "| \$CXX = $CXX shall be $(type -p "$CXX") (found in \$PATH)."
        echo "+---"
        echo
    else
        echo "+-"
        echo "| Note: there's also the \`$here/check-clang-include-paths.sh\`"
        echo "|       script which invoques Clang with ex. :"
        echo "|"
        echo "|       clang++ -Wp,-v -x c++ - -fsyntax-only < /dev/null"
        echo "|"
        echo "|       Or:  clang++ -E -x c++ - -v < /dev/null"
        echo "|"
    fi

    echo "+---"
fi

# vim: et sw=4 ts=4 ft=sh
