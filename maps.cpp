/*
 * maps.cpp
 *
 *  Created on: Oct 1, 2018
 *      Author: admin1
 */

#include<iostream>
#include<limits.h>
#include<stack>

#define MAX 10
using namespace std;

int main() {
	stack<int> s;
	int graph[MAX][MAX], nodes = 0, edges = 0, a, b, label[MAX], source,
			destination, min, min_index, prev_Node[MAX], source1, destination1;
	bool visited[MAX];
	for (int i = 0; i < MAX; i++) {
		label[i] = INT_MAX;
		visited[i] = false;
		for (int j = 0; j < MAX; j++)
			graph[i][j] = 0;
	}
	cout << "Enter number of nodes in the graph (upto MAX nodes): ";
	cin >> nodes;
	cout << "Enter number of edges in the graph (upto "
			<< nodes * (nodes - 1) / 2 << " edges): ";
	cin >> edges;
	cout << "Enter vertices of edge to be added in graph :-\n";
	for (int i = 0; i < edges; i++) {
		cout << "First vertex : ";
		cin >> a;
		a--;
		cout << "Second vertex : ";
		cin >> b;
		b--;
		cout << "Enter Weight : ";
		cin >> graph[a][b];
		graph[b][a] = graph[a][b];
	}
	cout << "\n\nEnter Source : ";
	cin >> source;
	source--;
	for (int i = 0; i < MAX; i++) {
		prev_Node[i] = source;
	}
	cout << "\n\nEnter Destination : ";
	cin >> destination;
	destination--;
	label[source] = 0;
	visited[source] = true;
	source1 = source;
	destination1 = destination;
	while (source != destination) {
		min = INT_MAX;
		for (int i = 0; i < nodes; i++)
			if (graph[source][i] && !visited[i]
					&& label[i] >= label[source] + graph[source][i]) {
				label[i] = label[source] + graph[source][i];
				prev_Node[i] = source;
			}
		for (int i = 0; i < nodes; i++)
			if (!visited[i] && min >= label[i]) {
				min = label[i];
				min_index = i;
			}
		visited[min_index] = true;
		source = min_index;
	}
	while (destination != source1) {
		s.push(destination+1);
		destination = prev_Node[destination];
	}
	s.push(destination+1);
	cout << "\n\n";
	while (!s.empty()) {
		cout << s.top();
		if (s.size() > 1)
			cout << " -> ";
		s.pop();
	}
	cout << "\nShortest path is of " << label[destination1] << endl;
	return 0;
}
/*
 8
 15
 1
 2
 2
 1
 3
 4
 1
 4
 5
 2
 3
 1
 2
 5
 3
 2
 6
 1
 3
 4
 2
 3
 6
 2
 4
 6
 1
 4
 7
 2
 5
 6
 2
 5
 8
 3
 6
 7
 4
 6
 8
 7
 7
 8
 1
 1
 8
 */
