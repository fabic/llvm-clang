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

#
# Boost C++ library
#
boost_modular_dir="$here/misc/boost"

if [ -d "$boost_modular_dir" ];
then
    # Note 'misc/boost/.git' may be a file instead of a directory in case it
    # is a Git submodule clone (hence the `test -e .git`).
    boost_cpp_version="$(cd "$boost_modular_dir" && test -e .git && git describe --abbrev=0)"

    if [ -z "$boost_cpp_version" ]; then
        echo "Couldn't find Boost C++ library at '$boost_modular_dir', skipping"
    else
        echo "Found Boost modular repository checked-out at '$boost_modular_dir'."

        # Once built we want it installed at target dir. :
        BOOST_ROOT="$HOME/${boost_cpp_version}-${CC}" 

        if [ -d "$BOOST_ROOT/include" ] && [ -d "$BOOST_ROOT/lib" ];
        then
            echo "FYI: Found Boost C++ installed @ '$BOOST_ROOT', ok."

            export BOOST_ROOT

            # This appears to be needed by CMake for it to find Boost,
            # BOOST_ROOT ain't enough it appears.
            BOOST_INCLUDE_DIRS="${BOOST_ROOT}/include"
            BOOST_LIBRARY_DIRS="${BOOST_ROOT}/lib"
            export BOOST_INCLUDE_DIRS BOOST_LIBRARY_DIRS

            pathprepend "$BOOST_INCLUDE_DIRS" CPLUS_INCLUDE_PATH
            pathprepend "$BOOST_LIBRARY_DIRS" LD_RUN_PATH
            export CPLUS_INCLUDE_PATH LD_RUN_PATH

            #INCLUDE_PATH="$BOOST_ROOT/include:$INCLUDE_PATH"
            #export INCLUDE_PATH

            #fhs_path_setup_for "$BOOST_ROOT"
        else
            echo "Couldn't find Boost installation @ \$BOOST_ROOT=\"${BOOST_ROOT}\"."
            unset BOOST_ROOT
        fi
    fi
else
    echo "No “ Boost C++ modular ” sources found at '$boost_modular_dir', skipping..."
fi

Env=( CC CXX BOOST_ROOT BOOST_INCLUDE_DIRS BOOST_LIBRARY_DIRS CPATH CPLUS_INCLUDE_PATH LD_RUN_PATH )

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
