# -*- Mode: Autoconf; tab-width: 4; indent-tabs-mode: nil -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ODFValidator], [0.0.9], [develop@ossii.com.tw])

LT_INIT([shared, disable-static])
AC_PREFIX_DEFAULT([/usr])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.10 foreign subdir-objects tar-pax -Wno-portability])

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

CXXFLAGS="$CXXFLAGS -Werror -std=c++17"
CFLAGS="$CFLAGS -Werror"
LIBS="$LIBS -lstdc++fs"

ENABLE_DEBUG=false
AC_ARG_ENABLE(debug,
    AS_HELP_STRING([--enable-debug], [Enable debug build]),
    [enable_debug=$enableval], [enable_debug=no])

AC_MSG_CHECKING([whether to enable debug build])
if test "$enable_debug" = "yes"; then
    AC_MSG_RESULT([yes])
    ENABLE_DEBUG=true
    AC_DEFINE([ENABLE_DEBUG],1,[Whether to compile in some extra debugging support code and disable some security pieces])
else
    AC_MSG_RESULT([no])
    AC_DEFINE([ENABLE_DEBUG],0,[Whether to compile in some extra debugging support code and disable some security pieces])
fi
AC_SUBST(ENABLE_DEBUG)
AM_CONDITIONAL([ENABLE_DEBUG], [test "$ENABLE_DEBUG" = "true"])

# AC_OUTPUT
AC_CONFIG_FILES([
    Makefile
    src/Makefile
    resources/Makefile
    odfvalidator.spec
])
AC_OUTPUT

AC_MSG_RESULT([
    ODFValidator version $PACKAGE_VERSION
    Build type: $build
    Compiler: $CC $CFLAGS
    C++ Compiler: $CXX $CXXFLAGS
    Enable debug: $ENABLE_DEBUG
])
dnl vim:set shiftwidth=4 softtabstop=4 expandtab:
