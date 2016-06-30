#!/bin/sh
#

echo "+--- $0 $@"
echo "| In '`pwd`'"
echo "|"

find \( -type d \( -name CMakeFiles -o -name .git -o -name .svn \) -prune \) \
    -o -type f -perm -a+x \
    "$@" \
    -print0 | \
        xargs -0r ls -ltrh | \
            sed -e 's@^@|  &@'
