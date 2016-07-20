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

echo
echo "+---"
echo "| At $HERE"
echo "+-"

##
## Process arguments :
##
while [ $# -gt 0 ];
do
	arg="$1"
	shift

	dir="$arg"
	dir=${dir%/}

	#[ -d "$dir" ] && echo "Processing dir. '$dir'" || continue

	# Treat build*/ directories as LLVM/Clang out-of-source build dir. :
	if [ -z "${dir##build*}" ];
	then
		echo "| Setting up '$dir' as an out-of-source LLVM/Clang build directory."

		fhs_path_setup_for "$dir"

		# Additional include dirs :
		pathprepend "$HERE/clang/include"      CPATH
		pathprepend "$HERE/libcxx/include"     CPATH
		pathprepend "$HERE/libcxxabi/include"  CPATH
	else
		echo "| Setting up '$dir' as an FHS-like directory."
		fhs_path_setup_for "$dir"
	fi

	export PATH LD_LIBRARY_PATH LD_RUN_PATH LIBRARY_PATH CPATH CPLUS_INCLUDE_PATH
done


##
## Define $CC & $CXX explicitly if these aren't already set.
##

# Further below we do not overwritte CC/CXX if defined :
[ ! -z "$CC" ]  && echo "| NOTE: \$CC=\"$CC\" (already defined, not overwritting it)"
[ ! -z "$CXX" ] && echo "| NOTE: \$CXX=\"$CXX\" (already defined, not overwritting it)"

# Favor Clang :
if [ ! -z "`which clang 2> /dev/null`" ];
then
	CC=${CC:-clang}
	CXX=${CXX:-clang++}
# or fallback to GCC :
elif [ ! -z "`which gcc 2> /dev/null`" ];
then
	CC=${CC:-gcc}
	CXX=${CXX:-g++}
fi

export CC CXX


# Preprend our utility bin/ dir. to $PATH :
[ -d "$HERE/bin" ] && pathprepend "$HERE/bin"


#######################
## OUTPUT SOME INFO. ##
#######################

echo "| $CC is: `which $CC 2> /dev/null` [`$CC --version | head -n1`]"
echo "| $CXX is: `which $CXX 2> /dev/null` [`$CXX --version | head -n1`]"
echo "|"
echo "+-- Environment:"
checkenv
echo "+-"
echo

# EXIT
[ ${BASH_SOURCE[0]} == $0 ] && exit || return








# Not to be export-ed env. var. :
EnvNonExported=( LD_LIBRARY_PATH LIBRARY_PATH CPATH CPLUS_INCLUDE_PATH )

# These ones we are export-ing :
Env=( CC CXX PATH LD_RUN_PATH )

export ${Env[*]}

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

