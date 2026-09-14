// Maximum and Minimum using Divide and Conquer
// Split the array into two halves, find max/min of each half,
// then compare the two results to get the final max and min.

#include <iostream>
using namespace std;

int comparisons = 0;   // counts element comparisons

void maxMin(int a[], int low, int high, int &maximum, int &minimum) {
    // only one element
    if (low == high) {
        maximum = a[low];
        minimum = a[low];
        return;
    }

    // two elements: one comparison is enough
    if (high == low + 1) {
        comparisons++;
        if (a[low] > a[high]) {
            maximum = a[low];
            minimum = a[high];
        } else {
            maximum = a[high];
            minimum = a[low];
        }
        return;
    }

    // more than two elements: divide
    int mid = (low + high) / 2;
    int max1, min1, max2, min2;

    maxMin(a, low, mid, max1, min1);
    maxMin(a, mid + 1, high, max2, min2);

    // combine the answers of both halves
    comparisons++;
    if (max1 > max2) {
        maximum = max1;
    } else {
        maximum = max2;
    }

    comparisons++;
    if (min1 < min2) {
        minimum = min1;
    } else {
        minimum = min2;
    }
}

int main() {
    int a[] = {22, 13, -5, -8, 15, 60, 17, 31, 47};
    int n = 9;

    cout << "Array: ";
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    cout << endl << endl;

    int maximum, minimum;
    maxMin(a, 0, n - 1, maximum, minimum);

    cout << "Maximum = " << maximum << endl;
    cout << "Minimum = " << minimum << endl;
    cout << endl;
    cout << "Comparisons (divide and conquer) = " << comparisons << endl;
    cout << "Comparisons (simple loop)        = " << 2 * (n - 1) << endl;
    return 0;
}