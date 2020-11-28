#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([word_count], [0.0.1], [vadim@tm-1612])
AC_CONFIG_SRCDIR([word_count.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AM_INIT_AUTOMAKE

# Checks for libraries.
AC_CHECK_LIB([glib-2.0], [g_hash_table_new])

# Checks for header files.
AC_CHECK_HEADERS([string.h glib.h glibconfig.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([strncmp fgets])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
