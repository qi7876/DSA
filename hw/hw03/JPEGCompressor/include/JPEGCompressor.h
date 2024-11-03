/**
 * @file include/JPEGCompressor.h
 * @author qi7876
 * @brief Difine the variables and functions for JPEH compression.
 */

#ifndef JPEG_COMPRESS_H
#define JPEG_COMPRESS_H

#include <math.h>
#include <stdint.h>

// Define 8x8 block structure.
typedef struct {
    float data[8][8];
} Block8x8;

// Define the quantization matrix.
extern const int quant_matrix[8][8];

// DCT function.
void performDCT(const Block8x8* input, float output[8][8]);

// Quantization function.
void performQuantization(const float input[8][8], int output[8][8], const int quant_matrix[8][8]);

// Zig-zag scan function.
void zScan(const int input[8][8], int output[64]);

// Huffman tree node structure.
typedef struct HuffmanNode {
    int symbol;
    int frequency;
    struct HuffmanNode *left, *right;
} HuffmanNode;

// Huffman coding structure.
typedef struct {
    int symbol;
    char code[256];
    int length;
} HuffmanCode;

// Huffman coding functions.
HuffmanNode* buildHuffmanTree(const int symbols[], const int frequencies[], int size);
void generateCodes(HuffmanNode* root, char* code, int top, HuffmanCode* huffmanCodes, int* index);
void printCodes(const HuffmanCode* huffmanCodes, int size);
void freeHuffmanTree(HuffmanNode* root);

#endif // JPEG_COMPRESS_H
