// Put any include statements here
#include <string>
#include <iostream>
#include <vector>

// This class will be used to create a graph library.
enum Type {DIRECTED, UNDIRECTED};

// stores edges for edge list
struct Edge {
	int vertex[2];
	Edge* link[2];
	double weight[2];
	size_t direction; // 0 = both, 1 = v[0]->v[1], 2 = v[1]->v[0]
};

typedef Edge* EdgePointer;

// used for head node of edge list
struct Vertex {
	int value;
	EdgePointer first;
};

class Graph{
	private:
		// directed or undirected
		Type gType;
		vector<Vertex> edgeList;
	public:
		// Construct an empty graph of the specified type
		Graph(Type t);
		// Delete a graph
		~Graph();
		// Read a graph from a file
		void readFromFile(std::string file);
		// Write a graph to a file
		void writeToFile(std::string file);
		// Empty
		bool empty();
		// Add edge
		void addEdge(int v1, int v2, double weight);
		// Add vertex
		void addVertex();
		// Count connected components
		int numConnectedComponents();
		// Tree check
		bool tree();
		// Depth First Traverse - proceed from source
		void DFT(int source);
		// Breadth First Traverse - proceed from source
		void BFT(int source);
		// Closeness - determine minimum number of edges to get
		//  from one node to the other
		int closeness(int v1, int v2);
		// * Partition - determine if you can partition the graph
		bool partitionable();
		// * MST - print the minimum spanning tree of the graph
		// to a file with the passed name
		bool MST(std::string file);
		// * Step Away - print the nodes who are a degree of
		// closeness from the source to a file with the passed name
		void stepAway(int source, int closeness, std::string file);
};
