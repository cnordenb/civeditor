#ifndef EDIT_TESTS_H
#define EDIT_TESTS_H

#include "../../test_outcome.h"
#include "number_edit_tests.h"
#include "../unique/stat_tests.h"

enum { EDIT_OWN_COUNT = 5, EDIT_TEST_COUNT = EDIT_OWN_COUNT + NUMBER_EDIT_TEST_COUNT + STAT_TEST_COUNT };

TestOutcome edit_tests_run(int index);

#endif
