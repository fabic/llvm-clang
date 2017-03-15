#!/bin/bash
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
if [ $retv -ne 0 ]; then
  echo "+~~~> ERROR: CTags exited with non-zero status: $retv"
  exit $retv
else
  echo "| ^ done."
fi

# Stupid check.
if [ ! -e .tags ]; then
  echo "| o_O\` Huh! .tags file not found, can't be !"
  exit 127
fi

## HARD-LINK
echo "+- Hard-linking .tags as tags (for Vim)"
ln .tags tags
ls -lh tags

echo "+-"
echo "| Creating .tags_sorted_by_file (for Sublime Text's CTags package)."
echo "| ( see https://github.com/SublimeText/CTags/blob/development/ctags.py#L330 )"

time \
  sort -t\t -k2,1 .tags > .tags_sorted_by_file

retv=$?

echo
echo "| \`sort ...\` exited with status: $retv"
echo "|"
echo "| .tags file(s) :"
echo "|"

ls -lh .tags* \
  | sed -e 's/^/|  &/'

echo "|"
echo "+-- $0 $@ : END."

exit $retv
