// Breadth First Search (BFS)
// Visit the start vertex, then all its neighbours, then their neighbours...
// A queue (first in, first out) is used. Also gives the shortest distance
// (number of edges) from the start vertex in an unweighted graph.

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

#define V 6

vector<int> adj[V];

void addEdge(int a, int b) {
    adj[a].push_back(b);
    adj[b].push_back(a);
}

void bfs(int start) {
    bool visited[V];
    int distance[V];
    int parent[V];
    for (int i = 0; i < V; i++) {
        visited[i] = false;
        distance[i] = -1;
        parent[i] = -1;
    }

    queue<int> q;
    visited[start] = true;
    distance[start] = 0;
    q.push(start);

    cout << "BFS order: ";
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        cout << u << " ";

        for (int i = 0; i < (int)adj[u].size(); i++) {
            int v = adj[u][i];
            if (!visited[v]) {
                visited[v] = true;
                distance[v] = distance[u] + 1;
                parent[v] = u;
                q.push(v);
            }
        }
    }
    cout << endl << endl;

    cout << "Vertex   Distance from " << start << "   Parent" << endl;
    for (int i = 0; i < V; i++) {
        cout << "  " << i << "\t     " << distance[i] << "\t\t   " << parent[i] << endl;
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

    bfs(0);
    return 0;
}
