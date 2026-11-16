#include "recipe_tests.h"

#include <string.h>

#include "small_dat.h"

bool test_the_replaced_civ_loses_its_own_techs(void)
{
    return recipe_game_tech(OLD_MAKE_TECH).civ == RECIPE_LOST_CIV;
}

bool test_the_unique_unit_is_made_by_a_tech_of_the_civ(void)
{
    return recipe_game_changes(NEW_MAKE_TECH, RECIPE_ENABLE_UNIT, OWN_UNIT) == 1;
}

bool test_a_unique_tech_is_researched_at_the_castle_in_its_age(void)
{
    return recipe_game_tech(NEW_CASTLE_TECH).research_locations[0].location_id == CASTLE;
}

bool test_a_built_unique_tech_changes_the_civs_own_unit(void)
{
    return recipe_game_changes(NEW_IMPERIAL_TECH, ADD_ATTRIBUTE, OWN_UNIT) == 1;
}

bool test_a_chosen_cost_takes_the_place_of_the_techs_own(void)
{
    return recipe_game_tech(NEW_CASTLE_TECH).costs[0].amount == CHOSEN_WOOD;
}

bool test_the_built_tech_tree_disables_the_tech_of_a_unit_the_civ_lacks(void)
{
    return recipe_game_tree_changes(FULL_CIV, RECIPE_DISABLE_TECH, LACKED_TECH) == 1;
}

bool test_the_report_counts_the_unique_techs_built(void)
{
    return recipe_game_report(FULL_CIV).unique_techs_built == TECH_TIER_COUNT;
}

bool test_the_built_unique_unit_is_named_by_the_string_id_of_its_slot(void)
{
    return recipe_game_unit_name(FULL_CIV) == UNIT_SID;
}

bool test_a_civ_bonus_made_of_a_civs_tech_is_copied_for_the_civ(void)
{
    return recipe_game_tech(NEW_BONUS_TECH).civ == SLOT;
}

bool test_a_civ_bonus_made_of_a_generic_tech_unlocks_it(void)
{
    return recipe_game_tree_changes(FULL_CIV, RECIPE_UNLOCK_TECH, GENERIC_TECH) == 1;
}

bool test_the_report_counts_the_civ_bonuses_built(void)
{
    return recipe_game_report(FULL_CIV).bonuses_built == BUILT_BONUSES;
}

bool test_the_report_counts_a_civ_bonus_the_catalog_lacks_as_skipped(void)
{
    return recipe_game_report(FULL_CIV).bonuses_skipped == 1;
}

bool test_the_team_bonus_takes_the_changes_of_its_effect(void)
{
    return recipe_game_team_last().c == TEAM_ATTRIBUTE;
}

bool test_the_elite_units_tech_takes_the_string_id_of_its_slot(void)
{
    return recipe_game_tech(NEW_ELITE_TECH).language_dll_name == ELITE_SID;
}

bool test_a_unique_tech_makes_no_change_twice_for_the_civs_unit(void)
{
    return recipe_game_changes(NEW_CASTLE_TECH, ANY_CHANGE, 0) == KEPT_CHANGES;
}

bool test_a_unique_tech_that_enabled_the_old_unit_enables_the_civs_own(void)
{
    return recipe_game_changes(NEW_CASTLE_TECH, RECIPE_ENABLE_UNIT, OWN_UNIT) == 1;
}

bool test_a_unique_tech_no_longer_upgrades_the_old_unit(void)
{
    return recipe_game_changes(NEW_CASTLE_TECH, UPGRADE_UNIT, OLD_UNIT) == 0;
}

bool test_a_unit_that_a_unique_tech_spawns_becomes_the_civs_elite_unit(void)
{
    return recipe_game_changes(NEW_CASTLE_TECH, SPAWN_UNIT, ELITE_UNIT) == 1;
}

bool test_the_tech_tree_unlocks_before_it_disables(void)
{
    return recipe_game_tree_last(FULL_CIV, ANY_CHANGE, 0).type == RECIPE_DISABLE_TECH;
}

bool test_a_tech_of_resources_that_is_researched_nowhere_stays(void)
{
    return recipe_game_tree_changes(FULL_CIV, RECIPE_DISABLE_TECH, NOWHERE_TECH) == 0;
}

bool test_the_report_says_that_the_unique_unit_was_skipped(void)
{
    return recipe_game_report(BARE_CIV).unit_skipped;
}

bool test_the_report_counts_the_unique_techs_the_catalog_lacks(void)
{
    return recipe_game_report(BARE_CIV).unique_techs_skipped == TECH_TIER_COUNT;
}

bool test_a_hero_is_hidden_in_the_tech_tree(void)
{
    return recipe_game_tree_last(BARE_CIV, RECIPE_ENABLE_UNIT, OWN_UNIT).b == 0;
}

bool test_an_empty_tree_disables_no_tech(void)
{
    return recipe_game_report(BARE_CIV).tree_disabled == 0;
}

bool test_a_team_bonus_without_an_effect_is_built_from_its_changes(void)
{
    return recipe_game_report(BARE_CIV).team_bonuses_built == 1;
}

bool test_a_named_hero_takes_the_string_id_of_its_slot(void)
{
    return recipe_game_unit_name(BARE_CIV) == HERO_SID;
}

bool test_a_copied_tech_requires_the_copy_of_a_civs_tech_it_required(void)
{
    return recipe_game_tech(NEW_BONUS_TECH).required_techs[0] == NEW_ELITE_TECH;
}

bool test_a_copied_tech_still_requires_a_tech_of_no_civ(void)
{
    return recipe_game_tech(NEW_BONUS_TECH).required_techs[1] == LACKED_TECH;
}

bool test_a_chosen_scout_is_written_as_the_civs_scout(void)
{
    return recipe_game_resource(SCOUT_CIV, SCOUT_RESOURCE) == (long)SCOUT;
}

bool test_a_hero_that_the_replaced_civ_lacks_is_skipped_and_reported(void)
{
    return recipe_game_report(LOST_HERO_CIV).hero_skipped;
}

bool test_a_dat_file_with_sounds_graphics_tasks_and_a_tech_tree_is_written_back(void)
{
    DatBytes result = { NULL, 0U };
    bool written;

    small.rich = true;
    written = small_dat_start(SMALL_NO_CIVS) && small_dat_write(&result);
    small.rich = false;
    return written && (result.size == small.size) && (memcmp(small.written, small.data, small.size) == 0);
}

bool test_the_tech_that_makes_the_unique_unit_belongs_to_the_civ(void)
{
    return recipe_game_tech(NEW_MAKE_TECH).civ == SLOT;
}

bool test_a_unique_tech_asks_for_the_age_of_its_tier(void)
{
    return recipe_game_tech(NEW_CASTLE_TECH).required_techs[0] == CASTLE_AGE;
}

bool test_the_report_counts_the_techs_that_the_tree_disables(void)
{
    return recipe_game_report(FULL_CIV).tree_disabled == 1;
}
