# environment-boost-cpp.sh

here=$( cd `dirname "${BASH_SOURCE[0]}"` && pwd )

. $here/functions.sh

echo "+-- ${BASH_SOURCE[0]} $@"
echo "|"
echo "| Note that this script may take the path of a Boost installation as 1st arg."
echo "|"


# Check for CC & CXX :
if [ -z "$CC" ];
then
    echo "| ERROR: \$CC is not defined (we need this to select the appropriate Boost variant you built)."
    echo "|        ^ for we're looking for Boost installed into ex. local/boost-1.xx.yy-clang/ if built with Clang++"
    echo "|          or if built with GCC into ex. local/boost-1.xx.yy-gcc/"
    echo "+-- ${BASH_SOURCE[0]} $@ [END]"
    return 127
elif [ -z "$CXX" ];
then
    echo "| ERROR: \$CXX is not defined."
    echo "+-- ${BASH_SOURCE[0]} $@ [END]"
    return 126
else
    echo "| \$CC  = $CC"
    echo "| \$CXX = $CXX"
    echo "+-"
fi


#
# Boost C++ library
#

# Boost “ modular ” Git sources repository :
boost_modular_dir="$here/misc/boost"


# 1) try local/boost-1.xx.yy-$CC/ install location
#    (if not provide as 1st command line arg.)
#
#    NOTE: first script arg. may be an absolute path.
#
# Beware that it will search under any local*/ here
# (hence searching for backup copies you happen to make sometimes).
# ( ^ and you may not want this btw, fixme ? )
BOOSTROOT=${1:-"$( find "$here/local"*/ -maxdepth 1 -type d -name "boost-*-${CC}" )"}

  # Ensure absolute path
  if [ "X${BOOSTROOT#/}" == "X${BOOSTROOT}" ]; then
    echo "|"
    echo "| ~> \$BOOSTROOT=\"$BOOSTROOT\" ain't an absolute path, "
    echo "|    prefixing with current directory :"
      BOOSTROOT="$(pwd)/$BOOSTROOT"
    echo "|"
    echo "|      \$BOOSTROOT = $BOOSTROOT"
    echo "|"
  fi


echo "|"
echo "|   Note that we're here searching for an \"adequate\" installation of"
echo "|   Boost where we would find the include/boost/config.hpp dir."
echo "|"
echo "|   And we'll attempt falling back to '$boost_modular_dir'"
echo "|   for a checked-out source tree of Boost “ modular ”"
echo "|"


echo -n "| Trying \$BOOSTROOT = $BOOSTROOT : "

if [ -f "$BOOSTROOT/include/boost/config.hpp" ];
then
  echo "found."
else # 2) try fallback to ~/boost-1.61.0-clang/ (i.e. right under $HOME)
  echo "not found."

  BOOSTROOT="$HOME/boost-*-${CC}"

  echo -n "| Trying \$BOOSTROOT = $BOOSTROOT (fallback) : "

  if [ -f "$BOOSTROOT/include/boost/config.hpp" ];
  then
    echo "found."
  else # 3) will entail fallback to the checked-out sources under misc/boost/
    echo "not found (will try eventually fallback to the Boost “modular” sources if checked-out (under '$boost_modular_dir')."
    BOOSTROOT="" 
  fi
fi


##
## Found Boost installation
##
if [ ! -z "$BOOSTROOT" ];
then
    echo "| FYI: Found Boost C++ @ '$BOOSTROOT', ok."

    export BOOSTROOT
    # ^ This one variable is the sole one one needed by CMake's find_package(Boost ...), actually.

    # These two may also help CMake's find_package()...
    [ -d "$BOOSTROOT/include" ] && BOOST_INCLUDE_DIRS="${BOOSTROOT}/include"
    [ -d "$BOOSTROOT/lib" ]     && BOOST_LIBRARY_DIRS="${BOOSTROOT}/lib"
    # ^ !! BUT !! we're _NOT_ exporting these (we want to rely on CMake only).
    #export BOOST_INCLUDE_DIRS BOOST_LIBRARY_DIRS

    # We're _NOT_ adding Boost's include dir. to any env. var.
    # (we leave this to CMake).
    #pathappend "$BOOST_INCLUDE_DIRS" CPLUS_INCLUDE_PATH
    #export CPLUS_INCLUDE_PATH 

    # However we need to set this which usually (in principe) entails that
    # -Wl,-rpath=... is passed to the linker.
    pathprepend "$BOOST_LIBRARY_DIRS" LD_RUN_PATH
    export LD_RUN_PATH

    # However it occurs that we also need LD_LIBRARY_PATH for _some_ libboost_...so
    # happen not to be resolved :-/
    pathprepend  "$BOOST_LIBRARY_DIRS" LD_LIBRARY_PATH
    export LD_LIBRARY_PATH

    #INCLUDE_PATH="$BOOSTROOT/include:$INCLUDE_PATH"
    #export INCLUDE_PATH

    #fhs_path_setup_for "$BOOSTROOT"

## Failed search for boost installation (FALLBACK)
##   => fallback to the eventually checked-out raw sources.
##      (we reach this point iff BOOSTROOT="", see earlier)
else
    echo "+- ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !"
    echo "|"
    echo "| OUPS! Couldn't find out where Boost C++ may be (\$BOOSTROOT)"
    echo "|       Last resort fallback trying an eventually checked-out Boost modular source tree under '$boost_modular_dir'..."
    echo "|"

    if [ -d "$boost_modular_dir/boost" ];
    then
        echo "| ~> Guessing checked-out version (\`git describe\`)"
        # Note 'misc/boost/.git' may be a file instead of a directory in case it
        # is a Git submodule clone (hence the `test -e .git`).
        boost_cpp_version="$(cd "$boost_modular_dir" && test -e .git && git describe)"

        if [ -z "$boost_cpp_version" ]; then
            echo "| ~~~> Couldn't find Boost C++ library at '$boost_modular_dir', skipping"
        else
            echo "| ~~~> Found Boost C++ “ modular ” repository checked-out at '$boost_modular_dir' (version: $boost_cpp_version)."
        fi
    else
        echo "| ~~~> FAIL: No “ Boost C++ modular ” sources found at '$boost_modular_dir', exiting..."
        echo "|"
        echo "+-- ${BASH_SOURCE[0]} $@ [END]"
        return 127
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


# We blindly modified LD_*_PATH, just assert that lib/ exists.
[ -d "$BOOSTROOT/lib" ] || echo "| WARNING: the \$BOOSTROOT/lib/ directory does not exist, FYI."


echo "|"
echo "| Note that we intently did _NOT_ add '$BOOST_INCLUDE_DIRS' to CPATH or CPLUS_INCLUDE_PATH"
echo "| since you're probably using CMake for your stuff."
echo "|"
echo "+-- ${BASH_SOURCE[0]} $@ [END]"


sh $here/show-environment.sh

# vim: et sw=2 ts=2 ft=sh
