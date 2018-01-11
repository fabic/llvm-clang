#ifndef _DUDE_NSTD_KERNEL_ERRNO_H
#define _DUDE_NSTD_KERNEL_ERRNO_H

namespace kernel {

// TODO: see if we can issue a pre-processor warning
//       about us unsetting that macro.
// Renamed the enum errno -> ErrorNo, it's better this way.
//#ifdef errno
//#undef errno
//#endif


  /**
   * Return a string with some detail about the error code `ec`.
   */
  const char * error_description(long ec);


  /**
   * * Replaced the first `Exxx` letter with `E_xxx` so that we do not conflict
   *   with kernel macros.
   * * `linux-headers/include/asm-generic/errno-base.h`
   * * `linux-headers/include/asm-generic/errno.h`
   */
  enum ErrorNo : long {
    /* From `linux-headers/include/asm-generic/errno-base.h` */
    E_PERM     =   1,  /* Operation not permitted */
    E_NOENT    =   2,  /* No such file or directory */
    E_SRCH     =   3,  /* No such process */
    E_INTR     =   4,  /* Interrupted system call */
    E_IO       =   5,  /* I/O error */
    E_NXIO     =   6,  /* No such device or address */
    E_2BIG     =   7,  /* Argument list too long */
    E_NOEXEC   =   8,  /* Exec format error */
    E_BADF     =   9,  /* Bad file number */
    E_CHILD    =  10,  /* No child processes */
    E_AGAIN    =  11,  /* Try again */
    E_NOMEM    =  12,  /* Out of memory */
    E_ACCES    =  13,  /* Permission denied */
    E_FAULT    =  14,  /* Bad address */
    E_NOTBLK   =  15,  /* Block device required */
    E_BUSY     =  16,  /* Device or resource busy */
    E_EXIST    =  17,  /* File exists */
    E_XDEV     =  18,  /* Cross-device link */
    E_NODEV    =  19,  /* No such device */
    E_NOTDIR   =  20,  /* Not a directory */
    E_ISDIR    =  21,  /* Is a directory */
    E_INVAL    =  22,  /* Invalid argument */
    E_NFILE    =  23,  /* File table overflow */
    E_MFILE    =  24,  /* Too many open files */
    E_NOTTY    =  25,  /* Not a typewriter */
    E_TXTBSY   =  26,  /* Text file busy */
    E_FBIG     =  27,  /* File too large */
    E_NOSPC    =  28,  /* No space left on device */
    E_SPIPE    =  29,  /* Illegal seek */
    E_ROFS     =  30,  /* Read-only file system */
    E_MLINK    =  31,  /* Too many links */
    E_PIPE     =  32,  /* Broken pipe */
    E_DOM      =  33,  /* Math argument out of domain of func */
    E_RANGE    =  34,  /* Math result not representable */

    /* From `linux-headers/include/asm-generic/errno.h` */
    E_DEADLK       = 35,  /* Resource deadlock would occur */
    E_DEADLOCK     = E_DEADLK,
    E_NAMETOOLONG  = 36,  /* File name too long */
    E_NOLCK        = 37,  /* No record locks available */

    /*
     * This error code is special: arch syscall entry code will return
     * -ENOSYS if users try to call a syscall that doesn't exist.  To keep
     * failures of syscalls that really do exist distinguishable from
     * failures due to attempts to use a nonexistent syscall, syscall
     * implementations should refrain from returning -ENOSYS.
     */
    E_NOSYS       = 38,  /* Invalid system call number */

    E_NOTEMPTY    = 39,      /* Directory not empty */
    E_LOOP        = 40,      /* Too many symbolic links encountered */
    E_WOULDBLOCK  = E_AGAIN,  /* Operation would block */
    E_NOMSG       = 42,      /* No message of desired type */
    E_IDRM        = 43,      /* Identifier removed */
    E_CHRNG       = 44,      /* Channel number out of range */
    E_L2NSYNC     = 45,      /* Level 2 not synchronized */
    E_L3HLT       = 46,      /* Level 3 halted */
    E_L3RST       = 47,      /* Level 3 reset */
    E_LNRNG       = 48,      /* Link number out of range */
    E_UNATCH      = 49,      /* Protocol driver not attached */
    E_NOCSI       = 50,      /* No CSI structure available */
    E_L2HLT       = 51,      /* Level 2 halted */
    E_BADE        = 52,      /* Invalid exchange */
    E_BADR        = 53,      /* Invalid request descriptor */
    E_XFULL       = 54,      /* Exchange full */
    E_NOANO       = 55,      /* No anode */
    E_BADRQC      = 56,      /* Invalid request code */
    E_BADSLT      = 57,      /* Invalid slot */

    E_BFONT          =  59,  /* Bad font file format */
    E_NOSTR          =  60,  /* Device not a stream */
    E_NODATA         =  61,  /* No data available */
    E_TIME           =  62,  /* Timer expired */
    E_NOSR           =  63,  /* Out of streams resources */
    E_NONET          =  64,  /* Machine is not on the network */
    E_NOPKG          =  65,  /* Package not installed */
    E_REMOTE         =  66,  /* Object is remote */
    E_NOLINK         =  67,  /* Link has been severed */
    E_ADV            =  68,  /* Advertise error */
    E_SRMNT          =  69,  /* Srmount error */
    E_COMM           =  70,  /* Communication error on send */
    E_PROTO          =  71,  /* Protocol error */
    E_MULTIHOP       =  72,  /* Multihop attempted */
    E_DOTDOT         =  73,  /* RFS specific error */
    E_BADMSG         =  74,  /* Not a data message */
    E_OVERFLOW       =  75,  /* Value too large for defined data type */
    E_NOTUNIQ        =  76,  /* Name not unique on network */
    E_BADFD          =  77,  /* File descriptor in bad state */
    E_REMCHG         =  78,  /* Remote address changed */
    E_LIBACC         =  79,  /* Can not access a needed shared library */
    E_LIBBAD         =  80,  /* Accessing a corrupted shared library */
    E_LIBSCN         =  81,  /* .lib section in a.out corrupted */
    E_LIBMAX         =  82,  /* Attempting to link in too many shared libraries */
    E_LIBEXEC        =  83,  /* Cannot exec a shared library directly */
    E_ILSEQ          =  84,  /* Illegal byte sequence */
    E_RESTART        =  85,  /* Interrupted system call should be restarted */
    E_STRPIPE        =  86,  /* Streams pipe error */
    E_USERS          =  87,  /* Too many users */
    E_NOTSOCK        =  88,  /* Socket operation on non-socket */
    E_DESTADDRREQ    =  89,  /* Destination address required */
    E_MSGSIZE        =  90,  /* Message too long */
    E_PROTOTYPE      =  91,  /* Protocol wrong type for socket */
    E_NOPROTOOPT     =  92,  /* Protocol not available */
    E_PROTONOSUPPORT =  93,  /* Protocol not supported */
    E_SOCKTNOSUPPORT =  94,  /* Socket type not supported */
    E_OPNOTSUPP      =  95,  /* Operation not supported on transport endpoint */
    E_PFNOSUPPORT    =  96,  /* Protocol family not supported */
    E_AFNOSUPPORT    =  97,  /* Address family not supported by protocol */
    E_ADDRINUSE      =  98,  /* Address already in use */
    E_ADDRNOTAVAIL   =  99,  /* Cannot assign requested address */
    E_NETDOWN        = 100,  /* Network is down */
    E_NETUNREACH     = 101,  /* Network is unreachable */
    E_NETRESET       = 102,  /* Network dropped connection because of reset */
    E_CONNABORTED    = 103,  /* Software caused connection abort */
    E_CONNRESET      = 104,  /* Connection reset by peer */
    E_NOBUFS         = 105,  /* No buffer space available */
    E_ISCONN         = 106,  /* Transport endpoint is already connected */
    E_NOTCONN        = 107,  /* Transport endpoint is not connected */
    E_SHUTDOWN       = 108,  /* Cannot send after transport endpoint shutdown */
    E_TOOMANYREFS    = 109,  /* Too many references: cannot splice */
    E_TIMEDOUT       = 110,  /* Connection timed out */
    E_CONNREFUSED    = 111,  /* Connection refused */
    E_HOSTDOWN       = 112,  /* Host is down */
    E_HOSTUNREACH    = 113,  /* No route to host */
    E_ALREADY        = 114,  /* Operation already in progress */
    E_INPROGRESS     = 115,  /* Operation now in progress */
    E_STALE          = 116,  /* Stale file handle */
    E_UCLEAN         = 117,  /* Structure needs cleaning */
    E_NOTNAM         = 118,  /* Not a XENIX named type file */
    E_NAVAIL         = 119,  /* No XENIX semaphores available */
    E_ISNAM          = 120,  /* Is a named type file */
    E_REMOTEIO       = 121,  /* Remote I/O error */
    E_DQUOT          = 122,  /* Quota exceeded */

    E_NOMEDIUM       = 123,  /* No medium found */
    E_MEDIUMTYPE     = 124,  /* Wrong medium type */
    E_CANCELED       = 125,  /* Operation Canceled */
    E_NOKEY          = 126,  /* Required key not available */
    E_KEYEXPIRED     = 127,  /* Key has expired */
    E_KEYREVOKED     = 128,  /* Key has been revoked */
    E_KEYREJECTED    = 129,  /* Key was rejected by service */

    /* for robust mutexes */
    E_OWNERDEAD      = 130,  /* Owner died */
    E_NOTRECOVERABLE = 131,  /* State not recoverable */

    E_RFKILL         = 132,  /* Operation not possible due to RF-kill */

    E_HWPOISON       = 133   /* Memory page has hardware error */
  };


} // kernel ns.

#endif // _DUDE_NSTD_KERNEL_ERRNO_H
