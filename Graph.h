#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <vector>
#include <queue>

//This class will be used to create a graph library.
enum Type {DIRECTED, UNDIRECTED};

class Graph {
	private:
		struct Edge {
			Edge(int _v1, int _v2, double _weight1, double _weight2, int _direction, Edge *_left, Edge *_right) : v1(_v1), v2(_v2), weight1(_weight1), weight2(_weight2), direction(_direction), left(_left), right(_right) {}
			int v1;			// v1 = min(v1, v2)
			int v2;			// v2 = max(v1, v2)
			double weight1;	// for v1->v2
			double weight2;	// for v2->v1
			int direction;	// 0 = both, 1 = v1->v2, 2 = v2->v1

			// Edge List pointers
			Edge *left;
			Edge *right;
		};

		std::set<const Graph::Edge *> allEdges(void) const;

		std::vector<Edge*> edgeList;
		size_t number_of_edges;

		bool directed;

		void DFUndirected(int node, std::vector<int> &vlist, std::queue<int> &oList);
		void DFDirected(int node, std::vector<int> &vlist, std::queue<int> &oList);

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
		void DFT(int source, std::string file);
		// Breadth First Traverse - proceed from source
		void BFT(int source, std::string file);
		// Closeness - determine minimum number of edges to get
		// from one node to the other
		int closeness(int v1, int v2);
		// * Partition - determine if you can partition the graph
		bool partitionable();
		// * MST - print the minimum spanning tree of the graph
		// to a file with the passed name (return whether or not
        // this operation was successful)
		bool MST(std::string file);
		// * Step Away - print the nodes who are a degree of
		// closeness from the source to a file with the passed name
		void stepAway(int source, int closeness, std::string file);
};

#endif 
