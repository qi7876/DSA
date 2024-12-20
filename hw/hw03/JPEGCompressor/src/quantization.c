/**
 * @file src/quantization.c
 * @author qi7876
 * @brief Quantization on DCT coefficients.
 */

#include "../include/JPEGCompressor.h"

const int quant_matrix[8][8] = {
    { 16, 11, 10, 16, 24, 40, 51, 61 },
    { 12, 12, 14, 19, 26, 58, 60, 55 },
    { 14, 13, 16, 24, 40, 57, 69, 56 },
    { 14, 17, 22, 29, 51, 87, 80, 62 },
    { 18, 22, 37, 56, 68, 109, 103, 77 },
    { 24, 35, 55, 64, 81, 104, 113, 92 },
    { 49, 64, 78, 87, 103, 121, 120, 101 },
    { 72, 92, 95, 98, 112, 100, 103, 99 }
};

void performQuantization(const float input[8][8], int output[8][8], const int quant_matrix[8][8])
{
    for (int u = 0; u < 8; u++) {
        for (int v = 0; v < 8; v++) {
            output[u][v] = roundf(input[u][v] / quant_matrix[u][v]);
        }
    }
}
