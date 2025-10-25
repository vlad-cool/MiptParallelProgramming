CXX = mpicc
CXXFLAGS = -Wall -O3 -Wextra -Werror=return-type
SOURCES = $(wildcard task*/*.c)
COMMON = $(wildcard common/*)
TARGETS = $(SOURCES:.c=)

.PHONY: clean all

all: $(TARGETS) $(OPENMP_TARGETS)
	cd tasks_omp && make
	cd tasks_omp_devices && make

%: %.c $(COMMON)
	$(CXX) $(CXXFLAGS) -o $@ $<

%: %.cpp
	$(OPENMP_CPPXX) $(OPENMP_CPPXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS) $(OPENMP_TARGETS)
	cd tasks_omp && make clean
	cd tasks_omp_devices && make clean