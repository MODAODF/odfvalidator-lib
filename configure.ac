# -*- Mode: Autoconf; tab-width: 4; indent-tabs-mode: nil -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ODFValidator], [0.0.9], [develop@ossii.com.tw])

LT_INIT([shared, disable-static])
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

# AC_OUTPUT
AC_CONFIG_FILES([
        Makefile
        src/Makefile
        resources/Makefile
])
AC_OUTPUT

dnl vim:set shiftwidth=4 softtabstop=4 expandtab:
