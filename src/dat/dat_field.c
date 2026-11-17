#include "dat_parts.h"

#include <stdint.h>
#include <string.h>

#include "../core/check.h"
#include "../core/text.h"

enum { BITS_PER_BYTE = 8, BYTE_HIGHEST = 255, SHORT_LOWEST = -32768, SHORT_HIGHEST = 32767 };

static const long SHORT_SIGN = 0x8000L;
static const long SHORT_RANGE = 0x10000L;
static const unsigned long LONG_SIGN = 0x80000000UL;
static const unsigned long LONG_MASK = 0xFFFFFFFFUL;
static const unsigned long SHORT_MASK = 0xFFFFUL;
static const unsigned long BYTE_MASK = 0xFFUL;

bool dat_cursor_start(DatCursor *cursor, const DatBytes *dat, size_t at)
{
    cursor->data = dat->data;
    cursor->size = dat->size;
    cursor->at = at;
    cursor->fine = CHECK(dat->data != NULL) && CHECK(at <= dat->size);
    return cursor->fine;
}

bool dat_cursor_skip(DatCursor *cursor, size_t count)
{
    if (!cursor->fine) {
        return false;
    }
    cursor->fine = CHECK(count <= cursor->size - cursor->at);
    cursor->at += cursor->fine ? count : 0U;
    return cursor->fine;
}

bool dat_cursor_skip_items(DatCursor *cursor, long count, size_t item_size)
{
    if (!cursor->fine) {
        return false;
    }
    cursor->fine = CHECK(count >= 0L) &&
                   CHECK((item_size == 0U) || ((size_t)count <= (cursor->size - cursor->at) / item_size));
    return dat_cursor_skip(cursor, cursor->fine ? (size_t)count * item_size : 0U);
}

static unsigned long cursor_take(DatCursor *cursor, size_t width)
{
    const unsigned char *data;
    unsigned long value = 0UL;
    size_t index;

    if (!dat_cursor_skip(cursor, width)) {
        return 0UL;
    }
    data = cursor->data;
    for (index = 0U; index < width; ++index) {
        value |= (unsigned long)data[cursor->at - width + index] << (BITS_PER_BYTE * index);
    }
    return value;
}

int dat_cursor_u8(DatCursor *cursor)
{
    return (int)cursor_take(cursor, DAT_BYTE_WIDTH);
}

int dat_cursor_i16(DatCursor *cursor)
{
    long value;

    value = (long)cursor_take(cursor, DAT_SHORT_WIDTH);
    return (int)((value >= SHORT_SIGN) ? value - SHORT_RANGE : value);
}

long dat_cursor_i32(DatCursor *cursor)
{
    unsigned long value;

    value = cursor_take(cursor, DAT_LONG_WIDTH);
    return (value >= LONG_SIGN) ? -(long)(LONG_MASK - value) - 1L : (long)value;
}

float dat_cursor_f32(DatCursor *cursor)
{
    uint32_t bits;
    float value = 0.0f;

    bits = (uint32_t)cursor_take(cursor, DAT_LONG_WIDTH);
    (void)memcpy(&value, &bits, sizeof value);
    return value;
}

static size_t cursor_text_length(DatCursor *cursor)
{
    unsigned long marker;
    unsigned long length;

    marker = cursor_take(cursor, DAT_SHORT_WIDTH);
    length = cursor_take(cursor, DAT_SHORT_WIDTH);
    cursor->fine = cursor->fine && CHECK(marker == DAT_TEXT_MARKER);
    return cursor->fine ? (size_t)length : 0U;
}

bool dat_cursor_skip_text(DatCursor *cursor)
{
    size_t length;

    length = cursor_text_length(cursor);
    return dat_cursor_skip(cursor, length);
}

bool dat_cursor_text(DatCursor *cursor, char *text, size_t capacity)
{
    const unsigned char *data;
    size_t length;

    text[0] = '\0';
    length = cursor_text_length(cursor);
    cursor->fine = cursor->fine && CHECK(length < capacity);
    if (!dat_cursor_skip(cursor, length)) {
        return false;
    }
    data = cursor->data;
    return text_copy_span(text, capacity, (const char *)&data[cursor->at - length], length);
}

DatSink dat_sink_over(unsigned char *bytes, size_t capacity, size_t length)
{
    DatSink sink;

    sink.bytes = bytes;
    sink.capacity = capacity;
    sink.length = length;
    sink.fine = CHECK(bytes != NULL) && CHECK(length <= capacity);
    return sink;
}

static bool sink_has_room(const DatSink *sink, size_t width)
{
    return CHECK(width <= sink->capacity - sink->length);
}

static void sink_put(DatSink *sink, unsigned long value, size_t width)
{
    unsigned char *bytes;
    size_t index;

    if (!sink->fine) {
        return;
    }
    sink->fine = sink_has_room(sink, width);
    bytes = sink->bytes;
    for (index = 0U; (index < width) && sink->fine; ++index) {
        bytes[sink->length + index] = (unsigned char)((value >> (BITS_PER_BYTE * index)) & BYTE_MASK);
    }
    sink->length += sink->fine ? width : 0U;
}

void dat_sink_u8(DatSink *sink, int value)
{
    sink->fine = sink->fine && CHECK(value >= 0) && CHECK(value <= BYTE_HIGHEST);
    sink_put(sink, (unsigned long)value, DAT_BYTE_WIDTH);
}

void dat_sink_i16(DatSink *sink, int value)
{
    sink->fine = sink->fine && CHECK(value >= SHORT_LOWEST) && CHECK(value <= SHORT_HIGHEST);
    sink_put(sink, (unsigned long)value & SHORT_MASK, DAT_SHORT_WIDTH);
}

void dat_sink_i32(DatSink *sink, long value)
{
    sink_put(sink, (unsigned long)value & LONG_MASK, DAT_LONG_WIDTH);
}

void dat_sink_f32(DatSink *sink, float value)
{
    uint32_t bits;

    if (!CHECK(sink->length <= sink->capacity)) {
        return;
    }
    (void)memcpy(&bits, &value, sizeof bits);
    sink_put(sink, (unsigned long)bits, DAT_LONG_WIDTH);
}

void dat_sink_text(DatSink *sink, const char *text)
{
    size_t length;
    size_t index;

    length = text_length(text, DAT_NAME_CAPACITY);
    sink->fine = sink->fine && CHECK(length < DAT_NAME_CAPACITY);
    sink_put(sink, DAT_TEXT_MARKER, DAT_SHORT_WIDTH);
    sink_put(sink, (unsigned long)length, DAT_SHORT_WIDTH);
    for (index = 0U; (index < length) && (index < DAT_NAME_CAPACITY); ++index) {
        sink_put(sink, (unsigned long)(unsigned char)text[index], DAT_BYTE_WIDTH);
    }
}
