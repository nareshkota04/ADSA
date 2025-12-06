#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int key;
    struct node *left, *right;
} Node;

/* ---------- Utility Function ---------- */
Node* newNode(int key) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

/* ---------- Rotations ---------- */
Node* rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

/* ---------- Splay Operation ---------- */
Node* splay(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;

    /* Key in left subtree */
    if (key < root->key) {
        if (root->left == NULL) 
            return root;

        // Zig-Zig (LL)
        if (key < root->left->key) {
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
        }
        // Zig-Zag (LR)
        else if (key > root->left->key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }

        return (root->left == NULL) ? root : rightRotate(root);
    }

    /* Key in right subtree */
    else {
        if (root->right == NULL) 
            return root;

        // Zig-Zig (RR)
        if (key > root->right->key) {
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }
        // Zig-Zag (RL)
        else if (key < root->right->key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }

        return (root->right == NULL) ? root : leftRotate(root);
    }
}

/* ---------- Insert ---------- */
Node* insert(Node* root, int key) {
    if (root == NULL)
        return newNode(key);

    root = splay(root, key);

    if (root->key == key)
        return root;

    Node* n = newNode(key);

    if (key < root->key) {
        n->right = root;
        n->left = root->left;
        root->left = NULL;
    } else {
        n->left = root;
        n->right = root->right;
        root->right = NULL;
    }

    return n;
}

/* ---------- Search ---------- */
Node* search(Node* root, int key) {
    return splay(root, key);
}

/* ---------- Delete ---------- */
Node* delete(Node* root, int key) {
    Node* temp;

    if (!root)
        return NULL;

    root = splay(root, key);

    if (root->key != key)
        return root; // Key not found

    if (root->left == NULL) {
        temp = root;
        root = root->right;
    } else {
        temp = root;
        root = splay(root->left, key);  // Splay max of left subtree
        root->right = temp->right;
    }

    free(temp);
    return root;
}

/* ---------- Display (Inorder) ---------- */
void inorder(Node* root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

/* ---------- MAIN (Menu + User input) ---------- */
int main() {
    Node* root = NULL;
    int choice, key;

    while (1) {
        printf("\n\n===== SPLAY TREE MENU =====\n");
        printf("1. Insert\n");
        printf("2. Search\n");
        printf("3. Delete\n");
        printf("4. Display (Inorder)\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter key to insert: ");
                scanf("%d", &key);
                root = insert(root, key);
                printf("Inserted %d\n", key);
                break;

            case 2:
                printf("Enter key to search: ");
                scanf("%d", &key);
                root = search(root, key);
                printf("Searched (Splayed) %d\n", key);
                break;

            case 3:
                printf("Enter key to delete: ");
                scanf("%d", &key);
                root = delete(root, key);
                printf("Deleted %d (if existed)\n", key);
                break;

            case 4:
                printf("Inorder Traversal: ");
                inorder(root);
                printf("\n");
                break;

            case 5:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}
