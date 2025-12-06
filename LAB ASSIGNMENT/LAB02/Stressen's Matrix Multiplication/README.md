## Strassen's Matrix Multiplication

### Problem Statement

This program implements Strassen's algorithm for multiplying two square matrices. Strassen's algorithm provides a more efficient approach than the naive matrix multiplication method, especially for large matrices, by reducing the number of scalar multiplications required.

### Related Algorithm: Strassen's Matrix Multiplication

Strassen's algorithm is a divide-and-conquer algorithm for matrix multiplication that offers an asymptotic improvement over the conventional algorithm. The traditional method for multiplying two `n x n` matrices has a time complexity of `O(n^3)`. Strassen's algorithm, developed by Volker Strassen in 1969, reduces the number of recursive multiplications from 8 to 7, achieving a time complexity of `O(n^log2(7))`, which is approximately `O(n^2.81)`.

The algorithm works as follows for two `n x n` matrices A and B, where `n` is a power of 2 (matrices can be padded with zeros if not):

1.  **Divide**: Both matrices A and B are divided into four `n/2 x n/2` sub-matrices:
    ```
    A = | A11 A12 |   B = | B11 B12 |
        | A21 A22 |       | B21 B22 |
    ```
2.  **Conquer (Recursive Multiplication)**: Instead of performing 8 recursive multiplications as in the naive divide-and-conquer approach, Strassen's algorithm computes 7 products (`M1` to `M7`) of `n/2 x n/2` sub-matrices:
    *   `M1 = (A11 + A22) * (B11 + B22)`
    *   `M2 = (A21 + A22) * B11`
    *   `M3 = A11 * (B12 - B22)`
    *   `M4 = A22 * (B21 - B11)`
    *   `M5 = (A11 + A12) * B22`
    *   `M6 = (A21 - A11) * (B11 + B12)`
    *   `M7 = (A12 - A22) * (B21 + B22)`
3.  **Combine**: The four sub-matrices of the resultant matrix C are then calculated using these 7 products with additions and subtractions:
    *   `C11 = M1 + M4 - M5 + M7`
    *   `C12 = M3 + M5`
    *   `C21 = M2 + M4`
    *   `C22 = M1 - M2 + M3 + M6`

These steps are applied recursively until the sub-matrices are small enough (e.g., 1x1) to be multiplied directly.

### Code Details

The `q1.c` file implements Strassen's matrix multiplication algorithm with several helper functions:

*   **`allocateMatrix(int n)`**: Allocates memory for an `n x n` integer matrix and initializes it with zeros.
*   **`freeMatrix(int** mat, int n)`**: Frees the dynamically allocated memory for a matrix.
*   **`copyBlock(int** src, int** dest, int r, int c, int n)`**: Copies an `n x n` block from the `src` matrix starting at row `r` and column `c` to the `dest` matrix (as a `n x n` matrix itself).
*   **`pasteBlock(int** src, int** dest, int r, int c, int n)`**: Pastes an `n x n` block from the `src` matrix into the `dest` matrix starting at row `r` and column `c`.
*   **`addMatrix(int** A, int** B, int** C, int n)`**: Adds two `n x n` matrices A and B, storing the result in C.
*   **`subMatrix(int** A, int** B, int** C, int n)`**: Subtracts matrix B from A, storing the result in C.
*   **`strassen(int** A, int** B, int** C, int n)`**: The core recursive function implementing Strassen's algorithm.
    *   Base case: If `n == 1`, it performs a simple scalar multiplication.
    *   Recursive step: Divides matrices, performs 7 recursive calls for `M1` to `M7`, and then combines them to form the result.
    *   Manages dynamic memory allocation and deallocation for sub-matrices.
*   **`nextPowerOf2(int n)`**: A helper function to find the next power of 2 greater than or equal to `n`. This is used for padding matrices.

The `main` function:
1.  Prompts the user to enter the size `n` of the square matrices.
2.  Takes input for the elements of matrix A and matrix B.
3.  Calculates `m`, the smallest power of 2 greater than or equal to `n`, for padding.
4.  Allocates padded matrices (`A_pad`, `B_pad`, `C_pad`) and copies the input matrices into the padded ones.
5.  Calls the `strassen` function to multiply `A_pad` and `B_pad`, storing the result in `C_pad`.
6.  Prints the top-left `n x n` portion of the `C_pad` matrix as the final result.
7.  Frees all dynamically allocated memory.

### Sample Input/Output

**Input:**

```
Enter size of matrix (n x n): 2

Enter elements of matrix A (2 x 2):
A[0][0] = 1
A[0][1] = 2
A[1][0] = 3
A[1][1] = 4

Enter elements of matrix B (2 x 2):
B[0][0] = 5
B[0][1] = 6
B[1][0] = 7
B[1][1] = 8
```

**Output:**

```
Resultant matrix C (A x B):
   19    22
   43    50
```
