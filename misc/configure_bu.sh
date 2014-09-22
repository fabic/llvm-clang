
local_dir=${local_dir:-$(cd ../../local && pwd)}

CC=gcc CXX=g++ \
	../binutils/configure \
		--target=`uname -m`-elf \
		--prefix=$local_dir \
		--bindir=$local_dir/bin \
		--with-sysroot=$local_dir \
		--with-build-sysroot=$local_dir \
		--disable-nls \
		--disable-werror \
			|& tee fconflog.txt
