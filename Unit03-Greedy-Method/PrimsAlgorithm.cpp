// Minimum Cost Spanning Tree - Prim's Algorithm
// Start from vertex 0. At every step add the cheapest edge that
// connects a vertex inside the tree to a vertex outside the tree.

#include <iostream>
using namespace std;

#define V 5
#define INF 9999

// pick the vertex (not yet in the tree) with the smallest key
int minKey(int key[], bool inMST[]) {
    int min = INF;
    int index = -1;
    for (int v = 0; v < V; v++) {
        if (inMST[v] == false && key[v] < min) {
            min = key[v];
            index = v;
        }
    }
    return index;
}

int main() {
    // adjacency matrix, 0 means there is no edge
    int graph[V][V] = {
        {0, 2, 0, 6, 0},
        {2, 0, 3, 8, 5},
        {0, 3, 0, 0, 7},
        {6, 8, 0, 0, 9},
        {0, 5, 7, 9, 0}
    };

    int parent[V];
    int key[V];
    bool inMST[V];

    for (int i = 0; i < V; i++) {
        key[i] = INF;
        inMST[i] = false;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V; count++) {
        int u = minKey(key, inMST);
        inMST[u] = true;

        // update the key of the neighbours of u
        for (int v = 0; v < V; v++) {
            if (graph[u][v] != 0 && inMST[v] == false && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                parent[v] = u;
            }
        }
    }

    int totalCost = 0;
    cout << "Edge    Weight" << endl;
    for (int i = 1; i < V; i++) {
        cout << parent[i] << " - " << i << "     " << graph[i][parent[i]] << endl;
        totalCost = totalCost + graph[i][parent[i]];
    }

    cout << endl << "Total cost of MST = " << totalCost << endl;
    return 0;
}