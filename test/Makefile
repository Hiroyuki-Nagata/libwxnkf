################################################################################
#
# libnkftestビルド用Makefile 
# Copyright (c) 2012 HIROYUKI Nagata <jagdironscrap@gmail.com>
#
################################################################################

# target and sources
TARGET  = libnkftest
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
LIBNKF	= libwxnkf.a

# basic command
MAKE	:= make
CXX		:= g++ -gstabs
RM 		:= rm

# compile option
CXXFLAGS = -Wall -I/c/MinGW/include -I../include
LDFLAGS  = -static -L/c/MinGW/lib #../lib/$(LIBNKF)

# make all
all : $(TARGET)

# target build
$(TARGET) : $(OBJECTS)
		$(CXX) $^ -o $@ $(LDFLAGS)
# suffix rule
.SUFFIXES: .cpp .o
.cpp.o:
		$(CXX) $(CXXFLAGS) -c $<
# clean
.PHONY: clean
clean:
	$(RM) -f *.o $(TARGET)