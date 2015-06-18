#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
AC_INIT([goptfoo], [2], [https://github.com/thrig/goptfoo])
AC_CONFIG_SRCDIR([lib/goptfoo.c])
AC_CONFIG_HEADER([config.h])

# standard configuration for shared libraries
AC_CONFIG_AUX_DIR([build-aux])
# NOTE older versions of auto* require that one manually mkdir this dir
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign]) # don't insert GNU files
LT_INIT([disable-static])

AC_SUBST(AM_CFLAGS,"-std=c99 -pedantic -Wall -fstack-protector-all")

# Checks for programs.
AC_PROG_CC_C99

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdlib.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_STRTOD
AC_CHECK_FUNCS([strtof strtoll strtoul])

AC_CONFIG_FILES([Makefile lib/Makefile man/Makefile goptfoo.pc])
AC_OUTPUT
