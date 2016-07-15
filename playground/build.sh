#!/bin/bash
#
# fabic/2016-06-20

here=$(dirname "$0")

pushd "$here" || exit 127

echo
echo "+-- $0"
echo "| We're here at: `pwd` (rel.: $here)"
echo "|"

cmake_binary=$( type -p cmake )
cmake_generator="Unix Makefiles"

cmake_extra_args=( )
make_extra_args=( )


# One first special arg. may be the CMake -G <generator> :
if [ $# -gt 0 ];
then
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
    *)
        ;;
    esac

    # Extra. arguments for CMake
    while [ $# -gt 0 ];
    do
      arg="$1"
      shift

      cmake_extra_args=( "${cmake_extra_args[@]}" "$arg" )

      [ "$arg" == "--" ] && break
    done

    # Extra. arguments for make/ninja.
    while [ $# -gt 0 ];
    do
      arg="$1"
      shift

      make_extra_args=( "${make_extra_args[@]}" "$arg" )

      [ "$arg" == "--" ] && break
    done

    echo "| Ok, CMake generator -G $cmake_generator"
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
  -DCMAKE_BUILD_TYPE=Debug
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
  -Wdev
  --warn-uninitialized
  --clean-first
  "${cmake_extra_args[@]}"
  ..
)

make_args=(
    "${make_extra_args[@]}"
  )

# Remove build/ subdir. only if no command line
# arguments were provided (i.e. targets) :
if false && [ -d build ] && [ $# -eq 0 ]; then
    echo "| Removing build/ directory."
    rm -rf build/
fi

if [ ! -d build ]; then
    echo "| Creating build/ sub-directory."
    mkdir build ||
      exit 126
fi

if ! cd build/ ; then
    echo "| FAILED: could not ch. dir. into 'build/'"
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


echo "|"
echo "| List of executable files under '$here/build/' :"
echo "|"

# move out of build/ (return to previous dir.)
echo &&
popd &&
echo &&
  find "$here/build/" \( -type d -name CMakeFiles -prune \) -o -type f -perm -a+x -print0 | \
    xargs -0r ls -ltrh | \
      sed -e 's@^@|    &@'

echo "|"
echo "+-- $0 : FINISHED, exit status: $retv"
echo

exit $retv

# vim: et ts=4 sw=4
