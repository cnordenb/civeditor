#ifndef TSV_H
#define TSV_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <wchar.h>

enum {
    TSV_FILE_CAPACITY = 1048576,
    TSV_LINE_CAPACITY = 1024,
    TSV_FIELD_LIMIT = 24,
    TSV_LIST_LIMIT = 16,
    TSV_MISSING = -1,
    TSV_BIT_LIMIT = 64
};

typedef struct {
    char data[TSV_FILE_CAPACITY];
    size_t length;
    size_t position;
} TsvFile;

typedef struct {
    char text[TSV_LINE_CAPACITY];
    int starts[TSV_FIELD_LIMIT];
    int lengths[TSV_FIELD_LIMIT];
    int field_count;
} TsvRecord;

typedef struct {
    int values[TSV_LIST_LIMIT];
    int count;
} TsvNumberList;

bool tsv_open(TsvFile *file, const wchar_t *path);
bool tsv_next(TsvFile *file, TsvRecord *record);
bool tsv_text(const TsvRecord *record, int field, char *destination, size_t capacity);
int tsv_number(const TsvRecord *record, int field);
int tsv_hundredths(const TsvRecord *record, int field);
bool tsv_decimal(const TsvRecord *record, int field, float *value);
TsvNumberList tsv_number_list(const TsvRecord *record, int field);
uint64_t tsv_bits(const TsvRecord *record, int field);

#endif
