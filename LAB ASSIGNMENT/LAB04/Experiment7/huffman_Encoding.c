#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 256

// Huffman Tree Node
struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// MinHeap: collection of heap (priority queue) nodes
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

// Create a new node
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// Create a min heap with given capacity
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode*)malloc(minHeap->capacity * sizeof(struct MinHeapNode));
    return minHeap;
}

// Swap two heap nodes
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Heapify function
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Check if size is 1
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// Extract minimum node
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// Insert new node into min heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// Build min heap
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; i--)
        minHeapify(minHeap, i);
}

// Print an array
void printArr(int arr[], int n) {
    for (int i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf(" ");
}

// Create a min heap of capacity equal to size and insert all characters
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

// Build Huffman Tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

// Store codes in an array
void storeCodes(struct MinHeapNode* root, int arr[], int top, char codes[256][256]) {
    if (root->left) {
        arr[top] = 0;
        storeCodes(root->left, arr, top + 1, codes);
    }

    if (root->right) {
        arr[top] = 1;
        storeCodes(root->right, arr, top + 1, codes);
    }

    if (!root->left && !root->right) {
        for (int i = 0; i < top; ++i)
            codes[(unsigned char)root->data][i] = arr[i] + '0';
        codes[(unsigned char)root->data][top] = '\0';
    }
}

// Calculate frequency of characters from file
void calcFrequency(char* filename, int freq[]) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("File not found!\n");
        exit(1);
    }

    char ch;
    while ((ch = fgetc(fp)) != EOF)
        freq[(unsigned char)ch]++;
    fclose(fp);
}

// Huffman Encoding main function
void HuffmanCodes(char* filename) {
    int freq[256] = {0};
    calcFrequency(filename, freq);

    char data[256];
    int freqArr[256];
    int size = 0;

    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            data[size] = (char)i;
            freqArr[size] = freq[i];
            size++;
        }
    }

    struct MinHeapNode* root = buildHuffmanTree(data, freqArr, size);

    int arr[MAX_TREE_HT], top = 0;
    char codes[256][256] = {{0}};
    storeCodes(root, arr, top, codes);

    printf("\nHuffman Codes for each character:\n");
    for (int i = 0; i < size; i++)
        printf("%c: %s\n", data[i], codes[(unsigned char)data[i]]);

    FILE* fp = fopen(filename, "r");
    FILE* out = fopen("encoded.txt", "w");

    if (!fp || !out) {
        printf("Error opening files!\n");
        return;
    }

    char ch;
    while ((ch = fgetc(fp)) != EOF)
        fprintf(out, "%s", codes[(unsigned char)ch]);

    fclose(fp);
    fclose(out);

    printf("\nEncoded data written to 'encoded.txt'\n");
}

// Main Function
int main() {
    char filename[100];
    printf("Enter input text filename: ");
    scanf("%s", filename);

    HuffmanCodes(filename);
    return 0;
}