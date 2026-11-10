#include "text.h"

#include "check.h"

enum {
    DECIMAL_BASE = 10,
    ASCII_CASE_GAP = 32,
    UTF8_LONGEST = 4,
    UTF8_CONTINUATION = 0x80,
    UTF8_TWO_LEAD = 0xC0,
    UTF8_THREE_LEAD = 0xE0,
    UTF8_FOUR_LEAD = 0xF0,
    UTF8_FIVE_LEAD = 0xF8,
    UTF8_LEAD_MASK = 0x7F,
    UTF8_TAG_MASK = 0xC0,
    UTF8_PAYLOAD_BITS = 6,
    UTF8_PAYLOAD_MASK = 0x3F,
    UTF16_PLANE = 0x10000,
    UTF16_LAST = 0x10FFFF,
    UTF16_HIGH = 0xD800,
    UTF16_LOW = 0xDC00,
    UTF16_HALF_BITS = 10,
    UTF16_HALF_MASK = 0x3FF
};

typedef struct {
    unsigned long code;
    int length;
} TextCode;

typedef struct {
    wchar_t *text;
    size_t used;
    size_t capacity;
} WideText;

static const long NUMBER_LIMIT = 999999999L;

size_t text_length(const char *text, size_t capacity)
{
    size_t index;

    if (!CHECK(text != NULL) || !CHECK(capacity <= (size_t)TEXT_SCAN_LIMIT)) {
        return 0U;
    }
    for (index = 0U; (index < capacity) && (index < (size_t)TEXT_SCAN_LIMIT); ++index) {
        if (text[index] == '\0') {
            return index;
        }
    }
    return capacity;
}

bool text_copy_span(char *destination, size_t capacity, const char *source, size_t length)
{
    size_t index;

    if (!CHECK(destination != NULL) || !CHECK(source != NULL) || !CHECK(capacity > 0U)) {
        return false;
    }
    for (index = 0U; (index < length) && (index < (size_t)TEXT_SCAN_LIMIT); ++index) {
        if (index + 1U >= capacity) {
            destination[index] = '\0';
            return false;
        }
        destination[index] = source[index];
    }
    destination[index] = '\0';
    return true;
}

bool text_copy(char *destination, size_t capacity, const char *source)
{
    if (!CHECK(destination != NULL) || !CHECK(source != NULL)) {
        return false;
    }
    return text_copy_span(destination, capacity, source, text_length(source, (size_t)TEXT_SCAN_LIMIT));
}

bool text_append(char *destination, size_t capacity, const char *source)
{
    size_t used;

    if (!CHECK(destination != NULL) || !CHECK(source != NULL) || !CHECK(capacity > 0U)) {
        return false;
    }
    used = text_length(destination, capacity);
    if (!CHECK(used < capacity)) {
        return false;
    }
    return text_copy_span(&destination[used], capacity - used, source,
                          text_length(source, (size_t)TEXT_SCAN_LIMIT));
}

bool text_append_number(char *destination, size_t capacity, long value)
{
    char reversed[TEXT_NUMBER_CAPACITY];
    char digits[TEXT_NUMBER_CAPACITY];
    unsigned long remaining = (value < 0) ? (unsigned long)(-(value + 1)) + 1UL : (unsigned long)value;
    int count = 0;
    int index;

    if (!CHECK(destination != NULL) || !CHECK(capacity > 0U)) {
        return false;
    }
    for (index = 0; index < TEXT_NUMBER_CAPACITY - 1; ++index) {
        reversed[count] = (char)('0' + (int)(remaining % (unsigned long)DECIMAL_BASE));
        ++count;
        remaining /= (unsigned long)DECIMAL_BASE;
        if (remaining == 0UL) {
            break;
        }
    }
    digits[0] = '-';
    for (index = 0; (index < count) && (index < TEXT_NUMBER_CAPACITY - 2); ++index) {
        digits[index + 1] = reversed[count - 1 - index];
    }
    digits[index + 1] = '\0';
    return text_append(destination, capacity, (value < 0) ? digits : &digits[1]);
}

static bool matches(const char *text, const char *other, bool whole)
{
    size_t index;

    if (!CHECK(text != NULL) || !CHECK(other != NULL)) {
        return false;
    }
    for (index = 0U; index < (size_t)TEXT_SCAN_LIMIT; ++index) {
        if (other[index] == '\0') {
            return !whole || (text[index] == '\0');
        }
        if (text[index] != other[index]) {
            return false;
        }
    }
    return false;
}

bool text_equal(const char *left, const char *right)
{
    return matches(left, right, true);
}

bool text_starts_with(const char *text, const char *prefix)
{
    return matches(text, prefix, false);
}

static char fold_case(char letter)
{
    if ((letter >= 'A') && (letter <= 'Z')) {
        return (char)(letter + ASCII_CASE_GAP);
    }
    return letter;
}

static bool matches_at(const char *text, const char *needle)
{
    size_t index;

    if (!CHECK(text != NULL) || !CHECK(needle != NULL)) {
        return false;
    }
    for (index = 0U; index < (size_t)TEXT_SCAN_LIMIT; ++index) {
        if (needle[index] == '\0') {
            return true;
        }
        if (fold_case(text[index]) != fold_case(needle[index])) {
            return false;
        }
    }
    return false;
}

bool text_contains_folded(const char *text, const char *needle)
{
    size_t start;

    if (!CHECK(text != NULL) || !CHECK(needle != NULL)) {
        return false;
    }
    for (start = 0U; start < (size_t)TEXT_SCAN_LIMIT; ++start) {
        if (matches_at(&text[start], needle)) {
            return true;
        }
        if (text[start] == '\0') {
            return false;
        }
    }
    return false;
}

TextNumber text_to_number(const char *text, size_t length)
{
    TextNumber result = { false, 0L };
    bool negative = (length > 0U) && (text != NULL) && (text[0] == '-');
    size_t index;

    if (!CHECK(text != NULL) || !CHECK(length < (size_t)TEXT_NUMBER_CAPACITY)) {
        return result;
    }
    for (index = negative ? 1U : 0U; (index < length) && (index < (size_t)TEXT_NUMBER_CAPACITY); ++index) {
        if ((text[index] < '0') || (text[index] > '9') || (result.value > NUMBER_LIMIT)) {
            result.valid = false;
            return result;
        }
        result.value = (result.value * DECIMAL_BASE) + (long)(text[index] - '0');
        result.valid = true;
    }
    result.value = negative ? -result.value : result.value;
    return result;
}

static int utf8_length(unsigned char lead)
{
    if (!CHECK(lead != 0U)) {
        return 0;
    }
    if (lead < UTF8_TWO_LEAD) {
        return (lead < UTF8_CONTINUATION) ? 1 : 0;
    }
    if (lead < UTF8_THREE_LEAD) {
        return 2;
    }
    return (lead < UTF8_FOUR_LEAD) ? 3 : ((lead < UTF8_FIVE_LEAD) ? UTF8_LONGEST : 0);
}

static TextCode utf8_decode(const char *text, size_t at)
{
    TextCode decoded = { 0UL, 0 };
    unsigned char byte;
    int length;
    int index;

    if (!CHECK(text != NULL) || !CHECK(text[at] != '\0')) {
        return decoded;
    }
    byte = (unsigned char)text[at];
    length = utf8_length(byte);
    decoded.code = (length == 1) ? byte : (unsigned long)(byte & (UTF8_LEAD_MASK >> length));
    for (index = 1; (index < length) && (index < UTF8_LONGEST); ++index) {
        byte = (unsigned char)text[at + (size_t)index];
        if ((byte & UTF8_TAG_MASK) != UTF8_CONTINUATION) {
            return decoded;
        }
        decoded.code = (decoded.code << UTF8_PAYLOAD_BITS) | (unsigned long)(byte & UTF8_PAYLOAD_MASK);
    }
    decoded.length = length;
    return decoded;
}

static bool put_wide(WideText *wide, unsigned long code)
{
    unsigned long rest;

    if (!CHECK(wide != NULL) || !CHECK(wide->used < wide->capacity)) {
        return false;
    }
    if (code < UTF16_PLANE) {
        wide->text[wide->used] = (wchar_t)code;
        ++wide->used;
        return true;
    }
    if ((wide->used + 2U >= wide->capacity) || (code > UTF16_LAST)) {
        return false;
    }
    rest = code - UTF16_PLANE;
    wide->text[wide->used] = (wchar_t)(UTF16_HIGH + (rest >> UTF16_HALF_BITS));
    wide->text[wide->used + 1U] = (wchar_t)(UTF16_LOW + (rest & UTF16_HALF_MASK));
    wide->used += 2U;
    return true;
}

bool text_widen(const char *text, wchar_t *wide, size_t capacity)
{
    WideText out = { wide, 0U, capacity };
    TextCode decoded;
    size_t in = 0U;
    int step;

    if (!CHECK(text != NULL) || !CHECK((wide != NULL) && (capacity > 0U))) {
        return false;
    }
    for (step = 0; (step < TEXT_SCAN_LIMIT) && (text[in] != '\0'); ++step) {
        decoded = utf8_decode(text, in);
        if ((decoded.length == 0) || (out.used + 1U >= capacity) || !put_wide(&out, decoded.code)) {
            wide[out.used] = L'\0';
            return false;
        }
        in += (size_t)decoded.length;
    }
    wide[out.used] = L'\0';
    return text[in] == '\0';
}
