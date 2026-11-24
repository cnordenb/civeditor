#include "suites.h"

#include "../shell/dialog_tests.h"

enum {
    SAVE_END = 4,
    VIEW_END = 7,
    CLOSE_END = 9,
    BUILD_END = 13,
    DRAW_END = 16,
    LOOP_END = 17,
    BAR_TEST_COUNT = 3,
    LIST_TEST_COUNT = 3,
    HERO_TEST_COUNT = 4,
    CHANGE_TEST_COUNT = 12,
    MEASURE_TEST_COUNT = 6,
    CIV_TEST_COUNT = 7
};

static TestOutcome run_save_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a named civ is saved under its name", test_a_named_civ_is_saved_under_its_name());
    case 2:
        return test_outcome_of("a save that is cancelled says that the civ was not saved",
                               test_a_save_that_is_cancelled_says_that_the_civ_was_not_saved());
    case 3:
        return test_outcome_of("a saved civ is opened again", test_a_saved_civ_is_opened_again());
    case 4:
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
        return test_outcome_of("build writes nothing when the mod is not named",
                               test_build_writes_nothing_when_the_mod_is_not_named());
    case 3:
        return test_outcome_of("build of an unfinished civ says that it is not finished",
                               test_build_of_an_unfinished_civ_says_that_it_is_not_finished());
    case 4:
        return test_outcome_of("a build that left something out says what in a message",
                               test_a_build_that_left_something_out_says_what_in_a_message());
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

static TestOutcome run_filter_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the filter of the team bonuses leaves the rows that hold its text",
                               test_the_filter_of_the_team_bonuses_leaves_the_rows_that_hold_its_text());
    case 2:
        return test_outcome_of("a civ's name in the filter of the units gives its unique unit",
                               test_a_civs_name_in_the_filter_of_the_units_gives_its_unique_unit());
    case 3:
        return test_outcome_of("a civ's name in the filter of the castle techs gives its castle tech",
                               test_a_civs_name_in_the_filter_of_the_castle_techs_gives_its_castle_tech());
    case 4:
        return test_outcome_of("a civ's name in the filter of the imperial techs gives its imperial tech",
                               test_a_civs_name_in_the_filter_of_the_imperial_techs_gives_its_imperial_tech());
    case 5:
        return test_outcome_of("the filter of Choose hero leaves the heroes that hold its text",
                               test_the_filter_of_choose_hero_leaves_the_heroes_that_hold_its_text());
    case 6:
        return test_outcome_of("Clear leaves the civ without a civ bonus",
                               test_clear_leaves_the_civ_without_a_civ_bonus());
    case 7:
        return test_outcome_of("a civ's name in the filter of the bonuses gives its civ bonuses",
                               test_a_civs_name_in_the_filter_of_the_bonuses_gives_its_civ_bonuses());
    case 8:
        return test_outcome_of("a civ's name in the filter of the tech tree gives its nodes",
                               test_a_civs_name_in_the_filter_of_the_tech_tree_gives_its_nodes());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_civ_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("Appearance opens the popup of the look choices",
                               test_appearance_opens_the_popup_of_the_look_choices());
    case 2:
        return test_outcome_of("the popup of the look choices is hidden until asked for",
                               test_the_popup_of_the_look_choices_is_hidden_until_asked_for());
    case 3:
        return test_outcome_of("the architecture is chosen in the popup of the look choices",
                               test_the_architecture_is_chosen_in_the_popup_of_the_look_choices());
    case 4:
        return test_outcome_of("a civ outside ranked play is not offered to edit",
                               test_a_civ_outside_ranked_play_is_not_offered_to_edit());
    case 5:
        return test_outcome_of("a changed civ is starred in Civ to edit",
                               test_a_changed_civ_is_starred_in_civ_to_edit());
    case 6:
        return test_outcome_of("a civ changed back loses its star", test_a_civ_changed_back_loses_its_star());
    case 7:
        return test_outcome_of("Finish counts a changed civ without a save",
                               test_finish_counts_a_changed_civ_without_a_save());
    default:
        return run_filter_test(index - CIV_TEST_COUNT);
    }
}

static TestOutcome run_measure_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("Options cancelled leaves the choice of asking to save as it was",
                               test_options_cancelled_leaves_the_choice_of_asking_as_it_was());
    case 2:
        return test_outcome_of("the tick box of a question lies above its buttons",
                               test_the_tick_box_of_a_question_lies_above_its_buttons());
    case 3:
        return test_outcome_of("the rows of a list shown are measured anew when it becomes wider",
                               test_the_rows_of_a_list_shown_are_measured_anew_when_it_becomes_wider());
    case 4:
        return test_outcome_of("a letter typed into the name leaves the tech tree nothing to redraw",
                               test_a_letter_typed_into_the_name_leaves_the_tech_tree_nothing_to_redraw());
    case 5:
        return test_outcome_of("the longest row of the narrow list of team bonuses is higher than the shortest",
                               test_the_longest_row_of_the_narrow_list_of_team_bonuses_is_higher_than_the_shortest());
    case 6:
        return test_outcome_of("a question with a tick box takes an empty text",
                               test_a_question_with_a_tick_box_takes_an_empty_text());
    default:
        return run_civ_test(index - MEASURE_TEST_COUNT);
    }
}

static TestOutcome run_change_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a build is written without a question of its folder",
                               test_a_build_is_written_without_a_question_of_its_folder());
    case 2:
        return test_outcome_of("Options takes a mods folder that exists",
                               test_options_takes_a_mods_folder_that_exists());
    case 3:
        return test_outcome_of("Options refuses a mods folder that does not exist",
                               test_options_refuses_a_mods_folder_that_does_not_exist());
    case 4:
        return test_outcome_of("closing asks nothing of a changed civ when the option says not to",
                               test_closing_asks_nothing_when_the_option_says_not_to());
    case 5:
        return test_outcome_of("Options keeps the choice not to ask to save before exiting",
                               test_options_keeps_the_choice_not_to_ask_to_save());
    case 6:
        return test_outcome_of("Options with the mods folder left as it is says that the options were kept",
                               test_options_with_the_mods_folder_left_says_that_the_options_were_kept());
    case 7:
        return test_outcome_of("a changed civ is asked to be saved when no option is kept",
                               test_a_changed_civ_is_asked_to_be_saved_when_no_option_is_kept());
    case 8:
        return test_outcome_of("a click ticks the tick box of a question",
                               test_a_click_ticks_the_tick_box_of_a_question());
    case 9:
        return test_outcome_of("a click unticks a tick box given as ticked",
                               test_a_click_unticks_a_tick_box_given_as_ticked());
    case 10:
        return test_outcome_of("the window shows the numbers it read from the game's data file",
                               test_the_window_shows_the_numbers_it_read_from_the_games_data_file());
    case 11:
        return test_outcome_of("a civ not yet changed takes its tech tree from the game's files once they are read",
                               test_a_civ_not_yet_changed_takes_its_tech_tree_from_the_games_files());
    case 12:
        return test_outcome_of("a changed civ keeps its tech tree when the game's files are read",
                               test_a_changed_civ_keeps_its_tech_tree_when_the_games_files_are_read());
    default:
        return run_measure_test(index - CHANGE_TEST_COUNT);
    }
}

static TestOutcome run_hero_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("ticking Hero names Attila the Hun above Choose hero",
                               test_ticking_hero_names_attila_the_hun_above_choose_hero());
    case 2:
        return test_outcome_of("unticking Hero hides Choose hero", test_unticking_hero_hides_choose_hero());
    case 3:
        return test_outcome_of("a hero picked in Choose hero keeps the game's name",
                               test_a_hero_picked_in_choose_hero_keeps_the_games_name());
    case 4:
        return test_outcome_of("a hero renamed in Choose hero takes the new name",
                               test_a_hero_renamed_in_choose_hero_takes_the_new_name());
    default:
        return run_change_test(index - HERO_TEST_COUNT);
    }
}

static TestOutcome run_list_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a key in a list moves its caret one row",
                               test_a_key_in_a_list_moves_its_caret_one_row());
    case 2:
        return test_outcome_of("the room under the last row of a list is painted in the colour of fields",
                               test_the_room_under_the_last_row_of_a_list_is_painted_in_the_colour_of_fields());
    case 3:
        return test_outcome_of("a list full of rows paints nothing under itself",
                               test_a_list_full_of_rows_paints_nothing_under_itself());
    default:
        return run_hero_test(index - LIST_TEST_COUNT);
    }
}

static TestOutcome run_scroll_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the knob of the window's scroll bar reaches its end with the step",
                               test_the_knob_of_the_windows_scroll_bar_reaches_its_end_with_the_step());
    case 2:
        return test_outcome_of("a step that just fits the window shows no scroll bar",
                               test_a_step_that_just_fits_the_window_shows_no_scroll_bar());
    case 3:
        return test_outcome_of("the up arrow of the window's scroll bar moves the step up a line",
                               test_the_up_arrow_of_the_windows_scroll_bar_moves_the_step_up_a_line());
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
