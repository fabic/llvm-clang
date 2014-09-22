
local_dir=${local_dir:-$(cd ../../local && pwd)}

CC=gcc CXX=g++ \
../glibc/configure                      \
	--prefix=$local_dir                      \
	--host=`uname -m`-fabic-linux            \
	--build=$(../glibc/scripts/config.guess) \
	--disable-profile                        \
	--enable-kernel=2.6.32                   \
	--with-headers=$local_dir/include        \
	libc_cv_forced_unwind=yes                \
	libc_cv_ctors_header=yes                 \
	libc_cv_c_cleanup=yes                    \
		|& tee fconflog.txt


exit $?
