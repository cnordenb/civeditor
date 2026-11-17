#ifndef CIV_FILE_TESTS_H
#define CIV_FILE_TESTS_H

#include <stdbool.h>

bool test_original_example_civ_keeps_its_name(void);
bool test_original_example_civ_keeps_its_bonuses(void);
bool test_original_example_civ_keeps_its_unique_unit(void);
bool test_original_example_civ_keeps_its_language(void);
bool test_saved_civ_loads_back_unchanged(void);
bool test_a_bonus_multiplier_is_read_back_from_a_civ_file(void);

#endif
