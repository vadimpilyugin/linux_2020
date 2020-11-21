#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libbuf], [0.0.0.1], [vadim_pilyugin@mail.ru])
AM_INIT_AUTOMAKE([foreign])
PKG_CHECK_MODULES([CHECK],[check])
LT_INIT([disable-static])

AC_CONFIG_SRCDIR([src/libbuf.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lbuf':
AC_CHECK_LIB([buf], [buf_grow1])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gettimeofday strtoull])

AC_ARG_ENABLE([gcov],
  [AS_HELP_STRING([--enable-gcov],[use Gcov to test the test suite])],
  [enable_gcov=yes],
  [enable_gcov=no])
AM_CONDITIONAL([COND_GCOV],[true])

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_OUTPUT
