# environment-boost-cpp.sh

here=$( cd `dirname "${BASH_SOURCE[0]}"` && pwd )

. $here/functions.sh

echo "+-- ${BASH_SOURCE[0]}"


# Check for CC & CXX :
if [ -z "$CC" ]; then
    echo "| ERROR: \$CC is not defined"
    exit 127
elif [ -z "$CXX" ]; then
    echo "| ERROR: \$CXX is not defined"
    exit 126
else
    echo "| \$CC  = $CC"
    echo "| \$CXX = $CXX"
fi


#
# Boost C++ library
#

# Boost “ modular ” Git sources repository :
boost_modular_dir="$here/misc/boost"

# 1) try local/boost-1.61.0-clang/ install location
BOOSTROOT="$( find "$here/local" -maxdepth 1 -type d -name "boost-*-${CC}" )"

echo -n "| Trying \$BOOSTROOT = $BOOSTROOT : "

# 2) fallback to ~/boost-1.61.0-clang/
if [ -d "$BOOSTROOT/include" ] && [ -d "$BOOSTROOT/lib" ]; then
    echo "found."
else
    BOOSTROOT="$HOME/boost-*-${CC}"
fi

# 3) will entail fallback to the checked-out sources under misc/boost/
[ -d "$BOOSTROOT/include" ] && [ -d "$BOOSTROOT/lib" ] || BOOSTROOT="" 

##
## Found Boost installation
##
if [ ! -z "$BOOSTROOT" ];
then
    echo "| FYI: Found Boost C++ @ '$BOOSTROOT', ok."

    export BOOSTROOT

    # This appears to be needed by CMake for it to find Boost,
    # BOOSTROOT ain't enough it appears.
    if false; then
        [ -d "$BOOSTROOT/include" ] && BOOST_INCLUDE_DIRS="${BOOSTROOT}/include"
        [ -d "$BOOSTROOT/lib" ]     && BOOST_LIBRARY_DIRS="${BOOSTROOT}/lib"
        export BOOST_INCLUDE_DIRS BOOST_LIBRARY_DIRS
    fi

    #pathappend "$BOOST_INCLUDE_DIRS" CPLUS_INCLUDE_PATH
    #export CPLUS_INCLUDE_PATH 

    pathappend "$BOOST_LIBRARY_DIRS" LD_RUN_PATH
    pathappend "$BOOST_LIBRARY_DIRS" LD_LIBRARY_PATH
    export LD_RUN_PATH LD_LIBRARY_PATH

    #INCLUDE_PATH="$BOOSTROOT/include:$INCLUDE_PATH"
    #export INCLUDE_PATH

    #fhs_path_setup_for "$BOOSTROOT"

## Failed search for boost installation
##   => fallback to the eventually checked-out raw sources :
else
    echo "| OUPS! Couldn't find out where Boost C++ may be (\$BOOSTROOT)"
    echo "|       Trying '$boost_modular_dir'..."

    if [ -d "$boost_modular_dir/boost" ];
    then
        echo "| Guessing checked-out version (\`git describe\`)"
        # Note 'misc/boost/.git' may be a file instead of a directory in case it
        # is a Git submodule clone (hence the `test -e .git`).
        boost_cpp_version="$(cd "$boost_modular_dir" && test -e .git && git describe)"

        if [ -z "$boost_cpp_version" ]; then
            echo "| Couldn't find Boost C++ library at '$boost_modular_dir', skipping"
        else
            echo "| Found Boost C++ “ modular ” repository checked-out at '$boost_modular_dir' (version: $boost_cpp_version)."

        fi
    else
        echo "| No “ Boost C++ modular ” sources found at '$boost_modular_dir', exiting..."
    fi

    BOOSTROOT="$boost_modular_dir"
    #BOOST_INCLUDE_DIRS="${BOOSTROOT}"
    #BOOST_LIBRARY_DIRS=""

    #pathappend "$BOOST_INCLUDE_DIRS" CPLUS_INCLUDE_PATH

    # TODO ?
    #[ -d "$BOOSTROOT/lib" ] && BOOST_LIBRARY_DIRS="${BOOSTROOT}/lib"
    #pathappend "$BOOST_LIBRARY_DIRS" LD_RUN_PATH

    export BOOSTROOT BOOST_INCLUDE_DIRS CPLUS_INCLUDE_PATH
fi

sh $here/show-environment.sh

# vim: et sw=4 ts=4 ft=sh
