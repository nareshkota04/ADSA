#include <stdio.h>
#include <stdlib.h>

#define MAX 8  // maximum matrix size (must be power of 2 for simplicity)

// Function to add two matrices
void addMatrix(int n, int A[MAX][MAX], int B[MAX][MAX], int C[MAX][MAX]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

// Function to subtract two matrices
void subMatrix(int n, int A[MAX][MAX], int B[MAX][MAX], int C[MAX][MAX]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

// Strassen’s matrix multiplication algorithm
void strassen(int n, int A[MAX][MAX], int B[MAX][MAX], int C[MAX][MAX]) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int newSize = n / 2;
    int A11[MAX][MAX], A12[MAX][MAX], A21[MAX][MAX], A22[MAX][MAX];
    int B11[MAX][MAX], B12[MAX][MAX], B21[MAX][MAX], B22[MAX][MAX];
    int M1[MAX][MAX], M2[MAX][MAX], M3[MAX][MAX], M4[MAX][MAX];
    int M5[MAX][MAX], M6[MAX][MAX], M7[MAX][MAX];
    int temp1[MAX][MAX], temp2[MAX][MAX];

    // Divide matrices A and B into 4 submatrices
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }
    }

    // M1 = (A11 + A22) * (B11 + B22)
    addMatrix(newSize, A11, A22, temp1);
    addMatrix(newSize, B11, B22, temp2);
    strassen(newSize, temp1, temp2, M1);

    // M2 = (A21 + A22) * B11
    addMatrix(newSize, A21, A22, temp1);
    strassen(newSize, temp1, B11, M2);

    // M3 = A11 * (B12 - B22)
    subMatrix(newSize, B12, B22, temp2);
    strassen(newSize, A11, temp2, M3);

    // M4 = A22 * (B21 - B11)
    subMatrix(newSize, B21, B11, temp2);
    strassen(newSize, A22, temp2, M4);

    // M5 = (A11 + A12) * B22
    addMatrix(newSize, A11, A12, temp1);
    strassen(newSize, temp1, B22, M5);

    // M6 = (A21 - A11) * (B11 + B12)
    subMatrix(newSize, A21, A11, temp1);
    addMatrix(newSize, B11, B12, temp2);
    strassen(newSize, temp1, temp2, M6);

    // M7 = (A12 - A22) * (B21 + B22)
    subMatrix(newSize, A12, A22, temp1);
    addMatrix(newSize, B21, B22, temp2);
    strassen(newSize, temp1, temp2, M7);

    // C11 = M1 + M4 - M5 + M7
    addMatrix(newSize, M1, M4, temp1);
    subMatrix(newSize, temp1, M5, temp2);
    addMatrix(newSize, temp2, M7, C);

    // C12 = M3 + M5
    addMatrix(newSize, M3, M5, temp1);
    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++)
            C[i][j + newSize] = temp1[i][j];

    // C21 = M2 + M4
    addMatrix(newSize, M2, M4, temp1);
    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++)
            C[i + newSize][j] = temp1[i][j];

    // C22 = M1 - M2 + M3 + M6
    subMatrix(newSize, M1, M2, temp1);
    addMatrix(newSize, temp1, M3, temp2);
    addMatrix(newSize, temp2, M6, temp1);
    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++)
            C[i + newSize][j + newSize] = temp1[i][j];
}

int main() {
    int n;
    int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX] = {0};

    printf("Enter the size of matrix (power of 2): ");
    scanf("%d", &n);

    printf("Enter elements of matrix A:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    printf("Enter elements of matrix B:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &B[i][j]);

    strassen(n, A, B, C);

    printf("\nResultant Matrix (A × B):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", C[i][j]);
        printf("\n");
    }

    return 0;
}