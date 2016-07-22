#!/bin/sh
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
    done |
            while read line; do
                echo "| $line"
            done
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

fi

# vim: et sw=4 ts=4 ft=sh
