# F.2011-08-16 : From LFS (http://www.linuxfromscratch.org/blfs/view/stable/postlfs/profile.html)
# F.2014-09-16 : Replaced 'export' with 'declare -g' for I don't want automatic exporting.

# Functions to help us manage paths.  Second argument is the name of the
# path variable to be modified (default: PATH)
pathremove () {
        local IFS=':'
        local NEWPATH
        local DIR
        local PATHVARIABLE=${2:-PATH}
        for DIR in ${!PATHVARIABLE} ; do
                if [ "$DIR" != "$1" ] ; then
                  NEWPATH=${NEWPATH:+$NEWPATH:}$DIR
                fi
        done
        declare -g $PATHVARIABLE="$NEWPATH"
}

pathprepend () {
        pathremove $1 $2
        local PATHVARIABLE=${2:-PATH}
        declare -g $PATHVARIABLE="$1${!PATHVARIABLE:+:${!PATHVARIABLE}}"
}

pathappend () {
        pathremove $1 $2
        local PATHVARIABLE=${2:-PATH}
        declare -g $PATHVARIABLE="${!PATHVARIABLE:+${!PATHVARIABLE}:}$1"
}

# @author FabiC
# @since 2014-09-16
pathpop() {
	local IFS=':'
	local PATHVARIABLE=${2:-PATH}
	local -a DIRS=( ${!PATHVARIABLE} )
	local REM=${DIRS[0]}
	unset DIRS[0]
	declare -g $PATHVARIABLE="${DIRS[*]}"
	echo "Removed \`$REM' from $PATHVARIABLE"
	echo "$PATHVARIABLE=${!PATHVARIABLE}"
}

## ^ End of LFS-imported stuff.
###

function fhs_path_setup_for() {
	if [ $# -eq 0 ]; then
		echo
		echo "Setup PATH, LD_LIBRARY_PATH / LD_RUN_PATH, LIBRARY_PATH, CPATH for FHS-like directories (which have e.g. bin/, include/, lib/)."
		echo
		echo "Usage: $0 <usr-local-like-dir1/> [<dir2/> ...]"
		echo
		return
	fi

	local fhs_dir

	while [ $# -gt 0 ];
	do
		fhs_dir="$1"
		shift

		[ -d "$fhs_dir" ] || echo "WARNING: $0: directory '$fhs_dir' doesn't exist."

		# bin/
		#[ -d "$fhs_dir/bin" ] &&
			pathprepend "$fhs_dir/bin"

		# lib/
		#if [ -d "$fhs_dir/lib" ]; then
			pathprepend "$fhs_dir/lib" LD_RUN_PATH
			pathprepend "$fhs_dir/lib" LIBRARY_PATH
		#fi

		# lib64/
		if [ -d "$fhs_dir/lib64" ]; then
			pathprepend "$fhs_dir/lib64" LD_RUN_PATH
			pathprepend "$fhs_dir/lib64" LIBRARY_PATH
		fi

		# include/
		#[ -d "$fhs_dir/include" ] &&
			pathprepend "$fhs_dir/include" CPATH

		export PATH LD_RUN_PATH LIBRARY_PATH CPATH
		#echo $PATH $LD_RUN_PATH $LIBRARY_PATH $CPATH
	done
}

# Simple func. for displaying compilation/linkage env. vars :
checkenv() {

	env |
		grep -E '^((|C.*|(LD_)?(LIBRARY_|RUN_))PATH|CC|CXX)=' |
		column -t -s= |
		while read var value;
		do
			echo "| unset $var=$value"
		done |
		column -t -s=
	return

	# todo: remove that old thing...
	for e in CC CXX PATH LD_LIBRARY_PATH LD_RUN_PATH LIBRARY_PATH CPATH CPLUS_INCLUDE_PATH ;
	do
		[[ ${!e-} ]] && echo "unset $e = ${!e}" || true #echo "undefined $e"
	done
}

