#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to allocate a matrix
int** allocateMatrix(int n) {
    int **mat = (int**)malloc(n * sizeof(int*));
    for(int i=0; i<n; i++)
        mat[i] = (int*)calloc(n, sizeof(int));
    return mat;
}

// Print matrix
void printMatrix(int **A, int n) {
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++)
            printf("%4d ", A[i][j]);
        printf("\n");
    }
}



void addMatrix(int **A, int **B, int **C, int n) {
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

void subMatrix(int **A, int **B, int **C, int n) {
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            C[i][j] = A[i][j] - B[i][j];
}


void strassenMultiply(int **A, int **B, int **C, int n) {
    if(n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int k = n/2;
    int **A11 = allocateMatrix(k), **A12 = allocateMatrix(k);
    int **A21 = allocateMatrix(k), **A22 = allocateMatrix(k);
    int **B11 = allocateMatrix(k), **B12 = allocateMatrix(k);
    int **B21 = allocateMatrix(k), **B22 = allocateMatrix(k);
    int **M1 = allocateMatrix(k), **M2 = allocateMatrix(k), **M3 = allocateMatrix(k),
         **M4 = allocateMatrix(k), **M5 = allocateMatrix(k), **M6 = allocateMatrix(k), **M7 = allocateMatrix(k);
    int **T1 = allocateMatrix(k), **T2 = allocateMatrix(k);
    int **C11 = allocateMatrix(k), **C12 = allocateMatrix(k),
         **C21 = allocateMatrix(k), **C22 = allocateMatrix(k);

    // Divide matrices
    for(int i=0; i<k; i++) {
        for(int j=0; j<k; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j+k];
            A21[i][j] = A[i+k][j];
            A22[i][j] = A[i+k][j+k];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j+k];
            B21[i][j] = B[i+k][j];
            B22[i][j] = B[i+k][j+k];
        }
    }

    // M1 = (A11+A22)(B11+B22)
    addMatrix(A11, A22, T1, k);
    addMatrix(B11, B22, T2, k);
    strassenMultiply(T1, T2, M1, k);

    // M2 = (A21+A22)B11
    addMatrix(A21, A22, T1, k);
    strassenMultiply(T1, B11, M2, k);

    // M3 = A11(B12-B22)
    subMatrix(B12, B22, T1, k);
    strassenMultiply(A11, T1, M3, k);

    // M4 = A22(B21-B11)
    subMatrix(B21, B11, T1, k);
    strassenMultiply(A22, T1, M4, k);

    // M5 = (A11+A12)B22
    addMatrix(A11, A12, T1, k);
    strassenMultiply(T1, B22, M5, k);

    // M6 = (A21-A11)(B11+B12)
    subMatrix(A21, A11, T1, k);
    addMatrix(B11, B12, T2, k);
    strassenMultiply(T1, T2, M6, k);

    // M7 = (A12-A22)(B21+B22)
    subMatrix(A12, A22, T1, k);
    addMatrix(B21, B22, T2, k);
    strassenMultiply(T1, T2, M7, k);

    // C11 = M1+M4-M5+M7
    addMatrix(M1, M4, T1, k);
    subMatrix(T1, M5, T2, k);
    addMatrix(T2, M7, C11, k);

    // C12 = M3+M5
    addMatrix(M3, M5, C12, k);

    // C21 = M2+M4
    addMatrix(M2, M4, C21, k);

    // C22 = M1-M2+M3+M6
    subMatrix(M1, M2, T1, k);
    addMatrix(T1, M3, T2, k);
    addMatrix(T2, M6, C22, k);

    // Combine results
    for(int i=0; i<k; i++) {
        for(int j=0; j<k; j++) {
            C[i][j] = C11[i][j];
            C[i][j+k] = C12[i][j];
            C[i+k][j] = C21[i][j];
            C[i+k][j+k] = C22[i][j];
        }
    }
}

// ---------------- Helper for padding ----------------
int nextPowerOf2(int n) {
    int p = 1;
    while(p < n) p <<= 1;
    return p;
}

int main() {
    int n;
    printf("Enter order of square matrix: ");
    scanf("%d", &n);

    int N = nextPowerOf2(n);

    int **A = allocateMatrix(N);
    int **B = allocateMatrix(N);
    int **C = allocateMatrix(N);

    printf("Enter matrix A (%dx%d):\n", n, n);
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            scanf("%d", &A[i][j]);

    printf("Enter matrix B (%dx%d):\n", n, n);
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            scanf("%d", &B[i][j]);

    

    // Strassen
    strassenMultiply(A,B,C,N);
    printf("\nStrassen Multiplication Result:\n");
    printMatrix(C,n);

    return 0;
}
