#include <stdio.h>

#include "../src/core/check.h"
#include "../src/core/text.h"
#include "test_outcome.h"

enum { CATEGORY_COUNT = 3, CATEGORY_TEST_LIMIT = 400, LINE_CAPACITY = 512, ARGUMENT_LIMIT = 8 };

typedef struct {
    int run;
    int failed;
} TestTally;

static char last_note[LINE_CAPACITY];

void test_note(const char *text)
{
    if (!CHECK(text != NULL) || !CHECK(LINE_CAPACITY > 1)) {
        return;
    }
    (void)text_copy(last_note, sizeof last_note, text);
}

TestOutcome test_outcome_of(const char *name, bool passed)
{
    TestOutcome outcome = { "", true, false };

    if (!CHECK(name != NULL) || !CHECK(name[0] != '\0')) {
        return outcome;
    }
    outcome.name = name;
    outcome.passed = passed;
    return outcome;
}

TestOutcome test_outcome_missing(void)
{
    TestOutcome outcome = { "", false, false };

    return outcome;
}

static bool rules_left_out;

static TestOutcome run_in_category(int category, int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= CATEGORY_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (category) {
    case 0:
        return functional_suite_run(index);
    case 1:
        return accessibility_suite_run(index);
    case 2:
        return rules_left_out ? maintainability_tools_run(index) : maintainability_suite_run(index);
    default:
        return test_outcome_missing();
    }
}

static bool print_outcome(int number, TestOutcome outcome)
{
    char line[LINE_CAPACITY] = "Test ";

    if (!CHECK(number > 0) || !CHECK(outcome.name != NULL)) {
        return false;
    }
    (void)text_append_number(line, sizeof line, number);
    (void)text_append(line, sizeof line, outcome.passed ? " passed: " : " FAILED: ");
    (void)text_append(line, sizeof line, outcome.name);
    (void)text_append(line, sizeof line, "\n");
    if (!outcome.passed && (last_note[0] != '\0')) {
        (void)text_append(line, sizeof line, "    at: ");
        (void)text_append(line, sizeof line, last_note);
        (void)text_append(line, sizeof line, "\n");
    }
    if (!outcome.passed && (check_failure_count() > 0)) {
        (void)text_append(line, sizeof line, "    ");
        (void)text_append(line, sizeof line, check_last_failure());
        (void)text_append(line, sizeof line, "\n");
    }
    return fputs(line, stdout) >= 0;
}

static void run_category(int category, TestTally *tally)
{
    TestOutcome outcome;
    int index;

    if (!CHECK(tally != NULL) || !CHECK(category < CATEGORY_COUNT)) {
        return;
    }
    for (index = 1; index <= CATEGORY_TEST_LIMIT; ++index) {
        check_forget_failures();
        last_note[0] = '\0';
        outcome = run_in_category(category, index);
        if (!outcome.exists) {
            return;
        }
        ++tally->run;
        tally->failed += outcome.passed ? 0 : 1;
        (void)print_outcome(tally->run, outcome);
    }
}

static bool window_left_out;

bool test_window_is_left_out(void)
{
    if (!CHECK(CATEGORY_COUNT > 0) || !CHECK(LINE_CAPACITY > 0)) {
        return false;
    }
    return window_left_out;
}

static bool print_summary(TestTally tally)
{
    char summary[LINE_CAPACITY] = "";

    if (!CHECK(tally.run >= 0) || !CHECK(tally.failed <= tally.run)) {
        return false;
    }
    (void)text_append_number(summary, sizeof summary, tally.run);
    (void)text_append(summary, sizeof summary, " tests run, ");
    (void)text_append_number(summary, sizeof summary, tally.failed);
    (void)text_append(summary, sizeof summary, " failed.\n");
    return fputs(summary, stdout) >= 0;
}

static int exit_code(TestTally tally)
{
    if (!CHECK(tally.run >= 0) || !CHECK(tally.failed <= tally.run)) {
        return 1;
    }
    return ((tally.failed == 0) && (tally.run > 0)) ? 0 : 1;
}

static void read_arguments(int argc, char *argv[])
{
    int index;

    if (!CHECK(argc >= 1) || !CHECK(argv != NULL)) {
        return;
    }
    for (index = 1; (index < argc) && (index < ARGUMENT_LIMIT); ++index) {
        window_left_out = window_left_out || text_equal(argv[index], "nowindow");
        rules_left_out = rules_left_out || text_equal(argv[index], "norules");
    }
}

int main(int argc, char *argv[])
{
    TestTally tally = { 0, 0 };
    int category;

    read_arguments(argc, argv);
    for (category = 0; category < CATEGORY_COUNT; ++category) {
        run_category(category, &tally);
    }
    (void)print_summary(tally);
    return exit_code(tally);
}
