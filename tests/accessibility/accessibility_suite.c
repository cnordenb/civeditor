#include "../test_outcome.h"

#include "../../src/core/check.h"
#include "access_tests.h"
#include "../reflow/reflow_tests.h"

enum { LAYOUT_TEST_COUNT = 6, SCROLL_START = 13, PLACE_START = 19, WINDOW_START = 23, READER_START = 31 };
enum { NARROW_START = 39 };

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

static TestOutcome run_keyboard_test(int index)
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
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_reader_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("Windows is told that the window's language is English",
                               test_windows_is_told_the_language_is_english());
    case 2:
        return test_outcome_of("the help key says what the control in focus is for",
                               test_help_is_given_for_the_control_in_focus());
    case 3:
        return test_outcome_of("the status line is a live region that a screen reader is told of",
                               test_the_status_line_is_told_to_a_screen_reader());
    case 4:
        return test_outcome_of("the text and the layout grow with the text size of Windows, up to 225%",
                               test_the_layout_grows_with_the_text_size_of_windows());
    case 5:
        return test_outcome_of("the glossary explains every shortened form in capitals of the catalog",
                               test_the_glossary_explains_every_short_form());
    case 6:
        return test_outcome_of("the control in focus has a solid frame at least two pixels thick",
                               test_the_control_in_focus_has_a_solid_frame());
    case 7:
        return test_outcome_of("a ticked row is drawn with a check, not a filled box",
                               test_a_ticked_row_is_drawn_with_a_check());
    case 8:
        return test_outcome_of("after an undo, the base civ shown is the design's again",
                               test_an_undone_choice_shows_the_designs_value_again());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_reflow_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a window narrower than the wide layout reflows",
                               test_a_window_narrower_than_the_wide_layout_reflows());
    case 2:
        return test_outcome_of("a window shorter than the wide layout reflows",
                               test_a_window_shorter_than_the_wide_layout_reflows());
    case 3:
        return test_outcome_of("a window as large as the wide layout does not reflow",
                               test_a_window_as_large_as_the_wide_layout_does_not_reflow());
    case 4:
        return test_outcome_of("no control reaches past a window 320 wide",
                               test_no_control_reaches_past_a_window_320_wide());
    case 5:
        return test_outcome_of("no two controls overlap in a window 320 wide",
                               test_no_two_controls_overlap_in_a_window_320_wide());
    case 6:
        return test_outcome_of("every control one acts on is at least 44 by 44 in a window 320 wide",
                               test_every_target_is_44_by_44_in_a_window_320_wide());
    case 7:
        return test_outcome_of("a step is as high as its last control and a margin",
                               test_a_step_is_as_high_as_its_last_control_and_a_margin());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_scroll_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a control below the window is scrolled up into it",
                               test_a_control_below_the_window_is_scrolled_up_into_it());
    case 2:
        return test_outcome_of("a control above the window is scrolled down into it",
                               test_a_control_above_the_window_is_scrolled_down_into_it());
    case 3:
        return test_outcome_of("a control in the window leaves it where it is",
                               test_a_control_in_the_window_leaves_it_where_it_is());
    case 4:
        return test_outcome_of("the window scrolls no further than its content",
                               test_the_window_scrolls_no_further_than_its_content());
    case 5:
        return test_outcome_of("the window scrolls no higher than its top",
                               test_the_window_scrolls_no_higher_than_its_top());
    case 6:
        return test_outcome_of("a window that holds its content does not scroll",
                               test_a_window_that_holds_its_content_does_not_scroll());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_narrow_window_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a narrow window places its controls in one column",
                               test_a_narrow_window_places_its_controls_in_one_column());
    case 2:
        return test_outcome_of("the control in focus is scrolled into a short window",
                               test_the_control_in_focus_is_scrolled_into_a_short_window());
    case 3:
        return test_outcome_of("the window may be as small as 320 by 256",
                               test_the_window_may_be_as_small_as_320_by_256());
    case 4:
        return test_outcome_of("Scroll down moves a short window down a page",
                               test_scroll_down_moves_a_short_window_down_a_page());
    case 5:
        return test_outcome_of("Scroll up takes back a Scroll down", test_scroll_up_takes_back_a_scroll_down());
    case 6:
        return test_outcome_of("the wheel scrolls a short window", test_the_wheel_scrolls_a_short_window());
    case 7:
        return test_outcome_of("a long row of a narrow list is higher than a short one",
                               test_a_long_row_of_a_narrow_list_is_higher_than_a_short_one());
    case 8:
        return test_outcome_of("a short row of a list is as high as a target",
                               test_a_short_row_of_a_list_is_as_high_as_a_target());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_place_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("the first control of a step comes under the help",
                               test_the_first_control_of_a_step_comes_under_the_help());
    case 2:
        return test_outcome_of("Back and Next come under the status line",
                               test_back_and_next_come_under_the_status_line());
    case 3:
        return test_outcome_of("a control starts a margin from the left",
                               test_a_control_starts_a_margin_from_the_left());
    case 4:
        return test_outcome_of("Next ends a margin from the right", test_next_ends_a_margin_from_the_right());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_plain_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= WINDOW_START)) {
        return test_outcome_missing();
    }
    if (index <= LAYOUT_TEST_COUNT) {
        return run_layout_test(index);
    }
    if (index <= SCROLL_START) {
        return run_reflow_test(index - LAYOUT_TEST_COUNT);
    }
    if (index <= PLACE_START) {
        return run_scroll_test(index - SCROLL_START);
    }
    return run_place_test(index - PLACE_START);
}

TestOutcome accessibility_suite_run(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    if (index <= WINDOW_START) {
        return run_plain_test(index);
    }
    if (test_window_is_left_out()) {
        return test_outcome_missing();
    }
    if (index <= READER_START) {
        return run_keyboard_test(index - WINDOW_START);
    }
    if (index <= NARROW_START) {
        return run_reader_test(index - READER_START);
    }
    return run_narrow_window_test(index - NARROW_START);
}
