#ifndef SUITES_H
#define SUITES_H

#include "../test_outcome.h"

enum {
    BUILD_SUITE_SIZE = 89,
    MOD_SUITE_SIZE = 30,
    CORE_SUITE_SIZE = 75
};

TestOutcome build_suite_run(int index);
TestOutcome mod_suite_run(int index);
TestOutcome core_suite_run(int index);
TestOutcome model_suite_run(int index);
TestOutcome shell_suite_run(int index);

#endif
