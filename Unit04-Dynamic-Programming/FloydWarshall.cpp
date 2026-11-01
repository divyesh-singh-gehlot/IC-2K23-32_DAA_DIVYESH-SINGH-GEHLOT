// All-Pairs Shortest Path using Floyd-Warshall (Dynamic Programming)
// Try every vertex k as an "in-between" vertex:
//   dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])

#include <iostream>
#include <iomanip>
using namespace std;

#define V 4
#define INF 99999

void printMatrix(int dist[V][V]) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF) {
                cout << setw(6) << "INF";
            } else {
                cout << setw(6) << dist[i][j];
            }
        }
        cout << endl;
    }
}

int main() {
    // graph[i][j] = weight of edge i -> j (INF means no edge)
    int dist[V][V] = {
        {0,   3,   INF, 7},
        {8,   0,   2,   INF},
        {5,   INF, 0,   1},
        {2,   INF, INF, 0}
    };

    cout << "Initial distance matrix:" << endl;
    printMatrix(dist);

    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
        cout << endl << "After using vertex " << k << " as intermediate:" << endl;
        printMatrix(dist);
    }

    // a negative number on the diagonal means a negative cycle
    bool negativeCycle = false;
    for (int i = 0; i < V; i++) {
        if (dist[i][i] < 0) {
            negativeCycle = true;
        }
    }

    cout << endl << "Final shortest distances between all pairs:" << endl;
    printMatrix(dist);
    cout << "Negative cycle present: " << (negativeCycle ? "Yes" : "No") << endl;
    return 0;
}
