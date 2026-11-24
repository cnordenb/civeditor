#include "number_tests.h"

#include <math.h>
#include <string.h>

#include "../../../src/bonus/bonus_text.h"

static const double CLOSE = 0.000001;

static BonusShown shown;

static bool spans(const char *text, const char *number)
{
    size_t length = strlen(number);

    return bonus_text_show(text, 1.0, &shown) && (shown.count == 1) &&
           (shown.numbers[0].end - shown.numbers[0].start == length) &&
           (strncmp(&shown.text[shown.numbers[0].start], number, length) == 0);
}

static bool shows(const char *text, const char *expected)
{
    return bonus_text_show(text, 1.0, &shown) && (strcmp(shown.text, expected) == 0);
}

static bool typed_gives(BonusNumberKind kind, double base, double value, double times)
{
    BonusNumber number = { kind, base, 0U, 0U };

    return fabs(bonus_number_times(number, value) - times) < CLOSE;
}

static bool holds_no_number(const char *text)
{
    return bonus_text_show(text, 1.0, &shown) && (shown.count == 0);
}

static bool holds_a_full_row(void)
{
    return bonus_text_show("+1 +1 +1 +1 +1 +1 +1 +1 +1 +1 +1 +1 +1 +1 +1 +1 +1 food", 1.0, &shown) &&
           (shown.count == BONUS_NUMBER_CAPACITY);
}

TestOutcome number_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the percent of a chosen bonus is a number to set where it is shown",
                               spans("Cavalry +20% hit points", "20"));
    case 2:
        return test_outcome_of("a percent cut typed gives the count that shows it",
                               typed_gives(BONUS_NUMBER_PERCENT, -0.5, 75.0, 2.0));
    case 3:
        return test_outcome_of("a power typed gives the count that shows it",
                               typed_gives(BONUS_NUMBER_POWER, 2.0, 8.0, 3.0));
    case 4:
        return test_outcome_of("an amount typed gives the count that shows it",
                               typed_gives(BONUS_NUMBER_AMOUNT, 5.0, 7.5, 1.5));
    case 5:
        return test_outcome_of("a chosen bonus without numbers reads as it is",
                               shows("Monks double healing range", "Monks double healing range"));
    case 6:
        return test_outcome_of("a percent without a sign before cheaper is a number to set",
                               spans("Siege Workshop units 15% cheaper", "15"));
    case 7:
        return test_outcome_of("a cut of a hundred percent is not a number to set",
                               holds_no_number("Farms cost -100% wood"));
    case 8:
        return test_outcome_of("a percent of nothing is not a number to set",
                               holds_no_number("Cavalry +0% hit points"));
    case 9:
        return test_outcome_of("an amount of nothing is not a number to set",
                               holds_no_number("Villagers carry +0 food"));
    case 10:
        return test_outcome_of("a power of one is not a number to set", holds_no_number("Monks heal 1x as fast"));
    case 11:
        return test_outcome_of("a row holds sixteen numbers to set at most", holds_a_full_row());
    case 12:
        return test_outcome_of("a percent without a sign before another word is not a number to set",
                               holds_no_number("Loot 25% of the resource cost of each destroyed building"));
    default:
        return test_outcome_missing();
    }
}
