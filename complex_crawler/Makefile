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

build: build_crawler build_threadpool build_utils
	$(CXX) -c $(CXXFILES) $(CHILKBUILDFLAGS)

build_crawler: buid_crawler_action build_crawler_scheduler build_crawler_task
	$(CXX) -c crawler/$(CXXFILES) $(CHILKBUILDFLAGS)

buid_crawler_action:
	$(CXX) -c crawler/action/$(CXXFILES) $(CHILKBUILDFLAGS)

build_crawler_scheduler:
	$(CXX) -c crawler/scheduler/$(CXXFILES) $(CHILKBUILDFLAGS)

build_crawler_task:
	$(CXX) -c crawler/task/$(CXXFILES) $(CHILKBUILDFLAGS)

build_threadpool:
	$(CXX) -c ../threadpool/$(CXXFILES) $(CHILKBUILDFLAGS)

build_utils:
	$(CXX) -c ../utils/$(CXXFILES) $(CHILKBUILDFLAGS)

link:
	$(CXX) -o $(PROGRAM) $(OBJFILES) $(LINKFLAGS)

clean:
	rm -rf *.o
	rm -rf *.out

clean_storage:
	rm -rf storage/*.html
	rm -rf storage/*.txt

.PHONY: all build build_crawler buid_crawler_action build_crawler_scheduler \
				build_utils link clean clean_storage
