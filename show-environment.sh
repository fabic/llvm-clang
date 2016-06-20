#!/bin/sh
#
# Output details about some of the environment variables.
#

here=$(dirname "${BASH_SOURCE[0]}")

Env=( CC CXX CFLAGS CXXFLAGS BOOST_ROOT BOOST_INCLUDE_DIRS BOOST_LIBRARY_DIRS CPATH CPLUS_INCLUDE_PATH HOME INCLUDE_PATH LD_RUN_PATH LD_LIBRARY_PATH LIBRARY_PATH PATH SSH_AUTH_SOCK )

if true;
then
    echo
    echo "+--- ${BASH_SOURCE[0]} :"

    for e in ${Env[*]}; do
        if [ ! -z "${!e}" ]; then
            echo "$e=${!e}"
        fi
    done |
        column -t -s= |
            while read line; do
                echo "| $line"
            done
    echo "|"
    echo "+--- $here/check-clang-include-paths.sh :"
    echo "|"
    $here/check-clang-include-paths.sh |& sed -e 's/^/|    /'
    echo "|"
    echo "+---"
    echo

fi

# vim: et sw=4 ts=4 ft=sh
