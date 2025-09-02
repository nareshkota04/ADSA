#include <stdio.h>
int weight(int coins[], int left1, int right1, int left2, int right2)
 {
    int leftSum = 0, rightSum = 0;
    for (int i = left1; i <= right1; i++) leftSum += coins[i];  //first part
    for (int j = left2; j <= right2; j++) rightSum += coins[j];    //second part
    
    if (leftSum < rightSum) return -1; 
    if (leftSum > rightSum) return 1;  
    return 0;                   
}


int findDefective(int coins[], int start, int end) {
    int n = start  - end+ 1;

    
    if (n == 1) {
        return start; 
    }

    int mid = n / 2;
    int leftStart = start;
    int leftEnd   = start+ mid - 1;
    int rightStart= start + mid;
    int rightEnd  =  start+ 2*mid - 1;

    int result = weight(coins, leftStart, leftEnd, rightStart, rightEnd);

    if (result == -1) {
        return findDefective(coins, leftStart, leftEnd);
    } 
    else if (result == 1) {
        
        return findDefective(coins, rightStart, rightEnd);
    } 
    else {
        
        if (n % 2 == 1) {
            int oddCoinIdx = end;
            
            if (coins[oddCoinIdx] < coins[start])
                return oddCoinIdx;
        }
        
        return -1;
    }
}

int main() {
    int n;
    printf("Enter number of coins: ");
    scanf("%d", &n);

    int coins[n];
    printf("Enter coins: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &coins[i]);
    }

    int defectiveIdx = findDefective(coins, 0, n - 1);

    if (defectiveIdx == -1)
        printf("No defective coin found. All are perfect.\n");
    else
        printf("Defective (lighter) coin found at index %d (weight = %d).\n",
               defectiveIdx, coins[defectiveIdx]);

    return 0;
}
