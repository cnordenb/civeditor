#include "text_tests.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"

enum { SMALL_PLACE = 4, NUMBER_PLACE = 16 };

static char place[SMALL_PLACE];

bool test_a_text_is_not_equal_to_a_longer_one_that_starts_with_it(void)
{
    return !text_equal("civ", "civics");
}

bool test_a_text_is_not_equal_to_its_own_beginning(void)
{
    return !text_equal("civics", "civ");
}

bool test_a_text_that_fills_its_place_is_copied(void)
{
    return text_copy(place, sizeof place, "abc") && text_equal(place, "abc");
}

bool test_a_text_one_letter_too_long_for_its_place_is_not_copied(void)
{
    return !text_copy(place, sizeof place, "abcd");
}

bool test_a_text_that_does_not_fit_is_not_appended(void)
{
    (void)CHECK(text_copy(place, sizeof place, "ab"));
    return !text_append(place, sizeof place, "cd");
}

bool test_a_negative_number_is_appended_with_its_minus_sign(void)
{
    char number[NUMBER_PLACE] = "";

    return text_append_number(number, sizeof number, -12L) && text_equal(number, "-12");
}

bool test_a_capital_z_is_found_by_a_small_z(void)
{
    return text_contains_folded("Zulu", "z");
}

bool test_a_text_does_not_contain_a_letter_it_lacks(void)
{
    return !text_contains_folded("civ", "x");
}
