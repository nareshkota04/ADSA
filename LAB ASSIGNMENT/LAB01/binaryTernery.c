#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_POINTS 250 
#define MAX_N 5000000 

struct Point {
    int n;
    int comparisons;
};

// --- Recursive Search Functions (These are correct, the test case was flawed) ---

int binarySearch(int low, int high, int target, int *comparisons) {
    if (low <= high) {
        int mid = low + (high - low) / 2;
        int mid_val = mid;
        (*comparisons)++;
        if (mid_val == target) return mid;
        (*comparisons)++;
        if (mid_val > target) return binarySearch(low, mid - 1, target, comparisons);
        else return binarySearch(mid + 1, high, low, comparisons);
    }
    return -1;
}

int ternarySearch(int low, int high, int target, int *comparisons) {
    if (low <= high) {
        int mid1 = low + (high-low) / 3;
        int mid2 = high - (high-low) / 3;
        int mid1_val = mid1;
        int mid2_val = mid2;
        (*comparisons)++;
        if (mid1_val == target) return mid1;
        (*comparisons)++;
        if (mid2_val == target) return mid2;
        (*comparisons)++;
        if (target < mid1_val) return ternarySearch(low, mid1 - 1, target, comparisons);
        (*comparisons)++;
        if (target > mid2_val) return ternarySearch(mid2 + 1, high, target, comparisons);
        else return ternarySearch(mid1 + 1, mid2 - 1, target, comparisons);
    }
    return -1;
}

int main() {
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe == NULL) {
        fprintf(stderr, "Error: Could not open pipe to Gnuplot.\n");
        return 1;
    }

    printf("Generating data points for the correct worst-case scenario...\n");
    struct Point bs_data[NUM_POINTS];
    struct Point ts_data[NUM_POINTS];
    int step = MAX_N / NUM_POINTS;

    for (int i = 0; i < NUM_POINTS; i++) {
        int n = (i + 1) * step;

        int bs_comps = 0;
        int ts_comps = 0;

        int element_to_find = n; 

        binarySearch(0, n - 1, element_to_find, &bs_comps);
        ternarySearch(0, n - 1,element_to_find, &ts_comps);

        bs_data[i].n = n;
        bs_data[i].comparisons = bs_comps;
        ts_data[i].n = n;
        ts_data[i].comparisons = ts_comps;
    }
    printf("Data generation complete. Plotting...\n");

    fprintf(gnuplotPipe, "set title Binary vs. Ternary Search Comparison'\n");
    fprintf(gnuplotPipe, "set xlabel 'Array Size (n)'\n");
    fprintf(gnuplotPipe, "set ylabel 'Number of Key Comparisons'\n");
    fprintf(gnuplotPipe, "set grid\n");
    fprintf(gnuplotPipe, "set key top left\n");
    fprintf(gnuplotPipe, "plot '-' with lines title 'Binary Search', '-' with lines title 'Ternary Search'\n");

    for (int i = 0; i < NUM_POINTS; i++) {
        fprintf(gnuplotPipe, "%d %d\n", bs_data[i].n, bs_data[i].comparisons);
        printf("data points = %d, comparison= %d, Algorithm = Binary Search\n", bs_data[i].n, bs_data[i].comparisons);
    }
    fprintf(gnuplotPipe, "e\n");

    for (int i = 0; i < NUM_POINTS; i++) {
        fprintf(gnuplotPipe, "%d %d\n", ts_data[i].n, ts_data[i].comparisons);
        printf("data points = %d, comparison= %d, Algorithm = Ternary Search\n", ts_data[i].n, ts_data[i].comparisons);

    }
    fprintf(gnuplotPipe, "e\n");

    fflush(gnuplotPipe);
    pclose(gnuplotPipe);

    return 0;
}