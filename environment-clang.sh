# clang-environment.sh

here=$( cd `dirname "${BASH_SOURCE[0]}"` && pwd )

. $here/functions.sh

echo "+--- ${BASH_SOURCE[0]} $@"
echo "|"

# Ah -_- Mac OS X
[ "`uname -s`" == "Darwin" ] && is_macosx=1 || is_macosx=0


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


# Create FHS-like directory structure under local/
#
# NOTE: local/share/aclocal/ is created as it may be needed by autotools,
#       and specifically for building Wayland ;
#       see https://wayland.freedesktop.org/building.html
#
if ! mkdir -pv "$localdir"/{,bin,include,lib/pkgconfig,share/{aclocal,info,man,pkgconfig}}/ ;
then
  echo "| FAILED: something went wrong while creating directories structure under '$localdir'."
  return 1
fi

[ -d "$here/bin" ] && pathprepend "$here/bin"


# Prepend local/bin to PATH (hopefully we have a Clang install. there).
if [ -d "$localdir/bin" ]; then
    echo "| Found directory '$localdir/bin' : prepending it to \$PATH. "
    pathprepend "$localdir/bin"
    export PATH
else
    echo "| FYI: No FHS-like '$localdir' directory, ok this is not a problem."
fi


# PKG-CONFIG
if [ -d "$localdir/lib/pkgconfig" ]; then
    echo "| Found a pkg-config dir. '$localdir/lib/pkgconfig' : prepending it to \$PKG_CONFIG_PATH. "
    pathprepend "$localdir/lib/pkgconfig"   PKG_CONFIG_PATH
    # Wayland also suggests that we have it under local/share/ :
    [ -d "$localdir/share/pkgconfig" ] &&
      pathprepend "$localdir/share/pkgconfig" PKG_CONFIG_PATH
    export PKG_CONFIG_PATH
else
    echo "| FYI: Didn't find a pkg-config dir. at '$localdir/lib/pkgconfig', not setting the \$PKG_CONFIG_PATH env. var."
fi


# AutoTools' aclocal
# ( see https://wayland.freedesktop.org/building.html )
if [ -d "$localdir/share/aclocal" ];
then
  export ACLOCAL_PATH="$localdir/share/aclocal"
  export ACLOCAL="aclocal -I $ACLOCAL_PATH"
  echo "| Set up autotools' ACLOCAL path."
fi


# CMAKE's CMAKE_MODULE_PATH
if false;
then
  if [ -d "$localdir/share" ];
  then
    echo "| Searching for CMake modules paths..."
    export CMAKE_MODULE_PATH="${CMAKE_MODULE_PATH:-}"

    find "$localdir" -type f -name '*.cmake' | \
      while read fn; do
        pathappend "`dirname "$fn"`" CMAKE_MODULE_PATH
      done

    # find "$localdir" -type f -name '*.cmake' | \
    #   while read fn; do
    #     dirname "$fn"
    #   done | \
    #     sort -u | \
    #       while read din; do
    #         pathprepend "$din" CMAKE_MODULE_PATH
    #         echo "|   » Found '$din'"
    #       done

    if [ ! -z "$CMAKE_MODULE_PATH" ];
    then
      echo "| ~> \$CMAKE_MODULE_PATH = $CMAKE_MODULE_PATH"
    else
      echo "| ~> found no CMake additionnal modules paths under $localdir"
    fi
  fi
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
        return $retv
      fi
    echo "| ~> Clang++ :"
      if ! ( clang++ --version | sed -e 's/^/|      &/' ); then
        retv=$?
        echo "| ~~~> Oups! couldn't execute clang++, exiting (retv=$retv)"
        return $retv
      fi
  echo "+-"
fi


# Guess version (may be needed later)
if true; then
  echo "| Finding out which version it is :"
  clang_version="$(clang --version | head -n1 | sed -re 's/^clang version (([0-9]\.?)*).*$/\1/')"
  if [ "x$clang_version" == "x" ]; then
    echo "| ~> Beware: couldn't determine the Clang version."
    echo "|            (this isn't bad though)"
  else
    echo "| ~> Clang version: $clang_version"
  fi
fi


# Clang LLVM ;-
CC=clang
CXX=clang++
export CC CXX


## Extraneous include & lib dir.
if false && [ ! -z "$clang_version" ];
then
  echo "+-- Searching for one additionnal include/ and lib/ dir. under '$localdir/lib/clang/$clang_version/' :"
  echo "|"
  echo "| Doin' so, but this may _not_ be neither needed nor desired."
  echo "| Trouble being that once built a bootstrap build of Clang,"
  echo "| those come very last, way after the GCC and system includes."
  echo "|"

  extra_inc="$localdir/lib/clang/$clang_version/include"
  if [ -d "$extra_inc" ];
  then
    echo "+-"
    echo "| Ok, found the extraneous include dir. '$extra_inc'"
    echo "| ~> Prepending it to CPATH & INCLUDE_PATH (not CPLUS_INCLUDE_PATH: these are C headers seemingly)."
    pathprepend "$extra_inc" CPATH
    pathprepend "$extra_inc" INCLUDE_PATH
    export CPATH INCLUDE_PATH
  fi

  # FIXME: Linux specific sub-dir.
  extra_lib="$localdir/lib/clang/$clang_version/lib/linux"
  if [ -d "$extra_lib" ];
  then
    echo "+-"
    echo "| Ok, found the extraneous library dir. '$extra_lib'"
    echo "| ~> Prepending it to LD_RUN_PATH & LD_LIBRARY_PATH (just in case)..."
    pathprepend "$extra_lib" LD_RUN_PATH
    pathprepend "$extra_lib" LIBRARY_PATH
    pathprepend "$extra_lib" LD_LIBRARY_PATH
    export LD_RUN_PATH LD_LIBRARY_PATH LIBRARY_PATH
  fi

  # And finally the libc++ STL, prepended last, so that it comes first.
  if [ -d "$localdir/include/c++/v1" ] && [ -f "$localdir/include/c++/v1/cxxabi.h" ]; then
    echo "+"
    echo "| ~> Hey! good, found $localdir/include/c++/v1 (LLVM/Clang libc++/abi STL implementation!)"
    echo "|    ( which is what you're looking for quite probably, goin' down this long road and to such extents... )"
    echo "|"
    #CXXFLAGS="-std=c++1y -stdlib=libc++ $CXXFLAGS"
    #CXXFLAGS="-g -Wall $CXXFLAGS"
    pathprepend "$localdir/include/c++/v1" CPLUS_INCLUDE_PATH
    export CXXFLAGS CPLUS_INCLUDE_PATH
  fi

  echo "+-- done (with the extraneous Clang include/ & lib/linux/ sub-dir."
fi


if [ $is_macosx -gt 0 ]; then
  echo "+- Mac OS X “Darwin” that is -_-"
  macosx_sdk="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk"
  if [ -d "$macosx_sdk" ]; then
    #pathprepend "$macosx_sdk/usr/lib/"     LD_RUN_PATH
    #pathprepend "$macosx_sdk/usr/lib/"     LD_LIBRARY_PATH
    pathprepend "$macosx_sdk/usr/include/" CPATH
    export CPATH LD_RUN_PATH LD_LIBRARY_PATH
  else
    echo "| WARNING: The MacOSX.sdk/ directory \`\` $macosx_sdk \`\` does _not_ exists."
    echo "| FYI: Head to https://developer.apple.com/download/more/ and download that XCode thing."
  fi
fi

# Traditionnal /usr/local/include
if [ -d "$localdir/include" ]; then
    pathprepend "$localdir/include" CPATH
    #pathprepend "$localdir/include" CPLUS_INCLUDE_PATH
    #pathprepend "$localdir/include" INCLUDE_PATH
    export CPATH CPLUS_INCLUDE_PATH
fi


# Traditionnal /usr/local/lib
# TODO: lib64 check ?
if [ -d "$localdir/lib" ]; then
    pathprepend "$localdir/lib" LD_RUN_PATH
    #pathprepend "$localdir/lib" LIBRARY_PATH
    pathprepend "$localdir/lib" LD_LIBRARY_PATH
    export LD_RUN_PATH LD_LIBRARY_PATH LIBRARY_PATH
fi



# Display environment for information
#sh $here/show-environment.sh


# vim: et sw=2 ts=2 ft=sh
