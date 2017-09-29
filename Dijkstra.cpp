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
void UnD_Dijkstra(int source);
void D_Dijkstra(int source);
int ShortestDist(int dist[], bool set[]);
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
	D_Dijkstra(0);
	UnD_Dijkstra(0);

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
		cout << " -- " << dest << '\t' << " The distance from source node 0 to destination node " << dest << " is " << Route[type].dist[dest] << endl;

		if (dest < 11)
			cout << dest;
	}

}

void PrintResult1(int source, int dest, int type) {
	switch (type) {

	case 0:cout << "This is performing Dijkstra algorithm to find the shortest path with undirected graph: " << endl;
		break;
	case 1:cout << endl << "This is performing Dijkstra algorithm to find the shortest path with directed graph: " << endl;
		break;
	}

	//cout << "The edges are displaying as following: ";
	PrintResult(source, dest, type);
	cout << "The total distance of the shortest path is: " << Route[type].dist[dest] << endl;
}

int ShortestDist(int dist[], bool set[])
{
	int min = INT_MAX, index;

	for (int v = 0; v < node; v++)
		if (set[v] == false && dist[v] <= min)
			min = dist[v], index = v;

	return index;
}

void UnD_Dijkstra(int source)
{
	int temp;
	
	// Use Route[0]
	// Intialize all nodes in array dist to infinity, nodes in array set to false, and nodes in previous to NULL
	for (int i = 0; i < node; i++) {
		Route[0].dist[i] = INT_MAX;
		Route[0].set[i] = 0;
		Route[0].previous[i] = NULL;
	}

	// Set first node to 0, and previous node to NULL
	Route[0].dist[source] = 0;
	Route[0].previous[source] = NULL;

	// Use UnD_Weightgraph since this is for undirected graph 
	// 1. Find the shortest distance first
	// 2. Go to current shortest path, if the distance of the destination node is longer than the previous node's,
	//    then update the destination by adding previous node's distance and current weight. 
	// 3. Update previous node.
	for (int j = 0; j < node - 1; j++) {

		temp = ShortestDist(Route[0].dist, Route[0].set);
		Route[0].set[temp] = true;

        for (int k = 0; k < node; k++) 
		{
			if (!Route[0].set[k] && UnD_Weightgraph[temp][k] && Route[0].dist[temp] != INT_MAX)
			{
				if (Route[0].dist[k] > Route[0].dist[temp] + UnD_Weightgraph[temp][k])
				{
					Route[0].dist[k] = Route[0].dist[temp] + UnD_Weightgraph[temp][k];
					Route[0].previous[k] = temp;
				}
			}
		}
	}
}


void D_Dijkstra(int source)
{
    int temp;
	
	// Use Route[1]
	// Intialize all nodes in array dist to infinity, nodes in array set to false, and nodes in previous to high
    for (int i = 0; i < node; i++) {
		Route[1].dist[i] = INT_MAX;
		Route[1].set[i] = 0;
		Route[1].previous[i] = NULL;
	}

	// Set first node to 0, and previous node to high
	Route[1].dist[source] = 0;
	Route[1].previous[source] = NULL;

	// Use D_Weightgraph since this is for directed graph 
	// 1. Find the shortest distance first
	// 2. Go to current shortest path, if the distance of the destination node is longer than the previous node's,
	//    then update the destination by adding previous node's distance and current weight. 
	// 3. Update previous node.
	for (int j = 0; j < node - 1; j++) 
	{
		temp = ShortestDist(Route[1].dist, Route[1].set);
		Route[1].set[temp] = true;

		for (int k = 0; k < node; k++)
		{
			if (!Route[1].set[k] && D_Weightgraph[temp][k] && Route[1].dist[temp] != INT_MAX)
			{
				if (Route[1].dist[temp] + D_Weightgraph[temp][k] < Route[1].dist[k])
				{
					Route[1].dist[k] = Route[1].dist[temp] + D_Weightgraph[temp][k];
					Route[1].previous[k] = temp;
				}
			}
		}
	}

}