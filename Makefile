LDFLAGS = -pthread

CXXFLAGS = \
	-g -D_GNU_SOURCE -std=c++11 \
	-I. -I./src -I./examples

CXXFLAGS += -Wall
#CXXFLAGS += -O3

BASIC_EXAMPLE = \
	examples/example.o \

PROGRAMS = \
	examples/example

all: $(PROGRAMS)

examples/example: $(BASIC_EXAMPLE)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(PROGRAMS) ./*.o ./*.so ./*/*.o ./*/*.so
