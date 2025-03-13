CXX = mpicc
CXXFLAGS = -Wall -O3
SOURCES = $(wildcard task*/*.c)
COMMON = $(wildcard common/*)
TARGETS = $(SOURCES:.c=)

all: $(TARGETS)

%: %.c $(COMMON)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)