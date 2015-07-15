bin_PROGRAMS = quetzal

BUILT_SOURCES = resources.c

resources.c: $(top_srcdir)/data/quetzal.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir=$(top_srcdir)/data/ $(top_srcdir)/data/quetzal.gresource.xml)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(top_srcdir)/data/ --generate-source $<

quetzal_SOURCES = \
	qt-application.c qt-application.h \
	quetzal.c \
	$(BUILT_SOURCES)

quetzal_CPPFLAGS = \
	-I$(top_srcdir) \
	-DG_LOG_DOMAIN=\"QuetzalTextEditor\" \
	-DDATADIR=\""$(pkgdatadir)"\" \
	-DQTLOCALEDIR=\""$(datadir)/locale"\" \
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
	$(INTLLIBS)