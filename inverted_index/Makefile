include ../Makefile.inc

CXXFILES = *.cpp
OBJFILES = *.o
PROGRAM = a.out

all: build link

build: build_utils build_html_parser build_document
	$(CXX) -c $(CXXFILES) $(COMMONBUILDFLAGS)

build_utils:
	$(CXX) -c ../utils/$(CXXFILES) $(COMMONBUILDFLAGS)

build_html_parser:
	$(CXX) -c ../html_parser/$(CXXFILES) $(COMMONBUILDFLAGS)

build_document:
	$(CXX) -c document/$(CXXFILES) $(COMMONBUILDFLAGS)

link:
	$(CXX) -o $(PROGRAM) $(OBJFILES)

clean:
	rm -rf *.o
	rm -rf *.out

.PHONY: all build link
