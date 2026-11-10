#include "../test_outcome.h"

#include "../../src/core/check.h"
#include "access_tests.h"

enum { LAYOUT_TEST_COUNT = 6 };

static TestOutcome run_layout_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("every text contrasts at least 7:1 with its background",
                               test_every_text_contrasts_seven_to_one());
    case 2:
        return test_outcome_of("every input has a visible label", test_every_input_has_a_visible_label());
    case 3:
        return test_outcome_of("every control one acts on is at least 44 by 44", test_every_target_is_44_by_44());
    case 4:
        return test_outcome_of("no two controls of a step overlap, even in the smallest window",
                               test_no_two_controls_of_a_step_overlap());
    case 5:
        return test_outcome_of("the window's title names the civ and the step",
                               test_the_window_title_names_the_civ_and_the_step());
    case 6:
        return test_outcome_of("the list of steps is on every step, with Back and Next wherever they lead",
                               test_the_steps_and_their_buttons_are_on_every_step());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_window_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("the Tab key reaches every control of every step",
                               test_tab_reaches_every_control_of_every_step());
    case 2:
        return test_outcome_of("the list of steps shows the current step as the chosen one",
                               test_the_list_of_steps_shows_the_current_step());
    case 3:
        return test_outcome_of("the Tab key follows the order in which a step is read",
                               test_tab_follows_the_reading_order());
    case 4:
        return test_outcome_of("a ticked row leaves the list of bonuses where it was scrolled",
                               test_a_ticked_row_leaves_the_list_where_it_was());
    case 5:
        return test_outcome_of("every control tells a screen reader a name that begins with its label",
                               test_every_control_is_named_by_its_label());
    case 6:
        return test_outcome_of("a ticked row is told to a screen reader as chosen",
                               test_a_ticked_row_is_told_as_chosen());
    case 7:
        return test_outcome_of("moving the focus through every control changes nothing",
                               test_moving_the_focus_changes_nothing());
    case 8:
        return test_outcome_of("the Tab key leaves every control", test_the_tab_key_leaves_every_control());
    case 9:
        return test_outcome_of("Windows is told that the window's language is English",
                               test_windows_is_told_the_language_is_english());
    case 10:
        return test_outcome_of("the help key says what the control in focus is for",
                               test_help_is_given_for_the_control_in_focus());
    case 11:
        return test_outcome_of("the status line is a live region that a screen reader is told of",
                               test_the_status_line_is_told_to_a_screen_reader());
    case 12:
        return test_outcome_of("the text and the layout grow with the text size of Windows, up to 225%",
                               test_the_layout_grows_with_the_text_size_of_windows());
    case 13:
        return test_outcome_of("the glossary explains every shortened form in capitals of the catalog",
                               test_the_glossary_explains_every_short_form());
    case 14:
        return test_outcome_of("the control in focus has a solid frame at least two pixels thick",
                               test_the_control_in_focus_has_a_solid_frame());
    case 15:
        return test_outcome_of("a ticked row is drawn with a check, not a filled box",
                               test_a_ticked_row_is_drawn_with_a_check());
    case 16:
        return test_outcome_of("after an undo, the base civ shown is the design's again",
                               test_an_undone_choice_shows_the_designs_value_again());
    default:
        return test_outcome_missing();
    }
}

TestOutcome accessibility_suite_run(int index)
{
    TestOutcome outcome;

    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    outcome = run_layout_test((index <= LAYOUT_TEST_COUNT) ? index : LAYOUT_TEST_COUNT + 1);
    if (outcome.exists || test_window_is_left_out()) {
        return outcome;
    }
    return run_window_test(index - LAYOUT_TEST_COUNT);
}
