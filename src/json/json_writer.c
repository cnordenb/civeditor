#include "json_writer.h"

#include "../core/check.h"
#include "../core/text.h"

enum { HEX_DIGITS = 16, CONTROL_LIMIT = 32, INDENT_WIDTH = 2, NIBBLE = 4, NIBBLE_MASK = 15 };

static const char HEX[HEX_DIGITS + 1] = "0123456789abcdef";

static void put_char(JsonWriter *writer, char letter)
{
    if (writer->failed) {
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

static void put_escape(JsonWriter *writer, char letter)
{
    put_char(writer, '\\');
    put_char(writer, letter);
}

static void put_escaped_char(JsonWriter *writer, char letter)
{
    unsigned int code = (unsigned int)(unsigned char)letter;

    if ((letter == '"') || (letter == '\\')) {
        put_escape(writer, letter);
    } else if (letter == '\n') {
        put_escape(writer, 'n');
    } else if (letter == '\t') {
        put_escape(writer, 't');
    } else if (code < (unsigned int)CONTROL_LIMIT) {
        put_escape(writer, 'u');
        put_char(writer, '0');
        put_char(writer, '0');
        put_char(writer, HEX[(code >> NIBBLE) & (unsigned int)NIBBLE_MASK]);
        put_char(writer, HEX[code & (unsigned int)NIBBLE_MASK]);
    } else {
        put_char(writer, letter);
    }
}

static void put_letters(JsonWriter *writer, const char *text, bool escaped)
{
    size_t index;

    for (index = 0U; (index < (size_t)TEXT_SCAN_LIMIT) && (text[index] != '\0'); ++index) {
        if (escaped) {
            put_escaped_char(writer, text[index]);
        } else {
            put_char(writer, text[index]);
        }
    }
}

static void put_string(JsonWriter *writer, const char *value)
{
    put_char(writer, '"');
    put_letters(writer, value, true);
    put_char(writer, '"');
}

static void put_indent(JsonWriter *writer)
{
    int column;

    put_char(writer, '\n');
    for (column = 0; column < writer->depth * INDENT_WIDTH; ++column) {
        put_char(writer, ' ');
    }
}

static void start_item(JsonWriter *writer, const char *key)
{
    if (writer->has_items[writer->depth - 1]) {
        put_char(writer, ',');
    }
    writer->has_items[writer->depth - 1] = true;
    put_indent(writer);
    if (key != NULL) {
        put_string(writer, key);
        put_letters(writer, ": ", false);
    }
}

void json_begin(JsonWriter *writer, const char *key, char bracket)
{
    if (writer->depth > 0) {
        start_item(writer, key);
    }
    put_char(writer, bracket);
    writer->has_items[writer->depth] = false;
    ++writer->depth;
}

void json_end(JsonWriter *writer, char bracket)
{
    --writer->depth;
    if (writer->has_items[writer->depth]) {
        put_indent(writer);
    }
    put_char(writer, bracket);
}

void json_writer_start(JsonWriter *writer)
{
    writer->length = 0U;
    writer->depth = 0;
    writer->failed = false;
    writer->data[0] = '\0';
}

void json_write_string(JsonWriter *writer, const char *key, const char *value)
{
    start_item(writer, key);
    put_string(writer, value);
}

void json_write_decimal(JsonWriter *writer, const char *key, long value, unsigned places)
{
    char digits[TEXT_NUMBER_CAPACITY] = "";

    (void)text_append_decimal(digits, sizeof digits, value, places);
    start_item(writer, key);
    put_letters(writer, digits, false);
}

void json_write_number(JsonWriter *writer, const char *key, long value)
{
    json_write_decimal(writer, key, value, 0U);
}

void json_write_bool(JsonWriter *writer, const char *key, bool value)
{
    start_item(writer, key);
    put_letters(writer, value ? "true" : "false", false);
}

void json_write_null(JsonWriter *writer, const char *key)
{
    start_item(writer, key);
    put_letters(writer, "null", false);
}

bool json_writer_succeeded(const JsonWriter *writer)
{
    return !writer->failed && (writer->depth == 0);
}
