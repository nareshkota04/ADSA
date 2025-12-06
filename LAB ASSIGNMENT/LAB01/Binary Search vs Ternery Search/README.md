## Binary Search vs. Ternary Search Comparison

### Problem Statement

This program compares the performance of binary search and ternary search algorithms. Both algorithms are used to find a specific value (a "key") within a sorted array. The primary measure of performance in this program is the number of comparisons made by each algorithm to find the key.

### Algorithms

#### Binary Search

Binary search is an efficient algorithm that works by repeatedly dividing the search interval in half.

1.  **Compare**: The key is compared with the middle element of the array.
2.  **Eliminate**:
    *   If the key matches, the search is successful.
    *   If the key is smaller than the middle element, the search continues on the left half of the array.
    *   If the key is larger, the search continues on the right half.
3.  **Repeat**: This process is repeated until the element is found or the interval is empty.

This approach has a time complexity of O(log n), making it very fast for large datasets.

#### Ternary Search

Ternary search is another divide-and-conquer algorithm that works by dividing the array into three parts.

1.  **Divide**: The array is divided into three parts by two middle elements, `mid1` and `mid2`.
2.  **Compare**: The key is compared with the elements at `mid1` and `mid2`.
3.  **Eliminate**:
    *   If the key is found at `mid1` or `mid2`, the search is successful.
    *   If the key is less than the element at `mid1`, the search continues in the first part.
    *   If the key is greater than the element at `mid2`, the search continues in the third part.
    *   Otherwise, the search continues in the middle part (between `mid1` and `mid2`).
4.  **Repeat**: This process continues until the element is found or the interval is empty.

Ternary search has a time complexity of O(log3 n). While this is asymptotically similar to binary search, it generally performs more comparisons in practice, making it slightly slower for most array-based searching tasks.

### Code Explanation

The C code implements both a `binarySearch` and a `ternarySearch` function. Both functions take a sorted array, the start and end indices of the search range, the key to find, and a pointer to a counter variable. This counter is incremented each time a comparison is made, allowing for a direct performance comparison between the two algorithms.

The `main` function:
1.  Prompts the user to enter the size of the array and its elements.
2.  Asks for the key to be searched.
3.  Calls both `binarySearch` and `ternarySearch` to find the key.
4.  Prints the index where the key was found (or -1 if not found) and the total number of comparisons made by each algorithm.

This allows for a clear, side-by-side demonstration of their efficiency on the same dataset.

### Sample Input/Output

**Input:**

```
Enter number of elements: 10
Enter sorted elements:
11
22
33
44
55
66
77
88
99
111
Enter element to search: 88
```

**Output:**

```
Binary Search: Found at index 7
Binary Search Comparisons: 4
Ternary Search: Found at index 7
Ternary Search Comparisons: 4
```
