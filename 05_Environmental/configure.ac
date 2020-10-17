#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([PcreWindow], [0.0.0.1], [vadim_pilyugin@mail.ru])
AC_CONFIG_SRCDIR([window.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AM_INIT_AUTOMAKE

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lncursesw':
AC_CHECK_LIB([ncursesw], [initscr], [], AC_MSG_ERROR([Curses is needed]))
AC_CHECK_LIB([pcre2-8], [pcre2_compile_8], [], AC_MSG_ERROR([Pcre is needed]))

# Checks for header files.
AC_CHECK_HEADERS([locale.h string.h])
AC_CHECK_HEADER([ncurses.h], [], AC_MSG_ERROR([ncurses.h is needed]))
AC_CHECK_HEADER([pcre2.h], [], AC_MSG_ERROR([pcre2.h is needed]), [
#ifndef PCRE2_CODE_UNIT_WIDTH
#define PCRE2_CODE_UNIT_WIDTH 8
#endif
])

AC_ARG_WITH([UTF], AS_HELP_STRING([--without-UTF], [Ignore UTF support]))

AS_IF([test "x$with_UTF" != "xno"],
      AC_DEFINE(UTF, [], [UTF recognition]))


# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([setlocale])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
