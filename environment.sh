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

# Prefer our local/ stuff :
if [ -x "$local_dir/bin/clang" ]; then
	PATH="$local_dir/bin:$PATH"
	# defining all of these, user will unset as needed :
	LD_LIBRARY_PATH="$local_dir/lib"
	LD_RUN_PATH="$local_dir/lib"
	LIBRARY_PATH="$local_dir/lib"
	CPATH="$local_dir/include"
	CPLUS_INCLUDE_PATH="$local_dir/include/c++/v1"
# over an eventual build/ :
elif [ -x "$llvm_build_dir/bin/clang" ]; then
	PATH="$llvm_build_dir/bin:$PATH"
fi

# Fallback to the host provided Clang or Gcc :
if [ ! -z "`which clang 2> /dev/null`" ]; then
	CC=${CC:-clang}
	CXX=${CXX:-clang++}
elif [ ! -z "`which gcc 2> /dev/null`" ]; then
	CC=${CC:-gcc}
	CXX=${CXX:-g++}
fi


Env=( CC CXX PATH LD_LIBRARY_PATH LD_RUN_PATH LIBRARY_PATH CPATH CPLUS_INCLUDE_PATH )

export ${Env[*]}

echo "| $CC is: `which $CC 2> /dev/null` [`$CC --version | head -n1`]"
echo "| $CXX is: `which $CXX 2> /dev/null` [`$CXX --version | head -n1`]"

if true; then
for e in ${Env[*]}; do
	echo "| $e = ${!e}"
done | column -t -s=
fi

echo "+---"

