#ifndef JSON_READER_H
#define JSON_READER_H

#include <stdbool.h>
#include <stddef.h>

#include "../core/text.h"

enum { JSON_INPUT_CAPACITY = 524288, JSON_TOKEN_CAPACITY = 32768, JSON_READER_DEPTH = 32, JSON_NONE = -1 };

typedef enum {
    JSON_KIND_OBJECT,
    JSON_KIND_ARRAY,
    JSON_KIND_STRING,
    JSON_KIND_NUMBER,
    JSON_KIND_TRUE,
    JSON_KIND_FALSE,
    JSON_KIND_NULL
} JsonKind;

typedef struct {
    JsonKind kind;
    int start;
    int end;
    int after;
} JsonToken;

typedef struct {
    char text[JSON_INPUT_CAPACITY];
    int length;
    JsonToken tokens[JSON_TOKEN_CAPACITY];
    int count;
} JsonDocument;

bool json_parse(JsonDocument *document);
bool json_is_kind(const JsonDocument *document, int token, JsonKind kind);
int json_member(const JsonDocument *document, int object, const char *key);
int json_first_child(const JsonDocument *document, int container);
int json_next_sibling(const JsonDocument *document, int container, int child);
bool json_copy_string(const JsonDocument *document, int token, char *destination, size_t capacity);
TextNumber json_number(const JsonDocument *document, int token);
TextDecimal json_decimal(const JsonDocument *document, int token);

#endif
