#ifndef TEST_OUTCOME_H
#define TEST_OUTCOME_H

#include <stdbool.h>

typedef struct {
    const char *name;
    bool exists;
    bool passed;
} TestOutcome;

TestOutcome test_outcome_of(const char *name, bool passed);
TestOutcome test_outcome_missing(void);
void test_note(const char *text);

TestOutcome functional_suite_run(int index);
TestOutcome accessibility_suite_run(int index);
TestOutcome maintainability_suite_run(int index);

#endif
