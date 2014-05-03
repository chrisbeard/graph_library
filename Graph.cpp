#include <string>
#include <iostream>

//Construct an empty graph of the specified type
Graph::Graph(Type t){

}
		
//Delete a graph
Graph::~Graph(){

}
		
//Read a graph from a file
void Graph::readFromFile(std::string file){

}
		
//Write a graph to a file
void Graph::writeToFile(std::string file){

}
		
//Empty
bool Graph::empty(){

}
		
//Add edge
void Graph::addEdge(int v1, int v2, double weight){

}
		
//Add vertex
void Graph::addVertex(){

}
		
//Count connected components
int Graph::numConnectedComponents(){

}
		
//Tree check
bool Graph::tree(){

}
		
//Depth First Traverse - proceed from source
void Graph::DFT(int source){

}
		
//Breadth First Traverse - proceed from source
void Graph::BFT(int source){

}
		
//Closeness - determine minimum number of edges to get
// from one node to the other
int Graph::closeness(int v1, int v2){

}

//* Partition - determine if you can partition the graph
bool Graph::partitionable(){

}
		
//* MST - print the minimum spanning tree of the graph
//to a file with the passed name
bool Graph::MST(std::string file){

}
		
//* Step Away - print the nodes who are a degree of
//closeness from the source to a file with the passed name
void Graph::stepAway(int source, int closeness, std::string file){

}
