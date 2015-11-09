# LLVM Clang bulk repo.

_This actually **is** LLVM/Clang stuff bundled up as one Git repo. for /me
 playing with it._

_**2014-09-24/25** WIP sorting things out..._

* Checked-out on disk usage is about 342 MB ;
* Current LLVM/Clang version is **3.5.0** (top-most commit is tagged `llvm-3.5.0`),
  in here included are :

Homepage | Github repository
---------|------------------
[llvm](http://llvm.org/)                  | [GH: llvm](https://github.com/llvm-mirror/llvm)
[clang](http://clang.llvm.org/)           | [GH: clang](https://github.com/llvm-mirror/clang)
[clang-tools-extra]()                     | [GH: clang-tools-extra](https://github.com/llvm-mirror/clang-tools-extra)
[compiler-rt](http://compiler-rt.llvm.org/) | [GH: compiler-rt](https://github.com/llvm-mirror/compiler-rt)
[libcxx](http://libcxx.llvm.org/)         | [GH: libcxx](https://github.com/llvm-mirror/libcxx)
[libcxxabi](http://libcxxabi.llvm.org/)   | [GH: libcxxabi](https://github.com/llvm-mirror/libcxxabi)
[LLDB](http://lldb.llvm.org/)             | [GH: lldb](https://github.com/llvm-mirror/lldb)
[LLD](http://lld.llvm.org/)               | [GH: lld](https://github.com/llvm-mirror/lld)
[test-suite](http://llvm.org/docs/TestingGuide.html#testsuiteoverview) | [GH: test-suite](https://github.com/llvm-mirror/test-suite)

* **Also included are :**
  - [musl libc](http://www.musl-libc.org) @ [v1.1.4](http://git.musl-libc.org/cgit/musl/tree/?id=v1.1.4)
  - [libunwind](http://www.nongnu.org/libunwind/) @ [v1.1](http://git.savannah.gnu.org/gitweb/?p=libunwind.git;a=shortlog;h=refs/tags/v1.1)

* Commits of imported stuff have "version" tags for easing diff-ing changes over time :
  - libunwind-1.1
  - llvm-3.5.0
  - musl-1.1.4


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

    # LLDB :
    ln -siv ../../lldb llvm/tools/

    # LLD :
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

#### (_optional_) Build a temporary bootstrap version

    [fabi@sabayon] ~/dev/llvm2 $ mkdir -pv build && cd build/

    [fabi@sabayon] ~/dev/llvm2/build $

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



## ChangeLog

* 2015-11-09 : Blindly bumped dependencies from release 3.5 to 3.7 _(probably this doesn't build)._
* 2014-09-24 : Replaced those Git-subtree-checked-out stuff with Git submodules.
* 2014-09-16 : Tested a full project clone + building of Clang **ok**, but failed building against **musl-libc** despite my patches.
* 2014-09-15 : Pushing "early-stage" version to Github.
* 2014-09-13 : Started this "project" under Git control so as to track my wanderings.

_EOF_
