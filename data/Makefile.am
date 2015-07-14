SUBDIRS = icons

gsettings_SCHEMAS = com.github.badwolfie.quetzal.gschema.xml
@GSETTINGS_RULES@

desktopdir = $(datadir)/applications
desktop_in_files = quetzal.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST = \
	menu.ui \
	quetzal.gresource.xml \
	$(desktop_in_files) \
	$(gsettings_SCHEMAS)

CLEANFILES = \
	$(desktop_DATA)