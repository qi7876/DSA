/**
 * @file src/main.c
 * @author qi7876
 * @brief Main program for JPEG compression.
 */

#include "../include/JPEGCompressor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // Define a input 8x8 block as an example.
    Block8x8 input_block = {
        .data = {
            { 52, 55, 61, 66, 70, 61, 64, 73 },
            { 63, 59, 55, 90, 109, 85, 69, 72 },
            { 62, 59, 68, 113, 144, 104, 66, 73 },
            { 63, 58, 71, 122, 154, 106, 70, 69 },
            { 67, 61, 68, 104, 126, 88, 68, 70 },
            { 79, 65, 60, 70, 77, 68, 58, 75 },
            { 85, 71, 64, 59, 55, 61, 65, 83 },
            { 87, 79, 69, 68, 65, 76, 78, 94 } }
    };

    // Perform DCT on the input block.
    float dct_coeffs[8][8];
    performDCT(&input_block, dct_coeffs);
    printf("DCT Coefficients:\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
            printf("%8.2f ", dct_coeffs[i][j]);
        printf("\n");
    }
    printf("\n");

    // Quantize the DCT coefficients.
    int quantized_coeffs[8][8];
    performQuantization(dct_coeffs, quantized_coeffs, quant_matrix);
    printf("Quantized Coefficients:\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
            printf("%4d ", quantized_coeffs[i][j]);
        printf("\n");
    }
    printf("\n");

    // Zig-zag scan the quantized coefficients.
    int scanned_coeffs[64];
    zScan(quantized_coeffs, scanned_coeffs);
    printf("Z-scanned Coefficients:\n");
    for (int i = 0; i < 64; i++) {
        printf("%4d ", scanned_coeffs[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");

    // Huffman coding for the scanned coefficients.
    // Statize the frequency of scanned coefficients.
    int freq_table[256] = { 0 };
    for (int i = 0; i < 64; ++i) {
        int index = scanned_coeffs[i] + 128;
        if (index >= 0 && index < 256) {
            freq_table[index]++;
        } else {
            printf("Error: scanned_coeffs[%d] = %d is out of range\n", i, scanned_coeffs[i]);
            return -1;
        }
    }

    // Prepare symbols and frequencies for Huffman coding.
    int symbols[256];
    int frequencies[256];
    int unique = 0;
    for (int i = -128; i <= 127; ++i) {
        int index = i + 128;
        if (freq_table[index] > 0) {
            symbols[unique] = i;
            frequencies[unique] = freq_table[index];
            unique++;
        }
    }

    // Print symbols and frequencies.
    printf("Symbols and Frequencies:\n");
    for (int i = 0; i < unique; ++i) {
        printf("Symbol: %d, Frequency: %d\n", symbols[i], frequencies[i]);
    }
    printf("\n");

    // Build the Huffman tree.
    HuffmanNode* root = buildHuffmanTree(symbols, frequencies, unique);

    // Generate Huffman codes.
    char code[256];
    HuffmanCode huffmanCodes[256];
    int index = 0;
    generateCodes(root, code, 0, huffmanCodes, &index);

    // Print Huffman codes.
    printf("Huffman Codes:\n");
    printCodes(huffmanCodes, index);
    printf("\n");

    // Encode the scanned coefficients.
    printf("Encoded Data:\n");
    for (int i = 0; i < 64; ++i) {
        // Find the Huffman code for the scanned coefficients.
        int found = 0;
        for (int j = 0; j < index; ++j) {
            if (huffmanCodes[j].symbol == scanned_coeffs[i]) {
                printf("%s ", huffmanCodes[j].code);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Error: Symbol %d not found in Huffman Codes\n", scanned_coeffs[i]);
        }
    }
    printf("\n");

    // Free the Huffman tree.
    freeHuffmanTree(root);

    return 0;
}
