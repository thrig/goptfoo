AC_PREREQ([2.69])
# TODO libtool errors out if use a versioned number such as 5.0
AC_INIT([goptfoo], [5], [jmates@cpan.org], [], [https://github.com/thrig/goptfoo])
AC_CONFIG_SRCDIR([lib/goptfoo.c])
AC_CONFIG_HEADERS([config.h])

# standard configuration for shared libraries
AC_CONFIG_AUX_DIR([build-aux])
# NOTE older versions of auto* require that one manually mkdir this dir
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign]) # don't insert GNU files
LT_INIT([disable-static])

# TODO is this redundant with the next check?
#AC_PROG_CC
AC_PROG_CC_C99([], [AC_MSG_ERROR([no c99 compiler])])
AC_CHECK_HEADERS([ctype.h err.h limits.h math.h stdint.h stdlib.h string.h sysexits.h unistd.h], [], [AC_MSG_ERROR([missing header])])
AC_C_CONST
AC_TYPE_SIZE_T
AC_CHECK_FUNCS([realloc strtod strtof strtoll strtoul], [], [AC_MSG_ERROR([missing function])])

# TODO ideally should have m4 that probes ld(1) for what rpath flavor it
# has as this is something of a KLUGE (LDFLAGS is not an option as those
# may get passed to the MacPorts gcc which then barfs on the mac linker
# flags...)
# KLUGE llvm linker wants --rpath while GNU -rpath but both support -R path
# so let's try that for compat between ld.lld and ld.bfd on OpenBSD 6.5...
AC_ARG_ENABLE([macos-rpath],
    [  --enable-macos-rpath    enable macOS rpath flags for linker],
    [macos_rpath=${enableval}], [macos_rpath=no])
AC_ARG_ENABLE([the-rpath],
    [  --enable-the-rpath      enable -rpath flag for linker],
    [the_rpath=${enableval}], [the_rpath=no])

if test "x${macos_rpath}" = xyes; then
    AC_SUBST([RPATH_FOO], ["-Wl,-macosx_version_min -Wl,10.11 -Wl,-rpath -Wl,\${libdir}"])
elif test "x${the_rpath}" = xyes; then
    AC_SUBST([RPATH_FOO], ["-Wl,-R \${libdir}"])
else
    AC_SUBST([RPATH_FOO], [""])
fi

AC_CONFIG_FILES([Makefile lib/Makefile man/Makefile goptfoo.pc])

AC_OUTPUT
