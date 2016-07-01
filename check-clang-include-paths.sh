#!/bin/sh
#
# FabiC/2016-06-20 -- https://github.com/fabic/llvm-clang

self="${BASH_SOURCE[0]}"

echo "+- $self $@"
echo "|"
echo "| This shell script basically invokes Clang++ as :"
echo "|"
echo "|     clang++ -Wp,-v -x c++ - -fsyntax-only < /dev/null"
echo "|"
echo "| FYI: Note that the following command would give you more verbose details :"
echo "|"
echo "|     clang++ -E -x c++ - -v < /dev/null"
echo "|"
echo "| Additionnal command line arguments are passed to Clang++, ex. :"
echo "|"
echo "|     $self -nostdinc -nostdinc++"
echo "|     $self -stdlib=libc++"
echo "|     $self -stdlib=libstdc++"
echo "|"
echo "+- Output :"
echo "|"

clang++ -Wp,-v -x c++ - -fsyntax-only "$@" < /dev/null |&
  sed -e 's/^/|  /'

retv=$?

echo "|"
echo "+- $self $@"
