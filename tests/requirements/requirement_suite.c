#include "../test_outcome.h"

#include "../../src/core/check.h"
#include "requirement_tests.h"

enum { PLAIN_TEST_COUNT = 6, WINDOW_TEST_COUNT = 5 };

static TestOutcome run_plain_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the local mods folder of a profile is found",
                               test_the_local_mods_folder_of_a_profile_is_found());
    case 2:
        return test_outcome_of("undo brings back the civ before a change",
                               test_undo_brings_back_the_civ_before_a_change());
    case 3:
        return test_outcome_of("a 25th bonus is refused", test_a_25th_bonus_is_refused());
    case 4:
        return test_outcome_of("the mod's folder name is made from the mod's name",
                               test_the_mod_folder_name_is_made_from_the_mod_name());
    case 5:
        return test_outcome_of("the replaced civ loses its own techs", test_the_replaced_civ_loses_its_own_techs());
    case 6:
        return test_outcome_of("the report counts a civ bonus the catalog lacks as skipped",
                               test_the_report_counts_a_civ_bonus_the_catalog_lacks_as_skipped());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_window_test(int index)
{
    if (!CHECK(index > 0) || test_window_is_left_out()) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a new civ is saved as civeditor civ by default",
                               test_a_new_civ_is_saved_as_civeditor_civ_by_default());
    case 2:
        return test_outcome_of("a wheel turn between the lists redraws nothing when the step fits",
                               test_a_wheel_turn_between_the_lists_redraws_nothing_when_the_step_fits());
    case 3:
        return test_outcome_of("a dropdown opens without animation", test_a_dropdown_opens_without_animation());
    case 4:
        return test_outcome_of("a number of the team bonus is set in its field",
                               test_a_number_of_the_team_bonus_is_set_in_its_field());
    case 5:
        return test_outcome_of("a list that is erased keeps the picture of its rows",
                               test_a_list_that_is_erased_keeps_the_picture_of_its_rows());
    default:
        return test_outcome_missing();
    }
}

TestOutcome requirement_suite_run(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= PLAIN_TEST_COUNT + WINDOW_TEST_COUNT + 1)) {
        return test_outcome_missing();
    }
    return (index <= PLAIN_TEST_COUNT) ? run_plain_test(index) : run_window_test(index - PLAIN_TEST_COUNT);
}
