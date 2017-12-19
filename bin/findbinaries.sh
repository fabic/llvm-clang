#!/bin/sh
#

echo "+--- $0 $@"
echo "| In '`pwd`'"

# Note about command line arguments (if none provided).
if [ $# -lt 1 ]; then
  echo "+"
  echo "| Note that additional arguments may be provided, these would be passed"
  echo "| to the \`find ...\` command."
fi

find_cmd=(
  find \( -type d \( -name CMakeFiles -o -name '.?*' \) -prune \)
    -o \( -type f \( -perm -a+x -o -name '*.a' \) \)
    "$@"
)

# Build a string for printing out the `find ...` command we are about to
# run, with Bash special characters ()* escaped ;
# (so that one may copy-paste it and just work).
escaped_cmd_str="${find_cmd[@]}"
escaped_cmd_str="${escaped_cmd_str//\(/\\(}"
escaped_cmd_str="${escaped_cmd_str//\)/\\)}"
escaped_cmd_str="${escaped_cmd_str//\*/\\\*}"

echo "+"
echo "| Find command :"
echo "|"
echo "|   $escaped_cmd_str"
echo "+"

"${find_cmd[@]}" \
  -print0 | \
    xargs -0r ls -ltrh | \
      sed -e 's@^@|  &@'

