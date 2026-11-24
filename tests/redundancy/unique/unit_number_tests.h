#ifndef UNIT_NUMBER_TESTS_H
#define UNIT_NUMBER_TESTS_H

#include "../../test_outcome.h"
#include "../dat/game_number_tests.h"

enum {
    UNIT_NUMBER_OWN_COUNT = 18,
    UNIT_NUMBER_TECH_COUNT = 9,
    UNIT_NUMBER_EFFECT_COUNT = 4,
    UNIT_NUMBER_TEST_COUNT =
        UNIT_NUMBER_OWN_COUNT + UNIT_NUMBER_TECH_COUNT + UNIT_NUMBER_EFFECT_COUNT + GAME_NUMBER_TEST_COUNT
};

TestOutcome unit_number_tests_run(int index);

#endif
