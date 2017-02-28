#!/bin/bash
#
# fabic/2016-06-30

here=$(cd `dirname "$0"`/.. && pwd)

. "$here/functions.sh"


# We _do want_ to build GCC, with GCC.
CC=gcc
CXX=g++


echo "+--- $0"
echo "|"
echo "| This script builds and installs GNU GCC."
echo "|"
echo "| Official Git repository :"
echo "|   https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git"
echo "|"
echo "| References :"
echo "|"
echo "|  - http://www.linuxfromscratch.org/lfs/view/development/chapter06/binutils.html"


  cd "$here" || exit 1

  localdir=${1:-"$(mkdir -p "$here/local" && cd "$here/local" && pwd)"}
  builddir=${2:-"build"}
  sources_dir=${3:-misc/gcc}

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
        --prefix="$localdir"     \
        --enable-languages=c,c++ \
        --disable-multilib       \
        --disable-bootstrap      \
        --with-system-zlib
  )

echo "|"
echo "| We'll be building GCC with the following ./configure arguments :"
echo "|"
echo "| ${configure_args[@]}"
echo "|"
echo "| NOTE that we did set \$CC=$CC and \$CXX=$CXX to GNU/GCC."
echo "|"
echo "| NOTE: Passing \`--disable-bootstap\` so that it won't perform multiple build passes of itself ;"
echo "|"
  if [ "${CC//\//}" == "${CC}" ]; then
    echo "|  \$CC=$CC  is `type -p $CC`"
    echo "|  \$CXX=$CXX is `type -p $CXX`"
  fi
echo "+-"

# Git check-out submodule if needed.
if [ -d "$sources_dir" ] && [ -e "$sources_dir/.git" ] && [ ! -f "$sources_dir/configure" ];
then
  echo "|"
  echo "| Git submodule checkout misc/gcc/ :"

    if ! git submodule update --init misc/gcc ;
    then
      retv=$?
      echo "| FAIL: Git submodule exited with status : $retv"
      echo "+-"
      exit $retv
    fi

  echo "| Done with Git submodule checkout, ok."
  echo "+-"
fi


if ! cd "$sources_dir" ; then
  echo "| FAILED to ch.dir into '$sources_dir'."
  exit 111
else
  echo "| Now in \$sources_dir = `pwd`."
fi


# Patch a conf. file for lib/ instead of lib64/ since we're not doing
# multi-arch. systems nowadays...
if true; then
  _conffile_to_patch=gcc/config/i386/t-linux64

  echo "+- Patching conf. file '$_conffile_to_patch' (as per the LFS book)"
  echo "| ( so that libraries are installed under lib/ instead of lib64/ )"

  case $(uname -m) in
    x86_64)
      sed -e '/m64=/s/lib64/lib/' -i.orig $_conffile_to_patch
      retv=$?
      ;;
  esac

  if [ $retv -ne 0 ]; then
    echo "|"
    echo "| OUCH! Sed exited with non-zero exit status $retv"
    echo "|       (while trying to patch file '$_conffile_to_patch')."
    echo "|"
    read -p "+~~> CONTINUE ANYWAY ? "
  fi

  unset _conffile_to_patch
fi


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
      "${configure_args[@]}"
            #|& tee "$configure_log_filename"
            # ^ FIXME: shadows configure exit status.

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
  -j$max_jobs \
  -l$max_sys_load
  )

echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
echo "| FYI: We'll be running \`make ${make_args[@]}\`"
echo "|"
echo "|      -j$max_jobs  : max. simultaneous jobs,"
echo "|      -l$max_sys_load  : max. system load level."
echo "|"
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
echo "| About to run \`make install\`"
echo "|"
echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
echo


  read -p "| ~~> Ok to proceed ? (Ctrl-C to abort) "
  echo


  make \
      install

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

if false;
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


echo "|"
echo "+--- $0 $@ : FINISHED, exit status: $retv"

exit $retv
# vim: et sw=2 ts=2
