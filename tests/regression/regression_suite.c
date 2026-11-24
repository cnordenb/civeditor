#include "../test_outcome.h"

#include "../../src/base/base_civ.h"
#include "../../src/core/check.h"
#include "../../src/model/history.h"
#include "../../src/present/binding.h"
#include "design_regression.h"
#include "mod_regression.h"
#include "window_regression.h"

enum { BRITONS = 1, ARCHER = 4, ARBALESTER = 492 };
enum { FIRST_TEST_COUNT = 9, LATER_TEST_COUNT = 9, WINDOW_TEST_COUNT = 12 };

static Catalog catalog;
static History history;

static bool britons_started(void)
{
    static CivDesign britons;

    history_start(&history);
    return CHECK(catalog_load(&catalog, L"data")) && CHECK(base_civ_apply(&britons, &catalog, BRITONS)) &&
           history_replace(&history, &britons);
}

static bool click_unit(int unit_id)
{
    ListItem item = { CONTROL_TREE_LIST, CATALOG_MISSING };
    Command command;

    item.item = catalog_find(&catalog, CATALOG_TREE, (int)TREE_UNITS, unit_id);
    if (!CHECK(item.item != CATALOG_MISSING)) {
        return false;
    }
    command = list_item_command(&catalog, &history.present, item);
    return history_do(&history, &command);
}

static bool test_ticking_the_arbalester_ticks_the_archer(void)
{
    return britons_started() && click_unit(ARCHER) &&
           CHECK(!civ_design_tree_has(&history.present, TREE_UNITS, ARCHER)) && click_unit(ARBALESTER) &&
           civ_design_tree_has(&history.present, TREE_UNITS, ARCHER);
}

static bool test_unticking_the_archer_unticks_the_arbalester(void)
{
    return britons_started() && CHECK(civ_design_tree_has(&history.present, TREE_UNITS, ARCHER)) &&
           click_unit(ARCHER) && !civ_design_tree_has(&history.present, TREE_UNITS, ARBALESTER);
}

static TestOutcome run_first_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("ticking the arbalester ticks the archer",
                               test_ticking_the_arbalester_ticks_the_archer());
    case 2:
        return test_outcome_of("unticking the archer unticks the arbalester",
                               test_unticking_the_archer_unticks_the_arbalester());
    case 3:
        return test_outcome_of("a bonus of the game's techs taken twice doubles the changes of its copy",
                               test_a_bonus_of_the_games_techs_taken_twice_doubles_the_changes_of_its_copy());
    case 4:
        return test_outcome_of("the original's example civ keeps its language",
                               test_original_example_civ_keeps_its_language());
    case 5:
        return test_outcome_of("the summary labels the civ's name", test_summary_labels_the_name());
    case 6:
        return test_outcome_of("the last step shows no Next button", test_the_last_step_shows_no_next_button());
    case 7:
        return test_outcome_of("the civ descriptions are written in id order",
                               test_the_civ_descriptions_are_written_in_id_order());
    case 8:
        return test_outcome_of("a skipped hero's name is not written", test_a_skipped_heros_name_is_not_written());
    case 9:
        return test_outcome_of("the civ list points at the civ's own description",
                               test_the_civ_list_points_at_the_civs_own_description());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_later_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a new civ starts with the tech tree of its civ",
                               test_a_new_civ_starts_with_the_tech_tree_of_its_civ());
    case 2:
        return test_outcome_of("a building the civ lacks is not available in its tech tree",
                               test_a_building_the_civ_lacks_is_not_available_in_its_tech_tree());
    case 3:
        return test_outcome_of("a building the civ has is available in its tech tree",
                               test_a_building_the_civ_has_is_available_in_its_tech_tree());
    case 4:
        return test_outcome_of("the catalog holds every look line of its file",
                               test_the_catalog_holds_every_look_line_of_its_file());
    case 5:
        return test_outcome_of("a tree that swaps a node of the base civ is changed",
                               test_a_tree_that_swaps_a_node_of_the_base_civ_is_changed());
    case 6:
        return test_outcome_of("bonuses that swap one of the base civ are changed",
                               test_bonuses_that_swap_one_of_the_base_civ_are_changed());
    case 7:
        return test_outcome_of("a team bonus other than the base civ's is a change",
                               test_a_team_bonus_other_than_the_base_civs_is_a_change());
    case 8:
        return test_outcome_of("the base civ's own team bonus is no change",
                               test_the_base_civs_own_team_bonus_is_no_change());
    case 9:
        return test_outcome_of("a list joined by commas is scaled as one",
                               test_a_list_joined_by_commas_is_scaled_as_one());
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
        return test_outcome_of("a cost cut by half and slid to twice cuts it by three quarters",
                               test_a_cost_cut_by_half_slid_to_twice_cuts_it_by_three_quarters());
    case 2:
        return test_outcome_of("no step is shown as done when the window opens",
                               test_no_step_is_shown_as_done_when_the_window_opens());
    case 3:
        return test_outcome_of("the exit command closes a window without changes",
                               test_the_exit_command_closes_a_window_without_changes());
    case 4:
        return test_outcome_of("a list's scroll bar shows its arrows while the mouse is away",
                               test_a_lists_scroll_bar_shows_its_arrows_while_the_mouse_is_away());
    case 5:
        return test_outcome_of("the window's scroll bar shows its arrows while the mouse is away",
                               test_the_windows_scroll_bar_shows_its_arrows_while_the_mouse_is_away());
    case 6:
        return test_outcome_of("a step ends a margin from the window's scroll bar",
                               test_a_step_ends_a_margin_from_the_windows_scroll_bar());
    case 7:
        return test_outcome_of("each wheel notch moves a list one row its way",
                               test_each_wheel_notch_moves_a_list_one_row_its_way());
    case 8:
        return test_outcome_of("a press on either arrow moves a list one row its way",
                               test_a_press_on_either_arrow_moves_a_list_one_row_its_way());
    case 9:
        return test_outcome_of("a shown list scrolls without animation", test_a_shown_list_scrolls_without_animation());
    case 10:
        return test_outcome_of("a held arrow keeps scrolling a list", test_a_held_arrow_keeps_scrolling_a_list());
    case 11:
        return test_outcome_of("a press between a list's arrows moves it a page",
                               test_a_press_between_a_lists_arrows_moves_it_a_page());
    case 12:
        return test_outcome_of("each wheel notch moves an open dropdown one row",
                               test_each_wheel_notch_moves_an_open_dropdown_one_row());
    default:
        return test_outcome_missing();
    }
}

TestOutcome regression_suite_run(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= FIRST_TEST_COUNT + LATER_TEST_COUNT + WINDOW_TEST_COUNT + 1)) {
        return test_outcome_missing();
    }
    if (index <= FIRST_TEST_COUNT) {
        return run_first_test(index);
    }
    if (index <= FIRST_TEST_COUNT + LATER_TEST_COUNT) {
        return run_later_test(index - FIRST_TEST_COUNT);
    }
    return run_window_test(index - FIRST_TEST_COUNT - LATER_TEST_COUNT);
}
