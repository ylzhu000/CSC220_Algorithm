#include <iostream>
#include <fstream> // For ifstream
#include <cstdlib> // For exit(1)

const int edge = 31; // Total # of edges
const int node = 12; // Total # of nodes


using namespace std;
struct EdgeSet {
	int start_node;
	int end_node;
	int weight;
} edges[edge];

struct Path_Set {
	int dist[node]; 
	int previous[node]; 
	bool set[node];
} Route[2]; 


int Dgraph[node][node];
int UnDgraph[node][node];
int D_Weightgraph[node][node];
int UnD_Weightgraph[node][node];

void read_file(ifstream& inFile);
void DgraphFunctin();
void UnDgraphFunction();
void UnD_Bellman_Ford(int source);
void D_Bellman_Ford(int source);
void PrintResult(int source, int dest, int path);
void PrintResult1(int source, int dest, int path);



int main() {
	
	ifstream inFile;
	inFile.open("input.txt"); 

	if (inFile.fail()) {
		cout << "Fail to open the file" << endl;
		exit(1);
	}

	read_file(inFile);
	inFile.close(); 

	// Call undirected and directed graph fucntion 
	DgraphFunctin();
	UnDgraphFunction();
	
	// Call undirected and directed bell_man_ford algorithm
    D_Bellman_Ford(0);
	UnD_Bellman_Ford(0);

	// Print out the result of the shortest path
	// Iterates two times since two routes.
	for (int i = 0; i < 2; i++) {
		PrintResult1(0, 11, i);
	}

	system("PAUSE");
	return 0;
}

void read_file(ifstream& inFile) {
	int start, end, weight = 0;
	int count = 0;

	while (!inFile.eof() && count < edge)
	{
		inFile >> start >> end >> weight;
		edges[count].start_node = start;
		edges[count].end_node = end;
		edges[count].weight = weight;
		count++;
	}
}

void UnDgraphFunction() {

	int n = 0;
	int temp;
     
	// Initialization for undiretced maxtrixs
	for (int i = 0; i < node; i++) {
		for (int j = 0; j < node; j++) {
			
			UnDgraph[i][j] = 0;
			UnD_Weightgraph[i][j] = 0;
		}
	}

    
	while (n < edge) {
		UnDgraph[edges[n].start_node][edges[n].end_node] = 1; //--
	                                                          //--> Initialize the undirected matrix, two edges are initialized since undirected
		UnDgraph[edges[n].end_node][edges[n].start_node] = 1; //--

		// Pick the lighest weight if two nodes point to each other. 
		if (UnD_Weightgraph[edges[n].start_node][edges[n].end_node] != 0) {

			temp = UnD_Weightgraph[edges[n].start_node][edges[n].end_node];

			if (temp > edges[n].weight) {
				UnD_Weightgraph[edges[n].start_node][edges[n].end_node] = edges[n].weight;
				UnD_Weightgraph[edges[n].end_node][edges[n].start_node] = edges[n].weight;
			}
		}

		else {
			UnD_Weightgraph[edges[n].start_node][edges[n].end_node] = edges[n].weight;
			UnD_Weightgraph[edges[n].end_node][edges[n].start_node] = edges[n].weight;
		}
		
		n++;
	}
}

void DgraphFunctin() {
	int n = 0;

	// Initialization for undiretced maxtrixs
	for (int i = 0; i < node; i++) {
		for (int j = 0; j < node; j++) {
			Dgraph[i][j] = 0;
			D_Weightgraph[i][j] = 0;
		}
	}

	while (n < edge) {
		
		Dgraph[edges[n].start_node][edges[n].end_node] = 1;
		D_Weightgraph[edges[n].start_node][edges[n].end_node] = edges[n].weight;
		n++;
	}
}

void PrintResult(int source, int dest, int type) {
	if (source == dest) {
		cout << endl;
		cout << source;
	}

	else if (Route[type].previous[dest] == 100) {

	
		cout << "There's no path from " << source << " to " << dest << " exist\n";
	}
	else {

		PrintResult(source, Route[type].previous[dest], type);
		cout << " -- " << dest <<'\t'<<" The distance from source node 0 to destination node " <<dest<<" is " << Route[type].dist[dest]<< endl;

		if (dest < 11)
			cout << dest;
	}

}

void PrintResult1(int source, int dest, int type) {
	switch (type) {
	
	case 0:cout << "This is performing Bellman_Ford algorithm to find the shortest path with undirected graph: " << endl;
		break;
	case 1:cout << endl<< "This is performing Bellman_Ford algorithm to find the shortest path with directed graph: " << endl;
		break;
	}

	//cout << "The edges are displaying as following: ";
	PrintResult(source, dest, type);
	cout << "The total distance of the shortest path is: " << Route[type].dist[dest] << endl;
}

void UnD_Bellman_Ford(int source) {
    int count = 0; // use to count total of edges, use it for later on
	EdgeSet edge1[48]; // 48 total edges in undirected graph

	// Initialization for undirected matrix
	for (int i = 0; i < node; i++)
	{
		for (int j = 0; j < node; j++) {
			if (UnD_Weightgraph[i][j] != 0) {
				edge1[count].start_node = i;
				edge1[count].end_node = j;
			    edge1[count].weight = UnD_Weightgraph[i][j];
				count++; }
		}
	}

	// Set all nodes to infinity and previous to null
	for (int i = 0; i < node; i++) {
		Route[0].dist[i] = INT_MAX;
		Route[0].previous[i] = NULL;
	}

	// Initialize the source to 0 and previous source node to null.
	Route[0].dist[source] = 0;
	Route[0].previous[source] = NULL;

	// If the distance is not infinity, and the distance of the destination node is longer than the source node's,
	// set the source node's distance to destination node. Also, update the previous node. 
	for (int i = 0; i <= node; i++) {
		for (int j = 0; j < count; j++) {
			if (Route[0].dist[edge1[j].start_node] != INT_MAX)
			{
				if (Route[0].dist[edge1[j].end_node] > Route[0].dist[edge1[j].start_node] + edge1[j].weight)
				{
					Route[0].dist[edge1[j].end_node] = Route[0].dist[edge1[j].start_node] + edge1[j].weight;
					Route[0].previous[edge1[j].end_node] = edge1[j].start_node;}
			}
		}
	}

	// Check if negative cycle exist
	for (int k = 0; k < count; k++) {
		if (Route[0].dist[edge1[k].start_node] != INT_MAX)
		{
			if (Route[0].dist[edge1[k].start_node] + edge1[k].weight < Route[0].dist[edge1[k].end_node])
			{
				cout << "The Graph contains negative egde cycle" << endl;
			}
		}
	}
}


void D_Bellman_Ford(int source) {

	
	// Set all nodes to infinity and previou nodes to high
	for (int i = 0; i < node; i++) {
		Route[1].dist[i] = INT_MAX;
		Route[1].previous[i] = NULL;
	}

	// Initialize the source to 0 and previous source node to high.
	Route[1].dist[source] = 0;
	Route[1].previous[source] = NULL;

	// Same as undircted's algorithm, but use different route to represent
	for (int i = 1; i <= node - 1; i++) {
		for (int j = 0; j < edge; j++) {
			if (Route[1].dist[edges[j].start_node] != INT_MAX)
			{
				if (Route[1].dist[edges[j].start_node] + edges[j].weight < Route[1].dist[edges[j].end_node])
				{
					Route[1].dist[edges[j].end_node] = Route[1].dist[edges[j].start_node] + edges[j].weight;
					Route[1].previous[edges[j].end_node] = edges[j].start_node;
				}
			}
		}
		
	}

	// Check if negative cycle exist.
	for (int i = 0; i < edge; i++) {
		if (Route[1].dist[edges[i].start_node] != INT_MAX)
		{
			if (Route[1].dist[edges[i].start_node] + edges[i].weight < Route[1].dist[edges[i].end_node])
			{
				cout << "Negative weight cycle detected" << endl;
			}
		}
	}
}