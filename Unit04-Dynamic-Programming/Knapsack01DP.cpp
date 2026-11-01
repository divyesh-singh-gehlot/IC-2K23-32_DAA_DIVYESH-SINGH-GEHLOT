// 0/1 Knapsack using Dynamic Programming
// dp[i][w] = best value using the first i items with capacity w.
// For every item we either skip it, or take it (only if it fits):
//   dp[i][w] = max(dp[i-1][w], value[i] + dp[i-1][w - weight[i]])

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    int n = 4;
    int capacity = 8;
    int weight[5] = {0, 2, 3, 4, 5};    // index 0 is not used
    int value[5]  = {0, 3, 4, 5, 6};

    int dp[5][9];

    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            } else if (weight[i] <= w) {
                int take = value[i] + dp[i - 1][w - weight[i]];
                int skip = dp[i - 1][w];
                dp[i][w] = (take > skip) ? take : skip;
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    cout << "Items (weight, value): ";
    for (int i = 1; i <= n; i++) {
        cout << "(" << weight[i] << "," << value[i] << ") ";
    }
    cout << endl << "Capacity: " << capacity << endl << endl;

    cout << "DP table (rows = items, columns = capacity 0.." << capacity << ")" << endl;
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            cout << setw(4) << dp[i][w];
        }
        cout << endl;
    }

    cout << endl << "Maximum value = " << dp[n][capacity] << endl;

    // go back through the table to find which items were taken
    cout << "Items taken: ";
    int w = capacity;
    for (int i = n; i >= 1; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            cout << i << " ";
            w = w - weight[i];
        }
    }
    cout << endl;
    return 0;
}
