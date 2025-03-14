CXX = mpicc
CXXFLAGS = -Wall -O2
SOURCES = $(wildcard task*/*.c)
COMMON = $(wildcard common/*)
TARGETS = $(SOURCES:.c=)

.PHONY: clean all

all: $(TARGETS)

%: %.c $(COMMON)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)