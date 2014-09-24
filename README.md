# LLVM Clang bulk repo.

_This actually **is** LLVM/Clang stuff bundled up as one Git repo. for /me
 playing with it._

_**2014-09-24** WIP sorting things out..._

* Checked-out on disk usage is about 342 MB ;
* Current LLVM/Clang version is **3.5.0** (top-most commit is tagged `llvm-3.5.0`),
  in here included are :

  [llvm](http://llvm.org/)        | [GH: llvm](git@github.com:llvm-mirror/llvm.git)
  [clang](http://clang.llvm.org/) | [GH: clang](git@github.com:llvm-mirror/clang.git)
  [clang-tools-extra]()           | [GH: clang-tools-extra](git@github.com:llvm-mirror/clang-tools-extra.git)
  [compiler-rt](http://compiler-rt.llvm.org/) | [GH: compiler-rt](git@github.com:llvm-mirror/compiler-rt.git)
  [libcxx](http://libcxx.llvm.org/)       | [GH: libcxx](git@github.com:llvm-mirror/libcxx.git)
  [libcxxabi](http://libcxxabi.llvm.org/) | [GH: libcxxabi](git@github.com:llvm-mirror/libcxxabi.git)
  [LLDB](http://lldb.llvm.org/)   | [GH: lldb](git@github.com:llvm-mirror/lldb.git)
  [LLD](http://lld.llvm.org/)     | [GH: lld](git@github.com:llvm-mirror/lld.git)
  [test-suite]() | [GH: test-suite](git@github.com:llvm-mirror/test-suite.git)

* **Also included are :**
  - [musl libc](http://www.musl-libc.org) @ [v1.1.4](http://git.musl-libc.org/cgit/musl/tree/?id=v1.1.4)
  - [libunwind](http://www.nongnu.org/libunwind/) @ [v1.1](http://git.savannah.gnu.org/gitweb/?p=libunwind.git;a=shortlog;h=refs/tags/v1.1)

* Commits of imported stuff have "version" tags for easing diff-ing changes over time :
  - libunwind-1.1
  - llvm-3.5.0
  - musl-1.1.4


## ChangeLog

* 2014-09-24 : Replaced those Git-subtree-checked-out stuff with Git submodules.
* 2014-09-16 : Tested a full project clone + building of Clang **ok**, but failed building against **musl-libc** despite my patches.
* 2014-09-15 : Pushing "early-stage" version to Github.
* 2014-09-13 : Started this "project" under Git control so as to track my wanderings.

_EOF_
