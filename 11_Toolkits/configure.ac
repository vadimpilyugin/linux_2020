#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([word_count], [0.0.1], [vadim@tm-1612])
AC_CONFIG_SRCDIR([word_count.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AM_INIT_AUTOMAKE
PKG_CHECK_MODULES(GLIB, glib-2.0)
AC_SUBST(GLIB_LIBS)
AC_SUBST(GLIB_CFLAGS)
CFLAGS="-I/usr/include/glib-2.0/ -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ ${CFLAGS}"

# Checks for libraries.
AC_CHECK_LIB([glib-2.0], [g_hash_table_new], [], AC_MSG_ERROR([glib is needed]))

# Checks for header files.
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([glib.h], [], AC_MSG_ERROR([glib.h is needed]))

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
