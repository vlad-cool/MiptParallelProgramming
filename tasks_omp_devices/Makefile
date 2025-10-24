CXX = g++
CXXFLAGS = -fopenmp -foffload=nvptx-none -O3 -Wextra -Werror=return-type

SOURCES = $(wildcard src/*.cpp)
TARGETS = $(addprefix bin/, $(SOURCES:src/%.cpp=%))

.PHONY: clean all

all: $(TARGETS)

bin/%: src/%.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $<

bin:
	mkdir bin

clean:
	rm -f $(TARGETS)
