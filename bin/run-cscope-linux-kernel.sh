#!/bin/sh
#
# @link http://cscope.sourceforge.net/large_projects.html
#

# LIST DIRS OF INTEREST :
# find -type d \( -path ./Documentation -o -path ./drivers -o -path ./firmware -o -path ./scripts -o -path ./sound \) -prune -o \( -path './arch/*' \! -path ./arch/x86 -prune \) -o -type d -print

# FILES for cscope.files :
# find -type d \( -path ./Documentation -o -path ./drivers -o -path ./firmware -o -path ./scripts -o -path ./sound \) -prune -o \( -path './arch/*' \! -path ./arch/x86 -prune \) -o -type f -name "*.[chxsS]" > cscope.files

# SIMPLER :
# find arch/x86/ block/ crypto/ fs/ include/ init/ ipc/ kernel/ lib/ mm/ net/ethernet/ net/ipv4/ net/core/ net/dns_resolver/ net/netfilter/ net/packet/ net/unix/ tools/arch/x86/ -type f -name "*.[chxsS]" -print | sort > cscope.files

LNX=$( cd `dirname "$0"`/../misc/linux-kernel/ && pwd )

cd "$LNX" || exit 127

echo "+-- $0 $@"
echo "|"
echo "| Creating cscope.files"

find "$LNX"/{include,arch/x86,block,crypto,fs,init,ipc,kernel,lib,mm,net/{ethernet,ipv4,core,dns_resolver,netfilter,packet,unix},tools/arch/x86} \
	-type f -name "*.[chxsS]" \
	-print | sort > cscope.files

echo "File cscope.files contains  `wc -l cscope.files` entries."

echo "Running cscope -b -q -k"

time \
    cscope -b -q -k

retv=$?
echo "Cscope exit status: $retv"

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
