#!/bin/bash
#
# FABIC/2014

# List of dir. to search for source files.
locations=()

# Root of our LLVM/Clang playground.
rewt=$( cd `dirname "$0"`/.. && pwd )

# Current working dir.
here=$( pwd )

# Egrep compatible regular expression used for searching for source files.
# It _must_ be an Egrep-compatible expression for we are also grep-ing around
# in places.
#
# See `find -regextype egrep -iregex "$regex" ...`
# and `grep -Hni -E "$regex"
#
# Nonte: The previous find-specific reg. ex. used long ago was :
#        `find -iregex '.+\.\(c\|h\|cc\|hh\|cpp\|hpp\|cxx\|hxx\|h\.inc\|hxx\.inc\|s\)$' ...`
#
# A shell command for testing :
# ~$ R='.*\.(c|h|cc|hh|cpp|hpp|cxx|hxx|h\.inc|hxx\.inc|s)$' ; ( echo $R ; find /usr/include/ -regextype egrep -iregex "$R" | grep -E "$R" )
regex=".*\.(c|h|cc|hh|cpp|hpp|cxx|hxx|h\.inc|hxx\.inc|s)$"

# List of output files generated by Cscope.
cscope_out_files=( cscope.{files,in.out,out,po.out} )

# A list of system packages from which we ought to list files from, and filter
# out source files (typically headers under /usr/include).
syspackages=( )
# TODO: Have a command line switch for enabling this ?
#syspackages=( glibc libdrm libxcb xcb-util libxkbcommon{,-x11} wayland )

##
# If we're being run at the file system root '/' or at '/usr', then
# we want to drop a cscope database here (there -_-), and :
#
#   * Add header files from a few packages of interest to you, e.g. glibc.
if true && # TODO ??
  [ $PWD == '/' ] || [ $PWD == '/usr' ]; then
    # Note that you may have to create the cscope output files as root first
    # and adjust permissions accordingly, ex. :
    #
    #   sudo touch ${cscope_out_files[@]}
    #   sudo chown $USER ${cscope_out_files[@]}

    syspackages=( glibc libdrm libxcb xcb-util libxkbcommon{,-x11} wayland )
fi

echo "+-- $0 $@"
echo "|"
echo "| We're in $here"
echo "|"

echo "+-"
echo "| Note that you may obtain your compiler default #include search path with ex. :"
echo "|"
echo "|   LLVM / Clang: clang++ -Wp,-v -x c++ - -fsyntax-only < /dev/null"
echo "|   GNU / GCC:        g++ -Wp,-v -x c++ - -fsyntax-only < /dev/null"
echo "+-"

# Curr. dir. source tree follows conventions ?
[ -d "$here/include" ] && locations=( ${locations[@]} "$here/include" )
[ -d "$here/src"     ] && locations=( ${locations[@]} "$here/src"     )

# Arguments: locations to search for source files.
for dir in "$@"; do
  if [ -d "$dir" ]; then
    # "realpath ..."
    # Resolve dotted components from $dir path.
    # This will also have a '.' argument be resolve to the absolute pathname
    # of the current working dir.
    # And also resolve these compiler header search paths: '/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/7.2.1/../../../../include/c++/7.2.1'
    # to ex. '/usr/include/c++/7.2.1'
    dir="$( cd "$dir" && pwd )"
    locations=( ${locations[@]} "$dir" )
  else
    echo "| BEWARE: '$dir' is not a directory (or does not exist)."
  fi
done

# No search locations at this point => we're adding the current dir.
# in hope that it is a top-level dir. in some source tree.
if [ ${#locations[@]} -lt 1 ]; then
  echo "+- (!!)"
  echo "| \$locations[] is empty at this point ;"
  echo "|   => Adding current directory '$here' to it."
  echo "+-"
  locations=( "$here" )
else
  # Check that the current source dir. is in the list.
  found_here=0
  for loc in ${locations[@]}; do
    if [ "$loc" == "$here" ]; then
      found_here=1
    fi
  done
  # Inform user if it is not (which may be totally okay).
  if [ $found_here -eq 0 ]; then
    echo "+ (!!)"
    echo "| Note that the current dir. '.' == '$here' was _NOT_ added to the search path."
    echo "| ( you may want to specify it as an additional argument (?)"
    echo "+-"
  fi
  unset found_here
fi

# If equal => curr. dir. is not a sub-dir. of the
#             root of our LLVM/Clang playground.
if [ "${here#$rewt}" == "${here}" ]; then
  localdir=""
  llvmClang=""
  misc=""
  echo "+- FYI: We're not being run from within our LLVM/Clang playground."
  echo "|"
  echo "|       (ignoring the local/ & llvm-clang/{llvm,clang,...}/ includes)."
  echo "|"
else
  localdir="$(cd "$rewt/local" && pwd)"
  llvmClang="$(cd "$rewt/llvm-clang" && pwd)"
  misc="$(cd "$rewt/misc" && pwd)"

  echo "+- We're somewhere with the LLVM/Clang playground."
  echo "|"
  echo "| \$localdir  = $localdir"
  echo "| \$llvmClang = $llvmClang"
  echo "| \$misc      = $misc"
  echo "|"

  [ -d "$localdir/include"        ] && locations=( ${locations[@]} "$localdir/include" )

  #[ -d "$llvmClang/llvm/include"  ] && locations=( ${locations[@]} "$llvmClang/llvm/include" )
  #[ -d "$llvmClang/clang/include" ] && locations=( ${locations[@]} "$llvmClang/clang/include" )

  # ^ Includes shall have ended up installed into local/include/,
  #   so we'll just have to provide locations of the impl. :

  #[ -d "$llvmClang/llvm/lib"  ] && locations=( ${locations[@]} "$llvmClang/llvm/lib"  )
  #[ -d "$llvmClang/clang/lib" ] && locations=( ${locations[@]} "$llvmClang/clang/lib" )

  # ^ Replaced with the following – essentially checking for the existence of
  #   header files under local/include/{llvm,clang}/ , and if not found ⇒ fallback
  #   to the source tree include dirs.

  # LLVM
  if [ -d "$llvmClang/llvm/lib" ]; then
    echo "| Found LLVM."
    locations=( ${locations[@]} "$llvmClang/llvm/lib" )
    if [ ! -d "$localdir/include/llvm" ]; then
      locations=( ${locations[@]} "$llvmClang/llvm/include" )
    fi
  fi

  # Clang
  if [ -d "$llvmClang/clang/lib" ]; then
    echo "| Found Clang."
    locations=( ${locations[@]} "$llvmClang/clang/lib" )
    if [ ! -d "$localdir/include/clang" ]; then
      locations=( ${locations[@]} "$llvmClang/clang/include" )
    fi
  fi

  # libunwind
  if [ -f "$misc/libunwind/include/unwind.h" ]; then
    echo "| Found libunwind."
    locations=( ${locations[@]}
      "$misc/libunwind/include"
      "$misc/libunwind/src" )
  fi

  # Musl-libc
  if [ -d "$misc/musl-libc/include" ]; then
    echo "| Found Musl-libc."
    locations=( ${locations[@]}
      "$misc/musl-libc/include"
      "$misc/musl-libc/src" )
  fi
fi

# BOOST C++ !
# For ex. $BOOSTROOT may be '/usr' a system-wide installation,
# or '/usr/local', or '/home/dude/.local/boost-1.65.1-clang', etc.
if [ ! -z "$BOOSTROOT" ];
then
  echo "| Got Boost C++ at \$BOOSTROOT = '$BOOSTROOT'"
  boostroot=""
  # Ex. '/home/dude/.local/boost-1.63.0-gcc/include
  if [ -f "$BOOSTROOT/include/config.hpp" ]; then
    boostroot="$BOOSTROOT/include"
    # Ex. /usr/include/boost  (typically system-wide)
  elif [ -f "$BOOSTROOT/include/boost/config.hpp" ]; then
    boostroot="$BOOSTROOT/include/boost"
  # Ex. ~/tmp/boost_1_65_1/boost/  (extracted from tarball).
  elif [ -f "$BOOSTROOT/boost/config.hpp" ]; then
    boostroot="$BOOSTROOT/boost"
  fi

  echo "| Boost header files reside '$boostroot'."

  locations=( ${locations[@]} "$boostroot" )

  # Warn if not found.
  if [ ! -f "$BOOSTROOT/include/boost/config.hpp" ]; then
    echo "| !! WARNING !! Boost C++ config.hpp header wasn't found -_-"
  fi

  unset boostroot

  echo "|"
else
  echo "+- (!!) Boost C++ not found ??"
  echo "|"
  echo "| \$BOOSTROOT = '$BOOSTROOT'"
  echo "|"
  echo "| Either it is unspecified or it doesn't contain an include/ sub-directory"
  echo "+"
fi

echo "+- \$locations[] :"
echo "|"
for loc in "${locations[@]}"; do
  echo "|      - $loc"
done
echo "+"

find_args=(
    "${locations[@]}"
    -type d \( -name .git -o -name .svn \) -prune
      -o \( -type f -regextype egrep -iregex "$regex" \) \
        -print
  )

echo "+- Generating the cscope.files list :"
echo "|"
echo "|  find \"${find_args[@]}\" | sort > cscope.files"
echo "|"

find "${find_args[@]}" \
  | sort \
    > cscope.files
  # ^ Beware here: we're truncating the files list.

retv=$?
if [ $retv -ne 0 ];
then
  echo "| Some error occured while generating the cscope.files list :-|"
  echo "+~~> \`find ... \` command failed with exit status $retv"
  exit $retv
else
  echo "| ^ done."
fi

# ArchLinux: We list system header files using the `pacman` package
# manager.
if [ ${#syspackages[@]} -gt 0 ]; then
  echo "+- About to list source files from your host's package manager."
  echo "|"
  echo "| Packages names : ${syspackages[@]}"
  echo "|"

  if type -p pacman > /dev/null ; then
    echo "| Found ArchLinux's pacman package manager."
    ( pacman -Ql ${syspackages[@]} \
      | awk '{print $2}' \
      | grep -E "^(/usr/include|/usr/local/include)/$regex" \
    ) >> cscope.files
    # ^^ Caution: use '>>' so as not to have the file list truncated.
  fi
fi

# Sort the files list.
if true; then
  echo "+--> Sorting the cscope.files list."
  tempfile="`mktemp`"
  [ ! -e "$tempfile" ] && echo "| WARNING: Temporary file '$tempfile' does not exist (!)"
  [ -s "$tempfile" ] && echo "| WARNING: Temporary file '$tempfile' should be empty (!!)"
  if ! sort -u cscope.files > "$tempfile" ; then
    echo "| WARNING: A problem occurred while attempting to sort the cscope.files (temporary file was: $tempfile)."
    echo "|          (proceeding with the cscope.files as-is (unsorted)."
  else
    [ ! -s "$tempfile" ] && echo "| WARNING: Temporary file after sorting is empty !?"
    mv "$tempfile" cscope.files
  fi
fi

# Display some details about the files list
# (also computing the total on-disk size).
echo "+-"
echo "| Found $(wc -l cscope.files | awk '{print $1}' ) source files (cscope.files)."
size_bytes=$(cat cscope.files | xargs -r du -b | awk '{s+=$1} END {print s}')
echo "|   \` on-disk size: $(( $size_bytes / 1024 )) KB, ($(( $size_bytes / 1024 /1024 )) MB)."
echo "|"

# PAUSE
read -p "+~~~> Run CScope on these ? (Ctrl-C to abort) "

echo "+-"
echo "| Now running \`time cscope -b -q\`"

time \
  cscope -b -q

retv=$?
if [ $retv -ne 0 ];
then
  echo
  echo "| \`cscope ... \` command failed with exit status $retv"
  echo "|"
  echo "+~~ $0 $@ ~~~> ERROR -_- (exiting)"
  exit $retv
else
  echo "| ^ done with \`cscope -b -q\`"
fi


echo "+-"
echo "| Your cscope files :"
echo "|"

ls -lh cscope.* \
  | sed -e 's/^/|  &/'

echo "|"
echo "| We're done here, have fun."
echo "|"
echo "+-- $0 : END, exit status: $retv"

exit $retv

