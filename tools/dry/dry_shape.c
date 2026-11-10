#include "dry_shape.h"

#include <stddef.h>

#include "../../src/core/check.h"
#include "../../tests/scan/source_scan.h"

enum { TOKEN_LIMIT = 20000, KEPT_COUNT = 6, KEPT_WIDTH = 12 };

typedef struct {
    int at;
    uint32_t print;
    bool found;
} Token;

static const uint32_t HASH_BASIS = 2166136261U;
static const uint32_t HASH_PRIME = 16777619U;
static const uint32_t NAME_PRINT = 1U;
static const uint32_t LITERAL_PRINT = 2U;
static const char KEPT_WORDS[KEPT_COUNT][KEPT_WIDTH] = { "return", "else", "break", "continue", "case", "default" };

static uint32_t print_of(const char *text, int start, int end)
{
    uint32_t print = HASH_BASIS;
    int index;

    if (!CHECK(text != NULL) || !CHECK(start <= end)) {
        return 0U;
    }
    for (index = start; (index < end) && (index < SCAN_TEXT_CAPACITY); ++index) {
        print = (print ^ (uint32_t)(unsigned char)text[index]) * HASH_PRIME;
    }
    return print;
}

static int skip_blank(const char *text, int at, int end)
{
    int index;

    if (!CHECK(text != NULL) || !CHECK(at >= 0)) {
        return end;
    }
    for (index = at; (index < end) && (index < SCAN_TEXT_CAPACITY); ++index) {
        if ((text[index] != ' ') && (text[index] != '\n') && (text[index] != '\r') && (text[index] != '\t')) {
            break;
        }
    }
    return index;
}

static int word_end(const char *text, int at, int end)
{
    int index;

    if (!CHECK(text != NULL) || !CHECK(at >= 0)) {
        return end;
    }
    for (index = at; (index < end) && (index < SCAN_TEXT_CAPACITY); ++index) {
        if (!source_is_word_letter(text[index])) {
            break;
        }
    }
    return index;
}

static bool keeps_its_name(const char *text, int start, int stop, int end)
{
    int next = skip_blank(text, stop, end);
    int word;

    if (!CHECK(text != NULL) || !CHECK(start < stop)) {
        return false;
    }
    if ((next < end) && (text[next] == '(')) {
        return true;
    }
    for (word = 0; word < KEPT_COUNT; ++word) {
        if (source_span_is(text, start, stop, KEPT_WORDS[word])) {
            return true;
        }
    }
    return false;
}

static Token next_token(const char *text, int at, int end)
{
    Token token = { end, 0U, false };
    int start = skip_blank(text, at, end);
    int stop;

    if (!CHECK(text != NULL) || !CHECK(at <= end) || (start >= end)) {
        return token;
    }
    token.found = true;
    if (!source_is_word_letter(text[start])) {
        token.at = start + 1;
        token.print = print_of(text, start, start + 1);
        return token;
    }
    stop = word_end(text, start, end);
    token.at = stop;
    if ((text[start] >= '0') && (text[start] <= '9')) {
        token.print = LITERAL_PRINT;
    } else {
        token.print = keeps_its_name(text, start, stop, end) ? print_of(text, start, stop) : NAME_PRINT;
    }
    return token;
}

static void add_print(DryShape *shape, const uint32_t *window)
{
    uint32_t print = HASH_BASIS;
    int index;

    if (!CHECK(shape != NULL) || !CHECK(window != NULL) || (shape->count >= DRY_PRINT_CAPACITY)) {
        return;
    }
    for (index = 0; index < DRY_SHINGLE; ++index) {
        print = (print ^ window[index]) * HASH_PRIME;
    }
    shape->prints[shape->count] = print;
    ++shape->count;
}

static void sort_prints(DryShape *shape)
{
    uint32_t held;
    int index;
    int back;

    if (!CHECK(shape != NULL) || !CHECK(shape->count <= DRY_PRINT_CAPACITY)) {
        return;
    }
    for (index = 1; (index < shape->count) && (index < DRY_PRINT_CAPACITY); ++index) {
        held = shape->prints[index];
        for (back = index; (back > 0) && (back < DRY_PRINT_CAPACITY); --back) {
            if (shape->prints[back - 1] <= held) {
                break;
            }
            shape->prints[back] = shape->prints[back - 1];
        }
        shape->prints[back] = held;
    }
}

static void drop_repeats(DryShape *shape)
{
    int kept = 0;
    int index;

    if (!CHECK(shape != NULL) || !CHECK(shape->count <= DRY_PRINT_CAPACITY)) {
        return;
    }
    for (index = 0; (index < shape->count) && (index < DRY_PRINT_CAPACITY); ++index) {
        if ((kept == 0) || (shape->prints[kept - 1] != shape->prints[index])) {
            shape->prints[kept] = shape->prints[index];
            ++kept;
        }
    }
    shape->count = kept;
}

bool dry_shape_of(DryShape *shape, const char *text, int start, int end)
{
    uint32_t window[DRY_SHINGLE] = { 0U, 0U, 0U, 0U };
    Token token = { start, 0U, true };
    int step;
    int slot;

    if (!CHECK(shape != NULL) || !CHECK(text != NULL) || !CHECK(start <= end)) {
        return false;
    }
    shape->count = 0;
    shape->tokens = 0;
    for (step = 0; step < TOKEN_LIMIT; ++step) {
        token = next_token(text, token.at, end);
        if (!token.found) {
            break;
        }
        for (slot = 0; slot < DRY_SHINGLE - 1; ++slot) {
            window[slot] = window[slot + 1];
        }
        window[DRY_SHINGLE - 1] = token.print;
        ++shape->tokens;
        if (shape->tokens >= DRY_SHINGLE) {
            add_print(shape, window);
        }
    }
    sort_prints(shape);
    drop_repeats(shape);
    return true;
}

int dry_alike_milli(const DryShape *left, const DryShape *right)
{
    int in_left = 0;
    int in_right = 0;
    int shared = 0;
    int step;

    if (!CHECK(left != NULL) || !CHECK(right != NULL)) {
        return 0;
    }
    for (step = 0; (step < 2 * DRY_PRINT_CAPACITY) && (in_left < left->count) && (in_right < right->count); ++step) {
        if (left->prints[in_left] == right->prints[in_right]) {
            ++shared;
            ++in_left;
            ++in_right;
        } else if (left->prints[in_left] < right->prints[in_right]) {
            ++in_left;
        } else {
            ++in_right;
        }
    }
    if ((left->count + right->count - shared) <= 0) {
        return 0;
    }
    return (shared * DRY_MILLI) / (left->count + right->count - shared);
}
