#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

enum { TEXT_SCAN_LIMIT = 1048576, TEXT_NUMBER_CAPACITY = 24, TEXT_PLACES_LIMIT = 9 };

typedef struct {
    bool valid;
    long value;
} TextNumber;

typedef struct {
    bool valid;
    double value;
} TextDecimal;

size_t text_length(const char *text, size_t capacity);
bool text_copy(char *destination, size_t capacity, const char *source);
bool text_copy_span(char *destination, size_t capacity, const char *source, size_t length);
bool text_append(char *destination, size_t capacity, const char *source);
bool text_append_number(char *destination, size_t capacity, long value);
bool text_append_decimal(char *destination, size_t capacity, long value, unsigned places);
bool text_append_two(char *destination, size_t capacity, const char *first, const char *second);
bool text_append_three(char *destination, size_t capacity, const char *first, const char *second, const char *third);
bool text_join(char *destination, size_t capacity, const char *first, const char *second);
bool text_is_between(char letter, char first, char last);
bool text_is_digit(char letter);
bool text_is_sign(char letter);
bool text_is_line_break(char letter);
bool text_equal(const char *left, const char *right);
bool text_starts_with(const char *text, const char *prefix);
bool text_starts_folded(const char *text, const char *prefix);
bool text_contains_folded(const char *text, const char *needle);
TextNumber text_to_number(const char *text, size_t length);
TextDecimal text_to_decimal(const char *text, size_t length);
bool text_widen(const char *text, wchar_t *wide, size_t capacity);

#endif
