// Fibonacci using Memoization (top-down Dynamic Programming)
// Every answer is stored in an array the first time it is calculated.
// Next time we just read it from the array instead of calculating again.

#include <iostream>
using namespace std;

long long memo[100];   // memo[i] = fib(i), -1 means "not calculated yet"
long long calls = 0;

long long fib(int n) {
    calls++;
    if (n <= 1) {
        return n;
    }
    if (memo[n] != -1) {
        return memo[n];          // already known
    }
    memo[n] = fib(n - 1) + fib(n - 2);
    return memo[n];
}

void resetMemo() {
    for (int i = 0; i < 100; i++) {
        memo[i] = -1;
    }
    calls = 0;
}

int main() {
    int n = 10;

    resetMemo();
    cout << "Fibonacci series of " << n << " terms: ";
    for (int i = 0; i < n; i++) {
        cout << fib(i) << " ";
    }
    cout << endl << endl;

    cout << "n    fib(n)                function calls" << endl;
    int sizes[6] = {10, 20, 30, 40, 50, 90};
    for (int i = 0; i < 6; i++) {
        resetMemo();
        long long result = fib(sizes[i]);
        cout << sizes[i] << "    " << result << "    " << calls << endl;
    }
    return 0;
}
