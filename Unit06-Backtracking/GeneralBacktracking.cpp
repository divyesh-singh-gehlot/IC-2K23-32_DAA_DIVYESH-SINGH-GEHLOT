// General Backtracking Technique
// The solution is built one step at a time as a vector x[1..n].
//
//   Backtrack(k):
//       for every possible value v for x[k]:
//           if v is allowed (bounding function):
//               x[k] = v                (choose)
//               if k == n: print solution
//               else:      Backtrack(k + 1)   (go deeper)
//               undo the choice         (backtrack)
//
// Example problem: arrange the numbers 1..n in a row (each used once) so that
// no two neighbouring numbers differ by exactly 1.
// The bounding function stops a bad arrangement as soon as it appears,
// instead of building all n! permutations and checking them at the end.

#include <iostream>
#include <cstdlib>
using namespace std;

int n = 4;
int x[10];
bool used[10];
int solutions = 0;
int nodesVisited = 0;     // how many partial arrangements were tried

// bounding function: can value v be placed at position k?
bool isAllowed(int k, int v) {
    if (used[v]) {
        return false;
    }
    if (k > 1 && abs(x[k - 1] - v) == 1) {
        return false;
    }
    return true;
}

void backtrack(int k) {
    for (int v = 1; v <= n; v++) {
        if (isAllowed(k, v)) {
            x[k] = v;                  // choose
            used[v] = true;
            nodesVisited++;

            if (k == n) {
                solutions++;
                cout << "Solution " << solutions << ": ";
                for (int i = 1; i <= n; i++) {
                    cout << x[i] << " ";
                }
                cout << endl;
            } else {
                backtrack(k + 1);      // go deeper
            }

            used[v] = false;           // undo the choice
        }
    }
}

int main() {
    for (int i = 0; i < 10; i++) {
        used[i] = false;
    }

    cout << "Arrangements of 1.." << n << " with no two neighbours differing by 1" << endl;
    backtrack(1);

    int factorial = 1;
    for (int i = 2; i <= n; i++) {
        factorial = factorial * i;
    }

    cout << endl << "Solutions found          : " << solutions << endl;
    cout << "Partial arrangements tried: " << nodesVisited << endl;
    cout << "Total permutations (n!)   : " << factorial << endl;
    return 0;
}
