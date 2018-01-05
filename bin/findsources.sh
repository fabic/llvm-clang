#!/bin/bash
#
# FC.2014-09-24 fabic.net

if [ $# -lt 1 ]; then
	echo
  echo "+- $0 $@"
	echo "| Find source code files"
  echo "|"
	echo "| Usage: $0 [<prefix_find_arguments>... --] <suffix_find_arguments>..."
  echo "|"
	echo "| Example: $0 clang/include/ clang/lib llvm/include -- -name '*.c'"
  echo "+-"
	echo
	exit 127
fi

# egrep compatible regular expression, which `find` understand
# thanks to `-regextype egrep`.
regex=".*\.(c|h|cc|hh|cpp|hpp|cxx|hxx|h\.inc|hxx\.inc|hpp.inc|hh.inc|s|x|cmake)$"

leftargs=()
rightargs=()

# Splice left and right hand arguments around '--' :
while [ $# -gt 0 ];
do
	arg="$1"
	shift

  # Met delimiter => left-hand side arguments were all consumed ;
  # the remaining ones are for the "right-hand side" of the `find ...`
  # command.
	if [ "$arg" == "--" ]; then
    rightargs=( "$@" )
    break
	fi

	leftargs=( "${leftargs[@]}" $arg )
done

findcmd=(
  find
    "${leftargs[@]}"
        -regextype egrep
        \(
             -name '.?*'
          -o \( -type d \( -name 'CMakeFiles' \) \)
          -o \( -type f \( -name '*~' -o -iregex '.*\.(old|bak)$' \) \)
        \) -prune
        -o -type f
           \(
             -iregex "$regex"
             -o -name "CMakeLists.txt"
           \)
    "${rightargs[@]}"
)

"${findcmd[@]}" \
  | sed -e 's@^\./@@'

retv=$?
if [ $retv -gt 0 ]; then
  echo
  echo "| FAIL: exit status of \`find ...\` is non-zero ;"
  echo "|        \` command was :"
  echo "|"
  echo "|   ${findcmd[@]}"
fi

exit $retv
