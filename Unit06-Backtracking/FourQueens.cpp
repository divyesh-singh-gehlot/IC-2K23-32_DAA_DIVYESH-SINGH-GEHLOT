// 4-Queens Problem using Backtracking
// Place 4 queens on a 4 x 4 board so that no two queens attack each other
// (no two in the same row, column or diagonal).
// One queen is placed in each row; col[row] = column of that queen.

#include <iostream>
#include <cstdlib>
using namespace std;

#define N 4

int col[N];
int solutions = 0;

// can a queen be placed at (row, c)?
bool isSafe(int row, int c) {
    for (int prev = 0; prev < row; prev++) {
        if (col[prev] == c) {
            return false;                          // same column
        }
        if (abs(col[prev] - c) == row - prev) {
            return false;                          // same diagonal
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
        cout << "Solution " << solutions << ":" << endl;
        printBoard();
        cout << endl;
        return;
    }

    for (int c = 0; c < N; c++) {
        if (isSafe(row, c)) {
            col[row] = c;          // place the queen
            solve(row + 1);        // go to the next row
        }                          // if not safe, try the next column
    }
}

int main() {
    solve(0);
    cout << "Total solutions for " << N << "-Queens: " << solutions << endl;
    return 0;
}
