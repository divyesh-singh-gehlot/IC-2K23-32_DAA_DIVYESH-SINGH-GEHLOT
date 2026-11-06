// Exhaustive Search - Assignment Problem
// There are 4 workers and 4 jobs. cost[i][j] is the cost if worker i does job j.
// Every worker must get exactly one job. Find the cheapest assignment.
// Exhaustive search tries EVERY possible assignment (all n! permutations)
// and keeps the best one.

#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    int n = 4;
    int cost[4][4] = {
        {9, 2, 7, 8},
        {6, 4, 3, 7},
        {5, 8, 1, 8},
        {7, 6, 9, 4}
    };

    int job[4];                  // job[i] = job given to worker i
    for (int i = 0; i < n; i++) {
        job[i] = i;
    }

    int bestCost = 999999;
    int bestJob[4];
    int tried = 0;

    // next_permutation gives every ordering of the jobs, one by one
    do {
        tried++;
        int total = 0;
        for (int i = 0; i < n; i++) {
            total = total + cost[i][job[i]];
        }

        if (total < bestCost) {
            bestCost = total;
            for (int i = 0; i < n; i++) {
                bestJob[i] = job[i];
            }
        }
    } while (next_permutation(job, job + n));

    cout << "Cost matrix (rows = workers, columns = jobs):" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << cost[i][j] << "  ";
        }
        cout << endl;
    }

    cout << endl << "Assignments tried: " << tried << endl;
    cout << "Cheapest total cost: " << bestCost << endl;
    for (int i = 0; i < n; i++) {
        cout << "  Worker " << i + 1 << " -> Job " << bestJob[i] + 1
             << " (cost " << cost[i][bestJob[i]] << ")" << endl;
    }
    return 0;
}
