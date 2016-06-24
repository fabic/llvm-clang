#!/bin/bash
#
# fabic/2016-06-20

here=$(dirname "$0")

pushd "$here" || exit 127

echo
echo "+-- $0"
echo "| We're here at: `pwd` (rel.: $here)"
echo "|"

if [ $# -ge 1 ]; then
    echo "| Ok, additional arguments provided will be for Ninja :"
    echo "|"
    echo "|   $@"
    echo "|"
fi

# Remove build/ subdir. only if no command line
# arguments were provided (i.e. targets) :
if false && [ -d build ] && [ $# -eq 0 ]; then
    echo "| Removing build/ directory."
    rm -rf build/
fi

if [ ! -d build ]; then
    echo "| Creating build/ sub-directory."
    mkdir -v build ||
      exit 126
fi

if ! cd build/ ; then
    echo "| FAILED: could not ch. dir. into 'build/'"
    exit 125
fi

echo "|"
echo "| Running CMake..."
echo "|"

cmake -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -Wdev --warn-uninitialized --clean-first ..

retv=$?
if [ $retv -gt 0 ]; then
    echo "|"
    echo "| CMake failed, exit status: $retv"
    echo "|"
    exit 125
fi

echo "|"
echo "| Running Ninja..."
echo "|"

#time make
time ninja -v "$@"

retv=$?
if [ $retv -gt 0 ]; then
    echo "|"
    echo "| make failed, exit status: $retv"
    echo "|"
    exit 125
fi

echo "|"
echo "| Ninja finished, ok"
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
echo "| FINISHED, exit status: $retv"
echo "|"
echo "+-- $0"
echo

exit $retv

# vim: et ts=4 sw=4
