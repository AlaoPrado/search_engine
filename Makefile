CXX = g++ -std=c++11
CXXFILES = *.cpp
OBJFILES = *.o
PROGRAM = a.out
CHILKATPATH = /home/alan/Downloads/chilkat/chilkat-9.5.0-x86_64-linux-gcc
CHILKATINC = $(CHILKATPATH)/include
CHILKATLIB =  $(CHILKATPATH)/lib
BUILDFLAGS = -O3 -Wall -Wextra -I$(CHILKATINC)
LINKFLAGS = -Wl,-rpath=$(CHILKATLIB) -L$(CHILKATLIB) -lchilkat-9.5.0 -lresolv -lpthread

all: build link

build:
	$(CXX) -c $(CXXFILES) $(BUILDFLAGS)

link:
	$(CXX) -o $(PROGRAM) $(OBJFILES) $(LINKFLAGS)

.PHONY: all build link
