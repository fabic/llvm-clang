#ifndef _FEATURES_H
#define _FEATURES_H

#ifdef _ALL_SOURCE
#define _GNU_SOURCE 1
#endif

#if !defined(_POSIX_SOURCE) && !defined(_POSIX_C_SOURCE) \
 && !defined(_XOPEN_SOURCE) && !defined(_GNU_SOURCE) \
 && !defined(_BSD_SOURCE) && !defined(__STRICT_ANSI__)
#define _BSD_SOURCE 1
#define _XOPEN_SOURCE 700
#endif

#if __STDC_VERSION__ >= 199901L
#define __restrict restrict
#elif !defined(__GNUC__)
#define __restrict
#endif

#if __STDC_VERSION__ >= 199901L || defined(__cplusplus)
#define __inline inline
#endif

#if __STDC_VERSION__ >= 201112L
#elif defined(__GNUC__)
#define _Noreturn __attribute__((__noreturn__))
#else
#define _Noreturn
#endif

// ---
// Fabic.2014-09-16, see glibc's features.h
//
#if 1
	// That one is from glibc's source :
	//#define __GNUC_PREREQ(maj, min) 0

	// And below was found in /usr/include/features.h :
	#define __GLIBC__       2
	#define __GLIBC_MINOR__ 17

	#define __GLIBC_PREREQ(maj, min) \
	        ((__GLIBC__ << 16) + __GLIBC_MINOR__ >= ((maj) << 16) + (min))
#endif
// ^^ FabiC ^^ //

#endif
