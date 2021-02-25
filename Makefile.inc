CXX = g++ -std=c++11
CXXFLAGS = -O3 -Wall -Wextra

CHILKATPATH = /home/alan/Downloads/chilkat/chilkat-9.5.0-x86_64-linux-gcc
CHILKATINC = $(CHILKATPATH)/include
CHILKATLIB =  $(CHILKATPATH)/lib
CHILKATLDFLAGS = -lchilkat-9.5.0 -lresolv -lpthread

GUMBOPARSERPATH = /usr/local
GUMBOPARSERINC = $(GUMBOPARSERPATH)/include
GUMBOPARSERLIB =  $(GUMBOPARSERPATH)/lib
GUMBOPARSERCXXFLAGS = `pkg-config --cflags gumbo`
GUMBOPARSERLDFLAGS = `pkg-config --libs gumbo`
