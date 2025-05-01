CXX = g++
CXXFLAGS = -O3 -std=c++23 -Wall

.PHONY: clean build

all: build

build: bin/main

bin/main: bin/main.o
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o bin/main

bin/main.o: main.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $^ -o bin/main.o

clean:
	rm -f bin/*
