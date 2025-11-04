#include <stdio.h>

// Function to perform LU Decomposition using Doolittle’s method
void luDecomposition(int n, double A[n][n], double L[n][n], double U[n][n]) {
    for (int i = 0; i < n; i++) {
        // Upper triangular matrix
        for (int k = i; k < n; k++) {
            double sum = 0;
            for (int j = 0; j < i; j++)
                sum += L[i][j] * U[j][k];
            U[i][k] = A[i][k] - sum;
        }

        // Lower triangular matrix
        for (int k = i; k < n; k++) {
            if (i == k)
                L[i][i] = 1;
            else {
                double sum = 0;
                for (int j = 0; j < i; j++)
                    sum += L[k][j] * U[j][i];
                L[k][i] = (A[k][i] - sum) / U[i][i];
            }
        }
    }
}

// Forward substitution: L * Y = B
void forwardSubstitution(int n, double L[n][n], double B[n], double Y[n]) {
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < i; j++)
            sum += L[i][j] * Y[j];
        Y[i] = B[i] - sum;
    }
}

// Backward substitution: U * X = Y
void backwardSubstitution(int n, double U[n][n], double Y[n], double X[n]) {
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0;
        for (int j = i + 1; j < n; j++)
            sum += U[i][j] * X[j];
        X[i] = (Y[i] - sum) / U[i][i];
    }
}

// Function to find the inverse of A using LU decomposition
void inverseMatrix(int n, double A[n][n], double inverse[n][n]) {
    double L[n][n], U[n][n];

    // Initialize L and U with 0
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            L[i][j] = 0;
            U[i][j] = 0;
        }

    luDecomposition(n, A, L, U);

    // Solve AX = I → For each column of I
    for (int i = 0; i < n; i++) {
        double e[n], y[n], x[n];

        for (int j = 0; j < n; j++) e[j] = 0;
        e[i] = 1;  // ith column of Identity

        forwardSubstitution(n, L, e, y);
        backwardSubstitution(n, U, y, x);

        for (int j = 0; j < n; j++)
            inverse[j][i] = x[j];
    }
}

// Function to print matrix
void printMatrix(int n, double A[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%10.4lf ", A[i][j]);
        printf("\n");
    }
}

int main() {
    int n;
    printf("Enter order of matrix (n x n): ");
    scanf("%d", &n);

    double A[n][n], inverse[n][n];

    printf("Enter matrix elements:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%lf", &A[i][j]);

    inverseMatrix(n, A, inverse);

    printf("\nInverse of the matrix is:\n");
    printMatrix(n, inverse);

    return 0;
}
