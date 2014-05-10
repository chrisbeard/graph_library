#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_CPP11_NULLPTR
#include "Graph.h"
#include "catch.hpp"
#include <sstream>

/*
TEST_CASE("readFromFile(std::string file)", "File input") {
	Graph G(DIRECTED);
	G.readFromFile("test_input.txt");
	G.writeToFile("test_input-output.txt");
}
*/

TEST_CASE("addVertex()", "Allocate space for an edge") {
	Graph GD(DIRECTED);
	REQUIRE(GD.empty());
	GD.addVertex();
	REQUIRE(!GD.empty());

	Graph GU(UNDIRECTED);
	REQUIRE(GU.empty());
	GU.addVertex();
	REQUIRE(!GU.empty());
}
	
TEST_CASE("addEdge(int v1, int v2, double weight)", "Add graph edge") {
	Graph G(DIRECTED);
	G.addEdge(1,2,3.12);
	G.addEdge(2,5,7.25);	// TODO: figure out why this isn't in output file
	G.addEdge(2,1,3.21);
	G.addEdge(5,1,6.51);
	G.writeToFile("test_add-edge.txt");
}
