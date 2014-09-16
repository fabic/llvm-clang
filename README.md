# LLVM Clang bulk repo.

_This actually **is** LLVM/Clang bundled up as one Git repo. as for some obscur reason even to myself I retrieved the various sources from their repositories by means of Git subtree._

_**NOTE:** Code is currently slightly altered._

* Compressed `.git` repository is about 44 MB ;
* Checked-out on disk usage is about 342 MB ;
* Current LLVM/Clang version is **3.5.0** (top-most commit is tagged `llvm-3.5.0`),
  in here included are :
  - llvm
  - clang
  - compiler-rt
  - libcxx
  - libcxxabi
  - clang-tools-extra
* **Also included are :**
  - [musl libc](http://www.musl-libc.org) @ [v1.1.4](http://git.musl-libc.org/cgit/musl/tree/?id=v1.1.4)
  - [libunwind](http://www.nongnu.org/libunwind/) @ [v1.1](http://git.savannah.gnu.org/gitweb/?p=libunwind.git;a=shortlog;h=refs/tags/v1.1)
* Commits of imported stuff have "version" tags for easing diff-ing changes over time :
  - libunwind-1.1
  - llvm-3.5.0
  - musl-1.1.4

## Git-subtree-imported stuff :

	*   d3c87a6     HEAD Merge commit '34138d6ba688d4241d7ab3bfda1bb8cbd508764e' as 'musl-libc
	| ` 34138d6     HEAD Squashed 'musl-libc/' content from commit 00733dd

	*   0e89238     HEAD Merge commit '526695fe2a5a59f9c29066b901f4e663c736f93c' as 'libunwind'
	| ` 526695f     HEAD Squashed 'libunwind/' content from commit d7322f0

	*   4459342     HEAD Merge commit 'd1d71c7b9080b420cb1694e2c0a884f528de05ba' as 'libcxxabi'
	| ` d1d71c7     HEAD Squashed 'libcxxabi/' content from commit 68fcfa1

	*   883d096     HEAD Merge commit '9c04e0efdd8d3e94b9096312d1b616e6f65fbdcc' as 'libcxx'
	| ` 9c04e0e     HEAD Squashed 'libcxx/' content from commit b9e5adb

	*   4463f68     HEAD Merge commit 'dcd332d8aa55c4af6162a2010aad392deac082bb' as 'compiler-rt'
	| ` dcd332d     HEAD Squashed 'compiler-rt/' content from commit 271e25e

	*   848a497     HEAD Merge commit 'f721c46bcc5bf9c9d2756e139ce91f4a958844d6' as 'clang-tools-extra'
	| ` f721c46     HEAD Squashed 'clang-tools-extra/' content from commit 68102a1

	*   f39c1cf     HEAD Merge commit 'fdfea04cb45b8270780305241c2f9dbce763093b' as 'clang'
	| ` fdfea04     HEAD Squashed 'clang/' content from commit b499f75

	*   5767a5d     HEAD Merge commit '64e283062eb3d62ac8e8d4b9069b28576871bbab' as 'llvm'
	| ` 64e2830     HEAD Squashed 'llvm/' content from commit df820bf

	* fca41ce       HEAD INITIAL COMMIT: README.md stub file.


## ChangeLog

* 2014-09-16 : Tested a full project clone + building of Clang **ok**, but failed building against **musl-libc** despite my patches.
* 2014-09-15 : Pushing "early-stage" version to Github.
* 2014-09-13 : Started this "project" under Git control so as to track my wanderings.

_EOF_