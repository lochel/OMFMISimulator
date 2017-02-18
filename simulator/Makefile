CC=gcc
CXX=g++
RM=rm -rf
CP=cp
MKDIR=mkdir -p
CPPFLAGS=-I../3rdParty/FMIL/install/include -I../build/include
LDFLAGS=
LDLIBS=-L../3rdParty/FMIL/install/lib -L../build/lib -lOMSimulator -lfmilib -ldl -lboost_system -lboost_filesystem

SRCS=main.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

ifeq ($(OS), Windows_NT)
  EXE=.exe
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S), Linux)
    EXE=
  endif
  ifeq ($(UNAME_S), Darwin)
    EXE=
  endif
endif

.PHONY: OMSimulator clean distclean

OMSimulator: $(OBJS)
	$(MKDIR) -p ../build/bin
	$(CXX) $(LDFLAGS) -o ../build/bin/OMSimulator$(EXE) $(OBJS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $<

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) ../build/bin
