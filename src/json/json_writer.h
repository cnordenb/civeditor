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
void json_begin_object(JsonWriter *writer, const char *key);
void json_end_object(JsonWriter *writer);
void json_begin_array(JsonWriter *writer, const char *key);
void json_end_array(JsonWriter *writer);
void json_write_string(JsonWriter *writer, const char *key, const char *value);
void json_write_number(JsonWriter *writer, const char *key, long value);
void json_write_bool(JsonWriter *writer, const char *key, bool value);
void json_write_null(JsonWriter *writer, const char *key);
bool json_writer_succeeded(const JsonWriter *writer);

#endif
