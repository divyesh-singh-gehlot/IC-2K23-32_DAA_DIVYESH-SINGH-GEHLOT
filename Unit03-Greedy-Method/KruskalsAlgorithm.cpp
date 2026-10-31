// Minimum Cost Spanning Tree - Kruskal's Algorithm
// Sort all edges by weight. Take the smallest edge each time,
// but skip it if it forms a cycle (checked using union-find).

#include <iostream>
#include <algorithm>
using namespace std;

struct Edge {
    int u, v, weight;
};

int parent[10];

bool compare(Edge a, Edge b) {
    return a.weight < b.weight;
}

// find the leader (root) of the set that x belongs to
int findSet(int x) {
    if (parent[x] == x) {
        return x;
    }
    parent[x] = findSet(parent[x]);   // path compression
    return parent[x];
}

int main() {
    int vertices = 5;
    int e = 7;

    // same graph as in prims.cpp so the answers can be compared
    Edge edges[7] = {
        {0, 1, 2}, {0, 3, 6}, {1, 2, 3}, {1, 3, 8},
        {1, 4, 5}, {2, 4, 7}, {3, 4, 9}
    };

    sort(edges, edges + e, compare);

    for (int i = 0; i < vertices; i++) {
        parent[i] = i;
    }

    int totalCost = 0;
    int edgesTaken = 0;

    cout << "Edge    Weight" << endl;
    for (int i = 0; i < e && edgesTaken < vertices - 1; i++) {
        int a = findSet(edges[i].u);
        int b = findSet(edges[i].v);

        if (a != b) {
            // different sets, so no cycle: take this edge
            parent[a] = b;
            cout << edges[i].u << " - " << edges[i].v << "     " << edges[i].weight << endl;
            totalCost = totalCost + edges[i].weight;
            edgesTaken++;
        }
    }

    cout << endl << "Total cost of MST = " << totalCost << endl;
    return 0;
}