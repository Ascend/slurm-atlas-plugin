# Makefile for npu/generic plugin

AUTOMAKE_OPTIONS = foreign

PLUGIN_FLAGS = -module -avoid-version --export-dynamic

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/src/common $(DSMI_CPPFLAGS) $(DSMI_LIBS)

DSMI_SOURCES = npu_generic.c

pkglib_LTLIBRARIES = npu_generic.la

# NPU GENERIC plugin.
npu_generic_la_SOURCES = $(DSMI_SOURCES)
npu_generic_la_LDFLAGS = $(PLUGIN_FLAGS)
npu_generic_la_LIBADD = $(DSMI_LIBS)

# force:
# $(npu_generic_la_LIBADD) : force
# 	@cd `dirname $@` && $(MAKE) `basename $@`
