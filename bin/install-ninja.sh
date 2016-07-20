#!/bin/bash
#
# fabic/2016-07-09

here=$(cd `dirname "$0"` && pwd)

. "$here/functions.sh"


build_command=(
  ../configure.py
  --bootstrap
  )


echo "+--- $0"
echo "|"
echo "| This script builds and install Ninja"
echo "|"
echo "| Official Git repository :"
echo "|   https://github.com/ninja-build/ninja/tree/master"
echo "|"


  cd "$here" || exit 1

  localdir=${1:-"$(mkdir -p "$here/local" && cd "$here/local" && pwd)"}
  builddir=${2:-"BUILD"}
  sources_dir=${3:-misc/ninja}

  # Ensure destination "install target" directory is an absolute path :
  if [ "X${localdir#/}" == "X${localdir}" ]; then
    echo "| First argument '$localdir' ain't an absolute path, "
    echo "| prefixing with current directory :"
      localdir="$(pwd)/$localdir"
    echo "|"
    echo "|   \$localdir = $localdir"
    echo "|"
  fi


if [ "x$sources_dir" == "xmisc/ninja" ];
then
  echo "|"
  echo "| Git submodule checkout misc/ninja/ :"

    if ! git submodule update --init misc/ninja ;
    then
      retv=$?
      echo "| FAIL: Git submodule exited with status : $retv"
      echo "+-"
      exit $retv
    fi

  echo "| Done with Git submodule checkout, ok."
  echo "+-"
else
  echo "| \$sources_dir = '$sources_dir' != 'misc/ninja' ==> assuming it is not a git checked-out source tree."
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


#
## MAKE
#

echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
echo "| Building it!"
echo "|"
echo "| \`${build_command[@]}\`"
echo "+-"
echo

  time \
    "${build_command[@]}"

  retv=$?
  if [ $retv -ne 0 ]; then
    echo
    echo "+ ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !"
    echo "|"
    echo "| FAIL: configure.py script exited with status : $retv"
    echo "|       ( ^ arguments were : ${make_args[@]} )"
    echo "|"
    echo "+-- $0 $@ ~~~> FAILED."
    exit $retv
  fi

  echo
  echo "| Ok, build complete."
  echo "+-"

  if [ ! -f ninja ]; then
    echo "|"
    echo "| Oups! the 'ninja' executable doesn't exist! (FAIL)"
    echo "+-- $0 $@ ~~~> FAILED."
    exit 1
  elif [ ! -x ninja ]; then
    echo "|"
    echo "| File 'ninja' exists but ain't an executable! (WTF?!)"
    exit 2
  fi

  bindir="$localdir/bin"

  echo "|"
  echo "| Copying ninja binary to '$bindir' :"
  cp -i ninja "$bindir/"

  # Display ninja binary info @ install location
  if [ -x "$bindir/ninja" ];
  then
    echo "+-"
    echo "| Ninja :"

      ls -l "$bindir/ninja" \
        | sed -e 's/^/|   &/'

    echo "+"
    echo "| \`ldd "$bindir/ninja\`" :"
    echo "|"

      ldd "$bindir/ninja" \
        | sed -e 's/^/|  &/'

    echo "|"
    echo "+"
    echo "| Checking that it is found first from your \$PATH :"
    echo "|"

    located=$(type -p ninja)

    echo "|"
    echo "| \`type -p ninja\` says '$located'"

    if [ "$bindir/ninja" != "$located" ];
    then
      echo "|"
      echo "| --> BEWARE! your \`ninja\` binary may _not_ be the one you want!"
      echo "|     Expected '$bindir/ninja'"
      echo "|     Got '$located'"
    else
      echo "| Good, your ninja is ready for use."
    fi

    echo "+-"
  else # ! -x ninja at intall. location
    echo "|"
    echo "| HUH! Couldn't find '$bindir/ninja' executable."
    echo "+ FAIL."
    exit 4
  fi

echo "|"
echo "+--- $0 $@ : FINISHED, exit status: $retv"

exit $retv
# vim: et sw=2 ts=2
