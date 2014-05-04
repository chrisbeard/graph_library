#include <string>
#include <iostream>

//Construct an empty graph of the specified type
Graph::Graph(Type t){
	gType = t;
	vertex<Edge*> temp;
	edgeList = temp;
}
		
//Delete a graph
Graph::~Graph(){

}
		
//Read a graph from a file
void Graph::readFromFile(std::string file){
	std::ifstream myfile(file);
	//if the file was opened successfully
        if(myfile.is_open()){
                std::string line;
                while(getline(myfile, line)){
			//read file
                }
                myfile.close();
        } 
        else {
                std::cerr << "Failed to open the file." << std::endl;
        }
}
		
//Write a graph to a file
void Graph::writeToFile(std::string file){

}
		
//Empty
bool Graph::empty(){
	bool retval = false;	
	if(edgeList.size() == 0){
		retval = true;
	}
	return retval;
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
