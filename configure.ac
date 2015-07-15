AC_PREREQ(2.61)
AC_INIT([Quetzal Text Editor], [0.11], [mailto:ihernandezs@openmailbox.org],[quetzal])

AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.10 foreign dist-bzip2 no-dist-gzip])

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

# Check for programs
AX_CHECK_ENABLE_DEBUG
AC_PROG_CC

# Initialize libtool
LT_PREREQ([2.2.6])
LT_INIT

# *****
# GNOME
# *****

GNOME_COMPILE_WARNINGS([maximum])

# ***************************
# Check for required packages
# ***************************

pkg_modules="gtk+-3.0 >= 3.14 gtksourceview-3.0 >= 3.16.1 gee-0.8 >= 0.18.0 vte-2.91 >= 0.40.0"
PKG_CHECK_MODULES(quetzal, [$pkg_modules])
AC_SUBST(quetzal_CFLAGS)
AC_SUBST(quetzal_LIBS)

# *************
# Documentation
# *************

# AX_REQUIRE_DEFINED([YELP_HELP_INIT])
# YELP_HELP_INIT

# ********************
# Internationalisation
# ********************

IT_PROG_INTLTOOL([0.40.0])

AC_SUBST([GETTEXT_PACKAGE], [quetzal])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], "$GETTEXT_PACKAGE", [Define to the gettext package name.])
# AM_GNU_GETTEXT_VERSION([0.11])
# AM_GLIB_GNU_GETTEXT

# *********
# GSettings
# *********

AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

GLIB_GSETTINGS

# *************************************
# *************************************

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
