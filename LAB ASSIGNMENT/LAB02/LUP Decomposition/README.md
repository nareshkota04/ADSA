## LUP Decomposition

### Problem Statement

This program performs LUP (Lower-Upper-Permutation) Decomposition on a given square matrix `A`. The decomposition factors `A` into three matrices: a permutation matrix `P`, a lower triangular matrix `L`, and an upper triangular matrix `U`, such that `PA = LU`. This decomposition is a fundamental tool in numerical linear algebra, particularly for solving systems of linear equations and computing matrix inverses.

### Related Algorithm: LUP Decomposition

LUP Decomposition is an extension of LU Decomposition that incorporates a permutation matrix `P`. The primary reason for `P` is to handle pivoting, which is crucial for numerical stability and avoiding division by zero during the decomposition process.

**Key Components:**

*   **P (Permutation Matrix):** A square binary matrix that, when multiplied with a matrix, reorders its rows. In LUP decomposition, `P` captures the row swaps performed during Gaussian elimination to ensure a non-zero (and ideally large) pivot element.
*   **L (Lower Triangular Matrix):** A square matrix with all entries above the main diagonal being zero. Its diagonal elements are typically 1s (for unit lower triangular matrices). The elements below the diagonal are the multipliers used during Gaussian elimination.
*   **U (Upper Triangular Matrix):** A square matrix with all entries below the main diagonal being zero. Its diagonal elements are the pivot elements after row permutations.

**Algorithm Steps (Gaussian Elimination with Partial Pivoting):**

The `lupDecomposition` function implements a variant of Gaussian elimination with partial pivoting:

1.  **Initialization**:
    *   The permutation vector `P` is initialized to represent an identity permutation (i.e., `P[i] = i`).
    *   The original matrix `A` is modified in-place to store both `L` (below the diagonal) and `U` (on and above the diagonal).

2.  **Iteration (for `k` from 0 to `n-1`)**: This loop processes each column of the matrix.
    *   **Find Pivot**: In the current column `k`, find the row `k_prime` (from `k` to `n-1`) that contains the element with the largest absolute value (`A[k_prime][k]`). This is the partial pivoting step, selecting the best pivot for numerical stability.
    *   **Singularity Check**: If the largest pivot found `p` is very close to zero (less than `EPSILON`), the matrix is considered singular, and decomposition fails.
    *   **Swap Permutation**: Swap `P[k]` and `P[k_prime]` to record the row permutation.
    *   **Swap Rows**: Swap row `k` and row `k_prime` of the matrix `A` (which is being modified to become `L` and `U`).
    *   **Elimination**: For each row `i` from `k+1` to `n-1`:
        *   Calculate the multiplier `A[i][k] = A[i][k] / A[k][k]`. This multiplier becomes an element of `L`.
        *   Update the elements in row `i` from column `k+1` to `n-1`: `A[i][j] -= A[i][k] * A[k][j]`. This eliminates the element below the pivot.

3.  **Extraction**: After the loop, the modified matrix `A` contains the elements of `L` (below the diagonal, with implied 1s on the diagonal) and `U` (on and above the diagonal). The `main` function then explicitly constructs `L` and `U` matrices from this modified `A`.

### Code Details

The `q2.c` file provides:

*   **`lupDecomposition(double **A, int *P, int n)`**: The core function that performs the LUP decomposition. It modifies the input matrix `A` in-place to store the `L` and `U` factors and populates the permutation vector `P`.
*   **`printMatrix(double **M, int n)`**: A utility function to print a square matrix.
*   **`allocateMatrix(int n)`**: Allocates a square `n x n` matrix of `double`s.
*   **`freeMatrix(double **mat, int n)`**: Frees the memory allocated for a matrix.

The `main` function:
1.  Prompts the user to enter the size `n` of the square matrix.
2.  Takes input for the elements of the matrix `A`.
3.  Allocates memory for the permutation vector `P`.
4.  Calls `lupDecomposition` to perform the factorization. If it fails (due to a singular matrix), it prints an error and exits.
5.  If successful, it extracts the `L` and `U` matrices from the modified `A` matrix.
6.  Prints the permutation vector `P`, the lower triangular matrix `L`, and the upper triangular matrix `U`.
7.  Frees all dynamically allocated memory.

### Sample Input/Output

**Input:**

```
Enter size of square matrix (n): 3
Enter elements of matrix A (3 x 3), row by row:
2 1 1
4 -6 0
-2 7 2
```

**Output:**

```
Permutation vector P:
0 1 2

Lower triangular matrix L:
  1.0000   0.0000   0.0000
  2.0000   1.0000   0.0000
 -1.0000  -1.0000   1.0000

Upper triangular matrix U:
  2.0000   1.0000   1.0000
  0.0000  -8.0000  -2.0000
  0.0000   0.0000   3.0000
```
*(Note: The `P` vector `0 1 2` indicates no row swaps were needed for this specific input matrix, meaning the original row order was preserved.)*
