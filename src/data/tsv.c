#include "tsv.h"

#include "../core/check.h"
#include "../core/file_io.h"
#include "../core/text.h"

enum { DECIMAL_BASE = 10, HUNDRED = 100, FRACTION_DIGITS = 2 };

typedef struct {
    int value;
    int fraction_digits;
} Hundredths;

bool tsv_open(TsvFile *file, const wchar_t *path)
{
    FileRead read;

    read = file_read_all(path, file->data, sizeof file->data);
    file->length = read.length;
    file->position = 0U;
    return read.succeeded;
}

static void split_fields(TsvRecord *record, int length)
{
    int index;

    record->field_count = 1;
    record->starts[0] = 0;
    for (index = 0; (index < length) && (index < TSV_LINE_CAPACITY); ++index) {
        if ((record->text[index] != '\t') || (record->field_count >= TSV_FIELD_LIMIT)) {
            continue;
        }
        record->lengths[record->field_count - 1] = index - record->starts[record->field_count - 1];
        record->starts[record->field_count] = index + 1;
        ++record->field_count;
    }
    record->lengths[record->field_count - 1] = length - record->starts[record->field_count - 1];
}

bool tsv_next(TsvFile *file, TsvRecord *record)
{
    int length = 0;
    int step;
    char letter;

    if (file->position >= file->length) {
        return false;
    }
    for (step = 0; (step < TSV_LINE_CAPACITY - 1) && (file->position < file->length); ++step) {
        letter = file->data[file->position];
        ++file->position;
        if (letter == '\n') {
            break;
        }
        if (letter != '\r') {
            record->text[length] = letter;
            ++length;
        }
    }
    record->text[length] = '\0';
    split_fields(record, length);
    return true;
}

static bool has_field(const TsvRecord *record, int field)
{
    return check_is_index(field, record->field_count);
}

bool tsv_text(const TsvRecord *record, int field, char *destination, size_t capacity)
{
    if (!CHECK(capacity > 0U)) {
        return false;
    }
    destination[0] = '\0';
    if (!has_field(record, field)) {
        return false;
    }
    if ((record->lengths[field] == 1) && (record->text[record->starts[field]] == '-')) {
        return true;
    }
    return text_copy_span(destination, capacity, &record->text[record->starts[field]], (size_t)record->lengths[field]);
}

int tsv_number(const TsvRecord *record, int field)
{
    TextNumber number;

    if (!has_field(record, field)) {
        return TSV_MISSING;
    }
    number = text_to_number(&record->text[record->starts[field]], (size_t)record->lengths[field]);
    return number.valid ? (int)number.value : TSV_MISSING;
}

static bool take_hundredths_letter(Hundredths *number, char letter)
{
    if (!CHECK(number->fraction_digits <= FRACTION_DIGITS)) {
        return false;
    }
    if ((letter == '.') && (number->fraction_digits < 0)) {
        number->fraction_digits = 0;
        return true;
    }
    if ((letter < '0') || (letter > '9')) {
        return false;
    }
    if (number->fraction_digits < FRACTION_DIGITS) {
        number->value = (number->value * DECIMAL_BASE) + (letter - '0');
        number->fraction_digits += (number->fraction_digits < 0) ? 0 : 1;
    }
    return true;
}

static int whole_hundredths(Hundredths number)
{
    if (number.fraction_digits <= 0) {
        return number.value * HUNDRED;
    }
    return (number.fraction_digits == 1) ? number.value * DECIMAL_BASE : number.value;
}

int tsv_hundredths(const TsvRecord *record, int field)
{
    Hundredths number = { 0, -1 };
    int index;

    if (!has_field(record, field) || (record->lengths[field] < 1)) {
        return TSV_MISSING;
    }
    for (index = 0; (index < record->lengths[field]) && (index < TEXT_NUMBER_CAPACITY); ++index) {
        if (!take_hundredths_letter(&number, record->text[record->starts[field] + index])) {
            return TSV_MISSING;
        }
    }
    return whole_hundredths(number);
}

TsvNumberList tsv_number_list(const TsvRecord *record, int field)
{
    TsvNumberList list = { { 0 }, 0 };
    TextNumber number;
    int start;
    int index;
    int end;

    if (!has_field(record, field)) {
        return list;
    }
    start = record->starts[field];
    end = start + record->lengths[field];
    for (index = start; (index <= end) && (index < TSV_LINE_CAPACITY); ++index) {
        if ((index < end) && (record->text[index] != ',')) {
            continue;
        }
        number = text_to_number(&record->text[start], (size_t)(index - start));
        if (number.valid && (list.count < TSV_LIST_LIMIT)) {
            list.values[list.count] = (int)number.value;
            ++list.count;
        }
        start = index + 1;
    }
    return list;
}

bool tsv_decimal(const TsvRecord *record, int field, float *value)
{
    TextDecimal number;

    if (!has_field(record, field) || (record->lengths[field] < 1)) {
        return false;
    }
    number = text_to_decimal(&record->text[record->starts[field]], (size_t)record->lengths[field]);
    if (number.valid) {
        *value = (float)number.value;
    }
    return number.valid;
}

static uint64_t bit_of(const char *text, int length)
{
    TextNumber number;

    number = text_to_number(text, (size_t)length);
    if (!number.valid || (number.value < 0) || (number.value >= TSV_BIT_LIMIT)) {
        return 0U;
    }
    return (uint64_t)1U << (unsigned)number.value;
}

uint64_t tsv_bits(const TsvRecord *record, int field)
{
    char text[TSV_LINE_CAPACITY];
    uint64_t bits = 0U;
    int start = 0;
    int index;

    if (!tsv_text(record, field, text, sizeof text)) {
        return bits;
    }
    for (index = 0; (index < TSV_LINE_CAPACITY) && CHECK(start <= index); ++index) {
        if ((text[index] == ',') || (text[index] == '\0')) {
            bits |= bit_of(&text[start], index - start);
            start = index + 1;
        }
        if (text[index] == '\0') {
            break;
        }
    }
    return bits;
}

bool tsv_list_holds(const TsvNumberList *list, int value)
{
    int index;

    for (index = 0; (index < list->count) && (index < TSV_LIST_LIMIT); ++index) {
        if (list->values[index] == value) {
            return true;
        }
    }
    return false;
}
