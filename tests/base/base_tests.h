#ifndef BASE_TESTS_H
#define BASE_TESTS_H

#include <stdbool.h>

#include "../../src/model/civ_design.h"

bool test_choosing_another_civ_takes_away_a_regional_node(void);
bool test_a_base_civ_has_the_nodes_its_civ_has_in_the_game(void);
bool test_a_base_civ_gives_the_civ_exactly_its_bonuses(void);
bool test_a_base_civ_gives_the_civ_its_team_bonus(void);
bool test_a_base_civ_gives_the_civ_its_unique_unit(void);

bool base_tests_apply(int base_id);
const CivDesign *base_tests_design(void);

#endif
