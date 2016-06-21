#!/bin/sh
#
# FabiC/2016-06-21
#
# @link https://www.chromium.org/developers/sublime-text#TOC-Preferences

time \
  ctags --languages=C,C++,Asm -R -f .tags \
    --exclude=build --exclude=.git --exclude=.svn \
    --exclude=tmp --exclude=out

echo
echo ".tags file :"
echo

ls -l .tags

