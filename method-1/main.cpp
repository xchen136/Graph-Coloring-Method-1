
#include<iostream>
#include<fstream>
using namespace std;

class node{
	private:
		int ID;
		int color;												
		int numEdges;											
		node* next = NULL;
		
	public:
		
		//initially unColored
		node(int id, int edges){
			ID = id;
			color = 0;
			numEdges = edges;
		}
		
		int getID(){
			return ID;
		}
		
		int getColor(){
			return color;
		}
		
		int getNumEdges(){
			return numEdges;
		}
		
		node* getNext(){
			return next;
		}
		
		void setNext(node* n){
			next = n;
		}
		
		void setColor(int c){
			color = c;
		}
		
};

class graphColoring{
	private:
		int** adjacencyMatrix;		//whether a node have an edge to another, diagonal reserved for node color
		node* listHead;											
		int newColor;				//acts both as the count and identity of color
		int numNode;
		
	public:
		graphColoring(int n){
			numNode = n;
			listHead = new node(0, 0);
			adjacencyMatrix = new int*[n];
			for(int row=0; row<n; row++){
				adjacencyMatrix[row] = new int[n]();
			}
			newColor = 0;
		}
		
		//using a minimum number of colors to color all nodes in the graph so that no two adjacent nodes will have the same color
		void colorNodes(ofstream& writeFile){
			int nodeID;
			node* current;
			
			//while there is an unColored node, color them 
			while(findUncolorNode() != NULL){
				++newColor;
				current = findUncolorNode();
				while(current != NULL){																//traversing from first uncoloredNode 
					if(current->getColor() == 0 && canColor(current->getID(), newColor)){			//if we can color current node with newColor
						nodeID = current->getID();
						adjacencyMatrix[nodeID-1][nodeID-1] = newColor;								//update the adjacencyMatrix 
						current->setColor(newColor);												//update node data
					}	
					current = current->getNext();
				}
				printMatrix(writeFile);			
			}	
			writeFile<<"Minimum Number of Colors Used: "<<newColor<<endl;
			printMatrix(writeFile);	
		}
		
		//update matrix with edges known
		void loadMatrix(ifstream& readFile, ofstream& writeFile){
			int node1, node2;
			writeFile<<"[Input Pairs]\n";
			while(readFile>>node1 && readFile>>node2){
				writeFile<<node1<<" "<<node2<<endl;
				adjacencyMatrix[node1-1][node2-1] = 1;
				adjacencyMatrix[node2-1][node1-1] = 1;
			}	
			writeFile<<endl;
			printMatrix(writeFile);
		}
		
		//insert node after the greatest numEdge node that's still smaller 
		void insertOneNode(node* n){
			node* current = listHead;
			while(current->getNext() != NULL){
				if(current->getNext()->getNumEdges() < n->getNumEdges()){
					current = current->getNext();
				}
				else{
					break;
				}
			}
			node* temp = current->getNext();
			current->setNext(n);
			n->setNext(temp);	
		}
		
		//create a list of nodes ordered by ascending number of edges
		void constructNodeList(){
			int edges;
			//create a node and insert it into list for each node
			for(int i=1; i<=numNode; i++){											
				edges = numEdges(i);
				node* newNode = new node(i, edges);
				insertOneNode(newNode);
			}
		}	
		
		//finds the first unColoredNode
		node* findUncolorNode(){
			node* current = listHead->getNext();
			while(current != NULL){
				if(current->getColor() == 0){
					return current;
				}
				else{
					current = current->getNext();
				}
			}
			return NULL;
		}	
		
		//counts the number of edges connected to this node
		int numEdges(int node){
			int numEdges = 0;
			for(int i=0; i<numNode; i++){
				if(adjacencyMatrix[node-1][i] == 1){
					++numEdges;
				}
			}
			return numEdges;
		}
		
		//returns true if there are no adjacent node of the same color
		bool canColor(int nodeID, int c){
			for(int i=0; i<numNode; i++){
				if(adjacencyMatrix[nodeID-1][i] == 1 && adjacencyMatrix[i][i] == c){
					return false;
				}
			}
			return true;
		}
		
		void printMatrix(ofstream& writeFile){
			writeFile<<"[Adjacency Matrix]"<<endl;
			for(int i=0; i<numNode; i++){
				for(int j=0; j<numNode; j++){
					writeFile<<adjacencyMatrix[i][j]<<" ";
				}
				writeFile<<endl;
			}
			writeFile<<endl;
		}
		
};


int main(int argc, char** argv){
	ifstream inFile;
	ofstream outFile;
	inFile.open(argv[1]);
	outFile.open(argv[2]);
	int nodes;
	
	//check the number of arguments
	if (argc != 3) {																
        cerr << "Error: Number of arguments unsatisfied.\n";
        exit(1);
    }
	
	if(inFile>>nodes){
		graphColoring method1(nodes);
		method1.loadMatrix(inFile, outFile);
		method1.constructNodeList();
		method1.colorNodes(outFile);
	}
	else{
		cerr<<"Error within input file\n";
		exit(1);
	}
	
	inFile.close();
	outFile.close();	
}
