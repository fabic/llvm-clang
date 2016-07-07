#!/bin/sh
#
# FabiC/2016-06-21
#
# @link https://www.chromium.org/developers/sublime-text#TOC-Preferences

args=(
    --languages=C,C++,Asm
    -R -f .tags
    --exclude=build --exclude=.git --exclude=.svn --exclude=tmp --exclude=out
    "$@"
  )

echo "+-- $0"
echo "|"
echo "| Running Ctags :"
echo "|"
echo "|   ctags ${args[@]}"
echo

time \
  ctags \
    "${args[@]}"

retv=$?

echo "|"
echo "| Creating .tags_sorted_by_file (for Sublime Text's CTags package)."
echo "| ( see https://github.com/SublimeText/CTags/blob/development/ctags.py#L330 )"

time \
  sort -t\t -k2,1 .tags > .tags_sorted_by_file

echo
echo "| Ctags exited with status: $retv"
echo "|"
echo "| .tags file :"
echo "|"

ls -lh .tags* | sed -e 's/^/|  &/'

echo "+-- $0 : END."

exit $retv
