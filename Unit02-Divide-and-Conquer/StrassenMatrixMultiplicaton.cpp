// Strassen's Matrix Multiplication (Divide and Conquer)
// Normal divide and conquer needs 8 multiplications of n/2 x n/2 matrices.
// Strassen reduces this to 7 multiplications (using extra additions),
// which gives T(n) = 7T(n/2) + O(n^2)  ->  O(n^2.81)
// Works for square matrices whose size is a power of 2 (2, 4, 8, ...)

#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

typedef vector< vector<int> > Matrix;

int multiplications = 0;   // counts single-number multiplications

Matrix makeMatrix(int n) {
    return Matrix(n, vector<int>(n, 0));
}

Matrix add(Matrix a, Matrix b, int n) {
    Matrix c = makeMatrix(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    return c;
}

Matrix subtract(Matrix a, Matrix b, int n) {
    Matrix c = makeMatrix(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = a[i][j] - b[i][j];
        }
    }
    return c;
}

// normal (school method) multiplication, used to check our answer
Matrix normalMultiply(Matrix a, Matrix b, int n) {
    Matrix c = makeMatrix(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

Matrix strassen(Matrix a, Matrix b, int n) {
    Matrix c = makeMatrix(n);

    // base case: 1 x 1 matrix
    if (n == 1) {
        multiplications++;
        c[0][0] = a[0][0] * b[0][0];
        return c;
    }

    int h = n / 2;

    // divide each matrix into 4 quarters
    Matrix a11 = makeMatrix(h), a12 = makeMatrix(h), a21 = makeMatrix(h), a22 = makeMatrix(h);
    Matrix b11 = makeMatrix(h), b12 = makeMatrix(h), b21 = makeMatrix(h), b22 = makeMatrix(h);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < h; j++) {
            a11[i][j] = a[i][j];
            a12[i][j] = a[i][j + h];
            a21[i][j] = a[i + h][j];
            a22[i][j] = a[i + h][j + h];

            b11[i][j] = b[i][j];
            b12[i][j] = b[i][j + h];
            b21[i][j] = b[i + h][j];
            b22[i][j] = b[i + h][j + h];
        }
    }

    // the 7 Strassen products
    Matrix m1 = strassen(add(a11, a22, h), add(b11, b22, h), h);
    Matrix m2 = strassen(add(a21, a22, h), b11, h);
    Matrix m3 = strassen(a11, subtract(b12, b22, h), h);
    Matrix m4 = strassen(a22, subtract(b21, b11, h), h);
    Matrix m5 = strassen(add(a11, a12, h), b22, h);
    Matrix m6 = strassen(subtract(a21, a11, h), add(b11, b12, h), h);
    Matrix m7 = strassen(subtract(a12, a22, h), add(b21, b22, h), h);

    // combine the products to get the 4 quarters of the answer
    Matrix c11 = add(subtract(add(m1, m4, h), m5, h), m7, h);
    Matrix c12 = add(m3, m5, h);
    Matrix c21 = add(m2, m4, h);
    Matrix c22 = add(subtract(add(m1, m3, h), m2, h), m6, h);

    // put the quarters back into one matrix
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < h; j++) {
            c[i][j] = c11[i][j];
            c[i][j + h] = c12[i][j];
            c[i + h][j] = c21[i][j];
            c[i + h][j + h] = c22[i][j];
        }
    }
    return c;
}

void printMatrix(Matrix m, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(5) << m[i][j];
        }
        cout << endl;
    }
}

bool sameMatrix(Matrix a, Matrix b, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i][j] != b[i][j]) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    int n = 4;

    // size must be a power of 2
    if ((n & (n - 1)) != 0) {
        cout << "Matrix size must be a power of 2" << endl;
        return 0;
    }

    Matrix a = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    Matrix b = {
        {1, 0, 2, 1},
        {3, 1, 0, 2},
        {0, 2, 1, 3},
        {1, 1, 2, 0}
    };

    cout << "Matrix A:" << endl;
    printMatrix(a, n);
    cout << endl << "Matrix B:" << endl;
    printMatrix(b, n);

    multiplications = 0;
    Matrix c = strassen(a, b, n);
    Matrix check = normalMultiply(a, b, n);

    cout << endl << "A x B using Strassen:" << endl;
    printMatrix(c, n);

    cout << endl << "Matches normal multiplication: "
         << (sameMatrix(c, check, n) ? "Yes" : "No") << endl;

    // compare the number of multiplications for different sizes
    cout << endl << "Size   Normal (n^3)   Strassen   Correct" << endl;
    for (int size = 2; size <= 16; size = size * 2) {
        Matrix x = makeMatrix(size);
        Matrix y = makeMatrix(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                x[i][j] = (i + j) % 5;
                y[i][j] = (i * 2 + j) % 7 - 3;
            }
        }
        multiplications = 0;
        Matrix s = strassen(x, y, size);
        bool ok = sameMatrix(s, normalMultiply(x, y, size), size);

        cout << setw(4) << size << setw(14) << size * size * size
             << setw(12) << multiplications << "   " << (ok ? "Yes" : "No") << endl;
    }
    return 0;
}