#include "text.h"

#include <limits.h>

#include "check.h"

static const long long DIGITS_LIMIT = 100000000000000000LL;

typedef struct {
    long long digits;
    double scale;
    bool fraction;
    int digit_count;
} Decimal;

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

size_t text_length(const char *text, size_t capacity)
{
    size_t index;

    if (!CHECK(capacity <= (size_t)TEXT_SCAN_LIMIT)) {
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

    if (!CHECK(capacity > 0U)) {
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
    return text_copy_span(destination, capacity, source, text_length(source, (size_t)TEXT_SCAN_LIMIT));
}

bool text_append(char *destination, size_t capacity, const char *source)
{
    size_t used;

    if (!CHECK(capacity > 0U)) {
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

    if (!CHECK(capacity > 0U)) {
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

static unsigned put_fraction(char *fraction, long *whole, unsigned places)
{
    unsigned count = places;
    unsigned index;

    while ((count > 0U) && ((*whole % DECIMAL_BASE) == 0L)) {
        *whole /= DECIMAL_BASE;
        --count;
    }
    fraction[0] = '.';
    fraction[count + 1U] = '\0';
    for (index = count; index > 0U; --index) {
        fraction[index] = (char)('0' + (int)(*whole % DECIMAL_BASE));
        *whole /= DECIMAL_BASE;
    }
    return count;
}

bool text_append_decimal(char *destination, size_t capacity, long value, unsigned places)
{
    char fraction[TEXT_NUMBER_CAPACITY];
    long whole = value;
    unsigned count;

    if (!CHECK((places <= (unsigned)TEXT_PLACES_LIMIT) && ((places == 0U) || (value >= 0L)))) {
        return false;
    }
    count = put_fraction(fraction, &whole, places);
    return text_append_number(destination, capacity, whole) &&
           ((count == 0U) || text_append(destination, capacity, fraction));
}

bool text_starts_with(const char *text, const char *prefix)
{
    size_t index;

    for (index = 0U; index < (size_t)TEXT_SCAN_LIMIT; ++index) {
        if (prefix[index] == '\0') {
            return true;
        }
        if (text[index] != prefix[index]) {
            return false;
        }
    }
    return false;
}

bool text_equal(const char *left, const char *right)
{
    return text_starts_with(left, right) && text_starts_with(right, left);
}

static char fold_case(char letter)
{
    if ((letter >= 'A') && (letter <= 'Z')) {
        return (char)(letter + ASCII_CASE_GAP);
    }
    return letter;
}

bool text_starts_folded(const char *text, const char *prefix)
{
    size_t index;

    for (index = 0U; index < (size_t)TEXT_SCAN_LIMIT; ++index) {
        if (prefix[index] == '\0') {
            return true;
        }
        if (fold_case(text[index]) != fold_case(prefix[index])) {
            return false;
        }
    }
    return false;
}

bool text_contains_folded(const char *text, const char *needle)
{
    size_t start;

    for (start = 0U; start < (size_t)TEXT_SCAN_LIMIT; ++start) {
        if (text_starts_folded(&text[start], needle)) {
            return true;
        }
        if (text[start] == '\0') {
            return false;
        }
    }
    return false;
}

static bool take_digit(TextNumber *number, char letter)
{
    long digit = (long)(letter - '0');

    if (!text_is_digit(letter) || (number->value > (LONG_MAX - digit) / DECIMAL_BASE)) {
        return false;
    }
    number->value = (number->value * DECIMAL_BASE) + digit;
    number->valid = true;
    return true;
}

TextNumber text_to_number(const char *text, size_t length)
{
    TextNumber result = { false, 0L };
    size_t index;
    bool negative;

    negative = (length > 0U) && (text[0] == '-');
    for (index = negative ? 1U : 0U; index < length; ++index) {
        if (!take_digit(&result, text[index])) {
            result.valid = false;
            return result;
        }
    }
    if (negative) {
        result.value = -result.value;
    }
    return result;
}

static bool take_decimal_letter(Decimal *number, char letter)
{
    if ((letter == '.') && !number->fraction) {
        number->fraction = true;
        return true;
    }
    if (!text_is_digit(letter) || (number->digits > DIGITS_LIMIT)) {
        return false;
    }
    number->digits = (number->digits * DECIMAL_BASE) + (long long)(letter - '0');
    ++number->digit_count;
    if (number->fraction) {
        number->scale *= (double)DECIMAL_BASE;
    }
    return true;
}

TextDecimal text_to_decimal(const char *text, size_t length)
{
    Decimal number = { 0LL, 1.0, false, 0 };
    TextDecimal result = { false, 0.0 };
    size_t sign = ((length > 0U) && (text[0] == '-')) ? 1U : 0U;
    size_t index;

    if (length > (size_t)TEXT_NUMBER_CAPACITY) {
        return result;
    }
    for (index = sign; index < length; ++index) {
        if (!take_decimal_letter(&number, text[index])) {
            return result;
        }
    }
    result.valid = number.digit_count > 0;
    result.value = (double)number.digits / number.scale;
    if (sign == 1U) {
        result.value = -result.value;
    }
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

    if (!CHECK(text[at] != '\0')) {
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

    while (text[in] != '\0') {
        decoded = utf8_decode(text, in);
        if ((decoded.length == 0) || (out.used + 1U >= capacity) || !put_wide(&out, decoded.code)) {
            wide[out.used] = L'\0';
            return false;
        }
        in += (size_t)decoded.length;
    }
    wide[out.used] = L'\0';
    return true;
}

bool text_is_between(char letter, char first, char last)
{
    return (letter >= first) && (letter <= last);
}

bool text_is_digit(char letter)
{
    return text_is_between(letter, '0', '9');
}

bool text_is_sign(char letter)
{
    return (letter == '+') || (letter == '-');
}

enum { LINE_BREAK_COUNT = 2 };

static const int LINE_BREAKS[LINE_BREAK_COUNT] = { '\n', '\r' };

bool text_is_line_break(char letter)
{
    return check_is_among(letter, LINE_BREAKS, LINE_BREAK_COUNT);
}

bool text_append_two(char *destination, size_t capacity, const char *first, const char *second)
{
    return text_append_three(destination, capacity, first, second, "");
}

bool text_append_three(char *destination, size_t capacity, const char *first, const char *second, const char *third)
{
    return text_append(destination, capacity, first) && text_append(destination, capacity, second) &&
           text_append(destination, capacity, third);
}

bool text_join(char *destination, size_t capacity, const char *first, const char *second)
{
    if (!CHECK(capacity > 0U)) {
        return false;
    }
    destination[0] = '\0';
    return text_append_two(destination, capacity, first, second);
}
