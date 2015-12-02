CXXSRCS = $(wildcard *.cpp) 
CSRCS = $(wildcard *.c) 
OBJS = $(CXXSRCS:.cpp=.o) $(CSRCS:.c=.o)
DEPS = $(OBJS:.o=.d)
EXE = applyTemplate

all : $(DEPS) $(EXE)

applyTemplate : applyTemplate.o crawler.o
	$(LD) -o $@ $^ $(LDFLAGS) -lcurl -lxml2 -lxslt -I/usr/include/libxml2/

clean : 
	rm -rf $(OBJS) $(DEPS) $(EXE)

-include $(DEPS)
include Rules.mk
