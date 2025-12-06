/*
   Ellipsoid Algorithm for Linear Programming (Maximize c^T x)

   Solve:
        maximize c^T x
        subject to A x <= b

   Uses feasibility-based ellipsoid method + binary search on objective.

   Author: ChatGPT (GPT-5)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPS     1e-6
#define MAX_IT  2000


/* Dot product */
double dot(double *a, double *b, int n) {
    double r = 0.0;
    for (int i = 0; i < n; i++)
        r += a[i] * b[i];
    return r;
}

/* Matrix-vector multiplication: y = M * x */
void mat_vec(double **M, double *x, double *y, int n) {
    for (int i = 0; i < n; i++) {
        y[i] = dot(M[i], x, n);
    }
}

/* Allocate matrix */
double **alloc_mat(int n) {
    double **M = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++)
        M[i] = (double*)calloc(n, sizeof(double));
    return M;
}

void free_mat(double **M, int n) {
    for (int i = 0; i < n; i++)
        free(M[i]);
    free(M);
}


/* Check feasibility of point x for Ax <= b */
int check_feasible(double **A, double *b, int m, int n, double *x, int *violated_idx) {
    for (int i = 0; i < m; i++) {
        if (dot(A[i], x, n) > b[i] + EPS) {
            *violated_idx = i;
            return 0;  // not feasible
        }
    }
    return 1;  // feasible
}


/* Ellipsoid Feasibility Algorithm */
int ellipsoid_feasible(double **A, double *b, int m, int n,
                       double *x, double **E) {

    for (int iter = 0; iter < MAX_IT; iter++) {
        int idx;
        if (check_feasible(A, b, m, n, x, &idx)) {
            return 1;  // feasible point found
        }

        // violated: A[idx] ⋅ x <= b[idx]
        double *g = A[idx];  // separating hyperplane

        double *Eg = malloc(n * sizeof(double));
        mat_vec(E, g, Eg, n);

        double gtEg = dot(g, Eg, n);

        if (fabs(gtEg) < EPS)
            return 0;

        for (int i = 0; i < n; i++)
            x[i] = x[i] - (Eg[i] / (sqrt(gtEg) * (n + 1)));

        // update ellipsoid matrix E = (n^2 / (n^2 - 1)) * (E - (2/(n+1)) * Eg*Eg^T/gtEg)
        double factor = 2.0 / ((double)n + 1);

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                E[i][j] = ((double)n * n / (n * n - 1.0)) *
                          (E[i][j] - factor * (Eg[i] * Eg[j]) / gtEg);

        free(Eg);
    }

    return 0; // Failed to find feasible solution
}


/* Solve LP using binary search + ellipsoid */
void ellipsoid_lp(double **A, double *b, double *c, int m, int n) {

    double low = -1000, high = 1000;   // search range for c^T x
    double *x = malloc(n * sizeof(double));

    while (high - low > EPS) {

        double mid = (low + high) / 2.0;

        // Convert optimization to feasibility: A x <= b and c^T x >= mid
        double **A2 = alloc_mat(m + 1);
        double *b2 = malloc((m + 1) * sizeof(double));

        for (int i = 0; i < m; i++) {
            A2[i] = A[i];
            b2[i] = b[i];
        }

        // Add constraint: -c^T x <= -mid   → c^T x >= mid
        A2[m] = malloc(n * sizeof(double));
        for (int j = 0; j < n; j++)
            A2[m][j] = -c[j];
        b2[m] = -mid;

        double **E = alloc_mat(n);
        for (int i = 0; i < n; i++)
            E[i][i] = 1.0;     // identity matrix (start ellipsoid)

        for (int i = 0; i < n; i++)
            x[i] = 0.0;    // initial guess

        int feasible = ellipsoid_feasible(A2, b2, m+1, n, x, E);

        free_mat(E, n);
        free(b2);
        free(A2[m]);
        free(A2);

        if (feasible)
            low = mid;      // feasible → improve lower bound
        else
            high = mid;     // infeasible → improve upper bound
    }

    // Print result
    printf("\nOptimal solution (approx):\n");
    for (int i = 0; i < n; i++)
        printf("x[%d] = %.6lf\n", i, x[i]);

    printf("\nMax value: %.6lf\n", low);
    free(x);
}


/* -------- Main -------- */
int main() {

    int m, n;

    printf("Ellipsoid Algorithm for LP: maximize c^T x subject to A x <= b\n");

    printf("\nEnter number of constraints (m): ");
    scanf("%d", &m);

    printf("Enter number of variables (n): ");
    scanf("%d", &n);

    double **A = alloc_mat(m);
    double *b = malloc(m * sizeof(double));
    double *c = malloc(n * sizeof(double));

    printf("\nEnter matrix A (each constraint row):\n");
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            scanf("%lf", &A[i][j]);

    printf("\nEnter constraint limits b:\n");
    for (int i = 0; i < m; i++)
        scanf("%lf", &b[i]);

    printf("\nEnter objective vector c (maximize c^T x):\n");
    for (int j = 0; j < n; j++)
        scanf("%lf", &c[j]);

    ellipsoid_lp(A, b, c, m, n);

    free_mat(A, m);
    free(b);
    free(c);

    return 0;
}
