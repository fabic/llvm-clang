#!/bin/sh
#
# Output details about some of the environment variables.
#

Env=( CC CXX BOOST_ROOT BOOST_INCLUDE_DIRS BOOST_LIBRARY_DIRS CPATH CPLUS_INCLUDE_PATH HOME INCLUDE_PATH LD_RUN_PATH LD_LIBRARY_PATH LIBRARY_PATH PATH SSH_AUTH_SOCK )

if true;
then
    echo
    echo "+---"

    for e in ${Env[*]}; do
        if [ ! -z "${!e}" ]; then
            echo "$e=${!e}"
        fi
    done |
        column -t -s= |
            while read line; do
                echo "| $line"
            done
    echo "+---"
    echo
fi

# vim: et sw=4 ts=4 ft=sh
