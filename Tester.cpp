#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_CPP11_NULLPTR
#include "Graph.h"
#include "catch.hpp"
#include <sstream>

TEST_CASE("readFromFile(std::string file)", "File input") {
	Graph G(DIRECTED);
	G.readFromFile("test_input.txt");
	G.writeToFile("text-output.txt");
}
	
