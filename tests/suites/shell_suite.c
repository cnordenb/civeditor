#include "suites.h"

#include "../../src/core/check.h"
#include "../shell/dialog_tests.h"

enum {
    SAVE_END = 5,
    VIEW_END = 11,
    CLOSE_END = 14,
    BUILD_END = 18,
    DRAW_END = 21
};

static TestOutcome run_save_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
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
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("the glossary is shown with its words", test_the_glossary_is_shown_with_its_words());
    case 2:
        return test_outcome_of("the next button goes to the next step", test_the_next_button_goes_to_the_next_step());
    case 3:
        return test_outcome_of("a filter empties a list that holds no such text",
                               test_a_filter_empties_a_list_that_holds_no_such_text());
    case 4:
        return test_outcome_of("a typed name is shown in the title of the window",
                               test_a_typed_name_is_shown_in_the_title_of_the_window());
    case 5:
        return test_outcome_of("a category narrows the list of civ bonuses",
                               test_a_category_narrows_the_list_of_civ_bonuses());
    case 6:
        return test_outcome_of("redo says when there is nothing to redo",
                               test_redo_says_when_there_is_nothing_to_redo());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_close_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
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
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
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
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
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
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("picking the picked row again makes no new change",
                               test_picking_the_picked_row_again_makes_no_new_change());
    case 2:
        return test_outcome_of("the program ends with the code of the message that ends it",
                               test_the_program_ends_with_the_code_of_the_message_that_ends_it());
    default:
        return test_outcome_missing();
    }
}

TestOutcome shell_suite_run(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
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
    return run_loop_test(index - DRAW_END);
}
