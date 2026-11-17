#include "suites.h"

#include "../model/present_tests.h"
#include "../model/wizard_tests.h"
#include "../model/history_tests.h"
#include "../design/design_tests.h"
#include "../design/saved_tests.h"

enum {
    COMMAND_END = 8,
    PICKS_END = 10,
    ROWS_TICK_END = 14,
    ROWS_PICK_END = 20,
    SUMMARY_END = 26,
    REPORT_END = 30,
    STEP_MADE_END = 37,
    STEP_PICKED_END = 45,
    HISTORY_FIRST_END = 51,
    HISTORY_MORE_END = 56,
    DESIGN_END = 64
};

static TestOutcome run_command_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a command to pick a unit picks it", test_a_command_to_pick_a_unit_picks_it());
    case 2:
        return test_outcome_of("a command to pick a tech picks it", test_a_command_to_pick_a_tech_picks_it());
    case 3:
        return test_outcome_of("a command to pick a hero picks it", test_a_command_to_pick_a_hero_picks_it());
    case 4:
        return test_outcome_of("a command to tick a civ bonus ticks it", test_a_command_to_tick_a_civ_bonus_ticks_it());
    case 5:
        return test_outcome_of("a command to tick a team bonus ticks it",
                               test_a_command_to_tick_a_team_bonus_ticks_it());
    case 6:
        return test_outcome_of("a command to tick a node ticks it", test_a_command_to_tick_a_node_ticks_it());
    case 7:
        return test_outcome_of("a command to set a choice sets it", test_a_command_to_set_a_choice_sets_it());
    case 8:
        return test_outcome_of("a command of no kind changes nothing", test_a_command_of_no_kind_changes_nothing());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_picks_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a civ bonus ticked twice is no longer picked",
                               test_a_civ_bonus_ticked_twice_is_no_longer_picked());
    case 2:
        return test_outcome_of("a team bonus ticked twice is no longer picked",
                               test_a_team_bonus_ticked_twice_is_no_longer_picked());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_rows_tick_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a row of the tech tree gives the command to tick its node",
                               test_a_row_of_the_tech_tree_gives_the_command_to_tick_its_node());
    case 2:
        return test_outcome_of("a row of the civ bonuses gives the command to tick its bonus",
                               test_a_row_of_the_civ_bonuses_gives_the_command_to_tick_its_bonus());
    case 3:
        return test_outcome_of("a row of the team bonuses gives the command to tick its bonus",
                               test_a_row_of_the_team_bonuses_gives_the_command_to_tick_its_bonus());
    case 4:
        return test_outcome_of("a row of a look gives the command to set that choice",
                               test_a_row_of_a_look_gives_the_command_to_set_that_choice());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_rows_pick_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a row of the units gives the command to pick its unit",
                               test_a_row_of_the_units_gives_the_command_to_pick_its_unit());
    case 2:
        return test_outcome_of("a row of the heroes gives the command to pick its hero",
                               test_a_row_of_the_heroes_gives_the_command_to_pick_its_hero());
    case 3:
        return test_outcome_of("the row no hero gives the command to pick no hero",
                               test_the_row_no_hero_gives_the_command_to_pick_no_hero());
    case 4:
        return test_outcome_of("a row of the castle techs gives the command to pick its tech",
                               test_a_row_of_the_castle_techs_gives_the_command_to_pick_its_tech());
    case 5:
        return test_outcome_of("a row of the imperial techs gives the command to pick its tech",
                               test_a_row_of_the_imperial_techs_gives_the_command_to_pick_its_tech());
    case 6:
        return test_outcome_of("a row of the castle techs shows its tech's name",
                               test_a_row_of_the_castle_techs_shows_its_techs_name());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_summary_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the summary names the unique unit and the unit it is made from",
                               test_the_summary_names_the_unique_unit_and_the_unit_it_is_made_from());
    case 2:
        return test_outcome_of("the summary lists a civ bonus by its description",
                               test_the_summary_lists_a_civ_bonus_by_its_description());
    case 3:
        return test_outcome_of("the summary names a civ bonus the catalog lacks by its number",
                               test_the_summary_names_a_civ_bonus_the_catalog_lacks_by_its_number());
    case 4:
        return test_outcome_of("the summary names a unique tech by its catalog name",
                               test_the_summary_names_a_unique_tech_by_its_catalog_name());
    case 5:
        return test_outcome_of("the summary says when the catalog lacks the unique unit",
                               test_the_summary_says_when_the_catalog_lacks_the_unique_unit());
    case 6:
        return test_outcome_of("the summary says when the catalog lacks a unique tech",
                               test_the_summary_says_when_the_catalog_lacks_a_unique_tech());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_report_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the build report counts the civ bonuses built and skipped",
                               test_the_build_report_counts_the_civ_bonuses_built_and_skipped());
    case 2:
        return test_outcome_of("the build report says when the hero was not built",
                               test_the_build_report_says_when_the_hero_was_not_built());
    case 3:
        return test_outcome_of("the build report says when a bonus counts only once",
                               test_the_build_report_says_when_a_bonus_counts_only_once());
    case 4:
        return test_outcome_of("the build report of a whole civ names the two folders",
                               test_the_build_report_of_a_whole_civ_names_the_two_folders());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_step_made_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("no step of a new design is done", test_no_step_of_a_new_design_is_done());
    case 2:
        return test_outcome_of("the identity step is done once the civ has a name",
                               test_the_identity_step_is_done_once_the_civ_has_a_name());
    case 3:
        return test_outcome_of("the tech tree step is done once a node is chosen",
                               test_the_tech_tree_step_is_done_once_a_node_is_chosen());
    case 4:
        return test_outcome_of("the bonuses step is done once a bonus is ticked",
                               test_the_bonuses_step_is_done_once_a_bonus_is_ticked());
    case 5:
        return test_outcome_of("the team bonus step is done once one is picked",
                               test_the_team_bonus_step_is_done_once_one_is_picked());
    case 6:
        return test_outcome_of("a tree that swaps a node of the base civ is changed",
                               test_a_tree_that_swaps_a_node_of_the_base_civ_is_changed());
    case 7:
        return test_outcome_of("bonuses that swap one of the base civ are changed",
                               test_bonuses_that_swap_one_of_the_base_civ_are_changed());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_step_picked_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the unique unit step is done once a unit is picked",
                               test_the_unique_unit_step_is_done_once_a_unit_is_picked());
    case 2:
        return test_outcome_of("the castle tech step is done once a tech is picked",
                               test_the_castle_tech_step_is_done_once_a_tech_is_picked());
    case 3:
        return test_outcome_of("the imperial tech step is done once a tech is picked",
                               test_the_imperial_tech_step_is_done_once_a_tech_is_picked());
    case 4:
        return test_outcome_of("the hero step is done once a hero is picked",
                               test_the_hero_step_is_done_once_a_hero_is_picked());
    case 5:
        return test_outcome_of("a pick finishes only its own step", test_a_pick_finishes_only_its_own_step());
    case 6:
        return test_outcome_of("back from the second step goes to the first",
                               test_back_from_the_second_step_goes_to_the_first());
    case 7:
        return test_outcome_of("a team bonus other than the base civ's is a change",
                               test_a_team_bonus_other_than_the_base_civs_is_a_change());
    case 8:
        return test_outcome_of("the base civ's own team bonus is no change",
                               test_the_base_civs_own_team_bonus_is_no_change());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_history_first_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a new design has nothing to undo", test_a_new_design_has_nothing_to_undo());
    case 2:
        return test_outcome_of("a new design has nothing to redo", test_a_new_design_has_nothing_to_redo());
    case 3:
        return test_outcome_of("an undo that fails leaves nothing to redo",
                               test_an_undo_that_fails_leaves_nothing_to_redo());
    case 4:
        return test_outcome_of("a change that is made is reported as made",
                               test_a_change_that_is_made_is_reported_as_made());
    case 5:
        return test_outcome_of("a change after an undo leaves nothing to redo",
                               test_a_change_after_an_undo_leaves_nothing_to_redo());
    case 6:
        return test_outcome_of("undo goes back as many changes as the history holds",
                               test_undo_goes_back_as_many_changes_as_the_history_holds());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_history_more_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("undo past the depth brings back the change before",
                               test_undo_past_the_depth_brings_back_the_change_before());
    case 2:
        return test_outcome_of("a design put in place of another can be undone",
                               test_a_design_put_in_place_of_another_can_be_undone());
    case 3:
        return test_outcome_of("a design put in place of another leaves nothing to redo",
                               test_a_design_put_in_place_of_another_leaves_nothing_to_redo());
    case 4:
        return test_outcome_of("a change after typing is undone by itself",
                               test_a_change_after_typing_is_undone_by_itself());
    case 5:
        return test_outcome_of("typing after an undo is a change of its own",
                               test_typing_after_an_undo_is_a_change_of_its_own());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_design_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a ticked bonus counts once", test_a_ticked_bonus_counts_once());
    case 2:
        return test_outcome_of("a picked team bonus counts once", test_a_picked_team_bonus_counts_once());
    case 3:
        return test_outcome_of("a multiplier past ten is refused", test_a_multiplier_past_ten_is_refused());
    case 4:
        return test_outcome_of("a multiplier below one is refused", test_a_multiplier_below_one_is_refused());
    case 5:
        return test_outcome_of("a bonus ticked twice is not chosen", test_a_bonus_ticked_twice_is_not_chosen());
    case 6:
        return test_outcome_of("a node chosen twice is not chosen", test_a_node_chosen_twice_is_not_chosen());
    case 7:
        return test_outcome_of("taking a node out keeps the other nodes",
                               test_taking_a_node_out_keeps_the_other_nodes());
    case 8:
        return test_outcome_of("a tree takes no more nodes than it holds",
                               test_a_tree_takes_no_more_nodes_than_it_holds());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_saved_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a unit of the game is saved with no custom units index",
                               test_a_unit_of_the_game_is_saved_with_no_custom_units_index());
    case 2:
        return test_outcome_of("a custom unit is saved with no id of the game",
                               test_a_custom_unit_is_saved_with_no_id_of_the_game());
    case 3:
        return test_outcome_of("the language of the britons is saved as index 0",
                               test_the_language_of_the_britons_is_saved_as_index_0());
    default:
        return test_outcome_missing();
    }
}

TestOutcome model_suite_run(int index)
{
    if (index <= COMMAND_END) {
        return run_command_test(index);
    }
    if (index <= PICKS_END) {
        return run_picks_test(index - COMMAND_END);
    }
    if (index <= ROWS_TICK_END) {
        return run_rows_tick_test(index - PICKS_END);
    }
    if (index <= ROWS_PICK_END) {
        return run_rows_pick_test(index - ROWS_TICK_END);
    }
    if (index <= SUMMARY_END) {
        return run_summary_test(index - ROWS_PICK_END);
    }
    if (index <= REPORT_END) {
        return run_report_test(index - SUMMARY_END);
    }
    if (index <= STEP_MADE_END) {
        return run_step_made_test(index - REPORT_END);
    }
    if (index <= STEP_PICKED_END) {
        return run_step_picked_test(index - STEP_MADE_END);
    }
    if (index <= HISTORY_FIRST_END) {
        return run_history_first_test(index - STEP_PICKED_END);
    }
    if (index <= HISTORY_MORE_END) {
        return run_history_more_test(index - HISTORY_FIRST_END);
    }
    if (index <= DESIGN_END) {
        return run_design_test(index - HISTORY_MORE_END);
    }
    return run_saved_test(index - DESIGN_END);
}
