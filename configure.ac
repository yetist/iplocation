#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(moses-iplocation, 0.0.2, http://bugs.isoft-linux.org)
AM_INIT_AUTOMAKE([1.11 no-dist-gzip dist-xz tar-ustar])
AC_CONFIG_SRCDIR([src/iplocation.c])
AC_CONFIG_HEADERS([config.h])

AM_MAINTAINER_MODE([enable])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT8_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
GIO_REQUIRED=2.35.0

PKG_CHECK_MODULES(GIO, gio-unix-2.0 >= $GIO_REQUIRED)

PKG_CHECK_VAR(SESSION_SERVICE_DIR, dbus-1, session_bus_services_dir)

AC_CONFIG_FILES([
Makefile
src/Makefile
data/Makefile
])
AC_OUTPUT
