#include "suites.h"

#include "../shell/dialog_tests.h"

enum {
    SAVE_END = 5,
    VIEW_END = 9,
    CLOSE_END = 12,
    BUILD_END = 16,
    DRAW_END = 19,
    LOOP_END = 20,
    BAR_TEST_COUNT = 7
};

static TestOutcome run_save_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a new civ is saved as civbuilder civ by default",
                               test_a_new_civ_is_saved_as_civbuilder_civ_by_default());
    case 2:
        return test_outcome_of("a named civ is saved under its name", test_a_named_civ_is_saved_under_its_name());
    case 3:
        return test_outcome_of("a save that is cancelled says that the civ was not saved",
                               test_a_save_that_is_cancelled_says_that_the_civ_was_not_saved());
    case 4:
        return test_outcome_of("a saved civ is opened again", test_a_saved_civ_is_opened_again());
    case 5:
        return test_outcome_of("a file that is no civ file is refused with a message",
                               test_a_file_that_is_no_civ_file_is_refused_with_a_message());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_view_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the next button goes to the next step", test_the_next_button_goes_to_the_next_step());
    case 2:
        return test_outcome_of("a typed name is shown in the title of the window",
                               test_a_typed_name_is_shown_in_the_title_of_the_window());
    case 3:
        return test_outcome_of("redo says when there is nothing to redo",
                               test_redo_says_when_there_is_nothing_to_redo());
    case 4:
        return test_outcome_of("no step is shown as done when the window opens",
                               test_no_step_is_shown_as_done_when_the_window_opens());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_close_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("closing is stopped when the question of saving is cancelled",
                               test_closing_is_stopped_when_the_question_of_saving_is_cancelled());
    case 2:
        return test_outcome_of("closing without saving closes the window",
                               test_closing_without_saving_closes_the_window());
    case 3:
        return test_outcome_of("the exit command closes a window without changes",
                               test_the_exit_command_closes_a_window_without_changes());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_build_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("build is refused when the game is not found",
                               test_build_is_refused_when_the_game_is_not_found());
    case 2:
        return test_outcome_of("build writes nothing when the question is not confirmed",
                               test_build_writes_nothing_when_the_question_is_not_confirmed());
    case 3:
        return test_outcome_of("build of an unfinished civ says that it is not finished",
                               test_build_of_an_unfinished_civ_says_that_it_is_not_finished());
    case 4:
        return test_outcome_of("build from the window reports the mod it built",
                               test_build_from_the_window_reports_the_mod_it_built());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_draw_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the help line is written in the muted colour",
                               test_the_help_line_is_written_in_the_muted_colour());
    case 2:
        return test_outcome_of("a field is written on the colour of fields",
                               test_a_field_is_written_on_the_colour_of_fields());
    case 3:
        return test_outcome_of("a row in focus is drawn with a frame in the focus colour",
                               test_a_row_in_focus_is_drawn_with_a_frame_in_the_focus_colour());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_loop_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("picking the picked row again makes no new change",
                               test_picking_the_picked_row_again_makes_no_new_change());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_list_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("each wheel notch moves a list one row its way",
                               test_each_wheel_notch_moves_a_list_one_row_its_way());
    case 2:
        return test_outcome_of("a press on either arrow moves a list one row its way",
                               test_a_press_on_either_arrow_moves_a_list_one_row_its_way());
    case 3:
        return test_outcome_of("a shown list scrolls without animation", test_a_shown_list_scrolls_without_animation());
    case 4:
        return test_outcome_of("a held arrow keeps scrolling a list", test_a_held_arrow_keeps_scrolling_a_list());
    case 5:
        return test_outcome_of("a press between a list's arrows moves it a page",
                               test_a_press_between_a_lists_arrows_moves_it_a_page());
    case 6:
        return test_outcome_of("a key in a list moves its caret one row",
                               test_a_key_in_a_list_moves_its_caret_one_row());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_scroll_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a list's scroll bar shows its arrows while the mouse is away",
                               test_a_lists_scroll_bar_shows_its_arrows_while_the_mouse_is_away());
    case 2:
        return test_outcome_of("the window's scroll bar shows its arrows while the mouse is away",
                               test_the_windows_scroll_bar_shows_its_arrows_while_the_mouse_is_away());
    case 3:
        return test_outcome_of("a step ends a margin from the window's scroll bar",
                               test_a_step_ends_a_margin_from_the_windows_scroll_bar());
    case 4:
        return test_outcome_of("the knob of the window's scroll bar reaches its end with the step",
                               test_the_knob_of_the_windows_scroll_bar_reaches_its_end_with_the_step());
    case 5:
        return test_outcome_of("a step that just fits the window shows no scroll bar",
                               test_a_step_that_just_fits_the_window_shows_no_scroll_bar());
    case 6:
        return test_outcome_of("the up arrow of the window's scroll bar moves the step up a line",
                               test_the_up_arrow_of_the_windows_scroll_bar_moves_the_step_up_a_line());
    case 7:
        return test_outcome_of("a wheel turn between the lists redraws nothing when the step fits",
                               test_a_wheel_turn_between_the_lists_redraws_nothing_when_the_step_fits());
    default:
        return run_list_test(index - BAR_TEST_COUNT);
    }
}

TestOutcome shell_suite_run(int index)
{
    if (index <= SAVE_END) {
        return run_save_test(index);
    }
    if (index <= VIEW_END) {
        return run_view_test(index - SAVE_END);
    }
    if (index <= CLOSE_END) {
        return run_close_test(index - VIEW_END);
    }
    if (index <= BUILD_END) {
        return run_build_test(index - CLOSE_END);
    }
    if (index <= DRAW_END) {
        return run_draw_test(index - BUILD_END);
    }
    if (index <= LOOP_END) {
        return run_loop_test(index - DRAW_END);
    }
    return run_scroll_test(index - LOOP_END);
}
