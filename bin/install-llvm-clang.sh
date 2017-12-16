#!/usr/bin/env bash
#
# fabic/2016-06-30
#
# 1st argument is the target installation directory, defaults to local/
# 2nd arg. is the targed temporary out-of-tree build dir.
#
# See also Gist https://gist.github.com/zchee/740e99acd893afeeae6d `build-llvm.bash`

here=$(cd `dirname "$0"`/.. && pwd)

. "$here/functions.sh"

# Will build both the Sphinx documentation _and_ the Doxygen one.
enable_build_docs=0

enable_sphinx_doc=0
enable_doxygen_doc=0
perform_make_install=0

enable_lto=0

echo "+--- $0"
echo "|"
echo "| This script builds the whole LLVM Clang compiler"
echo "|"
echo "| See also zchee's Gist \`build-llvm.bash\` :"
echo "|"
echo "|   https://gist.github.com/zchee/740e99acd893afeeae6d"
echo "|"

  cd "$here/llvm-clang" || exit 1

  localdir=${1:-"$(mkdir -p "$here/local" && cd "$here/local" && pwd)"}
  builddir=${2:-"build"}

  # Ensure destination "install target" directory is an absolute path
  # (relative to $here) :
  if [ "X${localdir#/}" == "X${localdir}" ]; then
    echo "| First argument '$localdir' ain't an absolute path, "
    echo "| prefixing with current directory :"
      localdir="$here/$localdir"
    echo "|"
    echo "|   \$localdir = $localdir"
    echo "|"
  fi

echo "| Entered `pwd`/"
echo "|"
echo "|   \$localdir = '$localdir'"
echo "|   \$builddir = '$builddir'"


##
function show_some_sensitive_settings() {
  echo "|"
  echo "| FYI: Here's the value of sensitive environment variables :"
  echo "|"
  echo "|   \$CC  = $CC    (`type -p "$CC"`  )"
  echo "|   \$CXX = $CXX  (`type -p "$CXX"`)"
  echo "|"
  echo "|   \$CFLAGS   = $CFLAGS"
  echo "|   \$CXXFLAGS = $CXXFLAGS"
  echo "|   \$LDFLAGS  = $LDFLAGS"
  echo "|"
  echo "|   \$CPATH              = $CPATH"
  echo "|   \$INCLUDE_PATH       = $INCLUDE_PATH"
  echo "|   \$CPLUS_INCLUDE_PATH = $CPLUS_INCLUDE_PATH"
  echo "|"
  echo "|   \$LD_RUN_PATH     = $LD_RUN_PATH"
  echo "|   \$LD_LIBRARY_PATH = $LD_LIBRARY_PATH"
  echo "|   \$LIBRARY_PATH    = $LIBRARY_PATH"
  echo "|"
  echo "|   ^ please ensure that somehow... (TODO: talk!)"
  echo "|"
  echo "| NOTE libcxx & libcxxabi : if you have built these two separately"
  echo "|      and installed these somewhere, please ensure if needs be that"
  echo "|      the above settings are correct."
  echo "|"
  echo "| Note that you may [re-]source the shell script '$here/environment-clang.sh'"
  echo "| which may set things up correctly if you're lucky."
  echo "+-"
}

show_some_sensitive_settings

# ^ env. var. details.
##


  # Ask/warn early about an eventually existing bootstrap/ directory.
  # DON'T !
  # if [ -d bootstrap ]; then
  #   read -p "| REMOVE existing temporary `pwd`/bootstrap/ directory, Ok ?"
  #   rm -rf bootstrap || exit 127
  # fi

  # Ask/warn early about an eventually existing build/ directory.
  if [ -d "$builddir" ]; then
    read -p "| REMOVE existing temporary `pwd`/$builddir/ directory, Ok ?  [no] " -i "yes" answer
    if [ "$answer" == "yes" ]; then
      rm -rf "$builddir" || exit 127
    else
      echo "| OK: _not_ removing the build directory."
    fi
  fi


echo
read -p "+~~> Ok to proceed ? (Ctrl-C to abort) "
echo


echo "|"
echo "| Git submodule checkout llvm clang clang-tools-extra compiler-rt libcxx libcxxabi..."
echo "|"

  if ! git submodule update --init \
    llvm clang clang-tools-extra compiler-rt \
    libcxx libcxxabi ;
  then
    retv=$?
    echo "| FAIL: Git submodule exited with status : $retv"
    echo "+-"
    exit $retv
  fi

echo "| Done with Git submodule checkout, ok."
echo "+-"

echo "|"
echo "| Dropping symlinks here'n'there :"
echo "|"
echo

  ln -sfnv ../../clang              llvm/tools/        &&
  ln -sfnv ../../clang-tools-extra/ clang/tools/extra  &&
  ln -sfnv ../../compiler-rt        llvm/projects/     &&
  ln -sfnv ../../libcxx{,abi}       llvm/projects/
  retv=$?

  echo
  if [ $retv -ne 0 ];
  then
    echo "| Ooops! something went wrong with the symlinking, exiting."
    exit $retv
  fi

  # LLDB ( http://lldb.llvm.org/build.html#BuildingLldbOnLinux )
  if [ -e lldb/CMakeLists.txt ]; then
    echo "+--"
    echo "| Found checked-out sources of LLDB, symlinking into llvm/tools/"
    echo "|"
    ln -sfnv ../../lldb llvm/tools/
    echo "|"
    echo "+--"
  fi

  # LLD ( http://lld.llvm.org/getting_started.html )
  if [ -e lld/CMakeLists.txt ]; then
    echo "+--"
    echo "| Found checked-out sources of LLD linker, symlinking into llvm/tools/"
    echo "|"
    ln -sfnv ../../lld llvm/tools/
    echo "|"
    echo "+--"
  fi

  # Output summary of sub-projects symlinks for information.
  if true;
  then
    echo "| FYI: Done with symlinking stuff into the LLVM tree :"
    echo "|"
    for symlnk in llvm/tools/{clang,lldb,lld} \
                  clang/tools/extra           \
                  llvm/projects/{compiler-rt,libcxx{,abi}} ;
    do
      if [ ! -e "$symlnk" ]; then
        echo "| - Symbolic link (or sub-dir.) \"$symlnk\" does not exist."
      elif [ -h "$symlnk" ]; then
          echo " - Found symbolic link \"$symlnk\", ok"
      else
        echo " - Found \"$symlnk\" but it _isn't_ a symbolic link :"
        ls -ld $symlnk
      fi
    done

    echo "|"
    echo "+"
    echo "| NOTE: that you may want to remove a few of those which are optional :"
    echo "|"
    echo "|       llvm/tools/lld     (LLD linker)"
    echo "|       llvm/tools/lldb    (LLDB debugger)"
    echo "|"
    echo "|       llvm/projects/compiler-rt"
    echo "|       llvm/projects/libcxx"
    echo "|       llvm/projects/libcxxabi"
    echo "|"
    echo "|       clang/tools/extra  (Clang tools extra)"
    echo "|"
    echo "| For example :"
    echo "|   $  cd llvm-clang/"
    echo "|   $  rm -i llvm/tools/lld llvm/tools/lldb clang/tools/extra"
    echo "|   $  rm -i llvm/projects/compiler-rt llvm/projects/libcxx llvm/projects/libcxxabi"
    echo "|"
    read -p "Hit Ctrl-Z now if you feel like so; or continue ?"
    echo "|"
    echo "+-"
  fi

###
## Step #1 : Build a bootstrap version under bootstrap/

if false; then
  echo "TODO: have a bootstrap stage ?"
  exit 127
  # TODO...
  # TODO...
  # TODO...
fi

# ^ end of step #1 bootstrap build.
##

if true; then
  pushd .

    echo "+-"
    echo "| About to build LLVM/Clang compiler !"
    echo "|"
    echo "| Entering temporary the build directory '$builddir'."

    if [ ! -d "$builddir" ]; then
      if ! mkdir "$builddir" || ! cd "$builddir" ;
      then
        echo "+---> FAILED to create and/or enter the temporary build dir.: '$builddir'"
        exit 126
      else
        echo "|"
        echo "| Ok, current dir. is now '`pwd`'"
        echo "|"
      fi
    else
      echo "| Entering existing build dir. '$builddir'"
      cd "$builddir" || exit 122
    fi

    cmake_args=(
      -DCMAKE_BUILD_TYPE=RelWithDebInfo
      -DCMAKE_INSTALL_PREFIX="$localdir"
      -DBUILD_SHARED_LIBS=ON
      #-DLLVM_TARGETS_TO_BUILD="host;X86"
      -DLLVM_TARGETS_TO_BUILD="X86"
      -DLLVM_OPTIMIZED_TABLEGEN=ON
      #-DLLVM_BUILD_LLVM_DYLIB=OFF

      # See further below $enable_XXX_doc
        #-DLLVM_BUILD_DOCS=OFF
        #-DLLVM_ENABLE_SPHINX=OFF
        #-DLLVM_ENABLE_DOXYGEN=OFF

      #-DLLVM_ENABLE_FFI=ON  (done below)
      -DLLVM_ENABLE_EH=ON
      -DLLVM_ENABLE_RTTI=ON
      -DLLVM_ENABLE_CXX1Y=ON

      # Explicitely enable assertion for any type of build
      # (defaults to ON only if Debug).
      -DLLVM_ENABLE_ASSERTIONS=ON

      # Done further below.
      #-DLLVM_ENABLE_LTO=ON
      #-DLLVM_BINUTILS_INCDIR="$localdir/include"

      # -DLIBCXXABI_USE_LLVM_UNWINDER=ON

      # Appended last later on.
        #-G Ninja
        #../llvm
      )

# todo: conditionnally enable building LLVMGold iff `local/include/plugin-api.h`
#       exists in include path ? how?
# todo: check if we do need to pass arg. DLLVM_BINUTILS_INCDIR=...

#-DCMAKE_CXX_FLAGS="-stdlib=libc++"

    # Find out if we have libffi.
    if pkg-config --exists libffi ; then
        libffi_include_dir="$(pkg-config --variable=includedir libffi )"
        libffi_lib_dir="$(pkg-config --variable=libdir libffi)"
        echo "+-- libffi was found (pkg-config says)"
        echo "|"
        echo "| libffi_include_dir = '$libffi_include_dir'"
        echo "| libffi_lib_dir     = '$libffi_lib_dir}'"
        echo "|"
        echo "| We'll pass -DLLVM_ENABLE_FFI=ON to CMake."
        echo "|"
        echo "| ( http://llvm.org/docs/CMake.html#frequently-used-cmake-variables )"
        cmake_args=( "${cmake_args[@]}"
            -DLLVM_ENABLE_FFI=ON
            -DFFI_INCLUDE_DIR="$libffi_include_dir"
            -DFFI_LIBRARY_DIR="${libffi_lib_dir}"
            )
        echo "+-"
    fi

    # Python Sphinx documentation generator.
    if [ $enable_sphinx_doc -gt 0 ] && type -p sphinx-build > /dev/null ; then
      echo "+- Found Sphinx documentation generator, ok."
      cmake_args=( "${cmake_args[@]}" -DLLVM_ENABLE_SPHINX=ON )
    else
      cmake_args=( "${cmake_args[@]}" -DLLVM_ENABLE_SPHINX=OFF )
      enable_sphinx_doc=0
    fi

    # Doxygen documentation generator.
    if [ $enable_doxygen_doc -gt 0 ] && type -p doxygen > /dev/null ; then
      echo "+- Found Doxygen documentation generator, ok."
      cmake_args=( "${cmake_args[@]}" -DLLVM_ENABLE_DOXYGEN=ON )
    else
      # Explicitely state we do not want Doxygen as CMake tries
      # to look for it even if we didn't set it to ON.
      cmake_args=( "${cmake_args[@]}" -DLLVM_ENABLE_DOXYGEN=OFF )
      enable_doxygen_doc=0
    fi

    # This would enable building both the Sphinx _and_ Doxygen
    # documentation.
    if [ $enable_build_docs -gt 0 ]; then
      cmake_args=( "${cmake_args[@]}" -DLLVM_BUILD_DOCS=ON )
    else
      cmake_args=( "${cmake_args[@]}" -DLLVM_BUILD_DOCS=OFF )
    fi

    # Binutils ld.gold => LTO ?
    if ! is_darwin_macosx ; then
      if [ $enable_lto -gt 0 ] && type -p ld.gold > /dev/null ; then

        echo "+- Found GNU Binutils' \`ld.gold\` => enabling LTO feature."
        cmake_args=( "${cmake_args[@]}" \
            -DLLVM_ENABLE_LTO=ON
            #-DLLVM_ENABLE_LTO=Full
            -DLLVM_BINUTILS_INCDIR="$localdir/include"
            # NOTE: this is unrelated to LTO, this instructs that LLVM/Clang/etc
            #       be linked with ld.gold.
            #-DLLVM_USE_LINKER=gold
            # ^ FIXME: Breaks the build with :
            #          “hidden symbol `__morestack` in /usr/lib64/gcc/x86_64-pc-linux-gnu/6.3.1/libgcc.a(morestack.o) is referenced by DSO”.
          )

        # (The correct include path will contain the file plugin-api.h.)
        if [ ! -f "$localdir/include/plugin-api.h" ]; then
          echo "| WARNING: Found Binutils' \`ld.gold\` but the 'plugin-api.h' header file"
          echo "|          wasn't found under $localdir/include/"
          echo "|          (this is fine if your distribution provides it, ex. under /usr/include)."
          if [ ! -f "/usr/include/plugin-api.h" ]; then
            echo "| WARNING: plugin-api.h _wasn't_ found under /usr/include/ either :-|"
          fi
        fi
      fi

    else
      echo "+-- FIXME: Mac OS X: we're _NOT_ passing DLLVM_ENABLE_LTO=ON/Full/Thin"
      echo "|          (it breaks the build / 2017-02-20)."
    fi

    ## CCACHE
    #
    # ( https://crascit.com/2016/04/09/using-ccache-with-cmake/ )
    # This does not (?) work well, better option is probably to patch
    # llvm/CMakeLists.txt with `llvm-clang/fabic-patch-llvm-CMakeLists.txt-ccache.diff`
    # so that CCACHE setup from within CMAKE.
    #
    # if type -p ccache > /dev/null ; then
    #   echo "+- Found \`ccache\` !"
    #   cmake_args=( "${cmake_args[@]}" -DRULE_LAUNCH_COMPILE="ccache" )
    # fi

    # Use Ninja if available...
    if type -p ninja > /dev/null ; then
      echo "+- Found Ninja at `type -p ninja` (we'll pass -G Ninja to CMake)"
      cmake_args=( "${cmake_args[@]}" -G Ninja )
    fi

    # Finally append the source tree path of LLVM :
    cmake_args=( "${cmake_args[@]}" ../llvm )

    function display_cmake_command_fyi()
    {
      echo "|"
      echo "|   cmake ${cmake_args[@]}"
      echo "|"
    }

    echo "+ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    echo "|"
    echo "| About to run CMake :"

    display_cmake_command_fyi

    echo "| Note that we're passing the following -Dxxx CMake options :"
    echo "|   - BUILD_SHARED_LIBS=ON                 (defaults to OFF)"
    echo "|   - LLVM_BUILD_LLVM_DYLIB=OFF            (defaults to OFF)"
    echo "|   - LLVM_TARGETS_TO_BUILD=\"host;X86\"     (defaults to 'all')"
    #echo "|   -CMAKE_CXX_FLAGS=\"-stdlib=libc++\"   (!!! BEWARE !!!)"
    echo "|   - LLVM_ENABLE_EH=ON"
    echo "|   - LLVM_ENABLE_RTTI=ON"
    echo "|   - LLVM_ENABLE_CXX1Y=ON"
    echo "|"
    echo "| See http://llvm.org/docs/CMake.html"
    echo "| See http://www.linuxfromscratch.org/blfs/view/svn/general/llvm.html"
    echo "|"
    echo "+ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~"
    echo


      read -p "Ok to proceed ? (Ctrl-C to abort)"
      echo


      time \
        cmake "${cmake_args[@]}"


      retv=$?
      if [ $retv -ne 0 ]; then
        echo "| CMake failed, exit status $retv"
        echo "|"
        echo "| Command was :"
        display_cmake_command_fyi
        echo "+-"
        exit $retv
      fi

    echo
    echo "+ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    echo "| CMake completed, please review the following stuff just in case"
    echo "| (we didn't alter anything by the way, it's your settings!)"
    echo "|"
    show_some_sensitive_settings

    echo
    read -p "Ok to proceed ? (Ctrl-C to abort)"
    echo


    echo
    echo "+ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    echo "|"
    echo "| Running Ninja..."
    echo "|"
    echo "| (patience, this will take a while, like 1 hour or so)."
    echo "|"
    echo "+ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~"


    if [ `uname -s` != "Darwin" ]; then
      max_jobs=`how_many_cpus 2`
      max_sys_load=`max_load_level`
      keep_going=1
    else
      max_jobs=2
      max_sys_load=2
      keep_going=1
    fi

    echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
    echo "| FYI: Running Ninja with :"
    echo "|        -j$max_jobs  : max. simultaneous jobs,"
    echo "|    and -l$max_sys_load  : max. system load level."
    echo "|    and -k$keep_going  : max. error count (keep going as far as possible)."
    echo "+-"
    echo


      time \
        ninja             \
          -j$max_jobs     \
          -l$max_sys_load \
          -k$keep_going


      retv=$?
      if [ $retv -ne 0 ];
      then
        echo
        echo "+ ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !"
        echo "|"
        echo "| Build failed, Ninja exited with status $retv"
        echo "|"
        echo "| CMake command was :"
        display_cmake_command_fyi
        echo "+-- $0 ~~~> FAILED."
        echo
        exit $retv
      fi

    echo "|"
    echo "| Finished build of LLVM/Clang !  \\^^/"
    echo "|"
    echo "+-"
    echo

  if [ $perform_make_install -gt 0 ];
  then
    echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
    echo "|"
    echo "| Running Ninja install..."
    echo "|"
    echo "| (your stuff should go under '$localdir',"
    echo "|  if everything is setup correctly here)."
    echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
    echo

      ninja install

      retv=$?
      if [ $retv -ne 0 ]; then
        echo
        echo "+ ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !"
        echo "|"
        echo "| FAIL: Ninja install failed with exit status $retv"
        display_cmake_command_fyi
        echo "+-"
        echo
        exit $retv
      fi

    echo
    echo "+"
    echo "|"
    echo "| Ninja install done ok ! (probably)  \\o/"
    echo "|"
  fi

  echo "+-"
  echo "| FYI: The CMake command used previously was :"
  echo "|"
  echo "| cd `pwd`"
  display_cmake_command_fyi

  popd
fi


echo "+--- $0 : FINISHED, exit status: $retv"

exit $retv
# vim: et sw=2 ts=2

