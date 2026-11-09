#ifndef BIT_WRITER_H
#define BIT_WRITER_H

#include <stdbool.h>
#include <stddef.h>

enum { BIT_WRITER_PUT_LIMIT = 24 };

typedef struct {
    unsigned char *bytes;
    size_t capacity;
    size_t size;
    unsigned long long waiting;
    int waiting_count;
    bool full;
} BitWriter;

void bit_writer_start(BitWriter *writer, unsigned char *bytes, size_t capacity);
void bit_writer_put(BitWriter *writer, unsigned int value, int count);
bool bit_writer_finish(BitWriter *writer);

#endif
