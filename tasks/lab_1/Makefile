CXX = mpic++
CXXFLAGS = -O3 -std=c++23

.PHONY: clean build run plot all

all: build run plot

build: bin/main bin/mpi

run: result/data.dat

result/data.dat: bin/main
	./bin/main > result/data.dat

plot: result/heatmap.png

result/heatmap.png: result/data.dat plot.gp
	./plot.gp

bin/main: bin/main.o bin/common.o bin/problem.o
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o bin/main

bin/mpi: bin/mpi.o bin/common.o bin/problem.o
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o bin/mpi

bin/main.o: main.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $^ -o bin/main.o

bin/mpi.o: mpi.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $^ -o bin/mpi.o

bin/common.o: common.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $^ -o bin/common.o

bin/problem.o: problem.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $^ -o bin/problem.o

clean:
	rm -f bin/*
