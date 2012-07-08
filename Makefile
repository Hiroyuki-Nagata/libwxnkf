################################################################################
# libnkfビルド用Makefile 
# Contributor:
#	Hiroyuki Nagata <newserver002@gmail.com>
################################################################################

# target and sources
TARGET  = libnkf.a
SOURCES = utf8tbl.c libnkf.c
OBJECTS = $(SOURCES:.c=.o)

# basic command
CC		:= gcc
CXX		:= g++
CCFLAGS	:= -s -O2 -Wall
CXXFLAGS:= -s -I. -O2 -Wall
LDFLAGS := -static
AR      := ar
ARFLAG	:= crsv
RM		:= rm
VERSION := 2.1.2
MKDIR	:= mkdir

# dummy target
.PHONY: clean test
# make all
all:	$(TARGET) $(OBJECTS)
# suffix rule
.c.o:
		$(CC) $(CCFLAGS) -c $<
# build library
$(TARGET): $(OBJECTS) $(SOURCES)
		$(AR) $(ARFLAG) $(TARGET) $(OBJECTS)
# clean
clean:
		$(RM) -f *.o $(TARGET)
# test
test:
		$(CXX) test.cpp $(CXXFLAGS) ./libnkf.a -o test
