#ifndef GRAPH_H
#define GRAPH_H

#include <queue>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <functional>

//This class will be used to create a graph library.
enum Type {DIRECTED, UNDIRECTED};
enum Direction {BOTH, LEFT, RIGHT};

class Graph {
	private:
		// Struct used to represent edges in edge list
		struct Edge {
			Edge(int v1, int v2, double w1, double w2, int d, Edge *l, Edge *r) {
				vertex[LEFT] = v1;
				vertex[RIGHT] = v2;
				weight[LEFT] = w1;
				weight[RIGHT] = w2;
				direction = d;
				link[LEFT] = l;
				link[RIGHT] = r;
			}
			int vertex[3];
			Edge* link[3];
			double weight[3];
			size_t direction;
		};
	
		std::vector<Edge*> edgeList;
		std::set<const Graph::Edge *> allEdges(void) const;	

		bool directed;
		size_t number_of_edges;

		void DFUndirected(int node, std::vector<int> &vlist, std::queue<int> &oList);
		void DFDirected(int node, std::vector<int> &vlist, std::queue<int> &oList);
		void BFUndirected(int node, std::vector<int> &vlist, std::queue<int> &oList);
		void BFDirected(int node, std::vector<int> &vlist, std::queue<int> &oList);
		void treeHelper(int source, std::vector<int> &vlist);
		void sortEdges(int node);
		void undirectedClose(int v1, std::vector<int> &distance, std::set<int> unvisited);
		void directedClose(int v1, int v2, std::vector<int> &dlist, std::queue<int> &alist);
		void breadthFirstApply(std::vector<bool> &visited, int source, const std::function<bool(int)> &lambda, bool ignoreDirections);
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
