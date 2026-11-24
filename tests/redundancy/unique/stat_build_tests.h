#ifndef STAT_BUILD_TESTS_H
#define STAT_BUILD_TESTS_H

#include "../../test_outcome.h"
#include "unit_number_tests.h"

enum { STAT_BUILD_OWN_COUNT = 26, STAT_BUILD_TEST_COUNT = STAT_BUILD_OWN_COUNT + UNIT_NUMBER_TEST_COUNT };

TestOutcome stat_build_tests_run(int index);

#endif
