all: solver

solver:
	g++ -O2 -W -Wall -Wshadow -std=c++17 -Wc++11-extensions -Wc++17-extensions -o solver solver.cc

clean:
	rm -rf solver

