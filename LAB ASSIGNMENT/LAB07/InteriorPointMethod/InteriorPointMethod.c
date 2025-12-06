#include <stdio.h>
#include <math.h>

#define MAX 10
#define MAX_ITER 50
#define TOL 1e-6

// Solve linear system H * dx = g using Gaussian elimination
void solve_linear(int n, double H[MAX][MAX], double g[MAX], double dx[MAX]) {
    double A[MAX][MAX+1];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            A[i][j] = H[i][j];
        A[i][n] = -g[i];
    }

    for(int i=0;i<n;i++){
        double pivot = A[i][i];
        for(int j=i;j<=n;j++)
            A[i][j] /= pivot;
        for(int k=i+1;k<n;k++){
            double factor = A[k][i];
            for(int j=i;j<=n;j++)
                A[k][j] -= factor * A[i][j];
        }
    }

    for(int i=n-1;i>=0;i--){
        dx[i] = A[i][n];
        for(int j=i+1;j<n;j++)
            dx[i] -= A[i][j] * dx[j];
    }
}

int main() {
    int n, m;
    double c[MAX], A[MAX][MAX], b[MAX];
    double x[MAX], dx[MAX];

    printf("Enter number of variables (n <= 10): ");
    scanf("%d", &n);

    printf("Enter number of equality constraints (m = 1 for this version): ");
    scanf("%d", &m);

    if(m != 1) {
        printf("ERROR: This version supports only m = 1.\n");
        return 0;
    }

    printf("Enter coefficients of objective function (c1 c2 ... cn):\n");
    for(int i=0;i<n;i++) scanf("%lf", &c[i]);

    printf("Enter constraint coefficients A (1 row: a11 a12 ... an):\n");
    for(int i=0;i<n;i++) scanf("%lf", &A[0][i]);

    printf("Enter constraint RHS value b:\n");
    scanf("%lf", &b[0]);

    printf("Enter initial positive starting point x0 (must satisfy Ax=b):\n");
    for(int i=0;i<n;i++) scanf("%lf", &x[i]);

    double t = 1.0, mu = 10.0;

    printf("\nRunning Interior Point Method...\n");

    for(int iter=1; iter<=MAX_ITER; iter++) {
        double g[MAX];
        double H[MAX][MAX];

        // gradient
        for(int i=0;i<n;i++)
            g[i] = c[i] - t * (1.0 / x[i]);

        // Hessian
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                H[i][j] = (i == j) ? t / (x[i] * x[i]) : 0.0;

        // solve Newton step
        solve_linear(n, H, g, dx);

        // step size positivity rule
        double alpha = 1.0;
        for(int i=0;i<n;i++)
            if(dx[i] < 0)
                alpha = fmin(alpha, -0.9 * x[i] / dx[i]);

        // update x
        for(int i=0;i<n;i++)
            x[i] += alpha * dx[i];

        // enforce equality constraint by projection (1 eqn only)
        double sum = 0;
        for(int i=0;i<n;i++) sum += A[0][i] * x[i];
        double res = sum - b[0];

        // distribute correction equally
        for(int i=0;i<n;i++)
            x[i] -= (res / n);

        // stop condition
        double norm = 0;
        for(int i=0;i<n;i++) norm += g[i] * g[i];
        if(sqrt(norm) < TOL)
            break;

        t *= mu;

        printf("Iter %2d: x = [", iter);
        for(int i=0;i<n;i++) printf("%.6f ", x[i]);
        printf("]\n");
    }

    printf("\n=== Optimal Solution (Approx) ===\n");
    for(int i=0;i<n;i++)
        printf("x%d = %.6f\n", i+1, x[i]);

    double obj = 0;
    for(int i=0;i<n;i++) obj += c[i] * x[i];
    printf("Optimal Objective = %.6f\n", obj);

    return 0;
}