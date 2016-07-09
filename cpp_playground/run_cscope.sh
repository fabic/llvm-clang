#!/bin/sh

here=$( cd `dirname "$0"` && pwd )

cd "$here" || exit 1

localdir=$(cd ../local && pwd)

echo "+-- $0"
echo "| In $here"
echo "|"

locations=(
    "$here" \
	"$localdir/include"
    #/usr/include/cairo
    #/usr/include/cairomm-1.0
    /usr/include/glibmm-2.4
    /usr/include/gdkmm-3.0
    /usr/include/gtkmm-3.0
    /usr/include/pangomm-1.4
    /usr/include/X11
    /usr/include/xcb
    "$localdir/boost-1.61.0-clang/include/boost/"
    $(cd ../misc/cpp-netlib/boost/ && pwd)
  )


find_args=(
    "${locations[@]}"
    -type f
    \(
         -name "*.[chxsS]"
      -o -name "*.hpp"
      -o -name "*.hxx"
      -o -name "*.cpp"
      -o -name "*.cxx"
    \)
    -print
  )


echo "| Generating the cscope.files (\`find ...) :\`"
echo "|"
echo "|   find \"${find_args[@]}\" | sort > cscope.files"
echo "|"

find "${find_args[@]}" \
  | sort \
    > cscope.files

retv=$?

if [ $retv -ne 0 ];
then
  echo
  echo "| \`find ... \` command failed with exit status $retv"
  exit $retv
fi


echo "|"
echo "| Running \`cscope -b -q\`"

time \
  cscope -b -q

if [ $retv -ne 0 ];
then
  echo
  echo "| \`cscope ... \` command failed with exit status $retv"
  exit $retv
fi


echo "| Cscope files :"
echo "|"

ls -lh cscope.* | sed -e 's/^/|  &/'


echo "+-- $0 : END, exit status: $retv"

exit $retv
