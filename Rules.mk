.SUFFIXES: .d

CC := gcc
CXX	:= g++
LD := g++
AR := ar

CFLAGS	+= -g -pipe -Wall \
  -Wextra \
  -Wno-unused-parameter \
  -Wno-missing-field-initializers \
  -Wmissing-include-dirs \
  -Wfloat-equal \
  -Wpointer-arith \
  -Wwrite-strings -Wshadow \
  -lxml2 -lxslt -I/usr/include/libxml2/

CXXFLAGS += -g -pipe -Wall \
  -Wextra \
  -Wno-unused-parameter \
  -Wno-missing-field-initializers \
  -Wmissing-include-dirs \
  -Wfloat-equal \
  -Wpointer-arith \
  -Wwrite-strings -Wshadow \
  -lxml2 -lxslt -I/usr/include/libxml2/

ifdef NDEBUG
	CFLAGS += -O2
	CXXFLAGS += -O2
endif

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.d: %.c
	$(CC) $(CFLAGS) -MM $< > $@

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $< > $@

%.d: %.cc
	$(CXX) $(CXXFLAGS) -MM $< > $@

