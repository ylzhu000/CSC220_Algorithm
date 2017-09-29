#include <iostream>
#include <fstream> // For ifstream
#include <cstdlib> // For exit(1)
#include <queue>
#include <list>

#define white 0;
#define grey 1;
#define black 2;

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
	int color[node];
	queue<int> Q;
} Route[2];

int Dgraph[node][node];
int UnDgraph[node][node];
int D_Weightgraph[node][node];
int UnD_Weightgraph[node][node];

void read_file(ifstream& inFile);
void DgraphFunctin();
void UnDgraphFunction();
void UnD_BFS(int source);
void D_BFS(int source);
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

	// Call undirected and directed Dijkstra algorithm
	D_BFS(0);
	UnD_BFS(0);

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

void PrintResult(int source, int dest, int i) {
	if (source == dest) {
		cout << endl;
		cout << source;
	}

	else if (Route[i].previous[dest] == 100) {


		cout << "There's no path from " << source << " to " << dest << " exist\n";
	}

	else {

		PrintResult(source, Route[i].previous[dest], i);
		cout << " -- " << dest << '\t' << " The distance from source node 0 to destination node " << dest << " is " << Route[i].dist[dest] << endl;

		if (dest < 11)
			cout << dest;
	}

}

void PrintResult1(int source, int dest, int i) {
	switch (i) {

	case 0:cout << "This is performing Breath-First Search algorithm to find the shortest path with undirected graph: " << endl;
		break;
	case 1:cout << endl << "This is performing Breath-First Search algorithm to find the shortest path with directed graph: " << endl;
		break;
	}

	//cout << "The edges are displaying as following: ";
	PrintResult(source, dest, i);
	cout << "The total distance of the shortest path is: " << Route[i].dist[dest] << endl;
}

// For undiretced graph 
void UnD_BFS(int source)
{
	int temp; // Temp variable is used for previous node
    // 1. Intialize all nodes in array dist to infinity
	// 2. Intialize all nodes in color is white, means not discovered and visited yet
	// 3. Initialize all nodes in previous array to null
	for (int i = 0; i < node; i++) 
	{
		Route[0].color[i] = white; 
		Route[0].dist[i] = INT_MAX; 
		Route[0].previous[i] = NULL; 
	}
    
	// 1. Set source node to grey, means discovered
	// 2. Set the distance of source node to 0
	// 3. Set previous node to null
	// 4. Put the source node into the queue.
	Route[0].color[source] = grey; 
	Route[0].dist[source] = 0; 
	Route[0].previous[source] = NULL; 
	Route[0].Q.push(source); 

	// While the queue is not empty:
	// 1. Save the first element in queue, then pop it
	// If there's a edge and no color for the edge yet
	// 2. Update the previous node and distance between the nodes
	// 3. Mark the current node as grey and push it into the queue
	while (!Route[0].Q.empty())
	{
		temp = Route[0].Q.front(); 
		Route[0].Q.pop();
        for (int i = 0; i < node; i++) 
		{
			if (UnDgraph[temp][i])
			{
				if (Route[0].color[i] == 0)
				{
					Route[0].previous[i] = temp;
					Route[0].dist[i] = Route[1].dist[temp] + 1;
					Route[0].color[i] = grey;
					Route[0].Q.push(i);
				}
			}
		}

		// Mark the discovered node as black, means visited after the for loop
		Route[0].color[temp] = black;
	}
}

// For directed graph 
void D_BFS(int source)
{
	int temp; // Temp variable is used for previous node

    // 1. Intialize all nodes in array dist to infinity
    // 2. Intialize all nodes in color is white, means not discovered and visited yet
    // 3. Initialize all nodes in previous array to null
	for (int i = 0; i < node; i++) 
	{
		Route[1].color[i] = white;
		Route[1].dist[i] = INT_MAX;
		Route[1].previous[i] = NULL;
	}

	// 1. Set source node to grey, means discovered
	// 2. Set the distance of source node to 0
	// 3. Set previous node to null
	// 4. Put the source node into the queue.
	Route[1].color[source] = grey;
	Route[1].dist[source] = 0;
	Route[1].previous[source] = NULL;
	Route[1].Q.push(source);

	// While the queue is not empty:
	// 1. Save the first element in queue, then pop it
	// If there's a edge and no color for the edge yet
	// 2. Update the previous node and distance between the nodes
	// 3. Mark the current node as grey and push it into the queue
	while (!Route[1].Q.empty()) 
	{
		temp = Route[1].Q.front();
		Route[1].Q.pop();

		for (int i = 0; i < node; i++) 
		{
			if (Dgraph[temp][i])
			{
				if (Route[1].color[i] == 0) 
				{
					Route[1].previous[i] = temp;
					Route[1].dist[i] = Route[1].dist[temp] + 1;
					Route[1].color[i] = grey;
					Route[1].Q.push(i);
				}
			}
		}

		// Mark the discovered node as black, means visited after the for loop
		Route[1].color[temp] = black;
	}

}