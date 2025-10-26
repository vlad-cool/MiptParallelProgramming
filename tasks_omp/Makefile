CXX = g++
CXXFLAGS = -Wall -O3 -fopenmp -lomp -Wextra -Werror=return-type

SOURCES = $(wildcard src/*.cpp)
TARGETS = $(addprefix bin/, $(SOURCES:src/%.cpp=%))

.PHONY: clean all

all: $(TARGETS)

bin/%: src/%.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f bin/*
