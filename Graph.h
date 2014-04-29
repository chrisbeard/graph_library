//Put any include statements here
#include <string>
#include <iostream>

//This class will be used to create a graph library.

class Graph{
	private:
		//Put your private data members here
		//Put your private methods here
	public:
		//Construct
		Graph();
		//Delete
		~Graph();
		//Read A Graph From A File
		void readFromFile(std::string file);
		//Write A Graph To A File
		void writeToFile(std::string file);
		//Empty
		bool empty();
		//Add Edge
		void addEdge(int v1, int v2, double weight);
		//Add Vertex
		void addVertex();
		//Count Connected Components
		int numConnectedComponents();
		//Tree Check
		bool tree();
		//Depth First Traverse
		void DFT(int source);
		//Breadth First Traverse
		void BFT(int source);
		//Closeness
		int closeness(std::string v1, std::stringv2);
		//* Partition
		bool partitionable();
		//* MST
		bool MST(const Graph& g, std::string file);
		//* Step Away
		void stepAway(int value, std::string file);
};
