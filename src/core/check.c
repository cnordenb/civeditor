#include "check.h"

#include <stddef.h>

enum { DIGIT_CAPACITY = 12, DECIMAL_BASE = 10 };

static int failure_count;
static char last_failure[CHECK_MESSAGE_CAPACITY];

static size_t append_text(size_t used, const char *text)
{
    size_t index;

    if (text == NULL) {
        return used;
    }
    for (index = 0U; (index < CHECK_MESSAGE_CAPACITY) && (text[index] != '\0'); ++index) {
        if (used + 1U >= (size_t)CHECK_MESSAGE_CAPACITY) {
            break;
        }
        last_failure[used] = text[index];
        ++used;
    }
    last_failure[used] = '\0';
    return used;
}

static size_t append_line_number(size_t used, int line)
{
    char digits[DIGIT_CAPACITY];
    int count = 0;
    int remaining = (line < 0) ? 0 : line;
    int index;

    for (index = 0; index < DIGIT_CAPACITY - 1; ++index) {
        digits[count] = (char)('0' + (remaining % DECIMAL_BASE));
        ++count;
        remaining /= DECIMAL_BASE;
        if (remaining == 0) {
            break;
        }
    }
    for (index = count - 1; (index >= 0) && (index < DIGIT_CAPACITY); --index) {
        char single[2] = { digits[index], '\0' };
        used = append_text(used, single);
    }
    return used;
}

bool check_report(bool holds, const char *file, int line, const char *condition_text)
{
    size_t used = 0U;

    if (holds) {
        return true;
    }
    if (failure_count < CHECK_MESSAGE_CAPACITY * CHECK_MESSAGE_CAPACITY) {
        ++failure_count;
    }
    used = append_text(used, file);
    used = append_text(used, ":");
    used = append_line_number(used, line);
    used = append_text(used, ": check failed: ");
    (void)append_text(used, condition_text);
    return false;
}

int check_failure_count(void)
{
    return failure_count;
}

const char *check_last_failure(void)
{
    return last_failure;
}

void check_forget_failures(void)
{
    failure_count = 0;
    last_failure[0] = '\0';
}

bool check_is_among(int value, const int *values, int count)
{
    int index;

    if (!CHECK(values != NULL) || !CHECK(count <= CHECK_AMONG_LIMIT) ||
        !CHECK(count > 0)) {
        return false;
    }
    for (index = 0; (index < count) && (index < CHECK_AMONG_LIMIT); ++index) {
        if (values[index] == value) {
            return true;
        }
    }
    return false;
}

int check_index(int index, int count, int fallback)
{
    return (CHECK(index >= 0) && CHECK(index < count)) ? index : fallback;
}
