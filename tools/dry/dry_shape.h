#ifndef DRY_SHAPE_H
#define DRY_SHAPE_H

#include <stdbool.h>
#include <stdint.h>

enum { DRY_PRINT_CAPACITY = 640, DRY_MILLI = 1000, DRY_SHINGLE = 4, DRY_MIN_TOKENS = 20 };

typedef struct {
    uint32_t prints[DRY_PRINT_CAPACITY];
    int count;
    int tokens;
} DryShape;

bool dry_shape_of(DryShape *shape, const char *text, int start, int end);
int dry_alike_milli(const DryShape *left, const DryShape *right);

#endif
