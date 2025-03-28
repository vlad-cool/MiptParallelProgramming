CXX = mpic++
CXXFLAGS = -Wall -O2

.PHONY: clean all

all: main

test: test.cpp bigint.h
	g++ test.cpp -o test

main: main.o
	$(CXX) $(CXXFLAGS) main.o -o main

main.o: task_e.cpp bigint.h
	$(CXX) $(CXXFLAGS) -c task_e.cpp -o main.o

clean:
	rm -f main *o
