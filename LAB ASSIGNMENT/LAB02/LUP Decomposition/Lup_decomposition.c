#include <stdio.h>

// Function for LU Decomposition
int lu_decomposition(int n, double A[n][n], double L[n][n], double U[n][n]) {
    // Initialize L and U
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            L[i][j] = 0;
            U[i][j] = 0;
        }
        L[i][i] = 1.0;  // diagonal of L = 1
    }

  
    for (int i = 0; i < n; i++) {
        // Compute U row
        for (int k = i; k < n; k++) {
            double sum = 0;
            for (int j = 0; j < i; j++)
                sum += L[i][j] * U[j][k];
            U[i][k] = A[i][k] - sum;
        }

        if (U[i][i] == 0.0) {   // direct zero check
            return -1; // pivot is zero → decomposition fails
        }

        // Compute L column
        for (int k = i + 1; k < n; k++) {
            double sum = 0;
            for (int j = 0; j < i; j++)
                sum += L[k][j] * U[j][i];
            L[k][i] = (A[k][i] - sum) / U[i][i];
        }
    }
    return 0; // success
}

void print_matrix(int n, double M[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%8.1f ", M[i][j]);
        printf("\n");
    }
}

int main() {
    int n;
    printf("Enter the order of matrix n: ");
    scanf("%d", &n);

    double A[n][n], L[n][n], U[n][n];  

    printf("Enter the %dx%d matrix A:\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%lf", &A[i][j]);

    // Perform LU decomposition
    if (lu_decomposition(n, A, L, U) != 0) {
        printf("LU decomposition failed (zero pivot encountered).\n");
        return 1;
    }

    // Print results
    printf("Matrix L:\n");
    print_matrix(n, L);
    printf("Matrix U:\n");
    print_matrix(n, U);

    return 0;
}
