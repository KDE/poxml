#ifndef ANTLR_EXPORT_H
#define ANTLR_EXPORT_H

#include <kdemacros.h>

#ifndef ANTLR_EXPORT
# if defined(MAKE_ANTLR_LIB)
   /* We are building this library */
#  define ANTLR_EXPORT KDE_EXPORT
# else
   /* We are using this library */
#  define ANTLR_EXPORT KDE_IMPORT
# endif
#endif

#ifdef COMPILING_TESTS
#ifndef ANTLR_TEST_EXPORT
# if defined(MAKE_ANTLR_TEST_LIB)
   /* We are building this library */
#  define ANTLR_TEST_EXPORT KDE_EXPORT
# else
   /* We are using this library */
#  define ANTLR_TEST_EXPORT KDE_IMPORT
# endif
#endif
#endif /* COMPILING_TESTS */

#endif
