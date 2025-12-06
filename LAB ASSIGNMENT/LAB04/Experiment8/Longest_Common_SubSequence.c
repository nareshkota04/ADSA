#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
#define MAX_SEQ 1000  // Max number of subsequences

int max(int a, int b) {
    return (a > b) ? a : b;
}

/* ------------------------------
   Function to find length of LCS
   ------------------------------ */
int LCSLength(char X[], char Y[], int m, int n, int dp[MAX][MAX]) {
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else if (X[i - 1] == Y[j - 1])
                dp[i][j] = 1 + dp[i - 1][j - 1];
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    return dp[m][n];
}

/* ----------------------------------------
   Function to print one Longest Subsequence
   ---------------------------------------- */
void printLCS(char X[], char Y[], int m, int n, int dp[MAX][MAX]) {
    char lcs[MAX];
    int index = dp[m][n];
    lcs[index] = '\0';

    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs[index - 1] = X[i - 1];
            i--;
            j--;
            index--;
        } else if (dp[i - 1][j] > dp[i][j - 1])
            i--;
        else
            j--;
    }
    printf("\nLongest Common Subsequence: %s\n", lcs);
}

/* ------------------------------------------
   Store unique subsequences (avoiding repeats)
   ------------------------------------------ */
int isDuplicate(char subs[][MAX], int count, char *str) {
    for (int i = 0; i < count; i++)
        if (strcmp(subs[i], str) == 0)
            return 1;
    return 0;
}

/* ---------------------------------------------------------
   Recursive function to collect all common subsequences
   --------------------------------------------------------- */
void allCommonSubsequences(char X[], char Y[], int m, int n, char result[], int index,
                           char subs[][MAX], int *count) {
    if (m == 0 || n == 0) {
        if (index > 0) {
            result[index] = '\0';
            if (!isDuplicate(subs, *count, result)) {
                strcpy(subs[*count], result);
                (*count)++;
            }
        }
        return;
    }

    if (X[m - 1] == Y[n - 1]) {
        result[index] = X[m - 1];
        allCommonSubsequences(X, Y, m - 1, n - 1, result, index + 1, subs, count);
    }

    allCommonSubsequences(X, Y, m - 1, n, result, index, subs, count);
    allCommonSubsequences(X, Y, m, n - 1, result, index, subs, count);
}

/* --------------------------
   Main Function
   -------------------------- */
int main() {
    char X[MAX], Y[MAX];
    int dp[MAX][MAX];

    printf("Enter first string: ");
    scanf("%s", X);
    printf("Enter second string: ");
    scanf("%s", Y);

    int m = strlen(X);
    int n = strlen(Y);

    int length = LCSLength(X, Y, m, n, dp);
    printf("\nLength of LCS: %d", length);

    printLCS(X, Y, m, n, dp);

    char subs[MAX_SEQ][MAX];
    int count = 0;
    char temp[MAX];
    allCommonSubsequences(X, Y, m, n, temp, 0, subs, &count);

    printf("\nAll Unique Common Subsequences:\n");
    for (int i = 0; i < count; i++) {
        int len = strlen(subs[i]);
        // Reverse to restore proper order
        for (int j = 0; j < len / 2; j++) {
            char c = subs[i][j];
            subs[i][j] = subs[i][len - j - 1];
            subs[i][len - j - 1] = c;
        }
        printf("%s\n", subs[i]);
    }

    return 0;
}