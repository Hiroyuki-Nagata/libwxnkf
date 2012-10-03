bin_PROGRAMS = test2
test2_SOURCES = main.cpp
test2_CXXFLAGS=@CXXFLAGS@
test2_LDADD=@LIBS@

if USE_WINDRES
test2_LDADD += test2_rc.o
endif

if USE_WINDRES
test2_rc.o : sample.rc
	@WX_RESCOMP@ -i $^ -O coff -o $@
endif