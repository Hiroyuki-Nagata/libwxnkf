################################################################################
# libnkfcppビルド用Makefile 
# Contributor:
#	Hiroyuki Nagata <newserver002@gmail.com>
################################################################################

# target and sources
TARGET  = libnkfcpp.a
SOURCES = FlagPool.cpp			 \
		  LibNKF.cpp			 \
		  Util.cpp				 \
		  GuessConv.cpp			 \
		  InputCodeList.cpp 	 \
		  NKFNativeEncodings.cpp \
		  UTF16Util.cpp			 \
		  UTF8Table.cpp			 
OBJECTS = $(SOURCES:.cpp=.o)
# sources for dependency
DEPSRCS = $(shell find src/ -name '*.cpp')
# test sources and objects
TESTS	= test
TESTSBIN= test1
TESTSRC	= test1.cpp
TESTOBJ	= $(TESTSRC:.cpp=.o)

# basic command
CXX		:= g++ -gstabs
AR      := ar
RM		:= rm
VERSION := 2.1.2
MKDIR	:= mkdir

# compile option
VPATH    = include src
CXXFLAGS:= -Wall -I. -I include
LDFLAGS := -static
ARFLAG	:= crsv

# dummy target
.PHONY: dep clean test
# make all
all:	$(TARGET) $(OBJECTS) $(TESTS)
# suffix rule
.cpp.o:
		$(CXX) $(CXXFLAGS) -c $<
# make dependency
dep:
	$(CXX) -MM -MG $(DEPSRCS) >makefile.dep
# build library
$(TARGET): $(OBJECTS) $(SOURCES)
		$(AR) $(ARFLAG) $(TARGET) $(OBJECTS)
# build test code
$(TESTS): $(TESTSBIN)
$(TESTSBIN):$(TESTOBJ)
		$(CXX) $^ -o $@ $(LDFLAGS) -L. -lnkfcpp
# clean
clean:
		$(RM) -f *.o $(TARGET) $(TESTS)1.exe makefile.dep
# dependency
-include makefile.dep