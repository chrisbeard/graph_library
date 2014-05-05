#include <string>
#include <iostream>
#include <vector>

//Construct an empty graph of the specified type
Graph::Graph(Type t){
	gType = t;
	vector<Vertex> temp;
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
		
//Add an edge to the edge list
void Graph::addEdge(int v1, int v2, double weight){
	Edge temp;
	temp.weight = weight;
	temp.vertex[0] = v1;
	temp.vertex[1] = v2;
	temp.link[0] = nullptr;
	temp.link[1] = nullptr;
	//add the edge to the edgelist for both vertices
	if(edgeList[v1].first != nullptr){
		temp.link[0] = edgeList[v1].first;
	}	
	edgeList[v1].first = temp;
	if(edgeList[v2].first != nullptr){
		temp.link[1] = edgeList[v2].first;
	}	
	edgeList[v2].first = temp;
}
		
//Add a vertex to the head of the edge list
void Graph::addVertex(){
	Vertex temp;
	temp.value = edgeList.size();
	temp.first = nullptr;
	edgeList.push_back(temp);
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
