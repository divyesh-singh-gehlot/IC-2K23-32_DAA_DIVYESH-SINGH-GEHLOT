// Travelling Salesperson Problem using Branch and Bound
// We build the tour city by city (the search tree). At every step we find a
// LOWER BOUND on the cost of the best tour that can still be made:
//   cost so far + (cheapest edge leaving the current city)
//               + (cheapest edge leaving each city not visited yet)
// If this bound is not smaller than the best complete tour found so far,
// that whole branch is cut off.

#include <iostream>
using namespace std;

#define INF 999999

int n = 4;
int dist[4][4] = {
    {0, 10, 15, 20},
    {10, 0, 35, 25},
    {15, 35, 0, 30},
    {20, 25, 30, 0}
};

int minOut[4];            // cheapest edge leaving each city
bool visited[4];
int path[4];
int bestPath[4];
int best = INF;
long long nodes = 0;      // nodes of the search tree that were visited

void branchAndBound(int city, int count, int cost) {
    nodes++;

    // all cities visited: add the road back to the start
    if (count == n) {
        int total = cost + dist[city][0];
        if (total < best) {
            best = total;
            for (int i = 0; i < n; i++) {
                bestPath[i] = path[i];
            }
        }
        return;
    }

    for (int next = 1; next < n; next++) {
        if (visited[next]) {
            continue;
        }

        int newCost = cost + dist[city][next];

        // lower bound for any tour that continues from here
        int bound = newCost + minOut[next];
        for (int u = 1; u < n; u++) {
            if (!visited[u] && u != next) {
                bound = bound + minOut[u];
            }
        }

        if (bound < best) {                 // otherwise: prune this branch
            visited[next] = true;
            path[count] = next;
            branchAndBound(next, count + 1, newCost);
            visited[next] = false;
        }
    }
}

int main() {
    for (int i = 0; i < n; i++) {
        minOut[i] = INF;
        for (int j = 0; j < n; j++) {
            if (i != j && dist[i][j] < minOut[i]) {
                minOut[i] = dist[i][j];
            }
        }
        visited[i] = false;
    }

    visited[0] = true;      // start from city 0
    path[0] = 0;
    branchAndBound(0, 1, 0);

    cout << "Minimum tour cost = " << best << endl;
    cout << "Tour: ";
    for (int i = 0; i < n; i++) {
        cout << bestPath[i] << " -> ";
    }
    cout << "0" << endl;

    long long fullTree = 1;
    for (int i = 1; i < n; i++) {
        fullTree = fullTree * i;
    }
    cout << "Search tree nodes visited: " << nodes << endl;
    cout << "Complete tours without pruning: " << fullTree << endl;
    return 0;
}
