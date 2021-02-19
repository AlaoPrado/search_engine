include ../Makefile.inc

CXXFILES = $(wildcard ../utils/*.cpp ../threadpool/*.cpp crawler/*.cpp \
	crawler/action/*.cpp crawler/scheduler/*.cpp crawler/scheduler/sync/*.cpp \
	crawler/task/*.cpp)
OBJFILES = $(CXXFILES:.cpp=.o)
CHILKATPATH = /home/alan/Downloads/chilkat/chilkat-9.5.0-x86_64-linux-gcc
CHILKATINC = $(CHILKATPATH)/include
CHILKATLIB =  $(CHILKATPATH)/lib
BUILDFLAGS = $(CXXFLAGS) -I$(CHILKATINC)

DEMOCXXFILES = $(wildcard demo/*.cpp)
DEMOOBJFILES = $(DEMOCXXFILES:.cpp=.o)
DEMOOUTFILES = $(DEMOCXXFILES:.cpp=.out)
LDFLAGS = -Wl,-rpath=$(CHILKATLIB) -L$(CHILKATLIB) \
	-lchilkat-9.5.0 -lresolv -lpthread

all: $(OBJFILES) $(DEMOOBJFILES) $(DEMOOUTFILES)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(BUILDFLAGS)

%.out: %.o $(OBJFILES) 
	$(CXX) -o $@ $< $(OBJFILES) $(LDFLAGS)

clean:
	rm -rf $(OBJFILES)
	rm -rf $(DEMOOBJFILES)

clean_storage:
	rm -rf storage/*.html
	rm -rf storage/*.txt

clean_storage_extra:
	rm -rf storage_extra/*.html
	rm -rf storage_extra/*.txt

.PHONY: all clean clean_storage clean_storage_extr
