CXX = mpic++
CXXFLAGS = -Wall -O3

.PHONY: clean all

all: main

test: test.cpp bigint.h
	g++ test.cpp -o test

main: main.o
	$(CXX) main.o -o main

main.o: task_e.cpp
	$(CXX) -c task_e.cpp -o main.o

clean:
	rm -f main *o
