#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([XMLCC], [0.2], [tsingyat@outlook.com])
AC_CONFIG_SRCDIR([example/example1.cxx])
AC_CONFIG_HEADERS([config.h xmlcc/detail/config.hxx])

AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_MACRO_DIRS([m4])

LT_INIT

# Checks for programs.
AC_PROG_CXX

AC_ARG_WITH([external-expat],
    AS_HELP_STRING([--with-external-expat], [Use external expat instead of the bundled one]))

AS_IF([test "x$with_external_expat" = "xyes"],
      [AC_CHECK_LIB([expat], [XML_ParserCreate], [LIBS="$LIBS -lexpat "; external_expat_found=yes], [external_expat_found=no])],
      [external_expat_found=no])

AS_IF([test "x$external_expat_found" != "xno"],
      [AC_DEFINE([XMLCC_USE_EXTERNAL_EXPAT], [], [Link to external expat])],
      [AS_IF([test "x$with_external_expat" = "xyes"],
             [AC_MSG_ERROR([external expat requested but not found])])])

AM_CONDITIONAL([XMLCC_USE_EXTERNAL_EXPAT], [test x"$with_external_expat" = "xyes"])

AC_ARG_WITH([external-genx],
    AS_HELP_STRING([--with-external-genx], [Use external genx instead of the bundled one]))

AS_IF([test "x$with_external_genx" = "xyes"],
      [AC_CHECK_LIB([genx], [main], [LIBS="$LIBS -lgenx "; external_genx_found=yes], [external_genx_found=no])],
      [external_genx_found=no])

AS_IF([test "x$external_genx_found" != "xno"],
      [AC_DEFINE([XMLCC_USE_EXTERNAL_GENX], [], [Link to external genx])],
      [AS_IF([test "x$with_external_genx" = "xyes"],
             [AC_MSG_ERROR([external genx requested but not found])])])

AM_CONDITIONAL([XMLCC_USE_EXTERNAL_GENX], [test x"$with_external_genx" = "xyes"])

AS_IF([test "x$enable_shared" = "xno"],
      [AC_DEFINE([XMLCC_STATIC_LIB], [], [Build static libraries])],
      [])

AM_CONDITIONAL([XMLCC_STATIC_LIB], [test "x$enable_shared" = "xno"])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.


AC_CONFIG_FILES([Makefile
                 xmlcc/Makefile
                 example/Makefile])
AC_OUTPUT
