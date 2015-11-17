# LLVM Clang bulk repo.

_This actually **is** LLVM/Clang stuff bundled up here as Git submodules for /me playing with._

_**2015-11-11** WIP sorting things out..._

* <http://blog.fabic.net/diary/2014/09/14/llvm-clang-from-scratch-take-3/>
* Checked-out on disk usage is about 342 MB ;
* Current LLVM/Clang version is **3.7.0**
* In here included are :

Homepage | Github repository
---------|------------------
[llvm](http://llvm.org/)                  | [GH: llvm](https://github.com/llvm-mirror/llvm)
[clang](http://clang.llvm.org/)           | [GH: clang](https://github.com/llvm-mirror/clang) [GH: fabic/clang (fork)](https://github.com/fabic/clang)
[clang-tools-extra]()                     | [GH: clang-tools-extra](https://github.com/llvm-mirror/clang-tools-extra)
[compiler-rt](http://compiler-rt.llvm.org/) | [GH: compiler-rt](https://github.com/llvm-mirror/compiler-rt)
[libcxx](http://libcxx.llvm.org/)         | [GH: libcxx](https://github.com/llvm-mirror/libcxx)
[libcxxabi](http://libcxxabi.llvm.org/)   | [GH: libcxxabi](https://github.com/llvm-mirror/libcxxabi)
[test-suite](http://llvm.org/docs/TestingGuide.html#testsuiteoverview) | [GH: test-suite](https://github.com/llvm-mirror/test-suite)
---|---
[LLDB](http://lldb.llvm.org/)             | [GH: lldb](https://github.com/llvm-mirror/lldb)
[LLD](http://lld.llvm.org/)               | [GH: lld](https://github.com/llvm-mirror/lld)

* **Also included are :**
  - [musl libc](http://www.musl-libc.org) @ [v1.1.9](http://git.musl-libc.org/cgit/musl/tree/?id=v1.1.4)
  - [libunwind](http://www.nongnu.org/libunwind/) @ [v1.1](http://git.savannah.gnu.org/gitweb/?p=libunwind.git;a=shortlog;h=refs/tags/v1.1)
  - [CMake](https://cmake.org/Wiki/CMake/Git) v3.3.2

## Git submodules

    $ git submodule status

    -5c847f8f6c54b62853656657e0b67755cc990068 clang
    -25af2d3ca20da8131f57717d26a10227e9050a4d clang-tools-extra
    -b5214093d4c91ed5352d35ee9126665fabfa97fe compiler-rt
    -de80a7d886d75188de210eb4d6f0768fdc4d4d0b libcxx
    -8f53d45e9ac7d3c078ab3fe8f91c40a4cc6b579a libcxxabi
    -d7322f0f64dab715c4feb5f08edef5660d8719c0 libunwind
    -0a5ecb70e178a5c8de5d3851d55f545b081dd634 lld
    -bf6a6982a755ffd0503fec7d02cfe7536cd41ea6 lldb
    -703d9535b699400d0e675e91779402bb51ebb7a8 llvm
    -237df3fa4a1d939e6fd1af0c3e5029a25a137310 misc/binutils
    -2eab1132c757ea4c8881005e5cbdb55850595370 misc/clang-tutorial
    -c6fa1b412663593960e6240eb66d82fa41a1fa0b misc/gcc
    -b8079dd0d360648e4e8de48656c5c38972621072 misc/glibc
    -ca54b01c064f4f979316d337e09382f3c4710366 misc/rtags
    -00733dd1cf791d13ff6155509cf139a5f7b2eecb musl
    -a5918f52802886a40c19f752635155266a75e1e9 test-suite


    $ git submodule foreach git --no-pager log --oneline -1
    ...
    ...


    $ git submodule foreach git remote -v | grep fetch

    origin  git@github.com:fabic/clang.git (fetch)
    upstream        git@github.com:llvm-mirror/clang.git (fetch)
    origin  git@github.com:llvm-mirror/clang-tools-extra.git (fetch)
    origin  git@github.com:llvm-mirror/compiler-rt.git (fetch)
    origin  git@github.com:llvm-mirror/libcxx.git (fetch)
    origin  git@github.com:llvm-mirror/libcxxabi.git (fetch)
    origin  git://git.sv.gnu.org/libunwind.git (fetch)
    origin  git@github.com:llvm-mirror/lld.git (fetch)
    origin  git@github.com:fabic/lldb.git (fetch)
    origin  git@github.com:llvm-mirror/llvm.git (fetch)
    origin  git://sourceware.org/git/binutils-gdb.git (fetch)
    origin  git@github.com:loarabia/Clang-tutorial.git (fetch)
    origin  git://cmake.org/cmake.git (fetch)
    origin  git://sourceware.org/git/glibc (fetch)
    origin  git://git.musl-libc.org/musl (fetch)
    origin  git@github.com:llvm-mirror/test-suite.git (fetch)

### Checkout those submodules that are needed for building Clang

    git submodule update --init clang clang-tools-extra compiler-rt libcxx libcxxabi llvm test-suite

## Getting started

## Clone repos & submodules checkout

... _todo_ ...

### Drop symlinks here'n'there

    [fabi@sabayon] ~/dev/llvm2 $

    # Clang :
    ln -siv ../../clang llvm/tools/

    # Clang tools extra :
    ln -siv ../../clang-tools-extra/ clang/tools/extra

    # Compiler-RT
    ln -siv ../../compiler-rt   llvm/projects/

    # libcxx & libcxxabi :
    ln -siv ../../libcxx{,abi}  llvm/projects/

    # LLDB (optional) :
    ln -siv ../../lldb llvm/tools/

    # LLD (optional) :
    ln -siv ../../lld  llvm/tools/


### (_optional_) Source the `./environment.sh` helper script

    [fabi@sabayon] ~/dev/llvm2 $ source environment.sh

    +---
    | At /home/fabi/dev/llvm2
    +-
    | clang is:   /usr/bin/clang   [clang version 3.3 (tags/RELEASE_33/final)]
    | clang++ is: /usr/bin/clang++ [clang version 3.3 (tags/RELEASE_33/final)]
    |
    +-- Environment:
    | unset PATH  /home/fabi/dev/llvm2/bin:/home/fabi/bin:/opt/bin:/usr/local/bin:/usr/bin:/bin:/usr/x86_64-pc-linux-gnu/gcc-bin/4.7.3:/usr/games/bin
    | unset CXX   clang++
    | unset CC    clang
    +-

### (_optional_) Install our own copy of Linux headers under `local/include/`

    mkdir -pv local/include &&
      cp -ruv misc/linux-headers/include/* local/include


### Build

#### (_optional_) Build a temporary “bootstrap” version

    [fabi@sabayon] ~/dev/llvm2 $ mkdir -pv build && cd build/

    [fabi@sabayon] ~/dev/llvm2/build $

    time ( cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(cd ../local/ && pwd) ../llvm/ -G Ninja  &&  ninja -l1 ) ; echo RETVAL=$?

    # Or with the whole documentation :
    time \
      cmake -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=$(cd ../bootstrap/ && pwd) \
        -DLLVM_BUILD_DOCS=ON \
        -DLLVM_ENABLE_DOXYGEN=ON \
        -DLLVM_ENABLE_SPHINX=ON \
          ../llvm/ \
          -G Ninja

See [blob/master/doc/llvm-cmake-output.md](doc/llvm-cmake-output.md) for some of
the output it produces.

    time \
      ninja -l3.9 -k128 ; echo RETVAL=$?

    ninja install

__NOTE:__ Binaries for this first “bootstrap” variant shall have been installed under `bootstrap/`.

##### Update environment with the new bootstrap/ stuff

  . environment.sh bootstrap

  $ which clang && which clang++
  $ clang --version && clang++ --version
  $ clang++ -std=c++11 test.cpp
  $ ./a.out
  $ ldd ./a.out


#### Build & install under `local/`

    mkdir -pv build2/ &&
    cd build2/

    time \
      cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=$(cd ../local/ && pwd) \
        ../llvm/ -G Ninja

    time \
      ninja -l3.9 -k128 ; echo RETVAL=$?

    ninja install

##### Test

  (19:32:20) ± [cadet@pc126] 0 /mnt/g/llvm-clang (master ↑1 S:1 ?:1 ✗)  clang++ -std=c++11 test.cpp

  (19:32:27) ± [cadet@pc126] 0 /mnt/g/llvm-clang (master ↑1 S:1 ?:1 ✗)  ./a.out 
  1: Hello dude!
  2: Hello dude!
  3: Hello dude!

  (19:32:31) ± [cadet@pc126] 0 /mnt/g/llvm-clang (master ↑1 S:1 ?:1 ✗)  ldd a.out 
          linux-vdso.so.1 (0x00007fff11e26000)
          libc++.so.1 => /mnt/g/llvm-clang/local/lib/libc++.so.1 (0x00007fe189b40000)
          libc++abi.so.1 => /mnt/g/llvm-clang/local/lib/libc++abi.so.1 (0x00007fe1898ee000)
          libpthread.so.0 => /lib64/libpthread.so.0 (0x00007fe1896d1000)
          libm.so.6 => /lib64/libm.so.6 (0x00007fe1893cd000)
          libgcc_s.so.1 => /usr/lib/gcc/x86_64-pc-linux-gnu/4.9.3/libgcc_s.so.1 (0x00007fe1891b6000)
          libc.so.6 => /lib64/libc.so.6 (0x00007fe188e04000)
          librt.so.1 => /lib64/librt.so.1 (0x00007fe188bfc000)
          /lib64/ld-linux-x86-64.so.2 (0x00007fe189dfb000)

_Oups! it linked against `libgcc_s.so.1` instead of `libunwind` (fixme)._

##### `clang++ -E -x c++ - -v < /dev/null`

* <http://stackoverflow.com/a/11946295>

    $ clang++ -E -x c++ - -v < /dev/null

    clang version 3.7.0 
    Target: x86_64-unknown-linux-gnu
    Thread model: posix

    Found candidate GCC installation: /usr/lib/gcc/x86_64-pc-linux-gnu/4.7.3
    Found candidate GCC installation: /usr/lib/gcc/x86_64-pc-linux-gnu/4.8.3
    Found candidate GCC installation: /usr/lib/gcc/x86_64-pc-linux-gnu/4.8.4
    Found candidate GCC installation: /usr/lib/gcc/x86_64-pc-linux-gnu/4.9.2
    Found candidate GCC installation: /usr/lib/gcc/x86_64-pc-linux-gnu/4.9.3
    Found candidate GCC installation: /usr/lib64/gcc/x86_64-pc-linux-gnu/4.7.3
    Found candidate GCC installation: /usr/lib64/gcc/x86_64-pc-linux-gnu/4.8.3
    Found candidate GCC installation: /usr/lib64/gcc/x86_64-pc-linux-gnu/4.8.4
    Found candidate GCC installation: /usr/lib64/gcc/x86_64-pc-linux-gnu/4.9.2
    Found candidate GCC installation: /usr/lib64/gcc/x86_64-pc-linux-gnu/4.9.3
    Selected GCC installation: /usr/lib64/gcc/x86_64-pc-linux-gnu/4.9.3

    Candidate multilib: .;@m64
    Candidate multilib: 32;@m32
    Selected multilib: .;@m64

     "/home/fabi/dev/llvm-clang/local/bin/clang-3.7" -cc1 -triple x86_64-unknown-linux-gnu -E -disable-free -disable-llvm-verifier -main-file-name - -mrelocation-model static -mthread-model posix -mdisable-fp-elim -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -fuse-init-array -target-cpu x86-64 -v -dwarf-column-info -resource-dir /home/fabi/dev/llvm-clang/local/bin/../lib/clang/3.7.0 -I/home/fabi/dev/llvm-clang/local/include -internal-isystem /home/fabi/dev/llvm-clang/local/bin/../include/c++/v1 -internal-isystem /usr/local/include -internal-isystem /home/fabi/dev/llvm-clang/local/bin/../lib/clang/3.7.0/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /home/fabi/dev/llvm-clang -ferror-limit 19 -fmessage-length 191 -mstackrealign -fobjc-runtime=gcc -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -o - -x c++ -

    clang -cc1 version 3.7.0 based upon LLVM 3.7.0 default target x86_64-unknown-linux-gnu

    ignoring nonexistent directory "/usr/local/include"

    #include "..." search starts here:
    #include <...> search starts here:
     /home/fabi/dev/llvm-clang/local/include
     /home/fabi/dev/llvm-clang/local/bin/../include/c++/v1
     /home/fabi/dev/llvm-clang/local/bin/../lib/clang/3.7.0/include
     /include
     /usr/include
    End of search list.

    # 1 "<stdin>"
    # 1 "<built-in>" 1
    # 1 "<built-in>" 3
    # 324 "<built-in>" 3
    # 1 "<command line>" 1
    # 1 "<built-in>" 2
    # 1 "<stdin>" 2


## ChangeLog

* 2015-11-10 : Trying to replay the whole build-bootstrap-build procedure, seems ok.
* 2015-11-09 : Blindly bumped dependencies from release 3.5 to 3.7 _(probably this doesn't build)._
* 2014-09-24 : Replaced those Git-subtree-checked-out stuff with Git submodules.
* 2014-09-16 : Tested a full project clone + building of Clang **ok**, but failed building against **musl-libc** despite my patches.
* 2014-09-15 : Pushing "early-stage" version to Github.
* 2014-09-13 : Started this "project" under Git control so as to track my wanderings.



## Appendix

### Linux headers

* As per [LFS &ndash; 5.6. Linux-4.2 API Headers](http://www.linuxfromscratch.org/lfs/view/stable/chapter05/linux-headers.html)
* Fetch latest tarball from <https://www.kernel.org/pub/linux/kernel/v4.x/>
* Uncompress `tar -xf ~/Downloads/linux-4.2.6.tar.xz -C tmp/`
* `cd tmp/linux-4.2.6/`
* `make mrproper`
* `make INSTALL_HDR_PATH=../../misc/linux-headers headers_install` :

    (11:27:28) ± [cadet@pc126] 0 /mnt/g/llvm-clang/tmp/linux-4.2.6 (master ✓)  make INSTALL_HDR_PATH=../../misc/linux-headers headers_install
      CHK     include/generated/uapi/linux/version.h
      UPD     include/generated/uapi/linux/version.h
      ...
      ...
      INSTALL include/uapi (0 file)
      INSTALL include/asm (65 files)

* `find ../../misc/linux-headers/ \( -name .install -o -name ..install.cmd \) -delete`

* `git add -u ../../misc/linux-headers/  &&  git add ../../misc/linux-headers/`

### CMake (build)

* `git submodule update --init misc/cmake/` (v3.3.2)
* <http://www.linuxfromscratch.org/blfs/view/svn/general/cmake.html>

    $ cd misc/cmake/
    $ ./bootstrap --help
    $ time \
      ./bootstrap --prefix=$(cd ../../local/ && pwd) &&
      gmake
    # gmake install


## Musl libc

__NOTE:__ building it using the host Gcc compiler, for Clang outputs warnings about unsupported compiler flags.

    # DON'T (LIBCC=-lunwind) !
    # ( export REWT=$(cd .. && pwd) ; export LIBCC="-lunwind" ; ./configure --prefix=$REWT/local --syslibdir=$REWT/local/lib --enable-debug --disable-static && time make )

    ( export REWT=$(cd .. && pwd) ; \
      ./configure --prefix=$REWT/local --syslibdir=$REWT/local/lib --enable-debug --disable-static &&
      time make )


    $ find -type f -newer config.mak -not -name \*.lo

    ./tools/musl-gcc
    ./src/internal/version.h
    ./crt/crti.o
    ./crt/crt1.o
    ./crt/Scrt1.o
    ./crt/crtn.o
    ./lib/librt.a
    ./lib/libutil.a
    ./lib/crti.o
    ./lib/musl-gcc.specs
    ./lib/libresolv.a
    ./lib/libxnet.a
    ./lib/libm.a
    ./lib/crt1.o
    ./lib/Scrt1.o
    ./lib/libpthread.a
    ./lib/libdl.a
    ./lib/libc.so
    ./lib/libcrypt.a
    ./lib/crtn.o
    ./arch/x86_64/bits/alltypes.h


    $ ldd ./lib/libc.so
    statically linked

    # DON'T !
    $ make install


    $ ls -l ../local/lib/ld-musl-x86_64.so.1 
    lrwxrwxrwx 1 fabi fabi 43 Nov 11 12:46 ../local/lib/ld-musl-x86_64.so.1 -> /home/fabi/dev/llvm-clang/local/lib/libc.so


## Libunwind

* It first failed to build with against Musl libc (error from the "coredump" thing), hence passing `--disable-coredump`.
* Then it turns out it has a dependency over a `liblzma` so we're passing `--disable-minidebuginfo`.
* Ended up _not_ building against Musl libc (I recall I had to patch Clang code last year to get it build anyway).

    $ autoreconf -i

    # Not doing so (this was for having it built against Musl libc) :
    # ( export REWT=$(cd .. && pwd) ; \
    #    ./configure --prefix=$REWT/local --enable-debug --enable-cxx-exceptions --disable-minidebuginfo --disable-coredump &&
    #    make )

    #  checking whether to enable C++ exception support... yes
    #  checking whether to support LZMA-compressed symbol tables... no
    #  checking if we should build libunwind-coredump... no
    #  checking whether to enable debug support... yes


    $ ( export REWT=$(cd .. && pwd) ; \
        ./configure --prefix=$REWT/local --enable-debug --enable-cxx-exceptions &&
        touch __sentinel &&
        make )


    $ find -newer __sentinel -type f -name \\*.so\\*

    ./src/.libs/libunwind-ptrace.so.0.0.0
    ./src/.libs/libunwind.so.8.0.1
    ./src/.libs/libunwind-setjmp.so.0.0.0
    ./src/.libs/libunwind-x86_64.so.8.0.1
    ./src/.libs/libunwind-coredump.so.0.0.0


    $ ldd ./src/.libs/libunwind.so.8.0.1
        linux-vdso.so.1 (0x00007ffdd65d7000)
        libc.so.6 => /lib64/libc.so.6 (0x00007f97822c9000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f978289a000)
        liblzma.so.5 => /lib64/liblzma.so.5 (0x00007f978209f000)
        libpthread.so.0 => /lib64/libpthread.so.0 (0x00007f9781e82000)

    $ make install

### Test build something with `-lunwind`

    $ clang++ -std=c++11 -lunwind test.cpp

    $ ldd a.out 
            linux-vdso.so.1 (0x00007fff00b0a000)
            libunwind.so.8 => /home/fabi/dev/llvm-clang/local/lib/libunwind.so.8 (0x00007f09595a8000)
            libc++.so.1 => /home/fabi/dev/llvm-clang/local/lib/libc++.so.1 (0x00007f09592ed000)
            libc++abi.so.1 => /home/fabi/dev/llvm-clang/local/lib/libc++abi.so.1 (0x00007f095909b000)
            libpthread.so.0 => /lib64/libpthread.so.0 (0x00007f0958e7e000)
            libm.so.6 => /lib64/libm.so.6 (0x00007f0958b7a000)
            libgcc_s.so.1 => /usr/lib/gcc/x86_64-pc-linux-gnu/4.9.3/libgcc_s.so.1 (0x00007f0958963000)
            libc.so.6 => /lib64/libc.so.6 (0x00007f09585b1000)
            /lib64/ld-linux-x86-64.so.2 (0x00007f09597c8000)
            liblzma.so.5 => /lib64/liblzma.so.5 (0x00007f0958387000)
            librt.so.1 => /lib64/librt.so.1 (0x00007f095817f000)

__NOTE :__ it somehow got linked against libunwind ; `libgcc_s` remains though, but I can't recall
much stuff about how/why this happens.

_EOF_
