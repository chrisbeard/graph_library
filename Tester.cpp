#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_CPP11_NULLPTR
#include "Graph.h"
#include "catch.hpp"
#include <sstream>


TEST_CASE("readFromFile(std::string file)", "File input") {
	Graph G(DIRECTED);

	std::string fileName = "g1.txt";
	G.readFromFile(fileName);
	G.writeToFile("g1-output.txt");
	G.DFT(1, "g1-dft1.txt");
	G.BFT(2, "g1-bft2.txt");
}

TEST_CASE("empty()", "Empty graph") {
	Graph G(DIRECTED);
	REQUIRE(G.empty());
	G.addVertex();
	REQUIRE(!G.empty());

	Graph GG(UNDIRECTED);
	REQUIRE(GG.empty());
	GG.addVertex();
	REQUIRE(!GG.empty());
}

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
	REQUIRE(G.empty());
	REQUIRE_THROWS(G.addEdge(1,2,3.12));

	Graph GG(UNDIRECTED);
	for (size_t i = 0; i < 5; ++i) {
		GG.addVertex();
	}
	GG.addEdge(1,2,3.12);
	GG.addEdge(2,5,7.25);
	GG.addEdge(2,1,3.21);
	GG.addEdge(5,1,6.51);
	GG.writeToFile("test_add-edge.txt");
}


TEST_CASE("tree()", "Is tree") {
	Graph G(UNDIRECTED);
	for (size_t i = 0; i < 3; ++i) {
		G.addVertex();
	}
	G.addEdge(1,2,3.12);
	G.addEdge(1,3,4.13);
	REQUIRE(G.tree());
	G.addVertex();
	REQUIRE(!G.tree());

	Graph DG(DIRECTED);
	for (size_t i = 0; i < 3; ++i) {
		DG.addVertex();
	}
	DG.addEdge(1,2,3.12);
	DG.addEdge(1,3,4.13);
	REQUIRE(DG.tree());
	DG.addEdge(3,2,5.32);
	REQUIRE(!DG.tree());
	DG.addEdge(3,1,4.31);
	REQUIRE(!DG.tree());

	Graph G1(UNDIRECTED);
	G1.readFromFile("g1.txt");
	REQUIRE(!G1.tree());

	Graph G2(DIRECTED);
	G2.readFromFile("g2.txt");
	REQUIRE(!G2.tree());
}

TEST_CASE("DFT(int, std::string)", "Depth Frist Traverse") {
	Graph G(UNDIRECTED);
	G.readFromFile("g1.txt");
	G.writeToFile("g1_output");
	G.DFT(1, "g1-dft1.txt");

	Graph G2(DIRECTED);
	G2.readFromFile("g2.txt");
	G2.writeToFile("g2_output.txt");
	G2.DFT(2, "g2-dft2.txt");
	G2.DFT(4, "g2-dft4.txt");
}

TEST_CASE("BFT(int, std::string)", "Breadth Frist Traverse") {
	Graph G(UNDIRECTED);
	G.readFromFile("g1.txt");
	G.writeToFile("g1_output");
	G.BFT(2, "g1-bft2.txt");

	Graph G2(DIRECTED);
	G2.readFromFile("g2.txt");
	G2.writeToFile("g2_output.txt");
	G2.BFT(7, "g2-bft7.txt");
}

TEST_CASE("numConnectedComponents()", "Number of Connected Components") {
	Graph G(UNDIRECTED);
	G.readFromFile("g1.txt");
	REQUIRE(G.numConnectedComponents() == 2);

	Graph G2(DIRECTED);
	G2.readFromFile("g2.txt");
	REQUIRE(G2.numConnectedComponents() == 1);
}

TEST_CASE("bool partitionable()", "Is partitionable") {
	Graph G(UNDIRECTED);
	G.readFromFile("g1.txt");
	REQUIRE_FALSE(G.partitionable());

	Graph G2(DIRECTED);
	G2.readFromFile("g2.txt");
	REQUIRE(G2.partitionable());
}


TEST_CASE("MST(std::string)", "Minimum spanning tree") {
	Graph G(UNDIRECTED);
	G.readFromFile("g1.txt");
	REQUIRE(G.MST("g1-mst.txt"));

	Graph G2(DIRECTED);
	G2.readFromFile("g2.txt");
	REQUIRE(G2.MST("g2-mst.txt"));
}
