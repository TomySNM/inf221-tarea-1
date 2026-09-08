CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall
LDLIBS   := -pthread
ALGOS    := $(wildcard algorithms/*.cpp)

all: matrix_multiplication

matrix_multiplication: matrix_multiplication.cpp $(ALGOS)
	$(CXX) $(CXXFLAGS) -o matrix_multiplication matrix_multiplication.cpp $(ALGOS) $(LDLIBS)

run: matrix_multiplication
	./matrix_multiplication

clean:
	rm -f matrix_multiplication
.PHONY: all run clean
