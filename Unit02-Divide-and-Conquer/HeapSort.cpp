// Heap Sort
// 1. Build a max heap from the array.
// 2. Swap the root (largest) with the last element, reduce the heap size,
//    and fix the heap again. Repeat until the array is sorted.

#include <iostream>
using namespace std;

void printArray(int a[], int n) {
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    cout << endl;
}

// make the subtree rooted at index i a max heap (size of heap is n)
void heapify(int a[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && a[left] > a[largest]) {
        largest = left;
    }
    if (right < n && a[right] > a[largest]) {
        largest = right;
    }

    if (largest != i) {
        int temp = a[i];
        a[i] = a[largest];
        a[largest] = temp;

        // the swapped child may break its own subtree, so fix that too
        heapify(a, n, largest);
    }
}

void heapSort(int a[], int n) {
    // build the max heap (start from the last non-leaf node)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(a, n, i);
    }
    cout << "After building max heap: ";
    printArray(a, n);
    cout << endl;

    // move the largest element to the end, one by one
    for (int i = n - 1; i > 0; i--) {
        int temp = a[0];
        a[0] = a[i];
        a[i] = temp;

        heapify(a, i, 0);

        cout << "After step " << n - i << ": ";
        printArray(a, n);
    }
}

int main() {
    int a[] = {12, 11, 13, 5, 6, 7, 15, 3};
    int n = 8;

    cout << "Original array: ";
    printArray(a, n);
    cout << endl;

    heapSort(a, n);

    cout << endl << "Sorted array: ";
    printArray(a, n);
    return 0;
}