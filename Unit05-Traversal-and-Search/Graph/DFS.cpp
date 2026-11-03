// Depth First Search (DFS)
// Go as deep as possible along one path, then come back (backtrack).
//   1. Recursive DFS  - uses the function call stack
//   2. Iterative DFS  - uses our own stack

#include <iostream>
#include <stack>
#include <vector>
using namespace std;

#define V 8       // vertices 6 and 7 form a separate part of the graph

vector<int> adj[V];
bool visited[V];

void addEdge(int a, int b) {
    adj[a].push_back(b);
    adj[b].push_back(a);
}

void resetVisited() {
    for (int i = 0; i < V; i++) {
        visited[i] = false;
    }
}

// ---------- recursive ----------
void dfsRecursive(int u) {
    visited[u] = true;
    cout << u << " ";
    for (int i = 0; i < (int)adj[u].size(); i++) {
        int v = adj[u][i];
        if (!visited[v]) {
            dfsRecursive(v);
        }
    }
}

// ---------- iterative ----------
void dfsIterative(int start) {
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int u = s.top();
        s.pop();

        if (visited[u]) {
            continue;          // a vertex can be pushed more than once
        }
        visited[u] = true;
        cout << u << " ";

        // push neighbours in reverse order so that the first neighbour
        // is taken out first (same order as the recursive version)
        for (int i = (int)adj[u].size() - 1; i >= 0; i--) {
            if (!visited[adj[u][i]]) {
                s.push(adj[u][i]);
            }
        }
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
    addEdge(6, 7);

    resetVisited();
    cout << "Recursive DFS from 0: ";
    dfsRecursive(0);
    cout << endl;

    resetVisited();
    cout << "Iterative DFS from 0: ";
    dfsIterative(0);
    cout << endl;

    // this graph has two separate parts, so DFS must be started again for each part
    cout << endl << "Starting DFS from every unvisited vertex (all components):" << endl;
    resetVisited();
    int components = 0;
    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            components++;
            cout << "  component " << components << ": ";
            dfsRecursive(i);
            cout << endl;
        }
    }
    return 0;
}
