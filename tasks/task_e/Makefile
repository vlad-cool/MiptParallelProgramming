CXX = mpic++
CXXFLAGS = -Wall -O2

.PHONY: clean all

all: main

main: main.o bignumber.o
	$(CXX) main.o bignumber.o -o main

main.o: task_e.cpp
	$(CXX) -c task_e.cpp -o main.o

bignumber.o:
	$(CXX) -c ../../common/BigNumber/src/bignumber.cpp -o bignumber.o

clean:
	rm -f main
