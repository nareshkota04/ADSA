#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

typedef struct FibNode {
    int key;
    int degree;
    struct FibNode *parent;
    struct FibNode *child;
    struct FibNode *left;
    struct FibNode *right;
    int mark;
} FibNode;

typedef struct FibHeap {
    FibNode *min;
    int n;
} FibHeap;

/* ---------------- Utility Functions ---------------- */

FibNode* createNode(int key) {
    FibNode *node = (FibNode*)malloc(sizeof(FibNode));
    node->key = key;
    node->degree = 0;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    node->mark = 0;
    return node;
}

FibHeap* makeHeap() {
    FibHeap *H = (FibHeap*)malloc(sizeof(FibHeap));
    H->min = NULL;
    H->n = 0;
    return H;
}

/* ---------------- Insert ---------------- */

void fibHeapInsert(FibHeap *H, FibNode *x) {
    if (H->min == NULL) {
        H->min = x;
    } else {
        x->left = H->min;
        x->right = H->min->right;
        H->min->right->left = x;
        H->min->right = x;

        if (x->key < H->min->key)
            H->min = x;
    }
    H->n++;
}

/* ---------------- Link ---------------- */

void fibHeapLink(FibHeap *H, FibNode *y, FibNode *x) {
    y->left->right = y->right;
    y->right->left = y->left;

    y->parent = x;

    if (x->child == NULL) {
        x->child = y;
        y->left = y;
        y->right = y;
    } else {
        y->right = x->child;
        y->left = x->child->left;
        x->child->left->right = y;
        x->child->left = y;
    }

    x->degree++;
    y->mark = 0;
}

/* ---------------- Consolidate ---------------- */

void fibHeapConsolidate(FibHeap *H) {
    int D = (int)(log(H->n) / log(2)) + 1;
    FibNode **A = calloc(D, sizeof(FibNode*));

    FibNode *w = H->min;
    FibNode *start = w;

    do {
        FibNode *x = w;
        int d = x->degree;

        while (A[d] != NULL) {
            FibNode *y = A[d];

            if (y->key < x->key) {
                FibNode *temp = x;
                x = y;
                y = temp;
            }

            fibHeapLink(H, y, x);
            A[d] = NULL;
            d++;
        }

        A[d] = x;
        w = w->right;

    } while (w != start);

    H->min = NULL;

    for (int i = 0; i < D; i++) {
        if (A[i] != NULL) {
            if (H->min == NULL)
                H->min = A[i];
            else {
                A[i]->left = H->min;
                A[i]->right = H->min->right;
                H->min->right->left = A[i];
                H->min->right = A[i];

                if (A[i]->key < H->min->key)
                    H->min = A[i];
            }
        }
    }

    free(A);
}

/* ---------------- Extract-Min ---------------- */

FibNode* fibHeapExtractMin(FibHeap *H) {
    FibNode *z = H->min;

    if (z != NULL) {
        if (z->child != NULL) {
            FibNode *x = z->child;
            do {
                FibNode *next = x->right;

                x->left = H->min;
                x->right = H->min->right;
                H->min->right->left = x;
                H->min->right = x;
                x->parent = NULL;

                x = next;
            } while (x != z->child);
        }

        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right) {
            H->min = NULL;
        } else {
            H->min = z->right;
            fibHeapConsolidate(H);
        }

        H->n--;
    }

    return z;
}

/* ---------------- Cut & Cascading Cut ---------------- */

void cut(FibHeap *H, FibNode *x, FibNode *y) {
    if (x->right == x)
        y->child = NULL;
    else {
        x->right->left = x->left;
        x->left->right = x->right;

        if (y->child == x)
            y->child = x->right;
    }

    y->degree--;

    x->left = H->min;
    x->right = H->min->right;
    H->min->right->left = x;
    H->min->right = x;

    x->parent = NULL;
    x->mark = 0;
}

void cascadingCut(FibHeap *H, FibNode *y) {
    FibNode *z = y->parent;

    if (z != NULL) {
        if (y->mark == 0)
            y->mark = 1;
        else {
            cut(H, y, z);
            cascadingCut(H, z);
        }
    }
}

/* ---------------- Decrease-Key ---------------- */

void fibHeapDecreaseKey(FibHeap *H, FibNode *x, int k) {
    if (k > x->key) {
        printf("Error: new key greater\n");
        return;
    }

    x->key = k;
    FibNode *y = x->parent;

    if (y != NULL && x->key < y->key) {
        cut(H, x, y);
        cascadingCut(H, y);
    }

    if (x->key < H->min->key)
        H->min = x;
}

/* ---------------- Delete ---------------- */

void fibHeapDelete(FibHeap *H, FibNode *x) {
    fibHeapDecreaseKey(H, x, INT_MIN);
    fibHeapExtractMin(H);
}

/* ---------------- Menu + User Input ---------------- */

int main() {
    FibHeap *H = makeHeap();
    int choice, key, newKey;

    FibNode *nodes[1000];  // store pointers for decrease/delete
    int nodeCount = 0;

    while (1) {
        printf("\n==== Fibonacci Heap Menu ====\n");
        printf("1. Insert\n");
        printf("2. Extract Min\n");
        printf("3. Find Min\n");
        printf("4. Decrease Key\n");
        printf("5. Delete Key\n");
        printf("6. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("Enter key to insert: ");
            scanf("%d", &key);
            nodes[nodeCount] = createNode(key);
            fibHeapInsert(H, nodes[nodeCount]);
            printf("Inserted %d\n", key);
            nodeCount++;
            break;

        case 2: {
            FibNode *m = fibHeapExtractMin(H);
            if (m)
                printf("Extracted Min = %d\n", m->key);
            else
                printf("Heap is empty!\n");
            break;
        }

        case 3:
            if (H->min)
                printf("Minimum = %d\n", H->min->key);
            else
                printf("Heap is empty!\n");
            break;

        case 4:
            printf("Enter old key: ");
            scanf("%d", &key);

            printf("Enter new (smaller) key: ");
            scanf("%d", &newKey);

            for (int i = 0; i < nodeCount; i++) {
                if (nodes[i]->key == key) {
                    fibHeapDecreaseKey(H, nodes[i], newKey);
                    printf("Key decreased\n");
                    break;
                }
            }
            break;

        case 5:
            printf("Enter key to delete: ");
            scanf("%d", &key);

            for (int i = 0; i < nodeCount; i++) {
                if (nodes[i]->key == key) {
                    fibHeapDelete(H, nodes[i]);
                    printf("Key deleted\n");
                    break;
                }
            }
            break;

        case 6:
            printf("Exiting...\n");
            return 0;

        default:
            printf("Invalid choice\n");
        }
    }

    return 0;
}
