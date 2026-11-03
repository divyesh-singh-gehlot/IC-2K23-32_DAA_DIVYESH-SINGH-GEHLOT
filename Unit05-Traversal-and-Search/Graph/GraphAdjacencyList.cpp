// Graph representation using an Adjacency List
// Every vertex keeps a list of its neighbours.
// Needs only V + E space, which is much less than a matrix for sparse graphs.

#include <iostream>
#include <vector>
using namespace std;

#define V 6

vector<int> adj[V];

void addEdge(int a, int b) {
    adj[a].push_back(b);
    adj[b].push_back(a);       // undirected graph
}

bool hasEdge(int a, int b) {
    for (int i = 0; i < (int)adj[a].size(); i++) {
        if (adj[a][i] == b) {
            return true;       // we have to search the list, so this is O(degree)
        }
    }
    return false;
}

void printList() {
    for (int i = 0; i < V; i++) {
        cout << i << " -> ";
        for (int j = 0; j < (int)adj[i].size(); j++) {
            cout << adj[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    addEdge(0, 1);
    addEdge(0, 2);
    addEdge(1, 3);
    addEdge(1, 4);
    addEdge(2, 4);
    addEdge(3, 5);
    addEdge(4, 5);

    cout << "Adjacency list:" << endl;
    printList();

    cout << endl << "Degree of each vertex:" << endl;
    for (int i = 0; i < V; i++) {
        cout << "  vertex " << i << " : " << adj[i].size() << endl;
    }

    cout << endl << "Is there an edge between 1 and 4? " << (hasEdge(1, 4) ? "Yes" : "No") << endl;
    cout << "Is there an edge between 0 and 5? " << (hasEdge(0, 5) ? "Yes" : "No") << endl;

    int totalEntries = 0;
    for (int i = 0; i < V; i++) {
        totalEntries = totalEntries + adj[i].size();
    }
    cout << endl << "Entries stored in the list: " << totalEntries
         << " (matrix would store " << V * V << ")" << endl;
    return 0;
}
