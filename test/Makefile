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
CXXFLAGS = -Wall -I../include `wx-config --cxxflags`
LDFLAGS  = ../lib/$(LIBNKF) `wx-config --libs`

# dummy target
.PHONY: clean mac $(BUNDLE) $(MACPKGINFO) $(MACINFOPLIST) $(COPY)

########################################## for mac ############################
# for application bundle
OUTPUTPATH		= .
PROGRAM			= $(TARGET)
PROGVER			= 1.0
# for Info.plist
COMPANY			= Hiroyuki Nagata
BUNDLEPACKAGE	= APPL
BUNDLESIGNATURE	=
# basic mac setting
BUNDLE			= $(OUTPUTPATH)/$(PROGRAM).app
MACPKGINFO		= $(BUNDLE)/Contents/PkgInfo
MACINFOPLIST	= $(BUNDLE)/Contents/Info.plist 
MACINC			= -I/Developer/SDKs/MacOSX10.7.sdk/usr/include/ -I/Developer/SDKs/MacOSX10.7.sdk/usr/include/c++/4.2.1/
MACLD			= -L/Developer/SDKs/MacOSX10.7.sdk/usr/lib/ -L/usr/lib
COPY			= copy

.PHONY	: mac
mac : CXXFLAGS += $(MACINC)
mac : LDFLAGS  += $(MACLD)
mac : $(BUNDLE) $(MACPKGINFO) $(MACINFOPLIST) all $(COPY)

$(BUNDLE):
	@echo "==== Building bundle directory structure ===="
	mkdir -p $(OUTPUTPATH)
	mkdir -p $(BUNDLE)/Contents
	mkdir -p $(BUNDLE)/Contents/MacOS
	
#  This creates the Contents/PkgInfo file.
$(MACPKGINFO):
	@echo "==== Creating PkgInfo ===="
	touch $(MACPKGINFO)
	@echo -n "$(BUNDLEPACKAGE)$(BUNDLESIGNATURE)" > $(MACPKGINFO)
	
#  This creates the Contents/Info.plist file.
$(MACINFOPLIST):
	@echo "==== Creating Info.plist ===="
	touch $(MACINFOPLIST)
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" >> $(MACINFOPLIST)
	@echo -n "<!DOCTYPE plist PUBLIC " >> $(MACINFOPLIST)
	@echo -n "\"-//Apple Computer//DTD PLIST 1.0//EN\" " >> $(MACINFOPLIST)
	@echo "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" >> $(MACINFOPLIST)
	@echo "<plist version=\"1.0\">" >> $(MACINFOPLIST)
	@echo "<dict>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleDevelopmentRegion</key>" >> $(MACINFOPLIST)
	@echo "   <string>English</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleExecutable</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PROGRAM)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleIconFile</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(ICONFILE)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleName</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PROGRAM)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleIdentifier</key>" >> $(MACINFOPLIST)
	@echo "   <string>com.$(COMPANY).$(PROGRAM)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleInfoDictionaryVersion</key>" >> $(MACINFOPLIST)
	@echo "   <string>6.0</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundlePackageType</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(BUNDLEPACKAGE)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleSignature</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(BUNDLESIGNATURE)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleVersion</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PROGVER)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleShortVersionString</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PROGVER)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleGetInfoString</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PROGRAM), Version $(PROGVER), $(COMPANY)</string>" >> $(MACINFOPLIST)
	@echo "</dict>" >> $(MACINFOPLIST)
	@echo "</plist>" >> $(MACINFOPLIST)

$(COPY):
	cp -p $(TARGET) $(BUNDLE)/Contents/MacOS/$(PROGRAM)
########################################## for mac ############################

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
clean:
	$(RM) -rf *.o $(TARGET) $(TARGET).app $(TARGET).exe