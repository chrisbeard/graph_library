#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <queue>
#include "Graph.h"

// Construct an empty graph of the specified type
Graph::Graph(Type t) {
	directed = (t == DIRECTED);
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
			e = e->link[LEFT] ?: e->link[Right];
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
	std::stringstream stream;
	std::string graphdef;
	std::string line;
	int node1, node2 = 0;
	double weight = 0.0;
	std::getline(inputFile, graphdef);
	if (graphdef == "directed") {
		directed = true;
	}
	else if (graphdef == "undirected") {
		directed = false;
	}
	size_t numberVertices = 0;
	std::getline(inputFile, numberVertices);
	for (size_t i = 0; i <= numberVertices; ++i) {
		addVertex();
	}
	while (std::getline(inputFile, line)) {
		stream(line);		
		stream >> node1;
		stream >> node2;
		stream >> weight;
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
	}
	else {
		outputFile << "undirected\n";
	}
	outputFile << edgeList.size()-1 << "\n";
	std::set<const Graph::Edge *> edges = allEdges();
	for (auto a = edges.begin(); a != edges.end(); ++a) {
		if (!(*a)) {
			continue;
		}
		if (!directed) {
			outputFile << (*a)->vertex[LEFT] << " " << (*a)->vertex[RIGHT] << " " << (*a)->weight[BOTH] << "\n";
		}
		else if (directed && (*a)->direction == 0) {
			outputFile << (*a)->vertex[LEFT] << " " << (*a)->vertex[RIGHT] << " " << (*a)->weight[LEFT] << "\n";
			outputFile << (*a)->vertex[RIGHT] << " " << (*a)->vertex[LEFT] << " " << (*a)->weight[RIGHT] << "\n";
		}
		else if (directed && (*a)->direction == 1) {
			outputFile << (*a)->vertex[LEFT] << " " << (*a)->vertex[RIGHT] << " " << (*a)->weight[LEFT] << "\n";
		}
		else {
			outputFile << (*a)->vertex[RIGHT] << " " << (*a)->vertex[LEFT] << " " << (*a)->weight[RIGHT] << "\n";
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
			direction = LEFT;
		}
		else {
			weight2 = weight;
			direction = RIGHT;
		}
	}
	else {
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
	sortEdges(node1);
	sortEdges(node2);
}

//used to sort the edges as they are added to the edge list
void Graph::sortEdges(int node) {
	Edge *edgePtr = edgeList[node];
	Edge *edgeNext = edgeList[node];
	edge *edgePrev = edgeList[node];
	int index, index2, index3 = 0;
	bool isFirst = true;
	bool isSorted = false;
	//sort in ascending order
	while(!isSorted){
		if(edgePtr->vertex[LEFT] == node){
			edgeNext = edgePtr->link[LEFT];
			index = RIGHT;
		}
		else{
			edgeNext = edgePtr->link[RIGHT];
			index = LEFT;
		}
		//if we've reached the end of the list
		if(edgeNext == nullptr){
			break;
		}
		index2 = (edgeNext->vertex[LEFT] == i ? RIGHT : LEFT);
		if(edgePtr->vertex[index] > edgeNext->vertex[index2]){
			//swap index values
			index = (index == LEFT ? RIGHT : LEFT);
			index2 = (index2 == LEFT ? RIGHT : LEFT);	
			//adjust pointers
			edgePtr->link[index] = edgeNext->line[index2];
			edgeNext->link[index2] = edgePtr;
			if(isFirst){
				edgeList[node] = edgeNext;
				isFirst = false;		
			}
			else{
				index3 = (edgePrev->vertex[LEFT] == i ? LEFT : RIGHT);
				edgePrev->link[LEFT] = edgeNext;
			}
		}
		//if this edge has the smaller vertex, the list is sorted
		else{
			isSorted = true;
		}
		//if we swapped the next node will now be the previous
		//otherwise, we will exit the loop anyway
		edgePrev = edgeNext;
	}
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
	bool retval = true;
	//keeps track of which nodes have been visited
	std::vector<int> visited = std::vector<int>(edgeList.size(), 0);
	//keeps track of the order in which the nodes are visited
	std::queue<int> order;
	//check if tree is connected and acyclic	
	treeHelper(0, visited);			
	for(int i = 0; i < visited.size(); i++){
		if(visited[i] != 1){
			retval = false;
			break;
		}
	}
	return retval;
}

//Performs the DFT to check if the graph is a tree
void Graph::treeHelper(int source, std::vector<int> &vlist) {
	Edge *edgePtr = edgeList[source];
	//mark node as visited
	if(vlist[node] >= 1){
		vlist = 2;
	}
	else{
		vlist[node] = 1;
	}
	//while there are still connected nodes
	while (edgePtr != nullptr) {
		//if this is the left vertex
		if (edgePtr->vertex[LEFT] == node) {
			//if the right vertex has not been visited
			if (!vlist[edgePtr->vertex[RIGHT]]){
				treeHelper(edgePtr->vertex[RIGHT], vlist);
			}
			else {
				//increment pointer
				edgePtr = edgePtr->link[LEFT];
			}
		}
		//if v1 has not been visited
		else if (!vlist[edgePtr->vertex[LEFT]]) {
			treeHelper(edgePtr->vertex[LEFT], vlist);	
		}
		else {
			//increment pointer
			edgePtr = edgePtr->link[RIGHT];
		}
	}
	return;
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
		//if this is the left vertex
		if (edgePtr->vertex[LEFT] == node) {
			//if the right vertex has not been visited
			if (!vlist[edgePtr->vertex[RIGHT]]){
				DFUndirected(edgePtr->vertex[RIGHT], vlist, oList);
			}
			else {
				//increment pointer
				edgePtr = edgePtr->link[LEFT];
			}
		}
		//if v1 has not been visited
		else if (!vlist[edgePtr->vertex[LEFT]]) {
			DFUndirected(edgePtr->vertex[LEFT], vlist, oList);	
		}
		else {
			//increment pointer
			edgePtr = edgePtr->link[RIGHT];
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
		if(edgePtr->direction == LEFT) {
			//if this is v1
			if(edgePtr->vertex[LEFT] == node) {
				//if v2 has not been visited
				if(!vlist[edgePtr->vertex[RIGHT]]){
					DFUndirected(edgePtr->vertex[RIGHT], vlist, oList);
				}
				else {
					//increment pointer
					edgePtr = edgePtr->link[LEFT];
				}
			}
			else {
				//increment pointer
				edgePtr = edgePtr->link[RIGHT];
			}
		}
		//if direction == 2
		else {
			//if this is v2
			if (edgePtr->vertex[RIGHT] == node){
				//if v1 has not been visited
				if (!vlist[edgePtr->vertex[LEFT]]){
					DFUndirected(edgePtr->vertex[LEFT], vlist, oList);
				}
				else {
					//increment pointer
					edgePtr = edgePtr->link[RIGHT];
				}
			}
			else {
				//increment pointer
				edgePtr = edgePtr->link[LEFT];
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
			if(edgePtr->vertex[LEFT] == group[i]){ 
				// if we can get to v2
				if(edgePtr->direction != RIGHT){
					// if graph is not partitionable
					if(group[edgePtr->vertex[RIGHT]] == group[i]){
						return false;
					}	
					//if node has not been set
					else if(group[edgePtr->vertex[RIGHT]] == 0){
						group[edgePtr->vertex[RIGHT]] = ((group[i] + 1) % 2);
					}
				}
				edgePtr = edgePtr->link[LEFT];
			}
			else{ // if this is v2
				// if we can get to v1
				if(edgePtr->direction != LEFT){
					//if the graph is not partionable
					if(group[edgePtr->vertex[LEFT]] == group[i]){
						return false;
					}
					//if node has not been set
					else if(group[edgePtr->vertex[LEFT]] == 0){
						group[edgePtr->vertex[LEFT]] = ((group[i] + 1) % 2);
					}
				}
				edgePtr = edgePtr->link[RIGHT];
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
