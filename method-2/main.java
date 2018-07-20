
import java.util.Scanner;
import java.io.*;

class node{
	private int colorID;
	private node next = null;
	
	node(int c){
		colorID = c;
	}
	
	public int getColor(){
		return colorID;
	}
	
	public node getNext(){
		return next;
	}
	
	public void setNext(node n){
		next = n;
	}
}

class graphColoring{
	private int[][] adjacencyMatrix;
	private node usedColorTop;						//a stack of colors, newest color are on the top
	private int newColor;
	private int numNode;
	
	graphColoring(int n){
		adjacencyMatrix = new int[n][n];
		usedColorTop = null;
		newColor = 0;
	}
	
	//color node by node with usedColor if possible, so that no two adjacent nodes have the same color
	public void colorGraph(FileWriter oFile){
		PrintWriter outFile = new PrintWriter(oFile);
		int currentNode = 1;
		int usedColor;
		
		//while not all nodes are colored
		while(currentNode<=numNode){
			usedColor = findUsedColor(currentNode);
			if(usedColor > 0){
				adjacencyMatrix[currentNode-1][currentNode-1] = usedColor;
			}
			else{
				++newColor;
				pushUsedColor(newColor);
				adjacencyMatrix[currentNode-1][currentNode-1] = newColor;
			}
			printMatrix(oFile);
			++currentNode;
		}
		outFile.println("Minimum Number of Colors Used: "+ newColor);
		printMatrix(oFile);
		outFile.close();
	}
	
	//initialize the adjacency matrix with known edges
	public void loadMatrix(File in, FileWriter out)throws FileNotFoundException{
		Scanner inFile = new Scanner(in);
		PrintWriter outFile = new PrintWriter(out);
		int node1, node2;
		
		outFile.println("[Input Pairs]");
		if(inFile.hasNextInt()){
			numNode = inFile.nextInt();
			outFile.println(numNode);
			while(inFile.hasNextInt() && inFile.hasNextInt()){
				node1 = inFile.nextInt();
				node2 = inFile.nextInt();
				adjacencyMatrix[node1-1][node2-1] = 1;
				adjacencyMatrix[node2-1][node1-1] = 1;
				outFile.println(node1 + " " + node2);
			}
		}	
		outFile.println("");
		printMatrix(out);
	}
	
	//returns true if no adjacent node is in this color
	public boolean canColor(int node, int color){
		for(int c=0; c<numNode; c++){
			if(adjacencyMatrix[node-1][c] == 1 && adjacencyMatrix[c][c] == color){
				return false;
			}
		}
		return true;
	}
	
	//adds this new color onto the top of the stack
	public void pushUsedColor(int newC){
		node newNode = new node(newC);
		node temp = usedColorTop;
		usedColorTop = newNode;
		newNode.setNext(temp);
	}
	
	//returns the newest used color that can color this node, else return 0
	public int findUsedColor(int node){
		node current = usedColorTop;
		while(current != null){
			if(canColor(node, current.getColor())){
				return current.getColor();
			}
			else{
				current = current.getNext();
			}
		}
		return 0;
	}
	
	public void printMatrix(FileWriter oFile){
		PrintWriter outFile = new PrintWriter(oFile);
		outFile.println("[Adjacency Matrix]");
		for(int i=0; i<numNode; i++){
			for(int j=0; j<numNode; j++){
				outFile.print(adjacencyMatrix[i][j] + " ");
			}
			outFile.println("");
		}
		outFile.println("");
	}
}

public class main {
	public static void main(String[] args) throws IOException{
		File input = new File(args[0]);
		Scanner inFile = new Scanner(input);
		FileWriter output = new FileWriter(args[1]); 
		int nodes = 0;
		
		if(inFile.hasNextInt()){
			nodes = inFile.nextInt();
			graphColoring method2 = new graphColoring(nodes);
			method2.loadMatrix(input, output);
			method2.colorGraph(output);
		}
		inFile.close();
	}
}
