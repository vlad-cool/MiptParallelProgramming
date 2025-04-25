CXX = mpic++
CXXFLAGS = -O3 -std=c++20

.PHONY: clean all

all: bin/main

bin/main: bin/main.o bin/gol_single.o bin/gol_shared.o bin/gol_mpi.o bin/gol_hybrid.o
	$(CXX) $(CXXFLAGS) $^ -o bin/main

bin/main.o: main.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $^ -o bin/main.o

bin/gol_single.o: gol_single.cpp gol_single.h
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c gol_single.cpp -o bin/gol_single.o

bin/gol_shared.o: gol_shared.cpp gol_shared.h gol_single.h
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c gol_shared.cpp -o bin/gol_shared.o

bin/gol_mpi.o: gol_mpi.cpp gol_mpi.h gol_single.h
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c gol_mpi.cpp -o bin/gol_mpi.o

bin/gol_hybrid.o: gol_hybrid.cpp gol_hybrid.h gol_single.h gol_shared.h
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c gol_hybrid.cpp -o bin/gol_hybrid.o

clean:
	rm -f bin/*
