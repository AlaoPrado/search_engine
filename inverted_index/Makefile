# Copyright 2021 Alan Prado Araújo
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# Author: alanpradoaraujo@gmail.com (Alan Prado Araújo)

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
