#include <stdio.h>


int weight(int coins[], int left1, int right1, int left2, int right2) {
    int sum1 = 0, sum2 = 0;
    for (int i = left1; i <= right1; i++) sum1 += coins[i];
    for (int j = left2; j <= right2; j++) sum2 += coins[j];
    
    if (sum1 < sum2) return -1; // left side lighter
    if (sum1 > sum2) return 1;  // right side lighter
    return 0;                   // equal
}

// Recursive function to find defective coin
int findDefective(int coins[], int start, int end) {
    int n = end - start + 1;

    // only one coin
    if (n == 1) {
        return start; 
    }

    int mid = n / 2;
    int leftStart = start;
    int leftEnd   = start + mid - 1;
    int rightStart= start + mid;
    int rightEnd  = start + 2*mid - 1;

    int result = weight(coins, leftStart, leftEnd, rightStart, rightEnd);

    if (result == -1) {
        //left side defective
        return findDefective(coins, leftStart, leftEnd);
    } else if (result == 1) {
        //right side defective
        return findDefective(coins, rightStart, rightEnd);
    } else {
        // both sides equal
        if (n % 2 == 1) {
            int oddCoinIndex = end;
            // Compare odd coin with any known good coin (say coins[l])
            if (coins[oddCoinIndex] < coins[start])
                return oddCoinIndex;
        }
        // No defective coin
        return -1;
    }
}

int main() {
    int n;
    printf("Enter number of coins: ");
    scanf("%d", &n);

    int coins[n];
    printf("Enter %d coins: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &coins[i]);
    }

    int defectiveIndex = findDefective(coins, 0, n - 1);

    if (defectiveIndex == -1)
        printf("No defective coin found. All are perfect.\n");
    else
        printf("Defective (lighter) coin found at index %d (weight = %d).\n",
               defectiveIndex, coins[defectiveIndex]);

    return 0;
}
