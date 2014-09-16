# F.2011-08-16 : From LFS (http://www.linuxfromscratch.org/blfs/view/stable/postlfs/profile.html)

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
        export $PATHVARIABLE="$NEWPATH"
}

pathprepend () {
        pathremove $1 $2
        local PATHVARIABLE=${2:-PATH}
        export $PATHVARIABLE="$1${!PATHVARIABLE:+:${!PATHVARIABLE}}"
}

pathappend () {
        pathremove $1 $2
        local PATHVARIABLE=${2:-PATH}
        export $PATHVARIABLE="${!PATHVARIABLE:+${!PATHVARIABLE}:}$1"
}

## ^ End of LFS-imported stuff.
###

# Simple func. for displaying compilation/linkage env. vars :
checkenv() {
	for e in CC CXX PATH LD_LIBRARY_PATH LD_RUN_PATH LIBRARY_PATH CPATH CPLUS_INCLUDE_PATH ;
	do
		[[ ${!e-} ]] && echo "unset $e = ${!e}" || true #echo "undefined $e"
	done
}

