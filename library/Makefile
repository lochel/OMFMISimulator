CC=gcc
CXX=g++
RM=rm -rf
CP=cp
MKDIR=mkdir -p
CPPFLAGS=-I../3rdParty/FMIL/install/include

SRCS=OMSimulator.cpp oms_model.cpp oms_logging.cpp oms_resultfile.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

.PHONY: libOMSimulator clean distclean

libOMSimulator: $(OBJS)
	$(MKDIR) -p ../build/lib
	$(MKDIR) -p ../build/include
	ar rvs ../build/lib/libOMSimulator.a $(OBJS)
	$(CP) OMSimulator.h ../build/include/

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $<

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) ../build/lib
	$(RM) ../build/include
