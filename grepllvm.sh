#!/bin/sh
# FC.2014-09-24 fabic.net

if [ $# -lt 1 ]; then
	echo
	echo "Find source code files and grep these"
	echo "Usage: $0 [<find_arguments>... --] <grep_arguments>..."
	echo "Example: $0 clang/include/ clang/lib llvm/include -- FrontendAction"
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
find "${leftargs[@]}" -type f -iregex '.+.\(c\|h\|cpp\|hpp\|cxx\|hxx\|txt\|md\|rst\|xml\|yml\|yaml\)$' -print0 |
	xargs -0 grep "${args[@]}"

exit $?
