#ifndef STAT_TESTS_H
#define STAT_TESTS_H

#include "../../test_outcome.h"
#include "stat_build_tests.h"

enum {
    STAT_UNIT_COUNT = 10,
    STAT_TECH_COUNT = 7,
    STAT_EFFECT_COUNT = 7,
    STAT_TEST_COUNT = STAT_UNIT_COUNT + STAT_TECH_COUNT + STAT_EFFECT_COUNT + STAT_BUILD_TEST_COUNT
};

TestOutcome stat_tests_run(int index);

#endif
