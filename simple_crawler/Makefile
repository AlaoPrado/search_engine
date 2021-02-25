include ../Makefile.inc

CXXFILES = $(wildcard *.cpp)
OBJFILES = $(CXXFILES:.cpp=.o)
OUTFILES = $(CXXFILES:.cpp=.out)
BUILDFLAGS = $(CXXFLAGS) -I$(CHILKATINC)
LDFLAGS = -Wl,-rpath=$(CHILKATLIB) -L$(CHILKATLIB) \
	$(CHILKATLDFLAGS)

all: $(OBJFILES) $(OUTFILES)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(BUILDFLAGS)

%.out: $(OBJFILES)
	$(CXX) -o $@ $(OBJFILES) $(LDFLAGS)

clean:
	rm -rf $(OBJFILES)
	rm -rf $(OUTFILES)

.PHONY: all clean
