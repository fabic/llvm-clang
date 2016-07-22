#!/bin/bash
#
# fabic/2016-07-09

rewt=$(cd `dirname "$0"`/.. && pwd)

. "$rewt/functions.sh"

cd "$rewt" || exit 1

echo "+--- $0"
echo "|"
echo "| This script builds and install Wayland."
echo "|"
echo "| Official Git repositories :"
echo "|   https://cgit.freedesktop.org/wayland/"
echo "|"
echo "| References :"
echo "|"
echo "|  - https://wayland.freedesktop.org/building.html            (howto)"
echo "|  - https://github.com/bryceharrington/wayland-build-tools/  (build scripts)"
echo "+-"


##
## CONFIGURE SOURCES ( AUTOTOOLS )
##
function autotools_based_sources_configure()
{
  local let retv=127
  local configure_log_filename="_configure-command-output.log.txt"

  # Arguments for ./configure...
  local -a configure_args=( "$@" )

  echo "+- autotools_based_sources_configure() $@"
  echo "|"
  echo "| Running configure with arguments :"
  echo "|"
  echo "|   ${configure_args[@]}"
  echo "|"

  local sources_dir=""

  if [ "$(basename `pwd`)" == "BUILD" ]; then
    sources_dir=".."
  else
    sources_dir="."
  fi

  echo "| We're in `pwd`/"
  echo "| Sources are at '$sources_dir'"

  # RUN ./autogen.sh if ./configure script ain't there.
  if [ ! -x "$sources_dir/configure" ];
  then
    echo "| '$sources_dir/configure' script not found, it's ok, we'll run the $sources_dir/autogen.sh script now..."
    echo

    srcdir=$sources_dir \
    NOCONFIGURE=1 \
      $sources_dir/autogen.sh

    retv=$?
    if [ $retv -ne 0 ]; then
      echo
      echo "| FAIL: $sources_dir/autogen.sh exited with status : $retv"
      echo "+- $0 : failed -_-"
      return $retv
    else
      echo
      echo "| Ok, $sources_dir/autogen.sh completed successfully."
    fi
  fi

  # MAKE DISTCLEAN ?
  if false; then
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
      return $retv
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
  echo "|   $sources_dir/configure ${configure_args[@]}"
  echo "|"
  echo "| ( output will be collected into file '$configure_log_filename' )"
  echo

  echo
  read -p "Ok to proceed ? (Ctrl-C to abort)"
  echo

    #echo \
    time \
      $sources_dir/configure \
        "${configure_args[@]}" \
              |& tee "$configure_log_filename"

    retv=$?
    if [ $retv -ne 0 ]; then
      echo
      echo "| FAIL: $sources_dir/configure exited with status : $retv"
      echo "|       ( ^ arguments were : ${configure_args[@]} )"
      echo "+-- $0() $@  //  FAIL."
      return $retv
    fi

    return $retv
}
# ^ end of func. autotools_based_sources_configure().
##


##
##
##
function autotools_based_sources_run_make()
{
  local max_jobs=`how_many_cpus 2`
  local max_sys_load=`max_load_level`
  local make_args=(
      -j$max_jobs
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
      return $retv
    else
      echo
      echo "| Make completed successfully."
      echo "+- $0() $@"
    fi

    return $retv
}
# ^ end of function autotools_based_sources_run_make().
##


##
## MAKE INSTALL ( autotools-based )
##
function autotools_based_sources_run_make_install()
{
  local let retv=127

  echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
  echo "|"
  echo "| Running \`make install\`..."
  echo "|"
  echo "+ ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~"
  echo

  make install

  retv=$?

  if [ $retv -ne 0 ];
  then
    echo
    echo "+ ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !"
    echo "|"
    echo "| FAIL: make install failed with exit status $retv"
    echo "|"
    echo "+- $0() $@  //  FAIL  //"
    echo
    return $retv
  else
    echo
    echo "+"
    echo "|"
    echo "| make install done !"
    echo "|"
    echo "+- $0() $@"
  fi

  return $retv
} #< end of function.


##
## CONFIGURE, MAKE, MAKE INSTALL
##
function autotools_based_sources_build_n_install()
{
  local sources_dir="$1"
  local localdir="$2"
  local builddir="$3"
  shift 3
  # ^ remainder arguments $@ will be for configure.

  local let retv=127

  if [ ! -d "$sources_dir" ]; then
    echo "| FAIL: no such directory \$sources_dir = '$sources_dir'"
    return 1
  fi

  # Ensure destination "install target" ( local/ )
  # directory is an absolute path :
  if [ "X${localdir#/}" == "X${localdir}" ]; then
    echo "| 2nd argument \$localdir = '$localdir' ain't an absolute path, "
    echo "| prefixing with current directory :"
      localdir="`pwd`/$localdir"
    echo "|"
    echo "|   \$localdir = $localdir"
    echo "|"
    read -p "|    ^ ¿ target install directory is ok for you ?"
  fi

  # Enter sources directory.
  if ! cd "$sources_dir" ; then
    echo "| FAIL: could not enter directory \$sources_dir = '$sources_dir'"
    return 2
  else
    echo "| Entered $sources_dir (sources directory)."
  fi

  # Ask/warn about an eventually existing build/ directory.
  if [ -d "$builddir" ]; then
    read -p "| REMOVE existing temporary `pwd`/$builddir/ directory, Ok ?"
    rm -rf "$builddir" || exit 127
  fi

  echo "| Creating & entering the temporary build sub-directory '$builddir'."

  if ! mkdir "$builddir" || ! cd "$builddir" ;
  then
    echo "+---> FAILED to create and/or enter the temporary build dir.: '$builddir'"
    return 3
  else
    echo "| Ok, current dir. is now '`pwd`'"
  fi

  autotools_based_sources_configure --prefix="$localdir" "$@" &&
  autotools_based_sources_run_make &&
  autotools_based_sources_run_make_install

  retv=$?

  return $retv
}
# ^ end of function autotools_based_sources_build_n_install().
##


function auto_build_thing()
{
  local answer="nil"

  echo "+-- $0() $@"
  echo "|"

  while read -p "| Proceed ? (y/n)" answer ;
  do
    case "$answer" in
      yes|y|Y)
        break
        ;;
      no|n|N)
        return 0
        ;;
      *)
        ;;
    esac
  done

  pushd .
    autotools_based_sources_build_n_install "$@"
    retv=$?
  popd

  return $retv
}


auto_build_thing misc/wayland/wayland           local BUILD --disable-documentation

auto_build_thing misc/wayland/wayland-protocols local BUILD --disable-documentation

auto_build_thing misc/wayland/mesa local BUILD \
  --enable-gles2                               \
  --with-egl-platforms=x11,wayland,drm         \
  --enable-gbm                                 \
  --enable-shared-glapi                        \
  --with-gallium-drivers=r300,r600,swrast,nouveau

echo "|"
echo "+--- $0 $@ : FINISHED, exit status: $retv"

exit $retv
# vim: et sw=2 ts=2
