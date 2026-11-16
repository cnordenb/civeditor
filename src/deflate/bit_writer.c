#include "bit_writer.h"

#include "../core/check.h"

enum {
    BYTE_BITS = 8,
    BYTE_MASK = 0xFF,
    FLUSH_LIMIT = 4
};

void bit_writer_start(BitWriter *writer, unsigned char *bytes, size_t capacity)
{
    const BitWriter empty = { bytes, capacity, 0U, 0U, 0, false };

    if (!CHECK(writer != NULL) || !CHECK(bytes != NULL)) {
        return;
    }
    *writer = empty;
}

static void flush_bytes(BitWriter *writer)
{
    int step;

    if (!CHECK(writer != NULL) || !CHECK(writer->waiting_count >= 0)) {
        return;
    }
    for (step = 0; (step < FLUSH_LIMIT) && (writer->waiting_count >= BYTE_BITS); ++step) {
        if (writer->size < writer->capacity) {
            writer->bytes[writer->size] = (unsigned char)(writer->waiting & (unsigned long long)BYTE_MASK);
            ++writer->size;
        } else {
            writer->full = true;
        }
        writer->waiting >>= BYTE_BITS;
        writer->waiting_count -= BYTE_BITS;
    }
}

void bit_writer_put(BitWriter *writer, unsigned int value, int count)
{
    unsigned long long kept;

    if (!CHECK(writer != NULL) || !CHECK((count >= 0) && (count <= BIT_WRITER_PUT_LIMIT))) {
        return;
    }
    kept = (unsigned long long)value & ((1ULL << (unsigned int)count) - 1ULL);
    writer->waiting |= kept << (unsigned int)writer->waiting_count;
    writer->waiting_count += count;
    flush_bytes(writer);
}

bool bit_writer_finish(BitWriter *writer)
{
    if (!CHECK(writer != NULL) || !CHECK(writer->waiting_count < BYTE_BITS)) {
        return false;
    }
    if (writer->waiting_count > 0) {
        bit_writer_put(writer, 0U, BYTE_BITS - writer->waiting_count);
    }
    return !writer->full;
}
