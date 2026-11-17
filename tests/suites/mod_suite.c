#include "suites.h"

#include "../mod/forge_tests.h"
#include "../mod/strings_tests.h"
#include "../places/places_tests.h"

enum {
    FORGE_MOD_END = 8,
    FORGE_REPORT_END = 13,
    STRINGS_ESCAPE_END = 17,
    STRINGS_LABEL_END = 20,
    STRINGS_TIP_END = 25
};

static TestOutcome run_forge_mod_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a built mod has a data folder named after the civ",
                               test_a_built_mod_has_a_data_folder_named_after_the_civ());
    case 2:
        return test_outcome_of("a built mod has a ui folder named after the civ",
                               test_a_built_mod_has_a_ui_folder_named_after_the_civ());
    case 3:
        return test_outcome_of("the data mod's info names the civ", test_the_data_mods_info_names_the_civ());
    case 4:
        return test_outcome_of("the ui mod's info says that it is the ui mod",
                               test_the_ui_mods_info_says_that_it_is_the_ui_mod());
    case 5:
        return test_outcome_of("the data mod holds the dat file with the civ's techs",
                               test_the_data_mod_holds_the_dat_file_with_the_civs_techs());
    case 6:
        return test_outcome_of("the data mod's list of civs names the civ's unique unit",
                               test_the_data_mods_list_of_civs_names_the_civs_unique_unit());
    case 7:
        return test_outcome_of("the data mod holds the tech tree file of the replaced civ",
                               test_the_data_mod_holds_the_tech_tree_file_of_the_replaced_civ());
    case 8:
        return test_outcome_of("the ui mod names the civ in english", test_the_ui_mod_names_the_civ_in_english());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_forge_report_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the ui mod is written for the last language of the game",
                               test_the_ui_mod_is_written_for_the_last_language_of_the_game());
    case 2:
        return test_outcome_of("a game folder without its list of civs is reported",
                               test_a_game_folder_without_its_list_of_civs_is_reported());
    case 3:
        return test_outcome_of("a data file that is not the game's is reported",
                               test_a_data_file_that_is_not_the_games_is_reported());
    case 4:
        return test_outcome_of("a game without the civ's tech tree file is reported",
                               test_a_game_without_the_civs_tech_tree_file_is_reported());
    case 5:
        return test_outcome_of("a civ with parts the catalog lacks is built and reported",
                               test_a_civ_with_parts_the_catalog_lacks_is_built_and_reported());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_strings_escape_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a line break in a text is written as an escape",
                               test_a_line_break_in_a_text_is_written_as_an_escape());
    case 2:
        return test_outcome_of("a carriage return before a line break is left out",
                               test_a_carriage_return_before_a_line_break_is_left_out());
    case 3:
        return test_outcome_of("a quote in a text is escaped", test_a_quote_in_a_text_is_escaped());
    case 4:
        return test_outcome_of("a quote that is escaped already is left as it is",
                               test_a_quote_that_is_escaped_already_is_left_as_it_is());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_strings_label_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a unique tech's label ends its description without a full stop",
                               test_a_unique_techs_label_ends_its_description_without_a_full_stop());
    case 2:
        return test_outcome_of("a unique tech without a description has a plain label",
                               test_a_unique_tech_without_a_description_has_a_plain_label());
    case 3:
        return test_outcome_of("a unit's tooltip carries its description",
                               test_a_units_tooltip_carries_its_description());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_strings_tip_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the first tip names the first civ bonus",
                               test_the_first_tip_names_the_first_civ_bonus());
    case 2:
        return test_outcome_of("the first tip names the team bonus of a civ without civ bonuses",
                               test_the_first_tip_names_the_team_bonus_of_a_civ_without_civ_bonuses());
    case 3:
        return test_outcome_of("the tip of a unique tech says where it is researched",
                               test_the_tip_of_a_unique_tech_says_where_it_is_researched());
    case 4:
        return test_outcome_of("the second tip names the unique unit", test_the_second_tip_names_the_unique_unit());
    case 5:
        return test_outcome_of("a line break in a tip is written as a space",
                               test_a_line_break_in_a_tip_is_written_as_a_space());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_places_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the game is found in the folder of steam",
                               test_the_game_is_found_in_the_folder_of_steam());
    case 2:
        return test_outcome_of("the game is found in a library that steam lists",
                               test_the_game_is_found_in_a_library_that_steam_lists());
    case 3:
        return test_outcome_of("the game is not found where no library holds it",
                               test_the_game_is_not_found_where_no_library_holds_it());
    case 4:
        return test_outcome_of("the local mods folder of a profile is found",
                               test_the_local_mods_folder_of_a_profile_is_found());
    case 5:
        return test_outcome_of("a home without a profile of the game has no mods folder",
                               test_a_home_without_a_profile_of_the_game_has_no_mods_folder());
    default:
        return test_outcome_missing();
    }
}

TestOutcome mod_suite_run(int index)
{
    if (index <= FORGE_MOD_END) {
        return run_forge_mod_test(index);
    }
    if (index <= FORGE_REPORT_END) {
        return run_forge_report_test(index - FORGE_MOD_END);
    }
    if (index <= STRINGS_ESCAPE_END) {
        return run_strings_escape_test(index - FORGE_REPORT_END);
    }
    if (index <= STRINGS_LABEL_END) {
        return run_strings_label_test(index - STRINGS_ESCAPE_END);
    }
    if (index <= STRINGS_TIP_END) {
        return run_strings_tip_test(index - STRINGS_LABEL_END);
    }
    return run_places_test(index - STRINGS_TIP_END);
}
