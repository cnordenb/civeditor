#include "recipe_tests.h"

#include <string.h>

#include "small_dat.h"
#include "../../src/core/check.h"
#include "../../src/dat/dat_parts.h"

static long unit_name(Scenario scenario)
{
    RebuiltGame rebuilt = recipe_game_rebuilt(scenario);
    DatCursor cursor;
    size_t at = 0U;

    if ((rebuilt.bytes == NULL) || !dat_find_unit(rebuilt.bytes, rebuilt.map->civ_at[SLOT], OWN_UNIT, &at) ||
        !dat_cursor_start(&cursor, rebuilt.bytes, at + DAT_UNIT_NAME_AT)) {
        return DAT_NO_ID;
    }
    return dat_cursor_i32(&cursor);
}

static long civ_resource(Scenario scenario, int resource)
{
    RebuiltGame rebuilt = recipe_game_rebuilt(scenario);
    DatCursor cursor;
    DatCiv civ;
    float value;

    if (!CHECK(resource >= 0) || !CHECK(resource < GAME_RESOURCES) || (rebuilt.bytes == NULL) ||
        !dat_civ_read(rebuilt.bytes, rebuilt.map->civ_at[SLOT], &civ)) {
        return DAT_NO_ID;
    }
    if (!dat_cursor_start(&cursor, rebuilt.bytes,
                          civ.team_bonus_id_at + DAT_SHORT_WIDTH + ((size_t)resource * DAT_LONG_WIDTH))) {
        return DAT_NO_ID;
    }
    value = dat_cursor_f32(&cursor);
    return (long)value;
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
    return unit_name(FULL_CIV) == UNIT_SID;
}

bool test_a_civ_bonus_made_of_a_civs_tech_is_copied_for_the_civ(void)
{
    return recipe_game_tech(NEW_BONUS_TECH).civ == SLOT;
}

bool test_a_copied_techs_effect_keeps_the_amounts_of_its_changes(void)
{
    return (int)recipe_game_tech_last(NEW_BONUS_TECH).d == 5;
}

bool test_a_copied_effect_names_the_copy_of_a_copied_tech(void)
{
    return recipe_game_changes(NEW_BONUS_TECH, DAT_CHANGE_TECH_COST, NEW_ELITE_TECH) == 1;
}

bool test_a_civ_bonus_made_of_a_generic_tech_unlocks_it(void)
{
    return recipe_game_tree_changes(FULL_CIV, RECIPE_UNLOCK_TECH, GENERIC_TECH) == 1;
}

bool test_the_report_counts_the_civ_bonuses_built(void)
{
    return recipe_game_report(FULL_CIV).bonuses_built == BUILT_BONUSES;
}

bool test_a_bonus_with_nothing_to_build_is_counted_as_skipped(void)
{
    return recipe_game_report(EMPTY_CIV).bonuses_skipped == 1;
}

bool test_a_bonus_requirement_is_a_tech_of_the_civ(void)
{
    return recipe_game_tech_civ(recipe_game_required_tech(REQUIRED_CIV, BONUS_TECH, FREE_SLOT)) == SLOT;
}

bool test_two_techs_given_one_requirement_share_its_copy(void)
{
    return recipe_game_required_tech(REQUIRED_CIV, GENERIC_TECH, 0) ==
           recipe_game_required_tech(REQUIRED_CIV, BONUS_TECH, FREE_SLOT);
}

bool test_a_bonus_of_requirements_is_counted_as_built(void)
{
    return recipe_game_report(REQUIRED_CIV).bonuses_built == 1;
}

bool test_a_bonus_whose_tech_has_no_room_is_skipped(void)
{
    return recipe_game_report(REQUIRED_CIV).bonuses_skipped == 1;
}

bool test_a_second_build_keeps_no_requirement_of_the_first(void)
{
    (void)recipe_game_build(REQUIRED_CIV);
    return recipe_game_required_tech(BARE_CIV, GENERIC_TECH, 0) == -1;
}

bool test_a_tech_that_no_line_names_keeps_its_requirements(void)
{
    return recipe_game_required_tech(REQUIRED_CIV, FIRST_TECH, 0) == -1;
}

bool test_the_report_counts_a_multiplier_of_a_bonus_that_names_a_generic_tech(void)
{
    return recipe_game_report(MULTIPLIED_BARE_CIV).multipliers_skipped == 1;
}

bool test_the_team_bonus_takes_the_changes_of_its_effect(void)
{
    return recipe_game_team_last(FULL_CIV).c == TEAM_ATTRIBUTE;
}

bool test_the_team_bonus_names_the_games_own_techs(void)
{
    return recipe_game_team_changes(DAT_CHANGE_TECH_COST, OWN_ELITE_TECH) == 1;
}

bool test_a_team_bonus_taken_twice_doubles_the_changes_of_its_effect(void)
{
    return (int)recipe_game_team_last(MULTIPLIED_CIV).d == 2;
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

bool test_a_team_bonus_with_nothing_to_build_is_counted_as_skipped(void)
{
    return recipe_game_report(EMPTY_CIV).team_bonuses_skipped == 1;
}

bool test_a_skipped_team_bonus_is_not_counted_as_built(void)
{
    return recipe_game_report(EMPTY_CIV).team_bonuses_built == 0;
}

bool test_a_built_team_bonus_is_not_counted_as_skipped(void)
{
    return recipe_game_report(FULL_CIV).team_bonuses_skipped == 0;
}

bool test_a_team_bonus_of_changes_taken_twice_doubles_them(void)
{
    return (int)recipe_game_team_last(MULTIPLIED_BARE_CIV).d == 6;
}

bool test_a_bonus_of_techs_builds_its_lines_too(void)
{
    return recipe_game_scenario_changes(MULTIPLIED_BARE_CIV, NEW_CHANGES_TECH, ADD_ATTRIBUTE, -1) == 1;
}

bool test_the_lines_of_a_bonus_of_techs_count_as_often_as_the_bonus(void)
{
    float amount = recipe_game_scenario_amount(MULTIPLIED_BARE_CIV, NEW_CHANGES_TECH);

    return (amount > 17.5F) && (amount < 18.5F);
}

bool test_a_named_hero_takes_the_string_id_of_its_slot(void)
{
    return unit_name(BARE_CIV) == HERO_SID;
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
    return civ_resource(SCOUT_CIV, SCOUT_RESOURCE) == (long)SCOUT;
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

bool test_a_unique_unit_not_built_counts_no_techs(void)
{
    return recipe_game_report(BARE_CIV).unit_techs_built == 0;
}

enum { NO_ID = -1, EARLY_TECH = 20, LATE_TECH = 30, UNLOCKED_TECH = 40 };

static RecipeChanges tree_changes;

static void add_tree_change(int type, int a, float d)
{
    DatChange change = { type, NO_ID, NO_ID, NO_ID, 0.0F };

    change.a = a;
    change.d = d;
    recipe_add_change(&tree_changes, &change);
}

bool test_the_trees_changes_are_sorted_unlocks_first_then_by_tech(void)
{
    tree_changes.count = 0;
    add_tree_change(RECIPE_DISABLE_TECH, NO_ID, (float)LATE_TECH);
    add_tree_change(RECIPE_UNLOCK_TECH, UNLOCKED_TECH, 0.0F);
    add_tree_change(RECIPE_DISABLE_TECH, NO_ID, (float)EARLY_TECH);
    recipe_sort_changes(&tree_changes);
    return (tree_changes.changes[0].a == UNLOCKED_TECH) && ((int)tree_changes.changes[1].d == EARLY_TECH) &&
           ((int)tree_changes.changes[2].d == LATE_TECH);
}

bool test_two_disabled_techs_are_two_changes_of_the_tree(void)
{
    tree_changes.count = 0;
    add_tree_change(RECIPE_DISABLE_TECH, NO_ID, (float)EARLY_TECH);
    add_tree_change(RECIPE_DISABLE_TECH, NO_ID, (float)LATE_TECH);
    return tree_changes.count == 2;
}
