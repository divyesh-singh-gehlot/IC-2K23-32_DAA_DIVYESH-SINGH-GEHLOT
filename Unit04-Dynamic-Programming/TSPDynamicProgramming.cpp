// Travelling Salesperson Problem using Dynamic Programming (Held-Karp)
// dp[mask][i] = cheapest cost to start at city 0, visit exactly the cities
//               in "mask", and end at city i.
// A bit in mask is 1 if that city has been visited.

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define INF 999999

int main() {
    int n = 4;
    int dist[4][4] = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    int full = (1 << n) - 1;
    vector< vector<int> > dp(1 << n, vector<int>(n, INF));
    vector< vector<int> > parent(1 << n, vector<int>(n, -1));

    dp[1][0] = 0;     // only city 0 visited, standing at city 0

    for (int mask = 1; mask <= full; mask++) {
        for (int last = 0; last < n; last++) {
            if (!(mask & (1 << last)) || dp[mask][last] == INF) {
                continue;
            }
            // try to go to a city that is not visited yet
            for (int next = 0; next < n; next++) {
                if (mask & (1 << next)) {
                    continue;
                }
                int newMask = mask | (1 << next);
                int newCost = dp[mask][last] + dist[last][next];
                if (newCost < dp[newMask][next]) {
                    dp[newMask][next] = newCost;
                    parent[newMask][next] = last;
                }
            }
        }
    }

    // finish by returning to city 0
    int best = INF;
    int lastCity = -1;
    for (int i = 1; i < n; i++) {
        if (dp[full][i] + dist[i][0] < best) {
            best = dp[full][i] + dist[i][0];
            lastCity = i;
        }
    }

    // find the tour by going back through parent[][]
    vector<int> path;
    int mask = full;
    int city = lastCity;
    while (city != -1) {
        path.push_back(city);
        int previous = parent[mask][city];
        mask = mask ^ (1 << city);
        city = previous;
    }
    reverse(path.begin(), path.end());

    cout << "Minimum tour cost = " << best << endl;
    cout << "Tour: ";
    for (int i = 0; i < (int)path.size(); i++) {
        cout << path[i] << " -> ";
    }
    cout << "0" << endl;
    return 0;
}
