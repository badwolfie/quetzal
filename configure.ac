# configure.ac

AC_INIT([quetzal], 0.11)
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11 no-dist-gzip dist-xz foreign])
GNOME_MAINTAINER_MODE_DEFINES

# Enable silent rules is available
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AM_MAINTAINER_MODE

AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

GLIB_GSETTINGS

AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_INSTALL
IT_PROG_INTLTOOL([0.50.1])

pkg_modules="gtk+-3.0 >= 3.14 gtksourceview-3.0 >= 3.16.1 gee-0.8 >= 0.18.0 vte-2.91 >= 0.40.0"
PKG_CHECK_MODULES(quetzal, [$pkg_modules])
AC_SUBST(quetzal_CFLAGS)
AC_SUBST(quetzal_LIBS)

GETTEXT_PACKAGE=quetzal
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [GETTEXT_PACKAGE])
AM_GNU_GETTEXT_VERSION([1.11])
AM_GLIB_GNU_GETTEXT

AC_CONFIG_FILES([
	Makefile
	data/Makefile
	data/icons/Makefile
	data/icons/48x48/Makefile
	data/icons/256x256/Makefile
	data/icons/scalable/Makefile
	po/Makefile.in
	src/Makefile
])
AC_OUTPUT
