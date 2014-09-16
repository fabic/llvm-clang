#!/bin/sh

#clangxx=`which clang++`
#${clangxx:?"ERROR: $0: clang++ wasn't found !?"}

rpath=$( cd `dirname "$0"`/../lib && pwd )

[ -d "$rpath" ] || unset rpath

# Can't find my way around this one (headers didn't get installed) :
libcxxabi_include_path=$( cd `dirname "$0"`/../../libcxxabi/include && pwd )

exec clang++ -stdlib=libc++ \
	-lc++abi ${libcxxabi_include_path:+-I$libcxxabi_include_path} \
	${rpath:+-Wl,-rpath=$rpath} \
	"$@"

