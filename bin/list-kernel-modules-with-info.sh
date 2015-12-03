#!/bin/sh
#
# Does `modinfo <module_name>` for each Linux kernel module listed by `lsmod`
#

echo
echo "--- \`lspci \`--------------------------------------------"
echo

lspci

echo
echo "--- MODULES LIST --------------------------------------------"
echo

lsmod | sort

echo
echo "--- MODULES INFO. -------------------------------------------"
echo

lsmod | sort | awk '{print $1}' |
    while read mumu ; do
        echo -e "\n---\n-- MODULE $mumu\n-";
        modinfo $mumu
    done 

retv=$?

exit $retv

