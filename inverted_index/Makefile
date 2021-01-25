include ../Makefile.inc

CXXFILES = *.cpp
OBJFILES = *.o
PROGRAM = a.out
CHILKATPATH = /home/alan/Downloads/chilkat/chilkat-9.5.0-x86_64-linux-gcc
CHILKATINC = $(CHILKATPATH)/include
CHILKATLIB =  $(CHILKATPATH)/lib
BUILDFLAGS = $(COMMONBUILDFLAGS) -I$(CHILKATINC)
LINKFLAGS = -Wl,-rpath=$(CHILKATLIB) -L$(CHILKATLIB) -lchilkat-9.5.0 -lresolv -lpthread

all: build link

build: build_utils build_html_parser build_text_parser build_document
	$(CXX) -c $(CXXFILES) $(COMMONBUILDFLAGS)

build_utils:
	$(CXX) -c ../utils/$(CXXFILES) $(BUILDFLAGS)

build_html_parser:
	$(CXX) -c ../html_parser/$(CXXFILES) $(BUILDFLAGS)

build_text_parser:
	$(CXX) -c ../text_parser/$(CXXFILES) $(BUILDFLAGS)

build_document:
	$(CXX) -c document/$(CXXFILES) $(BUILDFLAGS)

link:
	$(CXX) -o $(PROGRAM) $(OBJFILES) $(LINKFLAGS)

clean:
	rm -rf *.o
	rm -rf *.out

.PHONY: all build link
