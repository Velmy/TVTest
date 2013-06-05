/* Version ID for the JPEG library.
 * Might be useful for tests like "#if JPEG_LIB_VERSION >= 60".
 */
#define JPEG_LIB_VERSION  80	/* Version 8d */

/* libjpeg-turbo version */
#define LIBJPEG_TURBO_VERSION 1.3.0

/* Support arithmetic encoding */
#define C_ARITH_CODING_SUPPORTED

/* Support arithmetic decoding */
#define D_ARITH_CODING_SUPPORTED

/* Support in-memory source/destination managers */
#define MEM_SRCDST_SUPPORTED

/* Compiler supports function prototypes. */
#define HAVE_PROTOTYPES

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H

/* Compiler supports 'unsigned char'. */
#define HAVE_UNSIGNED_CHAR

/* Compiler supports 'unsigned short'. */
#define HAVE_UNSIGNED_SHORT

/* Compiler does not support pointers to unspecified structures. */
#undef INCOMPLETE_TYPES_BROKEN

/* Compiler has <strings.h> rather than standard <string.h>. */
#undef NEED_BSD_STRINGS

/* Linker requires that global names be unique in first 15 characters. */
#undef NEED_SHORT_EXTERNAL_NAMES

/* Need to include <sys/types.h> in order to obtain size_t. */
#undef NEED_SYS_TYPES_H

/* Broken compiler shifts signed values as an unsigned shift. */
#undef RIGHT_SHIFT_IS_UNSIGNED

/* Use accelerated SIMD routines. */
#define WITH_SIMD

/* Define to 1 if type `char' is unsigned and you are not using gcc.  */
#ifndef __CHAR_UNSIGNED__
//# undef __CHAR_UNSIGNED__
#endif

/* Define to empty if `const' does not conform to ANSI C. */
//#undef const

/* Define to `unsigned int' if <sys/types.h> does not define. */
//#undef size_t



/* ------------------------ import from jconfig.h.in ------------------------ */

/* Define "boolean" as unsigned char, not enum, per Windows custom */
#ifndef __RPCNDR_H__		/* don't conflict if rpcndr.h already read */
typedef unsigned char boolean;
#endif
#define HAVE_BOOLEAN

/* Define "INT32" as int, not long, per Windows custom */
#if !(defined(_BASETSD_H_) || defined(_BASETSD_H))   /* don't conflict if basetsd.h already read */
typedef short INT16;
typedef signed int INT32;
#endif
#define XMD_H                   /* prevent jmorecfg.h from redefining it */

#ifdef JPEG_INTERNALS

#undef RIGHT_SHIFT_IS_UNSIGNED

#endif /* JPEG_INTERNALS */

#ifdef JPEG_CJPEG_DJPEG

#define BMP_SUPPORTED		/* BMP image file format */
#define GIF_SUPPORTED		/* GIF image file format */
#define PPM_SUPPORTED		/* PBMPLUS PPM/PGM image file format */
#undef RLE_SUPPORTED		/* Utah RLE image file format */
#define TARGA_SUPPORTED		/* Targa image file format */

#define TWO_FILE_COMMANDLINE	/* optional */
#define USE_SETMODE		/* Microsoft has setmode() */
#undef NEED_SIGNAL_CATCHER
#undef DONT_USE_B_MODE
#undef PROGRESS_REPORT		/* optional */

#endif /* JPEG_CJPEG_DJPEG */
