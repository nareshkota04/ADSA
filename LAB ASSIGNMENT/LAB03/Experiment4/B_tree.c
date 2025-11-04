#include <stdio.h>
#include <stdlib.h>

#define MIN_DEGREE 3  // Minimum degree (t). Change as needed.

struct BTreeNode {
    int *keys;
    int t;
    struct BTreeNode **C;
    int n;
    int leaf;
};

// Function prototypes
struct BTreeNode *createNode(int t, int leaf);
void traverse(struct BTreeNode *root);
struct BTreeNode *search(struct BTreeNode *root, int k);
void insert(struct BTreeNode **root, int k);
void insertNonFull(struct BTreeNode *x, int k);
void splitChild(struct BTreeNode *x, int i, struct BTreeNode *y);
void deleteKey(struct BTreeNode *root, int k);
int findKey(struct BTreeNode *root, int k);
void removeFromLeaf(struct BTreeNode *root, int idx);
void removeFromNonLeaf(struct BTreeNode *root, int idx);
int getPred(struct BTreeNode *root, int idx);
int getSucc(struct BTreeNode *root, int idx);
void fill(struct BTreeNode *root, int idx);
void borrowFromPrev(struct BTreeNode *root, int idx);
void borrowFromNext(struct BTreeNode *root, int idx);
void merge(struct BTreeNode *root, int idx);

// Create a new B-Tree node
struct BTreeNode *createNode(int t, int leaf) {
    struct BTreeNode *node = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    node->t = t;
    node->leaf = leaf;
    node->keys = (int *)malloc(sizeof(int) * (2 * t - 1));
    node->C = (struct BTreeNode **)malloc(sizeof(struct BTreeNode *) * (2 * t));
    node->n = 0;
    return node;
}

// Traverse the tree
void traverse(struct BTreeNode *root) {
    if (root == NULL) return;
    int i;
    for (i = 0; i < root->n; i++) {
        if (!root->leaf)
            traverse(root->C[i]);
        printf("%d ", root->keys[i]);
    }
    if (!root->leaf)
        traverse(root->C[i]);
}

// Search key in B-Tree
struct BTreeNode *search(struct BTreeNode *root, int k) {
    int i = 0;
    while (i < root->n && k > root->keys[i])
        i++;
    if (i < root->n && root->keys[i] == k)
        return root;
    if (root->leaf)
        return NULL;
    return search(root->C[i], k);
}

// Insert operation
void insert(struct BTreeNode **root, int k) {
    if (*root == NULL) {
        *root = createNode(MIN_DEGREE, 1);
        (*root)->keys[0] = k;
        (*root)->n = 1;
    } else {
        if ((*root)->n == 2 * MIN_DEGREE - 1) {
            struct BTreeNode *s = createNode(MIN_DEGREE, 0);
            s->C[0] = *root;
            splitChild(s, 0, *root);
            int i = 0;
            if (s->keys[0] < k)
                i++;
            insertNonFull(s->C[i], k);
            *root = s;
        } else
            insertNonFull(*root, k);
    }
}

void insertNonFull(struct BTreeNode *x, int k) {
    int i = x->n - 1;
    if (x->leaf) {
        while (i >= 0 && x->keys[i] > k) {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = k;
        x->n++;
    } else {
        while (i >= 0 && x->keys[i] > k)
            i--;
        i++;
        if (x->C[i]->n == 2 * x->t - 1) {
            splitChild(x, i, x->C[i]);
            if (x->keys[i] < k)
                i++;
        }
        insertNonFull(x->C[i], k);
    }
}

void splitChild(struct BTreeNode *x, int i, struct BTreeNode *y) {
    int t = y->t;
    struct BTreeNode *z = createNode(t, y->leaf);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    if (!y->leaf) {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j + t];
    }

    y->n = t - 1;

    for (int j = x->n; j >= i + 1; j--)
        x->C[j + 1] = x->C[j];

    x->C[i + 1] = z;

    for (int j = x->n - 1; j >= i; j--)
        x->keys[j + 1] = x->keys[j];

    x->keys[i] = y->keys[t - 1];
    x->n++;
}

// Find index of key k
int findKey(struct BTreeNode *root, int k) {
    int idx = 0;
    while (idx < root->n && root->keys[idx] < k)
        ++idx;
    return idx;
}

// Delete operation
void deleteKey(struct BTreeNode *root, int k) {
    int idx = findKey(root, k);

    if (idx < root->n && root->keys[idx] == k) {
        if (root->leaf)
            removeFromLeaf(root, idx);
        else
            removeFromNonLeaf(root, idx);
    } else {
        if (root->leaf) {
            printf("The key %d does not exist.\n", k);
            return;
        }

        int flag = (idx == root->n);

        if (root->C[idx]->n < root->t)
            fill(root, idx);

        if (flag && idx > root->n)
            deleteKey(root->C[idx - 1], k);
        else
            deleteKey(root->C[idx], k);
    }
}

void removeFromLeaf(struct BTreeNode *root, int idx) {
    for (int i = idx + 1; i < root->n; ++i)
        root->keys[i - 1] = root->keys[i];
    root->n--;
}

void removeFromNonLeaf(struct BTreeNode *root, int idx) {
    int k = root->keys[idx];

    if (root->C[idx]->n >= root->t) {
        int pred = getPred(root, idx);
        root->keys[idx] = pred;
        deleteKey(root->C[idx], pred);
    } else if (root->C[idx + 1]->n >= root->t) {
        int succ = getSucc(root, idx);
        root->keys[idx] = succ;
        deleteKey(root->C[idx + 1], succ);
    } else {
        merge(root, idx);
        deleteKey(root->C[idx], k);
    }
}

int getPred(struct BTreeNode *root, int idx) {
    struct BTreeNode *cur = root->C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];
    return cur->keys[cur->n - 1];
}

int getSucc(struct BTreeNode *root, int idx) {
    struct BTreeNode *cur = root->C[idx + 1];
    while (!cur->leaf)
        cur = cur->C[0];
    return cur->keys[0];
}

void fill(struct BTreeNode *root, int idx) {
    if (idx != 0 && root->C[idx - 1]->n >= root->t)
        borrowFromPrev(root, idx);
    else if (idx != root->n && root->C[idx + 1]->n >= root->t)
        borrowFromNext(root, idx);
    else {
        if (idx != root->n)
            merge(root, idx);
        else
            merge(root, idx - 1);
    }
}

void borrowFromPrev(struct BTreeNode *root, int idx) {
    struct BTreeNode *child = root->C[idx];
    struct BTreeNode *sibling = root->C[idx - 1];

    for (int i = child->n - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    if (!child->leaf) {
        for (int i = child->n; i >= 0; --i)
            child->C[i + 1] = child->C[i];
    }

    child->keys[0] = root->keys[idx - 1];

    if (!root->leaf)
        child->C[0] = sibling->C[sibling->n];

    root->keys[idx - 1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;
}

void borrowFromNext(struct BTreeNode *root, int idx) {
    struct BTreeNode *child = root->C[idx];
    struct BTreeNode *sibling = root->C[idx + 1];

    child->keys[(child->n)] = root->keys[idx];

    if (!(child->leaf))
        child->C[(child->n) + 1] = sibling->C[0];

    root->keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->n; ++i)
        sibling->keys[i - 1] = sibling->keys[i];

    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; ++i)
            sibling->C[i - 1] = sibling->C[i];
    }

    child->n += 1;
    sibling->n -= 1;
}

void merge(struct BTreeNode *root, int idx) {
    struct BTreeNode *child = root->C[idx];
    struct BTreeNode *sibling = root->C[idx + 1];

    child->keys[child->n] = root->keys[idx];

    for (int i = 0; i < sibling->n; ++i)
        child->keys[child->n + 1 + i] = sibling->keys[i];

    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; ++i)
            child->C[child->n + 1 + i] = sibling->C[i];
    }

    for (int i = idx + 1; i < root->n; ++i)
        root->keys[i - 1] = root->keys[i];

    for (int i = idx + 2; i <= root->n; ++i)
        root->C[i - 1] = root->C[i];

    child->n += sibling->n + 1;
    root->n--;

    free(sibling);
}

// Main function
int main() {
    struct BTreeNode *root = NULL;
    int choice, key;

    while (1) {
        printf("\n\nB-TREE OPERATIONS\n");
        printf("1. Insert\n2. Delete\n3. Traverse\n4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter key to insert: ");
                scanf("%d", &key);
                insert(&root, key);
                break;
            case 2:
                printf("Enter key to delete: ");
                scanf("%d", &key);
                if (root != NULL)
                    deleteKey(root, key);
                else
                    printf("Tree is empty.\n");
                break;
            case 3:
                printf("B-Tree traversal: ");
                traverse(root);
                printf("\n");
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
