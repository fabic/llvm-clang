# clang-environment.sh

here=$( cd `dirname "${BASH_SOURCE[0]}"` && pwd )

. $here/functions.sh

echo "+--- ${BASH_SOURCE[0]} $@"
echo "|"

# FHS-like/compliant local/ dir.
localdir=${1:-"$here/local"}
localdir=${localdir%/}

# Prepend local/bin to PATH (hopefully we have a Clang install. there).
if [ -d "$localdir/bin" ]; then
    echo "| Found directory '$localdir/bin' : prepending it to \$PATH. "
    pathprepend "$here/local/bin"
else
    echo "| FYI: No FHS-like '$localdir' directory, ok this is fine."
fi


echo "| Searching for Clang/++"
ClangBin=$(type -p clang)
ClangCppBin=$(type -p clang)

echo ${ClangBin} -- ${ClangBin#$localdir/bin/}
echo "^^^ TODO ^^^"
unset ClangBin ClangCppBin


# Check/output Clang version
echo "| Checking Clang actually would execute (--version) :"
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
    pathprepend "$here/local/include" CPLUS_INCLUDE_PATH
    pathprepend "$here/local/include" INCLUDE_PATH
    export CPATH CPLUS_INCLUDE_PATH
fi

if [ -d "$here/local/lib" ]; then
    pathprepend "$here/local/lib" LD_RUN_PATH
    pathprepend "$here/local/lib" LIBRARY_PATH
    pathprepend "$here/local/lib" LD_LIBRARY_PATH
    export LD_RUN_PATH LIBRARY_PATH
fi

if [ -d "$here/local/include/c++/v1" ] && [ -f "$here/local/include/c++/v1/cxxabi.h" ]; then
    echo "|"
    echo "| » Hey! good, found local/include/c++/v1 (LLVM/Clang libc++/abi STL implementation)"
    echo "|"
    #CXXFLAGS="-std=c++1y -stdlib=libc++ $CXXFLAGS"
    #CXXFLAGS="-g -Wall $CXXFLAGS"
    pathprepend "$here/local/include/c++/v1" CPLUS_INCLUDE_PATH
    export CXXFLAGS CPLUS_INCLUDE_PATH
fi

sh $here/show-environment.sh

# vim: et sw=4 ts=4 ft=sh
