include ../Makefile.inc

CXXFILES = $(wildcard ../utils/*.cpp ../html_parser/*.cpp ../text_parser/*.cpp \
						*.cpp document/*.cpp)
OBJFILES = $(CXXFILES:.cpp=.o)
CHILKATPATH = /home/alan/Downloads/chilkat/chilkat-9.5.0-x86_64-linux-gcc
CHILKATINC = $(CHILKATPATH)/include
CHILKATLIB =  $(CHILKATPATH)/lib
CHILKATLDFLAGS = -lchilkat-9.5.0 -lresolv -lpthread
GUMBOPARSERPATH = /usr/local
GUMBOPARSERINC = $(GUMBOPARSERPATH)/include
GUMBOPARSERLIB =  $(GUMBOPARSERPATH)/lib
GUMBOPARSERCXXFLAGS = `pkg-config --cflags gumbo`
GUMBOPARSERLDFLAGS = `pkg-config --libs gumbo`
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

.PHONY: all clean
