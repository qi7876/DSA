cmake_minimum_required(VERSION 3.10)
project(JPEGCompression C)

set(CMAKE_C_STANDARD 99)

# Add include directory
include_directories(include)

# Add source files
set(SOURCES
    src/dct.c
    src/quantization.c
    src/zscan.c
    src/huffman.c
    src/main.c
)

# Creat executable file
add_executable(JPEGCompressor ${SOURCES})
