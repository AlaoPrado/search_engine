include ../Makefile.inc

CXXFILES = *.cpp
OBJFILES = *.o
PROGRAM = a.out

all: build link

build:
	$(CXX) -c $(CXXFILES) $(COMMONBUILDFLAGS)

link:
	$(CXX) -o $(PROGRAM) $(OBJFILES)

clean:
	rm -rf *.o
	rm -rf *.out

.PHONY: all build link
