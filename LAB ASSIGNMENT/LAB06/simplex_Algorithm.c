#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#define EPS 1e-9

/* Print tableau (for debugging) */
void print_tableau(double **T, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%10.4f ", T[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* Allocate 2D array */
double **alloc_tableau(int rows, int cols) {
    double **T = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; ++i) T[i] = calloc(cols, sizeof(double));
    return T;
}

void free_tableau(double **T, int rows) {
    for (int i = 0; i < rows; ++i) free(T[i]);
    free(T);
}

/* Find entering variable: most negative coefficient in objective row (for maximization).
   Returns column index or -1 if optimal.
*/
int find_entering(double **T, int cols) {
    int enter = -1;
    double minc = -EPS;
    for (int j = 0; j < cols - 1; ++j) { // last column is RHS
        double val = T[0][j];
        if (val < minc) {
            minc = val;
            enter = j;
        }
    }
    return enter;
}

/* Find leaving variable using minimum ratio test. Returns row index or -1 if unbounded */
int find_leaving(double **T, int rows, int cols, int enter) {
    int leave = -1;
    double minRatio = 0.0;
    for (int i = 1; i < rows; ++i) {
        double coeff = T[i][enter];
        double rhs = T[i][cols - 1];
        if (coeff > EPS) {
            double ratio = rhs / coeff;
            if (leave == -1 || ratio < minRatio - EPS || (fabs(ratio - minRatio) < EPS && i < leave)) {
                minRatio = ratio;
                leave = i;
            }
        }
    }
    return leave;
}

/* Pivot on (r, c) */
void pivot(double **T, int rows, int cols, int r, int c) {
    double pivot = T[r][c];
    // normalize pivot row
    for (int j = 0; j < cols; ++j) T[r][j] /= pivot;
    // eliminate other rows
    for (int i = 0; i < rows; ++i) {
        if (i == r) continue;
        double factor = T[i][c];
        if (fabs(factor) < EPS) continue;
        for (int j = 0; j < cols; ++j) {
            T[i][j] -= factor * T[r][j];
        }
    }
}

/* Simplex main routine. T is tableau with rows = m+1, cols = n+m+1
   BasicVars array of size rows-1 maps each constraint row -> column index of its basic variable.
*/
int simplex(double **T, int rows, int cols, int *basicVars) {
    int steps = 0;
    while (1) {
        int enter = find_entering(T, cols);
        if (enter == -1) {
            // optimal
            return 0;
        }
        int leave = find_leaving(T, rows, cols, enter);
        if (leave == -1) {
            // unbounded
            return 1;
        }
        // Update basic variable for the leaving row
        basicVars[leave - 1] = enter;
        pivot(T, rows, cols, leave, enter);
        steps++;
        if (steps > 10000) { // safeguard (shouldn't happen for small problems)
            fprintf(stderr, "Simplex: too many iterations\n");
            return 2;
        }
    }
}

/* Main program */
int main() {
    int m, n;
    printf("Simplex method (max) — Ax <= b, x >= 0\n");
    printf("Enter number of constraints (m) and number of variables (n): ");
    if (scanf("%d %d", &m, &n) != 2) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }
    if (m <= 0 || n <= 0) {
        fprintf(stderr, "m and n must be positive.\n");
        return 1;
    }

    double *c = malloc(n * sizeof(double));
    printf("Enter %d objective coefficients (c1..cn) for MAXIMIZATION:\n", n);
    for (int j = 0; j < n; ++j) scanf("%lf", &c[j]);

    double **A = malloc(m * sizeof(double *));
    double *b = malloc(m * sizeof(double));
    for (int i = 0; i < m; ++i) {
        A[i] = malloc(n * sizeof(double));
    }

    printf("Enter each constraint (coefficients a1..an and RHS b) as: a1 a2 ... an b\n");
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) scanf("%lf", &A[i][j]);
        scanf("%lf", &b[i]);
    }

    // Check that b[i] >= 0, otherwise we would need to multiply constraint by -1 or use two-phase method.
    for (int i = 0; i < m; ++i) {
        if (b[i] < -EPS) {
            printf("Note: RHS b[%d] = %g is negative. This simple implementation expects b >= 0.\n", i+1, b[i]);
            printf("Exiting. Use two-phase simplex for general cases.\n");
            // free memory
            for (int i2 = 0; i2 < m; ++i2) free(A[i2]);
            free(A); free(b); free(c);
            return 1;
        }
    }

    // Tableau dimensions:
    // rows = m + 1 (0 = objective row, 1..m constraints)
    // cols = n + m + 1 (original vars + slack vars + RHS)
    int rows = m + 1;
    int cols = n + m + 1;
    double **T = alloc_tableau(rows, cols);

    // Build objective row: we store -c for tableau (since we'll try to make objective row non-negative)
    // Our convention: Row 0 is objective row: T[0][j] = -c_j
    for (int j = 0; j < n; ++j) T[0][j] = -c[j];
    // Slack variables have 0 coefficient in objective row.

    // Constraints: for row i (1..m), place A[i-1][j] and slack variable at column n + (i-1) = 1-based
    for (int i = 1; i <= m; ++i) {
        for (int j = 0; j < n; ++j) {
            T[i][j] = A[i-1][j];
        }
        // slack variable coefficient = 1
        T[i][n + (i - 1)] = 1.0;
        // RHS
        T[i][cols - 1] = b[i - 1];
    }

    // Basic variables initially are the slack variables
    int *basicVars = malloc(m * sizeof(int));
    for (int i = 0; i < m; ++i) basicVars[i] = n + i; // column index of slack var

    // Note: Objective value is stored at T[0][cols-1] after pivoting (negated form)
    // Run simplex
    int status = simplex(T, rows, cols, basicVars);
    if (status == 1) {
        printf("The problem is unbounded.\n");
    } else if (status == 2) {
        printf("Iteration limit reached or numerical issue.\n");
    } else {
        // optimal
        // Read solution: basic vars give values in rows 1..m:
        double *x = calloc(n + m, sizeof(double)); // includes slack; we'll report x1..xn
        for (int i = 1; i <= m; ++i) {
            int col = basicVars[i - 1];
            if (col >= 0 && col < n + m) {
                x[col] = T[i][cols - 1];
            }
        }
        double optimal = T[0][cols - 1]; // because we maintained objective row as value in RHS
        // For maximization, we stored objective row such that T[0][cols-1] is the maximum value.
        printf("\nOptimal solution found.\n");
        printf("Objective value (max) = %.10g\n", optimal);
        for (int j = 0; j < n; ++j) {
            printf("x[%d] = %.10g\n", j+1, x[j]);
        }
        free(x);
    }

    // cleanup
    for (int i = 0; i < m; ++i) free(A[i]);
    free(A); free(b); free(c); free(basicVars);
    free_tableau(T, rows);

    return 0;
}
