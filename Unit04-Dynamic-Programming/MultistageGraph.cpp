// Multistage Graph - shortest path from source to sink (Dynamic Programming)
// The vertices are numbered stage by stage, and edges only go forward.
// cost[j] = cheapest cost from vertex j to the last vertex
//         = min over edges (j, k) of  c[j][k] + cost[k]
// We fill cost[] from the last vertex back to the first.

#include <iostream>
using namespace std;

#define N 12          // number of vertices (1 to 12)
#define INF 99999

int main() {
    int c[N + 1][N + 1];
    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= N; j++) {
            c[i][j] = 0;              // 0 means there is no edge
        }
    }

    // edges: from, to, cost
    int edges[][3] = {
        {1, 2, 9}, {1, 3, 7}, {1, 4, 3}, {1, 5, 2},
        {2, 6, 4}, {2, 7, 2}, {2, 8, 1},
        {3, 6, 2}, {3, 7, 7},
        {4, 8, 11},
        {5, 7, 11}, {5, 8, 8},
        {6, 9, 6}, {6, 10, 5},
        {7, 9, 4}, {7, 10, 3},
        {8, 10, 5}, {8, 11, 6},
        {9, 12, 4}, {10, 12, 2}, {11, 12, 5}
    };
    int totalEdges = sizeof(edges) / sizeof(edges[0]);
    for (int i = 0; i < totalEdges; i++) {
        c[edges[i][0]][edges[i][1]] = edges[i][2];
    }

    cout << "Stage 1: 1" << endl;
    cout << "Stage 2: 2 3 4 5" << endl;
    cout << "Stage 3: 6 7 8" << endl;
    cout << "Stage 4: 9 10 11" << endl;
    cout << "Stage 5: 12" << endl << endl;

    int cost[N + 1];
    int next[N + 1];       // next[j] = next vertex on the best path from j

    cost[N] = 0;
    for (int j = N - 1; j >= 1; j--) {
        cost[j] = INF;
        for (int k = j + 1; k <= N; k++) {
            if (c[j][k] != 0 && c[j][k] + cost[k] < cost[j]) {
                cost[j] = c[j][k] + cost[k];
                next[j] = k;
            }
        }
    }

    cout << "Vertex   Cost to reach vertex 12   Next vertex" << endl;
    for (int j = 1; j < N; j++) {
        cout << "  " << j << "\t\t" << cost[j] << "\t\t" << next[j] << endl;
    }

    cout << endl << "Minimum cost from 1 to 12 = " << cost[1] << endl;
    cout << "Path: 1";
    int v = 1;
    while (v != N) {
        v = next[v];
        cout << " -> " << v;
    }
    cout << endl;
    return 0;
}
