// Single-Source Shortest Path
// Part 1: Bellman-Ford  (works with negative edges, detects negative cycles)
// Part 2: Dijkstra      (faster, but only for non-negative edge weights)

#include <iostream>
using namespace std;

#define INF 99999

struct Edge {
    int from, to, weight;
};

void printDistances(int dist[], int V, int source) {
    for (int i = 0; i < V; i++) {
        cout << "  " << source << " -> " << i << " : ";
        if (dist[i] == INF) {
            cout << "unreachable" << endl;
        } else {
            cout << dist[i] << endl;
        }
    }
}

void bellmanFord(int V, Edge edges[], int E, int source) {
    int dist[10];
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
    }
    dist[source] = 0;

    // relax all edges V-1 times
    for (int round = 1; round <= V - 1; round++) {
        for (int e = 0; e < E; e++) {
            if (dist[edges[e].from] != INF &&
                dist[edges[e].from] + edges[e].weight < dist[edges[e].to]) {
                dist[edges[e].to] = dist[edges[e].from] + edges[e].weight;
            }
        }
    }

    // if any edge can still be relaxed, there is a negative cycle
    for (int e = 0; e < E; e++) {
        if (dist[edges[e].from] != INF &&
            dist[edges[e].from] + edges[e].weight < dist[edges[e].to]) {
            cout << "  Negative cycle found!" << endl;
            return;
        }
    }
    printDistances(dist, V, source);
}

void dijkstra(int V, Edge edges[], int E, int source) {
    int graph[10][10];
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph[i][j] = 0;
        }
    }
    for (int e = 0; e < E; e++) {
        graph[edges[e].from][edges[e].to] = edges[e].weight;
    }

    int dist[10];
    bool done[10];
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        done[i] = false;
    }
    dist[source] = 0;

    for (int count = 0; count < V; count++) {
        // pick the nearest vertex that is not finished yet
        int u = -1;
        for (int i = 0; i < V; i++) {
            if (!done[i] && (u == -1 || dist[i] < dist[u])) {
                u = i;
            }
        }
        if (dist[u] == INF) {
            break;
        }
        done[u] = true;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] != 0 && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
    printDistances(dist, V, source);
}

int main() {
    // graph 1: all weights positive
    Edge g1[] = {
        {0, 1, 10}, {0, 2, 5}, {1, 2, 2}, {1, 3, 1}, {2, 1, 3},
        {2, 3, 9}, {2, 4, 2}, {3, 4, 4}, {4, 3, 6}, {4, 0, 7}
    };
    int e1 = sizeof(g1) / sizeof(g1[0]);

    cout << "Graph 1 (positive weights), source = 0" << endl;
    cout << "Bellman-Ford:" << endl;
    bellmanFord(5, g1, e1, 0);
    cout << "Dijkstra:" << endl;
    dijkstra(5, g1, e1, 0);

    // graph 2: has a negative edge, so only Bellman-Ford can be used
    Edge g2[] = {{0, 1, 4}, {0, 2, 5}, {1, 2, -3}, {2, 3, 2}};
    int e2 = sizeof(g2) / sizeof(g2[0]);

    cout << endl << "Graph 2 (one negative edge), source = 0" << endl;
    cout << "Bellman-Ford:" << endl;
    bellmanFord(4, g2, e2, 0);

    // graph 3: negative cycle 1 -> 2 -> 3 -> 1
    Edge g3[] = {{0, 1, 1}, {1, 2, -1}, {2, 3, -1}, {3, 1, -1}};
    int e3 = sizeof(g3) / sizeof(g3[0]);

    cout << endl << "Graph 3 (negative cycle), source = 0" << endl;
    cout << "Bellman-Ford:" << endl;
    bellmanFord(4, g3, e3, 0);
    return 0;
}
