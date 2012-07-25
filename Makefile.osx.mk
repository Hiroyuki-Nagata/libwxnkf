################################################################################
# libnkfcppビルド用Makefile for Mac
# Contributor:
#	Hiroyuki Nagata <newserver002@gmail.com>
################################################################################

# target and sources
TARGET  = libnkfcpp.a
SOURCES = FlagPool.cpp			 \
		  GuessConv.cpp			 \
		  InputCodeList.cpp 	 \
		  LibNKF.cpp			 \
		  NKFNativeEncodings.cpp \
		  UTF16Util.cpp			 \
		  UTF8Table.cpp			 \
		  Util.cpp
OBJECTS = $(SOURCES:.cpp=.o)
# sources for dependency
DEPSRCS = $(SOURCES)
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
DEVHOME	 = /Developer/SDKs/MacOSX10.7.sdk/usr/
#CPPINC	 = /Developer/SDKs/MacOSX10.7.sdk/usr/include/c++/4.2.1/
VPATH    = include src
CXXFLAGS = -Wall -I. -I include -I/Developer/SDKs/MacOSX10.7.sdk/usr/include -I/Developer/SDKs/MacOSX10.7.sdk/usr/include/c++/4.2.1/
LDFLAGS := -L$(DEVHOME)lib -lcrt1.10.6.o
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
	$(CXX) -MM -MG $(DEPSRCS) >makefile.depend
# build library
$(TARGET): $(OBJECTS) $(SOURCES)
		$(AR) $(ARFLAG) $(TARGET) $(OBJECTS)
# build test code
$(TESTS): $(TESTSBIN)
$(TESTSBIN):$(TESTOBJ)
		$(CXX) $^ -o $@ -L. -lnkfcpp $(LDFLAGS)
# clean
clean:
		$(RM) -f *.o $(TARGET) $(TESTS)1.exe 
# dependency
-include makefile.dep