#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([stuph_temp_httpd], [0.1a], [charlie@stuphlabs.com])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES([LMHD], [libmicrohttpd >= 0.9])

# Checks for header files.
AC_CHECK_HEADERS([string.h syslog.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AX_CFLAGS_WARN_ALL_EXTRA

# Checks for library functions.
#AC_FUNC_MALLOC
AC_CHECK_FUNCS([malloc])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
