// Comparison of TSP: Dynamic Programming (Held-Karp) vs Branch and Bound
// Both methods find the cheapest tour on the same random distance matrix.
// We compare the tour cost (must be equal), the time taken, and the work:
//   Dynamic Programming : n * 2^n states
//   Branch and Bound    : search-tree nodes visited

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
using namespace std;

#define INF 999999

// ---------------- Dynamic Programming ----------------
int tspDP(vector< vector<int> >& dist) {
    int n = dist.size();
    int full = (1 << n) - 1;
    vector< vector<int> > dp(1 << n, vector<int>(n, INF));
    dp[1][0] = 0;

    for (int mask = 1; mask <= full; mask++) {
        for (int last = 0; last < n; last++) {
            if (!(mask & (1 << last)) || dp[mask][last] == INF) {
                continue;
            }
            for (int next = 0; next < n; next++) {
                if (mask & (1 << next)) {
                    continue;
                }
                int newMask = mask | (1 << next);
                int newCost = dp[mask][last] + dist[last][next];
                if (newCost < dp[newMask][next]) {
                    dp[newMask][next] = newCost;
                }
            }
        }
    }

    int best = INF;
    for (int i = 1; i < n; i++) {
        if (dp[full][i] + dist[i][0] < best) {
            best = dp[full][i] + dist[i][0];
        }
    }
    return best;
}

// ---------------- Branch and Bound ----------------
int n;
vector< vector<int> > dist;
vector<int> minOut;
vector<bool> visited;
int best;
long long nodes;

void branchAndBound(int city, int count, int cost) {
    nodes++;
    if (count == n) {
        int total = cost + dist[city][0];
        if (total < best) {
            best = total;
        }
        return;
    }

    for (int next = 1; next < n; next++) {
        if (visited[next]) {
            continue;
        }
        int newCost = cost + dist[city][next];

        int bound = newCost + minOut[next];
        for (int u = 1; u < n; u++) {
            if (!visited[u] && u != next) {
                bound = bound + minOut[u];
            }
        }

        if (bound < best) {
            visited[next] = true;
            branchAndBound(next, count + 1, newCost);
            visited[next] = false;
        }
    }
}

int tspBB(vector< vector<int> >& matrix) {
    n = matrix.size();
    dist = matrix;
    minOut.assign(n, INF);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && dist[i][j] < minOut[i]) {
                minOut[i] = dist[i][j];
            }
        }
    }
    visited.assign(n, false);
    visited[0] = true;
    best = INF;
    nodes = 0;
    branchAndBound(0, 1, 0);
    return best;
}

// ---------------- test data ----------------
unsigned long long seed = 2024;

int randomNumber(int low, int high) {
    seed = (seed * 1103515245ULL + 12345ULL) % 2147483648ULL;
    return low + (int)(seed % (high - low + 1));
}

vector< vector<int> > randomCities(int cities) {
    vector< vector<int> > d(cities, vector<int>(cities, 0));
    for (int i = 0; i < cities; i++) {
        for (int j = i + 1; j < cities; j++) {
            d[i][j] = randomNumber(10, 100);
            d[j][i] = d[i][j];          // symmetric distances
        }
    }
    return d;
}

int main() {
    cout << "cities  DP cost  DP time(ms)  DP states  B&B cost  B&B time(ms)  B&B nodes  Same?" << endl;

    int sizes[5] = {6, 8, 10, 12, 14};
    for (int s = 0; s < 5; s++) {
        int cities = sizes[s];
        vector< vector<int> > matrix = randomCities(cities);

        auto t1 = chrono::high_resolution_clock::now();
        int dpCost = tspDP(matrix);
        auto t2 = chrono::high_resolution_clock::now();
        int bbCost = tspBB(matrix);
        auto t3 = chrono::high_resolution_clock::now();

        double dpTime = chrono::duration<double, milli>(t2 - t1).count();
        double bbTime = chrono::duration<double, milli>(t3 - t2).count();

        cout << setw(6) << cities << setw(9) << dpCost << setw(13) << fixed << setprecision(3) << dpTime
             << setw(11) << cities * (1 << cities)
             << setw(10) << bbCost << setw(14) << bbTime
             << setw(11) << nodes << setw(8) << ((dpCost == bbCost) ? "Yes" : "NO!") << endl;
    }
    return 0;
}
