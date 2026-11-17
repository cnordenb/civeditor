#include "suites.h"

#include "../bonus/scale_tests.h"
#include "../dat/recipe_tests.h"
#include "../look/look_tests.h"

enum {
    RECIPE_UNIT_END = 8,
    RECIPE_BONUS_END = 22,
    RECIPE_TEAM_END = 26,
    RECIPE_CHANGE_END = 34,
    RECIPE_BARE_END = 46,
    RECIPE_PLAN_END = 55,
    LOOK_END = 67
};

static TestOutcome run_recipe_unit_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the replaced civ loses its own techs", test_the_replaced_civ_loses_its_own_techs());
    case 2:
        return test_outcome_of("the unique unit is made by a tech of the civ",
                               test_the_unique_unit_is_made_by_a_tech_of_the_civ());
    case 3:
        return test_outcome_of("a unique tech is researched at the castle in its age",
                               test_a_unique_tech_is_researched_at_the_castle_in_its_age());
    case 4:
        return test_outcome_of("a built unique tech changes the civ's own unit",
                               test_a_built_unique_tech_changes_the_civs_own_unit());
    case 5:
        return test_outcome_of("a chosen cost takes the place of the tech's own",
                               test_a_chosen_cost_takes_the_place_of_the_techs_own());
    case 6:
        return test_outcome_of("the built tech tree disables the tech of a unit the civ lacks",
                               test_the_built_tech_tree_disables_the_tech_of_a_unit_the_civ_lacks());
    case 7:
        return test_outcome_of("the report counts the unique techs built",
                               test_the_report_counts_the_unique_techs_built());
    case 8:
        return test_outcome_of("the built unique unit is named by the string id of its slot",
                               test_the_built_unique_unit_is_named_by_the_string_id_of_its_slot());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_recipe_bonus_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a civ bonus made of a civ's tech is copied for the civ",
                               test_a_civ_bonus_made_of_a_civs_tech_is_copied_for_the_civ());
    case 2:
        return test_outcome_of("a copied tech's effect keeps the amounts of its changes",
                               test_a_copied_techs_effect_keeps_the_amounts_of_its_changes());
    case 3:
        return test_outcome_of("a copied effect names the copy of a copied tech",
                               test_a_copied_effect_names_the_copy_of_a_copied_tech());
    case 4:
        return test_outcome_of("a civ bonus made of a generic tech unlocks it",
                               test_a_civ_bonus_made_of_a_generic_tech_unlocks_it());
    case 5:
        return test_outcome_of("the report counts the civ bonuses built",
                               test_the_report_counts_the_civ_bonuses_built());
    case 6:
        return test_outcome_of("the report counts a civ bonus the catalog lacks as skipped",
                               test_the_report_counts_a_civ_bonus_the_catalog_lacks_as_skipped());
    case 7:
        return test_outcome_of("a bonus with nothing to build is counted as skipped",
                               test_a_bonus_with_nothing_to_build_is_counted_as_skipped());
    case 8:
        return test_outcome_of("the report counts a multiplier of a bonus made of the game's techs",
                               test_the_report_counts_a_multiplier_of_a_bonus_made_of_the_games_techs());
    case 9:
        return test_outcome_of("a bonus requirement is a tech of the civ",
                               test_a_bonus_requirement_is_a_tech_of_the_civ());
    case 10:
        return test_outcome_of("two techs given one requirement share its copy",
                               test_two_techs_given_one_requirement_share_its_copy());
    case 11:
        return test_outcome_of("a bonus of requirements is counted as built",
                               test_a_bonus_of_requirements_is_counted_as_built());
    case 12:
        return test_outcome_of("a bonus whose tech has no room is skipped",
                               test_a_bonus_whose_tech_has_no_room_is_skipped());
    case 13:
        return test_outcome_of("a second build keeps no requirement of the first",
                               test_a_second_build_keeps_no_requirement_of_the_first());
    case 14:
        return test_outcome_of("a tech that no line names keeps its requirements",
                               test_a_tech_that_no_line_names_keeps_its_requirements());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_recipe_team_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the team bonus takes the changes of its effect",
                               test_the_team_bonus_takes_the_changes_of_its_effect());
    case 2:
        return test_outcome_of("the team bonus names the game's own techs",
                               test_the_team_bonus_names_the_games_own_techs());
    case 3:
        return test_outcome_of("a team bonus taken twice doubles the changes of its effect",
                               test_a_team_bonus_taken_twice_doubles_the_changes_of_its_effect());
    case 4:
        return test_outcome_of("the elite unit's tech takes the string id of its slot",
                               test_the_elite_units_tech_takes_the_string_id_of_its_slot());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_recipe_change_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a unique tech makes no change twice for the civ's unit",
                               test_a_unique_tech_makes_no_change_twice_for_the_civs_unit());
    case 2:
        return test_outcome_of("a unique tech that enabled the old unit enables the civ's own",
                               test_a_unique_tech_that_enabled_the_old_unit_enables_the_civs_own());
    case 3:
        return test_outcome_of("a unique tech no longer upgrades the old unit",
                               test_a_unique_tech_no_longer_upgrades_the_old_unit());
    case 4:
        return test_outcome_of("a unit that a unique tech spawns becomes the civ's elite unit",
                               test_a_unit_that_a_unique_tech_spawns_becomes_the_civs_elite_unit());
    case 5:
        return test_outcome_of("the tech tree unlocks before it disables",
                               test_the_tech_tree_unlocks_before_it_disables());
    case 6:
        return test_outcome_of("a tech of resources that is researched nowhere stays",
                               test_a_tech_of_resources_that_is_researched_nowhere_stays());
    case 7:
        return test_outcome_of("the tree's changes are sorted, unlocks first, then by tech",
                               test_the_trees_changes_are_sorted_unlocks_first_then_by_tech());
    case 8:
        return test_outcome_of("two disabled techs are two changes of the tree",
                               test_two_disabled_techs_are_two_changes_of_the_tree());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_recipe_bare_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the report says that the unique unit was skipped",
                               test_the_report_says_that_the_unique_unit_was_skipped());
    case 2:
        return test_outcome_of("the report counts the unique techs the catalog lacks",
                               test_the_report_counts_the_unique_techs_the_catalog_lacks());
    case 3:
        return test_outcome_of("a hero is hidden in the tech tree", test_a_hero_is_hidden_in_the_tech_tree());
    case 4:
        return test_outcome_of("an empty tree disables no tech", test_an_empty_tree_disables_no_tech());
    case 5:
        return test_outcome_of("a team bonus without an effect is built from its changes",
                               test_a_team_bonus_without_an_effect_is_built_from_its_changes());
    case 6:
        return test_outcome_of("a team bonus of changes taken twice doubles them",
                               test_a_team_bonus_of_changes_taken_twice_doubles_them());
    case 7:
        return test_outcome_of("a bonus of techs builds its lines too", test_a_bonus_of_techs_builds_its_lines_too());
    case 8:
        return test_outcome_of("the lines of a bonus of techs count once",
                               test_the_lines_of_a_bonus_of_techs_count_once());
    case 9:
        return test_outcome_of("a team bonus with nothing to build is counted as skipped",
                               test_a_team_bonus_with_nothing_to_build_is_counted_as_skipped());
    case 10:
        return test_outcome_of("a skipped team bonus is not counted as built",
                               test_a_skipped_team_bonus_is_not_counted_as_built());
    case 11:
        return test_outcome_of("a built team bonus is not counted as skipped",
                               test_a_built_team_bonus_is_not_counted_as_skipped());
    case 12:
        return test_outcome_of("a named hero takes the string id of its slot",
                               test_a_named_hero_takes_the_string_id_of_its_slot());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_recipe_plan_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a copied tech requires the copy of a civ's tech it required",
                               test_a_copied_tech_requires_the_copy_of_a_civs_tech_it_required());
    case 2:
        return test_outcome_of("a copied tech still requires a tech of no civ",
                               test_a_copied_tech_still_requires_a_tech_of_no_civ());
    case 3:
        return test_outcome_of("a chosen scout is written as the civ's scout",
                               test_a_chosen_scout_is_written_as_the_civs_scout());
    case 4:
        return test_outcome_of("a hero that the replaced civ lacks is skipped and reported",
                               test_a_hero_that_the_replaced_civ_lacks_is_skipped_and_reported());
    case 5:
        return test_outcome_of("a dat file with sounds graphics tasks and a tech tree is written back",
                               test_a_dat_file_with_sounds_graphics_tasks_and_a_tech_tree_is_written_back());
    case 6:
        return test_outcome_of("the tech that makes the unique unit belongs to the civ",
                               test_the_tech_that_makes_the_unique_unit_belongs_to_the_civ());
    case 7:
        return test_outcome_of("a unique tech asks for the age of its tier",
                               test_a_unique_tech_asks_for_the_age_of_its_tier());
    case 8:
        return test_outcome_of("the report counts the techs that the tree disables",
                               test_the_report_counts_the_techs_that_the_tree_disables());
    case 9:
        return test_outcome_of("a unique unit not built counts no techs",
                               test_a_unique_unit_not_built_counts_no_techs());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_look_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a warship takes the look of its architecture's civ",
                               test_a_warship_takes_the_look_of_its_architectures_civ());
    case 2:
        return test_outcome_of("a trade cart's load takes the look of its architecture's civ",
                               test_a_trade_carts_load_takes_the_look_of_its_architectures_civ());
    case 3:
        return test_outcome_of("a monk's healing takes the look of its monk skin",
                               test_a_monks_healing_takes_the_look_of_its_monk_skin());
    case 4:
        return test_outcome_of("a castle takes the look of the civ chosen for the castle",
                               test_a_castle_takes_the_look_of_the_civ_chosen_for_the_castle());
    case 5:
        return test_outcome_of("a soldier keeps the look of the replaced civ",
                               test_a_soldier_keeps_the_look_of_the_replaced_civ());
    case 6:
        return test_outcome_of("the look of an architecture is the civ that the catalog names",
                               test_the_look_of_an_architecture_is_the_civ_that_the_catalog_names());
    case 7:
        return test_outcome_of("a look line of a chosen bonus gives its unit the look of its civ",
                               test_a_look_line_of_a_chosen_bonus_gives_its_unit_the_look_of_its_civ());
    case 8:
        return test_outcome_of("a look line of a bonus not chosen leaves its unit its look",
                               test_a_look_line_of_a_bonus_not_chosen_leaves_its_unit_its_look());
    case 9:
        return test_outcome_of("a look line gives its unit the attack timing of its civ",
                               test_a_look_line_gives_its_unit_the_attack_timing_of_its_civ());
    case 10:
        return test_outcome_of("a look line gives its unit the attack graphic of its civ",
                               test_a_look_line_gives_its_unit_the_attack_graphic_of_its_civ());
    case 11:
        return test_outcome_of("a look line gives its unit the walking graphic of its civ",
                               test_a_look_line_gives_its_unit_the_walking_graphic_of_its_civ());
    case 12:
        return test_outcome_of("a look line gives its unit the icon of its civ",
                               test_a_look_line_gives_its_unit_the_icon_of_its_civ());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_bonus_scale_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("an added amount taken three times is tripled",
                               test_an_added_amount_taken_three_times_is_tripled());
    case 2:
        return test_outcome_of("a multiplied attribute taken twice is squared",
                               test_a_multiplied_attribute_taken_twice_is_squared());
    case 3:
        return test_outcome_of("a multiplied resource taken twice is squared",
                               test_a_multiplied_resource_taken_twice_is_squared());
    case 4:
        return test_outcome_of("a tech cost multiplied in mode two taken twice is squared",
                               test_a_tech_cost_multiplied_in_mode_two_taken_twice_is_squared());
    case 5:
        return test_outcome_of("a tech time multiplied in mode two taken twice is squared",
                               test_a_tech_time_multiplied_in_mode_two_taken_twice_is_squared());
    case 6:
        return test_outcome_of("a tech cost added in mode one is kept", test_a_tech_cost_added_in_mode_one_is_kept());
    case 7:
        return test_outcome_of("a spawn taken twice makes twice the units",
                               test_a_spawn_taken_twice_makes_twice_the_units());
    case 8:
        return test_outcome_of("a resource taken twice is doubled", test_a_resource_taken_twice_is_doubled());
    case 9:
        return test_outcome_of("the spawn limit is kept", test_the_spawn_limit_is_kept());
    case 10:
        return test_outcome_of("the effect function is kept", test_the_effect_function_is_kept());
    case 11:
        return test_outcome_of("a set attribute is kept", test_a_set_attribute_is_kept());
    case 12:
        return test_outcome_of("an armor added past 255 is split in two",
                               test_an_armor_added_past_255_is_split_in_two());
    case 13:
        return test_outcome_of("an armor taken away keeps its sign", test_an_armor_taken_away_keeps_its_sign());
    case 14:
        return test_outcome_of("an attack multiplied past 255 percent is split in two",
                               test_an_attack_multiplied_past_255_percent_is_split_in_two());
    case 15:
        return test_outcome_of("a multiplied attack halfway is rounded to even",
                               test_a_multiplied_attack_halfway_is_rounded_to_even());
    case 16:
        return test_outcome_of("a multiplied attack past a half is rounded up",
                               test_a_multiplied_attack_past_a_half_is_rounded_up());
    case 17:
        return test_outcome_of("a multiplied attack below a half is rounded down",
                               test_a_multiplied_attack_below_a_half_is_rounded_down());
    case 18:
        return test_outcome_of("an armor added to 255 is one change", test_an_armor_added_to_255_is_one_change());
    default:
        return test_outcome_missing();
    }
}

TestOutcome build_suite_run(int index)
{
    if (index <= RECIPE_UNIT_END) {
        return run_recipe_unit_test(index);
    }
    if (index <= RECIPE_BONUS_END) {
        return run_recipe_bonus_test(index - RECIPE_UNIT_END);
    }
    if (index <= RECIPE_TEAM_END) {
        return run_recipe_team_test(index - RECIPE_BONUS_END);
    }
    if (index <= RECIPE_CHANGE_END) {
        return run_recipe_change_test(index - RECIPE_TEAM_END);
    }
    if (index <= RECIPE_BARE_END) {
        return run_recipe_bare_test(index - RECIPE_CHANGE_END);
    }
    if (index <= RECIPE_PLAN_END) {
        return run_recipe_plan_test(index - RECIPE_BARE_END);
    }
    if (index <= LOOK_END) {
        return run_look_test(index - RECIPE_PLAN_END);
    }
    return run_bonus_scale_test(index - LOOK_END);
}
