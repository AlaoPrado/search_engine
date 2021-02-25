include ../Makefile.inc

CXXFILES = $(wildcard ../utils/*.cpp ../threadpool/*.cpp crawler/*.cpp \
	crawler/action/*.cpp crawler/scheduler/*.cpp crawler/scheduler/sync/*.cpp \
	crawler/task/*.cpp)
OBJFILES = $(CXXFILES:.cpp=.o)
BUILDFLAGS = $(CXXFLAGS) -I$(CHILKATINC)

DEMOCXXFILES = $(wildcard demo/*.cpp)
DEMOOBJFILES = $(DEMOCXXFILES:.cpp=.o)
DEMOOUTFILES = $(DEMOCXXFILES:.cpp=.out)
LDFLAGS = -Wl,-rpath=$(CHILKATLIB) -L$(CHILKATLIB) \
	$(CHILKATLDFLAGS)

all: $(OBJFILES) $(DEMOOBJFILES) $(DEMOOUTFILES)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(BUILDFLAGS)

%.out: %.o $(OBJFILES) 
	$(CXX) -o $@ $< $(OBJFILES) $(LDFLAGS)

clean:
	rm -rf $(OBJFILES)
	rm -rf $(DEMOOBJFILES)
	rm -rf $(DEMOOUTFILES)

clean_storage:
	rm -rf storage/*.html
	rm -rf storage/*.txt

clean_storage_extra:
	rm -rf storage_extra/*.html
	rm -rf storage_extra/*.txt

.PHONY: all clean clean_storage clean_storage_extr
