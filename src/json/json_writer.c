#include "json_writer.h"

#include "../core/check.h"
#include "../core/text.h"

enum { HEX_DIGITS = 16, CONTROL_LIMIT = 32, INDENT_WIDTH = 2, NIBBLE = 4, NIBBLE_MASK = 15 };

static const char HEX[HEX_DIGITS + 1] = "0123456789abcdef";

static void put_char(JsonWriter *writer, char letter)
{
    if (!CHECK(writer != NULL) || writer->failed) {
        return;
    }
    if (!CHECK(writer->length + 1U < (size_t)JSON_OUTPUT_CAPACITY)) {
        writer->failed = true;
        return;
    }
    writer->data[writer->length] = letter;
    ++writer->length;
    writer->data[writer->length] = '\0';
}

static void put_text(JsonWriter *writer, const char *text)
{
    size_t index;

    if (!CHECK(writer != NULL) || !CHECK(text != NULL)) {
        return;
    }
    for (index = 0U; (index < (size_t)TEXT_SCAN_LIMIT) && (text[index] != '\0'); ++index) {
        put_char(writer, text[index]);
    }
}

static void put_escaped_char(JsonWriter *writer, char letter)
{
    unsigned int code = (unsigned int)(unsigned char)letter;

    if (!CHECK(writer != NULL) || !CHECK(code < 256U)) {
        return;
    }
    if ((letter == '"') || (letter == '\\')) {
        put_char(writer, '\\');
        put_char(writer, letter);
    } else if (letter == '\n') {
        put_text(writer, "\\n");
    } else if (letter == '\t') {
        put_text(writer, "\\t");
    } else if (code < (unsigned int)CONTROL_LIMIT) {
        put_text(writer, "\\u00");
        put_char(writer, HEX[(code >> NIBBLE) & (unsigned int)NIBBLE_MASK]);
        put_char(writer, HEX[code & (unsigned int)NIBBLE_MASK]);
    } else {
        put_char(writer, letter);
    }
}

static void put_string(JsonWriter *writer, const char *value)
{
    size_t index;

    if (!CHECK(writer != NULL) || !CHECK(value != NULL)) {
        return;
    }
    put_char(writer, '"');
    for (index = 0U; (index < (size_t)TEXT_SCAN_LIMIT) && (value[index] != '\0'); ++index) {
        put_escaped_char(writer, value[index]);
    }
    put_char(writer, '"');
}

static void put_indent(JsonWriter *writer)
{
    int column;

    if (!CHECK(writer != NULL) || !CHECK(writer->depth <= JSON_WRITER_DEPTH)) {
        return;
    }
    put_char(writer, '\n');
    for (column = 0; (column < writer->depth * INDENT_WIDTH) && (column < JSON_WRITER_DEPTH * INDENT_WIDTH);
         ++column) {
        put_char(writer, ' ');
    }
}

static void start_item(JsonWriter *writer, const char *key)
{
    if (!CHECK(writer != NULL) || !CHECK((writer->depth > 0) && (writer->depth <= JSON_WRITER_DEPTH))) {
        if (writer != NULL) {
            writer->failed = true;
        }
        return;
    }
    if (writer->has_items[writer->depth - 1]) {
        put_char(writer, ',');
    }
    writer->has_items[writer->depth - 1] = true;
    put_indent(writer);
    if (key != NULL) {
        put_string(writer, key);
        put_text(writer, ": ");
    }
}

void json_begin(JsonWriter *writer, const char *key, char bracket)
{
    if (!CHECK(writer != NULL) || !CHECK((bracket == '{') || (bracket == '['))) {
        return;
    }
    if (writer->depth > 0) {
        start_item(writer, key);
    }
    put_char(writer, bracket);
    if (!CHECK(writer->depth < JSON_WRITER_DEPTH)) {
        writer->failed = true;
        return;
    }
    writer->has_items[writer->depth] = false;
    ++writer->depth;
}

void json_end(JsonWriter *writer, char bracket)
{
    if (!CHECK(writer != NULL) || !CHECK(writer->depth > 0)) {
        if (writer != NULL) {
            writer->failed = true;
        }
        return;
    }
    --writer->depth;
    if (writer->has_items[writer->depth]) {
        put_indent(writer);
    }
    put_char(writer, bracket);
}

void json_writer_start(JsonWriter *writer)
{
    if (!CHECK(writer != NULL) || !CHECK(JSON_OUTPUT_CAPACITY > 0)) {
        return;
    }
    writer->length = 0U;
    writer->depth = 0;
    writer->failed = false;
    writer->data[0] = '\0';
}

void json_write_string(JsonWriter *writer, const char *key, const char *value)
{
    if (!CHECK(writer != NULL) || !CHECK(value != NULL)) {
        return;
    }
    start_item(writer, key);
    put_string(writer, value);
}

void json_write_number(JsonWriter *writer, const char *key, long value)
{
    char digits[TEXT_NUMBER_CAPACITY] = "";

    if (!CHECK(writer != NULL) || !CHECK(writer->depth > 0) || !text_append_number(digits, sizeof digits, value)) {
        return;
    }
    start_item(writer, key);
    put_text(writer, digits);
}

void json_write_bool(JsonWriter *writer, const char *key, bool value)
{
    if (!CHECK(writer != NULL) || !CHECK(writer->depth > 0)) {
        return;
    }
    start_item(writer, key);
    put_text(writer, value ? "true" : "false");
}

void json_write_null(JsonWriter *writer, const char *key)
{
    if (!CHECK(writer != NULL) || !CHECK(writer->depth > 0)) {
        return;
    }
    start_item(writer, key);
    put_text(writer, "null");
}

bool json_writer_succeeded(const JsonWriter *writer)
{
    if (!CHECK(writer != NULL) || !CHECK(writer->length < (size_t)JSON_OUTPUT_CAPACITY)) {
        return false;
    }
    return !writer->failed && (writer->depth == 0);
}
