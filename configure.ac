# -*- Mode: Autoconf; tab-width: 4; indent-tabs-mode: nil -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ODFValidator], [0.0.9], [develop@ossii.com.tw])

LT_INIT([shared, static])
AC_PREFIX_DEFAULT([/usr])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.10 foreign subdir-objects tar-pax -Wno-portability])

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# 檢測 Windows 平臺
AC_CANONICAL_HOST
case "$host_os" in
  mingw* | cygwin* | msys*)
    AC_DEFINE([WINDOWS], [1], [Define if the host is Windows])
    ;;
  *)
    AC_DEFINE([WINDOWS], [0], [Define if the host is not Windows])
    ;;
esac

CXXFLAGS="$CXXFLAGS -Werror -std=c++17"
CFLAGS="$CFLAGS -Werror"
LIBS="$LIBS -lstdc++fs"

# 取得 jar 完整名稱
jar_name=`ls resources/odfvalidator*.jar`
ODFVALIDATOR_JAR_NAME=`basename $jar_name`

ENABLE_DEBUG=false
AC_ARG_ENABLE(debug,
    AS_HELP_STRING([--enable-debug], [Enable debug build]),
    [enable_debug=$enableval], [enable_debug=yes])

AC_MSG_CHECKING([whether to enable debug build])
if test "$enable_debug" = "yes"; then
    AC_MSG_RESULT([yes])
    ENABLE_DEBUG=true
    AC_DEFINE([ENABLE_DEBUG],1,[Whether to compile in some extra debugging support code and disable some security pieces])
    ODFVALIDATOR_JAR_PATH="`pwd`/resources/$ODFVALIDATOR_JAR_NAME"
else
    AC_MSG_RESULT([no])
    ENABLE_DEBUG=false
    AC_DEFINE([ENABLE_DEBUG],0,[Whether to compile in some extra debugging support code and disable some security pieces])
    ODFVALIDATOR_JAR_PATH="${prefix}/share/java/odfvalidator/$ODFVALIDATOR_JAR_NAME"
fi
AC_SUBST(ENABLE_DEBUG)
AM_CONDITIONAL([ENABLE_DEBUG], [test "$ENABLE_DEBUG" = "true"])
AC_SUBST(ODFVALIDATOR_JAR_PATH)
AC_DEFINE_UNQUOTED([ODFVALIDATOR_JAR_PATH],
    ["$ODFVALIDATOR_JAR_PATH"],
    [Path to ODFValidator JAR file])

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

    Target jar file: $ODFVALIDATOR_JAR_PATH
])
dnl vim:set shiftwidth=4 softtabstop=4 expandtab:
