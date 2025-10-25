CXX = mpic++
CXXFLAGS = -O2

.PHONY: clean all

all: main

main: task_sort.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) task_sort.cpp -o bin/main

clean:
	rm -f bin/*
