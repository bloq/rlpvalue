m4_define([librlpvalue_major_version], [1])
m4_define([librlpvalue_minor_version], [1])
m4_define([librlpvalue_micro_version], [4])
m4_define([librlpvalue_interface_age], [4])
# If you need a modifier for the version number. 
# Normally empty, but can be used to make "fixup" releases.
m4_define([librlpvalue_extraversion], [])

dnl libtool versioning from librlpvalue
m4_define([librlpvalue_current], [m4_eval(100 * librlpvalue_minor_version + librlpvalue_micro_version - librlpvalue_interface_age)])
m4_define([librlpvalue_binary_age], [m4_eval(100 * librlpvalue_minor_version + librlpvalue_micro_version)])
m4_define([librlpvalue_revision], [librlpvalue_interface_age])
m4_define([librlpvalue_age], [m4_eval(librlpvalue_binary_age - librlpvalue_interface_age)])
m4_define([librlpvalue_version], [librlpvalue_major_version().librlpvalue_minor_version().librlpvalue_micro_version()librlpvalue_extraversion()])


AC_INIT([rlpvalue], [0.0.1],
        [http://github.com/jgarzik/rlpvalue/])

dnl make the compilation flags quiet unless V=1 is used
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PREREQ(2.60)
AC_CONFIG_SRCDIR([lib/rlpvalue.cpp])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])
AC_CONFIG_HEADERS([rlpvalue-config.h])
AX_SUBDIRS_CONFIGURE([univalue], [--disable-shared])
AM_INIT_AUTOMAKE([subdir-objects foreign])

dnl Require C++11 compiler (no GNU extensions)
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory], [nodefault])
AC_LANG(C++)

LIBRLPVALUE_MAJOR_VERSION=librlpvalue_major_version
LIBRLPVALUE_MINOR_VERSION=librlpvalue_minor_version
LIBRLPVALUE_MICRO_VERSION=librlpvalue_micro_version
LIBRLPVALUE_INTERFACE_AGE=librlpvalue_interface_age

# ABI version
# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
LIBRLPVALUE_CURRENT=librlpvalue_current
LIBRLPVALUE_REVISION=librlpvalue_revision
LIBRLPVALUE_AGE=librlpvalue_age

AC_CHECK_LIB(argp, argp_parse, ARGP_LIBS=-largp)
AC_SUBST(ARGP_LIBS)

AC_SUBST(LIBRLPVALUE_CURRENT)
AC_SUBST(LIBRLPVALUE_REVISION)
AC_SUBST(LIBRLPVALUE_AGE)

LT_INIT
LT_LANG([C++])

case $host in
  *mingw*)
    LIBTOOL_APP_LDFLAGS="$LIBTOOL_APP_LDFLAGS -all-static"
  ;;
esac

BUILD_EXEEXT=
case $build in
  *mingw*)
    BUILD_EXEEXT=".exe"
  ;;
esac

AC_CONFIG_FILES([
    Makefile
    pc/librlpvalue.pc
    pc/librlpvalue-uninstalled.pc])

AC_SUBST(LIBTOOL_APP_LDFLAGS)
AC_SUBST(BUILD_EXEEXT)
AC_OUTPUT

