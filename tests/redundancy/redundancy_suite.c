#include "../test_outcome.h"

#include "../../src/core/check.h"
#include "civ_tests.h"
#include "dat/patch_tests.h"
#include "mod/built_mod_tests.h"
#include "game_tests.h"
#include "review_tests.h"
#include "present/filter_tests.h"
#include "tools/plan_tests.h"

enum { CIV_GROUP_SIZE = 7, PICK_GROUP_SIZE = 7, SAVE_GROUP_SIZE = 4, ROSTER_GROUP_SIZE = 10, COMPARE_GROUP_SIZE = 3,
       GAME_GROUP_SIZE = 7, CUT_GROUP_SIZE = 4, PLAN_GROUP_SIZE = 1, PATCH_GROUP_SIZE = 1, BUILT_GROUP_SIZE = 7,
       ROOM_GROUP_SIZE = 2, REVIEW_GROUP_SIZE = 6, BUILD_GROUP_SIZE = 8 };
enum {
    PICK_START = CIV_GROUP_SIZE,
    SAVE_START = PICK_START + PICK_GROUP_SIZE,
    ROSTER_START = SAVE_START + SAVE_GROUP_SIZE,
    COMPARE_START = ROSTER_START + ROSTER_GROUP_SIZE,
    GAME_START = COMPARE_START + COMPARE_GROUP_SIZE,
    CUT_START = GAME_START + GAME_GROUP_SIZE,
    PLAN_START = CUT_START + CUT_GROUP_SIZE,
    PATCH_START = PLAN_START + PLAN_GROUP_SIZE,
    BUILT_START = PATCH_START + PATCH_GROUP_SIZE,
    ROOM_START = BUILT_START + BUILT_GROUP_SIZE,
    FILTER_START = ROOM_START + ROOM_GROUP_SIZE,
    REVIEW_START = FILTER_START + FILTER_TEST_COUNT,
    BUILD_START = REVIEW_START + REVIEW_GROUP_SIZE,
    OPENED_START = BUILD_START + BUILD_GROUP_SIZE
};

static TestOutcome run_civ_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a civ of the game gives the design its name",
                               test_a_civ_of_the_game_gives_the_design_its_name());
    case 2:
        return test_outcome_of("a civ of the game names its unique unit as the game does",
                               test_a_civ_of_the_game_names_its_unique_unit_as_the_game_does());
    case 3:
        return test_outcome_of("a civ of the game names its Castle Age tech as the game does",
                               test_a_civ_of_the_game_names_its_castle_tech_as_the_game_does());
    case 4:
        return test_outcome_of("a civ of the game names its Imperial Age tech as the game does",
                               test_a_civ_of_the_game_names_its_imperial_tech_as_the_game_does());
    case 5:
        return test_outcome_of("a civ of the game gives the design its tagline",
                               test_a_civ_of_the_game_gives_the_design_its_tagline());
    case 6:
        return test_outcome_of("a civ of the game gives the design its description",
                               test_a_civ_of_the_game_gives_the_design_its_description());
    case 7:
        return test_outcome_of("a civ of the game takes away the hero of the civ before",
                               test_a_civ_of_the_game_takes_away_the_hero_of_the_civ_before());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_pick_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("picking a unique unit names it as the game does",
                               test_picking_a_unique_unit_names_it_as_the_game_does());
    case 2:
        return test_outcome_of("picking a Castle Age tech names it as the game does",
                               test_picking_a_castle_tech_names_it_as_the_game_does());
    case 3:
        return test_outcome_of("picking a hero names it as the game does",
                               test_picking_a_hero_names_it_as_the_game_does());
    case 4:
        return test_outcome_of("unticking Hero takes away the hero's name",
                               test_unticking_hero_takes_away_the_heros_name());
    case 5:
        return test_outcome_of("a hero whose name is emptied lacks a name",
                               test_a_hero_whose_name_is_emptied_lacks_a_name());
    case 6:
        return test_outcome_of("a civ of the game shows no step done", test_a_civ_of_the_game_shows_no_step_done());
    case 7:
        return test_outcome_of("Finish is done once a whole civ differs from the game's",
                               test_finish_is_done_once_a_whole_civ_differs_from_the_games());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_save_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a changed civ is counted", test_a_changed_civ_is_counted());
    case 2:
        return test_outcome_of("a civ as the game has it is left out of the mod",
                               test_a_civ_as_the_game_has_it_is_left_out_of_the_mod());
    case 3:
        return test_outcome_of("a civ changed again holds its newer edits in the mod",
                               test_a_civ_changed_again_holds_its_newer_edits_in_the_mod());
    case 4:
        return test_outcome_of("a civ made as the game has it again is taken out of the mod",
                               test_a_civ_made_as_the_game_has_it_again_is_taken_out_of_the_mod());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_roster_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the draft of a civ is kept while another is edited",
                               test_the_draft_of_a_civ_is_kept_while_another_is_edited());
    case 2:
        return test_outcome_of("a changed civ is marked as changed", test_a_changed_civ_is_marked_as_changed());
    case 3:
        return test_outcome_of("a civ as the game has it is not marked as changed",
                               test_a_civ_as_the_game_has_it_is_not_marked_as_changed());
    case 4:
        return test_outcome_of("the civs of the mod are given in the order of their slots",
                               test_the_civs_of_the_mod_are_given_in_the_order_of_their_slots());
    case 5:
        return test_outcome_of("a roster started anew holds no draft", test_a_roster_started_anew_holds_no_draft());
    case 6:
        return test_outcome_of("a civ past the last slot is left out of the mod",
                               test_a_civ_past_the_last_slot_is_left_out_of_the_mod());
    case 7:
        return test_outcome_of("a changed civ in the first slot is counted",
                               test_a_changed_civ_in_the_first_slot_is_counted());
    case 8:
        return test_outcome_of("a changed civ in the first slot is given to the mod",
                               test_a_changed_civ_in_the_first_slot_is_given_to_the_mod());
    case 9:
        return test_outcome_of("a changed civ in the first slot is marked as changed",
                               test_a_changed_civ_in_the_first_slot_is_marked_as_changed());
    case 10:
        return test_outcome_of("a roster started anew forgets a changed civ in the first slot",
                               test_a_roster_started_anew_forgets_a_changed_civ_in_the_first_slot());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_compare_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a civ of another first look differs from its civ of the game",
                               test_a_civ_of_another_first_look_differs_from_its_civ_of_the_game());
    case 2:
        return test_outcome_of("a civ of another first cost differs from its civ of the game",
                               test_a_civ_of_another_first_cost_differs_from_its_civ_of_the_game());
    case 3:
        return test_outcome_of("a civ given a bonus of the civ before differs from its civ of the game",
                               test_a_civ_given_a_bonus_of_the_civ_before_differs_from_its_civ_of_the_game());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_game_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the game gives a civ its tagline", test_the_game_gives_a_civ_its_tagline());
    case 2:
        return test_outcome_of("the description of a civ starts after its tagline",
                               test_the_description_of_a_civ_starts_after_its_tagline());
    case 3:
        return test_outcome_of("a line break of the game breaks the description",
                               test_a_line_break_of_the_game_breaks_the_description());
    case 4:
        return test_outcome_of("a quote of the game is kept in the description",
                               test_a_quote_of_the_game_is_kept_in_the_description());
    case 5:
        return test_outcome_of("a civ the game does not describe keeps no tagline",
                               test_a_civ_the_game_does_not_describe_keeps_no_tagline());
    case 6:
        return test_outcome_of("texts are read from a game that is found",
                               test_texts_are_read_from_a_game_that_is_found());
    case 7:
        return test_outcome_of("a civ the game does not describe keeps no description",
                               test_a_civ_the_game_does_not_describe_keeps_no_description());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_cut_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a text without its closing quote ends with its line",
                               test_a_text_without_its_closing_quote_ends_with_its_line());
    case 2:
        return test_outcome_of("the spaces and breaks at the end of a text are cut",
                               test_the_spaces_and_breaks_at_the_end_of_a_text_are_cut());
    case 3:
        return test_outcome_of("a first line without civilization is the whole tagline",
                               test_a_first_line_without_civilization_is_the_whole_tagline());
    case 4:
        return test_outcome_of("a tagline longer than its room is cut to fit",
                               test_a_tagline_longer_than_its_room_is_cut_to_fit());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_review_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the window opens with the Britons to edit",
                               test_the_window_opens_with_the_britons_to_edit());
    case 2:
        return test_outcome_of("the Name field holds the name of the civ to edit",
                               test_the_name_field_holds_the_name_of_the_civ_to_edit());
    case 3:
        return test_outcome_of("another civ keeps the edits of the civ before",
                               test_another_civ_keeps_the_edits_of_the_civ_before());
    case 4:
        return test_outcome_of("the review counts no civ changed at first",
                               test_the_review_counts_no_civ_changed_at_first());
    case 5:
        return test_outcome_of("Finish counts a civ that was changed",
                               test_finish_counts_a_civ_that_was_changed());
    case 6:
        return test_outcome_of("Finish counts no civ that was changed back",
                               test_finish_counts_no_civ_that_was_changed_back());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_plan_test(int index)
{
    if (index == 1) {
        return test_outcome_of("the analyzed build compiles with the analyzer",
                               test_the_analyzed_build_compiles_with_the_analyzer());
    }
    return test_outcome_missing();
}

static TestOutcome run_patch_test(int index)
{
    if (index == 1) {
        return test_outcome_of("an edit knows the first place it patched",
                               test_an_edit_knows_the_first_place_it_patched());
    }
    return test_outcome_missing();
}

static TestOutcome run_built_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a job with no civ is not finished", test_a_job_with_no_civ_is_not_finished());
    case 2:
        return test_outcome_of("a build of an unfinished civ fails", test_a_build_of_an_unfinished_civ_fails());
    case 3:
        return test_outcome_of("the description of a civ takes the place of the game's once",
                               test_the_description_of_a_civ_takes_the_place_of_the_games_once());
    case 4:
        return test_outcome_of("the other descriptions of the game are kept",
                               test_the_other_descriptions_of_the_game_are_kept());
    case 5:
        return test_outcome_of("a civ past the descriptions of the game gets its own",
                               test_a_civ_past_the_descriptions_of_the_game_gets_its_own());
    case 6:
        return test_outcome_of("a civ left from a bigger build gets no description",
                               test_a_civ_left_from_a_bigger_build_gets_no_description());
    case 7:
        return test_outcome_of("the description of the first civ described is written once",
                               test_the_description_of_the_first_civ_described_is_written_once());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_room_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a tech takes no more requirements than it has room for",
                               test_a_tech_takes_no_more_requirements_than_it_has_room_for());
    case 2:
        return test_outcome_of("the strings of a civ fail once its description passes its room",
                               test_the_strings_of_a_civ_fail_once_its_description_passes_its_room());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_build_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the review text shows no caret when clicked",
                               test_the_review_text_shows_no_caret_when_clicked());
    case 2:
        return test_outcome_of("a build with no civ changed says so", test_a_build_with_no_civ_changed_says_so());
    case 3:
        return test_outcome_of("a mod is named civeditor_mod unless named otherwise",
                               test_a_mod_is_named_civeditor_mod_unless_named_otherwise());
    case 4:
        return test_outcome_of("a filter leaves the bonuses that hold it and no other",
                               test_a_filter_leaves_the_bonuses_that_hold_it_and_no_other());
    case 5:
        return test_outcome_of("the categories together hold every bonus once",
                               test_the_categories_together_hold_every_bonus_once());
    case 6:
        return test_outcome_of("an empty mod name builds no mod", test_an_empty_mod_name_builds_no_mod());
    case 7:
        return test_outcome_of("a long civ name is cut in the name of its file",
                               test_a_long_civ_name_is_cut_in_the_name_of_its_file());
    case 8:
        return test_outcome_of("a text field shows its caret when it takes the focus",
                               test_a_text_field_shows_its_caret_when_it_takes_the_focus());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_opened_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the window opens with the focus on the civ to edit",
                               test_the_window_opens_with_the_focus_on_the_civ_to_edit());
    case 2:
        return test_outcome_of("the menu Go names each step as its title reads",
                               test_the_menu_go_names_each_step_as_its_title_reads());
    case 3:
        return test_outcome_of("a wheel turn on the minimized window leaves the step unscrolled",
                               test_a_wheel_turn_on_the_minimized_window_leaves_the_step_unscrolled());
    case 4:
        return test_outcome_of("Choose hero lists every hero of the catalog once",
                               test_choose_hero_lists_every_hero_of_the_catalog_once());
    case 5:
        return test_outcome_of("the list of Choose hero is labelled Heroes",
                               test_the_list_of_choose_hero_is_labelled_heroes());
    case 6:
        return test_outcome_of("Choose hero opens with the focus on its list",
                               test_choose_hero_opens_with_the_focus_on_its_list());
    case 7:
        return test_outcome_of("a name typed in Choose hero stays when its list takes the focus again",
                               test_a_name_typed_in_choose_hero_stays_when_its_list_takes_the_focus_again());
    case 8:
        return test_outcome_of("the first hero of Choose hero can be picked",
                               test_the_first_hero_of_choose_hero_can_be_picked());
    case 9:
        return test_outcome_of("a one-letter name typed in Choose hero is taken",
                               test_a_one_letter_name_typed_in_choose_hero_is_taken());
    case 10:
        return test_outcome_of("a civ named with one letter is saved under that letter",
                               test_a_civ_named_with_one_letter_is_saved_under_that_letter());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_window_test(int index)
{
    if (test_window_is_left_out()) {
        return test_outcome_missing();
    }
    if (index <= BUILD_START) {
        return run_review_test(index - REVIEW_START);
    }
    if (index <= OPENED_START) {
        return run_build_test(index - BUILD_START);
    }
    return run_opened_test(index - OPENED_START);
}

TestOutcome redundancy_suite_run(int index)
{
    if (!CHECK(index > 0)) {
        return test_outcome_missing();
    }
    if (index <= PICK_START) {
        return run_civ_test(index);
    }
    if (index <= SAVE_START) {
        return run_pick_test(index - PICK_START);
    }
    if (index <= ROSTER_START) {
        return run_save_test(index - SAVE_START);
    }
    if (index <= COMPARE_START) {
        return run_roster_test(index - ROSTER_START);
    }
    if (index <= GAME_START) {
        return run_compare_test(index - COMPARE_START);
    }
    if (index <= CUT_START) {
        return run_game_test(index - GAME_START);
    }
    if (index <= PLAN_START) {
        return run_cut_test(index - CUT_START);
    }
    if (index <= PATCH_START) {
        return run_plan_test(index - PLAN_START);
    }
    if (index <= BUILT_START) {
        return run_patch_test(index - PATCH_START);
    }
    if (index <= ROOM_START) {
        return run_built_test(index - BUILT_START);
    }
    if (index <= FILTER_START) {
        return run_room_test(index - ROOM_START);
    }
    return (index <= REVIEW_START) ? filter_tests_run(index - FILTER_START) : run_window_test(index);
}
