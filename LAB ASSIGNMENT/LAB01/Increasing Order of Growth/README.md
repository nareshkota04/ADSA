## Comparison of Algorithm Complexities

### Problem Statement

This program aims to visualize and compare the growth rates (time complexities) of various mathematical functions, which commonly represent the efficiency of algorithms. By plotting these functions using `gnuplot`, it demonstrates how different algorithms scale with increasing input size.

### Related Algorithm: Big O Notation and Growth Rates

The core concept illustrated here is Big O notation, which describes the upper bound of an algorithm's runtime complexity or space requirements as the input size (denoted as 'n') grows. It provides a standardized way to express how an algorithm's performance scales.

The program includes functions representing common growth rates:

*   **O(log n)** (Logarithmic): e.g., `log2(n)`
*   **O(sqrt(n))** (Square Root): e.g., `12 * sqrt(n)`, `50 * sqrt(n)`
*   **O(n)** (Linear): e.g., `((double)pow(2,32)) * a`
*   **O(n log n)** (Linearithmic): e.g., `n * log2(n)`
*   **O(n^2)** (Quadratic): e.g., `100 * n^2 + 6*n`, `n^2 - 324`
*   **O(n^3)** (Cubic): e.g., `2 * n^3`
*   **O(c^n)** (Exponential): e.g., `3^n`
*   **Other/Specific**: `1/n`, `n^log2(n)`, `n^0.51`

By plotting these, one can visually observe that:
`O(1) < O(log n) < O(sqrt(n)) < O(n) < O(n log n) < O(n^2) < O(n^3) < O(c^n) < O(n!)`
(where O(1) is constant time, not directly represented by a function here but conceptually the fastest).

### Code Details

The `q1.c` file defines twelve `double` functions (`fun1` through `fun12`), each representing a different mathematical expression related to algorithm complexity.

The `main` function:
1.  Initializes `gnuplot` for plotting. It sets the title, axis labels, grid, and uses a logarithmic scale for the y-axis (`set logscale y`) to better compare functions with vastly different growth rates.
2.  Prepares `gnuplot` to plot 12 datasets, assigning a title to each representing its complexity class.
3.  Prompts the user to enter `maxn` (the number of data points to evaluate) and then `maxn` integer values, which are used as the 'n' inputs for the functions.
4.  For each input `n`, it calculates and prints the value of each of the twelve functions.
5.  It then feeds the calculated `(n, f(n))` pairs for each function to `gnuplot`, generating a visual comparison graph.

This setup allows for a dynamic visualization of how various complexity functions behave as the input size changes.

### Sample Input/Output

**Input:**

```
Enter the value of n
5
Enter 5 numbers:
10
50
100
200
500
```

**Output (Console Output - `gnuplot` window will also appear with the plot):**

```
n=10 33.219281
n=10 37.947332
n=10 0.100000
n=10 1000.000000
n=10 10060.000000
n=10 3.235937
n=10 -224.000000
n=10 158.113883
n=10 2000.000000
n=10 59049.000000
n=10 42949672960.000000
n=10 3.321928


n=50 282.192810
n=50 84.852814
n=50 0.020000
n=50 2500000.000000
n=50 250300.000000
n=50 5.485141
n=50 2176.000000
n=50 353.553391
n=50 250000.000000
n=50 717897987691852567248.000000
n=50 214748364800.000000
n=50 5.643856

... (similar output for other 'n' values and functions)
```
*(The gnuplot window will display a graph with 12 lines, each representing one of the functions, showing their growth relative to each other.)*
