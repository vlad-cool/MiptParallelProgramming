CXX = mpicc
CXXFLAGS = -Wall -O2 -Wextra -Werror=return-type
SOURCES = $(wildcard task*/*.c)
COMMON = $(wildcard common/*)
TARGETS = $(SOURCES:.c=)

OPENMP_CPPXX = g++
OPENMP_CPPXXFLAGS = -Wall -O2 -fopenmp -lomp
OPENMP_SOURCES = $(wildcard tasks_omp/*.cpp)
OPENMP_TARGETS = $(OPENMP_SOURCES:.cpp=)

.PHONY: clean all

all: $(TARGETS) $(OPENMP_TARGETS)

%: %.c $(COMMON)
	$(CXX) $(CXXFLAGS) -o $@ $<

%: %.cpp
	$(OPENMP_CPPXX) $(OPENMP_CPPXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS) $(OPENMP_TARGETS)