CXX = mpic++
CXXFLAGS = -O3 -std=c++20

.PHONY: clean build run plot all

build: bin/main

all: build run plot

run: data.dat

data.dat: bin/main
	./bin/main > data.dat

plot: heatmap.png

heatmap.png: data.dat plot.gp
	./plot.gp

bin/main: bin/main.o
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o bin/main

bin/main.o: main.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $^ -o bin/main.o

clean:
	rm -f bin/*
