#include <stdio.h>

#include "../src/core/check.h"
#include "../src/core/text.h"
#include "suites/suites.h"
#include "test_outcome.h"

enum {
    CATEGORY_COUNT = 8,
    LABEL_COUNT = 3,
    REGRESSION_CATEGORY = 3,
    REDUNDANCY_CATEGORY = 4,
    CATEGORY_TEST_LIMIT = 1000,
    LINE_CAPACITY = 512,
    ARGUMENT_LIMIT = 8
};

static const char *const LABELS[LABEL_COUNT] = { "Test ", "Regression test ", "Redundancy test " };
static const int LABEL_OF[CATEGORY_COUNT] = { 0, 0, 0, 1, 2, 2, 2, 2 };

typedef struct {
    int run;
    int failed;
    int numbered[LABEL_COUNT];
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
static bool redundancy_asked;

static TestOutcome run_redundancy_category(int category, int index)
{
    switch (category) {
    case REDUNDANCY_CATEGORY:
        return redundancy_suite_run(index);
    case REDUNDANCY_CATEGORY + 1:
        return functional_suite_run(index);
    case REDUNDANCY_CATEGORY + 2:
        return test_window_is_left_out() ? test_outcome_missing() : shell_suite_run(index);
    default:
        return rules_left_out ? rule_redundancy_tools_run(index) : rule_redundancy_run(index);
    }
}

static TestOutcome run_in_category(int category, int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= CATEGORY_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (category) {
    case 0:
        return requirement_suite_run(index);
    case 1:
        return accessibility_suite_run(index);
    case 2:
        return rules_left_out ? test_outcome_missing() : maintainability_suite_run(index);
    case REGRESSION_CATEGORY:
        return regression_suite_run(index);
    default:
        return redundancy_asked ? run_redundancy_category(category, index) : test_outcome_missing();
    }
}

static bool print_outcome(int category, int number, TestOutcome outcome)
{
    char line[LINE_CAPACITY] = "";

    if (!CHECK(number > 0) || !CHECK(outcome.name != NULL)) {
        return false;
    }
    (void)text_copy(line, sizeof line, LABELS[LABEL_OF[category]]);
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
        ++tally->numbered[LABEL_OF[category]];
        tally->failed += outcome.passed ? 0 : 1;
        (void)print_outcome(category, tally->numbered[LABEL_OF[category]], outcome);
    }
    ++tally->failed;
    (void)fputs("A category holds more tests than CATEGORY_TEST_LIMIT in tests/runner.c lets run.\n", stdout);
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
        redundancy_asked = redundancy_asked || text_equal(argv[index], "redundancy");
    }
}

int main(int argc, char *argv[])
{
    TestTally tally = { 0, 0, { 0, 0, 0 } };
    int category;

    read_arguments(argc, argv);
    for (category = 0; category < CATEGORY_COUNT; ++category) {
        run_category(category, &tally);
    }
    (void)print_summary(tally);
    return exit_code(tally);
}
