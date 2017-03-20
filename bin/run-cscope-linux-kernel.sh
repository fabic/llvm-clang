#!/bin/bash
#
# @link http://cscope.sourceforge.net/large_projects.html
#
# FABIC/2015
#

# LIST DIRS OF INTEREST :
# find -type d \( -path ./Documentation -o -path ./drivers -o -path ./firmware -o -path ./scripts -o -path ./sound \) -prune -o \( -path './arch/*' \! -path ./arch/x86 -prune \) -o -type d -print

# FILES for cscope.files :
# find -type d \( -path ./Documentation -o -path ./drivers -o -path ./firmware -o -path ./scripts -o -path ./sound \) -prune -o \( -path './arch/*' \! -path ./arch/x86 -prune \) -o -type f -name "*.[chxsS]" > cscope.files

# SIMPLER :
# find arch/x86/ block/ crypto/ fs/ include/ init/ ipc/ kernel/ lib/ mm/ net/ethernet/ net/ipv4/ net/core/ net/dns_resolver/ net/netfilter/ net/packet/ net/unix/ tools/arch/x86/ -type f -name "*.[chxsS]" -print | sort > cscope.files

LNX=$( cd `dirname "$0"`/../misc/linux-kernel/ && pwd )

echo "+-- $0 $@"
echo "|"

cd "$LNX" \
  && echo "| Now in `pwd`" \
    || exit 127

[ ! -f include/linux/cpu.h ] &&
  echo "| WARNING: source tree may have no files." \
    && exit 126

echo "| Creating cscope.files (find ...)"

dirlist=(
  arch/x86
  include
  init kernel lib mm
  block
  #crypto fs ipc
  #net/{ethernet,ipv4,core,dns_resolver,netfilter,packet,unix}
  tools/arch/x86
)

excldirs=(
     -path arch/x86/crypto
  -o -path arch/x86/xen
  -o -path arch/x86/include/asm/xen
  -o -path include/crypto
  -o -path include/dt-bindings
  -o -path include/net
  -o -path include/pcmcia
  -o -path include/sound
  -o -path include/xen
)

echo "|"
echo "| Directories list :"
echo "|   ${dirlist[@]}"
echo "|"
echo "| Exclusions \`find ...\` expression :"
echo "|   ${excldirs[@]}"


# Have absolute path (fixme: wron't work w/ excluded dirs)
# (for better compatibility with editors)
if false; then
  dirlist=( "${dirlist[@]/#/$LNX/}"  )
  echo "| Using absolute paths :"
  echo "|   ${dirlist[@]}"
fi


#echo \
find "${dirlist[@]}" \
  -type d \( ${excldirs[@]} \) -prune \
	-o -type f -name "*.[chxsS]" \
    -print | sort > cscope.files

#find "$LNX"/{include,arch/x86,block,crypto,fs,init,ipc,kernel,lib,mm,net/{ethernet,ipv4,core,dns_resolver,netfilter,packet,unix},tools/arch/x86}  -type f -name "*.[chxsS]"  -print | sort > cscope.files

echo "| File cscope.files contains  `wc -l cscope.files` entries."
echo
read -p "+~~~> continue ? "
echo

echo "| Running cscope -b -q -k"

cscope -b -q -k
retv=$?

echo "| Cscope exit status: $retv"
echo "| Cscope files :"

ls -lh cscope*

echo "+-- $0 $@ : FINISHED."

exit $retv




# Note: using absolute paths in cscope.files for better compatibility (with editors).

find "$LNX"                                                                   \
	-path "$LNX/arch/*" ! -path "$LNX/arch/x86*" -prune -o                \
	-path "$LNX/include/asm-*" ! -path "$LNX/include/asm-i386*" -prune -o \
	-path "$LNX/tmp*" -prune -o                                           \
	-path "$LNX/Documentation*" -prune -o                                 \
	-path "$LNX/scripts*" -prune -o                                       \
	-path "$LNX/drivers*" -prune -o                                       \
	-name "*.[chxsS]" -print                                              \
		| sort                                                        \
		| tee "$LNX/cscope.files"

retv=$?

[ $retv -eq 0 ] || exit 126


exit


time \
	cscope -b -q -k

retv=$?

echo "Cscope exit status : $retv"

exit $retv
# FIXME: vim: set et ts=4 sts=4 sw=4
