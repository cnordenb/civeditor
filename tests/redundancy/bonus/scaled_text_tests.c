#include "scaled_text_tests.h"

#include <string.h>

#include "../../../src/bonus/bonus_text.h"
#include "../../../src/core/check.h"

enum { TWICE = 2 };

static bool scaled_is(const char *text, int multiplier, const char *expected)
{
    static char scaled[BONUS_TEXT_CAPACITY];

    return CHECK(bonus_text_scaled(text, multiplier, scaled, sizeof scaled)) && (strcmp(scaled, expected) == 0);
}

TestOutcome scaled_text_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("each signed percent of a list that counts twice is scaled",
                               scaled_is("Cavalry +20/+30% hit points", TWICE, "Cavalry +44/+69% hit points"));
    case 2:
        return test_outcome_of("a sign inside a word is no sign",
                               scaled_is("Elite-2 units", TWICE, "Elite-2 units"));
    case 3:
        return test_outcome_of("a number after a space is not in the list before it",
                               scaled_is("+5 10 food", TWICE, "+10 10 food"));
    case 4:
        return test_outcome_of("a comma that no space follows is not in the list before it",
                               scaled_is("+5,10 food", TWICE, "+10,10 food"));
    default:
        return test_outcome_missing();
    }
}
