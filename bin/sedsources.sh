#!/bin/sh
#
# FC.2016-07-27 fabic.net

# Usage note if no arguments :
if [ $# -lt 1 ]; then
  echo
  echo "Find source code files and grep into these..."
  echo
  echo "Usage: $0 <grep_arguments>... -- <find_arguments>..."
  echo
  echo "Example: $0 FrontendAction -- clang/include/ clang/lib llvm/include"
  echo
  exit 127
fi

# Left-side of `--' are for grep :
sedargs=()

# Right side arguments will be for find :
findargs=()

# Splice left and right hand arguments around '--' :
while [ $# -gt 0 ];
do
  arg="$1"
  shift

  [ "$arg" == "--" ] && break

  sedargs=( "${sedargs[@]}" "$arg" )
done

# Remainder arguments after '--' are for find :
findargs=( "$@" )

# Search sources files and grep :
findcmd=(
  find
    "${findargs[@]}"
       \( -type d -iregex '.+/\..+$' -prune \)
    -o -type f
       \(
         -iregex '.+.\(c\|h\|cpp\|hpp\|cxx\|hxx\|s\|S\|x\|cmake\)$'
         -o
         -name "CMakeLists.txt"
       \)
    -print0
)

echo "+-- $0 : BEGIN."
echo "|"
echo "| Running :"
echo "|"
echo "| ${findcmd[@]} \\"
echo "|   | xargs -0r sed ${sedargs[@]}"
echo

"${findcmd[@]}" |
  xargs -0r \
    sed "${sedargs[@]}"

retv=$?

echo
echo "+-- $0 : END, exit status: $retv"

exit $retv

