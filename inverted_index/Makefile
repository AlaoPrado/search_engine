include ../Makefile.inc

CXXFILES = $(wildcard ../utils/*.cpp ../html_parser/*.cpp ../text_parser/*.cpp \
	*.cpp document/*.cpp)
OBJFILES = $(CXXFILES:.cpp=.o)
BUILDFLAGS = $(CXXFLAGS) -I$(CHILKATINC) $(GUMBOPARSERCXXFLAGS)

DEMOCXXFILES = $(wildcard demo/*.cpp)
DEMOOBJFILES = $(DEMOCXXFILES:.cpp=.o)
DEMOOUTFILES = $(DEMOCXXFILES:.cpp=.out)
LDFLAGS = -Wl,-rpath=$(CHILKATLIB) -L$(CHILKATLIB) \
	-Wl,-rpath=$(GUMBOPARSERLIB) -L$(GUMBOPARSERLIB) \
	$(CHILKATLDFLAGS) $(GUMBOPARSERLDFLAGS)

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
	rm -rf storage/*.txt

.PHONY: all clean
