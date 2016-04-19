#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
AC_INIT([goptfoo], [4], [https://github.com/thrig/goptfoo])
AC_CONFIG_SRCDIR([lib/goptfoo.c])

# standard configuration for shared libraries
AC_CONFIG_AUX_DIR([build-aux])
# NOTE older versions of auto* require that one manually mkdir this dir
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign]) # don't insert GNU files
LT_INIT([disable-static])

# TODO probably should check ac_cv_prog_cc_c99 shell variable and fail
# if c99 compiler not present.
AC_PROG_CC_C99

AC_SUBST(AM_CFLAGS,"-pedantic -Wall -fstack-protector-all")
AC_CONFIG_HEADERS([config.h])

AC_HEADER_STDC
AC_CHECK_HEADERS([ctype.h err.h limits.h math.h stdlib.h sysexits.h unistd.h])

AC_C_CONST

AC_FUNC_STRTOD
AC_CHECK_FUNCS([realloc strtof strtoll strtoul])

AC_CONFIG_FILES([Makefile lib/Makefile man/Makefile demo/Makefile goptfoo.pc])
AC_OUTPUT
