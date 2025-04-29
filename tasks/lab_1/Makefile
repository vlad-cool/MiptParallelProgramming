CXX = mpic++
CXXFLAGS = -O3 -std=c++20

.PHONY: clean build run plot all

all: build run plot

build: bin/main

run: data.dat

data.dat: bin/main
	./bin/main > result/data.dat

plot: heatmap.png

heatmap.png: data.dat plot.gp
	./plot.gp

bin/main: bin/main.o bin/common.o bin/problem.o
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o bin/main

bin/main.o: main.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $^ -o bin/main.o

bin/common.o: common.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $^ -o bin/common.o

bin/problem.o: problem.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $^ -o bin/problem.o

clean:
	rm -f bin/*
