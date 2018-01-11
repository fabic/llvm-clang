#!/bin/sh
#
# Open vim/gvim with source files in current subtree.

files=( `find "$@" -type d -name build -prune \
  -o -type f \( -name \*.h -o -name \*.cpp \) -print | sort` )

echo "+- $0 $@"
echo "| Source files :"
echo "|   ${files[@]}"
echo

if [ -z "$DISPLAY" ]; then
  vim "${files[@]}"
else
  gvim "${files[@]}"
fi

echo "+- $0"
