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
			e = e->link[LEFT] ?: e->link[RIGHT];
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
			outputFile << (*a)->vertex[LEFT] << " " << (*a)->vertex[RIGHT] << " " << (*a)->weight[LEFT] << "\n";
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
	return (edgeList.size() < 2);
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
	Edge *e = new Edge(node1, node2, weight1, weight2, direction, nullptr, nullptr);
	number_of_edges++;

	if (!edgeList[node1]) {
		edgeList[node1] = e;
	} else {
		Edge *current = edgeList[node1];
		while (current) {
			if (current->vertex[LEFT] == e->vertex[LEFT]) {
				if (current->link[LEFT] && current->link[LEFT]->vertex[RIGHT] >= e->vertex[RIGHT]) {
					e->link[LEFT] = current->link[LEFT];
					if (edgeList[node1] == current) {
						edgeList[node1] = e;
					}

					break;
				} else if (current->link[LEFT] && current->link[LEFT]->vertex[RIGHT] < e->vertex[RIGHT]) {
					current = current->link[LEFT];
				} else {
					current->link[LEFT] = e;
					break;
				}
			} else {
				if (current->link[RIGHT] && current->link[RIGHT]->vertex[RIGHT] >= e->vertex[RIGHT]) {
					e->link[LEFT] = current->link[LEFT];
					if (edgeList[node2] == current) {
						edgeList[node2] = e;
					}

					break;
				} else if (current->link[RIGHT] && current->link[RIGHT]->vertex[RIGHT] < e->vertex[RIGHT]) {
					current = current->link[RIGHT];
				} else {
					current->link[RIGHT] = e;
					break;
				}
			}
		}
	}

	if (!edgeList[node2]) {
		edgeList[node2] = e;
	} else {
		Edge *current = edgeList[node2];
		while (current) {
			if (current->vertex[RIGHT] == e->vertex[RIGHT]) {
				if (current->link[RIGHT] && current->link[RIGHT]->vertex[LEFT] >= e->vertex[LEFT]) {
					e->link[RIGHT] = current->link[RIGHT];
					if (edgeList[node2] == current) {
						edgeList[node2] = e;
					}

					break;
				} else if (current->link[RIGHT] && current->link[RIGHT]->vertex[LEFT] < e->vertex[LEFT]) {
					current = current->link[RIGHT];
				} else {
					current->link[RIGHT] = e;
					break;
				}
			} else {
				if (current->link[LEFT] && current->link[LEFT]->vertex[LEFT] >= e->vertex[LEFT]) {
					e->link[RIGHT] = current->link[RIGHT];
					if (edgeList[node1] == current) {
						edgeList[node1] = e;
					}

					break;
				} else if (current->link[LEFT] && current->link[LEFT]->vertex[LEFT] < e->vertex[LEFT]) {
					current = current->link[LEFT];
				} else {
					current->link[LEFT] = e;
					break;
				}
			}
		}
	}
}


//used to sort the edges as they are added to the edge list
// void Graph::sortEdges(int node) {
// 	Edge *edgePtr = edgeList[node];
// 	Edge *edgeNext = edgeList[node];
// 	Edge *edgePrev = edgeList[node];
// 	int index, index2 = 0;
// 	bool isFirst = true;
// 	bool isSorted = false;
// 	//sort in ascending order
// 	while(!isSorted) {
// 		if (edgePtr->vertex[LEFT] == node) {
// 			edgeNext = edgePtr->link[LEFT];
// 			index = RIGHT;
// 		}
// 		else {
// 			edgeNext = edgePtr->link[RIGHT];
// 			index = LEFT;
// 		}
// 		//if we've reached the end of the list
// 		if (edgeNext == nullptr) {
// 			break;
// 		}
// 		index2 = (edgeNext->vertex[LEFT] == node ? RIGHT : LEFT);
// 		if (edgePtr->vertex[index] > edgeNext->vertex[index2]) {
// 			//swap index values
// 			index = (index == LEFT ? RIGHT : LEFT);
// 			index2 = (index2 == LEFT ? RIGHT : LEFT);	
// 			//adjust pointers
// 			edgePtr->link[index] = edgeNext->link[index2];
// 			edgeNext->link[index2] = edgePtr;
// 			if (isFirst) {
// 				edgeList[node] = edgeNext;
// 				isFirst = false;		
// 			}
// 			else {
// 				// index3 = (edgePrev->vertex[LEFT] == node ? LEFT : RIGHT);
// 				edgePrev->link[LEFT] = edgeNext;
// 			}
// 		}
// 		//if this edge has the smaller vertex, the list is sorted
// 		else {
// 			isSorted = true;
// 		}
// 		//if we swapped the next node will now be the previous
// 		//otherwise, we will exit the loop anyway
// 		edgePrev = edgeNext;
// 	}
// }
		
// Add a vertex to the head of the edge list
void Graph::addVertex() {
	// All we need to do is allocated space for an edge
	// There is no need to keep track of node values
	edgeList.push_back(nullptr);
}
		
// Count connected components
int Graph::numConnectedComponents() {
	size_t components = 0;
	bool done = false;

	std::vector<int> visited = std::vector<int>(edgeList.size(), 0);
	std::queue<int> order;
	while (!done) {
		int start = 0;
		done = true;
		for (size_t i = 0; i < visited.size(); i ++) {
			if (visited[i] == 0 && edgeList[i]) {
				done = false;
				start = i;
				break;
			}
		}
		if (!done) {
			// Undirected search is what we want here - direction doesn't matter
			BFUndirected(start, visited, order);
			++components;
		}
	}

	return components;
}
		
// Tree check
bool Graph::tree() {
//	std::cout << "Size: " << edgeList.size()-1 << " Edges: " << number_of_edges << "\n";
	// Undirected case
	// std::cout << "Size: " << edgeList.size() - 1 << " Edges: " << number_of_edges << "\n";

	// if (!directed && (edgeList.size() - 2) != number_of_edges) {
	// 	return false;
	// }
	if (directed && (number_of_edges >= edgeList.size() - 1)) {
		return false;
	}
	
	bool retval = true;
	//keeps track of which nodes have been visited
	std::vector<int> visited = std::vector<int>(edgeList.size(), 0);
	//keeps track of the order in which the nodes are visited
	std::queue<int> order;
	//check if tree is connected and acyclic	
	treeHelper(1, visited);			
	for (size_t i = 1; i < visited.size(); i++) {
		if (visited[i] != 1) {
			retval = false;
			break;
		}
	}
	return retval;
}

// Performs the DFT to check if the graph is a tree
void Graph::treeHelper(int node, std::vector<int> &vlist) {
	Edge *edgePtr = edgeList[node];
	// mark node as visited
	if (vlist[node] >= 1) {
		vlist[node] = 2;
	}
	else {
		vlist[node] = 1;
	}

	// while there are still connected nodes
	while (edgePtr != nullptr) {
		// if this is the left vertex
		if (edgePtr->vertex[LEFT] == node) {
			// if the right vertex has not been visited
			if (!vlist[edgePtr->vertex[RIGHT]]) {
				treeHelper(edgePtr->vertex[RIGHT], vlist);
			}
			else {
				// increment pointer
				edgePtr = edgePtr->link[LEFT];
			}
		}
		// if vertex[LEFT] has not been visited
		else if (!vlist[edgePtr->vertex[LEFT]]) {
			treeHelper(edgePtr->vertex[LEFT], vlist);	
		}
		else {
			// increment pointer
			edgePtr = edgePtr->link[RIGHT];
		}
	}
	return;
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
		if (!vlist[edgePtr->vertex[RIGHT]]) {
			DFUndirected(edgePtr->vertex[RIGHT], vlist, olist);
		} else if (!vlist[edgePtr->vertex[LEFT]]) {
			DFUndirected(edgePtr->vertex[LEFT], vlist, olist);
		}
		edgePtr = edgePtr->vertex[LEFT] == node ? edgePtr->link[LEFT] : edgePtr->link[RIGHT];
	}

	olist.push(node);
}

// Performs the DFT for a directed graph
void Graph::DFDirected(int node, std::vector<int> &vlist, std::queue<int>& olist) {

	Edge *edgePtr = edgeList[node];
	vlist[node] = 1;

	while(edgePtr != nullptr) {
		if (edgePtr->direction == 1 && !vlist[edgePtr->vertex[RIGHT]]) {
			DFDirected(edgePtr->vertex[RIGHT], vlist, olist);
		} else if (edgePtr->direction == 2 && !vlist[edgePtr->vertex[LEFT]]) {
			DFDirected(edgePtr->vertex[LEFT], vlist, olist);
		}
		edgePtr = edgePtr->vertex[LEFT] == node ? edgePtr->link[LEFT] : edgePtr->link[RIGHT];
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
			if (edge->vertex[LEFT] == node && !vlist[edge->vertex[RIGHT]]) {
				nodeQueue.push(edge->vertex[RIGHT]);
			} else if (edge->vertex[RIGHT] == node && !vlist[edge->vertex[LEFT]]) {
				nodeQueue.push(edge->vertex[LEFT]);
			}

			edge = edge->vertex[LEFT] == node ? edge->link[LEFT] : edge->link[RIGHT];
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
			if (edge->vertex[LEFT] == node && !vlist[edge->vertex[RIGHT]] && edge->direction == 1) {
				nodeQueue.push(edge->vertex[RIGHT]);
			} else if (edge->vertex[RIGHT] == node && !vlist[edge->vertex[LEFT]] && edge->direction == 2) {
				nodeQueue.push(edge->vertex[LEFT]);
			}

			edge = edge->vertex[LEFT] == node ? edge->link[LEFT] : edge->link[RIGHT];
		}

		olist.push(node);
	}
}

/*		
		
// Closeness - determine minimum number of edges to get
//  from one node to the other
int Graph::closeness(int vertex[LEFT], int vertex[RIGHT]) {
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
				// if we can get to vertex[RIGHT]
				if(edgePtr->direction != 2){
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
			else{ // if this is vertex[RIGHT]
				// if we can get to vertex[LEFT]
				if(edgePtr->direction != 1){
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
