CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11 -g
EXECUTABLE = graph_exec

$(EXECUTABLE): Tester.o Graph.o
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY clean:
	rm -rf *.o $(EXECUTABLE)
