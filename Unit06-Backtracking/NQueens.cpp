// N-Queens Problem using Backtracking
// The user enters N. The program counts all solutions and prints the first one.

#include <iostream>
#include <cstdlib>
using namespace std;

int n;
int col[20];
int solutions = 0;
bool firstPrinted = false;

bool isSafe(int row, int c) {
    for (int prev = 0; prev < row; prev++) {
        if (col[prev] == c || abs(col[prev] - c) == row - prev) {
            return false;
        }
    }
    return true;
}

void printBoard() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
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
    if (row == n) {
        solutions++;
        if (!firstPrinted) {
            cout << "First solution:" << endl;
            printBoard();
            firstPrinted = true;
        }
        return;
    }

    for (int c = 0; c < n; c++) {
        if (isSafe(row, c)) {
            col[row] = c;
            solve(row + 1);
        }
    }
}

int main() {
    cout << "Enter N (1 to 15): ";
    cin >> n;

    if (cin.fail() || n < 1 || n > 15) {
        cout << "Invalid value of N" << endl;
        return 0;
    }

    solve(0);

    if (solutions == 0) {
        cout << "No solution exists for N = " << n << endl;
    } else {
        cout << endl << "Total solutions for N = " << n << ": " << solutions << endl;
    }
    return 0;
}
