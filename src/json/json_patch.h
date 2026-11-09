#ifndef JSON_PATCH_H
#define JSON_PATCH_H

#include <stdbool.h>
#include <stddef.h>

#include "json_reader.h"

enum {
    JSON_PATCH_EDIT_CAPACITY = 1024,
    JSON_PATCH_VALUE_CAPACITY = 256,
    JSON_PATCH_OUTPUT_CAPACITY = JSON_INPUT_CAPACITY + 8192
};

typedef struct {
    int start;
    int end;
    char value[JSON_PATCH_VALUE_CAPACITY];
} JsonEdit;

typedef struct {
    const JsonDocument *document;
    JsonEdit edits[JSON_PATCH_EDIT_CAPACITY];
    int count;
} JsonPatch;

typedef struct {
    char text[JSON_PATCH_OUTPUT_CAPACITY];
    size_t length;
} JsonPatchOutput;

void json_patch_start(JsonPatch *patch, const JsonDocument *document);
void json_patch_replace(JsonPatch *patch, int token, const char *value);
bool json_patch_write(JsonPatch *patch, JsonPatchOutput *output);

#endif
