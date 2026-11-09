#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdbool.h>

enum {
    HUFFMAN_MAX_BITS = 15,
    HUFFMAN_SYMBOL_LIMIT = 288,
    HUFFMAN_DISTANCE_LIMIT = 30
};

typedef struct {
    short count[HUFFMAN_MAX_BITS + 1];
    short symbol[HUFFMAN_SYMBOL_LIMIT];
} HuffmanCode;

bool huffman_build(HuffmanCode *code, const short *length, int symbols);
bool huffman_build_fixed_literals(HuffmanCode *code);
bool huffman_build_fixed_distances(HuffmanCode *code);

#endif
