CXX = mpic++
CXXFLAGS = -Wall -O2

.PHONY: clean all

all: main

main: main.o
	$(CXX) main.o -o main

main.o: task_e.cpp
	$(CXX) -c task_e.cpp -o main.o

clean:
	rm -f main *o
