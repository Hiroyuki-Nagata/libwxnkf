check_PROGRAMS		= wxnkf-test1
TESTS			= wxnkf-test1
AUTOMAKE_OPTIONS	= color-tests

wxnkf_test1_SOURCES	= wxnkf-test1.cpp
wxnkf_test1_CXXFLAGS	= @CXXFLAGS@ -I$(top_builddir)/src
wxnkf_test1_LDADD	= $(top_builddir)/src/libwxnkf.la
wxnkf_test1_LDFLAGS	= `@WX_CONFIG_PATH@ --libs base,core`