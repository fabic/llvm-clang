#!/bin/sh
#
# See @link http://clang.llvm.org/docs/IntroductionToTheClangAST.html


while [ $# -gt 0 ];
do
	[ -z "$1" ] && break
	header_file="$1"
	echo "$header_file"
	shift
done |

while read header_file ;
do
	echo "#include <$header_file>"
done |

clang \
	-Xclang -ast-dump -fsyntax-only \
	-std=c++11 \
	-x c++ \
	-

exit $?
