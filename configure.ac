#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([measuresuite],[0.5.2],[])
AC_CONFIG_SRCDIR([src/measuresuite.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AM_PROG_AR

# Checks for libraries.
AC_CHECK_LIB([dl], [dlopen])
AC_CHECK_LIB([m], [ceil])
AC_CHECK_LIB([assemblyline], [asm_create_instance])
AC_SUBST([LIBASSEMBLYLINE_CFLAGS])
AC_SUBST([LIBASSEMBLYLINE_LIBS])

PKG_CHECK_MODULES([LIBASSEMBLYLINE], [assemblyline >= 1.2.0])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h stdint.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

#defines LIBTOOL variable
LT_INIT()

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gettimeofday memset])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

# Check for node and node-gyp
# AC_CHECK_PROG(HAVE_NODE, [node], [node])
# AC_CHECK_PROG(HAVE_NODE_GYP, [node-gyp], [node-gyp])
# test -z "$HAVE_NODE"     && AC_MSG_ERROR([Could not find node.js. Please make sure you have it installed])
# test -z "$HAVE_NODE_GYP" && AC_MSG_ERROR([Could not find node_gyp. Please make sure you have it installed])

# Run node-gyp configure during ./configure

# (cd src-ts && node-gyp configure)

AC_CONFIG_FILES([
                 src/Makefile
                 src-ts/Makefile
                 Makefile
                 test/Makefile
                 ])
AC_OUTPUT
