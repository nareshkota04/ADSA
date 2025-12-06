#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX 100

int graph[MAX][MAX];   // adjacency matrix
int matchR[MAX];       // match result for right set
bool seen[MAX];

int nL, nR;            // sizes of left and right sets

// DFS to find augmenting path
bool bpm(int u) {
    for (int v = 0; v < nR; v++) {
        if (graph[u][v] && !seen[v]) {
            seen[v] = true;

            // If right vertex is not assigned or previous left
            // vertex for right vertex has alternate match 
            if (matchR[v] < 0 || bpm(matchR[v])) {
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}

int maxBPM() {
    // Initialize all right vertices as unmatched
    memset(matchR, -1, sizeof(matchR));
    int result = 0;

    // Try to match each vertex in left set
    for (int u = 0; u < nL; u++) {
        memset(seen, 0, sizeof(seen));

        if (bpm(u)) {
            result++;
        }
    }
    return result;
}

int main() {
    printf("Enter number of vertices in Left set: ");
    scanf("%d", &nL);
    printf("Enter number of vertices in Right set: ");
    scanf("%d", &nR);

    printf("\nEnter adjacency matrix (%d x %d):\n", nL, nR);
    for (int i = 0; i < nL; i++) {
        for (int j = 0; j < nR; j++) {
            scanf("%d", &graph[i][j]);
        }
    }

    int result = maxBPM();
    printf("\nMaximum Bipartite Matching = %d\n", result);

    printf("\nMatched pairs (Left -> Right):\n");
    for (int v = 0; v < nR; v++) {
        if (matchR[v] != -1) {
            printf("L%d -> R%d\n", matchR[v], v);
        }
    }

    return 0;
}
