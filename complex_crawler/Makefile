include ../Makefile.inc

CXXFILES = *.cpp
OBJFILES = *.o
PROGRAM = a.out
CHILKATPATH = /home/alan/Downloads/chilkat/chilkat-9.5.0-x86_64-linux-gcc
CHILKATINC = $(CHILKATPATH)/include
CHILKATLIB =  $(CHILKATPATH)/lib
CHILKBUILDFLAGS = $(COMMONBUILDFLAGS) -I$(CHILKATINC)
LINKFLAGS = -Wl,-rpath=$(CHILKATLIB) -L$(CHILKATLIB) -lchilkat-9.5.0 -lresolv -lpthread

all: build link

build: build_crawler build_utils
	$(CXX) -c $(CXXFILES) $(CHILKBUILDFLAGS)

build_crawler: build_crawler_scheduler
	$(CXX) -c crawler/$(CXXFILES) $(CHILKBUILDFLAGS)

build_crawler_scheduler:
	$(CXX) -c crawler/scheduler/$(CXXFILES) $(CHILKBUILDFLAGS)

build_utils:
	$(CXX) -c ../utils/$(CXXFILES) $(COMMONBUILDFLAGS)

link:
	$(CXX) -o $(PROGRAM) $(OBJFILES) $(LINKFLAGS)

clean:
	rm -rf *.o
	rm -rf *.out

.PHONY: all build build_crawler build_crawler_scheduler build_utils link clean
