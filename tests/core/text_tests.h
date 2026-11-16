#ifndef TEXT_TESTS_H
#define TEXT_TESTS_H

#include <stdbool.h>

bool test_a_text_is_not_equal_to_a_longer_one_that_starts_with_it(void);
bool test_a_text_is_not_equal_to_its_own_beginning(void);
bool test_a_text_that_fills_its_place_is_copied(void);
bool test_a_text_one_letter_too_long_for_its_place_is_not_copied(void);
bool test_a_text_that_does_not_fit_is_not_appended(void);
bool test_a_negative_number_is_appended_with_its_minus_sign(void);
bool test_a_capital_z_is_found_by_a_small_z(void);
bool test_a_text_does_not_contain_a_letter_it_lacks(void);

#endif
