# File: environment.sh
#
# FabiC.2014-09-14
#
# Usage:
#
# 	- source environment.sh
#
#   - sh environment.sh (for testing).
#

HERE=$( cd `dirname "${BASH_SOURCE[0]}"` && pwd )

. $HERE/functions.sh

llvm_build_dir="$HERE/build"
bootstrap_dir="$HERE/bootstrap"
local_dir="$HERE/local"

echo "+---"
echo "| At $HERE"
echo "+-"

# Prepend our bootstrap/bin/ to search path if we can't find a clang
# binary under local/bin :
if [ -x "$bootstrap_dir/bin/clang" -a ! -x "$local_dir/bin/clang" ];
then
	pathprepend "$bootstrap_dir/bin"
	pathprepend "$bootstrap_dir/lib" LD_LIBRARY_PATH

# Prefer our local/ stuff :
elif [ -x "$local_dir/bin/clang" ];
then
	pathprepend "$local_dir/bin"
	pathprepend "$local_dir/lib" LD_RUN_PATH
	pathprepend "$local_dir/lib" LD_LIBRARY_PATH

	# according to ``man gcc` :
	pathprepend "$local_dir/lib" LIBRARY_PATH
	pathprepend "$local_dir/include" CPATH

	# this one isn't needed as its auto/hard-coded into Clang :
	#pathprepend "$local_dir/include/c++/v1" CPLUS_INCLUDE_PATH

# over an eventual fallback to build/bin :
elif [ -x "$llvm_build_dir/bin/clang" ];
then
	pathprepend "$llvm_build_dir/bin"
fi

# further below we do not overwritte CC/CXX if defined :
[ ! -z "$CC" ]  && echo "| NOTE: \$CC=\"$CC\" (we're not overwritting)"
[ ! -z "$CXX" ] && echo "| NOTE: \$CXX=\"$CXX\" (we're not overwritting)"

# Fallback to the host provided Clang or Gcc :
if [ ! -z "`which clang 2> /dev/null`" ];
then
	CC=${CC:-clang}
	CXX=${CXX:-clang++}
elif [ ! -z "`which gcc 2> /dev/null`" ];
then
	CC=${CC:-gcc}
	CXX=${CXX:-g++}
fi

# Not to be export-ed env. var. :
EnvNonExported=( LD_LIBRARY_PATH LIBRARY_PATH CPATH CPLUS_INCLUDE_PATH )

# These ones we are export-ing :
Env=( CC CXX PATH LD_RUN_PATH )

export ${Env[*]}

#######################
## OUTPUT SOME INFO. ##
#######################

echo "| $CC is: `which $CC 2> /dev/null` [`$CC --version | head -n1`]"
echo "| $CXX is: `which $CXX 2> /dev/null` [`$CXX --version | head -n1`]"
echo "|"


if true; then
	echo "+- Some of the defined (exported) environment :"
	for e in ${Env[*]}; do
		#[ -z "${e%*PATH}" ] && echo "| pathremove ${!e} $e" || echo "| unset $e = ${!e}"
		echo "| $e = ${!e}"
	done | column -t -s=
	echo "|"
fi

if true; then
	echo "+- Environment variables of interest which are *defined* but that where NOT export-ed here :"
	for e in ${EnvNonExported[*]}; do
		echo "| export $e = ${!e}"
	done | column -t -s=
	echo "|"
fi

if false; then
	echo "+- Defined shell functions :"
	echo "| " $( declare -F | awk '{ print $3 }' )
fi
echo "+---"

