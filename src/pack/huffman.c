#include "huffman.h"

#include "../core/check.h"

enum {
    END_OF_BLOCK = 256,
    FIXED_EIGHT_END = 144,
    FIXED_SEVEN_START = 280,
    FIXED_SHORT_BITS = 7,
    FIXED_MIDDLE_BITS = 8,
    FIXED_LONG_BITS = 9,
    FIXED_DISTANCE_BITS = 5,
    FIXED_SHORT_COUNT = 24,
    FIXED_KINDS = 2
};

static short fixed_lengths[HUFFMAN_SYMBOL_LIMIT];

static bool lengths_fit(const HuffmanCode *code)
{
    int left = 1;
    int bits;

    if (!CHECK(code != NULL) || !CHECK(HUFFMAN_MAX_BITS > 0)) {
        return false;
    }
    for (bits = 1; bits <= HUFFMAN_MAX_BITS; ++bits) {
        left = (left << 1) - code->count[bits];
        if (left < 0) {
            return false;
        }
    }
    return true;
}

static bool count_lengths(HuffmanCode *code, const short *length, int symbols)
{
    int bits;
    int symbol;

    if (!CHECK(code != NULL) || !CHECK(length != NULL)) {
        return false;
    }
    for (bits = 0; bits <= HUFFMAN_MAX_BITS; ++bits) {
        code->count[bits] = 0;
    }
    for (symbol = 0; (symbol < symbols) && (symbol < HUFFMAN_SYMBOL_LIMIT); ++symbol) {
        if ((length[symbol] < 0) || (length[symbol] > HUFFMAN_MAX_BITS)) {
            return false;
        }
        ++code->count[length[symbol]];
    }
    return lengths_fit(code);
}

bool huffman_build(HuffmanCode *code, const short *length, int symbols)
{
    short offsets[HUFFMAN_MAX_BITS + 1];
    int bits;
    int symbol;

    if (!CHECK((symbols > 0) && (symbols <= HUFFMAN_SYMBOL_LIMIT)) || !count_lengths(code, length, symbols)) {
        return false;
    }
    offsets[1] = 0;
    for (bits = 1; bits < HUFFMAN_MAX_BITS; ++bits) {
        offsets[bits + 1] = (short)(offsets[bits] + code->count[bits]);
    }
    for (symbol = 0; (symbol < symbols) && (symbol < HUFFMAN_SYMBOL_LIMIT); ++symbol) {
        if (length[symbol] != 0) {
            code->symbol[offsets[length[symbol]]] = (short)symbol;
            ++offsets[length[symbol]];
        }
    }
    return true;
}

static short fixed_length(int symbol)
{
    if (!CHECK(symbol >= 0) || !CHECK(symbol < HUFFMAN_SYMBOL_LIMIT)) {
        return 0;
    }
    if (symbol < FIXED_EIGHT_END) {
        return FIXED_MIDDLE_BITS;
    }
    if (symbol < END_OF_BLOCK) {
        return FIXED_LONG_BITS;
    }
    return (symbol < FIXED_SEVEN_START) ? FIXED_SHORT_BITS : FIXED_MIDDLE_BITS;
}

bool huffman_build_fixed(HuffmanCode *code, bool literals)
{
    static const int LIMITS[FIXED_KINDS] = { HUFFMAN_DISTANCE_LIMIT, HUFFMAN_SYMBOL_LIMIT };
    static const int TOLD_BITS[FIXED_KINDS] = { FIXED_DISTANCE_BITS, FIXED_SHORT_BITS };
    static const int TOLD_COUNTS[FIXED_KINDS] = { HUFFMAN_DISTANCE_LIMIT, FIXED_SHORT_COUNT };
    int kind = (int)literals;
    int symbol;

    if (!CHECK(code != NULL) || !CHECK(LIMITS[kind] <= HUFFMAN_SYMBOL_LIMIT)) {
        return false;
    }
    for (symbol = 0; (symbol < LIMITS[kind]) && (symbol < HUFFMAN_SYMBOL_LIMIT); ++symbol) {
        fixed_lengths[symbol] = literals ? fixed_length(symbol) : FIXED_DISTANCE_BITS;
    }
    return huffman_build(code, fixed_lengths, LIMITS[kind]) && CHECK(code->count[TOLD_BITS[kind]] == TOLD_COUNTS[kind]);
}
