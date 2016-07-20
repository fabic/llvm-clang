#!/bin/bash
#
# fabic/2016-07-09

here=$(cd `dirname "$0"` && pwd)

. "$here/functions.sh"


libffi_version="3.2.1"


echo "+--- $0"
echo "|"
echo "| This script downloads, builds and install libffi-$libffi_version"
echo "|"
echo "| (FFI : Foreign Function Interface)."
echo "|"
echo "| - https://sourceware.org/libffi/"
echo "| - http://www.linuxfromscratch.org/blfs/view/stable/general/libffi.html"
echo "|"
echo "| NB: libffi-3.2.1 was released on November 12, 2014"
echo "+-"

cat <<EOS | sed -e 's/^/|   /'
Quoted from https://sourceware.org/libffi/ :
--
  What is libffi ?
  ----------------

  Compilers for high level languages generate code that follows certain
  conventions. These conventions are necessary, in part, for separate
  compilation to work. One such convention is the "calling convention". The
  "calling convention" is a set of assumptions made by the compiler about where
  function arguments will be found on entry to a function. A "calling
  convention" also specifies where the return value for a function is found.
  Some programs may not know at the time of compilation what arguments are to be
  passed to a function. For instance, an interpreter may be told at run-time
  about the number and types of arguments used to call a given function. Libffi
  can be used in such programs to provide a bridge from the interpreter program
  to compiled code.

  The libffi library provides a portable, high level programming interface to
  various calling conventions. This allows a programmer to call any function
  specified by a call interface description at run-time.

  FFI stands for Foreign Function Interface. A foreign function interface is the
  popular name for the interface that allows code written in one language to
  call code written in another language. The libffi library really only provides
  the lowest, machine dependent layer of a fully featured foreign function
  interface. A layer must exist above libffi that handles type conversions for
  values passed between the two languages.
--
EOS

echo "+-"


  cd "$here" || exit 1

  localdir=${1:-"$(mkdir -p "$here/local" && cd "$here/local" && pwd)"}
  builddir=${2:-"BUILD"}
  sources_dir="${3:-misc/libffi-$libffi_version}"

  # Ensure destination "install target" directory is an absolute path :
  if [ "X${localdir#/}" == "X${localdir}" ]; then
    echo "| First argument '$localdir' ain't an absolute path, "
    echo "| prefixing with current directory :"
      localdir="$(pwd)/$localdir"
    echo "|"
    echo "|   \$localdir = $localdir"
    echo "|"
  fi


# Download & extract tarball if sources not found :
if [ ! -d "misc/libffi-$libffi_version" ];
then

  # Download tarball under tmp/ if not exists :
  if [ ! -f "tmp/libffi-$libffi_version.tar.gz" ] &&
    ! wget ftp://sourceware.org/pub/libffi/libffi-$libffi_version.tar.gz \
      -O tmp/libffi-$libffi_version.tar.gz ;
  then
    echo "OUPS!"
    exit 1
  fi

  # Extract tarbal content under misc/
  if ! tar -xf tmp/libffi-$libffi_version.tar.gz -C misc/ ;
  then
    echo "GOOUPS!"
    exit 2
  fi

  # Check sources misc/libffi-x.y.z/ exists.
  if [ ! -d "$sources_dir" ]; then
    echo "ARGH!"
    exit 3
  fi
fi


configure_log_filename="_configure-command-output.log.txt"

# Arguments for ./configure...
configure_args=(
        --prefix="$localdir"
        --disable-static
  )


echo "|"
echo "| We'll be building libffi with the following ./configure arguments :"
echo "|"
echo "| ${configure_args[@]}"
echo "|"


cd "$sources_dir" || exit 1


if true; then
  echo "+-"
  echo "| Patching Makefile.in & libffi.pc.in"
  echo "| (as per the BLFS book)"

  sed -e '/^includesdir/ s/$(libdir).*$/$(includedir)/' \
      -i include/Makefile.in

  sed -e '/^includedir/ s/=.*$/=@includedir@/' \
      -e 's/^Cflags: -I${includedir}/Cflags:/' \
      -i libffi.pc.in

  echo "+-"
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

  echo "| Creating & entering the temporary build sub-directory '$builddir'."

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
