CXX = mpic++
CXXFLAGS = -O3 -std=c++23 -Wall

.PHONY: clean build run plot run_mpi all show

all: build run plot run_mpi

show:
	feh result/heatmap.png
	feh result/heatmap_mpi.png

build: bin/main bin/mpi

run: result/data.dat

run_mpi: result/data_mpi.dat

result/data.dat: bin/main
	./bin/main > result/data.dat

result/data_mpi.dat: bin/mpi
	mpirun -np 8 ./bin/mpi > result/data_mpi.dat

plot: result/heatmap.png result/heatmap_mpi.png

result/heatmap.png: result/data.dat result/data_mpi.dat plot.gp
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
