// 8-Queens Problem using Backtracking
// Same idea as 4-Queens on an 8 x 8 board. We count all the solutions
// and print the first one.

#include <iostream>
#include <cstdlib>
using namespace std;

#define N 8

int col[N];
int solutions = 0;
long long placementsTried = 0;     // how many times isSafe() was called

bool isSafe(int row, int c) {
    placementsTried++;
    for (int prev = 0; prev < row; prev++) {
        if (col[prev] == c || abs(col[prev] - c) == row - prev) {
            return false;
        }
    }
    return true;
}

void printBoard() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (col[i] == j) {
                cout << "Q ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
}

void solve(int row) {
    if (row == N) {
        solutions++;
        if (solutions == 1) {
            cout << "First solution:" << endl;
            printBoard();
            cout << "Queen columns (row by row): ";
            for (int i = 0; i < N; i++) {
                cout << col[i] + 1 << " ";
            }
            cout << endl;
        }
        return;
    }

    for (int c = 0; c < N; c++) {
        if (isSafe(row, c)) {
            col[row] = c;
            solve(row + 1);
        }
    }
}

int main() {
    solve(0);
    cout << endl << "Total solutions for " << N << "-Queens: " << solutions << endl;
    cout << "Safety checks made: " << placementsTried << endl;
    return 0;
}
