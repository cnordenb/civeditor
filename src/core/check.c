#include "check.h"

#include "text.h"

static int failure_count;
static char last_failure[CHECK_MESSAGE_CAPACITY];

static bool append(const char *text)
{
    return text_append(last_failure, sizeof last_failure, text);
}

bool check_report(bool holds, const char *file, int line, const char *condition_text)
{
    if (holds) {
        return true;
    }
    ++failure_count;
    last_failure[0] = '\0';
    (void)(append(file) && append(":") && text_append_number(last_failure, sizeof last_failure, line) &&
           append(": check failed: ") && append(condition_text));
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

    for (index = 0; index < count; ++index) {
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
