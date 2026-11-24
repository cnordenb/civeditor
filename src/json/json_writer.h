#ifndef JSON_WRITER_H
#define JSON_WRITER_H

#include <stdbool.h>
#include <stddef.h>

enum { JSON_OUTPUT_CAPACITY = 131072, JSON_WRITER_DEPTH = 16 };

typedef struct {
    char data[JSON_OUTPUT_CAPACITY];
    size_t length;
    int depth;
    bool has_items[JSON_WRITER_DEPTH];
    bool failed;
} JsonWriter;

void json_writer_start(JsonWriter *writer);
void json_begin(JsonWriter *writer, const char *key, char bracket);
void json_end(JsonWriter *writer, char bracket);
void json_write_string(JsonWriter *writer, const char *key, const char *value);
void json_write_number(JsonWriter *writer, const char *key, long value);
void json_write_decimal(JsonWriter *writer, const char *key, long value, unsigned places);
void json_write_bool(JsonWriter *writer, const char *key, bool value);
void json_write_null(JsonWriter *writer, const char *key);
bool json_writer_succeeded(const JsonWriter *writer);

#endif
