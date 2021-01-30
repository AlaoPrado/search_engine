include ../Makefile.inc

CXXFILES = $(wildcard *.cpp)
OBJFILES = $(CXXFILES:.cpp=.o)
OUTFILES = $(CXXFILES:.cpp=.out)
CHILKATPATH = /home/alan/Downloads/chilkat/chilkat-9.5.0-x86_64-linux-gcc
CHILKATINC = $(CHILKATPATH)/include
CHILKATLIB =  $(CHILKATPATH)/lib
BUILDFLAGS = $(CXXFLAGS) -I$(CHILKATINC)
LDFLAGS = -Wl,-rpath=$(CHILKATLIB) -L$(CHILKATLIB) -lchilkat-9.5.0 -lresolv -lpthread

all: $(OBJFILES) $(OUTFILES)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(BUILDFLAGS)

%.out: $(OBJFILES)
	$(CXX) -o $@ $(OBJFILES) $(LDFLAGS)

clean:
	rm -rf $(OBJFILES)

.PHONY: all clean
