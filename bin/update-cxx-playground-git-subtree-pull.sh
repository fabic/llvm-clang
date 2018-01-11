#!/bin/bash
#
# FABIC 2018-01-11

rewt="$( cd `dirname "$0"`/.. && pwd )"

cd "$rewt" || exit 1

if [ ! -e playground ]; then
  echo "ERROR: Dude (!) can't find your playground/ sub-dir. (!)"
  exit 2
fi

git subtree pull -m "CXX PLAYGROUND -- PULL" \
                 --prefix=playground \
                 --squash git@github.com:fabic/cxx-playground.git master
retv=$?
if [ $retv -gt 0 ]; then
  echo "WARNING: the \`git subtree ...\` command exited with non-zero status code $retv"
else
  echo "DONE, OK"
fi

exit $retv
