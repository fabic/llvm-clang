
local_dir=${local_dir:-$(cd ../../local && pwd)}

CC=gcc CXX=g++ \
../gcc/configure                     \
    --target=`uname -m`-elf          \
    --prefix=$local_dir              \
    --with-sysroot=$local_dir        \
    --with-local-prefix=$local_dir   \
    --enable-languages=c,c++ \
	--disable-libstdcxx-pch \
    --without-headers                \
    --with-native-system-header-dir=/include \
    --with-newlib                    \
    --disable-shared                                 \
    --disable-multilib               \
    --disable-libstdc++-v3 \
    --disable-nls                    \
    --disable-decimal-float                          \
    --disable-threads                                \
    --disable-libatomic                              \
    --disable-libgomp                                \
    --disable-libitm                                 \
    --disable-libquadmath                            \
    --disable-libsanitizer                           \
    --disable-libssp                                 \
    --disable-libvtv                                 \
    --disable-libcilkrts \
	--with-mpfr-include=$(pwd)/../gcc/mpfr/src   \
	--with-mpfr-lib=$(pwd)/mpfr/src/.libs



exit $?

CC=gcc CXX=g++ \
../gcc/configure                     \
    --target=`uname -m`-elf          \
    --prefix=$local_dir              \
    --with-sysroot=$local_dir        \
    --with-local-prefix=$local_dir   \
    --disable-nls                    \
    --disable-multilib               \
	--enable-shared                  \
    --with-native-system-header-dir=/include \
    --enable-languages=c,c++ \
    --without-headers                \
	--disable-libstdcxx-pch 

    #--with-newlib                    \

    #--disable-decimal-float                          \
    #--disable-threads                                \
    #--disable-libatomic                              \
    #--disable-libgomp                                \
    #--disable-libitm                                 \
    #--disable-libquadmath                            \
    #--disable-libsanitizer                           \
    #--disable-libssp                                 \
    #--disable-libvtv                                 \
    #--disable-libcilkrts                             \
    #--disable-libstdc++-v3                           \

    #--disable-shared                                 \
