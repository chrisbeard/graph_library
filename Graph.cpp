#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <queue>
#include "Graph.h"

// Construct an empty graph of the specified type
Graph::Graph(Type t) {
	directed = t == DIRECTED;
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

    std::ifstream inputFile(file.c_str());

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

	size_t numberVertices = 0;
	std::string line;
	std::getline(inputFile, line);
	stream.str(line);
	stream.clear();
	if (!(stream >> numberVertices)) {
		std::cerr << "Invalid graph input. Need number of vertices.\n";
		return;
	}
	for (size_t i = 0; i <= numberVertices; ++i) {
		addVertex();
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
		std::cout << node1 << " " << node2 << " " << weight << "\n";
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
	return !edgeList.size();
}
		
// Add an edge to the edge list
void Graph::addEdge(int v1, int v2, double weight) {
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

	// Need to make sure the list can hold largest node
	for (size_t i = edgeList.size(); i < (unsigned)std::max(v1,v2); ++i) {
		addVertex();
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
	return false;
}

// Depth First Traverse - proceed from source
void Graph::DFT(int source, std::string file) {
	//keeps track of which nodes have been visited
	std::vector<int> visited = std::vector<int>(edgeList.size(), 0);
	//keeps track of the order in which the nodes are visited
	std::queue<int> order;
	if(!directed) {
		DFUndirected(source, visited, order);			
	}
	else {
		DFDirected(source, visited, order);
	}
	//print results to file
	std::ofstream outfile(file, std::ofstream::out);
	if (outfile.is_open()) {
		while (!order.empty()) {
			outfile << order.front();
			order.pop();
		}
		outfile.close();
	}
	//if the file could not be opened
	else{
		std::cerr << "Could not open input file.\n";
	}
}

//Performs the DFT for an undirected graph
void Graph::DFUndirected(int node, std::vector<int> &vlist, std::queue<int>& oList) {
	Edge *edgePtr = edgeList[node];
	//mark node as visited
	vlist[node] = 1;
	oList.push(node);
	//while there are still connected nodes
	while (edgePtr != nullptr) {
		//if this is v1
		if (edgePtr->v1 == node) {
			//if v2 has not been visited
			if (!vlist[edgePtr->v2]){
				DFUndirected(edgePtr->v2, vlist, oList);
			}
			else {
				//increment pointer
				edgePtr = edgePtr->left;
			}
		}
		//if v1 has not been visited
		else if (!vlist[edgePtr->v1]) {
			DFUndirected(edgePtr->v1, vlist, oList);	
		}
		else {
			//increment pointer
			edgePtr = edgePtr->right;
		}
	}

	return;
}

//Performs the DFT for a directed graph
void Graph::DFDirected(int node, std::vector<int> &vlist, std::queue<int>& oList) {
	Edge *edgePtr = edgeList[node];
	//mark node as visited
	vlist[node] = 1;
	oList.push(node);
	//while there are still connected nodes
	while(edgePtr != nullptr) {
		if(edgePtr->direction == 1) {
			//if this is v1
			if(edgePtr->v1 == node) {
				//if v2 has not been visited
				if(!vlist[edgePtr->v2]){
					DFUndirected(edgePtr->v2, vlist, oList);
				}
				else {
					//increment pointer
					edgePtr = edgePtr->left;
				}
			}
			else {
				//increment pointer
				edgePtr = edgePtr->right;
			}
		}
		//if direction == 2
		else {
			//if this is v2
			if (edgePtr->v2 == node){
				//if v1 has not been visited
				if (!vlist[edgePtr->v1]){
					DFUndirected(edgePtr->v1, vlist, oList);
				}
				else {
					//increment pointer
					edgePtr = edgePtr->right;
				}
			}
			else {
				//increment pointer
				edgePtr = edgePtr->left;
			}
		}
	}
	return;
}

/*		
// Breadth First se - proceed from source
void Graph::BFT(int source, std::string file) {
	return;
}
		
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
