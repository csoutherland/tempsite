#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([stuph_temp_httpd], [0.1a], [charlie@stuphlabs.com])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror])

# Checks for programs.
AC_PROG_CC
AC_CHECK_TOOL([OBJCOPY],[objcopy])

# Checks for libraries.
AX_PTHREAD
PKG_CHECK_MODULES([LMHD], [libmicrohttpd >= 0.9])

# Checks for header files.
AC_CHECK_HEADERS([string.h syslog.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
#AX_CFLAGS_WARN_ALL_EXTRA


# BFD checks I found at:
#   http://svnview.digium.com/svn/zaptel/team/tzafrir/kernelmove/configure.ac
AC_LANG(C)
AC_LANG_CONFTEST([AC_LANG_SOURCE(,)])
# FIXME: using cc directly here is probably not the best choice.
# There must be a predefined configure variable for the detected cc.
cc -c conftest.c
BFD_NAME=`LANG=C objdump -f conftest.o | grep -e "conftest.o:" | sed "s/.*file format \(.*\)/\1/"`
BFD_ARCH=`LANG=C objdump -f conftest.o | grep -e "architecture:" | sed "s/.*ture: \(.*\),.*/\1/"`
AC_SUBST(BFD_NAME)
AC_SUBST(BFD_ARCH)


# Checks for library functions.
#AC_FUNC_MALLOC
AC_CHECK_FUNCS([malloc])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
