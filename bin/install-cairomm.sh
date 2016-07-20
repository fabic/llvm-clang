#!/bin/bash
#
# fabic/2016-07-09

here=$(cd `dirname "$0"` && pwd)

. "$here/functions.sh"


echo "+--- $0"
echo "|"
echo "| This script builds and install Cairomm (C++ bindings/wrapper for Cairo)."
echo "|"
echo "| Official Git repository :"
echo "|   https://cgit.freedesktop.org/cairomm/"
echo "|"
echo "| References :"
echo "|"
echo "|  - http://linuxfromscratch.org/blfs/view/svn/x/cairomm.html"


  cd "$here" || exit 1

  localdir=${1:-"$(mkdir -p "$here/local" && cd "$here/local" && pwd)"}
  builddir=${2:-"BUILD"}
  sources_dir=${3:-misc/cairomm}

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
        --prefix="$localdir"
  )

# Boost C++ is optional
if [ ! -z "$BOOSTROOT" ];
then
  echo "|"
  echo "| Ok, \$BOOSTROOT = '$BOOSTROOT'."

  if [ -d "$BOOSTROOT/include" ]; then
    configure_args=(
        "${configure_args[@]}"
        --with-boost="$BOOSTROOT"
      )
  else
    echo "| Oups, couldn't find Boost C++ include/ directory, ignoring."
  fi
fi


echo "|"
echo "| We'll be building Cairomm with the following ./configure arguments :"
echo "|"
echo "| ${configure_args[@]}"
echo "|"


if [ "x$sources_dir" == "xmisc/cairomm" ];
then
  echo "|"
  echo "| Git submodule checkout misc/cairomm/ :"

    if ! git submodule update --init misc/cairomm ;
    then
      retv=$?
      echo "| FAIL: Git submodule exited with status : $retv"
      echo "+-"
      exit $retv
    fi

  echo "| Done with Git submodule checkout, ok."
  echo "+-"
else
  echo "| \$sources_dir = '$sources_dir' != 'misc/cairomm' ==> assuming it is not a git checked-out source tree."
fi

cd "$sources_dir" || exit 1

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

  echo "| Creating & entering the temporary build sub-directory '$builddir'."

  if ! mkdir "$builddir" || ! cd "$builddir" ;
  then
    echo "+---> FAILED to create and/or enter the temporary build dir.: '$builddir'"
    exit 126
  else
    echo "| Ok, current dir. is now '`pwd`'"
  fi


  # RUN ./autogen.sh if ./configure ain't there.
  if [ ! -f ../configure ];
  then
    echo "| ../configure script not found, it's ok, we'll run the ../autogen.sh script now..."
    echo

    srcdir=.. \
    NOCONFIGURE=1 \
      ../autogen.sh

    retv=$?
    if [ $retv -ne 0 ]; then
      echo
      echo "| FAIL: ../autogen.sh exited with status : $retv"
      echo "+- $0 : failed -_-"
      exit $retv
    else
      echo
      echo "| Ok, ../autogen.sh completed successfully."
    fi
  fi


  # MAKE DISTCLEAN
  if true; then
    echo "+-"
    echo "| Running \`make distclean\`"
    echo "| ( just in case, specifically because the autogen.sh script has"
    echo "|   quite probably run it on its own will on our behalf )"
    echo "|"
    echo

    make distclean

    if [ $retv -ne 0 ]; then
      echo
      echo "| Huh!: \`make distclean\` exited with status: $retv"
      echo "|       this is fine though, probably..."
      echo "+"
      exit $retv
    else
      echo
      echo "| \`make distclean\`, ok."
    fi
  fi


#
## CONFIGURE
#

echo "|"
echo "| About to run :"
echo "|"
echo "|   ../configure ${configure_args[@]}"
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
    echo
    read -p "--> ^ review the configuration report above, ok to proceed ? (Ctrl-C to abort)"
    echo
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

  read -p "--> Ok to proceed ? (Ctrl-C to abort)"
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
echo "| Running \`make install\`..."
echo "|"
echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
echo


  read -p "Ok to proceed ? (Ctrl-C to abort)"
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


echo "|"
echo "+--- $0 $@ : FINISHED, exit status: $retv"

exit $retv
# vim: et sw=2 ts=2
