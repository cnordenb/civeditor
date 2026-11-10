#include "inflate.h"

#include <string.h>

#include "../core/check.h"
#include "huffman.h"

enum {
    BYTE_BITS = 8,
    MAX_BITS = HUFFMAN_MAX_BITS,
    TAKE_LIMIT = 16,
    READ_LIMIT = 4,
    LITERAL_CODE_LIMIT = 288,
    LITERAL_CODE_MOST = 286,
    DISTANCE_CODE_LIMIT = DEFLATE_DISTANCE_SYMBOLS,
    ALL_CODE_LIMIT = LITERAL_CODE_LIMIT + DISTANCE_CODE_LIMIT,
    LENGTH_CODE_COUNT = 19,
    END_OF_BLOCK = 256,
    FIRST_LENGTH = 257,
    LENGTH_SYMBOLS = DEFLATE_LENGTH_SYMBOLS,
    LITERAL_COUNT_BITS = 5,
    DISTANCE_COUNT_BITS = 5,
    LENGTH_COUNT_BITS = 4,
    LENGTH_COUNT_BASE = 4,
    LENGTH_CODE_BITS = 3,
    FINAL_BITS = 1,
    TYPE_BITS = 2,
    REPEAT_PREVIOUS = 16,
    REPEAT_RULE_COUNT = 3,
    REPEAT_LIMIT = 138,
    MATCH_LIMIT = 258,
    STORED_HEADER = 4,
    STORED_MASK = 0xFFFF,
    BLOCK_STORED = 0,
    BLOCK_FIXED = 1,
    BLOCK_DYNAMIC = 2,
    BLOCK_LIMIT = 16777216,
    SYMBOL_LIMIT = 2147483647,
    NO_SYMBOL = -1
};

typedef struct {
    const unsigned char *in;
    size_t in_size;
    size_t in_pos;
    unsigned char *out;
    size_t out_capacity;
    size_t out_pos;
    unsigned int bit_buffer;
    int bit_count;
    bool failed;
} Inflater;

typedef struct {
    int value;
    int extra_bits;
    int extra_base;
} RepeatRule;

const int DEFLATE_LENGTH_BASE[DEFLATE_LENGTH_SYMBOLS] = {
    3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258
};

const int DEFLATE_LENGTH_EXTRA[DEFLATE_LENGTH_SYMBOLS] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0
};

const int DEFLATE_DISTANCE_BASE[DEFLATE_DISTANCE_SYMBOLS] = {
    1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097,
    6145, 8193, 12289, 16385, 24577
};

const int DEFLATE_DISTANCE_EXTRA[DEFLATE_DISTANCE_SYMBOLS] = {
    0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13
};

static const int CODE_LENGTH_ORDER[LENGTH_CODE_COUNT] = {
    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

static const RepeatRule REPEAT_RULES[REPEAT_RULE_COUNT] = { { NO_SYMBOL, 2, 3 }, { 0, 3, 3 }, { 0, 7, 11 } };

static HuffmanCode literal_code;
static HuffmanCode distance_code;
static HuffmanCode length_code;
static short lengths[ALL_CODE_LIMIT];

static unsigned int take_bits(Inflater *z, int need)
{
    unsigned int value;
    int read;

    if (!CHECK(z != NULL) || !CHECK((need >= 0) && (need <= TAKE_LIMIT))) {
        return 0U;
    }
    for (read = 0; (z->bit_count < need) && (read < READ_LIMIT); ++read) {
        if (z->in_pos >= z->in_size) {
            z->failed = true;
            return 0U;
        }
        z->bit_buffer |= (unsigned int)z->in[z->in_pos] << (unsigned int)z->bit_count;
        ++z->in_pos;
        z->bit_count += BYTE_BITS;
    }
    value = z->bit_buffer & ((1U << (unsigned int)need) - 1U);
    z->bit_buffer >>= (unsigned int)need;
    z->bit_count -= need;
    return value;
}

static int take_bit(Inflater *z)
{
    int bit;

    if (z->bit_count == 0) {
        if (z->in_pos >= z->in_size) {
            z->failed = true;
            return 0;
        }
        z->bit_buffer = z->in[z->in_pos];
        ++z->in_pos;
        z->bit_count = BYTE_BITS;
    }
    bit = (int)(z->bit_buffer & 1U);
    z->bit_buffer >>= 1U;
    --z->bit_count;
    return bit;
}

static int decode_symbol(Inflater *z, const HuffmanCode *code)
{
    int value = 0;
    int first = 0;
    int index = 0;
    int length;
    int count;

    if (!CHECK(z != NULL) || !CHECK(code != NULL)) {
        return NO_SYMBOL;
    }
    for (length = 1; length <= MAX_BITS; ++length) {
        value |= take_bit(z);
        count = code->count[length];
        if (value - count < first) {
            return code->symbol[index + (value - first)];
        }
        index += count;
        first = (first + count) << 1;
        value <<= 1;
    }
    z->failed = true;
    return NO_SYMBOL;
}

static bool copy_match(Inflater *z, int symbol)
{
    int index = symbol - FIRST_LENGTH;
    int distance_symbol;
    size_t length;
    size_t distance;
    size_t step;

    if (!CHECK(z != NULL) || !CHECK(index >= 0) || (index >= LENGTH_SYMBOLS)) {
        return false;
    }
    length = (size_t)DEFLATE_LENGTH_BASE[index] + take_bits(z, DEFLATE_LENGTH_EXTRA[index]);
    distance_symbol = decode_symbol(z, &distance_code);
    if ((distance_symbol < 0) || (distance_symbol >= DISTANCE_CODE_LIMIT)) {
        return false;
    }
    distance = (size_t)DEFLATE_DISTANCE_BASE[distance_symbol] +
               take_bits(z, DEFLATE_DISTANCE_EXTRA[distance_symbol]);
    if (z->failed || (distance > z->out_pos) || (length > z->out_capacity - z->out_pos)) {
        return false;
    }
    for (step = 0U; (step < length) && (step < MATCH_LIMIT); ++step) {
        z->out[z->out_pos] = z->out[z->out_pos - distance];
        ++z->out_pos;
    }
    return true;
}

static bool inflate_codes(Inflater *z)
{
    int symbol;
    int count;

    if (!CHECK(z != NULL) || !CHECK(z->out != NULL)) {
        return false;
    }
    for (count = 0; count < SYMBOL_LIMIT; ++count) {
        symbol = decode_symbol(z, &literal_code);
        if (z->failed || (symbol < 0)) {
            return false;
        }
        if (symbol == END_OF_BLOCK) {
            return true;
        }
        if (symbol > END_OF_BLOCK) {
            if (!copy_match(z, symbol)) {
                return false;
            }
        } else if (z->out_pos < z->out_capacity) {
            z->out[z->out_pos] = (unsigned char)symbol;
            ++z->out_pos;
        } else {
            return false;
        }
    }
    return false;
}

static bool inflate_stored(Inflater *z)
{
    size_t length;
    size_t complement;

    if (!CHECK(z != NULL) || !CHECK(z->out_pos <= z->out_capacity)) {
        return false;
    }
    z->bit_buffer = 0U;
    z->bit_count = 0;
    if (z->in_size - z->in_pos < (size_t)STORED_HEADER) {
        return false;
    }
    length = (size_t)z->in[z->in_pos] | ((size_t)z->in[z->in_pos + 1U] << BYTE_BITS);
    complement = (size_t)z->in[z->in_pos + 2U] | ((size_t)z->in[z->in_pos + 3U] << BYTE_BITS);
    z->in_pos += (size_t)STORED_HEADER;
    if ((length != (~complement & (size_t)STORED_MASK)) || (length > z->in_size - z->in_pos) ||
        (length > z->out_capacity - z->out_pos)) {
        return false;
    }
    (void)memcpy(z->out + z->out_pos, z->in + z->in_pos, length);
    z->in_pos += length;
    z->out_pos += length;
    return true;
}

static bool read_length_code(Inflater *z, int count)
{
    int index;

    if (!CHECK(z != NULL) || !CHECK(count <= LENGTH_CODE_COUNT)) {
        return false;
    }
    for (index = 0; index < LENGTH_CODE_COUNT; ++index) {
        lengths[index] = 0;
    }
    for (index = 0; (index < count) && (index < LENGTH_CODE_COUNT); ++index) {
        lengths[CODE_LENGTH_ORDER[index]] = (short)take_bits(z, LENGTH_CODE_BITS);
    }
    return !z->failed && huffman_build(&length_code, lengths, LENGTH_CODE_COUNT);
}

static int repeat_lengths(Inflater *z, int symbol, int index)
{
    RepeatRule rule;
    int repeat;
    int step;

    if (!CHECK(z != NULL) || !CHECK((symbol >= REPEAT_PREVIOUS) && (symbol < LENGTH_CODE_COUNT))) {
        return NO_SYMBOL;
    }
    rule = REPEAT_RULES[symbol - REPEAT_PREVIOUS];
    if ((symbol == REPEAT_PREVIOUS) && (index == 0)) {
        return NO_SYMBOL;
    }
    rule.value = (symbol == REPEAT_PREVIOUS) ? lengths[index - 1] : rule.value;
    repeat = rule.extra_base + (int)take_bits(z, rule.extra_bits);
    if (z->failed || (repeat > ALL_CODE_LIMIT - index)) {
        return NO_SYMBOL;
    }
    for (step = 0; (step < repeat) && (step < REPEAT_LIMIT); ++step) {
        lengths[index + step] = (short)rule.value;
    }
    return index + repeat;
}

static bool read_lengths(Inflater *z, int total)
{
    int index;
    int symbol;

    if (!CHECK(z != NULL) || !CHECK(total <= ALL_CODE_LIMIT)) {
        return false;
    }
    for (index = 0; (index >= 0) && (index < total) && (index < ALL_CODE_LIMIT);) {
        symbol = decode_symbol(z, &length_code);
        if (z->failed || (symbol < 0)) {
            return false;
        }
        if (symbol < REPEAT_PREVIOUS) {
            lengths[index] = (short)symbol;
            ++index;
        } else {
            index = repeat_lengths(z, symbol, index);
        }
    }
    return (index == total) && (lengths[END_OF_BLOCK] != 0);
}

static bool build_dynamic_codes(Inflater *z)
{
    int literals;
    int distances;
    int code_lengths;

    if (!CHECK(z != NULL) || !CHECK(!z->failed)) {
        return false;
    }
    literals = (int)take_bits(z, LITERAL_COUNT_BITS) + FIRST_LENGTH;
    distances = (int)take_bits(z, DISTANCE_COUNT_BITS) + 1;
    code_lengths = (int)take_bits(z, LENGTH_COUNT_BITS) + LENGTH_COUNT_BASE;
    if (z->failed || (literals > LITERAL_CODE_MOST) || (distances > DISTANCE_CODE_LIMIT) ||
        !read_length_code(z, code_lengths) || !read_lengths(z, literals + distances)) {
        return false;
    }
    return huffman_build(&literal_code, lengths, literals) &&
           huffman_build(&distance_code, &lengths[literals], distances);
}

static bool inflate_block(Inflater *z, unsigned int type)
{
    if (!CHECK(z != NULL) || !CHECK(!z->failed)) {
        return false;
    }
    switch (type) {
    case BLOCK_STORED:
        return inflate_stored(z);
    case BLOCK_FIXED:
        return huffman_build_fixed(&literal_code, true) && huffman_build_fixed(&distance_code, false) &&
               inflate_codes(z);
    case BLOCK_DYNAMIC:
        return build_dynamic_codes(z) && inflate_codes(z);
    default:
        return false;
    }
}

bool inflate_raw(ByteSpan packed, ByteBuffer *plain)
{
    Inflater z = { NULL, 0U, 0U, NULL, 0U, 0U, 0U, 0, false };
    unsigned int last = 0U;
    int block;

    if (!CHECK(plain != NULL) || !CHECK(packed.bytes != NULL) || !CHECK(plain->bytes != NULL)) {
        return false;
    }
    z.in = packed.bytes;
    z.in_size = packed.size;
    z.out = plain->bytes;
    z.out_capacity = plain->capacity;
    for (block = 0; (block < BLOCK_LIMIT) && (last == 0U); ++block) {
        last = take_bits(&z, FINAL_BITS);
        if (!inflate_block(&z, take_bits(&z, TYPE_BITS))) {
            return false;
        }
    }
    plain->size = z.out_pos;
    return (last != 0U) && !z.failed;
}
