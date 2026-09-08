CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall
LDLIBS   := -pthread
ALGOS    := $(wildcard algorithms/*.cpp)

all: sorting

sorting: sorting.cpp $(ALGOS) algorithms/sorting.hpp ../common/bench.hpp
	$(CXX) $(CXXFLAGS) -o sorting sorting.cpp $(ALGOS) $(LDLIBS)

run: sorting
	./sorting

clean:
	rm -f sorting
.PHONY: all run clean