#!/bin/bash
#
# fabic/2016-06-30

here=$(cd `dirname "$0"`/.. && pwd)

. "$here/functions.sh"


# We _do want_ to build this one thing with GCC.
CC=gcc
CXX=g++


echo "+--- $0"
echo "|"
echo "| This script builds and installs GNU Binutils."
echo "|"
echo "| Official Git repository :"
echo "|   https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git"
echo "|"
echo "| References :"
echo "|"
echo "|  - http://www.linuxfromscratch.org/lfs/view/development/chapter06/binutils.html"
echo "|  - http://llvm.org/docs/GoldPlugin.html"


  cd "$here" || exit 1

  localdir=${1:-"$(mkdir -p "$here/local" && cd "$here/local" && pwd)"}
  builddir=${2:-"build"}
  binutils_dir=${3:-misc/binutils}

  # Ensure destination "install target" directory is an absolute path :
  if [ "X${localdir#/}" == "X${localdir}" ]; then
    echo "| First argument '$localdir' ain't an absolute path, "
    echo "| prefixing with current directory :"
      localdir="$(pwd)/$localdir"
    echo "|"
    echo "|   \$localdir = $localdir"
    echo "|"
  fi


configure_log_filename="_configure-command-output.log.txt"

# Arguments for ./configure...
configure_args=(
        --prefix="$localdir" \
        --enable-shared      \
        --disable-werror     \
        --enable-gold        \
        --enable-ld=default  \
        --enable-plugins     \
  )

        #--with-system-zlib   \
echo "|"
echo "| We'll be building Binutils with the following ./configure arguments :"
echo "|"
echo "| ${configure_args[@]}"
echo "|"
echo "| NOTE that we did set here \$CC=$CC and \$CXX=$CXX to GNU/GCC."
echo "|"
echo "| NOTE that we'll _only_ be building the “ gold linker ”"
echo "|      ( make targets: all-gold install-gold )"


# Git check-out submodule if needed.
if [ -d "$binutils_dir" ] && [ -e "$binutils_dir/.git" ] && [ ! -f "$binutils_dir/configure" ];
then
  echo "|"
  echo "| Git submodule checkout misc/binutils/ :"

    if ! git submodule update --init misc/binutils ;
    then
      retv=$?
      echo "| FAIL: Git submodule exited with status : $retv"
      echo "+-"
      exit $retv
    fi

  echo "| Done with Git submodule checkout, ok."
  echo "+-"
else
  echo "| \$binutils_dir = '$binutils_dir' != 'misc/binutils' ==> assuming it is not a git checked-out source tree."
fi

cd "$binutils_dir" || exit 1

echo "| Entered `pwd`/"
echo "|"
echo "|   \$localdir = '$localdir'"
echo "|   \$builddir = '$builddir'"

  echo
  read -p "Ok to proceed ? (Ctrl-C to abort)"
  echo


  # Ask/warn about an eventually existing build/ directory.
  if [ -d "$builddir" ]; then
    read -p "| REMOVE existing temporary `pwd`/$builddir/ directory, Ok ?"
    rm -rf "$builddir" || exit 127
  fi


echo "| Entering the temporary build sub-directory '$builddir'."

if ! mkdir "$builddir" || ! cd "$builddir" ;
then
  echo "+---> FAILED to create and/or enter the temporary build dir.: '$builddir'"
  exit 126
else
  echo "| Ok, current dir. is now '`pwd`'"
fi

#
## CONFIGURE
#

echo "|"
echo "| About to run :"
echo "|"
echo "|   ./configure ${configure_args[@]}"
echo "|"
echo "| ( output will be collected into file '$configure_log_filename' )"
echo

  echo
  read -p "Ok to proceed ? (Ctrl-C to abort)"
  echo

  #echo \
  time \
    ../configure \
      "${configure_args[@]}" \
            |& tee "$configure_log_filename"

  retv=$?
  if [ $retv -ne 0 ]; then
    echo
    echo "| FAIL: ./configure exited with status : $retv"
    echo "|       ( ^ arguments were : ${configure_args[@]} )"
    echo "+-"
    exit $retv
  else
    echo
    echo "| Ok, ./configure completed successfully (quite possibly)."
  fi

#
## MAKE
#
# todo: regular 'all' target ?

max_jobs=`how_many_cpus 2`
max_sys_load=`max_load_level`
make_args=(
  tooldir="$localdir" \
  -j$max_jobs \
  -l$max_sys_load
  #all
  #all-gold
  )

echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
echo "| FYI: We'll be running \`make ${make_args[@]}\`"
echo "|"
echo "|      -j$max_jobs  : max. simultaneous jobs,"
echo "|      -l$max_sys_load  : max. system load level."
echo "|"
echo "| NOTE: “ all-gold ” target argument is from"
echo "|       http://llvm.org/docs/GoldPlugin.html"
echo "|"
echo "| NOTE: 'tooldir=...' argument is from the LFS book :"

cat << EOS | sed -e 's/^/|     /'
“ Normally, the tooldir (the directory where the executables will ultimately be located)
  is set to \$(exec_prefix)/\$(target_alias). For example, x86_64 machines would expand
  that to /usr/x86_64-unknown-linux-gnu. Because this is a custom system, this
  target-specific directory in /usr is not required. \$(exec_prefix)/\$(target_alias)
  would be used if the system was used to cross-compile (for example, compiling a
  package on an Intel machine that generates code that can be executed on PowerPC
  machines). ”
      _Linux From Scratch -- Chapter 6.13 -- Binutils-2.26_
      http://www.linuxfromscratch.org/lfs/view/development/chapter06/binutils.html
EOS


echo "+-"
echo

  read -p "Ok to proceed ? (Ctrl-C to abort)"
  echo


  #echo \
  time \
    make "${make_args[@]}"

  retv=$?
  if [ $retv -ne 0 ]; then
    echo
    echo "+ ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !"
    echo "|"
    echo "| FAIL: make exited with status : $retv"
    echo "|       ( ^ arguments were : ${make_args[@]} )"
    echo "|"
    echo "+-- $0 $@ ~~~> FAILED."
    exit $retv
  else
    echo
    echo "| Ok, \`make ${make_args[@]}\` completed successfully."
  fi


#
## MAKE INSTALL
#

echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
echo "|"
echo "| Running \`make tooldir=\"$localdir\" install\`"
echo "|"
echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
echo


  read -p "Ok to proceed ? (Ctrl-C to abort)"
  echo


  make \
    tooldir="$localdir" \
      install

      #todo: install target ?


  retv=$?
  if [ $retv -ne 0 ]; then
    echo
    echo "+ ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !"
    echo "|"
    echo "| FAIL: make install failed with exit status $retv"
    echo "|"
    echo "+-"
    echo
    exit $retv
  else
    echo
    echo "+"
    echo "|"
    echo "| make install done !"
    echo "|"
    echo "+-"
  fi


#
## MAKE TEST
#

if true;
then
  read -p "+-- RUN THE TEST SUITE ?"

  time \
    make -k check

  echo "| \`make -k chech\` completed with exit status $?"
else
  echo "|"
  echo "| NOTE: we're not running the test suite..."
  echo "|"
  echo "|       just do \`make -k check\` if you feel like so."
  echo "|"
fi

# List a few files we're interested in.
if true; then
  echo "+--"
  echo "|"
  ls -l "$localdir/include/plugin-api.h" "$localdir/bin/ld*"
fi

echo "|"
echo "+--- $0 $@ : FINISHED, exit status: $retv"

exit $retv
# vim: et sw=2 ts=2
