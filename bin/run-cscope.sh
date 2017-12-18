#!/bin/bash
#
# FABIC/2014

# List of dir. to search for source files.
locations=()

# Root of our LLVM/Clang playground.
rewt=$( cd `dirname "$0"`/.. && pwd )

# Current working dir.
here=$( pwd )

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
  echo "+ (!!)"
  echo "| Note that the current dir. '.' == '$here' was _NOT_ added to the search path."
  echo "| ( you may want to specify it as an additional argument (?)"
  echo "+-"
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
if [ ! -z "$BOOSTROOT" ] && [ -d "$BOOSTROOT/include" ];
then
  echo "+-"
  echo "| Got Boost C++ at \$BOOSTROOT = '$BOOSTROOT'"

  locations=( ${locations[@]} "$BOOSTROOT/include" )

  # Warn if not found.
  if [ ! -f "$BOOSTROOT/include/boost/config.hpp" ]; then
    echo "| !! WARNING !! Boost C++ config.hpp header wasn't found -_-"
  fi

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
      -o \( -type f -iregex '.+\.\(c\|h\|cc\|hh\|cpp\|hpp\|cxx\|hxx\|h\.inc\|hxx\.inc\|s\)$' \) \
        -print
  )

echo "+- Generating the cscope.files list :"
echo "|"
echo "|  find \"${find_args[@]}\" | sort > cscope.files"
echo "|"

find "${find_args[@]}" \
  | sort \
    > cscope.files

retv=$?
if [ $retv -ne 0 ];
then
  echo "| Some error occured while generating the cscope.files list :-|"
  echo "+~~> \`find ... \` command failed with exit status $retv"
  exit $retv
else
  echo "| ^ done."
fi


echo "+-"
echo "| Found $(wc -l cscope.files | awk '{print $1}' ) source files (cscope.files)."
echo "|   \` on-disk size: $(( $(cat cscope.files | xargs -r du -b | awk '{s+=$1} END {print s}') / 1024 )) kilo-bytes."
echo "|"
read -p "+~~~> Run CScope on these ? (Ctrl-C to abort) "

echo "+-"
echo "| Now running \`cscope -b -q\`"

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

