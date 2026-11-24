#ifndef MOD_TESTS_H
#define MOD_TESTS_H

#include <stdbool.h>

#include "../../src/civlist/civ_list.h"
#include "../../src/mod/mod_text.h"

bool test_the_name_is_written_under_the_replaced_civs_name_id(void);
bool test_a_mod_is_not_built_from_an_unfinished_civ(void);
bool test_a_unique_tech_is_aimed_at_the_civs_own_unique_unit(void);
bool test_a_unique_techs_changes_of_one_attribute_by_two_amounts_are_one_target(void);
bool test_a_unique_techs_changes_of_two_armor_classes_are_two_targets(void);
bool test_a_unique_techs_name_is_written_under_its_slots_string_id(void);
bool test_a_unique_units_name_is_written_under_the_string_id_of_its_slot(void);
bool test_a_heros_name_is_written_under_the_string_id_of_its_slot(void);
bool test_the_civ_list_names_the_civs_own_unique_unit(void);
bool test_a_unique_unit_missing_from_the_civ_list_takes_its_own_icon_and_texts(void);
bool test_a_value_the_civ_keeps_stays_the_games_in_the_civ_list(void);
bool test_a_civ_with_an_empty_tree_keeps_the_games_tech_tree(void);
bool test_a_civ_keeps_its_villagers_in_its_tech_tree(void);
bool test_a_building_the_civ_has_keeps_the_research_it_needs_in_its_tech_tree(void);
bool test_a_building_the_replaced_civ_lacked_takes_its_usual_status(void);

ModCiv mod_tests_named_civ(void);
bool mod_tests_write_hero_strings(bool hero_skipped);
const char *mod_tests_written_strings(void);
bool mod_tests_list_written_as(const char *list, const CivListEntry *entry, const char *expected);
bool mod_tests_tree_written_as(const char *tree, int ticked, const char *expected);

#endif
