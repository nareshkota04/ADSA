#include <stdio.h>

void heapify(int arr[], int n, int i) {
    int large = i;          // Initialize largest as root
    int left = 2 * i + 1;     // left child
    int right = 2 * i + 2;    // right child

    //left child is larger than root
    if (left < n && arr[left] > arr[large])
        large = left;

    //right child is larger than large so far
    if (right < n && arr[right] > arr[large])
        large = right;

    // If large is not root
    if (large != i) {
        int temp = arr[i];
        arr[i] = arr[large];
        arr[large] = temp;

        // Recursively heapify
        heapify(arr, n, large);
    }
}

//perform heap sort
void heapSort(int arr[], int n) {
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Extract elements one by one 
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // Call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}


int main() {
    int n;
    scanf("%d",&n);
    int arr[n];
    for(int i=0;i<n;i++){
        scanf("%d",&arr[i]);
    }
    printf("Before Sorting: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    heapSort(arr, n);

    printf("\n After Sorting: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    

    return 0;
}
