################################################################################
# libnkfcppビルド用Makefile 
# Contributor:
#	Hiroyuki Nagata <newserver002@gmail.com>
################################################################################

# target and sources
TARGET  = libwxnkf.a
SOURCES = $(shell find src/ -name '*.cpp')
OBJECTS = $(SOURCES:.cpp=.o)

# sources for dependency
DEPSRCS = $(shell find src/ -name '*.cpp')
DEP		= dep

# basic command
CXX		:= g++ -gstabs
AR      := ar
RM		:= rm
VERSION := 2.1.2
MKDIR	:= mkdir

# compile option
VPATH    = include src
CXXFLAGS = -Wall -I./include `wx-config --cxxflags`
LDFLAGS := -static `wx-config --libs`
ARFLAG	:= crsv

# dummy target
.PHONY: dep clean
# make all
all:	$(DEP) $(TARGET)
# suffix rule
.cpp.o:
		$(CXX) $(CXXFLAGS) -c $<
# make dependency
dep:
	$(CXX) -MM -MG $(DEPSRCS) >makefile.dep
# build library
$(TARGET): $(OBJECTS) $(SOURCES)
		$(AR) $(ARFLAG) $(TARGET) $(OBJECTS)
# clean
clean:
		$(RM) -f *.o $(TARGET) makefile.dep
# dependency
-include makefile.dep