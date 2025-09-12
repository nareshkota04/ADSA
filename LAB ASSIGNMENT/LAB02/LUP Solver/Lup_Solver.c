#include <stdio.h>

//Swap two rows in a matrix 
void swap_rows(int n, double A[n][n], int r1, int r2) {
    for (int j = 0; j < n; ++j) {
        double tmp = A[r1][j];
        A[r1][j] = A[r2][j];
        A[r2][j] = tmp;
    }
}

// LUP decomposition with partial pivoting 
int lup_decompose(int n, double A[n][n], int P[n]) {
    for (int i = 0; i < n; ++i) P[i] = i;

    for (int k = 0; k < n; ++k) {
        double max = 0.0;
        int pivot = -1;

        // find pivot (inline abs) 
        for (int i = k; i < n; ++i) {
            double val = A[i][k];
            if (val < 0) val = -val;   // inline absolute value
            if (val > max) { max = val; pivot = i; }
        }

        if (A[pivot][k] == 0.0) return -1;  // singular

        // swap if needed 
        if (pivot != k) {
            swap_rows(n, A, k, pivot);
            int tmp = P[k]; P[k] = P[pivot]; P[pivot] = tmp;
        }

        // elimination 
        for (int i = k+1; i < n; ++i) {
            A[i][k] /= A[k][k];
            for (int j = k+1; j < n; ++j) {
                A[i][j] -= A[i][k] * A[k][j];
            }
        }
    }
    return 0;
}





//Solve system Ax = b 
void lup_solve(int n, double A[n][n], int P[n],double b[n], double x[n]) 
{
    double y[n];

    // forward substitution: L*y = Pb 
    for (int i = 0; i < n; ++i) {
        y[i] = b[P[i]];
        for (int j = 0; j < i; ++j) {
            y[i] -= A[i][j] * y[j];
        }
    }

    //back substitution: U*x = y 
    for (int i = n-1; i >= 0; --i) {
        double sum = y[i];
        for (int j = i+1; j < n; ++j) {
            sum -= A[i][j] * x[j];
        }
        x[i] = sum / A[i][i];
    }
}



int main(void) {
    int n;
    printf("Enter matrix size n: ");
    scanf("%d", &n);

    double A[n][n], b[n], x[n];
    int P[n];

    printf("Enter the %dx%d matrix A row by row:\n", n, n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            scanf("%lf", &A[i][j]);

    printf("Enter the vector b (%d elements):\n", n);
    for (int i = 0; i < n; ++i)
        scanf("%lf", &b[i]);

    if (lup_decompose(n, A, P) != 0) {
        printf("Matrix is singular!\n");
        return 1;
    }

    lup_solve(n, A, P, b, x);

    printf("Result X vector :");
    printf("[");
    for(int i=0;i<n;i++){
        printf("%d",x[i]);
        if(i+1<n)printf(", ");
    }
    printf("]");

    return 0;
}
