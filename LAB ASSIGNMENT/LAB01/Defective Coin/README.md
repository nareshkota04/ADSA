## Find Defective Coin

### Problem Statement

This program aims to identify a single defective coin among a set of coins using a simulated balance scale. A defective coin has a different weight (either lighter or heavier) than the standard coins. The goal is to find the index and weight of this odd coin.

### Related Algorithm: Defective Coin Problem (Balance Scale Puzzle)

The classic defective coin problem involves finding an odd-weighted coin (either heavier or lighter) among a group of `N` coins using a balance scale. The most efficient general solution often involves a divide-and-conquer strategy that divides the coins into three groups: A, B, and C (with A and B being of equal size).

1.  **Weigh Group A against Group B:**
    *   **If they balance**: The defective coin is in Group C, and all coins in A and B are standard. Further weighings can then pinpoint the defective coin in C and determine if it's lighter or heavier.
    *   **If A is lighter than B**: The defective coin is either in Group A (and is lighter) or in Group B (and is heavier). All coins in C are standard.
    *   **If A is heavier than B**: The defective coin is either in Group A (and is heavier) or in Group B (and is lighter). All coins in C are standard.

This process is applied recursively to the smaller group of suspicious coins, typically leading to a solution in `O(log3 N)` weighings.

The provided code implements a variation of this divide-and-conquer strategy. While the standard approach divides into three, this implementation often divides into two main groups for comparison and has specific handling for edge cases.

### Code Details

The `q3.c` file contains two main functions:

*   **`compareweigh(int arr[], int r1, int l1, int r2, int l2)`**: This function simulates a balance scale. It takes two ranges of indices (`r1` to `l1` and `r2` to `l2`) from the `arr` (representing coin weights). It calculates the sum of weights for each range.
    *   Returns `-1` if the sum of the first range is less than the second (simulating the left side being lighter).
    *   Returns `1` if the sum of the first range is greater than the second (simulating the left side being heavier).
    *   Returns `0` if the sums are equal (simulating the scale balancing).

*   **`FindDefectiveCoin(int arr[], int low, int high)`**: This is a recursive function that implements the search logic.
    *   **Base Cases**:
        *   If `low == high`, the single remaining coin at `low` is the defective one.
        *   If `high - low == 1` (two coins), it directly compares their weights to find the lighter one (assuming the defective coin is lighter if one exists).
    *   **Recursive Step**:
        *   It divides the current range of coins (`low` to `high`) into two roughly equal halves.
        *   It uses `compareweigh` to compare the sum of weights of these two halves.
        *   Based on the `compareweigh` result, it narrows down the search to the suspected half.
        *   The logic for handling even numbers of coins (`n % 2 == 0`) is more intricate, involving multiple `compareweigh` calls to isolate the defective coin when initial comparisons result in a balance or a lighter side. The final conditional check `if (arr[high] < arr[low]) return high;` suggests a primary focus on finding a *lighter* defective coin if ambiguities persist.

The `main` function:
1.  Prompts the user to enter the total number of coins (`n`).
2.  Asks the user to input the weights of each coin.
3.  Calls `FindDefectiveCoin` with the entire array.
4.  Prints the index and weight of the identified defective coin, or a message if no defective coin is found (which would imply all coins have equal weight, though the problem assumes one is defective).

### Sample Input/Output

**Input:**

```
Enter the size of array(No of Coins)
5
Enter the array elements(Weights of coins)
10
10
9
10
10
```

**Output:**

```
Index of Defective Coin = 2 and Weigh is 9
```
