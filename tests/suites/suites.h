#ifndef SUITES_H
#define SUITES_H

#include "../test_outcome.h"

enum {
    BUILD_SUITE_SIZE = 85,
    MOD_SUITE_SIZE = 30,
    CORE_SUITE_SIZE = 76,
    MODEL_SUITE_SIZE = 70
};

TestOutcome build_suite_run(int index);
TestOutcome mod_suite_run(int index);
TestOutcome core_suite_run(int index);
TestOutcome model_suite_run(int index);
TestOutcome shell_suite_run(int index);

#endif
