#include "pack_tests.h"

#include <string.h>

#include "../../src/deflate/deflate.h"
#include "../../src/pack/deflate_store.h"

enum {
    DYNAMIC_SIZE = 91,
    FIXED_SIZE = 7,
    TEXT_CAPACITY = 256,
    STORED_SIZE = 140000,
    PATTERN_PERIOD = 251,
    PACKED_CAPACITY = STORED_SIZE * 2
};

static const unsigned char DYNAMIC_STREAM[DYNAMIC_SIZE] = {
    0x5d, 0x8c, 0x41, 0x0a, 0x80, 0x20, 0x10, 0x45, 0xaf, 0xf2, 0x37, 0xad, 0x22, 0x09, 0xa2, 0x03,
    0x84, 0x57, 0x08, 0xf7, 0x52, 0xa3, 0x09, 0xe6, 0xc0, 0x28, 0x48, 0xb7, 0xcf, 0x5c, 0xb6, 0xfc,
    0xff, 0x3d, 0x9e, 0x09, 0x31, 0x5a, 0x4f, 0x92, 0x71, 0x58, 0x91, 0x07, 0xe3, 0x02, 0xc7, 0x7c,
    0x2a, 0xec, 0x5c, 0x13, 0x34, 0xa5, 0xd2, 0x19, 0xe7, 0x82, 0x69, 0x9d, 0x07, 0xd4, 0x0e, 0x37,
    0x39, 0xae, 0xef, 0x77, 0x41, 0x08, 0xce, 0xe6, 0x26, 0x21, 0x24, 0x94, 0x8b, 0xa0, 0xdb, 0x8a,
    0x84, 0xcd, 0x93, 0x82, 0xf9, 0xc5, 0x6f, 0x6e, 0x7a, 0x2f, 0xbc
};

static const char DYNAMIC_TEXT[] = "Villagers carry +3 food. Town Centers cost -50% wood. Archers fire faster in the "
                                   "Castle Age. Villagers carry more wood.";

static const unsigned char FIXED_STREAM[FIXED_SIZE] = { 0xcb, 0x48, 0xcd, 0xc9, 0xc9, 0x07, 0x00 };

static unsigned char text[TEXT_CAPACITY];
static unsigned char original[STORED_SIZE];
static unsigned char packed[PACKED_CAPACITY];
static unsigned char unpacked[STORED_SIZE];

static bool inflates_to(ByteSpan stream, const char *expected)
{
    ByteBuffer plain = { text, sizeof text, 0U };
    size_t length = strlen(expected);

    return inflate_raw(stream, &plain) && (plain.size == length) && (memcmp(text, expected, length) == 0);
}

bool test_a_dynamic_huffman_stream_inflates(void)
{
    ByteSpan stream = { DYNAMIC_STREAM, sizeof DYNAMIC_STREAM };

    return inflates_to(stream, DYNAMIC_TEXT);
}

bool test_a_fixed_huffman_stream_inflates(void)
{
    ByteSpan stream = { FIXED_STREAM, sizeof FIXED_STREAM };

    return inflates_to(stream, "hello");
}

bool test_a_stored_stream_inflates_back(void)
{
    ByteSpan plain = { original, sizeof original };
    ByteBuffer stored = { packed, sizeof packed, 0U };
    ByteBuffer back = { unpacked, sizeof unpacked, 0U };
    ByteSpan stream;
    int index;

    for (index = 0; index < STORED_SIZE; ++index) {
        original[index] = (unsigned char)(index % PATTERN_PERIOD);
    }
    if (!deflate_store(plain, &stored)) {
        return false;
    }
    stream.bytes = packed;
    stream.size = stored.size;
    return inflate_raw(stream, &back) && (back.size == sizeof original) &&
           (memcmp(unpacked, original, sizeof original) == 0);
}

enum {
    MIXED_SEED = 20261109,
    LCG_MULTIPLIER = 1103515245,
    LCG_INCREMENT = 12345,
    SEGMENT_SPAN = 300,
    SEGMENT_SHIFT = 8,
    DISTANCE_SPAN = 32767,
    DISTANCE_SHIFT = 12,
    COPY_SHIFT = 30,
    BYTE_SHIFT = 16,
    SMALL_FRACTION = 10
};

static unsigned int next_seed(unsigned int seed)
{
    return (seed * (unsigned int)LCG_MULTIPLIER) + (unsigned int)LCG_INCREMENT;
}

static void make_mixed_data(void)
{
    unsigned int seed = (unsigned int)MIXED_SEED;
    int index = 0;
    int segment;
    int step;
    int length;
    int distance;
    bool copied;

    for (segment = 0; (segment < STORED_SIZE) && (index < STORED_SIZE); ++segment) {
        seed = next_seed(seed);
        length = 1 + (int)((seed >> SEGMENT_SHIFT) % (unsigned int)SEGMENT_SPAN);
        distance = 1 + (int)((seed >> DISTANCE_SHIFT) % (unsigned int)DISTANCE_SPAN);
        copied = (((seed >> COPY_SHIFT) & 1U) != 0U) && (distance <= index);
        for (step = 0; (step < SEGMENT_SPAN) && (step < length) && (index < STORED_SIZE); ++step) {
            seed = next_seed(seed);
            original[index] = copied ? original[index - distance] : (unsigned char)(seed >> BYTE_SHIFT);
            ++index;
        }
    }
}

bool test_packed_data_unpacks_to_the_same_bytes(void)
{
    ByteSpan plain = { original, sizeof original };
    ByteBuffer squeezed = { packed, sizeof packed, 0U };
    ByteBuffer back = { unpacked, sizeof unpacked, 0U };
    ByteSpan stream;

    make_mixed_data();
    if (!deflate_pack(plain, &squeezed)) {
        return false;
    }
    stream.bytes = packed;
    stream.size = squeezed.size;
    return inflate_raw(stream, &back) && (back.size == sizeof original) &&
           (memcmp(unpacked, original, sizeof original) == 0);
}

bool test_repeated_data_packs_small(void)
{
    ByteSpan plain = { original, sizeof original };
    ByteBuffer squeezed = { packed, sizeof packed, 0U };
    int index;

    for (index = 0; index < STORED_SIZE; ++index) {
        original[index] = (unsigned char)(index % PATTERN_PERIOD);
    }
    return deflate_pack(plain, &squeezed) && (squeezed.size < sizeof original / SMALL_FRACTION);
}
