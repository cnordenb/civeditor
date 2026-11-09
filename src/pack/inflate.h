#ifndef INFLATE_H
#define INFLATE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    const unsigned char *bytes;
    size_t size;
} ByteSpan;

typedef struct {
    unsigned char *bytes;
    size_t capacity;
    size_t size;
} ByteBuffer;

enum { DEFLATE_LENGTH_SYMBOLS = 29, DEFLATE_DISTANCE_SYMBOLS = 30 };

extern const int DEFLATE_LENGTH_BASE[DEFLATE_LENGTH_SYMBOLS];
extern const int DEFLATE_LENGTH_EXTRA[DEFLATE_LENGTH_SYMBOLS];
extern const int DEFLATE_DISTANCE_BASE[DEFLATE_DISTANCE_SYMBOLS];
extern const int DEFLATE_DISTANCE_EXTRA[DEFLATE_DISTANCE_SYMBOLS];

bool inflate_raw(ByteSpan packed, ByteBuffer *plain);

#endif
