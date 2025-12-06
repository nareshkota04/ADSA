// ======= Prim's Algorithm - Minimum Spanning Tree =======
// Graph input: number of vertices, number of edges, then edges (u v w)

#include <stdio.h>
#define INF 999999

void prim(int graph[100][100], int n) {
    int selected[100] = {0};
    int i, j, edgeCount = 0, totalCost = 0;

    selected[0] = 1; // Start from vertex 0

    printf("\nPrim's MST:\n");

    while (edgeCount < n - 1) {
        int min = INF;
        int x = 0, y = 0;

        for (i = 0; i < n; i++) {
            if (selected[i]) {
                for (j = 0; j < n; j++) {
                    if (!selected[j] && graph[i][j] < min) {
                        min = graph[i][j];
                        x = i;
                        y = j;
                    }
                }
            }
        }

        printf("Edge: %d - %d  Cost: %d\n", x, y, graph[x][y]);
        totalCost += graph[x][y];
        selected[y] = 1;
        edgeCount++;
    }

    printf("Total MST Cost = %d\n", totalCost);
}

int main() {
    int n, e;
    int graph[100][100];

    printf("Enter number of vertices: ");
    scanf("%d", &n);

    printf("Enter number of edges: ");
    scanf("%d", &e);

    // Initialize graph with INF
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            graph[i][j] = INF;

    printf("\nEnter edges (u v w):\n");
    for (int i = 0; i < e; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        graph[u][v] = w;
        graph[v][u] = w;
    }

    prim(graph, n);
    return 0;
}
