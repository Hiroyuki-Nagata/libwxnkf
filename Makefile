################################################################################
# libnkfcppビルド用Makefile 
# Contributor:
#	Hiroyuki Nagata <newserver002@gmail.com>
################################################################################

# target and sources
TARGET  = libwxnkf.a
SOURCES = wxnkf.cpp flagset.cpp utf8table.cpp wxnkfencoding.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGETPATH = lib/libwxnkf.a

# sources for dependency
DEPSRCS = $(shell find src/ -name '*.cpp')
DEP		= dep

# test sources and objects
TEST	= test

# basic command
CXX		:= g++ -gstabs
AR      := ar
RM		:= rm
MV		:= mv
VERSION := 2.1.2
MKDIR	:= mkdir
MAKE	:= make

# compile option
VPATH    = include src
CXXFLAGS = -Wall -I./include `wx-config --cxxflags`
LDFLAGS := -static `wx-config --libs`
ARFLAG	:= crsv

# dummy target
.PHONY: dep clean
# make all
all:	$(DEP) $(TARGET) $(TEST)
# suffix rule
.cpp.o:
		$(CXX) $(CXXFLAGS) -c $<
# make dependency
$(DEP):
	$(CXX) -MM -MG $(DEPSRCS) >makefile.dep
# build library
$(TARGET) : $(OBJECTS) $(SOURCES)
		$(AR) $(ARFLAG) $(TARGET) $(OBJECTS)
		$(MV) $(TARGET) $(TARGETPATH)
		$(MAKE) -C $(TEST)
# clean
clean:
		$(RM) -f *.o $(TARGET) makefile.dep $(TARGETPATH)
		$(MAKE) -C $(TEST) clean
# build test code
test: 
		$(MAKE) -C $(TEST)
# dependency
-include makefile.dep