CXX = mpic++
CXXFLAGS = -O3

.PHONY: clean all

all: bin/main

bin/main: bin/main.o bin/gol_single.o bin/gol_shared.o
	g++ $(CXXFLAGS) $^ -o bin/main

bin/main.o: main.cpp
	g++ $(CXXFLAGS) -c $^ -o bin/main.o

bin/gol_single.o: gol_single.cpp gol_single.h
	g++ $(CXXFLAGS) -c gol_single.cpp -o bin/gol_single.o

bin/gol_shared.o: gol_shared.cpp gol_shared.h
	g++ $(CXXFLAGS) -c gol_shared.cpp -o bin/gol_shared.o

clean:
	rm -f bin/*
