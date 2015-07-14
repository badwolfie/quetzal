bin_PROGRAMS = quetzal

BUILT_SOURCES = resources.c

resources.c: $(top_srcdir)/data/quetzal.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir=$(top_srcdir)/data/ $(top_srcdir)/data/quetzal.gresource.xml)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(top_srcdir)/data/ --generate-source $<

AM_CFLAGS = \
	-include config.h \
	$(quetzal_CFLAGS) \
	-DLOCALEDIR=\""$(localedir)"\" \
	-DPKGDATADIR.=\""$(pkgdatadir)"\" \
	-DPKGLIBDIR=\""$(pkglibdir)"\" \
	-Wall

quetzal_SOURCES = \
	$(BUILT_SOURCES)

quetzal_LDADD = \
	$(quetzal_LIBS)
