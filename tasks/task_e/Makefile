CXX = mpic++
CXXFLAGS = -O2

.PHONY: clean all

all: main

test: test.cpp bigint.h
	g++ test.cpp -o test

main: task_e.cpp bigint.h
	$(CXX) $(CXXFLAGS) task_e.cpp -o main

clean:
	rm -f main
