// ======= Kruskal's Algorithm - Minimum Spanning Tree =======
// Graph input: number of vertices, number of edges, then edges (u v w)

#include <stdio.h>

struct Edge {
    int u, v, w;
};

int parent[100];

int find(int x) {
    if (parent[x] == x) return x;
    return parent[x] = find(parent[x]); // Path compression
}

void union_set(int a, int b) {
    parent[find(a)] = find(b);
}

void kruskal(struct Edge edges[], int n, int e) {
    int i, j, totalCost = 0;

    // Initialize parent array
    for (i = 0; i < n; i++)
        parent[i] = i;

    // Sort edges (Bubble sort by weight)
    for (i = 0; i < e - 1; i++)
        for (j = 0; j < e - i - 1; j++)
            if (edges[j].w > edges[j + 1].w) {
                struct Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }

    printf("\nKruskal's MST:\n");

    for (i = 0; i < e; i++) {
        if (find(edges[i].u) != find(edges[i].v)) {
            printf("Edge: %d - %d  Cost: %d\n", edges[i].u, edges[i].v, edges[i].w);
            totalCost += edges[i].w;
            union_set(edges[i].u, edges[i].v);
        }
    }

    printf("Total MST Cost = %d\n", totalCost);
}

int main() {
    int n, e;
    struct Edge edges[100];

    printf("Enter number of vertices: ");
    scanf("%d", &n);

    printf("Enter number of edges: ");
    scanf("%d", &e);

    printf("\nEnter edges (u v w):\n");
    for (int i = 0; i < e; i++)
        scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].w);

    kruskal(edges, n, e);
    return 0;
}
