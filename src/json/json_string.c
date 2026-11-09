#include "json_reader.h"

#include "../core/check.h"

enum {
    HEX_LENGTH = 4,
    HEX_BASE = 16,
    DECIMAL_DIGITS = 10,
    SURROGATE_FIRST = 0xD800,
    SURROGATE_LOW = 0xDC00,
    SURROGATE_LAST = 0xDFFF,
    SURROGATE_SPAN = 0x400,
    SUPPLEMENTARY_START = 0x10000,
    ONE_BYTE_LIMIT = 0x80,
    TWO_BYTE_LIMIT = 0x800,
    THREE_BYTE_LIMIT = 0x10000,
    SIX_BITS = 6,
    TWELVE_BITS = 12,
    EIGHTEEN_BITS = 18,
    LOW_SIX_MASK = 0x3F,
    CONTINUATION = 0x80,
    TWO_BYTE_LEAD = 0xC0,
    THREE_BYTE_LEAD = 0xE0,
    FOUR_BYTE_LEAD = 0xF0,
    JSON_STRING_LIMIT = 4096
};

typedef struct {
    char bytes[JSON_STRING_LIMIT];
    size_t used;
    bool overflow;
} Utf8Sink;

static int hex_value(char letter)
{
    if ((letter >= '0') && (letter <= '9')) {
        return letter - '0';
    }
    if ((letter >= 'a') && (letter <= 'f')) {
        return letter - 'a' + DECIMAL_DIGITS;
    }
    if ((letter >= 'A') && (letter <= 'F')) {
        return letter - 'A' + DECIMAL_DIGITS;
    }
    return -1;
}

static long read_hex(const char *text, int available)
{
    long value = 0L;
    int index;
    int digit;

    if (!CHECK(text != NULL) || !CHECK(available >= 0) || (available < HEX_LENGTH)) {
        return -1L;
    }
    for (index = 0; index < HEX_LENGTH; ++index) {
        digit = hex_value(text[index]);
        if (digit < 0) {
            return -1L;
        }
        value = (value * HEX_BASE) + digit;
    }
    return value;
}

static void put_byte(Utf8Sink *sink, unsigned long value)
{
    if (!CHECK(sink != NULL) || !CHECK(value < 256UL)) {
        return;
    }
    if (sink->used + 1U >= (size_t)JSON_STRING_LIMIT) {
        sink->overflow = true;
        return;
    }
    sink->bytes[sink->used] = (char)(unsigned char)value;
    ++sink->used;
}

static void put_code_point(Utf8Sink *sink, unsigned long code)
{
    if (!CHECK(sink != NULL) || !CHECK(code <= 0x10FFFFUL)) {
        return;
    }
    if (code < (unsigned long)ONE_BYTE_LIMIT) {
        put_byte(sink, code);
    } else if (code < (unsigned long)TWO_BYTE_LIMIT) {
        put_byte(sink, (unsigned long)TWO_BYTE_LEAD | (code >> SIX_BITS));
        put_byte(sink, (unsigned long)CONTINUATION | (code & (unsigned long)LOW_SIX_MASK));
    } else if (code < (unsigned long)THREE_BYTE_LIMIT) {
        put_byte(sink, (unsigned long)THREE_BYTE_LEAD | (code >> TWELVE_BITS));
        put_byte(sink, (unsigned long)CONTINUATION | ((code >> SIX_BITS) & (unsigned long)LOW_SIX_MASK));
        put_byte(sink, (unsigned long)CONTINUATION | (code & (unsigned long)LOW_SIX_MASK));
    } else {
        put_byte(sink, (unsigned long)FOUR_BYTE_LEAD | (code >> EIGHTEEN_BITS));
        put_byte(sink, (unsigned long)CONTINUATION | ((code >> TWELVE_BITS) & (unsigned long)LOW_SIX_MASK));
        put_byte(sink, (unsigned long)CONTINUATION | ((code >> SIX_BITS) & (unsigned long)LOW_SIX_MASK));
        put_byte(sink, (unsigned long)CONTINUATION | (code & (unsigned long)LOW_SIX_MASK));
    }
}

static int decode_unicode_escape(Utf8Sink *sink, const char *text, int available)
{
    long first = read_hex(text, available);
    long second;

    if (!CHECK(sink != NULL) || !CHECK(text != NULL) || (first < 0L)) {
        return -1;
    }
    if ((first < SURROGATE_FIRST) || (first > SURROGATE_LAST)) {
        put_code_point(sink, (unsigned long)first);
        return HEX_LENGTH;
    }
    if ((first >= SURROGATE_LOW) || (available < (2 * HEX_LENGTH) + 2) || (text[HEX_LENGTH] != '\\')) {
        return -1;
    }
    second = read_hex(&text[HEX_LENGTH + 2], available - HEX_LENGTH - 2);
    if ((second < SURROGATE_LOW) || (second > SURROGATE_LAST)) {
        return -1;
    }
    put_code_point(sink, (unsigned long)(SUPPLEMENTARY_START + ((first - SURROGATE_FIRST) * SURROGATE_SPAN) +
                                         (second - SURROGATE_LOW)));
    return (2 * HEX_LENGTH) + 2;
}

static int decode_escape(Utf8Sink *sink, const char *text, int available)
{
    if (!CHECK(sink != NULL) || !CHECK(available > 0)) {
        return -1;
    }
    switch (text[0]) {
    case 'n':
        put_byte(sink, (unsigned long)'\n');
        return 1;
    case 't':
        put_byte(sink, (unsigned long)'\t');
        return 1;
    case 'r':
        put_byte(sink, (unsigned long)'\r');
        return 1;
    case 'b':
    case 'f':
        return 1;
    case 'u':
        return 1 + decode_unicode_escape(sink, &text[1], available - 1);
    default:
        put_byte(sink, (unsigned long)(unsigned char)text[0]);
        return 1;
    }
}

bool json_copy_string(const JsonDocument *document, int token, char *destination, size_t capacity)
{
    static Utf8Sink sink;
    int position;
    int end;
    int step;
    int consumed;

    if (!CHECK(destination != NULL) || !CHECK(capacity > 0U) || !json_is_kind(document, token, JSON_KIND_STRING)) {
        return false;
    }
    sink.used = 0U;
    sink.overflow = false;
    position = document->tokens[token].start;
    end = document->tokens[token].end;
    for (step = 0; (step < JSON_INPUT_CAPACITY) && (position < end) && !sink.overflow; ++step) {
        if (document->text[position] != '\\') {
            put_byte(&sink, (unsigned long)(unsigned char)document->text[position]);
            ++position;
            continue;
        }
        consumed = decode_escape(&sink, &document->text[position + 1], end - position - 1);
        if (consumed <= 0) {
            break;
        }
        position += 1 + consumed;
    }
    sink.bytes[sink.used] = '\0';
    return !sink.overflow && (position >= end) && text_copy(destination, capacity, sink.bytes);
}
