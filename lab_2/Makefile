MPICXX = mpic++
MPICXXFLAGS = -Wall -O3 -Wextra -Werror=return-type -std=c++23

CXX = g++
CXXFLAGS = -Wall -O3 -fopenmp -lomp -Wextra -Werror=return-type -std=c++23

MPI_SOURCES := $(wildcard src/*mpi.cpp)
MPI_TARGETS := $(addprefix bin/, $(MPI_SOURCES:src/%.cpp=%))

SOURCES := $(filter-out $(MPI_SOURCES), $(wildcard src/*.cpp))
TARGETS := $(addprefix bin/, $(SOURCES:src/%.cpp=%))

.PHONY: clean all

all: $(TARGETS) $(MPI_TARGETS)

bin/%_mpi: src/%_mpi.cpp
	mkdir -p bin
	$(MPICXX) $(MPICXXFLAGS) -o $@ $<


bin/%: src/%.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f bin/*
