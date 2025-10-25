CXX = mpicc
CXXFLAGS = -Wall -O3 -Wextra -Werror=return-type

SOURCES = $(wildcard src/*.c)
TARGETS = $(addprefix bin/, $(SOURCES:src/%.c=%))

.PHONY: clean all

all: $(TARGETS)
	cd lab_1 && make
	cd lab_2 && make
	cd task_e && make
	cd task_gol && make
	cd task_sort && make

bin/%: src/%.c
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f bin/*
	cd lab_1 && make clean
	cd lab_2 && make clean
	cd task_e && make clean
	cd task_gol && make clean
	cd task_sort && make clean
