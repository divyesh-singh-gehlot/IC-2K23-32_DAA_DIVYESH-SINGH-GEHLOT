// Sum of Subsets using Backtracking
// Find every subset of the given numbers whose sum is exactly m.
// The numbers are sorted in increasing order. We decide for each number
// "include" or "exclude" and stop early whenever the sum cannot work out.

#include <iostream>
using namespace std;

int w[] = {5, 10, 12, 13, 15, 18};     // must be in increasing order
int n = 6;
int m = 30;                            // target sum
int x[10];                             // x[i] = 1 if w[i] is taken
int solutions = 0;

void printSubset(int last) {
    solutions++;
    cout << "Subset " << solutions << ": { ";
    for (int i = 0; i <= last; i++) {
        if (x[i] == 1) {
            cout << w[i] << " ";
        }
    }
    cout << "}" << endl;
}

// sum       = sum of the numbers chosen so far
// remaining = sum of the numbers from position k to the end
void sumOfSubsets(int k, int sum, int remaining) {
    // try to include w[k]
    x[k] = 1;
    if (sum + w[k] == m) {
        printSubset(k);                     // found a subset
    } else if (k + 1 < n && sum + w[k] + w[k + 1] <= m) {
        sumOfSubsets(k + 1, sum + w[k], remaining - w[k]);
    }

    // try to exclude w[k]
    // (only useful if the rest can still reach m, and the next number fits)
    if (k + 1 < n && sum + remaining - w[k] >= m && sum + w[k + 1] <= m) {
        x[k] = 0;
        sumOfSubsets(k + 1, sum, remaining - w[k]);
    }
}

int main() {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total = total + w[i];
    }

    cout << "Numbers: ";
    for (int i = 0; i < n; i++) {
        cout << w[i] << " ";
    }
    cout << endl << "Target sum: " << m << endl << endl;

    if (total < m || w[0] > m) {
        cout << "No subset is possible" << endl;
        return 0;
    }

    sumOfSubsets(0, 0, total);

    if (solutions == 0) {
        cout << "No subset adds up to " << m << endl;
    }
    return 0;
}
