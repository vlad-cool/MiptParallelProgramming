CPPXX = g++
CXXFLAGS = -Wall -O2 -Wextra -Werror=return-type
# CXXFLAGS = -Wall -O0 -Wextra -Werror=return-type
# CXXFLAGS = -Wall -g -fsanitize=address -Wextra -Werror=return-type
SOURCES = $(wildcard *.cpp)
COMMON = $(wildcard common/*)
TARGETS = $(SOURCES:.cpp=)

.PHONY: clean all

all: $(TARGETS)

%: %.cpp $(COMMON)
	$(CXX) $(CXXFLAGS) -o $@ $<


clean:
	rm -f $(TARGETS)
