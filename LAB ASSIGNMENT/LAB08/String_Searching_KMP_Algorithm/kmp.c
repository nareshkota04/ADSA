#include <stdio.h>
#include <string.h>

void computeLPS(char *pat, int m, int *lps)
{
    int len = 0;
    lps[0] = 0; 
    int i = 1;

    while (i < m) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

void KMP(char *text, char *pat)
{
    int n = strlen(text);
    int m = strlen(pat);

    int lps[m];
    computeLPS(pat, m, lps);

    int i = 0, j = 0;

    while (i < n) {
        if (pat[j] == text[i]) {
            i++;
            j++;
        }

        if (j == m) {
            printf("Pattern found at index %d\n", i - j);
            j = lps[j - 1];
        }

        else if (i < n && pat[j] != text[i]) {
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }
}

int main()
{
    char text[100], pattern[100];

    printf("Enter Text: ");
    gets(text);

    printf("Enter Pattern: ");
    gets(pattern);

    KMP(text, pattern);

    return 0;
}
