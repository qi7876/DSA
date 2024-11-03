/**
 * @file src/dct.c
 * @author qi7876
 * @brief Discrete Cosine Transform (DCT) implementation.
 */

#include "../include/JPEGCompressor.h"

#define PI 3.14159265358979323846

// Pre-calculate DCT coefficients.
static float C(int u)
{
    return (u == 0) ? (1.0f / sqrtf(2.0f)) : 1.0f;
}

void performDCT(const Block8x8* input, float output[8][8])
{
    for (int u = 0; u < 8; u++) {
        for (int v = 0; v < 8; v++) {
            float sum = 0.0f;
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    sum += input->data[x][y] * cosf(((2 * x + 1) * u * PI) / 16.0f) * cosf(((2 * y + 1) * v * PI) / 16.0f);
                }
            }
            sum *= 0.25f * C(u) * C(v);
            output[u][v] = sum;
        }
    }
}
