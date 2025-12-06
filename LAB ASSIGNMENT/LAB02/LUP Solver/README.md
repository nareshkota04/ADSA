## LUP Solver: Solving Linear Equations with LUP Decomposition

### Problem Statement

This program solves a system of linear equations of the form `Ax = b`, where `A` is a square matrix, `x` is the unknown vector, and `b` is the known right-hand side vector. It achieves this by first performing LUP (Lower-Upper-Permutation) Decomposition on matrix `A` and then using forward and backward substitution to find the solution vector `x`.

### Related Algorithm: Solving Linear Equations using LUP Decomposition

Solving linear systems `Ax = b` is a fundamental problem in many scientific and engineering domains. When dealing with potentially large or ill-conditioned matrices, directly computing the inverse of `A` can be computationally expensive and numerically unstable. LUP Decomposition offers a robust and efficient alternative.

The process involves two main phases:

1.  **LUP Decomposition of A**:
    *   The matrix `A` is decomposed into three matrices: a permutation matrix `P`, a lower triangular matrix `L`, and an upper triangular matrix `U`, such that `PA = LU`.
    *   The `P` matrix is crucial for numerical stability, as it performs row interchanges (pivoting) to ensure that the largest possible pivot element is used at each step, thereby minimizing rounding errors and avoiding division by zero.
    *   This decomposition is typically performed using a variant of Gaussian elimination with partial pivoting.

2.  **Solving the System `LUx = Pb`**:
    Once `PA = LU` is obtained, the original system `Ax = b` is transformed into `PAx = Pb`, or `LUx = Pb`. This can then be solved in two steps:

    *   **Step 1: Solve `Ly = Pb` for `y` (Forward Substitution)**:
        *   Let `y` be an intermediate vector such that `Ux = y`.
        *   The system `Ly = Pb` is a lower triangular system. This can be efficiently solved for `y` using forward substitution, starting from the first equation and solving for `y1`, then `y2`, and so on.

    *   **Step 2: Solve `Ux = y` for `x` (Backward Substitution)**:
        *   With `y` now known, the system `Ux = y` is an upper triangular system.
        *   This can be efficiently solved for `x` using backward substitution, starting from the last equation and solving for `xn`, then `xn-1`, and so on.

This two-step substitution process is much faster than directly inverting `A`, especially for large matrices, and maintains good numerical properties.

### Code Details

The `q3.c` file integrates the LUP Decomposition with the forward and backward substitution steps to solve linear systems. It includes the following key functions:

*   **`allocateMatrix(int n)` / `freeMatrix(double** mat, int n)`**: Utility functions for dynamic memory management of matrices.
*   **`lupDecomposition(double **A, int *P, int n)`**: This function is identical to the one in `q2.c`. It takes a matrix `A` and a permutation vector `P`, and performs the LUP decomposition in-place on `A`, also populating `P`. It handles pivot selection and row swapping for numerical stability.
*   **`forwardSubstitution(double **L, double *b, double *y, int *P, int n)`**: Solves the lower triangular system `Ly = Pb`.
    *   It takes the `L` matrix (extracted from the decomposed `A`), the original `b` vector, an empty `y` vector, and the permutation vector `P`.
    *   It first applies the permutation `P` to `b` to get `Pb`.
    *   Then, it performs forward substitution to compute `y`.
*   **`backwardSubstitution(double **U, double *y, double *x, int n)`**: Solves the upper triangular system `Ux = y`.
    *   It takes the `U` matrix (extracted from the decomposed `A`), the intermediate vector `y`, and an empty `x` vector.
    *   It performs backward substitution to compute `x`.
*   **`lupSolve(double **A, int *P, double *b, double *x, int n)`**: Orchestrates the solving process.
    *   It first extracts the `L` and `U` matrices from the `A` matrix (which has been modified by `lupDecomposition`).
    *   It calls `forwardSubstitution` and `backwardSubstitution` sequentially to find `x`.

The `main` function:
1.  Prompts the user to enter the size `n` of the square matrix.
2.  Takes input for the elements of matrix `A` and vector `b`.
3.  Calls `lupDecomposition` to decompose `A`. If the matrix is singular, it prints an error and exits.
4.  Calls `lupSolve` to compute the solution vector `x`.
5.  Prints the solution vector `x`.
6.  Frees all dynamically allocated memory.

### Sample Input/Output

**Input:**

```
Enter size of matrix (n): 3
Enter matrix A (3 x 3):
2 1 1
4 -6 0
-2 7 2
Enter vector b (3 elements):
5
10
3
```

**Output:**

```
Solution vector x:
x[0] = 2.000000
x[1] = 1.000000
x[2] = 0.000000
```
*(This output implies that for the given `A` and `b`, the solution is `x = [2, 1, 0]`. You can verify this by computing `Ax`.)*
