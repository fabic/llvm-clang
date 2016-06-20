#!/bin/bash
#
# fabic/2016-06-20

here=$(dirname "$0")

pushd "$here" || exit 127

echo
echo "+-- $0"
echo "| We're here at: `pwd` (rel.: $here)"
echo "|"

if [ -d build ]; then
    echo "| Removing build/ directory."
    rm -rf build/
fi

mkdir build &&
cd build || exit 126

cmake -G Ninja ..

retv=$?
if [ $retv -gt 0 ]; then
    echo "|"
    echo "| CMake failed, exit status: $retv"
    echo "|"
    exit 125
fi

echo "|"
echo "| Running make..."
echo "|"

#time make
time ninja -v

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
popd &&
    find "$here/build/" \( -type d -name CMakeFiles -prune \) -o -type f -perm -a+x -ls

echo "|"
echo "| FINISHED, exit status: $retv"
echo "|"
echo "+-- $0"
echo

exit $retv
