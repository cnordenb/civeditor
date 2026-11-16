#include "deflate.h"

#include <stdint.h>

#include "../core/check.h"
#include "bit_writer.h"

enum {
    MIN_MATCH = 3,
    MAX_MATCH = 258,
    FAR_SHORT_MATCH = 4096,
    WINDOW_SIZE = 32768,
    WINDOW_MASK = WINDOW_SIZE - 1,
    HASH_SIZE = 65536,
    HASH_SHIFT = 16,
    CHAIN_LIMIT = 64,
    PLAIN_LIMIT = 2147483647,
    LITERAL_SYMBOLS = 288,
    END_OF_BLOCK = 256,
    FIRST_LENGTH = 257,
    FIXED_RANGES = 4,
    CODE_BITS_LIMIT = 9,
    DISTANCE_CODE_BITS = 5,
    FINAL_FIXED_BLOCK = 3,
    BLOCK_HEADER_BITS = 3,
    SECOND_BYTE_SHIFT = 8,
    THIRD_BYTE_SHIFT = 16,
    NO_POSITION = -1
};

typedef struct {
    int first;
    unsigned int start;
    int length;
} CodeRange;

typedef struct {
    unsigned int bits;
    int length;
} Code;

typedef struct {
    int length;
    int distance;
} Match;

typedef struct {
    const unsigned char *bytes;
    int size;
} Plain;

static const CodeRange FIXED_CODES[FIXED_RANGES] = {
    { 0, 0x30U, 8 }, { 144, 0x190U, 9 }, { 256, 0x00U, 7 }, { 280, 0xC0U, 8 }
};

static const uint32_t HASH_MULTIPLIER = 2654435761U;

static Code literal_codes[LITERAL_SYMBOLS];
static Code distance_codes[DEFLATE_DISTANCE_SYMBOLS];
static int length_symbols[MAX_MATCH + 1];
static int distance_symbols[WINDOW_SIZE];
static int heads[HASH_SIZE];
static int previous[WINDOW_SIZE];

static unsigned int reversed(unsigned int code, int length)
{
    unsigned int turned = 0U;
    int bit;

    if (!CHECK(length > 0) || !CHECK(length <= CODE_BITS_LIMIT)) {
        return 0U;
    }
    for (bit = 0; (bit < CODE_BITS_LIMIT) && (bit < length); ++bit) {
        turned = (turned << 1U) | ((code >> (unsigned int)bit) & 1U);
    }
    return turned;
}

static Code fixed_code(int symbol)
{
    Code code = { 0U, 0 };
    int range = 0;
    int step;

    if (!CHECK(symbol >= 0) || !CHECK(symbol < LITERAL_SYMBOLS)) {
        return code;
    }
    for (step = 1; step < FIXED_RANGES; ++step) {
        range = (symbol >= FIXED_CODES[step].first) ? step : range;
    }
    code.length = FIXED_CODES[range].length;
    code.bits = reversed(FIXED_CODES[range].start + (unsigned int)(symbol - FIXED_CODES[range].first), code.length);
    return code;
}

static void fill_symbols(int *symbols, const int *bases, int base_count, int first, int limit)
{
    int index = 0;
    int value;

    if (!CHECK(symbols != NULL) || !CHECK(bases != NULL)) {
        return;
    }
    for (value = first; (value < limit) && (value < WINDOW_SIZE); ++value) {
        index += ((index + 1 < base_count) && (bases[index + 1] <= value)) ? 1 : 0;
        symbols[value] = index;
    }
}

static bool make_codes(void)
{
    int symbol;

    if (!CHECK(DEFLATE_LENGTH_BASE[0] == MIN_MATCH) || !CHECK(DEFLATE_DISTANCE_BASE[0] == 1)) {
        return false;
    }
    for (symbol = 0; symbol < LITERAL_SYMBOLS; ++symbol) {
        literal_codes[symbol] = fixed_code(symbol);
    }
    for (symbol = 0; symbol < DEFLATE_DISTANCE_SYMBOLS; ++symbol) {
        distance_codes[symbol].bits = reversed((unsigned int)symbol, DISTANCE_CODE_BITS);
        distance_codes[symbol].length = DISTANCE_CODE_BITS;
    }
    fill_symbols(length_symbols, DEFLATE_LENGTH_BASE, DEFLATE_LENGTH_SYMBOLS, MIN_MATCH, MAX_MATCH + 1);
    fill_symbols(distance_symbols, DEFLATE_DISTANCE_BASE, DEFLATE_DISTANCE_SYMBOLS, 1, WINDOW_SIZE);
    return true;
}

static int hash_at(Plain plain, int position)
{
    uint32_t key;

    if (!CHECK(position >= 0) || !CHECK(position + MIN_MATCH <= plain.size)) {
        return 0;
    }
    key = (uint32_t)plain.bytes[position] | ((uint32_t)plain.bytes[position + 1] << SECOND_BYTE_SHIFT) |
          ((uint32_t)plain.bytes[position + 2] << THIRD_BYTE_SHIFT);
    return (int)((key * HASH_MULTIPLIER) >> HASH_SHIFT);
}

static void remember(Plain plain, int position)
{
    int hash;

    if (!CHECK(position >= 0) || (position + MIN_MATCH > plain.size)) {
        return;
    }
    hash = hash_at(plain, position);
    previous[position & WINDOW_MASK] = heads[hash];
    heads[hash] = position;
}

static int match_length(Plain plain, int candidate, int position, int most)
{
    int length;

    if (!CHECK((candidate >= 0) && (candidate < position)) || !CHECK(position + most <= plain.size)) {
        return 0;
    }
    for (length = 0; (length < MAX_MATCH) && (length < most); ++length) {
        if (plain.bytes[candidate + length] != plain.bytes[position + length]) {
            return length;
        }
    }
    return length;
}

static Match better_of(Match best, int length, int distance)
{
    if (length > best.length) {
        best.length = length;
        best.distance = distance;
    }
    return best;
}

static Match search_chain(Plain plain, int position, int most)
{
    Match best = { 0, 0 };
    int candidate = heads[hash_at(plain, position)];
    int chain;

    if (!CHECK(position >= 0) || !CHECK(most >= MIN_MATCH)) {
        return best;
    }
    for (chain = 0; (chain < CHAIN_LIMIT) && (candidate > NO_POSITION) && (position - candidate < WINDOW_SIZE) &&
                    (best.length < most); ++chain) {
        if (plain.bytes[candidate + best.length] == plain.bytes[position + best.length]) {
            best = better_of(best, match_length(plain, candidate, position, most), position - candidate);
        }
        candidate = previous[candidate & WINDOW_MASK];
    }
    return best;
}

static bool is_too_short(Match match)
{
    return (match.length < MIN_MATCH) || ((match.length == MIN_MATCH) && (match.distance > FAR_SHORT_MATCH));
}

static Match longest_match(Plain plain, int position)
{
    Match best = { 0, 0 };
    int most = plain.size - position;

    if (!CHECK(position >= 0) || !CHECK(position < plain.size) || (most < MIN_MATCH)) {
        return best;
    }
    best = search_chain(plain, position, (most > MAX_MATCH) ? MAX_MATCH : most);
    if (is_too_short(best)) {
        best.length = 0;
    }
    return best;
}

static void put_literal(BitWriter *writer, int value)
{
    if (!CHECK(writer != NULL) || !CHECK((value >= 0) && (value < LITERAL_SYMBOLS))) {
        return;
    }
    bit_writer_put(writer, literal_codes[value].bits, literal_codes[value].length);
}

static void put_match(BitWriter *writer, Match match)
{
    int length_index;
    int distance_index;

    if (!CHECK((match.length >= MIN_MATCH) && (match.length <= MAX_MATCH)) ||
        !CHECK((match.distance > 0) && (match.distance < WINDOW_SIZE))) {
        return;
    }
    length_index = length_symbols[match.length];
    distance_index = distance_symbols[match.distance];
    put_literal(writer, FIRST_LENGTH + length_index);
    bit_writer_put(writer, (unsigned int)(match.length - DEFLATE_LENGTH_BASE[length_index]),
                   DEFLATE_LENGTH_EXTRA[length_index]);
    bit_writer_put(writer, distance_codes[distance_index].bits, distance_codes[distance_index].length);
    bit_writer_put(writer, (unsigned int)(match.distance - DEFLATE_DISTANCE_BASE[distance_index]),
                   DEFLATE_DISTANCE_EXTRA[distance_index]);
}

static void pack_symbols(Plain plain, BitWriter *writer)
{
    Match match;
    int position = 0;
    int covered;
    int step;

    if (!CHECK(writer != NULL) || !CHECK(plain.size >= 0)) {
        return;
    }
    for (step = 0; (step < PLAIN_LIMIT) && (position < plain.size); ++step) {
        match = longest_match(plain, position);
        if (match.length == 0) {
            put_literal(writer, plain.bytes[position]);
            remember(plain, position);
            ++position;
            continue;
        }
        put_match(writer, match);
        for (covered = 0; (covered < MAX_MATCH) && (covered < match.length); ++covered) {
            remember(plain, position + covered);
        }
        position += match.length;
    }
}

bool deflate_pack(ByteSpan span, ByteBuffer *packed)
{
    BitWriter writer;
    Plain plain;
    int index;

    if (!CHECK(packed != NULL) || !CHECK(packed->bytes != NULL) || !CHECK((span.bytes != NULL) || (span.size == 0U)) ||
        (span.size >= (size_t)PLAIN_LIMIT) || !make_codes()) {
        return false;
    }
    for (index = 0; index < HASH_SIZE; ++index) {
        heads[index] = NO_POSITION;
    }
    plain.bytes = span.bytes;
    plain.size = (int)span.size;
    bit_writer_start(&writer, packed->bytes, packed->capacity);
    bit_writer_put(&writer, (unsigned int)FINAL_FIXED_BLOCK, BLOCK_HEADER_BITS);
    pack_symbols(plain, &writer);
    put_literal(&writer, END_OF_BLOCK);
    if (!bit_writer_finish(&writer)) {
        return false;
    }
    packed->size = writer.size;
    return true;
}
