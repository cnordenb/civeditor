#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

enum { TEXT_SCAN_LIMIT = 1048576, TEXT_NUMBER_CAPACITY = 24 };

typedef struct {
    bool valid;
    long value;
} TextNumber;

size_t text_length(const char *text, size_t capacity);
bool text_copy(char *destination, size_t capacity, const char *source);
bool text_copy_span(char *destination, size_t capacity, const char *source, size_t length);
bool text_append(char *destination, size_t capacity, const char *source);
bool text_append_number(char *destination, size_t capacity, long value);
bool text_equal(const char *left, const char *right);
bool text_starts_with(const char *text, const char *prefix);
bool text_contains_folded(const char *text, const char *needle);
TextNumber text_to_number(const char *text, size_t length);
bool text_widen(const char *text, wchar_t *wide, size_t capacity);

#endif
