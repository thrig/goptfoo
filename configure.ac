#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([goptfoo], [1:1], [https://github.com/thrig/goptfoo])
AC_CONFIG_SRCDIR([lib/goptfoo.c])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([foreign]) # don't insert GNU files
LT_INIT([disable-static])

AC_SUBST(AM_CFLAGS,"-std=c99 -pedantic -Wall -Werror -fstack-protector-all")

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
AC_CHECK_FUNCS([strtoul])

AC_CONFIG_FILES([Makefile lib/Makefile man/Makefile goptfoo.pc])
AC_OUTPUT