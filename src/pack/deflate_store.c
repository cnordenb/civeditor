#include "deflate_store.h"

#include <string.h>

#include "../core/check.h"

enum {
    BLOCK_BYTES = 65535,
    HEADER_BYTES = 5,
    LOW_BYTE = 0xFF,
    BYTE_BITS = 8,
    FINAL_BLOCK = 1,
    BLOCK_LIMIT = 16777216
};

size_t deflate_store_size(size_t plain_size)
{
    size_t blocks = (plain_size / (size_t)BLOCK_BYTES) + 1U;

    if (!CHECK(blocks > 0U) || !CHECK(plain_size / (size_t)BLOCK_BYTES < (size_t)BLOCK_LIMIT)) {
        return 0U;
    }
    return plain_size + (blocks * (size_t)HEADER_BYTES);
}

static void write_header(unsigned char *at, size_t length, bool last)
{
    size_t complement = ~length;

    if (!CHECK(at != NULL) || !CHECK(length <= (size_t)BLOCK_BYTES)) {
        return;
    }
    at[0] = (unsigned char)(last ? FINAL_BLOCK : 0);
    at[1] = (unsigned char)(length & (size_t)LOW_BYTE);
    at[2] = (unsigned char)((length >> BYTE_BITS) & (size_t)LOW_BYTE);
    at[3] = (unsigned char)(complement & (size_t)LOW_BYTE);
    at[4] = (unsigned char)((complement >> BYTE_BITS) & (size_t)LOW_BYTE);
}

static bool store_fits(ByteSpan plain, const ByteBuffer *packed)
{
    return CHECK(packed != NULL) && CHECK(packed->bytes != NULL) &&
           CHECK((plain.bytes != NULL) || (plain.size == 0U)) && (deflate_store_size(plain.size) <= packed->capacity);
}

bool deflate_store(ByteSpan plain, ByteBuffer *packed)
{
    size_t done = 0U;
    size_t length;
    bool last = false;
    int block;

    if (!store_fits(plain, packed)) {
        return false;
    }
    packed->size = 0U;
    for (block = 0; (block < BLOCK_LIMIT) && !last; ++block) {
        length = plain.size - done;
        length = (length > (size_t)BLOCK_BYTES) ? (size_t)BLOCK_BYTES : length;
        last = (done + length == plain.size);
        write_header(packed->bytes + packed->size, length, last);
        packed->size += (size_t)HEADER_BYTES;
        if (length > 0U) {
            (void)memcpy(packed->bytes + packed->size, plain.bytes + done, length);
        }
        packed->size += length;
        done += length;
    }
    return last;
}
