#!/usr/bin/env bash

here=$(cd `dirname "$0"` && pwd)

cd "$here" || exit 1

builddir="build"

[ ! -z "$builddir" ] || exit 2

# Remove build dir.
if [ -d "$builddir" ];
then
  echo "| REMOVING build dir. '$builddir'"
  rm -rf "$builddir" || exit 3
fi

# Try ./build.sh
if [ ! -x build.sh ];
then
  echo "| WARNING: Found no ./build.sh shell script \$here at '$here' -_-"
  echo "| TRYING with your other \`\` simple-build-cmake-project.sh \`\` generic script."

  if type -p simple-build-cmake-project.sh > /dev/null ;
  then
    echo "| FOUND IT ! ( `type -p simple-build-cmake-project.sh` )"
    echo "+-"
    echo

    simple-build-cmake-project.sh "$@"
    retv=$?
  else
    echo "+~~~> FOUND NO WAY TO BUILD YOUR STUFF, EXITING..."
    retv=$?
  fi
else
  echo "| Found a ./build.sh script, running it !"

  ./build.sh "$@"
  retv=$?

  #exit $retv
fi

# Display exit code.
if [ $retv -gt 0 ];
then
  echo
  echo "+~> "$0 $@" : FAILED, exit status : $retv"
  echo
  exit $retv
fi

