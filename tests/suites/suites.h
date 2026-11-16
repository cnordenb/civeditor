#ifndef SUITES_H
#define SUITES_H

#include "../test_outcome.h"

enum {
    SUITE_TEST_LIMIT = 400,
    BUILD_SUITE_SIZE = 40,
    MOD_SUITE_SIZE = 29,
    CORE_SUITE_SIZE = 68,
    MODEL_SUITE_SIZE = 65
};

TestOutcome build_suite_run(int index);
TestOutcome mod_suite_run(int index);
TestOutcome core_suite_run(int index);
TestOutcome model_suite_run(int index);
TestOutcome shell_suite_run(int index);

#endif
