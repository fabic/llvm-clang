#!/bin/sh
#
# See @link http://clang.llvm.org/docs/IntroductionToTheClangAST.html

exec clang \
	-Xclang -ast-dump -fsyntax-only \
	"$@"

