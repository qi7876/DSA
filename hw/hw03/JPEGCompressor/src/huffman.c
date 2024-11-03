/**
 * @file src/huffman.c
 * @author qi7876
 * @brief Huffman coding for quantized DCT coefficients.
 */

#include "../include/JPEGCompressor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a new huffman node.
HuffmanNode* createNode(int symbol, int frequency)
{
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    node->symbol = symbol;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

// Define the structure of a min heap.
typedef struct MinHeap {
    int size;
    int capacity;
    HuffmanNode** array;
} MinHeap;

// Create a new min heap.
MinHeap* createMinHeap(int capacity)
{
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HuffmanNode**)malloc(minHeap->capacity * sizeof(HuffmanNode*));
    return minHeap;
}

// Swap two nodes.
void swapNodes(HuffmanNode** a, HuffmanNode** b)
{
    HuffmanNode* t = *a;
    *a = *b;
    *b = t;
}

// Min heapify.
void minHeapify(MinHeap* minHeap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
        smallest = right;

    if (smallest != idx) {
        swapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Check if the size of the min heap is one.
int isSizeOne(MinHeap* minHeap)
{
    return minHeap->size == 1;
}

// Extract the minimum node from the min heap.
HuffmanNode* extractMin(MinHeap* minHeap)
{
    HuffmanNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);
    return temp;
}

// Insert a new node into the min heap.
void insertMinHeap(MinHeap* minHeap, HuffmanNode* node)
{
    minHeap->size++;
    int i = minHeap->size - 1;
    while (i && node->frequency < minHeap->array[(i - 1) / 2]->frequency) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

// Build a min heap.
MinHeap* buildMinHeap(int symbols[], int freq[], int size)
{
    MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = createNode(symbols[i], freq[i]);
    minHeap->size = size;
    // Heapify all internal nodes from the last non-leaf node to the root.
    for (int i = (minHeap->size - 2) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
    return minHeap;
}

// Build a huffman tree.
HuffmanNode* buildHuffmanTree(const int symbols[], const int frequencies[], int size)
{
    HuffmanNode *left, *right, *top;
    MinHeap* minHeap = buildMinHeap((int*)symbols, (int*)frequencies, size);
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = createNode(-1, left->frequency + right->frequency);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    HuffmanNode* root = extractMin(minHeap);
    free(minHeap->array);
    free(minHeap);
    return root;
}

// Generate huffman codes.
void generateCodes(HuffmanNode* root, char* code, int top, HuffmanCode* huffmanCodes, int* index)
{
    if (root->left) {
        code[top] = '0';
        generateCodes(root->left, code, top + 1, huffmanCodes, index);
    }
    if (root->right) {
        code[top] = '1';
        generateCodes(root->right, code, top + 1, huffmanCodes, index);
    }
    if (!root->left && !root->right) {
        code[top] = '\0';
        strcpy(huffmanCodes[*index].code, code);
        huffmanCodes[*index].symbol = root->symbol;
        huffmanCodes[*index].length = top;
        (*index)++;
    }
}

// Print huffman codes.

void printCodes(const HuffmanCode* huffmanCodes, int size)
{
    for (int i = 0; i < size; ++i)
        printf("Symbol: %d, Code: %s\n", huffmanCodes[i].symbol, huffmanCodes[i].code);
}

// Free huffman tree.
void freeHuffmanTree(HuffmanNode* root)
{
    if (root == NULL)
        return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}
