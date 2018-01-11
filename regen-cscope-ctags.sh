#!/bin/bash
#
# FABIC/2017-12-19

here="$( cd `dirname "$0"` && pwd )"

srcdirs=( . )

if [ -d "$here/../llvm-clang/llvm-clang/clang" ]; then
  echo "~~> Found LLVM & Clang impl. sources."
  srcdirs=( ${srcdirs[@]} $here/../llvm-clang/llvm-clang/{llvm,clang}/{include,lib} )
else
  echo "~~> LLVM/Clang sources not found, will scan /usr/include/{llvm,clang}/"
  srcdirs=( ${srcdirs[@]} /usr/include/{llvm,clang} )
fi

cscope_sh="$( type -p run-cscope.sh )"
ctags_sh="$( type -p run-ctags.sh )"

# TEMP. FIXME.
if [ -z "$cscope_sh" ] && [ -z "$ctags_sh" ]; then
  if [ -d "$here/../llvm-clang/bin" ]; then
    cscope_sh="$here/../llvm-clang/bin/run-cscope.sh"
    ctags_sh="$here/../llvm-clang/bin/run-ctags.sh"
  fi
fi

# CSCOPE
if [ ! -x "$cscope_sh" ]; then
  echo "| WARNING: run-cscope.sh shell script wasn't found -_-"
else
  cmd=( $cscope_sh ${srcdirs[@]} )
  echo "+- ${cmd[@]}"
  time \
    ${cmd[@]}
fi

# CTAGS
if [ ! -x "$ctags_sh" ]; then
  echo "| WARNING: run-cscope.sh shell script wasn't found -_-"
else
  cmd=( $ctags_sh ${srcdirs[@]} )
  echo "+- ${cmd[@]}"
  time \
    ${cmd[@]}
fi
