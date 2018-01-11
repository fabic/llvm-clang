#include <nstd/kernel/linux/errno.hpp>
#include <nstd/c/assert.h>

namespace kernel {

  /** Mapping of `E_xxx` kernel erroc codes to a pair of string:
   * 0) the constant name from the `ErrorNo` enumeration,
   * 1) a more detailed text.
   *
   * * For `error_description()`.
   * * FIXME: having the numeric error value in parenthesis in bad.
   * * FIXME: obviously this is not a very good idea -_-
   */
  static const char * error_description_table[134][2] = {
    { "<zero> - dude! zero is _not_ an error condition `-_-" },
    { "E_PERM (1)", "E_PERM (1) - Operation not permitted" },
    { "E_NOENT (2)", "E_NOENT (2) - No such file or directory" },
    { "E_SRCH (3)", "E_SRCH (3) - No such process" },
    { "E_INTR (4)", "E_INTR (4) - Interrupted system call" },
    { "E_IO (5)", "E_IO (5) - I/O error" },
    { "E_NXIO (6)", "E_NXIO (6) - No such device or address" },
    { "E_2BIG (7)", "E_2BIG (7) - Argument list too long" },
    { "E_NOEXEC (8)", "E_NOEXEC (8) - Exec format error" },
    { "E_BADF (9)", "E_BADF (9) - Bad file number" },
    { "E_CHILD (10)", "E_CHILD (10) - No child processes" },
    { "E_AGAIN (11)", "E_AGAIN (11) - Try again (or E_WOULDBLOCK: Operation would block)" },
    { "E_NOMEM (12)", "E_NOMEM (12) - Out of memory" },
    { "E_ACCES (13)", "E_ACCES (13) - Permission denied" },
    { "E_FAULT (14)", "E_FAULT (14) - Bad address" },
    { "E_NOTBLK (15)", "E_NOTBLK (15) - Block device required" },
    { "E_BUSY (16)", "E_BUSY (16) - Device or resource busy" },
    { "E_EXIST (17)", "E_EXIST (17) - File exists" },
    { "E_XDEV (18)", "E_XDEV (18) - Cross-device link" },
    { "E_NODEV (19)", "E_NODEV (19) - No such device" },
    { "E_NOTDIR (20)", "E_NOTDIR (20) - Not a directory" },
    { "E_ISDIR (21)", "E_ISDIR (21) - Is a directory" },
    { "E_INVAL (22)", "E_INVAL (22) - Invalid argument" },
    { "E_NFILE (23)", "E_NFILE (23) - File table overflow" },
    { "E_MFILE (24)", "E_MFILE (24) - Too many open files" },
    { "E_NOTTY (25)", "E_NOTTY (25) - Not a typewriter" },
    { "E_TXTBSY (26)", "E_TXTBSY (26) - Text file busy" },
    { "E_FBIG (27)", "E_FBIG (27) - File too large" },
    { "E_NOSPC (28)", "E_NOSPC (28) - No space left on device" },
    { "E_SPIPE (29)", "E_SPIPE (29) - Illegal seek" },
    { "E_ROFS (30)", "E_ROFS (30) - Read-only file system" },
    { "E_MLINK (31)", "E_MLINK (31) - Too many links" },
    { "E_PIPE (32)", "E_PIPE (32) - Broken pipe" },
    { "E_DOM (33)", "E_DOM (33) - Math argument out of domain of func" },
    { "E_RANGE (34)", "E_RANGE (34) - Math result not representable" },
    { "E_DEADLOCK (35)", "E_DEADLOCK (35) - Resource deadlock would occur" },
    { "E_NAMETOOLONG (36)", "E_NAMETOOLONG (36) - File name too long" },
    { "E_NOLCK (37)", "E_NOLCK (37) - No record locks available" },
    { "E_NOSYS (38)", "E_NOSYS (38) - Invalid system call number" },
    { "E_NOTEMPTY (39)", "E_NOTEMPTY (39) - Directory not empty" },
    { "E_LOOP (40)", "E_LOOP (40) - Too many symbolic links encountered" },

    { "<undefined error code> (41)", "<undefined error code> (41)" },

    { "E_NOMSG (42)", "E_NOMSG (42) - No message of desired type" },
    { "E_IDRM (43)", "E_IDRM (43) - Identifier removed" },
    { "E_CHRNG (44)", "E_CHRNG (44) - Channel number out of range" },
    { "E_L2NSYNC (45)", "E_L2NSYNC (45) - Level 2 not synchronized" },
    { "E_L3HLT (46)", "E_L3HLT (46) - Level 3 halted" },
    { "E_L3RST (47)", "E_L3RST (47) - Level 3 reset" },
    { "E_LNRNG (48)", "E_LNRNG (48) - Link number out of range" },
    { "E_UNATCH (49)", "E_UNATCH (49) - Protocol driver not attached" },
    { "E_NOCSI (50)", "E_NOCSI (50) - No CSI structure available" },
    { "E_L2HLT (51)", "E_L2HLT (51) - Level 2 halted" },
    { "E_BADE (52)", "E_BADE (52) - Invalid exchange" },
    { "E_BADR (53)", "E_BADR (53) - Invalid request descriptor" },
    { "E_XFULL (54)", "E_XFULL (54) - Exchange full" },
    { "E_NOANO (55)", "E_NOANO (55) - No anode" },
    { "E_BADRQC (56)", "E_BADRQC (56) - Invalid request code" },
    { "E_BADSLT (57)", "E_BADSLT (57) - Invalid slot" },

    { "<undefined error code> (58)", "<undefined error code> (58)" },

    { "E_BFONT (59)", "E_BFONT (59) - Bad font file format" },
    { "E_NOSTR (60)", "E_NOSTR (60) - Device not a stream" },
    { "E_NODATA (61)", "E_NODATA (61) - No data available" },
    { "E_TIME (62)", "E_TIME (62) - Timer expired" },
    { "E_NOSR (63)", "E_NOSR (63) - Out of streams resources" },
    { "E_NONET (64)", "E_NONET (64) - Machine is not on the network" },
    { "E_NOPKG (65)", "E_NOPKG (65) - Package not installed" },
    { "E_REMOTE (66)", "E_REMOTE (66) - Object is remote" },
    { "E_NOLINK (67)", "E_NOLINK (67) - Link has been severed" },
    { "E_ADV (68)", "E_ADV (68) - Advertise error" },
    { "E_SRMNT (69)", "E_SRMNT (69) - Srmount error" },
    { "E_COMM (70)", "E_COMM (70) - Communication error on send" },
    { "E_PROTO (71)", "E_PROTO (71) - Protocol error" },
    { "E_MULTIHOP (72)", "E_MULTIHOP (72) - Multihop attempted" },
    { "E_DOTDOT (73)", "E_DOTDOT (73) - RFS specific error" },
    { "E_BADMSG (74)", "E_BADMSG (74) - Not a data message" },
    { "E_OVERFLOW (75)", "E_OVERFLOW (75) - Value too large for defined data type" },
    { "E_NOTUNIQ (76)", "E_NOTUNIQ (76) - Name not unique on network" },
    { "E_BADFD (77)", "E_BADFD (77) - File descriptor in bad state" },
    { "E_REMCHG (78)", "E_REMCHG (78) - Remote address changed" },
    { "E_LIBACC (79)", "E_LIBACC (79) - Can not access a needed shared library" },
    { "E_LIBBAD (80)", "E_LIBBAD (80) - Accessing a corrupted shared library" },
    { "E_LIBSCN (81)", "E_LIBSCN (81) - .lib section in a.out corrupted" },
    { "E_LIBMAX (82)", "E_LIBMAX (82) - Attempting to link in too many shared libraries" },
    { "E_LIBEXEC (83)", "E_LIBEXEC (83) - Cannot exec a shared library directly" },
    { "E_ILSEQ (84)", "E_ILSEQ (84) - Illegal byte sequence" },
    { "E_RESTART (85)", "E_RESTART (85) - Interrupted system call should be restarted" },
    { "E_STRPIPE (86)", "E_STRPIPE (86) - Streams pipe error" },
    { "E_USERS (87)", "E_USERS (87) - Too many users" },
    { "E_NOTSOCK (88)", "E_NOTSOCK (88) - Socket operation on non-socket" },
    { "E_DESTADDRREQ (89)", "E_DESTADDRREQ (89) - Destination address required" },
    { "E_MSGSIZE (90)", "E_MSGSIZE (90) - Message too long" },
    { "E_PROTOTYPE (91)", "E_PROTOTYPE (91) - Protocol wrong type for socket" },
    { "E_NOPROTOOPT (92)", "E_NOPROTOOPT (92) - Protocol not available" },
    { "E_PROTONOSUPPORT (93)", "E_PROTONOSUPPORT (93) - Protocol not supported" },
    { "E_SOCKTNOSUPPORT (94)", "E_SOCKTNOSUPPORT (94) - Socket type not supported" },
    { "E_OPNOTSUPP (95)", "E_OPNOTSUPP (95) - Operation not supported on transport endpoint" },
    { "E_PFNOSUPPORT (96)", "E_PFNOSUPPORT (96) - Protocol family not supported" },
    { "E_AFNOSUPPORT (97)", "E_AFNOSUPPORT (97) - Address family not supported by protocol" },
    { "E_ADDRINUSE (98)", "E_ADDRINUSE (98) - Address already in use" },
    { "E_ADDRNOTAVAIL (99)", "E_ADDRNOTAVAIL (99) - Cannot assign requested address" },
    { "E_NETDOWN (100)", "E_NETDOWN (100) - Network is down" },
    { "E_NETUNREACH (101)", "E_NETUNREACH (101) - Network is unreachable" },
    { "E_NETRESET (102)", "E_NETRESET (102) - Network dropped connection because of reset" },
    { "E_CONNABORTED (103)", "E_CONNABORTED (103) - Software caused connection abort" },
    { "E_CONNRESET (104)", "E_CONNRESET (104) - Connection reset by peer" },
    { "E_NOBUFS (105)", "E_NOBUFS (105) - No buffer space available" },
    { "E_ISCONN (106)", "E_ISCONN (106) - Transport endpoint is already connected" },
    { "E_NOTCONN (107)", "E_NOTCONN (107) - Transport endpoint is not connected" },
    { "E_SHUTDOWN (108)", "E_SHUTDOWN (108) - Cannot send after transport endpoint shutdown" },
    { "E_TOOMANYREFS (109)", "E_TOOMANYREFS (109) - Too many references: cannot splice" },
    { "E_TIMEDOUT (110)", "E_TIMEDOUT (110) - Connection timed out" },
    { "E_CONNREFUSED (111)", "E_CONNREFUSED (111) - Connection refused" },
    { "E_HOSTDOWN (112)", "E_HOSTDOWN (112) - Host is down" },
    { "E_HOSTUNREACH (113)", "E_HOSTUNREACH (113) - No route to host" },
    { "E_ALREADY (114)", "E_ALREADY (114) - Operation already in progress" },
    { "E_INPROGRESS (115)", "E_INPROGRESS (115) - Operation now in progress" },
    { "E_STALE (116)", "E_STALE (116) - Stale file handle" },
    { "E_UCLEAN (117)", "E_UCLEAN (117) - Structure needs cleaning" },
    { "E_NOTNAM (118)", "E_NOTNAM (118) - Not a XENIX named type file" },
    { "E_NAVAIL (119)", "E_NAVAIL (119) - No XENIX semaphores available" },
    { "E_ISNAM (120)", "E_ISNAM (120) - Is a named type file" },
    { "E_REMOTEIO (121)", "E_REMOTEIO (121) - Remote I/O error" },
    { "E_DQUOT (122)", "E_DQUOT (122) - Quota exceeded" },
    { "E_NOMEDIUM (123)", "E_NOMEDIUM (123) - No medium found" },
    { "E_MEDIUMTYPE (124)", "E_MEDIUMTYPE (124) - Wrong medium type" },
    { "E_CANCELED (125)", "E_CANCELED (125) - Operation Canceled" },
    { "E_NOKEY (126)", "E_NOKEY (126) - Required key not available" },
    { "E_KEYEXPIRED (127)", "E_KEYEXPIRED (127) - Key has expired" },
    { "E_KEYREVOKED (128)", "E_KEYREVOKED (128) - Key has been revoked" },
    { "E_KEYREJECTED (129)", "E_KEYREJECTED (129) - Key was rejected by service" },
    { "E_OWNERDEAD (130)", "E_OWNERDEAD (130) - Owner died" },
    { "E_NOTRECOVERABLE (131)", "E_NOTRECOVERABLE (131) - State not recoverable" },
    { "E_RFKILL (132)", "E_RFKILL (132) - Operation not possible due to RF-kill" },
    { "E_HWPOISON (133)", "E_HWPOISON (133) - Memory page has hardware error" }
  };


  // At least we have a simple compile-time guard in case we mess up above.
  static_assert(
      sizeof(error_description_table) / sizeof(char*) / 2 == E_HWPOISON+1,
      "Ouch! The number of elements in our `error_description_table` thing should be 134."
    );


  const char *
    error_description(long ec)
    {
      ec = ec < 0 ? -ec : ec;
      assert(ec < sizeof(error_description_table) / sizeof(char *) / 2);
      return error_description_table[ ec ][1];
    }

} // kernel ns.
