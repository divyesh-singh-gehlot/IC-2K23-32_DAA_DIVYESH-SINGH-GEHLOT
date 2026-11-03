// Graph representation using an Adjacency Matrix
// matrix[i][j] = 1 if there is an edge between vertex i and vertex j.
// Needs V x V space, but checking if an edge exists takes only O(1).

#include <iostream>
using namespace std;

#define V 6

int matrix[V][V];

void addEdge(int a, int b) {
    matrix[a][b] = 1;
    matrix[b][a] = 1;          // undirected graph, so both directions
}

void removeEdge(int a, int b) {
    matrix[a][b] = 0;
    matrix[b][a] = 0;
}

void printMatrix() {
    cout << "   ";
    for (int j = 0; j < V; j++) {
        cout << j << " ";
    }
    cout << endl;
    for (int i = 0; i < V; i++) {
        cout << i << "  ";
        for (int j = 0; j < V; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            matrix[i][j] = 0;
        }
    }

    addEdge(0, 1);
    addEdge(0, 2);
    addEdge(1, 3);
    addEdge(1, 4);
    addEdge(2, 4);
    addEdge(3, 5);
    addEdge(4, 5);

    cout << "Adjacency matrix:" << endl;
    printMatrix();

    cout << endl << "Degree of each vertex:" << endl;
    for (int i = 0; i < V; i++) {
        int degree = 0;
        for (int j = 0; j < V; j++) {
            degree = degree + matrix[i][j];
        }
        cout << "  vertex " << i << " : " << degree << endl;
    }

    cout << endl << "Is there an edge between 1 and 4? " << (matrix[1][4] ? "Yes" : "No") << endl;
    cout << "Is there an edge between 0 and 5? " << (matrix[0][5] ? "Yes" : "No") << endl;

    removeEdge(1, 4);
    cout << endl << "After removing edge 1-4:" << endl;
    printMatrix();
    return 0;
}
