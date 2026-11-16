#include "suites.h"

#include "../../src/core/check.h"
#include "../dat/recipe_tests.h"
#include "../look/look_tests.h"

enum {
    RECIPE_UNIT_END = 8,
    RECIPE_BONUS_END = 14,
    RECIPE_CHANGE_END = 20,
    RECIPE_BARE_END = 26,
    RECIPE_PLAN_END = 34
};

static TestOutcome run_recipe_unit_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
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
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a civ bonus made of a civ's tech is copied for the civ",
                               test_a_civ_bonus_made_of_a_civs_tech_is_copied_for_the_civ());
    case 2:
        return test_outcome_of("a civ bonus made of a generic tech unlocks it",
                               test_a_civ_bonus_made_of_a_generic_tech_unlocks_it());
    case 3:
        return test_outcome_of("the report counts the civ bonuses built",
                               test_the_report_counts_the_civ_bonuses_built());
    case 4:
        return test_outcome_of("the report counts a civ bonus the catalog lacks as skipped",
                               test_the_report_counts_a_civ_bonus_the_catalog_lacks_as_skipped());
    case 5:
        return test_outcome_of("the team bonus takes the changes of its effect",
                               test_the_team_bonus_takes_the_changes_of_its_effect());
    case 6:
        return test_outcome_of("the elite unit's tech takes the string id of its slot",
                               test_the_elite_units_tech_takes_the_string_id_of_its_slot());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_recipe_change_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
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
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_recipe_bare_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
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
        return test_outcome_of("a named hero takes the string id of its slot",
                               test_a_named_hero_takes_the_string_id_of_its_slot());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_recipe_plan_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
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
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_look_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
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
    default:
        return test_outcome_missing();
    }
}

TestOutcome build_suite_run(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    if (index <= RECIPE_UNIT_END) {
        return run_recipe_unit_test(index);
    }
    if (index <= RECIPE_BONUS_END) {
        return run_recipe_bonus_test(index - RECIPE_UNIT_END);
    }
    if (index <= RECIPE_CHANGE_END) {
        return run_recipe_change_test(index - RECIPE_BONUS_END);
    }
    if (index <= RECIPE_BARE_END) {
        return run_recipe_bare_test(index - RECIPE_CHANGE_END);
    }
    if (index <= RECIPE_PLAN_END) {
        return run_recipe_plan_test(index - RECIPE_BARE_END);
    }
    return run_look_test(index - RECIPE_PLAN_END);
}
