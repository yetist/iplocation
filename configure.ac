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
#AC_CHECK_HEADERS([fcntl.h limits.h locale.h memory.h netdb.h stdlib.h string.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT8_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
#AC_CONFIG_MACRO_DIR([m4])
#LT_INIT

GIO_REQUIRED=2.35.0

PKG_CHECK_MODULES(GIO, gio-unix-2.0 >= $GIO_REQUIRED)

#MOSES_DBUS_PATH=/org/moses/SessionManager
#AC_DEFINE_UNQUOTED(MOSES_DBUS_PATH, "$MOSES_DBUS_PATH", [The moses session dbus path])
#AC_SUBST(MOSES_DBUS_PATH)
#MOSES_DBUS_NAME=org.moses.SessionManager
#AC_DEFINE_UNQUOTED(MOSES_DBUS_NAME, "$MOSES_DBUS_NAME", [The moses session dbus name])
#AC_SUBST(MOSES_DBUS_NAME)

dnl PKG_CHECK_VAR(SESSION_SERVECE_DIR, dbus-1, session_bus_services_dir)

dnl ====================================================================
dnl Language Support
dnl ====================================================================
#IT_PROG_INTLTOOL([0.40.6])
#
#GETTEXT_PACKAGE=moses-session
#AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [The gettext translation domain])
#AC_SUBST(GETTEXT_PACKAGE)
#
#AM_GLIB_GNU_GETTEXT
GLIB_GSETTINGS

AC_CONFIG_FILES([
Makefile
src/Makefile
data/Makefile
])
AC_OUTPUT
