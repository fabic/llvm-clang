#!/bin/sh

rewt=$( cd `dirname "$0"`/.. && pwd )

# Move up out of bin/
cd "$rewt" || exit 1

localdir="$(cd "local" && pwd)"
llvmClang="$(cd "llvm-clang" && pwd)"
misc="$(cd "misc" && pwd)"

echo "+-- $0"
echo "| In $rewt"
echo "|"


locations=(
    "$rewt/cpp_playground"
	"$localdir/include"
    /usr/include/X11
    /usr/include/xcb
    "$misc/cairo/src"
    "$llvmClang/clang/examples"
    "$llvmClang/clang/lib"
    "$llvmClang/clang/tools"
    "$llvmClang/clang-tools-extra"
    "$llvmClang/compiler-rt/include"
    "$llvmClang/compiler-rt/lib"
    "$llvmClang/libcxx/src"
    "$llvmClang/libcxxabi/src"
    "$llvmClang/llvm/lib"
    "$llvmClang/llvm/tools"
    "$misc/cpp-netlib/boost/"
    "$misc/glibc"
    "$misc/libunwind/src"
    "$misc/linux-headers/include"
    "$misc/ninja/src"
  )

# BOOST C++ !
if [ ! -z "$BOOSTROOT" ] && [ -d "$BOOSTROOT/include" ];
then
  echo "| Got Boost C++ at \$BOOSTROOT = '$BOOSTROOT'"
  locations=(
      "${locations[@]}"
      "$BOOSTROOT/include"
      )
fi

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


echo "+-"
echo "| Found $(wc -l cscope.files | awk '{print $1}' ) source files (cscope.files)."
echo "|"
read -p "| Run CScope on these ? (Ctrl-C to abort)"


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
