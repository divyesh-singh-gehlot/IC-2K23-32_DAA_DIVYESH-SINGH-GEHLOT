// Fibonacci using Recursion
// fib(n) = fib(n-1) + fib(n-2), with fib(0) = 0 and fib(1) = 1
// The same values are calculated again and again, so it becomes very slow.

#include <iostream>
using namespace std;

long long calls = 0;   // counts how many times fib() is called

long long fib(int n) {
    calls++;
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

int main() {
    int n = 10;

    cout << "Fibonacci series of " << n << " terms: ";
    for (int i = 0; i < n; i++) {
        cout << fib(i) << " ";
    }
    cout << endl << endl;

    cout << "n    fib(n)    function calls" << endl;
    for (int i = 10; i <= 35; i = i + 5) {
        calls = 0;
        long long result = fib(i);
        cout << i << "    " << result << "    " << calls << endl;
    }
    return 0;
}
