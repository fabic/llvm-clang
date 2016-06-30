# clang-environment.sh

here=$( cd `dirname "${BASH_SOURCE[0]}"` && pwd )

. $here/functions.sh

echo "+--- ${BASH_SOURCE[0]} $@"
echo "|"


# FHS-like/compliant local/ dir.
localdir=${1:-"$(cd "$here/local" && pwd)"}
localdir=${localdir%/}

  # Ensure destination "install target" directory is an absolute path :
  if [ "X${localdir#/}" == "X${localdir}" ]; then
    echo "| First argument '$localdir' ain't an absolute path, "
    echo "| prefixing with current directory :"
      localdir="$(pwd)/$localdir"
    echo "|"
    echo "|   \$localdir = $localdir"
    echo "|"
  fi


# Prepend local/bin to PATH (hopefully we have a Clang install. there).
if [ -d "$localdir/bin" ]; then
    echo "| Found directory '$localdir/bin' : prepending it to \$PATH. "
    pathprepend "$localdir/bin"
else
    echo "| FYI: No FHS-like '$localdir' directory, ok this is not a problem."
fi


#
# Optional, verify if we have a Clang setup under local/
# for information to user.
#
if true; then
    echo "+-"
    echo "| Searching for Clang/++ (just for your information)"

    ClangBin=$(type -p clang)
    ClangCppBin=$(type -p clang++)

    # DEBUG BASH EXPR.
    if false; then
      echo "+-- Bash script debug :"
      echo "+~> \$localdir = $localdir"
      echo "+~> \$ClangBin=$ClangBin -- ${ClangBin#"$localdir/bin/"}"
      echo "+-- Bash script debug :"
    fi

    if [ "X${ClangBin}" == "X${ClangBin#"$localdir/bin/"}" ];
    then
        echo "| -> no clang executable found under '$localdir', ok."
        echo "| -> Clang   would be : $ClangBin"
        echo "| -> Clang++ would be : $ClangCppBin"
        echo "|    ^ based on your \$PATH"
    else
        echo "| -> found clang executable under '$localdir' :"
        echo "|"
        echo "|      $ClangBin"
        echo "|"
        echo "| -> Checking that we also have the clang++ binary (reachable from \$PATH) also lives there :"

        if [ "X${ClangCppBin}" == "X${ClangCppBin#"$localdir/bin/"}" ];
        then
            echo "| -> Huh! beware! clang++ may not reside under $localdir, while cland does, please investigate (but I'm not stopping you here)."
        else
            echo "| -> found clang++ executable under '$localdir' :"
            echo "|"
            echo "|      $ClangCppBin"
        fi
    fi

    echo "|"
    echo "+-"

    # We ain't need those (clang/++ is guessed from $PATH).
    unset ClangBin ClangCppBin
fi


# Check/output Clang version (for information)
if true; then
  echo "+-"
    echo "| Checking Clang/++ actually would execute ( --version arg. ) :"
    echo "| ~> Clang :"
      if ! ( clang --version | sed -e 's/^/|      &/' ); then
        retv=$?
        echo "| ~~~> Oups! couldn't execute clang, exiting (retv=$retv)"
        exit $retv
      fi
    echo "| ~> Clang++ :"
      if ! ( clang++ --version | sed -e 's/^/|      &/' ); then
        retv=$?
        echo "| ~~~> Oups! couldn't execute clang++, exiting (retv=$retv)"
        exit $retv
      fi
  echo "+-"
fi


# Clang LLVM ;-
CC=clang
CXX=clang++
export CC CXX


if [ -d "$localdir/include" ]; then
    pathprepend "$localdir/include" CPATH
    pathprepend "$localdir/include" CPLUS_INCLUDE_PATH
    pathprepend "$localdir/include" INCLUDE_PATH
    export CPATH CPLUS_INCLUDE_PATH
fi


if [ -d "$localdir/lib" ]; then
    pathprepend "$localdir/lib" LD_RUN_PATH
    pathprepend "$localdir/lib" LIBRARY_PATH
    pathprepend "$localdir/lib" LD_LIBRARY_PATH
    export LD_RUN_PATH LIBRARY_PATH
fi


if [ -d "$localdir/include/c++/v1" ] && [ -f "$localdir/include/c++/v1/cxxabi.h" ]; then
    echo "|"
    echo "| » Hey! good, found $localdir/include/c++/v1 (LLVM/Clang libc++/abi STL implementation)"
    echo "|"
    #CXXFLAGS="-std=c++1y -stdlib=libc++ $CXXFLAGS"
    #CXXFLAGS="-g -Wall $CXXFLAGS"
    pathprepend "$localdir/include/c++/v1" CPLUS_INCLUDE_PATH
    export CXXFLAGS CPLUS_INCLUDE_PATH
fi


# Display environment for information
sh $here/show-environment.sh


# vim: et sw=2 ts=2 ft=sh
