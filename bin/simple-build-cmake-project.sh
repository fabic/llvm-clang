#!/bin/bash
#
# fabic/2016-06-20

# Root of the llvm-clang project tree :
rewt="$( cd `dirname "$0"`/.. && pwd )"

# Current source directory :
here="$( pwd )"

pushd "$here"

# FHS-like local/ dir.
localdir="$(mkdir -pv "$rewt/local" && cd "$rewt/local" && pwd)"

# Target directory for out-of-tree build :
builddir="build"

echo
echo "+-- $0 $@"
echo "|"
echo "| Current dir. : `pwd`"
echo "|"

do_rebuild="no"

cmake_binary=$( type -p cmake )
cmake_generator="Unix Makefiles"

cmake_extra_args=( )
make_extra_args=( )


# Parse command line arguments for both CMake, and eventually make/ninja
# (which are separated by '--')
#
# Consume a few special arguments which may be the CMake -G <generator>
# (for ex. 'ninja'), and 'rebuild'.
if [ $# -gt 0 ];
then
  while [ $# -gt 0 ];
  do
    case "$1" in
    "make")
        cmake_generator="Unix Makefiles"
        shift
        ;;
    "ninja")
        cmake_generator="Ninja"
        echo "| Ninja is `type -p ninja`"
        echo "|   -> version `ninja --version`"
        shift
        ;;
    "rebuild")
        do_rebuild="yes"
        echo "| Rebuild asked (will remove the build dir. '$builddir')"
        shift
        ;;
    *) # Stop once we fing something we don't recognize.
        break
    esac
  done

    # Consume extra. arguments for CMake, until we hit the '--' separator.
    while [ $# -gt 0 ];
    do
      arg="$1"
      shift

      cmake_extra_args=( "${cmake_extra_args[@]}" "$arg" )

      [ "$arg" == "--" ] && break
    done

    # Consume extra. arguments for make/ninja
    # (until we hit the '--' separator).
    while [ $# -gt 0 ];
    do
      arg="$1"
      shift

      make_extra_args=( "${make_extra_args[@]}" "$arg" )

      [ "$arg" == "--" ] && break
    done

    echo "| Ok, CMake generator -G $cmake_generator"

    if [ $# -gt 0 ]; then
        echo "+-"
        echo "| WARNING: extraneous arguments were provided, these will be ignored :"
        echo "|          $0 [...] $@"
        echo "+-"
    fi
fi


  if [ ${#cmake_extra_args[@]} -gt 0 ]; then
    echo "| Additional CMake arguments :"
    echo "|"
    echo "|   ${cmake_extra_args[@]}"
    echo "|"
  fi

  if [ ${#make_extra_args[@]} -gt 0 ]; then
    echo "| Additional $cmake_generator arguments :"
    echo "|"
    echo "|   ${make_extra_args[@]}"
    echo "|"
  fi


cmake_args=(
  -G "$cmake_generator"
  -DFABIC_LOCAL_DIR="$localdir"
  -DCMAKE_INSTALL_PREFIX="$localdir"
  #-DCMAKE_BUILD_TYPE=Debug
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
  -Wdev
  # --warn-uninitialized
  --clean-first
  "${cmake_extra_args[@]}"
  ..
)

make_args=(
    "${make_extra_args[@]}"
  )


# Remove build/ subdir. only if no command line
# arguments were provided (i.e. targets) :
if [ "x$do_rebuild" == "xyes" ] &&
  [ -d "$builddir" ];
then
    echo "| Removing $builddir/ directory."
    rm -rf "$builddir"
fi


# Create BUILD/ if not exists :
if [ ! -d "$builddir" ]; then
    echo "| Creating $builddir/ sub-directory."
    mkdir "$builddir" ||
      exit 126
fi


# Enter BUILD/ dir.
if ! cd "$builddir" ; then
    echo "| FAILED: could not ch. dir. into '$builddir/'"
    exit 125
fi


echo "| Running CMake..."
echo "|"
echo "|   $cmake_binary \\"
echo "|     ${cmake_args[@]}"
echo "|"
echo "|   » CMAKE_EXPORT_COMPILE_COMMANDS=ON"
echo "|     ^ so that we get a 'compilation_commands.json' file"
echo "|       See http://clang.llvm.org/docs/HowToSetupToolingForLLVM.html"
echo "|     ^ for us to play with Clang Tooling abilities (which needs to build"
echo "|        a “compilation database”)."
echo

"$cmake_binary" \
  "${cmake_args[@]}"

retv=$?
if [ $retv -gt 0 ]; then
    echo
    echo "| CMake failed, exit status: $retv"
    echo "+-"
    exit 125
fi


read -p "CMake completed, proceed with actual build (make/ninja) ?"


if [ "$cmake_generator" == "Ninja" ];
then
  echo
  echo "+- Running Ninja..."
  echo "|"

  time \
    ninja \
      "${make_extra_args[@]}"

  retv=$?
  if [ $retv -gt 0 ]; then
      echo
      echo "| Ninja failed, exit status: $retv"
      echo "+"
      exit 125
  fi

  echo
  echo "| Ninja finished, ok"
  echo "+-"
elif [ "$cmake_generator" == "Unix Makefiles" ];
then
  echo
  echo "+- Running make..."
  echo "|"

  time \
    make \
      "${make_extra_args[@]}"

  retv=$?
  if [ $retv -gt 0 ]; then
      echo
      echo "| make failed, exit status: $retv"
      echo "+"
      exit 125
  fi

  echo
  echo "| make completed, ok"
  echo "+-"
else
  echo "|"
  echo "+- $0: ERROR: unknown \"generator\" \$cmake_generator='$cmake_generator'"
  exit 3
fi


# move out of BUILD/ (return to previous dir.)
echo "popd" &&
popd

# equiv. to $here actually...
prevdir="${OLDPWD}"

echo "|"
echo "| List of executable files under '$prevdir' :"
echo "|"

echo &&
  find "$prevdir" \( -type d -name CMakeFiles -prune \) -o -type f -perm -a+x -print0 | \
    xargs -0r ls -ltrh | \
      sed -e 's@^@|    &@'

echo "|"
echo "+-- $0 : FINISHED, exit status: $retv"
echo

exit $retv

# vim: et ts=4 sw=4
