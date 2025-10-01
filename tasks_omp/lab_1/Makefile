CPPXX = g++
CXXFLAGS = -Wall -O2
SOURCES = $(wildcard *.cpp)
COMMON = $(wildcard common/*)
TARGETS = $(SOURCES:.cpp=)

.PHONY: clean all

all: $(TARGETS)

%: %.cpp $(COMMON)
	$(CXX) $(CXXFLAGS) -o $@ $<


clean:
	rm -f $(TARGETS)
