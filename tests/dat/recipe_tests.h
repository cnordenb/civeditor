#ifndef RECIPE_TESTS_H
#define RECIPE_TESTS_H

#include <stdbool.h>

#include "../../src/forge/recipe_parts.h"

enum {
    GAME_CIVS = 2,
    GAME_TECHS = 11,
    GAME_CHANGE_COUNT = 20,
    SLOT = 1,
    OTHER_CIV = 3,
    NO_CIV = -1,
    OLD_UNIT = 5,
    OWN_UNIT = 6,
    ELITE_UNIT = 11,
    KEPT_UNIT = 4,
    LACKED_UNIT = 9,
    INFANTRY_CLASS = 6,
    OLD_PICK = 7,
    OWN_PICK = 8,
    MISSING_PICK = 77,
    PICKED_TECH = 0
};

enum {
    OLD_MAKE_TECH = 1,
    CASTLE_SOURCE = 2,
    IMPERIAL_SOURCE = 3,
    OWN_MAKE_TECH = 4,
    LACKED_TECH = 5,
    BONUS_TECH = 6,
    NOWHERE_TECH = 7,
    OWN_ELITE_TECH = 8,
    TEAM_EFFECT = 9,
    GENERIC_TECH = 10,
    NEW_CHANGES_TECH = 11,
    NEW_BONUS_TECH = 12,
    NEW_MAKE_TECH = 14,
    NEW_ELITE_TECH = 13,
    NEW_CASTLE_TECH = 15,
    NEW_IMPERIAL_TECH = 16
};

enum {
    TECH_BONUS = 50,
    GENERIC_BONUS = 51,
    CHANGES_BONUS = 52,
    EMPTY_BONUS = 53,
    REQUIRE_BONUS = 54,
    NO_ROOM_BONUS = 55,
    MIXED_BONUS = 56,
    FREE_SLOT = 2,
    FIRST_TECH = 0,
    MISSING_BONUS = 99,
    EFFECT_TEAM_BONUS = 60,
    CHANGES_TEAM_BONUS = 61,
    EMPTY_TEAM_BONUS = 62,
    BUILT_BONUSES = 3,
    KEPT_CHANGES = 4
};

enum {
    CASTLE = 82,
    CASTLE_AGE = 102,
    RESOURCE = 1,
    UPGRADE_UNIT = 3,
    ADD_ATTRIBUTE = 4,
    SPAWN_UNIT = 7,
    TEAM_ENABLE_UNIT = 12,
    WOOD = 1,
    CHOSEN_WOOD = 300,
    TEAM_ATTRIBUTE = 5,
    UNIT_SID = 111003,
    ELITE_SID = 111004,
    HERO_SID = 111005,
    SCOUT = 448,
    GAME_RESOURCES = 300,
    SCOUT_RESOURCE = 263,
    NO_TECH = -2,
    ANY_CHANGE = -1
};

typedef enum {
    FULL_CIV,
    BARE_CIV,
    SCOUT_CIV,
    LOST_HERO_CIV,
    MULTIPLIED_CIV,
    MULTIPLIED_BARE_CIV,
    EMPTY_CIV,
    REQUIRED_CIV
} Scenario;

bool recipe_game_build(Scenario scenario);
RecipeReport recipe_game_report(Scenario scenario);
DatTech recipe_game_tech(int id);
int recipe_game_changes(int tech_id, int type, int key);
int recipe_game_scenario_changes(Scenario scenario, int tech_id, int type, int key);
float recipe_game_scenario_amount(Scenario scenario, int tech_id);
int recipe_game_tree_changes(Scenario scenario, int type, int key);
DatChange recipe_game_tree_last(Scenario scenario, int type, int key);
DatChange recipe_game_team_last(Scenario scenario);
DatChange recipe_game_tech_last(int tech_id);
int recipe_game_required_tech(Scenario scenario, int tech_id, int slot);
int recipe_game_tech_civ(int tech_id);
int recipe_game_team_changes(int type, int key);
long recipe_game_unit_name(Scenario scenario);
long recipe_game_resource(Scenario scenario, int resource);
const CivDesign *recipe_game_design(Scenario scenario);
const Catalog *recipe_game_catalog(void);
bool recipe_game_make(DatBytes *game);

bool test_the_replaced_civ_loses_its_own_techs(void);
bool test_the_unique_unit_is_made_by_a_tech_of_the_civ(void);
bool test_a_unique_tech_is_researched_at_the_castle_in_its_age(void);
bool test_a_built_unique_tech_changes_the_civs_own_unit(void);
bool test_a_chosen_cost_takes_the_place_of_the_techs_own(void);
bool test_the_built_tech_tree_disables_the_tech_of_a_unit_the_civ_lacks(void);
bool test_the_report_counts_the_unique_techs_built(void);
bool test_the_built_unique_unit_is_named_by_the_string_id_of_its_slot(void);
bool test_a_civ_bonus_made_of_a_civs_tech_is_copied_for_the_civ(void);
bool test_a_copied_techs_effect_keeps_the_amounts_of_its_changes(void);
bool test_a_copied_effect_names_the_copy_of_a_copied_tech(void);
bool test_a_civ_bonus_made_of_a_generic_tech_unlocks_it(void);
bool test_the_report_counts_the_civ_bonuses_built(void);
bool test_the_report_counts_a_civ_bonus_the_catalog_lacks_as_skipped(void);
bool test_a_bonus_with_nothing_to_build_is_counted_as_skipped(void);
bool test_a_bonus_requirement_is_a_tech_of_the_civ(void);
bool test_two_techs_given_one_requirement_share_its_copy(void);
bool test_a_bonus_of_requirements_is_counted_as_built(void);
bool test_a_bonus_whose_tech_has_no_room_is_skipped(void);
bool test_a_second_build_keeps_no_requirement_of_the_first(void);
bool test_a_tech_that_no_line_names_keeps_its_requirements(void);
bool test_the_report_counts_a_multiplier_of_a_bonus_made_of_the_games_techs(void);
bool test_the_team_bonus_takes_the_changes_of_its_effect(void);
bool test_the_team_bonus_names_the_games_own_techs(void);
bool test_a_team_bonus_taken_twice_doubles_the_changes_of_its_effect(void);
bool test_the_elite_units_tech_takes_the_string_id_of_its_slot(void);
bool test_a_unique_tech_makes_no_change_twice_for_the_civs_unit(void);
bool test_a_unique_tech_that_enabled_the_old_unit_enables_the_civs_own(void);
bool test_a_unique_tech_no_longer_upgrades_the_old_unit(void);
bool test_a_unit_that_a_unique_tech_spawns_becomes_the_civs_elite_unit(void);
bool test_the_tech_tree_unlocks_before_it_disables(void);
bool test_a_tech_of_resources_that_is_researched_nowhere_stays(void);
bool test_the_trees_changes_are_sorted_unlocks_first_then_by_tech(void);
bool test_two_disabled_techs_are_two_changes_of_the_tree(void);
bool test_the_report_says_that_the_unique_unit_was_skipped(void);
bool test_the_report_counts_the_unique_techs_the_catalog_lacks(void);
bool test_a_hero_is_hidden_in_the_tech_tree(void);
bool test_an_empty_tree_disables_no_tech(void);
bool test_a_team_bonus_without_an_effect_is_built_from_its_changes(void);
bool test_a_team_bonus_with_nothing_to_build_is_counted_as_skipped(void);
bool test_a_skipped_team_bonus_is_not_counted_as_built(void);
bool test_a_built_team_bonus_is_not_counted_as_skipped(void);
bool test_a_team_bonus_of_changes_taken_twice_doubles_them(void);
bool test_a_bonus_of_techs_builds_its_lines_too(void);
bool test_the_lines_of_a_bonus_of_techs_count_once(void);
bool test_a_named_hero_takes_the_string_id_of_its_slot(void);
bool test_a_copied_tech_requires_the_copy_of_a_civs_tech_it_required(void);
bool test_a_copied_tech_still_requires_a_tech_of_no_civ(void);
bool test_a_chosen_scout_is_written_as_the_civs_scout(void);
bool test_a_hero_that_the_replaced_civ_lacks_is_skipped_and_reported(void);
bool test_a_dat_file_with_sounds_graphics_tasks_and_a_tech_tree_is_written_back(void);
bool test_the_tech_that_makes_the_unique_unit_belongs_to_the_civ(void);
bool test_a_unique_tech_asks_for_the_age_of_its_tier(void);
bool test_the_report_counts_the_techs_that_the_tree_disables(void);
bool test_a_unique_unit_not_built_counts_no_techs(void);

#endif
