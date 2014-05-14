#include "Graph.h"
#include <algorithm>
#include <fstream>
#include <map>
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
	if (edgeList.size() < 2) {
		throw("Not enough space");
	}
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
	
// Add a vertex to the head of the edge list
void Graph::addVertex() {
	// All we need to do is allocated space for an edge
	// There is no need to keep track of node values
	edgeList.push_back(nullptr);
}
		
int Graph::numConnectedComponents() {
	size_t components = 0;
	std::vector<bool> visited(edgeList.size(), false);
	for (size_t i = 1; i < visited.size(); ++i) {
		if (!visited[i]) {
			++components;
			breadthFirstApply(visited, i, [&](int){ return false; }, true);
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

void Graph::BFT(int source, std::string file) {
	std::queue<int> order;
	std::vector<bool> visited(edgeList.size(), false);
	breadthFirstApply(visited, source, [&](int node) {
		order.push(node);	
		return false;
	}, !directed);

	std::ofstream outfile(file);
	if (outfile) {
		// std::cout << order.size() << "\n";
		while (!order.empty()) {
			int node = order.front();
			order.pop();
			outfile << node << "\n";
		}
	}
}
// Breadth First search - proceed from source
/*void Graph::BFT(int source, std::string file) {
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
}*/

/*void Graph::BFDirected(int node, std::vector<int> &vlist, std::queue<int> &olist) {
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
}*/

	
// Closeness - determine minimum number of edges to get
// from one node to the other
int Graph::closeness(int v1, int v2) {
	if(v1 == v2){
		return 0;
	}
	std::vector<int> distance = std::vector<int>(edgeList.size(), 0xFF);
	std::set<int> unvisited;
	for(uint i = 0; i < edgeList.size(); i++){
		unvisited.insert(i);
	}
	bool finished = false;
	int retval = -1;
	distance[v1] = 0;
	int node = v1;
	//while all nodes have not been visited
	while(!finished){
		//find distances of all adjacent nodes
		undirectedClose(node, distance, unvisited);
		//mark current node as visited
		unvisited.erase(node);
		finished = true;
		//find the next unvisited node with the smallest distance
		for(auto i = unvisited.begin(); i != unvisited.end(); i++){
			if(!finished){
				node = *i;
				finished = false;
			}
			else if(distance[*i] < distance[node]){
				node = *i;
			}
		}
		//if there is no path to the next node
		if(distance[node] == 0xFF){
			retval = -1;
			finished = true;
		}
		//if we have visited all nodes
		if(finished && retval != -1){
			retval = distance[v2];
		}
	}
	return retval;
}

void Graph::undirectedClose(int v1, std::vector<int> &distance, std::set<int> unvisited){
	Edge *edgePtr = edgeList[v1];
	//for all edges
	while(edgePtr != nullptr){
		//find which node it is pointing to
		int next = (edgePtr->vertex[LEFT] == v1 ? edgePtr->vertex[RIGHT] : edgePtr->vertex[LEFT]);	
		//relative distance = distance of v1 + 1, if smaller than current value
		if(!unvisited.count(next)){
			distance[next] = std::min((distance[v1] + 1), distance[next]);
		}
		//iterate edge pointer
		edgePtr = (edgePtr->vertex[LEFT] == v1 ? edgePtr->link[LEFT] : edgePtr->link[RIGHT]);
	}
}
/*
void Graph::directedClose(int v1, int v2, std::vector<int> &distance){
	Edge *edgePtr = edgeList[v1];
	while(edgePtr != nullptr){

	}
}
*/

// Partition - determine if you can partition the graph
bool Graph::partitionable() {
	std::vector<int> group(edgeList.size(), 0);
	std::vector<bool> visited(edgeList.size(), 0);

	int currentGroup = 1;
	bool partitionable = true;
	breadthFirstApply(visited, 1, [&](int node) {
		if (group[node] == 0) {
			group[node] = currentGroup;
		}

		currentGroup = group[node] == 1 ? 2 : 1;
		Edge *e = edgeList[node];
		while (e) {
			int child = e->vertex[LEFT] == node ? e->vertex[RIGHT] : e->vertex[LEFT];
			if (group[child] == 0) {
				group[child] = currentGroup;
			} else if (group[child] != currentGroup) {
				partitionable = false;
				return true;
			}

			e = e->vertex[LEFT] == node ? e->link[LEFT] : e->link[RIGHT];
		}

		return false;
	}, true);

	return partitionable;
}


void Graph::breadthFirstApply(std::vector<bool> &visited, int source, const std::function<bool(int)> &lambda, bool ignoreDirections) {
	std::queue<int> vertices;
	vertices.push(source);
	while (!vertices.empty()) {
		int node = vertices.front();
//		std::cout << node << "\n";
		vertices.pop();

		if (visited[node]) {
			break;
		}

		visited[node] = true;
		if (lambda(node)) {
			break;
		}

		Edge *e = edgeList[node];
		while (e) {
			if (e->vertex[LEFT] && !visited[e->vertex[RIGHT]]
				&& (ignoreDirections || (directed && e->direction == LEFT))) {
				vertices.push(e->vertex[RIGHT]);
			} else if (e->vertex[RIGHT] && !visited[e->vertex[LEFT]]
				&& (ignoreDirections || (directed && e->direction == RIGHT))) {
				vertices.push(e->vertex[LEFT]);
			}

			e = e->vertex[LEFT] == node ? e->link[LEFT] : e->link[RIGHT];
		}
	}
}
		
// * MST - print the minimum spanning tree of the graph
// to a file with the passed name
// Kruskal's - minimum spanning forrest 

// There is a lot of stuff here. It works, and that's what matters.
bool Graph::MST(std::string file) {
	std::vector<bool> included(edgeList.size(), 0);

	std::ofstream outfile(file);
	if (!outfile) {
		return false;
	}

	if (empty()) {
		return true;
	}

	// Declare a comparator for use in sortingv1 the edges by minimum weight.
	auto edgeComparator = [](const Edge *e1, const Edge *e2) {
		return (e1->direction == RIGHT ? e1->weight[RIGHT] : e1->weight[LEFT])
			 < (e2->direction == RIGHT ? e2->weight[RIGHT] : e2->weight[LEFT]);
		// return (std::min(e1->weight[RIGHT], e1->weight[LEFT])
		// 	  < std::min(e2->weight[RIGHT], e2->weight[LEFT]));
	};

	// Awful syntax, but declares edges to be a set of edge pointers guaranteed
	// to be sorted in non-decreasing order by weight. Add all the edges to it
	// and they will be automatically sorted.
	std::set<const Edge *, decltype(edgeComparator)> edges(edgeComparator);
	for (const Edge *e : allEdges()) {
		// std::cout << e->vertex[LEFT] << " " << e->vertex[RIGHT] << "\n";
		edges.insert(e);
	}

	std::vector<size_t> disjointSet(edgeList.size());
	for (size_t i = 0; i < disjointSet.size(); ++i) {
		disjointSet[i] = i;
	}

	std::map<size_t, std::vector<const Edge *>> components;

	while (!edges.empty()) {
		// Because the edges set is always sorted (because we gave it a custom
		// comparator, edges.front() is guaranteed to return the minimum weight
		// edge);
		const Edge *e = *edges.begin();
		edges.erase(e);

		int v1, v2;
		if (e->direction == RIGHT) {
			v1 = e->vertex[RIGHT];
			v2 = e->vertex[LEFT];
		} else {
			v1 = e->vertex[LEFT];
			v2 = e->vertex[RIGHT];
		}

		// Both nodes already belong to the same set; taking this edge would
		// introduce a cycle.
		if (disjointSet[v1] == disjointSet[v2]) {
			continue;
		}

		// Take the union of the two sets by combining both to use the
		// minimum representative.
		size_t newSet = std::min(disjointSet[v1], disjointSet[v2]);
		size_t oldSet = std::max(disjointSet[v1], disjointSet[v2]);
		
		for (size_t i = 0; i < disjointSet.size(); ++i) {
			if (disjointSet[i] == oldSet) {
				disjointSet[i] = newSet;
			}
		}

		std::vector<const Edge *> component1 = components[newSet];
		if (components.find(newSet) == components.end()) {
			component1 = std::vector<const Edge *>();
		}

		std::vector<const Edge *> component2 = components[oldSet];
		if (components.find(oldSet) == components.end()) {
			component2 = std::vector<const Edge *>();
		} else {
			components.erase(oldSet);
		}

		std::merge(component1.begin(), component1.end(), component2.begin(),
				   component2.end(), component1.begin(), edgeComparator);
		
		component1.push_back(e);
		components[newSet] = component1;
	}

	for (const std::pair<size_t, std::vector<const Edge *>> &component : components) {
		outfile << "{ {";

		bool printed = false;
		for (size_t i = 1; i < disjointSet.size(); ++i) {
			if (disjointSet[i] == component.first) {
				if (printed) {
					outfile << ", ";	
				} else {
					printed = true;
				}

				outfile << i;
			}
		}

		outfile << "}, { ";
		for (size_t i = 0; i < component.second.size(); ++i) {
			const Edge *e = component.second[i];
			outfile << "(" << e->vertex[LEFT] << ", " << e->vertex[RIGHT] << ", ";
			if (e->direction == RIGHT) {
				outfile << e->weight[RIGHT];
			} else {
				outfile << e->weight[LEFT];
			}

			if (i < component.second.size() - 1) {
				outfile << "), ";
			}
		}

		outfile << ") } }\n";
	}

	return true;
}
		
// * Step Away - print the nodes who are a degree of
// closeness from the source to a file with the passed name
void Graph::stepAway(int source, int closeness, std::string file) {
	// TODO: Actually take care of this case. Not hard -- run through the entire
	// component starting from the given source and if anything's not visited,
	// print it.
	
	std::ofstream outfile(file);
	if (!outfile) {
		throw ("Could not open output file for writing");
	}

	std::vector<bool> visited(edgeList.size(), 0);
	std::queue<int> currentQueue;
	std::queue<int> nextQueue;
	
	currentQueue.push(source);
	visited[source] = true;

	int degree = 0;

	while (!currentQueue.empty()) {
		while (!currentQueue.empty()) {
			int current = currentQueue.front();
			currentQueue.pop();
				
			Edge *e = edgeList[current];
			while (e) {

				if (e->vertex[LEFT] == current) {
					if (!visited[e->vertex[RIGHT]] && e->direction != RIGHT) {
						nextQueue.push(e->vertex[RIGHT]);
						visited[e->vertex[RIGHT]] = true;
					}
					e = e->link[LEFT];
				} else if (e->vertex[RIGHT] == current) {
					if (!visited[e->vertex[LEFT]] && e->direction != LEFT) {
						nextQueue.push(e->vertex[LEFT]);
						visited[e->vertex[LEFT]] = true;
					}
					e = e->link[RIGHT];
				}
			}
		}

		++degree;
		currentQueue = nextQueue;
		nextQueue = std::queue<int> ();
		if (degree == closeness) {
			break;
		}
	}

	if (closeness == -1) {
		for (size_t i = 1; i < visited.size(); ++i) {
			if (!visited[i]) {
				outfile << i << "\n";
			}
		}
		return;
	}

	while (!currentQueue.empty()) {
		outfile << currentQueue.front() << "\n";
		currentQueue.pop();	
	}
}
