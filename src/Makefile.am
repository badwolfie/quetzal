bin_PROGRAMS = quetzal

BUILT_SOURCES = resources.c

resources.c: $(top_srcdir)/data/quetzal.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir=$(top_srcdir)/data/ $(top_srcdir)/data/quetzal.gresource.xml)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(top_srcdir)/data/ --generate-source $<

quetzal_SOURCES = \
	quetzal-application.c quetzal-application.h \
	quetzal-appwindow.c quetzal-appwindow.h \
	quetzal-headerbar.c quetzal-headerbar.h \
	quetzal.c quetzal.h \
	$(BUILT_SOURCES)

quetzal_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/inc \
	-DG_LOG_DOMAIN=\"QuetzalTextEditor\" \
	-DDATADIR=\""$(pkgdatadir)"\" \
	-DQUETZAL_LOCALEDIR=\""$(datadir)/locale"\" \
	$(DISABLE_DEPRECATED) \
	$(AM_CPPFLAGS)

quetzal_CFLAGS = \
	$(QUETZAL_CFLAGS) \
	$(WARN_CFLAGS) \
	$(AM_CFLAGS)

quetzal_LDFLAGS = \
	$(AM_LDFLAGS)

quetzal_LDADD = \
	$(QUETZAL_LIBS) \
	$(top_srcdir)/inc/bdw-utils.a \
	$(INTLLIBS)

