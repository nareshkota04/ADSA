#include <stdio.h>
#include <string.h>
#include <math.h>

#define PRIME 101  // prime number for Rabin-Karp hashing

// ---------------------------
// KMP ALGORITHM IMPLEMENTATION
// ---------------------------

// Function to compute LPS (Longest Prefix Suffix) array for KMP
void computeLPSArray(char* pattern, int M, int* lps) {
    int len = 0;  // length of previous longest prefix suffix
    lps[0] = 0;   // LPS of first character is always 0
    int i = 1;

    while (i < M) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0)
                len = lps[len - 1];
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// Function to perform KMP Search
void KMPSearch(char* pattern, char* text) {
    int M = strlen(pattern);
    int N = strlen(text);
    int lps[M];

    computeLPSArray(pattern, M, lps);

    int i = 0; // index for text
    int j = 0; // index for pattern
    int found = 0;

    printf("\n--- Knuth-Morris-Pratt (KMP) Algorithm ---\n");

    while (i < N) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == M) {
            printf("Pattern found at index %d\n", i - j);
            found = 1;
            j = lps[j - 1];
        } else if (i < N && pattern[j] != text[i]) {
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }

    if (!found)
        printf("Pattern not found in text.\n");
}

// --------------------------------
// RABIN-KARP ALGORITHM IMPLEMENTATION
// --------------------------------

// Rabin-Karp Search Function
void RabinKarpSearch(char* pattern, char* text, int q) {
    int M = strlen(pattern);
    int N = strlen(text);
    int d = 256;  // number of characters in input alphabet
    int p = 0;    // hash value for pattern
    int t = 0;    // hash value for text window
    int h = 1;
    int found = 0;

    // The value of h would be pow(d, M-1) % q
    for (int i = 0; i < M - 1; i++)
        h = (h * d) % q;

    // Calculate hash value for pattern and first window of text
    for (int i = 0; i < M; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    printf("\n--- Rabin-Karp Fingerprinting Algorithm ---\n");

    // Slide the pattern over text
    for (int i = 0; i <= N - M; i++) {
        // If hash values match, check characters one by one
        if (p == t) {
            int j;
            for (j = 0; j < M; j++) {
                if (text[i + j] != pattern[j])
                    break;
            }

            if (j == M) {
                printf("Pattern found at index %d\n", i);
                found = 1;
            }
        }

        // Calculate hash value for next window
        if (i < N - M) {
            t = (d * (t - text[i] * h) + text[i + M]) % q;
            if (t < 0)
                t += q;
        }
    }

    if (!found)
        printf("Pattern not found in text.\n");
}

// -------------------------
// MAIN FUNCTION
// -------------------------
int main() {
    char text[100], pattern[50];

    printf("Enter the text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0'; // remove newline

    printf("Enter the pattern: ");
    fgets(pattern, sizeof(pattern), stdin);
    pattern[strcspn(pattern, "\n")] = '\0'; // remove newline

    KMPSearch(pattern, text);
    RabinKarpSearch(pattern, text, PRIME);

    return 0;
}