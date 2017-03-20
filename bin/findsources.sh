#!/bin/bash
# FC.2014-09-24 fabic.net

if [ $# -lt 1 ]; then
	echo
	echo "Find source code files"
	echo "Usage: $0 [<prefix_find_arguments>... --] <suffix_find_arguments>..."
	echo "Example: $0 clang/include/ clang/lib llvm/include -- -name '*.c'"
	echo
	exit 127
fi

args=()
leftargs=()

# Splice left and right hand arguments around '--' :
while [ $# -gt 0 ];
do
	arg="$1"
	shift
	if [ "$arg" == "--" ]; then
		leftargs=( "${args[@]}" )
		args=()
		continue
	fi
	args=( "${args[@]}" $arg )
done

# Search sources files and grep :
find "${leftargs[@]}" \
	\( -type f -iregex '.+\.\(c\|h\|hh\|cpp\|hpp\|cxx\|hxx\|h\.inc\|s\)$' \) \
	"${args[@]}"

exit $?
