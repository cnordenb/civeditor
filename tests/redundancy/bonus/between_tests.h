#ifndef BETWEEN_TESTS_H
#define BETWEEN_TESTS_H

#include "../../test_outcome.h"
#include "number_tests.h"
#include "scaled_text_tests.h"

enum { BETWEEN_TEST_COUNT = 15, BONUS_TEST_COUNT = SCALED_TEXT_TEST_COUNT + BETWEEN_TEST_COUNT + NUMBER_TEST_COUNT };

TestOutcome bonus_tests_run(int index);

#endif
