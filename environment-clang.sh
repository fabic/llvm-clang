# clang-environment.sh

here=$( cd `dirname "${BASH_SOURCE[0]}"` && pwd )

. $here/functions.sh

# Check/output Clang version
echo "Checking for Clang..."
if ! ( clang --version | sed -e 's/^/    &/' ); then
    retv=$?
    echo "Oups! couldn't execute Clang, exiting (retv=$retv)"
    exit $retv
fi

# Clang LLVM ;-
CC=clang
CXX=clang++
export CC CXX

if [ -d "$here/local/include" ]; then
    pathprepend "$here/local/include" CPATH
fi

if [ -d "$here/local/lib" ]; then
    pathprepend "$here/local/lib" LD_RUN_PATH
fi

Env=( CC CXX CPATH CPLUS_INCLUDE_PATH LD_LIBRARY_PATH LD_RUN_PATH LIBRARY_PATH PATH )

#
# Output details about some of the environment variables.
#
if true;
then
    echo
    echo "+---"

    for e in ${Env[*]}; do
        echo "$e=${!e}"
    done |
        column -t -s= |
            while read line; do
                echo "| $line"
            done
    echo "+---"
fi

# vim: et sw=4 ts=4 ft=sh
