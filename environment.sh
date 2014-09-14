# File: environment.sh
#
# FabiC.2014-09-14
#
# Usage:
# 	- source environment.sh
#   - sh environment.sh (for testing).

HERE=$( cd `dirname "${BASH_SOURCE[0]}"` && pwd )

llvm_build_dir="$HERE/build"
local_dir="$HERE/local"

echo "+---"
echo "| At $HERE"
echo "| LLVM build dir.: $llvm_build_dir"
echo "+-"

if [ -x "$local_dir/bin/clang" ]; then
	PATH="$local_dir/bin:$PATH"
elif [ -x "$llvm_build_dir/bin/clang" ]; then
	PATH="$llvm_build_dir/bin:$PATH"
elif [ -z "`which clang 2> /dev/null`" ]; then
	echo "| ERROR: Can't find clang binary."
	echo "+-"
	echo
	exit 127
fi

CC=clang
CXX=clang++

Env=( CC CXX PATH )

export ${Env[*]}

echo "| clang is: `which clang` [`clang --version | head -n1`]"

for e in ${Env[*]}; do
	echo "| $e = ${!e}"
done | column -t -s=


echo "+---"

