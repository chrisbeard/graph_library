#include "Graph.h"
#include <fstream>
#include <queue>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

// Construct an empty graph of the specified type
Graph::Graph(Type t) {
	directed = t == DIRECTED;
	edgeList = std::vector<Edge *>(1, nullptr);
	number_of_edges = 0;
}
		
// Delete a graph
Graph::~Graph() {
	std::set<const Graph::Edge *> edges = allEdges();
	for (auto a = edges.begin(); a != edges.end(); ++a) {
		delete *a;
	}
	return;
}

std::set<const Graph::Edge *> Graph::allEdges(void) const {
	std::set<const Graph::Edge *> edges;
	for (size_t i = 1; i < edgeList.size(); ++i) {
		Graph::Edge *e = edgeList[i];
		while (e) {
			edges.insert(e);
			e = e->left ?: e->right;
		}
	}

	return edges;
}
		
// Read a graph from a file
void Graph::readFromFile(std::string file) {
    std::ifstream inputFile(file);
	if (!inputFile) {
		std::cerr << "Could not open input file.\n";
		return;
	}
	
	
	std::string graphdef;

	std::getline(inputFile, graphdef);
	std::stringstream stream(graphdef);
	stream >> graphdef;
	if (graphdef == "directed") {
		directed = true;
	} else if (graphdef == "undirected") {
		directed = false;
	} else {
		std::cerr << "Invalid graph input. Need direction.\n";
		return;
	}

	// Number of Vertices
	size_t numberVertices = 0;
	std::string line;
	std::getline(inputFile, line);
	stream.str(line);
	stream.clear();
	if (!(stream >> numberVertices)) {
		std::cerr << "Invalid graph input. Need number of vertices.\n";
		return;
	}
	for (size_t i = 0; i < numberVertices; ++i) {
		addVertex();
	}

	// Number of Edges
	size_t numberEdges = 0;
	std::getline(inputFile, line);
	stream.str(line);
	stream.clear();
	if (!(stream >> numberEdges)) {
		std::cerr << "Invalid graph input. Need number of edges.\n";
		return;
	}

	while (std::getline(inputFile, line)) {
		std::stringstream stream(line);	
		int node1, node2;
		double weight;
		if (!(stream >> node1)) {
			std::cerr << "Invalid file format\n";
			return;
		}
		if (!(stream >> node2)) {
			std::cerr << "Invalid file format\n";
			return;
		}
		if (!(stream >> weight)) {
			std::cerr << "Invalid file format\n";
			return;
		}

		addEdge(node1, node2, weight);
	}
}
		
// Write a graph to a file
void Graph::writeToFile(std::string file) {
	std::ofstream outputFile(file.c_str());
	
	if (!outputFile) {
		std::cerr << "Invalid file output.\n";
		return;
	}

	if (directed) {
		outputFile << "directed\n";
	} else {
		outputFile << "undirected\n";
	}
	outputFile << edgeList.size()-1 << "\n";
	outputFile << number_of_edges << "\n";

	std::set<const Graph::Edge *> edges = allEdges();
	for (auto a = edges.begin(); a != edges.end(); ++a) {
		if (!(*a)) {
			continue;
		}

		if (!directed) {
			outputFile << (*a)->v1 << " " << (*a)->v2 << " " << (*a)->weight1 << "\n";
		}
		else if (directed && (*a)->direction == 0) {
			outputFile << (*a)->v1 << " " << (*a)->v2 << " " << (*a)->weight1 << "\n";
			outputFile << (*a)->v2 << " " << (*a)->v1 << " " << (*a)->weight2 << "\n";
		}
		else if (directed && (*a)->direction == 1) {
			outputFile << (*a)->v1 << " " << (*a)->v2 << " " << (*a)->weight1 << "\n";
		}
		else {
			outputFile << (*a)->v2 << " " << (*a)->v1 << " " << (*a)->weight2 << "\n";
		}
	}
}
		
// Empty
bool Graph::empty() {
	return (edgeList.size() < 2);
}

// Add an edge to the edge list
void Graph::addEdge(int v1, int v2, double weight) {
	if ((size_t)v1 >= edgeList.size() || (size_t)v2 >= edgeList.size()) {
		throw std::invalid_argument("Given vertices must exist in the graph.");
	}

	int node1 = std::min(v1, v2);
	int node2 = std::max(v1, v2);
	double weight1 = 0;
	double weight2 = 0;
	size_t direction = 0;
	if (directed) {
		if (node1 == v1) {
			weight1 = weight;
			direction = 1;
		}
		else {
			weight2 = weight;
			direction = 2;
		}
	} else {
		weight1 = weight;
		weight2 = weight;
	}
	
	// Allocates an Edge on the heap
	// Edge is inserted at the front of the edge list for both node values
	// The left & right pointers are adjusted to point to the previous first edges in the list
	Edge *e = new Edge(node1, node2, weight1, weight2, direction, edgeList[node1], edgeList[node2]);
	edgeList[node1] = e;
	edgeList[node2] = e;
	++number_of_edges;
}
		
// Add a vertex to the head of the edge list
void Graph::addVertex() {
	// All we need to do is allocated space for an edge
	// There is no need to keep track of node values
	edgeList.push_back(nullptr);
}
		
// Count connected components
int Graph::numConnectedComponents() {
	return -1;
}
		
// Tree check
bool Graph::tree() {
//	std::cout << "Size: " << edgeList.size()-1 << " Edges: " << number_of_edges << "\n";
	// Undirected case
	if (!directed && (edgeList.size() - 2) != number_of_edges) {
		return false;
	}
	if (directed && (number_of_edges >= edgeList.size() - 1)) {
		return false;
	}

	return true;
}

// Depth First Traverse - proceed from source
void Graph::DFT(int source, std::string file) {
	// keeps track of which nodes have been visited
	std::vector<int> visited = std::vector<int>(edgeList.size(), 0);
	// keeps track of the order in which the nodes are visited
	std::queue<int> order;
	if(!directed) {
		DFUndirected(source, visited, order);			
	} else {
		DFDirected(source, visited, order);
	}
	// print results to file
	std::ofstream outfile(file, std::ofstream::out);
	if (outfile.is_open()) {
		while (!order.empty()) {
			outfile << order.front() << "\n";
			order.pop();
		}
		outfile.close();
	} else {
		std::cerr << "Could not open input file.\n";
	}
}

// Performs the DFT for an undirected graph
void Graph::DFUndirected(int node, std::vector<int> &vlist, std::queue<int>& olist) {
	Edge *edgePtr = edgeList[node];
	vlist[node] = 1;

	while (edgePtr != nullptr) {
		if (!vlist[edgePtr->v2]) {
			DFUndirected(edgePtr->v2, vlist, olist);
		} else if (!vlist[edgePtr->v1]) {
			DFUndirected(edgePtr->v1, vlist, olist);
		}
		edgePtr = edgePtr->v1 == node ? edgePtr->left : edgePtr->right;
	}

	olist.push(node);
}

// Performs the DFT for a directed graph
void Graph::DFDirected(int node, std::vector<int> &vlist, std::queue<int>& olist) {
	Edge *edgePtr = edgeList[node];
	vlist[node] = 1;

	while(edgePtr != nullptr) {
		if (edgePtr->direction == 1 && !vlist[edgePtr->v2]) {
			DFDirected(edgePtr->v2, vlist, olist);
		} else if (edgePtr->direction == 2 && !vlist[edgePtr->v1]) {
			DFDirected(edgePtr->v1, vlist, olist);
		}
		edgePtr = edgePtr->v1 == node ? edgePtr->left : edgePtr->right;
	}

	olist.push(node);
}

// Breadth First search - proceed from source
void Graph::BFT(int source, std::string file) {
	// keeps track of which nodes have been visited
	std::vector<int> visited = std::vector<int>(edgeList.size(), 0);
	// keeps track of the order in which the nodes are visited
	std::queue<int> order;
	if(!directed) {
		BFUndirected(source, visited, order);			
	} else {
		BFDirected(source, visited, order);
	}
	// print results to file
	std::ofstream outfile(file, std::ofstream::out);
	if (outfile.is_open()) {
		while (!order.empty()) {
			outfile << order.front() << "\n";
			order.pop();
		}
		outfile.close();
	} else {
		std::cerr << "Could not open input file.\n";
	}
}

void Graph::BFUndirected(int node, std::vector<int> &vlist, std::queue<int> &olist) {
	std::queue<int> nodeQueue;
	nodeQueue.push(node);
	while (!nodeQueue.empty()) {
		int node = nodeQueue.front();
		nodeQueue.pop();
		if (vlist[node] == 1) {
			return;
		}
		vlist[node] = 1;

		Edge *edge = edgeList[node];
		while (edge) {
			if (edge->v1 == node && !vlist[edge->v2]) {
				nodeQueue.push(edge->v2);
			} else if (edge->v2 == node && !vlist[edge->v1]) {
				nodeQueue.push(edge->v1);
			}

			edge = edge->v1 == node ? edge->left : edge->right;
		}

		olist.push(node);
	}
}

void Graph::BFDirected(int node, std::vector<int> &vlist, std::queue<int> &olist) {
	std::queue<int> nodeQueue;
	nodeQueue.push(node);
	while (!nodeQueue.empty()) {
		int node = nodeQueue.front();
		nodeQueue.pop();
		if (vlist[node] == 1) {
			return;
		}
		vlist[node] = 1;

		Edge *edge = edgeList[node];
		while (edge) {
			if (edge->v1 == node && !vlist[edge->v2] && edge->direction == 1) {
				nodeQueue.push(edge->v2);
			} else if (edge->v2 == node && !vlist[edge->v1] && edge->direction == 2) {
				nodeQueue.push(edge->v1);
			}

			edge = edge->v1 == node ? edge->left : edge->right;
		}

		olist.push(node);
	}
}

/*		
		
// Closeness - determine minimum number of edges to get
//  from one node to the other
int Graph::closeness(int v1, int v2) {
	return -1;
}
*/
// Partition - determine if you can partition the graph
bool Graph::partitionable() {
	// 0 = not set, 1 = group 1, 2 = group 2;
	std::vector<int> group = std::vector<int>(edgeList.size(), 0); 
	for(size_t i = 0; i < edgeList.size(); i++){
		if(group[i] == 0){ // if this node has not been set
			group[i] = 1;
		}
		Edge* edgePtr = edgeList[i];
		while(edgePtr != nullptr){ // for all connected nodes
			if(edgePtr->v1 == group[i]){ 
				// if we can get to v2
				if(edgePtr->direction != 2){
					// if graph is not partitionable
					if(group[edgePtr->v2] == group[i]){
						return false;
					}	
					//if node has not been set
					else if(group[edgePtr->v2] == 0){
						group[edgePtr->v2] = ((group[i] + 1) % 2);
					}
				}
				edgePtr = edgePtr->left;
			}
			else{ // if this is v2
				// if we can get to v1
				if(edgePtr->direction != 1){
					//if the graph is not partionable
					if(group[edgePtr->v1] == group[i]){
						return false;
					}
					//if node has not been set
					else if(group[edgePtr->v1] == 0){
						group[edgePtr->v1] = ((group[i] + 1) % 2);
					}
				}
				edgePtr = edgePtr->right;
			}
		}
	}
	return true; // if it makes it to this point, the graph is partitionable
}

/*		
// * MST - print the minimum spanning tree of the graph
// to a file with the passed name
// Prim's - number of edges closer to V^2
// Kruskal's - otherwise
bool Graph::MST(std::string file) {
	return false;
}
		
// * Step Away - print the nodes who are a degree of
// closeness from the source to a file with the passed name
void Graph::stepAway(int source, int closeness, std::string file) {
	return;
}
*/
